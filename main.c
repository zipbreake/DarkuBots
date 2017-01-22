/* Services -- main source file.
 * Copyright (c) 1996-1999 Andy Church <achurch@dragonfire.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING); if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * DarkuBots es una adaptación de Javier Fernández Viña, ZipBreake.
 * E-Mail: javier@jfv.es || Web: http://jfv.es/
 *
 */

#include "services.h"
#include "timeout.h"
#include "version.h"


/******** Global variables! ********/

/* Command-line options: (note that configuration variables are in config.c) */
char *services_dir = SERVICES_DIR;	/* -dir dirname */
char *log_filename = LOG_FILENAME;	/* -log filename */
int   debug        = 0;			/* -debug */
int   readonly     = 0;			/* -readonly */
int   skeleton     = 0;			/* -skeleton */
int   nofork       = 0;			/* -nofork */
int   forceload    = 0;			/* -forceload */

/* Set to 1 if we are to quit */
int quitting = 0;

/* Set to 1 if we are to quit after saving databases */
int delayed_quit = 0;

/* Contains a message as to why services is terminating */
char *quitmsg = NULL;

/* Input buffer - global, so we can dump it if something goes wrong */
char inbuf[BUFSIZE];

/* Socket for talking to server */
int servsock = -1;

/* Should we update the databases now? */
int save_data = 0;

/* At what time were we started? */
time_t start_time;


/******** Local variables! ********/

/* Set to 1 if we are waiting for input */
static int waiting = 0;

/* Set to 1 after we've set everything up */
static int started = 0;

/* If we get a signal, use this to jump out of the main loop. */
static jmp_buf panic_jmp;

/*************************************************************************/

/* If we get a weird signal, come here. */

void sighandler(int signum)
{
    if (started) {
	if (signum == SIGHUP) {  /* SIGHUP = save databases and restart */
	    save_data = -2;
	    signal(SIGHUP, SIG_IGN);
	    log("Recibido SIGHUP, reiniciando.");
	    if (!quitmsg)
		quitmsg = "Reiniciando por señal SIGHUP";
	    longjmp(panic_jmp, 1);
	} else if (signum == SIGTERM) {
	    save_data = 1;
	    delayed_quit = 1;
	    signal(SIGTERM, SIG_IGN);
	    signal(SIGHUP, SIG_IGN);
	    log("Recibido SIGTERM, saliendo.");
	    quitmsg = "Cierre conexion por señal SIGTERM";
	    longjmp(panic_jmp, 1);
	} else if (signum == SIGINT || signum == SIGQUIT) {
	    /* nothing -- terminate below */
	} else if (!waiting) {
	    log("PANIC! buffer = %s", inbuf);
	    /* Cut off if this would make IRC command >510 characters. */
	    if (strlen(inbuf) > 448) {
		inbuf[446] = '>';
		inbuf[447] = '>';
		inbuf[448] = 0;
	    }
	    canalopers(NULL, "PANIC! buffer = %s\r\n", inbuf);
	} else if (waiting < 0) {
	    /* This is static on the off-chance we run low on stack */
	    static char buf[BUFSIZE];
	    switch (waiting) {
		case  -1: snprintf(buf, sizeof(buf), "in timed_update");
		          break;
		case -11: snprintf(buf, sizeof(buf), "saving %s", NickDBName);
		          break;
		case -12: snprintf(buf, sizeof(buf), "saving %s", ChanDBName);
		          break;
		case -14: snprintf(buf, sizeof(buf), "saving %s", OperDBName);
		          break;
		case -15: snprintf(buf, sizeof(buf), "saving %s", AutokillDBName);
		          break;
		case -16: snprintf(buf, sizeof(buf), "saving %s", NewsDBName);
		          break;
		case -21: snprintf(buf, sizeof(buf), "expiring nicknames");
		          break;
		case -22: snprintf(buf, sizeof(buf), "expiring channels");
		          break;
		case -25: snprintf(buf, sizeof(buf), "expiring autokills");
		          break;
		default : snprintf(buf, sizeof(buf), "waiting=%d", waiting);
	    }
	    canalopers(NULL, "PANIC! %s (%s)", buf, strsignal(signum));
	    log("PANIC! %s (%s)", buf, strsignal(signum));
	}
    }
    if (signum == SIGUSR1 || !(quitmsg = malloc(BUFSIZE))) {
	quitmsg = "Out of memory!";
	quitting = 1;
    } else {
#if HAVE_STRSIGNAL
	snprintf(quitmsg, BUFSIZE, "Services ha terminado: %s", strsignal(signum));
#else
	snprintf(quitmsg, BUFSIZE, "Services ha terminado por señal %d", signum);
#endif
	quitting = 1;
    }
    if (started)
	longjmp(panic_jmp, 1);
    else {
	log("%s", quitmsg);
	if (isatty(2))
	    fprintf(stderr, "%s\n", quitmsg);
	exit(1);
    }
}

