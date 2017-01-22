/* Initalization and related routines.
 *
 * Services is copyright (c) 1996-1999 Andy Church.
 *     E-mail: <achurch@dragonfire.net>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 *
 * DarkuBots es una adaptación de Javier Fernández Viña, ZipBreake.
 * E-Mail: javier@jfv.es || Web: http://jfv.es/
 *
 */

#include "services.h"

/*************************************************************************/

/* Send a NICK command for the given pseudo-client.  If `user' is NULL,
 * send NICK commands for all the pseudo-clients. */

#define NICK(nick,name) \
    do { \
        send_cmd(ServerName, "NICK %s 1 %ld %s %s %s :%s", (nick), time(NULL),\
                ServiceUser, ServiceHost, ServerName, (name)); \
    } while (0)

void introduce_user(const char *user)
{
    /* Watch out for infinite loops... */
#define LTSIZE 20
    static int lasttimes[LTSIZE];
    if (lasttimes[0] >= time(NULL)-3)
	fatal("introduce_user() loop detected");
    memmove(lasttimes, lasttimes+1, sizeof(lasttimes)-sizeof(int));
    lasttimes[LTSIZE-1] = time(NULL);
#undef LTSIZE

    if (!user || stricmp(user, s_ChanServ) == 0) {
	NICK(s_ChanServ, desc_ChanServ);
        send_cmd(s_ChanServ, "MODE %s +Bikdor", s_ChanServ);
    }
    if (!user || stricmp(user, s_CregServ) == 0) {
	NICK(s_CregServ, desc_CregServ);
        send_cmd(s_CregServ, "MODE %s +Bikdor", s_CregServ);
    }
    if (!user || stricmp(user, s_NickServ) == 0) {
        NICK(s_NickServ, desc_NickServ);
        send_cmd(s_NickServ, "MODE %s +kdBr", s_NickServ);
    }
    if (!user || stricmp(user, s_HelpServ) == 0) {
	NICK(s_HelpServ, desc_HelpServ);
        send_cmd(s_HelpServ, "MODE %s +dBrk", s_HelpServ);

    }
    if (!user || stricmp(user, s_MemoServ) == 0) {
	NICK(s_MemoServ, desc_MemoServ);
	send_cmd(s_MemoServ, "MODE %s +krdB", s_MemoServ);
    }
    if (!user || stricmp(user, s_OperServ) == 0) {
	NICK(s_OperServ, desc_OperServ);
        send_cmd(s_OperServ, "MODE %s +Bikdor", s_OperServ);
        send_cmd(s_OperServ, "JOIN #%s", CanalAdmins);
        send_cmd(s_ChanServ, "MODE #%s +o %s", CanalAdmins, s_OperServ);
        send_cmd(s_OperServ, "JOIN #%s", CanalOpers);
        send_cmd(s_ChanServ, "MODE #%s +o %s", CanalOpers, s_OperServ);
	send_cmd(s_OperServ, "MODE #%s +ntsO", CanalAdmins);
	send_cmd(s_OperServ, "MODE #%s +ntsO", CanalOpers);
    }
    if (!user || stricmp(user, s_BddServ) == 0) {
        NICK(s_BddServ, desc_BddServ);
	send_cmd(s_BddServ, "MODE %s +iXkoBrd", s_BddServ);
    }
    if (!user || stricmp(user, s_GlobalNoticer) == 0) {
	NICK(s_GlobalNoticer, desc_GlobalNoticer);
	send_cmd(s_GlobalNoticer, "MODE %s +ikorBd", s_GlobalNoticer);
    }
    if (!user || stricmp(user, s_ShadowServ) == 0) {
        NICK(s_ShadowServ, desc_ShadowServ);
	send_cmd(s_ShadowServ, "MODE %s +rokXBd", s_ShadowServ);
    }
    if (!user || stricmp(user, s_NewsServ) == 0) {
        NICK(s_NewsServ, desc_NewsServ);
        send_cmd(s_NewsServ, "MODE %s +kBbord", s_NewsServ);
    	send_cmd(NULL, "STATS b");
    }
}

