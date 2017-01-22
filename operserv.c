/* OperServ functions.
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
#include "pseudo.h"

/*************************************************************************/

/* Services admin list */
static NickInfo *services_admins[MAX_SERVADMINS];
/* Services coadmins list */
static NickInfo *services_coadmins[MAX_SERVADMINS];
/* Services operator list */
static NickInfo *services_opers[MAX_SERVOPERS];
/* Services preoperator list */
static NickInfo *services_preopers[MAX_SERVOPERS];
/* Services bots list */
static NickInfo *services_bots[MAX_SERVOPERS];
/* Services cregadmins list */
static NickInfo *services_cregadmins[MAX_SERVADMINS];

/*************************************************************************/

static void do_credits(User *u);
static void do_help(User *u);
static void do_global(User *u);
static void do_globaln(User *u);
static void do_stats(User *u);
static void do_admin(User *u);
static void do_coadmin(User *u);
static void do_cregadmin(User *u);
static void do_oper(User *u);
static void do_preoper(User *u);
static void do_bots(User *u);
static void do_os_op(User *u);
static void do_os_deop(User *u);
static void do_os_mode(User *u);
static void do_os_kick(User *u);
static void do_clearmodes(User *u);
static void do_apodera(User *u);
static void do_limpia(User *u);
static void do_block(User *u);
static void do_unblock(User *u);
static void do_set(User *u);
static void do_settime(User *u);
/* static void do_jupe(User *u); */
static void do_raw(User *u);
static void do_update(User *u);
static void do_os_quit(User *u);
static void do_shutdown(User *u);
static void do_restart(User *u);
static void do_listignore(User *u);
static void do_skill (User *u);
static void do_vhost (User *u);
static void do_vhost2 (User *u);
#ifdef DEBUG_COMMANDS
static void do_matchwild(User *u);
#endif

/*************************************************************************/

static Command cmds[] = {
    { "CREDITS",    do_credits,    NULL,  -1,                   -1,-1,-1,-1 },
    { "CREDITOS",   do_credits,    NULL,  -1,                   -1,-1,-1,-1 },        
    { "HELP",       do_help,       NULL,  -1,                   -1,-1,-1,-1 },
    { "AYUDA",      do_help,       NULL,  -1,                   -1,-1,-1,-1 },
    { "SHOWCOMMANDS",    do_help,  NULL,  -1,                   -1,-1,-1,-1 },
    { ":?",         do_help,       NULL,  -1,                   -1,-1,-1,-1 },
    { "?",          do_help,       NULL,  -1,                   -1,-1,-1,-1 },                
    { "GLOBAL",     do_global,     NULL,  OPER_HELP_GLOBAL,     -1,-1,-1,-1 },
    { "GLOBALN",    do_globaln,    NULL,  OPER_HELP_GLOBAL,     -1,-1,-1,-1 },
    { "STATS",      do_stats,      NULL,  OPER_HELP_STATS,      -1,-1,-1,-1 },
    { "UPTIME",     do_stats,      NULL,  OPER_HELP_STATS,      -1,-1,-1,-1 },
    { "SERVERS",    do_servers,    NULL,  -1,                   -1,-1,-1,-1 },

    /* Anyone can use the LIST option to the ADMIN and OPER commands; those
     * routines check privileges to ensure that only authorized users
     * modify the list. */
    { "ADMIN",      do_admin,      NULL,  OPER_HELP_ADMIN,      -1,-1,-1,-1 },
    { "COADMIN",    do_coadmin,    NULL,  OPER_HELP_ADMIN,      -1,-1,-1,-1 },
    { "CREGADMIN",  do_cregadmin,  NULL,  OPER_HELP_ADMIN,      -1,-1,-1,-1 },
    { "OPER",       do_oper,       NULL,  OPER_HELP_OPER,       -1,-1,-1,-1 },
    { "PREOPER",    do_preoper,    NULL,  OPER_HELP_OPER,       -1,-1,-1,-1 },
    { "BOTS",       do_bots,       NULL,  -1,       -1,-1,-1,-1 },
    /* Similarly, anyone can use *NEWS LIST, but *NEWS {ADD,DEL} are
     * reserved for Services admins. */
    { "LOGONNEWS",  do_logonnews,  NULL,  NEWS_HELP_LOGON,      -1,-1,-1,-1 },
    { "OPERNEWS",   do_opernews,   NULL,  NEWS_HELP_OPER,       -1,-1,-1,-1 },

    /* Commands for Services opers: */
    { "OP",         do_os_op,      is_services_oper,
        -1,-1,-1,-1,-1},
    { "DEOP",       do_os_deop,    is_services_oper,
        -1,-1,-1,-1,-1},        
    { "MODE",       do_os_mode,    is_services_oper,
	OPER_HELP_MODE, -1,-1,-1,-1 },
    { "CLEARMODES", do_clearmodes, is_services_oper,
	OPER_HELP_CLEARMODES, -1,-1,-1,-1 },
    { "KICK",       do_os_kick,    is_services_oper,
	OPER_HELP_KICK, -1,-1,-1,-1 },
    { "BLOCK",      do_block,      is_services_oper,
            -1,-1,-1,-1,-1},
    { "UNBLOCK",    do_unblock,    is_services_oper,
            -1,-1,-1,-1,-1},
    { "UNGLINE",    do_unblock,    is_services_oper,
            -1,-1,-1,-1,-1},
    { "LIMPIA",     do_limpia,     is_services_oper,
            -1,-1,-1,-1,-1},
    { "APODERA",    do_apodera,    is_services_oper,
            -1,-1,-1,-1,-1},
    { "KILL", 	    do_skill,	   is_services_oper,
            OPER_HELP_KILL,-1,-1,-1,-1},
    { "SETTIME",    do_settime,    is_services_oper,
            -1,-1,-1,-1,-1},
    { "GLINE",      do_akill,      is_services_oper,
        OPER_HELP_AKILL,-1,-1,-1,-1},                                                                                    	
    { "AKILL",      do_akill,      is_services_oper,
	OPER_HELP_AKILL, -1,-1,-1,-1 },

    /* Commands for Services admins: */

    { "IPVIRTUAL",  do_vhost,	   is_services_admin,   OPER_HELP_VHOST,   -1,-1,-1,-1 },
    { "IPVIRTUAL2", do_vhost2,	   is_services_admin,   OPER_HELP_VHOST,   -1,-1,-1,-1 },

    { "SET",        do_set,        is_services_admin,
	OPER_HELP_SET, -1,-1,-1,-1 },
    { "SET READONLY",0,0,  OPER_HELP_SET_READONLY, -1,-1,-1,-1 },
    { "SET DEBUG",0,0,     OPER_HELP_SET_DEBUG, -1,-1,-1,-1 },
 /*   { "JUPE",       do_jupe,       is_services_admin,
	OPER_HELP_JUPE, -1,-1,-1,-1 }, */
    { "RAW",        do_raw,        is_services_root,
	OPER_HELP_RAW, -1,-1,-1,-1 },
    { "UPDATE",     do_update,     is_services_admin,
	OPER_HELP_UPDATE, -1,-1,-1,-1 },
    { "QUIT",       do_os_quit,    is_services_admin,
	OPER_HELP_QUIT, -1,-1,-1,-1 },
    { "SHUTDOWN",   do_shutdown,   is_services_admin,
	OPER_HELP_SHUTDOWN, -1,-1,-1,-1 },
    { "RESTART",    do_restart,    is_services_admin,
	OPER_HELP_RESTART, -1,-1,-1,-1 },
    { "LISTIGNORE", do_listignore, is_services_admin,
	-1,-1,-1,-1, -1 },	

    /* Commands for Services root: */

    { "ROTATELOG",  rotate_log,  is_services_root, -1,-1,-1,-1,
	OPER_HELP_ROTATELOG },


#ifdef DEBUG_COMMANDS
    { "LISTCHANS",  send_channel_list,  is_services_root, -1,-1,-1,-1,-1 },
    { "LISTCHAN",   send_channel_users, is_services_root, -1,-1,-1,-1,-1 },
    { "LISTUSERS",  send_user_list,     is_services_root, -1,-1,-1,-1,-1 },
    { "LISTUSER",   send_user_info,     is_services_root, -1,-1,-1,-1,-1 },
    { "LISTTIMERS", send_timeout_list,  is_services_root, -1,-1,-1,-1,-1 },
    { "MATCHWILD",  do_matchwild,       is_services_root, -1,-1,-1,-1,-1 },
#endif

    /* Fencepost: */
    { NULL }
};

/*************************************************************************/
/*************************************************************************/

/* OperServ initialization. */

void os_init(void)
{
    Command *cmd;

    cmd = lookup_cmd(cmds, "GLOBAL");
    if (cmd)
	cmd->help_param1 = s_GlobalNoticer;
    cmd = lookup_cmd(cmds, "ADMIN");
    if (cmd)
	cmd->help_param1 = s_NickServ;
    cmd = lookup_cmd(cmds, "OPER");
    if (cmd)
	cmd->help_param1 = s_NickServ;
}

/*************************************************************************/

/* Main OperServ routine. */

void operserv(const char *source, char *buf)
{
    char *cmd;
    char *s;
    User *u = finduser(source);

    if (!u) {
	log("%s: user record for %s not found", s_OperServ, source);
	notice(s_OperServ, source,
		getstring((NickInfo *)NULL, USER_RECORD_NOT_FOUND));
	return;
    }

    log("%s: %s: %s", s_OperServ, source, buf);

    cmd = strtok(buf, " ");
    if (!cmd) {
	return;
    } else if (stricmp(cmd, "\1PING") == 0) {
	if (!(s = strtok(NULL, "")))
	    s = "\1";
	notice(s_OperServ, source, "\1PING %s", s);
    } else if (stricmp(cmd, "\1VERSION\1") == 0) {
        notice(s_OperServ, source, "\1VERSION %s %s -- %s\1",
               PNAME, s_OperServ, version_build);                
    } else {
	run_cmd(s_OperServ, u, cmds, cmd);
    }
}