/*************************************************************************/

/* Main routine.  (What does it look like? :-) ) */

int main(int ac, char **av, char **envp)
{
    volatile time_t last_update; /* When did we last update the databases? */
    volatile time_t last_expire; /* When did we last expire nicks/channels? */
    volatile time_t last_check;  /* When did we last check timeouts? */
    int i;

    /* Initialization stuff. */
    if ((i = init(ac, av)) != 0)
	return i;


    /* We have a line left over from earlier, so process it first. */
    process();

    /* Set up timers. */
    last_update = time(NULL);
    last_expire = time(NULL);
    last_check  = time(NULL);

    /* The signal handler routine will drop back here with quitting != 0
     * if it gets called. */
    setjmp(panic_jmp);

    started = 1;


    /*** Main loop. ***/

    while (!quitting) {
	time_t t = time(NULL);

	if (debug >= 2)
	    log("debug: Top of main loop");
	if (!readonly && (save_data || t-last_expire >= ExpireTimeout)) {
	    waiting = -3;
	    if (debug)
		log("debug: Running expire routines");
//            canalopers(ServerName, "Ejecutando rutinas de expiracion");
	    if (!skeleton) {
		waiting = -21;
		expire_nicks();
		waiting = -22;
		expire_chans();
                waiting = -23;
                expire_creg();
	    }
	    waiting = -25;
	    expire_akills();
	    last_expire = t;
	}
	if (!readonly && (save_data || t-last_update >= UpdateTimeout)) {
	    waiting = -2;
	    if (debug)
		log("debug: Saving databases");
//	    canalopers(ServerName, "Grabando DB's");	
	    if (!skeleton) {
		waiting = -11;
		save_ns_dbase();
		waiting = -12;
		save_cs_dbase();
                waiting = -13;
                save_cr_dbase();
	    }
	    waiting = -14;
	    save_os_dbase();
	    waiting = -15;
	    save_akill();
	    waiting = -16;
	    save_news();
	    if (save_data < 0)
		break;	/* out of main loop */

	    save_data = 0;
	    last_update = t;
	}
	if (delayed_quit)
	    break;
	waiting = -1;
	if (t-last_check >= TimeoutCheck) {
	    check_timeouts();
	    last_check = t;
	}
	waiting = 1;
	i = (int)(long)sgets2(inbuf, sizeof(inbuf), servsock);
	waiting = 0;
	if (i > 0) {
	    process();
	} else if (i == 0) {
	    int errno_save = errno;
	    quitmsg = malloc(BUFSIZE);
	    if (quitmsg) {
		snprintf(quitmsg, BUFSIZE,
			"Read error from server: %s", strerror(errno_save));
	    } else {
		quitmsg = "Read error from server";
	    }
	    quitting = 1;
	}
	waiting = -4;
    }


    /* Check for restart instead of exit */
    if (save_data == -2) {
#ifdef SERVICES_BIN
	log("Reiniciando");
	if (!quitmsg)
	    quitmsg = "Reiniciando";
	send_cmd(ServerName, "SQUIT %s 0 :%s", ServerName, quitmsg);
	disconn(servsock);
	close_log();
	execve(SERVICES_BIN, av, envp);
	if (!readonly) {
	    open_log();
	    log_perror("Restart failed");
	    close_log();
	}
	return 1;
#else
	quitmsg = "Restart attempt failed--SERVICES_BIN not defined (rerun configure)";
#endif
    }

    /* Disconnect and exit */
    if (!quitmsg)
	quitmsg = "Services ha terminado, razón desconocida";
    log("%s", quitmsg);
    if (started)
        send_cmd(ServerName, "SQUIT %s 0 :%s", ServerName, quitmsg);
    disconn(servsock);
    return 0;
}

/*************************************************************************/