#undef NICK

/*************************************************************************/

/* Set GID if necessary.  Return 0 if successful (or if RUNGROUP not
 * defined), else print an error message to logfile and return -1.
 */

static int set_group(void)
{
#if defined(RUNGROUP) && defined(HAVE_SETGRENT)
    struct group *gr;

    setgrent();
    while ((gr = getgrent()) != NULL) {
	if (strcmp(gr->gr_name, RUNGROUP) == 0)
	    break;
    }
    endgrent();
    if (gr) {
	setgid(gr->gr_gid);
	return 0;
    } else {
	log("Grupo desconocido `%s'\n", RUNGROUP);
	return -1;
    }
#else
    return 0;
#endif
}

/*************************************************************************/

/* Parse command-line options for the "-dir" option only.  Return 0 if all
 * went well or -1 for a syntax error.
 */

/* XXX this could fail if we have "-some-option-taking-an-argument -dir" */

static int parse_dir_options(int ac, char **av)
{
    int i;
    char *s;

    for (i = 1; i < ac; i++) {
	s = av[i];
	if (*s == '-') {
	    s++;
	    if (strcmp(s, "dir") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-dir requiere un parametro\n");
		    return -1;
		}
		services_dir = av[i];
	    }
	}
    }
    return 0;
}

/*************************************************************************/

/* Parse command-line options.  Return 0 if all went well, -1 for an error
 * with an option, or 1 for -help.
 */

static int parse_options(int ac, char **av)
{
    int i;
    char *s, *t;

    for (i = 1; i < ac; i++) {
	s = av[i];
	if (*s == '-') {
	    s++;
	    if (strcmp(s, "remote") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-remote requiere hostname[:puerto]\n");
		    return -1;
		}
		s = av[i];
		t = strchr(s, ':');
		if (t) {
		    *t++ = 0;
		    if (atoi(t) > 0)
			RemotePort = atoi(t);
		    else {
			fprintf(stderr, "-remote: el numero de puerto debe ser positivo. Usando el puerto de por defecto.");
			return -1;
		    }
		}
		RemoteServer = s;
	    } else if (strcmp(s, "local") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-local requiere hostname o [hostname]:[puerto]\n");
		    return -1;
		}
		s = av[i];
		t = strchr(s, ':');
		if (t) {
		    *t++ = 0;
		    if (atoi(t) >= 0)
			LocalPort = atoi(t);
		    else {
			fprintf(stderr, "-local: el numero de puerto debe ser positivo o 0. Usando el puerto de por defecto.");
			return -1;
		    }
		}
		LocalHost = s;
	    } else if (strcmp(s, "name") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-name requiere un parametro\n");
		    return -1;
		}
		ServerName = av[i];
	    } else if (strcmp(s, "desc") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-desc requiere un parametro\n");
		    return -1;
		}
		ServerDesc = av[i];
	    } else if (strcmp(s, "user") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-user requiere a parametro\n");
		    return -1;
		}
		ServiceUser = av[i];
	    } else if (strcmp(s, "host") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-host requiere a parametro\n");
		    return -1;
		}
		ServiceHost = av[i];
	    } else if (strcmp(s, "dir") == 0) {
		/* Handled by parse_dir_options() */
		i++;  /* Skip parameter */
	    } else if (strcmp(s, "log") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-log requiere un parametro\n");
		    return -1;
		}
		log_filename = av[i];
	    } else if (strcmp(s, "update") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-update requiere un parametro\n");
		    return -1;
		}
		s = av[i];
		if (atoi(s) <= 0) {
		    fprintf(stderr, "-update: el numero de segundos debe ser positivo");
		    return -1;
		} else
		    UpdateTimeout = atol(s);
	    } else if (strcmp(s, "expire") == 0) {
		if (++i >= ac) {
		    fprintf(stderr, "-expire requiere un parametro\n");
		    return -1;
		}
		s = av[i];
		if (atoi(s) <= 0) {
		    fprintf(stderr, "-expire: el numero de segundos debe ser positivo");
		    return -1;
		} else
		    ExpireTimeout = atol(s);
	    } else if (strcmp(s, "debug") == 0) {
		debug++;
	    } else if (strcmp(s, "readonly") == 0) {
		readonly = 1;
		skeleton = 0;
	    } else if (strcmp(s, "skeleton") == 0) {
		readonly = 0;
		skeleton = 1;
	    } else if (strcmp(s, "nofork") == 0) {
		nofork = 1;
	    } else if (strcmp(s, "forceload") == 0) {
		forceload = 1;
	    } else {
		fprintf(stderr, "Opcion desconocida -%s\n", s);
		return -1;
	    }
	} else {
	    fprintf(stderr, "Argumentos de opcion no permitidos\n");
	    return -1;
	}
    }
    return 0;
}