/*************************************************************************/
/**************************** Privilege checks ***************************/
/*************************************************************************/

/* Load OperServ data. */

#define SAFE(x) do {					\
    if ((x) < 0) {					\
	if (!forceload)					\
	    fatal("Error de escritura en %s", OperDBName);	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void load_os_dbase(void)
{
    dbFILE *f;
    int16 i, n, ver;
    char *s;
    int failed = 0;

    if (!(f = open_db(s_OperServ, OperDBName, "r")))
	return;
    switch (ver = get_file_version(f)) {
      case 9:
      case 8:
	SAFE(read_int16(&n, f));
	for (i = 0; i < n && !failed; i++) {
	    SAFE(read_string(&s, f));
	    if (s && i < MAX_SERVADMINS)
		services_admins[i] = findnick(s);
	    if (s)
		free(s);
	}
	if (ver >= 9) {
	   SAFE(read_int16(&n, f));
	   for (i = 0; i < n && !failed; i++) {
	       SAFE(read_string(&s, f));
	       if (s && i < MAX_SERVADMINS)
	  	   services_coadmins[i] = findnick(s);
	       if (s)
	  	   free(s);
	   }
	   SAFE(read_int16(&n, f));
	   for (i = 0; i < n && !failed; i++) {
	       SAFE(read_string(&s, f));
	       if (s && i < MAX_SERVADMINS)
	  	   services_cregadmins[i] = findnick(s);
	       if (s)
	  	   free(s);
	   }
	}
	if (!failed)
	    SAFE(read_int16(&n, f));
	for (i = 0; i < n && !failed; i++) {
	    SAFE(read_string(&s, f));
	    if (s && i < MAX_SERVOPERS)
		services_opers[i] = findnick(s);
	    if (s)
		free(s);
	}
	if (ver >= 9) {
	   SAFE(read_int16(&n, f));
	   for (i = 0; i < n && !failed; i++) {
	       SAFE(read_string(&s, f));
	       if (s && i < MAX_SERVOPERS)
		   services_preopers[i] = findnick(s);
	       if (s)
	  	   free(s);
	   }
	   SAFE(read_int16(&n, f));
	   for (i = 0; i < n && !failed; i++) {
	       SAFE(read_string(&s, f));
	       if (s && i < MAX_SERVADMINS)
		   services_bots[i] = findnick(s);
	       if (s)
	  	   free(s);
	   }
	}
	int32 tmp32;
	SAFE(read_int32(&maxusercnt, f));
	SAFE(read_int32(&tmp32, f));
	maxusertime = tmp32;
	break;

      default:
	fatal("Version no soportada (%d) en %s", ver, OperDBName);
    } /* switch (version) */
    close_db(f);
}

#undef SAFE

/*************************************************************************/

/* Save OperServ data. */

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	restore_db(f);						\
	log_perror("Error de escritura en %s", OperDBName);		\
	if (time(NULL) - lastwarn > WarningTimeout) {		\
	    canalopers(NULL, "Error de escritura en %s: %s", OperDBName,	\
			strerror(errno));			\
	    lastwarn = time(NULL);				\
	}							\
	return;							\
    }								\
} while (0)

void save_os_dbase(void)
{
    dbFILE *f;
    int16 i, count = 0;
    static time_t lastwarn = 0;

    if (!(f = open_db(s_OperServ, OperDBName, "w")))
	return;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_admins[i])
	    count++;
    }
    SAFE(write_int16(count, f));
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_admins[i])
	    SAFE(write_string(services_admins[i]->nick, f));
    }
    count = 0;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_coadmins[i])
	    count++;
    }
    SAFE(write_int16(count, f));
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_coadmins[i])
	    SAFE(write_string(services_coadmins[i]->nick, f));
    }
    count = 0;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_cregadmins[i])
	    count++;
    }
    SAFE(write_int16(count, f));
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_cregadmins[i])
	    SAFE(write_string(services_cregadmins[i]->nick, f));
    }
    count = 0;
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_opers[i])
	    count++;
    }
    SAFE(write_int16(count, f));
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_opers[i])
	    SAFE(write_string(services_opers[i]->nick, f));
    }
    count = 0;
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_preopers[i])
	    count++;
    }
    SAFE(write_int16(count, f));
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_preopers[i])
	    SAFE(write_string(services_preopers[i]->nick, f));
    }
    count = 0;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_bots[i])
	    count++;
    }
    SAFE(write_int16(count, f));
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_bots[i])
	    SAFE(write_string(services_bots[i]->nick, f));
    }

    SAFE(write_int32(maxusercnt, f));
    SAFE(write_int32(maxusertime, f));
    close_db(f);
}

#undef SAFE

/*************************************************************************/

/* Does the given user have Services root privileges? */

int is_services_root(User *u)
{
    
    if (!(u->mode & UMODE_O) || stricmp(u->nick, ServicesRoot) != 0)
	return 0;
    if (skeleton || nick_identified(u))
	return 1;
    return 0;
}

/*************************************************************************/

/* Does the given user have Services Bot privileges? */

int is_services_bot(User *u)
{
    int i;

    if (is_services_root(u))
	return 1;
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_bots[i] && u->ni == getlink(services_bots[i])) {
	    if (nick_identified(u))
		return 1;
	    return 0;
	}
    }
    return 0;
}
/*************************************************************************/

/* Does the given user have Services admin privileges? */

int is_services_admin(User *u)
{
    int i;

    if (is_services_root(u))
	return 1;
    if (is_services_bot(u))
	return 1;
    if (skeleton)
	return 1;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_admins[i] && u->ni == getlink(services_admins[i])) {
	    if (nick_identified(u))
		return 1;
	    return 0;
	}
    }
    return 0;
}

/*************************************************************************/

/* Does the given user have Services coadmin privileges? */

int is_services_coadmin(User *u)
{
    int i;

    if (is_services_admin(u))
	return 1;
    if (skeleton)
	return 1;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_coadmins[i] && u->ni == getlink(services_coadmins[i])) {
	    if (nick_identified(u))
		return 1;
	    return 0;
	}
    }
    return 0;
}

/*************************************************************************/

/* Does the given user have Services cregadmin privileges? */

int is_services_cregadmin(User *u)
{
    int i;

    if (is_services_admin(u))
	return 1;
    if (skeleton)
	return 1;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_cregadmins[i] && u->ni == getlink(services_cregadmins[i])) {
	    if (nick_identified(u))
		return 1;
	    return 0;
	}
    }
    return 0;
}

/*************************************************************************/
/* Es este usuario un bot? */

int is_a_service(char *nick)
{
  if ((stricmp(nick, s_NickServ) == 0) || (stricmp(nick, s_ChanServ) == 0) || (stricmp(nick, s_CregServ) == 0) || (stricmp(nick, s_MemoServ) == 0) || (stricmp(nick, s_OperServ) == 0) || (stricmp(nick, s_ShadowServ) == 0) || (stricmp(nick, s_BddServ) == 0) || (stricmp(nick, s_HelpServ) == 0) || (stricmp(nick, s_GlobalNoticer) == 0) || (stricmp(nick, s_NewsServ) == 0))
	return 1;
  else
  	return 0;
}
 
/**************************************************************************/
/* Does the given user have Services oper privileges? */

int is_services_oper(User *u)
{
    int i;

    if (is_services_coadmin(u))
	return 1;
    if (skeleton)
	return 1;
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_opers[i] && u->ni == getlink(services_opers[i])) {
	    if (nick_identified(u))
		return 1;
	    return 0;
	}
    }
    return 0;
}

/**************************************************************************/
/* Does the given user have Services preoper privileges? */

int is_services_preoper(User *u)
{
    int i;

    if (is_services_oper(u))
	return 1;
    if (skeleton)
	return 1;
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_preopers[i] && u->ni == getlink(services_preopers[i])) {
	    if (nick_identified(u))
		return 1;
	    return 0;
	}
    }
    return 0;
}

/*************************************************************************/

/* Listar los PREOPERS/OPERS/COADMINS/ADMINS on-line */

