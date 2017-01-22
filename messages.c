/* Definitions of IRC message functions and list of messages.
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
#include "extern.h"
#include "messages.h"
#include "language.h"

/* List of messages is at the bottom of the file. */

/*************************************************************************/

static void m_nickcoll(char *source, int ac, char **av)
{
    if (ac < 1)
	return;
    if (!skeleton && !readonly)
	introduce_user(av[0]);
}

/*************************************************************************/

static void m_away(char *source, int ac, char **av)
{
    User *u = finduser(source);

    if (u && (ac == 0 || *av[0] == 0))	/* un-away */
	check_memos(u);
}

/*************************************************************************/

#ifdef IRC_HISPANO
static void m_bmode(char *source, int ac, char **av)
/* Parseo de los BMODES de services virtual
 * zoltan 24-09-2000 */
{
    if (*av[0] == '#' || *av[0] == '&') {
        if (ac < 2)
            return;
        do_cmode(source, ac, av);
    }
}
#endif              
/*************************************************************************/
#ifdef IRC_UNDERNET_P10
static void m_burst(char *source, int ac, char **av)
{

     do_burst(source, ac, av);

}

/*************************************************************************/

static void m_create(char *source, int ac, char **av)
{

     do_create(source, ac, av);
     
}

/**************************************************************************/

static void m_end_of_burst(char *source, int ac, char **av)
{
     Server *server;
     server = find_servernumeric(source);
     if (!server)
         return;
     else if (stricmp(server->name, ServerHUB) == 0) 
         send_cmd(NULL, "%c EB", convert2y[ServerNumerico]);
     else
        return;
}

/**************************************************************************/

static void m_eob_ack(char *source, int ac, char **av)
{
    Server *server;  
    server = find_servernumeric(source); 
      
    if (!server)
        return;
    else if (stricmp(server->name, ServerHUB) == 0) 
        send_cmd(NULL, "%c EA", convert2y[ServerNumerico]);
    } else
        return; 
}
#endif /* IRC_UNDERNET_P10 */
/*************************************************************************/

static void m_info(char *source, int ac, char **av)
{
    int i;
    struct tm *tm;
    char timebuf[64];

    tm = localtime(&start_time);
    strftime(timebuf, sizeof(timebuf), "%a %b %d %H:%M:%S %Y %Z", tm);

    for (i = 0; info_text[i]; i++)
        send_cmd(ServerName, "371 %s :%s", source, info_text[i]);
    send_cmd(ServerName, "371 %s :DarkuBots-%s, %s", source, version_darku, version_build);
    send_cmd(ServerName, "371 %s :On-line since %s", source, timebuf);
    send_cmd(ServerName, "374 %s :End of /INFO list.", source);
}    
                                                            
/*************************************************************************/

static void m_join(char *source, int ac, char **av)
{
    if (ac != 1)
	return;
    do_join(source, ac, av);
}

/*************************************************************************/

static void m_kick(char *source, int ac, char **av)
{
    if (ac != 3)
	return;
    do_kick(source, ac, av);
}

/*************************************************************************/

static void m_kill(char *source, int ac, char **av)
{
    if (ac != 2)
	return;
    /* Recover if someone kills us. */
    if (stricmp(av[0], s_ChanServ) == 0) {
        introduce_user(av[0]);    
        join_chanserv();
    }
    if (stricmp(av[0], s_OperServ) == 0 ||
        stricmp(av[0], s_NickServ) == 0 ||
        stricmp(av[0], s_CregServ) == 0 ||
        stricmp(av[0], s_MemoServ) == 0 ||
        stricmp(av[0], s_HelpServ) == 0 ||
        stricmp(av[0], s_NewsServ) == 0 ||
        stricmp(av[0], s_GlobalNoticer) == 0
    ) {
	if (!readonly && !skeleton)
	    introduce_user(av[0]);	    
    } else {
	do_kill(source, ac, av);
    }
}

/*************************************************************************/

static void m_mode(char *source, int ac, char **av)
{
    if (*av[0] == '#' || *av[0] == '&') {
	if (ac < 2)
	    return;
	do_cmode(source, ac, av);
    } else {
	if (ac != 2)
	    return;
	do_umode(source, ac, av);
    }
}