/*************************************************************************/

/* Remove our PID file.  Done at exit. */

static void remove_pidfile(void)
{
    remove(PIDFilename);
}

/*************************************************************************/

/* Create our PID file and write the PID to it. */

static void write_pidfile(void)
{
    FILE *pidfile;

    pidfile = fopen(PIDFilename, "w");
    if (pidfile) {
	fprintf(pidfile, "%d\n", (int)getpid());
	fclose(pidfile);
	atexit(remove_pidfile);
    } else {
	log_perror("ATENCION: No puedo abrir el fichero PID %s", PIDFilename);
    }
}

/*************************************************************************/

/* Overall initialization routine.  Returns 0 on success, -1 on failure. */

int init(int ac, char **av)
{
    int i;
    int openlog_failed = 0, openlog_errno = 0;
    int started_from_term = isatty(0) && isatty(1) && isatty(2);

    /* Imported from main.c */
    extern void sighandler(int signum);


    /* Set file creation mask and group ID. */
#if defined(DEFUMASK) && HAVE_UMASK
    umask(DEFUMASK);
#endif
    if (set_group() < 0)
	return -1;
    
    /* Parse command line for -dir option. */
    parse_dir_options(ac, av);

    /* Chdir to Services data directory. */
    if (chdir(services_dir) < 0) {
	fprintf(stderr, "chdir(%s): %s\n", services_dir, strerror(errno));
	return -1;
    }

    /* Open logfile, and complain if we didn't. */
    if (open_log() < 0) {
	openlog_errno = errno;
	if (started_from_term) {
	    fprintf(stderr, "ATENCION: No puedo abrir el archivo de log %s: %s\n",
			log_filename, strerror(errno));
	} else {
	    openlog_failed = 1;
	}
    }

    /* Read configuration file; exit if there are problems. */
    if (!read_config())
	return -1;

    /* Parse all remaining command-line options. */
    parse_options(ac, av);

    /* Detach ourselves if requested. */
    if (!nofork) {
	if ((i = fork()) < 0) {
	    perror("fork()");
	    return -1;
	} else if (i != 0) {
	    exit(0);
	}
	if (started_from_term) {
	    close(0);
	    close(1);
	    close(2);
	}
	if (setpgid(0, 0) < 0) {
	    perror("setpgid()");
	    return -1;
	}
    }

    /* Write our PID to the PID file. */
    write_pidfile();

    /* Announce ourselves to the logfile. */
    if (debug || readonly || skeleton) {
	log("Services %s (compilados para %s) iniciados (opciones:%s%s%s)",
		version_number, version_protocol,
		debug ? " debug" : "", readonly ? " readonly" : "",
		skeleton ? " skeleton" : "");
    } else {
	log("Services %s (compilados para %s) iniciados.",
		version_number, version_protocol);
    }
    start_time = time(NULL);

    /* If in read-only mode, close the logfile again. */
    if (readonly)
	close_log();

    /* Set signal handlers.  Catch certain signals to let us do things or
     * panic as necessary, and ignore all others.
     */
#ifdef NSIG
    for (i = 1; i <= NSIG; i++)
#else
    for (i = 1; i <= 32; i++)
#endif
	signal(i, SIG_IGN);

    signal(SIGINT, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGQUIT, sighandler);
    signal(SIGSEGV, sighandler);
    signal(SIGBUS, sighandler);
    signal(SIGQUIT, sighandler);
    signal(SIGHUP, sighandler);
    signal(SIGILL, sighandler);
    signal(SIGTRAP, sighandler);
#ifdef SIGIOT
    signal(SIGIOT, sighandler);
#endif
    signal(SIGFPE, sighandler);

    signal(SIGUSR1, sighandler);  /* This is our "out-of-memory" panic switch */

    /* Initialize multi-language support */
    lang_init();
    if (debug)
	log("debug: Cargando lenguajes");

    /* Initialiize subservices */
    ns_init();
    cs_init();
    cr_init();
    ms_init();
    os_init();

    /* Load up databases */
    if (!skeleton) {
	load_ns_dbase();
	if (debug)
	    log("debug: Cargando la DB de %s (1/7)", s_NickServ);
	load_cs_dbase();
	if (debug)
	    log("debug: Cargando la DB de %s (2/7)", s_ChanServ);
	load_cr_dbase();
	if (debug)
	    log("debug: Cargando la DB de %s (3/7)", s_CregServ);
    }
    load_os_dbase();
    if (debug)
	log("debug: Cargando la DB de %s (4/7)", s_OperServ);
    load_akill();
    if (debug)
	log("debug: Cargando la DB de GLINES (5/7)");
    load_news();
    if (debug)
	log("debug: Cargando la DB de NOTICIAS (6/7)");
    log("Cargadas las bases de datos");

    /* Connect to the remote server */
    servsock = conn(RemoteServer, RemotePort, LocalHost, LocalPort);
    if (servsock < 0)
	fatal_perror("No puedo conectar al servidor");
    send_cmd(NULL, "PASS :%s", RemotePassword);
#ifdef IRC_UNDERNET_P09
    send_cmd(NULL, "SERVER %s 1 %lu %lu P09 :%s",
             ServerName, start_time, start_time, ServerDesc);
#else /* IRC_UNDERNET_P10 */
    send_cmd(NULL, "SERVER %s %d 0 %ld J10 %cD] :%s",
             ServerName, 2, start_time, convert2y[ServerNumerico], ServerDesc); 
#endif
    sgets2(inbuf, sizeof(inbuf), servsock);
    if (strnicmp(inbuf, "ERROR", 5) == 0) {
	/* Close server socket first to stop wallops, since the other
	 * server doesn't want to listen to us anyway */
	disconn(servsock);
	servsock = -1;
	fatal("El servidor ha devuelto: %s", inbuf);
    }


    /* Announce a logfile error if there was one */
    if (openlog_failed) {
	canalopers(NULL, "4ATENCION: No puedo abrir el fichero de log: 12%s",
		strerror(openlog_errno));
    }

    /* Bring in our pseudo-clients */
    introduce_user(NULL);

    send_cmd(ServerName, "SETTIME %lu", time(NULL));
/* #if HAVE_ALLWILD_NOTICE
    send_cmd(s_OperServ, "NOTICE $*.%s :Establecidos los servicios de la RED.", NETWORK_DOMAIN);
    
#else
# ifdef NETWORK_DOMAIN
    send_cmd(s_OperServ, "NOTICE $*.%s :Establecidos los servicios de la RED.", NETWORK_DOMAIN);
# else
     Go through all common top-level domains.  If you have others,
     * add them here.
     
    send_cmd(s_OperServ, "NOTICE $*.es :Establecidos los servicios de la RED.");
    send_cmd(s_OperServ, "NOTICE $*.com :Establecidos los servicios de la RED.");
    send_cmd(s_OperServ, "NOTICE $*.net :Establecidos los servicios de la RED.");
    send_cmd(s_OperServ, "NOTICE $*.org :Establecidos los servicios de la RED.");    
    send_cmd(s_OperServ, "NOTICE $*.edu :Establecidos los servicios de la RED.");
# endif
#endif */
        
    join_chanserv();
      

    /* Success! */
    return 0;
}

/*************************************************************************/