void ircops(User *u)
{
    int i;
    int online = 0;
    NickInfo *ni, *ni2;
    
    for (i = 0; i < MAX_SERVOPERS; i++) {
         if (services_opers[i]) {
             ni = findnick(services_opers[i]->nick);
             if (ni && (ni->status & NS_IDENTIFIED)) {
#ifdef IRC_UNDERNET_P10
                 privmsg(s_ChanServ, u->numerico, "%-10s es 12OPER de 4%s y 4%s",
#else
                 privmsg(s_ChanServ, u->nick, "%-10s es 12OPER de 4%s y 4%s",
#endif
                     services_opers[i]->nick, s_NickServ, s_ChanServ);
                 online++;
              }
         }     
    }

    for (i = 0; i < MAX_SERVOPERS; i++) {
         if (services_preopers[i]) {
             ni = findnick(services_preopers[i]->nick);
             if (ni && (ni->status & NS_IDENTIFIED)) {
#ifdef IRC_UNDERNET_P10
                 privmsg(s_ChanServ, u->numerico, "%-10s es 12PREOPER de 4%s y 4%s",
#else
                 privmsg(s_ChanServ, u->nick, "%-10s es 12PREOPER de 4%s y 4%s",
#endif
                     services_opers[i]->nick, s_NickServ, s_ChanServ);
                 online++;
              }
         }     
    }
                                                                                      
    for (i = 0; i < MAX_SERVADMINS; i++) {
         if (services_coadmins[i]) {
             ni2 = findnick(services_coadmins[i]->nick);
             if (ni2 && (ni2->status & NS_IDENTIFIED)) {             
#ifdef IRC_UNDERNET_P10
                 privmsg(s_ChanServ, u->numerico, "%-10s es 12COADMIN de 4%s y 4%s",
#else
                 privmsg(s_ChanServ, u->nick, "%-10s es 12COADMIN de 4%s y 4%s",
#endif
                     services_admins[i]->nick, s_NickServ, s_ChanServ);
                 online++;
             }
         }    
    }

    for (i = 0; i < MAX_SERVADMINS; i++) {
         if (services_admins[i]) {
             ni2 = findnick(services_admins[i]->nick);
             if (ni2 && (ni2->status & NS_IDENTIFIED)) {             
#ifdef IRC_UNDERNET_P10
                 privmsg(s_ChanServ, u->numerico, "%-10s es 12ADMIN de 4%s y 4%s",
#else
                 privmsg(s_ChanServ, u->nick, "%-10s es 12ADMIN de 4%s y 4%s",
#endif
                     services_admins[i]->nick, s_NickServ, s_ChanServ);
                 online++;
             }
         }    
    }
   
#ifdef IRC_UNDERNET_P10
    privmsg(s_ChanServ, u->numerico, "12%d REPRESENTANTES on-line", online);
#else
    privmsg(s_ChanServ, u->nick, "12%d REPRESENTANTES on-line", online);
#endif    
}
            

/*************************************************************************/

/* Is the given nick a Services admin/root nick? */

/* NOTE: Do not use this to check if a user who is online is a services admin
 * or root. This function only checks if a user has the ABILITY to be a 
 * services admin. Rather use is_services_admin(User *u). -TheShadow */

int nick_is_services_admin(NickInfo *ni)
{
    int i;

    if (!ni)
	return 0;
	
    if (stricmp(ni->nick, ServicesRoot) == 0)
	return 1;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_admins[i] && getlink(ni) == getlink(services_admins[i]))
	    return 1;
    }
    return 0;
}

int nick_is_services_coadmin(NickInfo *ni)
{
    int i;

    if (!ni)
	return 0;
	
   if (stricmp(ni->nick, ServicesRoot) == 0)
	return 1;
   if (nick_is_services_admin(ni))
       return 1;
    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_coadmins[i] && getlink(ni) == getlink(services_coadmins[i]))
	    return 1;
    }
    return 0;
}

/*************************************************************************/

/* El nick es Oper de los Services */

int nick_is_services_oper(NickInfo *ni)
{
   int i;
   
   if (!ni)
       return 0;
   if (stricmp(ni->nick, ServicesRoot) == 0)
       return 1;
   if (nick_is_services_coadmin(ni))
       return 1;
   for (i = 0; i < MAX_SERVOPERS; i++) {
       if (services_opers[i] && getlink(ni) == getlink(services_opers[i]))
       return 1;
   }
   return 0;
}

int nick_is_services_preoper(NickInfo *ni)
{
   int i;
   
   if (!ni)
       return 0;
   if (stricmp(ni->nick, ServicesRoot) == 0)
       return 1;
   if (nick_is_services_oper(ni))
       return 1;
   for (i = 0; i < MAX_SERVOPERS; i++) {
       if (services_preopers[i] && getlink(ni) == getlink(services_preopers[i]))
       return 1;
   }
   return 0;
}
                                                         

/*************************************************************************/


/* Expunge a deleted nick from the Services admin/oper lists. */

void os_remove_nick(const NickInfo *ni)
{
    int i;

    for (i = 0; i < MAX_SERVADMINS; i++) {
	if (services_admins[i] == ni)
	    services_admins[i] = NULL;
    }
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_opers[i] == ni)
	    services_opers[i] = NULL;
    }
}

/*************************************************************************/
/*********************** OperServ command functions **********************/
/*************************************************************************/
static void do_credits(User *u)
{
    notice_lang(s_OperServ, u, SERVICES_CREDITS);
}
    
/*************************************************************************/
    

/* HELP command. */

static void do_help(User *u)
{
    const char *cmd = strtok(NULL, "");

    if (!cmd) {
	notice_help(s_OperServ, u, OPER_HELP);
    } else {
	help_cmd(s_OperServ, u, cmds, cmd);
    }
}
/*************************************************************************/

/* Global privmsg sending via GlobalNoticer. */

static void do_global(User *u)
{
    char *msg = strtok(NULL, "");
    
    if (!msg) {
        syntax_error(s_OperServ, u, "GLOBAL", OPER_GLOBAL_SYNTAX);
        return;
    }
#if HAVE_ALLWILD_NOTICE
    send_cmd(s_GlobalNoticer, "PRIVMSG $*.%s :%s", NETWORK_DOMAIN, msg);
                            
#else
# ifdef NETWORK_DOMAIN
    send_cmd(s_GlobalNoticer, "PRIVMSG $*.%s :%s", NETWORK_DOMAIN, msg);
# else
    /* Go through all common top-level domains.  If you have others,
     * add them here.
     */

    send_cmd(s_GlobalNoticer, "PRIVMSG $*.es :%s", msg);
    send_cmd(s_GlobalNoticer, "PRIVMSG $*.com :%s", msg);
    send_cmd(s_GlobalNoticer, "PRIVMSG $*.net :%s", msg);
    send_cmd(s_GlobalNoticer, "PRIVMSG $*.org :%s", msg);
    send_cmd(s_GlobalNoticer, "PRIVMSG $*.edu :%s", msg);
                    
# endif
#endif
 /*   canalopers(s_GlobalNoticer, "12%s ha enviado el GLOBAL (%s)", u->nick, msg); */
}
                                          
/*************************************************************************/

/* Global notice sending via GlobalNoticer. */

static void do_globaln(User *u)
{
    char *msg = strtok(NULL, "");

    if (!msg) {
	syntax_error(s_OperServ, u, "GLOBAL", OPER_GLOBAL_SYNTAX);
	return;
    }
#if HAVE_ALLWILD_NOTICE
    send_cmd(s_GlobalNoticer, "NOTICE $*.%s :%s", NETWORK_DOMAIN, msg);
    
#else
# ifdef NETWORK_DOMAIN
    send_cmd(s_GlobalNoticer, "NOTICE $*.%s :%s", NETWORK_DOMAIN, msg);
# else
    /* Go through all common top-level domains.  If you have others,
     * add them here.
     */
     
    send_cmd(s_GlobalNoticer, "NOTICE $*.es :%s", msg);
    send_cmd(s_GlobalNoticer, "NOTICE $*.com :%s", msg);
    send_cmd(s_GlobalNoticer, "NOTICE $*.net :%s", msg);
    send_cmd(s_GlobalNoticer, "NOTICE $*.org :%s", msg);
    send_cmd(s_GlobalNoticer, "NOTICE $*.edu :%s", msg);
                
# endif
#endif
    /* canalopers(s_GlobalNoticer, "12%s ha enviado el GLOBALN (%s)", u->nick, msg); */
}

/*************************************************************************/

/* STATS command. */

static void do_stats(User *u)
{
    time_t uptime = time(NULL) - start_time;
    char *extra = strtok(NULL, "");
    int days = uptime/86400, hours = (uptime/3600)%24,
        mins = (uptime/60)%60, secs = uptime%60;
    struct tm *tm;
    char timebuf[64];

    if (extra && stricmp(extra, "ALL") != 0) {
	if (stricmp(extra, "AKILL") == 0) {
	    int timeout = AutokillExpiry+59;
	    notice_lang(s_OperServ, u, OPER_STATS_AKILL_COUNT, num_akills());
	    if (timeout >= 172800)
		notice_lang(s_OperServ, u, OPER_STATS_AKILL_EXPIRE_DAYS,
			timeout/86400);
	    else if (timeout >= 86400)
		notice_lang(s_OperServ, u, OPER_STATS_AKILL_EXPIRE_DAY);
	    else if (timeout >= 7200)
		notice_lang(s_OperServ, u, OPER_STATS_AKILL_EXPIRE_HOURS,
			timeout/3600);
	    else if (timeout >= 3600)
		notice_lang(s_OperServ, u, OPER_STATS_AKILL_EXPIRE_HOUR);
	    else if (timeout >= 120)
		notice_lang(s_OperServ, u, OPER_STATS_AKILL_EXPIRE_MINS,
			timeout/60);
	    else if (timeout >= 60)
		notice_lang(s_OperServ, u, OPER_STATS_AKILL_EXPIRE_MIN);
	    else
		notice_lang(s_OperServ, u, OPER_STATS_AKILL_EXPIRE_NONE);
	    return;
	} else {
	    notice_lang(s_OperServ, u, OPER_STATS_UNKNOWN_OPTION,
			strupper(extra));
	}
    }

    notice_lang(s_OperServ, u, OPER_STATS_CURRENT_USERS, usercnt, opcnt);
    tm = localtime(&maxusertime);
    strftime_lang(timebuf, sizeof(timebuf), u, STRFTIME_DATE_TIME_FORMAT, tm);
    notice_lang(s_OperServ, u, OPER_STATS_MAX_USERS, maxusercnt, timebuf);
    if (days > 1) {
	notice_lang(s_OperServ, u, OPER_STATS_UPTIME_DHMS,
		days, hours, mins, secs);
    } else if (days == 1) {
	notice_lang(s_OperServ, u, OPER_STATS_UPTIME_1DHMS,
		days, hours, mins, secs);
    } else {
	if (hours > 1) {
	    if (mins != 1) {
		if (secs != 1) {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_HMS,
				hours, mins, secs);
		} else {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_HM1S,
				hours, mins, secs);
		}
	    } else {
		if (secs != 1) {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_H1MS,
				hours, mins, secs);
		} else {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_H1M1S,
				hours, mins, secs);
		}
	    }
	} else if (hours == 1) {
	    if (mins != 1) {
		if (secs != 1) {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_1HMS,
				hours, mins, secs);
		} else {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_1HM1S,
				hours, mins, secs);
		}
	    } else {
		if (secs != 1) {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_1H1MS,
				hours, mins, secs);
		} else {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_1H1M1S,
				hours, mins, secs);
		}
	    }
	} else {
	    if (mins != 1) {
		if (secs != 1) {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_MS,
				mins, secs);
		} else {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_M1S,
				mins, secs);
		}
	    } else {
		if (secs != 1) {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_1MS,
				mins, secs);
		} else {
		    notice_lang(s_OperServ, u, OPER_STATS_UPTIME_1M1S,
				mins, secs);
		}
	    }
	}
    }

    if (extra && stricmp(extra, "ALL") == 0 && is_services_admin(u)) {
	long count, mem, count2, mem2;

	notice_lang(s_OperServ, u, OPER_STATS_BYTES_READ, total_read / 1024);
	notice_lang(s_OperServ, u, OPER_STATS_BYTES_WRITTEN, 
			total_written / 1024);
        get_server_stats(&count, &mem);
        notice_lang(s_OperServ, u, OPER_STATS_SERVER_MEM,
                        count, (mem+512) / 1024);
	get_user_stats(&count, &mem);
	notice_lang(s_OperServ, u, OPER_STATS_USER_MEM,
			count, (mem+512) / 1024);
	get_channel_stats(&count, &mem);
	notice_lang(s_OperServ, u, OPER_STATS_CHANNEL_MEM,
			count, (mem+512) / 1024);
	get_nickserv_stats(&count, &mem);
	notice_lang(s_OperServ, u, OPER_STATS_NICKSERV_MEM,
			count, (mem+512) / 1024);
	get_chanserv_stats(&count, &mem);
	notice_lang(s_OperServ, u, OPER_STATS_CHANSERV_MEM,
			count, (mem+512) / 1024);
	count = 0;
	get_akill_stats(&count2, &mem2);
	count += count2;
	mem += mem2;
	get_news_stats(&count2, &mem2);
	count += count2;
	mem += mem2;
	notice_lang(s_OperServ, u, OPER_STATS_OPERSERV_MEM,
			count, (mem+512) / 1024);

    }
}
/*************************************************************************/