/*************************************************************************/
static void m_motd(char *source, int ac, char **av)
{
    FILE *f;
    char buf[BUFSIZE];

    f = fopen(MOTDFilename, "r");
    send_cmd(ServerName, "375 %s :- %s Mensaje del día",
		source, ServerName);
   if (f) {
	while (fgets(buf, sizeof(buf), f)) {
	    buf[strlen(buf)-1] = 0;
	    send_cmd(ServerName, "372 %s :- %s", source, buf);
	}
	fclose(f);
    } else {
	send_cmd(ServerName, "372 %s :- Archivo MOTD no ha sido encontrado!"
			" Por favor contacta con tu IRC Administrador.", source);
    }

    /* Look, people.  I'm not asking for payment, praise, or anything like
     * that for using Services... is it too much to ask that I at least get
     * some recognition for my work?  Please don't remove the copyright
     * message below.
     */

    send_cmd(ServerName, "372 %s :-", source);
    send_cmd(ServerName, "372 %s :- Services is copyright (c) "
		"1996-1999 Andy Church.", source);
    send_cmd(ServerName, "372 %s :- 2000-2001, Toni García, Zoltan. Upworld ", source);
    send_cmd(ServerName, "327 %s :- 2002, David Martin, [x].", source);
    send_cmd(ServerName, "327 %s :- 2005, Javier Fdez. Viña, ZipBreake.", source);
    send_cmd(ServerName, "376 %s :End of /MOTD command.", source);
}

/*************************************************************************/

static void m_nick(char *source, int ac, char **av)
{
    /* ircu sends the server as the source for a NICK message for a new
     * user. */
    if (strchr(source, '.'))
	*source = 0;
#ifdef IRC_UNDERNET_P10
  /* En P10, el comando nick es mas largo de en otras redes :) */
    if ((ac != 8) && (ac != 9) && (ac != 2)) {
	if (debug) {
	    log("debug: NICK message: expecting 2, 8 or 9 parameters after "
	        "parsing; got %d, source=`%s'", ac, source);
	}
	return;
    }
#else
    if ((!*source && ac != 7) || (*source && ac != 2)) {
        if (debug) {
            log("debug: NICK message: expecting 2 or 7 parameters after "
                "parsing; got %d, source=%s'", ac, source);
        }
        return;
    }
#endif    
    do_nick(source, ac, av);
}

/*************************************************************************/

static void m_part(char *source, int ac, char **av)
{
    if (ac < 1 || ac > 2)
	return;
    do_part(source, ac, av);
}

/*************************************************************************/

static void m_ping(char *source, int ac, char **av)
{
    if (ac < 1)
        return;
    send_cmd(ServerName, "PONG %s %s", ac>1 ? av[1] : ServerName, av[0]);
}

/*************************************************************************/

static void m_count(char *source, int ac, char **av)
{
	char *cntbdd;
	int finalc;

	cntbdd = strtok(av[4],"S=");
	finalc = atoi(cntbdd);
	if (ac < 1)
	  return;

	if (stricmp(av[3],"'n'") == 0) 
       	   do_count_bdd(1,finalc);
           	
	if (stricmp(av[3],"'v'") == 0) 
	   do_count_bdd(2,finalc);

	if (stricmp(av[3],"'o'") == 0)
	   do_count_bdd(3,finalc);

	if (stricmp(av[3],"'w'") == 0)
	   do_count_bdd(4,finalc);

	if (stricmp(av[3],"'i'") == 0)
	   do_count_bdd(5,finalc);
}

/************************************************************************/