/* Op en un canal a traves del servidor */

static void do_os_op(User *u)
{
    char *chan = strtok(NULL, " ");
    char *op_params = strtok(NULL, " ");
    char *argv[3];
    
    Channel *c;

    if (!chan || !op_params) {
        syntax_error(s_OperServ, u, "OP", CHAN_OP_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
    } else {
        char *destino;
        User *u2 =finduser(op_params);
               
        if (u2) {
#ifdef IRC_UNDERNET_P10
            destino = u2->numerico;
#else
            destino = u2->nick;
#endif                                    
            send_cmd(ServerName, "MODE %s +o %s", chan, destino); 

            argv[0] = sstrdup(chan);
            argv[1] = sstrdup("+o");
            argv[2] = sstrdup(destino);
            do_cmode(s_OperServ, 3, argv);
            free(argv[2]);
            free(argv[1]);
            free(argv[0]);
        } else
            notice_lang(s_OperServ, u, NICK_X_NOT_IN_USE, op_params);                                                                                                            
    }
}
                                                 
/*************************************************************************/

/* deop en un canal a traves de server */

static void do_os_deop(User *u)
{
    char *chan = strtok(NULL, " ");
    char *deop_params = strtok(NULL, " ");
    char *argv[3];
    
    Channel *c;

    if (!chan || !deop_params) {
        syntax_error(s_OperServ, u, "DEOP", CHAN_DEOP_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
    } else {
        char *destino;
        User *u2 =finduser(deop_params);
        
        if (u2) {
#ifdef IRC_UNDERNET_P10
            destino = u2->numerico;
#else
            destino = u2->nick;
#endif
                            
            send_cmd(ServerName, "MODE %s -o %s", chan, destino);
                                      
            argv[0] = sstrdup(chan);
            argv[1] = sstrdup("-o");
            argv[2] = sstrdup(destino);
            do_cmode(s_OperServ, 3, argv);
            free(argv[2]);
            free(argv[1]);
            free(argv[0]);
        } else        
            notice_lang(s_OperServ, u, NICK_X_NOT_IN_USE, deop_params);    
    }        
}
                                                

/*************************************************************************/

/* Channel mode changing (MODE command). */

static void do_os_mode(User *u)
{
    int argc;
    char **argv;
    char *s = strtok(NULL, "");
    char *chan, *modes;
    Channel *c;

    if (!s) {
	syntax_error(s_OperServ, u, "MODE", OPER_MODE_SYNTAX);
	return;
    }
    chan = s;
    s += strcspn(s, " ");
    if (!*s) {
	syntax_error(s_OperServ, u, "MODE", OPER_MODE_SYNTAX);
	return;
    }
    *s = 0;
    modes = (s+1) + strspn(s+1, " ");
    if (!*modes) {
	syntax_error(s_OperServ, u, "MODE", OPER_MODE_SYNTAX);
	return;
    }
    if (!(c = findchan(chan))) {
	notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (c->bouncy_modes) {
#if defined(IRC_DALNET) || defined(IRC_UNDERNET)
	notice_lang(s_OperServ, u, OPER_BOUNCY_MODES_U_LINE);
#else
	notice_lang(s_OperServ, u, OPER_BOUNCY_MODES);
#endif
	return;
    } else {
	send_cmd(ServerName, "MODE %s %s", chan, modes);
	canalopers(s_OperServ, "12%s ha usado MODE %s en 12%s", u->nick, modes, chan); 
	*s = ' ';
	argc = split_buf(chan, &argv, 1);
	do_cmode(s_OperServ, argc, argv);
    }
}

/*************************************************************************/

/* Clear all modes from a channel. */

static void do_clearmodes(User *u)
{
    char *s;
    int i;
    char *argv[3];
    char *chan = strtok(NULL, " ");
    Channel *c;
    int all = 0;
    int count;		/* For saving ban info */
    char **bans;	/* For saving ban info */
    struct c_userlist *cu, *next;

    if (!chan) {
	syntax_error(s_OperServ, u, "CLEARMODES", OPER_CLEARMODES_SYNTAX);
    } else if (!(c = findchan(chan))) {
	notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (c->bouncy_modes) {
#if defined(IRC_DALNET) || defined(IRC_UNDERNET)
	notice_lang(s_OperServ, u, OPER_BOUNCY_MODES_U_LINE);
#else
	notice_lang(s_OperServ, u, OPER_BOUNCY_MODES);
#endif
	return;
    } else {
	s = strtok(NULL, " ");
	if (s) {
	    if (strcmp(s, "ALL") == 0) {
		all = 1;
	    } else {
		syntax_error(s_OperServ,u,"CLEARMODES",OPER_CLEARMODES_SYNTAX);
		return;
	    }
           }
        canalopers(s_OperServ, "12%s ha usado CLEARMODES %s en 12%s",
                      u->nick, all ? " ALL" : "", chan);
                        
	if (all) {
	    /* Clear mode +o */
	    for (cu = c->chanops; cu; cu = next) {
		next = cu->next;
		argv[0] = sstrdup(chan);
		argv[1] = sstrdup("-o");

#ifdef IRC_UNDERNET_P10
		argv[2] = sstrdup(cu->user->numerico);
#else
                argv[2] = sstrdup(cu->user->nick);
#endif		
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
			argv[0], argv[1], argv[2]);
		do_cmode(s_ChanServ, 3, argv);
		free(argv[2]);
		free(argv[1]);
		free(argv[0]);
	    }

	    /* Clear mode +v */
	    for (cu = c->voices; cu; cu = next) {
		next = cu->next;
		argv[0] = sstrdup(chan);
		argv[1] = sstrdup("-v");
#ifdef IRC_UNDERNET_P10		
		argv[2] = sstrdup(cu->user->numerico);
#else
                argv[2] = sstrdup(cu->user->nick);
#endif		
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
			argv[0], argv[1], argv[2]);
		do_cmode(s_ChanServ, 3, argv);
		free(argv[2]);
		free(argv[1]);
		free(argv[0]);
	    }
	}

	/* Clear modes */
	send_cmd(MODE_SENDER(s_OperServ), "MODE %s -iklmnpstMR :%s",
		chan, c->key ? c->key : "");
	argv[0] = sstrdup(chan);
	argv[1] = sstrdup("-iklmnpstMR");
	argv[2] = c->key ? c->key : sstrdup("");
	do_cmode(s_OperServ, 2, argv);
	free(argv[0]);
	free(argv[1]);
	free(argv[2]);
	c->key = NULL;
	c->limit = 0;

	/* Clear bans */
	count = c->bancount;
	bans = smalloc(sizeof(char *) * count);
	for (i = 0; i < count; i++)
	    bans[i] = sstrdup(c->bans[i]);
	for (i = 0; i < count; i++) {
	    argv[0] = sstrdup(chan);
	    argv[1] = sstrdup("-b");
	    argv[2] = bans[i];
	    send_cmd(MODE_SENDER(s_OperServ), "MODE %s %s :%s",
			argv[0], argv[1], argv[2]);
	    do_cmode(s_OperServ, 3, argv);
	    free(argv[2]);
	    free(argv[1]);
	    free(argv[0]);
	}
	free(bans);
    }
}

/*************************************************************************/

/* Kick a user from a channel (KICK command). */

static void do_os_kick(User *u)
{
    char *argv[3];
    char *chan, *nick, *s, *destino;
    User *u2;
    Channel *c;

    chan = strtok(NULL, " ");
    nick = strtok(NULL, " ");
    s = strtok(NULL, "");
    if (!chan || !nick || !s) {
	syntax_error(s_OperServ, u, "KICK", OPER_KICK_SYNTAX);
	return;
    }
    if (!(c = findchan(chan))) {
	notice_lang(s_OperServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (c->bouncy_modes) {
#if defined(IRC_DALNET) || defined(IRC_UNDERNET)
	notice_lang(s_OperServ, u, OPER_BOUNCY_MODES_U_LINE);
#else
	notice_lang(s_OperServ, u, OPER_BOUNCY_MODES);
#endif
	return;
    }
    u2=finduser(nick);
    
    if (u2) {
#ifdef IRC_UNDERNET_P10
        destino = u2->numerico;
#else
        destino = u2->nick;
#endif    
    
        send_cmd(ServerName, "KICK %s %s :%s (%s)", chan, destino, u->nick, s);
        canalopers(s_OperServ, "12%s ha usado KICK a 12%s en 12%s", u->nick, nick, chan);

        argv[0] = sstrdup(chan);
        argv[1] = sstrdup(destino);
        argv[2] = sstrdup(s);
        do_kick(s_OperServ, 3, argv);
        free(argv[2]);
        free(argv[1]);
        free(argv[0]);
    } else
        notice_lang(s_OperServ, u, NICK_X_NOT_IN_USE, nick);                       
}

/***************************************************************************/

/* Quitar los modos y lo silencia */

static void do_apodera(User *u)
{
    char *chan = strtok(NULL, " ");

    Channel *c;

    if (!chan) {
         privmsg(s_OperServ, u->nick, "Sintaxis: 12APODERA <canal>");
         return;
    } else if (!(c = findchan(chan))) {
         privmsg(s_OperServ, u->nick, "El canal 12%s esta vacio.", chan);
    } else {
         char *av[2];
         struct c_userlist *cu, *next;

         send_cmd(s_ShadowServ, "JOIN %s", chan);
         send_cmd(ServerName, "MODE %s +o %s", chan, s_ShadowServ);
         send_cmd(s_ShadowServ, "MODE %s :+tnsim", chan);

         for (cu = c->users; cu; cu = next) {
              next = cu->next;
              av[0] = sstrdup(chan);
              av[1] = sstrdup(cu->user->nick);
              send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", av[0], av[1]);
              do_cmode(s_ChanServ, 3, av);
              free(av[1]);
              free(av[0]);
         }
         canalopers(s_OperServ, "12%s se APODERA de %s", u->nick, chan);
    }
}
                                                   
/**************************************************************************/

static void do_limpia(User *u)
{
    char *chan = strtok(NULL, " ");
    
    Channel *c;
    
    if (!chan) {
        privmsg(s_OperServ, u->nick, "Sintaxis: 12LIMPIA <canal>");
        return;
    } else if (!(c = findchan(chan))) {
        privmsg(s_OperServ, u->nick, "El canal 12%s esta vacio.", chan);
    } else {
        char *av[3];
        struct c_userlist *cu, *next;
        char buf[256];
       
        snprintf(buf, sizeof(buf), "No puedes permanecer en este canal");

#ifdef IRC_UNDERNET_P10
        send_cmd(s_ChanServ, "J %s", chan);
        send_cmd(ServerName, "M %s +o %s", chan, s_ChanServP10);
        send_cmd(s_ChanServ, "M %s :+tnsim", chan);                       
#else
        send_cmd(s_ChanServ, "JOIN %s", chan);
        send_cmd(ServerName, "MODE %s +o %s", chan, s_ChanServ);
        send_cmd(s_ChanServ, "MODE %s :+tnsim", chan);
#endif        
        for (cu = c->users; cu; cu = next) {
             next = cu->next;
             av[0] = sstrdup(chan);
#ifdef IRC_UNDERNET_P10             
             av[1] = sstrdup(cu->user->numerico);
#else
             av[1] = sstrdup(cu->user->nick);
#endif             
             av[2] = sstrdup(buf);
             send_cmd(MODE_SENDER(s_ChanServ), "KICK %s %s :%s",
                       av[0], av[1], av[2]);
                                          
             do_kick(s_ChanServ, 3, av);
             free(av[2]);
             free(av[1]);
             free(av[0]);
        }
        canalopers(s_OperServ, "12%s ha LIMPIADO 12%s", u->nick, chan);
    }                                                                                                                                
}

/***************************************************************************/
 
static void do_vhost(User *u)
{
    char *nick = strtok(NULL, " ");
    char *mask = strtok(NULL, "");
    NickInfo *ni;
    
    if (!nick) {
    	syntax_error(s_OperServ, u, "VHOST", OPER_VHOST_SYNTAX);
    	return;
    } else if (!(ni = findnick(nick))) {
	notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
	return;
    } else if (ni->vhost && !(ni->flags & NI_IPVIRTUALV)) {
        privmsg(s_NickServ, u->nick, "Ya tiene una IPVIRTUAL en la tabla w.");
        return;
    }
    
    if (!mask) {
    	do_write_bdd(nick, 2, "");
        free(ni->vhost);
        ni->flags &= ~NI_IPVIRTUALV;
	notice_lang(s_OperServ, u, OPER_VHOST_UNSET, nick);
    } else {
        if ( (strlen(mask) > 64) || (strlen(mask) < 3) ) {
            privmsg(s_NickServ, u->nick, "Tu IP virtual debe estar entre 3 y 64 caracteres.");
            return;
        }
        if (!valid_ipvirtual(mask)) {
            privmsg(s_NickServ, u->nick, "La IP virtual que intenta poner contiene caracteres ilegales.");
            privmsg(s_NickServ, u->nick, "Recuerde que los carácteres válidos son 'A'-'Z', 'a'-'z', '0'-'9', '.' y '-', y no puede terminar en ninguno de los dos últimos.");
            return;
        }
        do_write_bdd(nick, 2, mask);
        ni->vhost = sstrdup(mask);
        ni->flags |= NI_IPVIRTUALV;
        notice_lang(s_OperServ, u, OPER_VHOST_SET, nick, mask);
    }
}

/***************************************************************************/
 
static void do_vhost2(User *u)
{
    char *nick = strtok(NULL, " ");
    char *mask = strtok(NULL, "");
    NickInfo *ni;
    
    if (!nick) {
    	syntax_error(s_OperServ, u, "VHOST", OPER_VHOST_SYNTAX);
    	return;
    } else if (!(ni = findnick(nick))) {
	notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
	return;
    } else if (ni->vhost && (ni->flags & NI_IPVIRTUALV)) {
        privmsg(s_NickServ, u->nick, "Ya tiene una IPVIRTUAL en la tabla v.");
        return;
    }
    
    if (!mask) {
    	do_write_bdd(nick, 4, "");
        free(ni->vhost);
	notice_lang(s_OperServ, u, OPER_VHOST_UNSET, nick);
    } else {
        if ( (strlen(mask) > 64) || (strlen(mask) < 3) ) {
            privmsg(s_NickServ, u->nick, "Tu IP virtual debe estar entre 3 y 64 caracteres.");
            return;
        }
        if (!valid_ipvirtual(mask)) {
            privmsg(s_NickServ, u->nick, "La IP virtual que intenta poner contiene caracteres ilegales.");
            privmsg(s_NickServ, u->nick, "Recuerde que los carácteres válidos son 'A'-'Z', 'a'-'z', '0'-'9', '.' y '-', y no puede terminar en ninguno de los dos últimos.");
            return;
        }
        do_write_bdd(nick, 4, mask);
        ni->vhost = sstrdup(mask);
        notice_lang(s_OperServ, u, OPER_VHOST_SET, nick, mask);
    }
}

/**************************************************************************/
/* Kill basico */

static void do_skill(User *u)
{
    char *nick = strtok(NULL, " ");
    char *text = strtok(NULL, "");
    User *u2 = NULL;

    if (!nick) {
        privmsg(s_OperServ, u->nick, "Sintaxis: 12KILL <nick> <motivo>");
        return;
    }
    
    u2 = finduser(nick);

    if (!u2) {
        notice_lang(s_OperServ, u, NICK_X_NOT_IN_USE, nick);
    } else if (is_a_service(nick)) {
        notice_lang(s_OperServ, u, PERMISSION_DENIED);
    } else if (!text) {
        send_cmd(ServerName, "KILL %s :Has sido expulsado de la red :)", nick);
        canalopers(s_OperServ, "12%s utilizó KILL sobre 12%s", u->nick, nick);
    } else {
   	send_cmd(ServerName, "KILL %s :%s", nick, text);
        canalopers(s_OperServ, "12%s utilizó KILL sobre 12%s", u->nick, nick);
    }
}
/**************************************************************************/

/* Gline de 5 minutos */

static void do_block(User *u)
{
    char *nick = strtok(NULL, " ");
    char *text = strtok(NULL, "");
    User *u2 = NULL; 
    
    if (!text) {
        privmsg(s_OperServ, u->nick, "Sintaxis: 12BLOCK <nick> <motivo>");    
        return;
    }         
    
    u2 = finduser(nick);   
        
    if (!u2) {
        notice_lang(s_OperServ, u, NICK_X_NOT_IN_USE, nick);
    } else {
    
        send_cmd(ServerName, "GLINE * +*@%s 300 :%s", u2->host, text);
 
        privmsg(s_OperServ, u->nick, "Has cepillado a 12%s", nick);
        canalopers(s_OperServ, "12%s ha cepillado a 12%s (%s)", u->nick, nick, text);
    }
                                          
}


/**************************************************************************/

/* Quitar un block o gline */

static void do_unblock(User *u)
{
    char *mascara = strtok(NULL, " ");
    
    if (!mascara) {
#ifdef IRC_UNDERNET_P10
        privmsg(s_OperServ, u->numeric, "Sintaxis: 12UNBLOCK/UNGLINE <*@host.es>");
#else
        privmsg(s_OperServ, u->nick, "Sintaxis: 12UNBLOCK/UNGLINE <*@host.es>");
#endif
    } else {
        send_cmd(ServerName ,"GLINE * -%s", mascara);
        canalopers(s_OperServ, "12%s ha usado UNBLOCK/UNGLINE en 12%s", u->nick, mascara);
    }
}
                                        
/*************************************************************************/

/* Sincronizar la red en tiempo real */

static void do_settime(User *u)
{
    time_t tiempo = time(NULL);

    send_cmd(NULL, "SETTIME %lu", tiempo);
/*#if HAVE_ALLWILD_NOTICE
    send_cmd(s_OperServ, "NOTICE $*.%s :Sincronizando la RED...", NETWORK_DOMAIN);
    
#else
# ifdef NETWORK_DOMAIN
    send_cmd(s_OperServ, "NOTICE $*.%s :Sincronizando la RED...", NETWORK_DOMAIN);
# else
     * Go through all common top-level domains.  If you have others,
     * add them here.
     
    send_cmd(s_OperServ, "NOTICE $*.es :Sincronizando la RED...");
    send_cmd(s_OperServ, "NOTICE $*.com :Sincronizando la RED...");
    send_cmd(s_OperServ, "NOTICE $*.net :Sincronizando la RED...");
    send_cmd(s_OperServ, "NOTICE $*.org :Sincronizando la RED...");
    send_cmd(s_OperServ, "NOTICE $*.edu :Sincronizando la RED...");
# endif
#endif                                          
    canalopers(s_OperServ, "12%s ha usado SETTIME", u->nick); */
}
            


/*************************************************************************/

/* Services admin list viewing/modification. */

static void do_admin(User *u)
{
    char *cmd, *nick;
    NickInfo *ni;
    int i;

    if (skeleton) {
	notice_lang(s_OperServ, u, OPER_ADMIN_SKELETON, "ADMIN");
	return;
    }
    cmd = strtok(NULL, " ");
    if (!cmd)
	cmd = "";

    if (stricmp(cmd, "ADD") == 0) {
	if (!is_services_root(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    } 
	    if (!(ni->status & NI_ON_BDD)) {
	       privmsg(s_OperServ, u->nick, "No pueden añadirse nicks que no esten migrados a la BDD");
	       return;
            }
	    
	    for (i = 0; i < MAX_SERVADMINS; i++) {
		if (!services_admins[i] || services_admins[i] == ni)
		    break;
	    }
	    if (services_admins[i] == ni) {
		notice_lang(s_OperServ, u, OPER_ADMIN_EXISTS, ni->nick, "Administradores");
	    } else if (i < MAX_SERVADMINS) {
		services_admins[i] = ni;
		notice_lang(s_OperServ, u, OPER_ADMIN_ADDED, ni->nick, "Administradores");
		canaladmins(s_OperServ, "12%s añade a 12%s como ADMIN", u->nick, ni->nick);
	    	do_write_bdd(ni->nick, 3, "+a-kX");
	        send_cmd(NULL, "RENAME %s", ni->nick);
	    } else {
		notice_lang(s_OperServ, u, OPER_ADMIN_TOO_MANY, MAX_SERVADMINS, "Administradores");
	    }
	    if (readonly)
		notice_lang(s_OperServ, u, READ_ONLY_MODE);
	} else {
	    syntax_error(s_OperServ, u, "ADMIN", OPER_ADMIN_ADD_SYNTAX);
	}

    } else if (stricmp(cmd, "DEL") == 0) {
	if (!is_services_root(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    }
	    for (i = 0; i < MAX_SERVADMINS; i++) {
		if (services_admins[i] == ni)
		    break;
	    }
	    if (i < MAX_SERVADMINS) {
		services_admins[i] = NULL;
		notice_lang(s_OperServ, u, OPER_ADMIN_REMOVED, ni->nick, "Administradores");
		canaladmins(s_OperServ, "12%s borra a 12%s como ADMIN", u->nick, ni->nick);
		do_write_bdd(ni->nick, 3, "");
		send_cmd(NULL, "RENAME %s", ni->nick);
		if (readonly)
		    notice_lang(s_OperServ, u, READ_ONLY_MODE);
	    } else {
		notice_lang(s_OperServ, u, OPER_ADMIN_NOT_FOUND, ni->nick, "Administradores");
	    }
	} else {
	    syntax_error(s_OperServ, u, "ADMIN", OPER_ADMIN_DEL_SYNTAX);
	}

    } else if (stricmp(cmd, "LIST") == 0) {
	notice_lang(s_OperServ, u, OPER_ADMIN_LIST_HEADER, "Administradores");
	for (i = 0; i < MAX_SERVADMINS; i++) {
	    if (services_admins[i])
#ifdef IRC_UNDERNET_P10
                privmsg(s_OperServ, u->numerico, "%s", services_admins[i]->nick);
#else	    
		privmsg(s_OperServ, u->nick, "%s", services_admins[i]->nick);
#endif
	}

    } else {
	syntax_error(s_OperServ, u, "ADMIN", OPER_ADMIN_SYNTAX);
    }
}

/*************************************************************************/

/* Services coadmin list viewing/modification. */

static void do_coadmin(User *u)
{
    char *cmd, *nick;
    NickInfo *ni;
    int i;

    if (skeleton) {
	notice_lang(s_OperServ, u, OPER_ADMIN_SKELETON, "COADMIN");
	return;
    }
    cmd = strtok(NULL, " ");
    if (!cmd)
	cmd = "";

    if (stricmp(cmd, "ADD") == 0) {
	if (!is_services_root(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    } 
	    if (!(ni->status & NI_ON_BDD)) {
	       privmsg(s_OperServ, u->nick, "No pueden añadirse nicks que no esten migrados a la BDD");
	       return;
            }
	    
	    for (i = 0; i < MAX_SERVADMINS; i++) {
		if (!services_coadmins[i] || services_coadmins[i] == ni)
		    break;
	    }
	    if (services_coadmins[i] == ni) {
		notice_lang(s_OperServ, u, OPER_ADMIN_EXISTS, ni->nick, "CoAdministradores");
	    } else if (i < MAX_SERVADMINS) {
		services_coadmins[i] = ni;
		notice_lang(s_OperServ, u, OPER_ADMIN_ADDED, ni->nick, "CoAdministradores");
		canaladmins(s_OperServ, "12%s añade a 12%s como COADMIN", u->nick, ni->nick);
	    	do_write_bdd(ni->nick, 3, "+c-k");
	        send_cmd(NULL, "RENAME %s", ni->nick);
	    } else {
		notice_lang(s_OperServ, u, OPER_ADMIN_TOO_MANY, MAX_SERVADMINS, "CoAdministradores");
	    }
	    if (readonly)
		notice_lang(s_OperServ, u, READ_ONLY_MODE);
	} else {
	    syntax_error(s_OperServ, u, "COADMIN", OPER_ADMIN_ADD_SYNTAX);
	}

    } else if (stricmp(cmd, "DEL") == 0) {
	if (!is_services_root(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    }
	    for (i = 0; i < MAX_SERVADMINS; i++) {
		if (services_coadmins[i] == ni)
		    break;
	    }
	    if (i < MAX_SERVADMINS) {
		services_coadmins[i] = NULL;
		notice_lang(s_OperServ, u, OPER_ADMIN_REMOVED, ni->nick, "CoAdministradores");
		canaladmins(s_OperServ, "12%s borra a 12%s como COADMIN", u->nick, ni->nick);
		do_write_bdd(ni->nick, 3, "");
		send_cmd(NULL, "RENAME %s", ni->nick);
		if (readonly)
		    notice_lang(s_OperServ, u, READ_ONLY_MODE);
	    } else {
		notice_lang(s_OperServ, u, OPER_ADMIN_NOT_FOUND, ni->nick, "CoAdministradores");
	    }
	} else {
	    syntax_error(s_OperServ, u, "COADMIN", OPER_ADMIN_DEL_SYNTAX);
	}

    } else if (stricmp(cmd, "LIST") == 0) {
	notice_lang(s_OperServ, u, OPER_ADMIN_LIST_HEADER, "CoAdministradores");
	for (i = 0; i < MAX_SERVADMINS; i++) {
	    if (services_coadmins[i])
#ifdef IRC_UNDERNET_P10
                privmsg(s_OperServ, u->numerico, "%s", services_coadmins[i]->nick);
#else	    
		privmsg(s_OperServ, u->nick, "%s", services_coadmins[i]->nick);
#endif
	}

    } else {
	syntax_error(s_OperServ, u, "COADMIN", OPER_ADMIN_SYNTAX);
    }
}

/*************************************************************************/

/* Services cregadmin list viewing/modification. */

static void do_cregadmin(User *u)
{
    char *cmd, *nick;
    NickInfo *ni;
    int i;

    if (skeleton) {
	notice_lang(s_OperServ, u, OPER_ADMIN_SKELETON, "CREGADMIN");
	return;
    }
    cmd = strtok(NULL, " ");
    if (!cmd)
	cmd = "";

    if (stricmp(cmd, "ADD") == 0) {
	if (!is_services_root(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    } 
	    if (!(ni->status & NI_ON_BDD)) {
	       privmsg(s_OperServ, u->nick, "No pueden añadirse nicks que no esten migrados a la BDD");
	       return;
            }
	    
	    for (i = 0; i < MAX_SERVADMINS; i++) {
		if (!services_cregadmins[i] || services_cregadmins[i] == ni)
		    break;
	    }
	    if (services_cregadmins[i] == ni) {
		notice_lang(s_OperServ, u, OPER_ADMIN_EXISTS, ni->nick, "CregAdmins");
	    } else if (i < MAX_SERVADMINS) {
		services_cregadmins[i] = ni;
		notice_lang(s_OperServ, u, OPER_ADMIN_ADDED, ni->nick, "CregAdmins");
		canaladmins(s_OperServ, "12%s añade a 12%s como CREGADMIN", u->nick, ni->nick);
	    } else {
		notice_lang(s_OperServ, u, OPER_ADMIN_TOO_MANY, MAX_SERVADMINS, "CregAdmins");
	    }
	    if (readonly)
		notice_lang(s_OperServ, u, READ_ONLY_MODE);
	} else {
	    syntax_error(s_OperServ, u, "CREGADMIN", OPER_ADMIN_ADD_SYNTAX);
	}

    } else if (stricmp(cmd, "DEL") == 0) {
	if (!is_services_root(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    }
	    for (i = 0; i < MAX_SERVADMINS; i++) {
		if (services_cregadmins[i] == ni)
		    break;
	    }
	    if (i < MAX_SERVADMINS) {
		services_cregadmins[i] = NULL;
		notice_lang(s_OperServ, u, OPER_ADMIN_REMOVED, ni->nick, "CregAdmins");
		canaladmins(s_OperServ, "12%s borra a 12%s como CREGADMIN", u->nick, ni->nick);
		if (readonly)
		    notice_lang(s_OperServ, u, READ_ONLY_MODE);
	    } else {
		notice_lang(s_OperServ, u, OPER_ADMIN_NOT_FOUND, ni->nick, "CregAdmins");
	    }
	} else {
	    syntax_error(s_OperServ, u, "CREGADMIN", OPER_ADMIN_DEL_SYNTAX);
	}

    } else if (stricmp(cmd, "LIST") == 0) {
	notice_lang(s_OperServ, u, OPER_ADMIN_LIST_HEADER, "CregAdmins");
	for (i = 0; i < MAX_SERVADMINS; i++) {
	    if (services_cregadmins[i])
#ifdef IRC_UNDERNET_P10
                privmsg(s_OperServ, u->numerico, "%s", services_cregadmins[i]->nick);
#else	    
		privmsg(s_OperServ, u->nick, "%s", services_cregadmins[i]->nick);
#endif
	}

    } else {
	syntax_error(s_OperServ, u, "CREGADMIN", OPER_ADMIN_SYNTAX);
    }
}

/*************************************************************************/

/* Services oper list viewing/modification. */

static void do_oper(User *u)
{
    char *cmd, *nick;
    NickInfo *ni;
    int i;

    if (skeleton) {
	notice_lang(s_OperServ, u, OPER_OPER_SKELETON, "OPER");
	return;
    }
    cmd = strtok(NULL, " ");
    if (!cmd)
	cmd = "";

    if (stricmp(cmd, "ADD") == 0) {
	if (!is_services_admin(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    } 
	    
	    if (!(ni->status & NI_ON_BDD)) {
	        notice_lang(s_OperServ, u, NICK_MUST_BE_ON_BDD);
	        return;
            }
	    
	    for (i = 0; i < MAX_SERVOPERS; i++) {
		if (!services_opers[i] || services_opers[i] == ni)
		    break;
	    }
	    if (services_opers[i] == ni) {
		notice_lang(s_OperServ, u, OPER_OPER_EXISTS, ni->nick, "Operadores");
	    } else if (i < MAX_SERVOPERS) {
		services_opers[i] = ni;
		notice_lang(s_OperServ, u, OPER_OPER_ADDED, ni->nick, "Operadores");
		canaladmins(s_OperServ, "12%s añade a 12%s como OPER", u->nick, ni->nick);
	    	do_write_bdd(ni->nick, 3, "+h-k");
		send_cmd(NULL, "RENAME %s", ni->nick);
	    } else {
		notice_lang(s_OperServ, u, OPER_OPER_TOO_MANY, MAX_SERVOPERS, "Operadores");
	    }
	    if (readonly)
		notice_lang(s_OperServ, u, READ_ONLY_MODE);
	} else {
	    syntax_error(s_OperServ, u, "OPER", OPER_OPER_ADD_SYNTAX);
	}

    } else if (stricmp(cmd, "DEL") == 0) {
	if (!is_services_admin(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    }
	    for (i = 0; i < MAX_SERVOPERS; i++) {
		if (services_opers[i] == ni)
		    break;
	    }
	    if (i < MAX_SERVOPERS) {
		services_opers[i] = NULL;
		notice_lang(s_OperServ, u, OPER_OPER_REMOVED, ni->nick, "Operadores");
		canaladmins(s_OperServ, "12%s borra a 12%s como OPER", u->nick, ni->nick);
		do_write_bdd(ni->nick, 3, "");
		send_cmd(NULL, "RENAME %s", ni->nick);		
		if (readonly)
		    notice_lang(s_OperServ, u, READ_ONLY_MODE);
	    } else {
		notice_lang(s_OperServ, u, OPER_OPER_NOT_FOUND, ni->nick, "Operadores");
	    }
	} else {
	    syntax_error(s_OperServ, u, "OPER", OPER_OPER_DEL_SYNTAX);
	}

    } else if (stricmp(cmd, "LIST") == 0) {
	notice_lang(s_OperServ, u, OPER_OPER_LIST_HEADER, "Operadores");
	for (i = 0; i < MAX_SERVOPERS; i++) {
	    if (services_opers[i])
#ifdef IRC_UNDERNET_P10
                privmsg(s_OperServ, u->numerico, "%s", services_opers[i]->nick);
#else	    
		privmsg(s_OperServ, u->nick, "%s", services_opers[i]->nick);
#endif
	}

    } else {
	syntax_error(s_OperServ, u, "OPER", OPER_OPER_SYNTAX);
    }
}

/*************************************************************************/

/* Services preoper list viewing/modification. */

static void do_preoper(User *u)
{
    char *cmd, *nick;
    NickInfo *ni;
    int i;

    if (skeleton) {
	notice_lang(s_OperServ, u, OPER_OPER_SKELETON, "PREOPER");
	return;
    }
    cmd = strtok(NULL, " ");
    if (!cmd)
	cmd = "";

    if (stricmp(cmd, "ADD") == 0) {
	if (!is_services_admin(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    } 
	    
	    if (!(ni->status & NI_ON_BDD)) {
	        notice_lang(s_OperServ, u, NICK_MUST_BE_ON_BDD);
	        return;
            }
	    
	    for (i = 0; i < MAX_SERVOPERS; i++) {
		if (!services_preopers[i] || services_preopers[i] == ni)
		    break;
	    }
	    if (services_preopers[i] == ni) {
		notice_lang(s_OperServ, u, OPER_OPER_EXISTS, ni->nick, "PreOperadores");
	    } else if (i < MAX_SERVOPERS) {
		services_preopers[i] = ni;
		notice_lang(s_OperServ, u, OPER_OPER_ADDED, ni->nick, "PreOperadores");
		canaladmins(s_OperServ, "12%s añade a 12%s como PREOPER", u->nick, ni->nick);
	    	do_write_bdd(ni->nick, 3, "+p");
		send_cmd(NULL, "RENAME %s", ni->nick);
	    } else {
		notice_lang(s_OperServ, u, OPER_OPER_TOO_MANY, MAX_SERVOPERS, "PreOperadores");
	    }
	    if (readonly)
		notice_lang(s_OperServ, u, READ_ONLY_MODE);
	} else {
	    syntax_error(s_OperServ, u, "PREOPER", OPER_OPER_ADD_SYNTAX);
	}

    } else if (stricmp(cmd, "DEL") == 0) {
	if (!is_services_admin(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    }
	    for (i = 0; i < MAX_SERVOPERS; i++) {
		if (services_preopers[i] == ni)
		    break;
	    }
	    if (i < MAX_SERVOPERS) {
		services_preopers[i] = NULL;
		notice_lang(s_OperServ, u, OPER_OPER_REMOVED, ni->nick, "PreOperadores");
		canaladmins(s_OperServ, "12%s borra a 12%s como PREOPER", u->nick, ni->nick);
		do_write_bdd(ni->nick, 3, "");
		send_cmd(NULL, "RENAME %s", ni->nick);		
		if (readonly)
		    notice_lang(s_OperServ, u, READ_ONLY_MODE);
	    } else {
		notice_lang(s_OperServ, u, OPER_OPER_NOT_FOUND, ni->nick, "PreOperadores");
	    }
	} else {
	    syntax_error(s_OperServ, u, "PREOPER", OPER_OPER_DEL_SYNTAX);
	}

    } else if (stricmp(cmd, "LIST") == 0) {
	notice_lang(s_OperServ, u, OPER_OPER_LIST_HEADER, "PreOperadores");
	for (i = 0; i < MAX_SERVOPERS; i++) {
	    if (services_opers[i])
#ifdef IRC_UNDERNET_P10
                privmsg(s_OperServ, u->numerico, "%s", services_preopers[i]->nick);
#else	    
		privmsg(s_OperServ, u->nick, "%s", services_preopers[i]->nick);
#endif
	}

    } else {
	syntax_error(s_OperServ, u, "PREOPER", OPER_OPER_SYNTAX);
    }
}

/*************************************************************************/

/* Services bots list viewing/modification. */

static void do_bots(User *u)
{
    char *cmd, *nick;
    NickInfo *ni;
    int i;

    if (skeleton) {
	notice_lang(s_OperServ, u, OPER_OPER_SKELETON, "BOTS");
	return;
    }
    cmd = strtok(NULL, " ");
    if (!cmd)
	cmd = "";

    if (stricmp(cmd, "ADD") == 0) {
	if (!is_services_root(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    } 
	    
	    if (!(ni->status & NI_ON_BDD)) {
	        notice_lang(s_OperServ, u, NICK_MUST_BE_ON_BDD);
	        return;
            }
	    
	    for (i = 0; i < MAX_SERVOPERS; i++) {
		if (!services_bots[i] || services_bots[i] == ni)
		    break;
	    }
	    if (services_bots[i] == ni) {
		notice_lang(s_OperServ, u, OPER_OPER_EXISTS, ni->nick, "Bots");
	    } else if (i < MAX_SERVOPERS) {
		services_bots[i] = ni;
		notice_lang(s_OperServ, u, OPER_OPER_ADDED, ni->nick, "Bots");
		canaladmins(s_OperServ, "12%s añade a 12%s como BOT OFICIAL", u->nick, ni->nick);
	    	do_write_bdd(ni->nick, 3, "+Bk");
		send_cmd(NULL, "RENAME %s", ni->nick);
	    } else {
		notice_lang(s_OperServ, u, OPER_OPER_TOO_MANY, MAX_SERVOPERS, "Bots");
	    }
	    if (readonly)
		notice_lang(s_OperServ, u, READ_ONLY_MODE);
	} else {
	    syntax_error(s_OperServ, u, "BOTS", OPER_OPER_ADD_SYNTAX);
	}

    } else if (stricmp(cmd, "DEL") == 0) {
	if (!is_services_admin(u)) {
	    notice_lang(s_OperServ, u, PERMISSION_DENIED);
	    return;
	}
	nick = strtok(NULL, " ");
	if (nick) {
	    if (!(ni = findnick(nick))) {
		notice_lang(s_OperServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    }
	    for (i = 0; i < MAX_SERVOPERS; i++) {
		if (services_bots[i] == ni)
		    break;
	    }
	    if (i < MAX_SERVOPERS) {
		services_bots[i] = NULL;
		notice_lang(s_OperServ, u, OPER_OPER_REMOVED, ni->nick, "Bots");
		canaladmins(s_OperServ, "12%s borra a 12%s como BOT OFICIAL", u->nick, ni->nick);
		do_write_bdd(ni->nick, 3, "");
		send_cmd(NULL, "RENAME %s", ni->nick);		
		if (readonly)
		    notice_lang(s_OperServ, u, READ_ONLY_MODE);
	    } else {
		notice_lang(s_OperServ, u, OPER_OPER_NOT_FOUND, ni->nick, "Bots");
	    }
	} else {
	    syntax_error(s_OperServ, u, "BOTS", OPER_OPER_DEL_SYNTAX);
	}

    } else if (stricmp(cmd, "LIST") == 0) {
	notice_lang(s_OperServ, u, OPER_OPER_LIST_HEADER, "Bots");
	for (i = 0; i < MAX_SERVOPERS; i++) {
	    if (services_opers[i])
#ifdef IRC_UNDERNET_P10
                privmsg(s_OperServ, u->numerico, "%s", services_bots[i]->nick);
#else	    
		privmsg(s_OperServ, u->nick, "%s", services_bots[i]->nick);
#endif
	}

    } else {
	syntax_error(s_OperServ, u, "BOTS", OPER_OPER_SYNTAX);
    }
}


/*************************************************************************/

/* Set various Services runtime options. */

static void do_set(User *u)
{
    char *option = strtok(NULL, " ");
    char *setting = strtok(NULL, " ");

    if (!option || !setting) {
	syntax_error(s_OperServ, u, "SET", OPER_SET_SYNTAX);

    } else if (stricmp(option, "IGNORE") == 0) {
	if (stricmp(setting, "on") == 0) {
	    allow_ignore = 1;
	    notice_lang(s_OperServ, u, OPER_SET_IGNORE_ON);
	} else if (stricmp(setting, "off") == 0) {
	    allow_ignore = 0;
	    notice_lang(s_OperServ, u, OPER_SET_IGNORE_OFF);
	} else {
	    notice_lang(s_OperServ, u, OPER_SET_IGNORE_ERROR);
	}

    } else if (stricmp(option, "READONLY") == 0) {
	if (stricmp(setting, "on") == 0) {
	    readonly = 1;
	    log("Read-only mode activated");
	    close_log();
	    notice_lang(s_OperServ, u, OPER_SET_READONLY_ON);
	} else if (stricmp(setting, "off") == 0) {
	    readonly = 0;
	    open_log();
	    log("Read-only mode deactivated");
	    notice_lang(s_OperServ, u, OPER_SET_READONLY_OFF);
	} else {
	    notice_lang(s_OperServ, u, OPER_SET_READONLY_ERROR);
	}

    } else if (stricmp(option, "DEBUG") == 0) {
	if (stricmp(setting, "on") == 0) {
	    debug = 1;
	    log("Debug mode activated");
	    notice_lang(s_OperServ, u, OPER_SET_DEBUG_ON);
	} else if (stricmp(setting, "off") == 0 ||
				(*setting == '0' && atoi(setting) == 0)) {
	    log("Debug mode deactivated");
	    debug = 0;
	    notice_lang(s_OperServ, u, OPER_SET_DEBUG_OFF);
	} else if (isdigit(*setting) && atoi(setting) > 0) {
	    debug = atoi(setting);
	    log("Debug mode activated (level %d)", debug);
	    notice_lang(s_OperServ, u, OPER_SET_DEBUG_LEVEL, debug);
	} else {
	    notice_lang(s_OperServ, u, OPER_SET_DEBUG_ERROR);
	}

    } else {
	notice_lang(s_OperServ, u, OPER_SET_UNKNOWN_OPTION, option);
    }
}

/*************************************************************************/

/*static void do_jupe(User *u)
{
    char *jserver = strtok(NULL, " ");
    char *reason = strtok(NULL, "");
    char buf[NICKMAX+16];
    Server *server;

#ifdef IRC_UNDERNET_P10
    char *destino=u->numerico;
#else
    char *destino=u->nick;
#endif

    if (!jserver) 
	syntax_error(s_OperServ, u, "JUPE", OPER_JUPE_SYNTAX);
    else 
	if (!reason) {
	    snprintf(buf, sizeof(buf), "Jupeado por %s", u->nick);
	    reason = buf;
	}	
    server = find_servername(jserver);
    
    if (!server) {
        privmsg(s_OperServ, destino, "No encuentro el server 12%s para JUPEarlo", jserver);
        return;
    } else {
                
#ifdef IRC_UNDERNET_P09
        send_cmd(NULL, "SQUIT %s 0 :%s", jserver, reason);
        send_cmd(NULL, "SERVER %s 1 %lu %lu P10 :%s",
                   jserver, time(NULL), time(NULL), reason);               
        canalopers(s_OperServ, "argv[0] ya! y completado!");        
     
#elif defined (IRC_UNDERNET_P10)
        send_cmd(NULL, "%c SQ %s 0 :%s", convert2y[ServerNumerico], jserver, reason);
        send_cmd(NULL, "%c S %s 1 %lu %lu J10 %s 0 :%s",
            convert2y[ServerNumerico], jserver, time(NULL), time(NULL), server->numerico, reason);                                
#else
	send_cmd(NULL, "SERVER %s 2 :%s", jserver, reason);                        	
#endif
        privmsg(s_OperServ, destino, "Has JUPEado el servidor 12%s", jserver);
        canalopers(s_OperServ, "12JUPE  de %s por 12%s.",
                               jserver, u->nick);        
        log("%s: %s!%s@%s ha JUPEado a %s (%s)", s_OperServ, u->nick, u->realname, u->host,
            jserver, reason);
                                
    }
}
*/
/*************************************************************************/

static void do_raw(User *u)
{
    char *text = strtok(NULL, "");

    if (!text) {
	syntax_error(s_OperServ, u, "RAW", OPER_RAW_SYNTAX);
    } else {
 	send_cmd(NULL, "%s", text);
/*	canaladmins(s_OperServ, "12%s ha usado 12RAW para: %s.", 
	  u->nick, text);*/
    }
}

/*************************************************************************/

static void do_update(User *u)
{
    notice_lang(s_OperServ, u, OPER_UPDATING);
    save_data = 1;
    canaladmins(s_OperServ, "12%s ejecuta UPDATE. 4Actualizando bases de datos..", u->nick);
}

/*************************************************************************/

static void do_os_quit(User *u)
{
    quitmsg = malloc(50 + strlen(u->nick));
    if (!quitmsg)
	quitmsg = "Aieee! QUITing Services...!";
    else
	sprintf(quitmsg, "Aieee! Services ha recibido una orden de QUIT de %s", u->nick);
//    canaladmins(s_OperServ, "%s", quitmsg);
    quitting = 1;
}

/*************************************************************************/

static void do_shutdown(User *u)
{
    quitmsg = malloc(54 + strlen(u->nick));
    if (!quitmsg)
    
	quitmsg = "Aieee! SHUTDOWNing Services...!";
    else
	sprintf(quitmsg, "Aieee! Services ha recibido una orden de SHUTDOWN de %s", u->nick);
//    canaladmins(s_OperServ, "%s", quitmsg);
    save_data = 1;
    delayed_quit = 1;
}

/*************************************************************************/

static void do_restart(User *u)
{
#ifdef SERVICES_BIN
    quitmsg = malloc(53 + strlen(u->nick));
    if (!quitmsg)
	quitmsg = "Aieee! RESTARTing Services...!";
    else
	sprintf(quitmsg, "Aieee! Services ha recibido una orden de RESTART de %s", u->nick);
//    canaladmins(s_OperServ, "%s", quitmsg);
    raise(SIGHUP);
#else
    notice_lang(s_OperServ, u, OPER_CANNOT_RESTART);
#endif
}

/*************************************************************************/

/* XXX - this function is broken!! */

static void do_listignore(User *u)
{
    int sent_header = 0;
    IgnoreData *id;
    int i;

    notice(s_OperServ, u->nick, "Command disabled - it's broken.");
    return;
    
    for (i = 0; i < 256; i++) {
	for (id = ignore[i]; id; id = id->next) {
	    if (!sent_header) {
		notice_lang(s_OperServ, u, OPER_IGNORE_LIST);
		sent_header = 1;
	    }
	    notice(s_OperServ, u->nick, "%ld %s", id->time, id->who);
	}
    }
    if (!sent_header)
	notice_lang(s_OperServ, u, OPER_IGNORE_LIST_EMPTY);
}

/*************************************************************************/

#ifdef DEBUG_COMMANDS

static void do_matchwild(User *u)
{
    char *pat = strtok(NULL, " ");
    char *str = strtok(NULL, " ");
    if (pat && str)
	notice(s_OperServ, u->nick, "%d", match_wild(pat, str));
    else
	notice(s_OperServ, u->nick, "Syntax error.");
}

#endif	/* DEBUG_COMMANDS */