static void m_privmsg(char *source, int ac, char **av)
{
    time_t starttime, stoptime;	/* When processing started and finished */
    char *s;

    if (ac != 2)
	return;

    /* Check if we should ignore.  Operators always get through. */
    if (allow_ignore && !is_oper(source)) {
	IgnoreData *ign = get_ignore(source);
	if (ign && ign->time > time(NULL)) {
	    log("Ignored message from %s: \"%s\"", source, inbuf);
	    return;
	}
    }

    /* If a server is specified (nick@server format), make sure it matches
     * us, and strip it off. */
    s = strchr(av[0], '@');
    if (s) {
	*s++ = 0;
	if (stricmp(s, ServerName) != 0)
	    return;
    }
    starttime = time(NULL);

#ifdef IRC_UNDERNET_P10
    if (stricmp(av[0], s_OperServP10) == 0) {
#else
    if (stricmp(av[0], s_OperServ) == 0) {
#endif
	if (is_oper(source)) {
	    operserv(source, av[1]);
	} else {
	    User *u = finduser(source);
	    if (u)
		notice_lang(s_OperServ, u, ACCESS_DENIED);
	    else
		privmsg(s_OperServ, source, "4Acceso denegado.");
            canalopers(s_OperServ, "Denegando el acceso a 12%s desde %s (No es OPER)",
                        s_OperServ, source);
	}

    } else if (stricmp(av[0], s_NickServ) == 0) {
	nickserv(source, av[1]);
    } else if (stricmp(av[0], s_ChanServ) == 0) {
	chanserv(source, av[1]);
    } else if (stricmp(av[0], s_CregServ) == 0) {
	cregserv(source, av[1]);
    } else if (stricmp(av[0], s_MemoServ) == 0) {
	memoserv(source, av[1]);
    } else if (stricmp(av[0], s_HelpServ) == 0) {
	helpserv(s_HelpServ, source, av[1]);
    } else if (stricmp(av[0], s_BddServ) ==  0) {
	if (is_oper(source)) {
	        bddserv(source, av[1]);
	} else {
	    User *u = finduser(source);
	    if (u)
		notice_lang(s_BddServ, u, ACCESS_DENIED);
	    else
		privmsg(s_BddServ, source, "4Acceso denegado.");
            canalopers(s_BddServ, "Denegando el acceso a 12%s desde %s (No es OPER)",
                        s_BddServ, source);                                      
	}     
    }

    /* Add to ignore list if the command took a significant amount of time. */
    if (allow_ignore) {
	stoptime = time(NULL);
	if (stoptime > starttime && *source && !strchr(source, '.'))
	    add_ignore(source, stoptime-starttime);
    }
}

/*************************************************************************/

static void m_quit(char *source, int ac, char **av)
{
    if (ac != 1)
	return;
    do_quit(source, ac, av);
}

/*************************************************************************/

static void m_server(char *source, int ac, char **av)
{
    do_server(source, ac, av);
}

/*************************************************************************/

static void m_squit(char *source, int ac, char **av)
{
    if (ac != 3)
        return;
    do_squit(source, ac, av);
}   
             
/*************************************************************************/

static void m_stats(char *source, int ac, char **av)
{
    if (ac < 1)
	return;
    switch (*av[0]) {
      case 'u': {
	int uptime = time(NULL) - start_time;
	send_cmd(NULL, "242 %s :Services up %d day%s, %02d:%02d:%02d",
		source, uptime/86400, (uptime/86400 == 1) ? "" : "s",
		(uptime/3600) % 24, (uptime/60) % 60, uptime % 60);
	send_cmd(NULL, "250 %s :Current users: %d (%d ops); maximum %d",
		source, usercnt, opcnt, maxusercnt);
	send_cmd(NULL, "219 %s u :End of /STATS report.", source);
	break;
      } /* case 'u' */

      case 'l': {
	send_cmd(NULL, "211 %s Server SendBuf SentBytes SentMsgs RecvBuf "
		"RecvBytes RecvMsgs ConnTime", source);
	send_cmd(NULL, "211 %s %s %d %d %d %d %d %d %ld", source, RemoteServer,
		read_buffer_len(), total_read, -1,
		write_buffer_len(), total_written, -1,
		start_time);
	send_cmd(NULL, "219 %s l :End of /STATS report.", source);
	break;
      }
      /* Añado soporte para shadow */
      case 'x': {
        if (s_ShadowServ) {
            join_shadow();            
        }      	
        break;
      }   

      case 'c':
      case 'h':
      case 'i':
      case 'k':
      case 'm':
      case 'o':
      case 'y':
	send_cmd(NULL, "219 %s %c :/STATS %c not applicable or not supported.",
		source, *av[0], *av[0]);
	break;
    }
}

/*************************************************************************/

static void m_time(char *source, int ac, char **av)
{
    time_t t;
    struct tm *tm;
    char buf[64];

    time(&t);
    tm = localtime(&t);
    strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y %Z", tm);
    send_cmd(NULL, "391 :%s", buf);
}

/*************************************************************************/

static void m_topic(char *source, int ac, char **av)
{

    if (ac != 2)
	return;
    do_topic(source, ac, av);
}

/*************************************************************************/

static void m_user(char *source, int ac, char **av)
{
    /* Do nothing - we get everything we need from the NICK command. */
}

/*************************************************************************/

void m_version(char *source, int ac, char **av)
{
    if (source) {
	send_cmd(ServerName, "351 %s %s-%s ~ %s :-- %s", source, DNAME, version_darku, ServerName, version_build);
    }
}

/*************************************************************************/

void m_whois(char *source, int ac, char **av)
{
    const char *clientdesc;

    if (source && ac >= 1) {
	if (stricmp(av[0], s_NickServ) == 0)
	    clientdesc = desc_NickServ;
	else if (stricmp(av[0], s_ChanServ) == 0)
	    clientdesc = desc_ChanServ;
	else if (stricmp(av[0], s_CregServ) == 0)
	    clientdesc = desc_CregServ;
	else if (stricmp(av[0], s_MemoServ) == 0)
	    clientdesc = desc_MemoServ;
	else if (stricmp(av[0], s_HelpServ) == 0)
	    clientdesc = desc_HelpServ;
        else if (stricmp(av[0], s_GlobalNoticer) == 0)
            clientdesc = desc_GlobalNoticer;                    
	else if (stricmp(av[0], s_OperServ) == 0)
	    clientdesc = desc_OperServ;
	else if (stricmp(av[0], s_GlobalNoticer) == 0)
	    clientdesc = desc_GlobalNoticer;
	else {
	    send_cmd(ServerName, "401 %s %s :No such service.", source, av[0]);
	    return;
	}
	send_cmd(ServerName, "311 %s %s %s %s :%s", source, av[0],
		ServiceUser, ServiceHost, clientdesc);
	send_cmd(ServerName, "312 %s %s %s :%s", source, av[0],
		ServerName, ServerDesc);
	send_cmd(ServerName, "318 End of /WHOIS response.");
    }
}

/*************************************************************************/
/*************************************************************************/

Message messages[] = {
    { "436",       m_nickcoll },
    { "249",	   m_count },
    { "AWAY",      m_away },
    { "INFO",      m_info },
    { "JOIN",      m_join },
    { "KICK",      m_kick },
    { "KILL",      m_kill },
    { "MODE",      m_mode },
    { "MOTD",      m_motd },
    { "NICK",      m_nick },
    { "NOTICE",    NULL },
    { "PART",      m_part },
    { "PASS",      NULL },
    { "PING",      m_ping },
    { "PRIVMSG",   m_privmsg },
    { "QUIT",      m_quit },
    { "SERVER",    m_server },
    { "SQUIT",     m_squit }, 
    { "STATS",     m_stats }, 
    { "TIME",      m_time },
    { "TOPIC",     m_topic },
    { "USER",      m_user },
    { "VERSION",   m_version },
    { "WALLOPS",   NULL }, 
    { "WHOIS",     m_whois },
    { "GLINE",     NULL }, 

#ifdef IRC_UNDERNET_P10
    { "A",                m_away },
    { "J",                m_join },
    { "K",                m_kick },
    { "D",                m_kill }, 
    { "M",                m_mode },            
    { "MO",               m_motd },
    { "N",                m_nick },   
    { "O",                NULL },
    { "L",                m_part },
    { "PA",               NULL },
    { "SE",               NULL },   
    { "G",                m_ping }, 
    { "P",                m_privmsg },
    { "Q",                m_quit },
    { "S",                m_server },
    { "SQ",               m_squit },
    { "T",                m_topic }, 
    { "R",                m_stats },
    { "TI",               m_time },               
    { "V",                m_version },    
    { "WA",               NULL },
    { "W",                m_whois },
    { "GL",               NULL },
    { "END_OF_BURST",     m_end_of_burst },
    { "EB",               m_end_of_burst },
    { "EOB_ACK",          m_eob_ack },
    { "EA",               m_eob_ack },
    { "CREATE",           m_create },
    { "C",                m_create },
    { "B",                m_burst },
    { "BURST",            m_burst }, 
#endif

#ifdef IRC_HISPANO
    { "BMODE",     m_bmode }, 
    { "DB",        NULL }, 
    { "DBQ",       NULL },
    { "DBH",       NULL },        
    { "CONFIG",    NULL },
#endif
#ifdef IRC_TERRA
    { "SVSNICK",        NULL },
    { "SVSMODE",        NULL },
    { "SVSVHOST",       NULL },
    { "SAMODE",         m_mode },
    { "KEY",            NULL },                    
#endif
    { NULL }

};

/*************************************************************************/

Message *find_message(const char *name)
{
    Message *m;

    for (m = messages; m->name; m++) {
	if (stricmp(name, m->name) == 0)
	    return m;
    }
    return NULL;
}

/*************************************************************************/
