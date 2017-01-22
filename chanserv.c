/* ChanServ functions.
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

/*************************************************************************/

#include "services.h"
#include "pseudo.h"

/*************************************************************************/
#define TEXT_AUTOHALFOP "Permite obtenet automaticamente nivel de semi-operador"
#define TEXT_HALFOP "Para ponerse y quitarse el nivel de semi-operador"

static ChannelInfo *chanlists[256];

static int def_levels[][2] = {
    { CA_AUTOOP,            300 },
    { CA_AUTOVOICE,         100 },
    { CA_AUTODEOP,            -1 },
    { CA_NOJOIN,              -1 },
    { CA_INVITE,            300 },
    { CA_AKICK,             450 },
    { CA_SET,    ACCESS_INVALID },
    { CA_CLEAR,  ACCESS_INVALID },
    { CA_UNBAN,             300 },
    { CA_OPDEOP,            200 },
    { CA_ACCESS_LIST,           0 },
    { CA_ACCESS_CHANGE,     450 },
    { CA_MEMO,              100 },
    { CA_RESET,             450 },
    { CA_AUTODEVOICE          -1 },
    { CA_VOICEDEVOICE,        50 },
    { CA_KICK,              400 },
    { CA_BAN,               400 },                    
    { CA_AUTOHALFOP,        200 },
    { CA_HALFOPDEHALFOP,    150 },
    { -1 }
};

typedef struct {
    int what;
    char *name;    
    int desc;
} LevelInfo;
static LevelInfo levelinfo[] = {
    { CA_AUTOOP,        "AUTOOP",     CHAN_LEVEL_AUTOOP },
    { CA_AUTOVOICE,     "AUTOVOICE",  CHAN_LEVEL_AUTOVOICE },
    { CA_AUTODEOP,      "AUTODEOP",   CHAN_LEVEL_AUTODEOP },
/*    { CA_AUTODEVOICE,   "AUTODEVOICE", CHAN_LEVEL_AUTODEVOICE },  */
    { CA_NOJOIN,        "NOJOIN",     CHAN_LEVEL_NOJOIN },
    { CA_INVITE,        "INVITE",     CHAN_LEVEL_INVITE },
    { CA_OPDEOP,        "OPDEOP",     CHAN_LEVEL_OPDEOP },
    { CA_VOICEDEVOICE,  "VOICEDEVOICE", CHAN_LEVEL_VOICEDEVOICE },        
    { CA_KICK,          "KICK",       CHAN_LEVEL_KICK },
/*    { CA_BAN,           "BAN",        CHAN_LEVEL_BAN }  */
    { CA_UNBAN,         "UNBAN",      CHAN_LEVEL_UNBAN },            
    { CA_AKICK,         "AKICK",      CHAN_LEVEL_AKICK },
    { CA_SET,           "SET",        CHAN_LEVEL_SET },
    { CA_CLEAR,         "CLEAR",      CHAN_LEVEL_CLEAR },
    { CA_RESET,         "RESET",      CHAN_LEVEL_RESET },
    { CA_ACCESS_LIST,   "ACC-LIST",   CHAN_LEVEL_ACCESS_LIST },
    { CA_ACCESS_CHANGE, "ACC-CHANGE", CHAN_LEVEL_ACCESS_CHANGE },
    { CA_MEMO,          "MEMO",       CHAN_LEVEL_MEMO },
    //{ CA_AUTOHALFOP,    "AUTOHALFOP", TEXT_AUTOHALFOP },
    //{ CA_HALFOPDEHALFOP, "HALFOPDEHALFOP", TEXT_HALFOP },
    { -1 }
};
static int levelinfo_maxwidth = 0;

/*************************************************************************/

static void alpha_insert_chan(ChannelInfo *ci);
static ChannelInfo *makechan(const char *chan);
static int delchan(ChannelInfo *ci);
static void reset_levels(ChannelInfo *ci);
static int is_founder(User *user, ChannelInfo *ci);
static int is_identified(User *user, ChannelInfo *ci);
static int get_access(User *user, ChannelInfo *ci);

static void do_credits(User *u);
static void do_help(User *u);
static void do_register(User *u);
static void do_identify(User *u);
static void do_drop(User *u);
static void do_set(User *u);
static void do_set_founder(User *u, ChannelInfo *ci, char *param);
static void do_set_successor(User *u, ChannelInfo *ci, char *param);
static void do_set_password(User *u, ChannelInfo *ci, char *param);
static void do_set_desc(User *u, ChannelInfo *ci, char *param);
static void do_set_url(User *u, ChannelInfo *ci, char *param);
static void do_set_email(User *u, ChannelInfo *ci, char *param);
static void do_set_entrymsg(User *u, ChannelInfo *ci, char *param);
static void do_set_topic(User *u, ChannelInfo *ci, char *param);
static void do_set_mlock(User *u, ChannelInfo *ci, char *param);
static void do_set_keeptopic(User *u, ChannelInfo *ci, char *param);
static void do_set_topiclock(User *u, ChannelInfo *ci, char *param);
static void do_set_private(User *u, ChannelInfo *ci, char *param);
static void do_set_securevoices(User *u, ChannelInfo *ci, char *param);
static void do_set_secureops(User *u, ChannelInfo *ci, char *param);
static void do_set_leaveops(User *u, ChannelInfo *ci, char *param);
static void do_set_restricted(User *u, ChannelInfo *ci, char *param);
static void do_set_secure(User *u, ChannelInfo *ci, char *param);
static void do_set_opnotice(User *u, ChannelInfo *ci, char *param);
static void do_set_stay(User *u, ChannelInfo *ci, char *param);
static void do_set_noexpire(User *u, ChannelInfo *ci, char *param);
static void do_access(User *u);
static void do_akick(User *u);
static void do_info(User *u);
static void do_list(User *u);
static void do_invite(User *u);
static void do_levels(User *u);
static void do_op(User *u);
static void do_deop(User *u);
static void do_halfop(User *u);
static void do_dehalfop(User *u);
static void do_voice(User *u);
static void do_devoice(User *u);
static void do_ckick(User *u);
// static void do_ban(User *u);
static void do_unban(User *u);
static void do_clear(User *u);
static void do_reset(User *u);
static void do_verify(User *u);
static void do_ircops(User *u);
static void do_getpass(User *u);
static void do_suspend(User *u);
static void do_unsuspend(User *u);
static void do_forbid(User *u);
static void do_unforbid(User *u);
static void do_status(User *u);
static void do_say(User *u);
static void do_delaccess(User *u);

/*************************************************************************/

static Command cmds[] = {
    { "CREDITS",  do_credits,  NULL,  -1,                       -1,-1,-1,-1 },
    { "CREDITOS", do_credits,  NULL,  -1,                       -1,-1,-1,-1 },        
    { "HELP",     do_help,     NULL,  -1,                       -1,-1,-1,-1 },
    { "AYUDA",    do_help,     NULL,  -1,                       -1,-1,-1,-1 },
    { "DELACCESS", do_delaccess, NULL, CHAN_HELP_DELACCESS,	-1,-1,-1,-1 },
    { "SHOWCOMMANDS",    do_help,   NULL,  -1,                  -1,-1,-1,-1 },
    { ":?",       do_help,     NULL,  -1,                       -1,-1,-1,-1 },
    { "?",        do_help,     NULL,  -1,                       -1,-1,-1,-1 },                 
    { "REGISTER", do_register, NULL,  CHAN_HELP_REGISTER,       -1,-1,-1,-1 },
    { "IDENTIFY", do_identify, NULL,  CHAN_HELP_IDENTIFY,       -1,-1,-1,-1 },    
    { "AUTH",     do_identify, NULL,  CHAN_HELP_IDENTIFY,       -1,-1,-1,-1 },
    { "DROP",     do_drop,     is_services_oper,  -1,
		CHAN_HELP_DROP, CHAN_SERVADMIN_HELP_DROP,
		CHAN_SERVADMIN_HELP_DROP, CHAN_SERVADMIN_HELP_DROP },
    { "SET",      do_set,      NULL,  CHAN_HELP_SET,
		-1, CHAN_SERVADMIN_HELP_SET,
		CHAN_SERVADMIN_HELP_SET, CHAN_SERVADMIN_HELP_SET },
    { "SET FOUNDER",    NULL,  NULL,  CHAN_HELP_SET_FOUNDER,    -1,-1,-1,-1 },
    { "SET SUCCESSOR",  NULL,  NULL,  CHAN_HELP_SET_SUCCESSOR,  -1,-1,-1,-1 },
    { "SET PASSWORD",   NULL,  NULL,  CHAN_HELP_SET_PASSWORD,   -1,-1,-1,-1 },     
    { "SET PASS",       NULL,  NULL,  CHAN_HELP_SET_PASSWORD,   -1,-1,-1,-1 },
    { "SET DESC",       NULL,  NULL,  CHAN_HELP_SET_DESC,       -1,-1,-1,-1 },
    { "SET URL",        NULL,  NULL,  CHAN_HELP_SET_URL,        -1,-1,-1,-1 },
    { "SET EMAIL",      NULL,  NULL,  CHAN_HELP_SET_EMAIL,      -1,-1,-1,-1 },
    { "SET ENTRYMSG",   NULL,  NULL,  CHAN_HELP_SET_ENTRYMSG,   -1,-1,-1,-1 },
    { "SET TOPIC",      NULL,  NULL,  CHAN_HELP_SET_TOPIC,      -1,-1,-1,-1 },
    { "SET KEEPTOPIC",  NULL,  NULL,  CHAN_HELP_SET_KEEPTOPIC,  -1,-1,-1,-1 },
    { "SET TOPICLOCK",  NULL,  NULL,  CHAN_HELP_SET_TOPICLOCK,  -1,-1,-1,-1 },
    { "SET MLOCK",      NULL,  NULL,  CHAN_HELP_SET_MLOCK,      -1,-1,-1,-1 },
    { "SET RESTRICTED", NULL,  NULL,  CHAN_HELP_SET_RESTRICTED, -1,-1,-1,-1 },
    { "SET SECURE",     NULL,  NULL,  CHAN_HELP_SET_SECURE,     -1,-1,-1,-1 },
    { "SET SECUREVOICES",  NULL,  NULL,  CHAN_HELP_SET_SECUREOPS,  -1,-1,-1,-1 },     
    { "SET SECUREOPS",  NULL,  NULL,  CHAN_HELP_SET_SECUREOPS,  -1,-1,-1,-1 },
    { "SET LEAVEOPS",   NULL,  NULL,  CHAN_HELP_SET_LEAVEOPS,   -1,-1,-1,-1 },
    { "SET ISSUED",     NULL,  NULL,  CHAN_HELP_SET_OPNOTICE,   -1,-1,-1,-1 },
    { "SET DEBUG",      NULL,  NULL,  CHAN_HELP_SET_OPNOTICE,   -1,-1,-1,-1 },
    { "SET OPNOTICE",   NULL,  NULL,  CHAN_HELP_SET_OPNOTICE,   -1,-1,-1,-1 },
    { "SET STAY",   NULL,  NULL,  CHAN_HELP_SET_OPNOTICE,   -1,-1,-1,-1 },             
    { "SET NOEXPIRE",   NULL,  NULL,  -1, -1,
		CHAN_SERVADMIN_HELP_SET_NOEXPIRE,
		CHAN_SERVADMIN_HELP_SET_NOEXPIRE,
		CHAN_SERVADMIN_HELP_SET_NOEXPIRE },
    { "ACCESS",   do_access,   NULL,  CHAN_HELP_ACCESS,         -1,-1,-1,-1 },
    { "ACCESS LEVELS",  NULL,  NULL,  CHAN_HELP_ACCESS_LEVELS,  -1,-1,-1,-1 },
    { "AKICK",    do_akick,    NULL,  CHAN_HELP_AKICK,          -1,-1,-1,-1 },
    { "LEVELS",   do_levels,   NULL,  CHAN_HELP_LEVELS,         -1,-1,-1,-1 },
    { "INFO",     do_info,     NULL,  CHAN_HELP_INFO,           
		-1, CHAN_SERVADMIN_HELP_INFO, CHAN_SERVADMIN_HELP_INFO, 
		CHAN_SERVADMIN_HELP_INFO },
    { "LIST",     do_list,     NULL,  -1,
		CHAN_HELP_LIST, CHAN_SERVADMIN_HELP_LIST,
		CHAN_SERVADMIN_HELP_LIST, CHAN_SERVADMIN_HELP_LIST },
    { "VOICE",    do_voice,    NULL,  CHAN_HELP_VOICE,          -1,-1,-1,-1 },
    { "DEVOICE",  do_devoice,  NULL,  CHAN_HELP_DEVOICE,        -1,-1,-1,-1 },        		
    { "HALFOP",   do_halfop,    NULL, -1,        -1,-1,-1,-1 },
    { "DEHALFOP", do_dehalfop,  NULL, -1,        -1,-1,-1,-1 },
    { "OP",       do_op,       NULL,  CHAN_HELP_OP,             -1,-1,-1,-1 },
    { "DEOP",     do_deop,     NULL,  CHAN_HELP_DEOP,           -1,-1,-1,-1 },
    { "INVITE",   do_invite,   NULL,  CHAN_HELP_INVITE,         -1,-1,-1,-1 },
    { "KICK",     do_ckick,    NULL,  CHAN_HELP_KICK,           -1,-1,-1,-1 },
//    { "BAN",      do_ban,      NULL,  CHAN_HELP_BAN,            -1,-1,-1,-1 },        
    { "UNBAN",    do_unban,    NULL,  CHAN_HELP_UNBAN,          -1,-1,-1,-1 },
    { "CLEAR",    do_clear,    NULL,  CHAN_HELP_CLEAR,          -1,-1,-1,-1 },
    { "RESET",    do_reset,    NULL,  CHAN_HELP_RESET,          -1,-1,-1,-1 },
    { "VERIFY",   do_verify,   is_services_oper,  CHAN_HELP_VERIFY,         -1,-1,-1,-1 },        
    { "IRCOPS",   do_ircops,   is_services_oper,  CHAN_HELP_IRCOPS,         -1,-1,-1,-1 },    
    { "GETPASS",  do_getpass,  is_services_oper,  -1,
		-1, CHAN_SERVADMIN_HELP_GETPASS,
		CHAN_SERVADMIN_HELP_GETPASS, CHAN_SERVADMIN_HELP_GETPASS },
    { "SUSPEND",  do_suspend,  is_services_oper,  -1,
                -1, CHAN_SERVADMIN_HELP_SUSPEND,
                CHAN_SERVADMIN_HELP_SUSPEND, CHAN_SERVADMIN_HELP_SUSPEND },
    { "UNSUSPEND",  do_unsuspend,  is_services_oper,  -1,
                -1, CHAN_SERVADMIN_HELP_UNSUSPEND,
                CHAN_SERVADMIN_HELP_UNSUSPEND, CHAN_SERVADMIN_HELP_UNSUSPEND },                                                                        
    { "FORBID",   do_forbid,   is_services_admin,  -1,
		-1, CHAN_SERVADMIN_HELP_FORBID,
		CHAN_SERVADMIN_HELP_FORBID, CHAN_SERVADMIN_HELP_FORBID },
    { "UNFORBID",   do_unforbid,   is_services_admin,  -1,
                -1, CHAN_SERVADMIN_HELP_UNFORBID,
                CHAN_SERVADMIN_HELP_UNFORBID, CHAN_SERVADMIN_HELP_UNFORBID },                                    
    { "STATUS",   do_status,   is_services_oper,  -1,
		-1, CHAN_SERVADMIN_HELP_STATUS,
		CHAN_SERVADMIN_HELP_STATUS, CHAN_SERVADMIN_HELP_STATUS },
    { "SAY",      do_say,      is_services_oper,  -1, -1, -1, -1, -1 },    
    { NULL }
};

/*************************************************************************/
/*************************************************************************/

/* Return information on memory use.  Assumes pointers are valid. */

void get_chanserv_stats(long *nrec, long *memuse)
{
    long count = 0, mem = 0;
    int i, j;
    ChannelInfo *ci;

    for (i = 0; i < 256; i++) {
	for (ci = chanlists[i]; ci; ci = ci->next) {
	    count++;
	    mem += sizeof(*ci);
	    if (ci->desc)
		mem += strlen(ci->desc)+1;
	    if (ci->url)
		mem += strlen(ci->url)+1;
	    if (ci->email)
		mem += strlen(ci->email)+1;
	    mem += ci->accesscount * sizeof(ChanAccess);
	    mem += ci->akickcount * sizeof(AutoKick);
	    for (j = 0; j < ci->akickcount; j++) {
		if (!ci->akick[j].is_nick && ci->akick[j].u.mask)
		    mem += strlen(ci->akick[j].u.mask)+1;
		if (ci->akick[j].reason)
		    mem += strlen(ci->akick[j].reason)+1;
	    }
	    if (ci->mlock_key)
		mem += strlen(ci->mlock_key)+1;
	    if (ci->last_topic)
		mem += strlen(ci->last_topic)+1;
	    if (ci->entry_message)
		mem += strlen(ci->entry_message)+1;
            if (ci->suspendby)
                mem += strlen(ci->suspendby)+1;            
            if (ci->suspendreason)
                mem += strlen(ci->suspendreason)+1;
            if (ci->forbidby)
                mem += strlen(ci->forbidby)+1;
            if (ci->forbidreason)
                mem += strlen(ci->forbidreason)+1;                                                                                                                    
	    if (ci->levels)
		mem += sizeof(*ci->levels) * CA_SIZE;
	    mem += ci->memos.memocount * sizeof(Memo);
	    for (j = 0; j < ci->memos.memocount; j++) {
		if (ci->memos.memos[j].text)
		    mem += strlen(ci->memos.memos[j].text)+1;
	    }
	}
    }
    *nrec = count;
    *memuse = mem;
}

/*************************************************************************/
/*************************************************************************/

/* ChanServ initialization. */

void cs_init(void)
{
    Command *cmd;

    cmd = lookup_cmd(cmds, "REGISTER");
    if (cmd)
	cmd->help_param1 = s_NickServ;
    cmd = lookup_cmd(cmds, "SET SECURE");
    if (cmd)
	cmd->help_param1 = s_NickServ;
    cmd = lookup_cmd(cmds, "SET SUCCESSOR");
    if (cmd)
	cmd->help_param1 = (char *)(long)CSMaxReg;
}

/*************************************************************************/

/* Main ChanServ routine. */

void chanserv(const char *source, char *buf)
{
    char *cmd, *s;
    User *u = finduser(source);

    if (!u) {
	log("%s: registro de usuario para %s no encontrado", s_ChanServ, source);
#ifndef IRC_UNDERNET_P10
/* En undernet P10, no sabremos el trio, y por lo tanto innecesario */
	privmsg(s_NickServ, source,
		getstring((NickInfo *)NULL, USER_RECORD_NOT_FOUND));
#endif		
	return;
    }

    cmd = strtok(buf, " ");

    if (!cmd) {
	return;
    } else if (stricmp(cmd, "\1PING") == 0) {
	if (!(s = strtok(NULL, "")))
	    s = "\1";
#ifdef IRC_UNDERNET_P10
	notice(s_ChanServ, u->numerico, "\1PING %s", s);
#else
        notice(s_ChanServ, source, "\1PING %s", s);
#endif	
    } else if (stricmp(cmd, "\1VERSION\1") == 0) {
        notice(s_ChanServ, source, "\1VERSION %s %s -- %s\1",
        PNAME, s_ChanServ, version_build);
            
    } else if (skeleton) {
	notice_lang(s_ChanServ, u, SERVICE_OFFLINE, s_ChanServ);
    } else if (stricmp(cmd, "AOP") == 0 || stricmp(cmd, "SOP") == 0) {
	notice_lang(s_ChanServ, u, CHAN_NO_AOP_SOP, s_ChanServ);
    } else {
	run_cmd(s_ChanServ, u, cmds, cmd);
    }
}

/*************************************************************************/

/* Load/save data files. */


#define SAFE(x) do {					\
    if ((x) < 0) {					\
	if (!forceload)					\
	    fatal("Error de lectura en %s", ChanDBName);	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void load_cs_dbase(void)
{
    dbFILE *f;
    int ver, i, j, c;
    ChannelInfo *ci, **last, *prev;
    int failed = 0;

    if (!(f = open_db(s_ChanServ, ChanDBName, "r")))
	return;

    switch (ver = get_file_version(f)) {
      case 9:
      case 8:
	for (i = 0; i < 256 && !failed; i++) {
	    int16 tmp16;
	    int32 tmp32;
	    int n_levels;
	    char *s;

	    last = &chanlists[i];
	    prev = NULL;
	    while ((c = getc_db(f)) != 0) {
		if (c != 1)
		    fatal("Formato invalido en %s", ChanDBName);
		ci = smalloc(sizeof(ChannelInfo));
		*last = ci;
		last = &ci->next;
		ci->prev = prev;
		prev = ci;
		SAFE(read_buffer(ci->name, f));
		SAFE(read_string(&s, f));
		if (s)
		    ci->founder = findnick(s);
		else
		    ci->founder = NULL;
		if (ver >= 7) {
		    SAFE(read_string(&s, f));
		    if (s)
			ci->successor = findnick(s);
		    else
			ci->successor = NULL;
		} else {
		    ci->successor = NULL;
		}
		if (ver == 5 && ci->founder != NULL) {
		    /* Channel count incorrect in version 5 files */
		    ci->founder->channelcount++;
		}
		SAFE(read_buffer(ci->founderpass, f));
		SAFE(read_string(&ci->desc, f));
		if (!ci->desc)
		    ci->desc = sstrdup("");
		SAFE(read_string(&ci->url, f));
		SAFE(read_string(&ci->email, f));
		SAFE(read_int32(&tmp32, f));
		ci->time_registered = tmp32;
		SAFE(read_int32(&tmp32, f));
		ci->last_used = tmp32;
		SAFE(read_string(&ci->last_topic, f));
		SAFE(read_buffer(ci->last_topic_setter, f));
		SAFE(read_int32(&tmp32, f));
		ci->last_topic_time = tmp32;
		SAFE(read_int32(&ci->flags, f));
#ifdef USE_ENCRYPTION
		if (!(ci->flags & (CI_ENCRYPTEDPW | CI_VERBOTEN))) {
		    if (debug)
			log("debug: %s: encrypting password for %s on load",
				s_ChanServ, ci->name);
		    if (encrypt_in_place(ci->founderpass, PASSMAX) < 0)
			fatal("%s: load database: Can't encrypt %s password!",
				s_ChanServ, ci->name);
		    ci->flags |= CI_ENCRYPTEDPW;
		}
#else
		if (ci->flags & CI_ENCRYPTEDPW) {
		    /* Bail: it makes no sense to continue with encrypted
		     * passwords, since we won't be able to verify them */
		    fatal("%s: load database: password for %s encrypted "
		          "but encryption disabled, aborting",
		          s_ChanServ, ci->name);
		}
#endif
                if (ver >= 8) {
                    SAFE(read_string(&ci->suspendby, f));
                    SAFE(read_string(&ci->suspendreason, f));                                        
                    SAFE(read_int32(&tmp32, f));
                    ci->time_suspend = tmp32;
                    SAFE(read_int32(&tmp32, f));
                    ci->time_expiresuspend = tmp32;                                                                             
                    SAFE(read_string(&ci->forbidby, f));
                    SAFE(read_string(&ci->forbidreason, f));
                } else {
                    ci->suspendby = NULL;
                    ci->suspendreason = NULL;
                    ci->time_suspend = 0;
                    ci->time_expiresuspend = 0;
                    ci->forbidby = NULL;                                                            
                    ci->forbidreason = NULL;                    
                }
		SAFE(read_int16(&tmp16, f));
		n_levels = tmp16;
		ci->levels = smalloc(2*CA_SIZE);
		reset_levels(ci);
		for (j = 0; j < n_levels; j++) {
		    if (j < CA_SIZE)
			SAFE(read_int16(&ci->levels[j], f));
		    else
			SAFE(read_int16(&tmp16, f));
		}

		SAFE(read_int16(&ci->accesscount, f));
		if (ci->accesscount) {
		    ci->access = scalloc(ci->accesscount, sizeof(ChanAccess));
		    for (j = 0; j < ci->accesscount; j++) {
			SAFE(read_int16(&ci->access[j].in_use, f));
			if (ci->access[j].in_use) {
			    SAFE(read_int16(&ci->access[j].level, f));
			    SAFE(read_string(&s, f));
			    if (s) {
				ci->access[j].ni = findnick(s);
				free(s);
			    }
			    if (ci->access[j].ni == NULL)
				ci->access[j].in_use = 0;
			}
		    }
		} else {
		    ci->access = NULL;
		}

		SAFE(read_int16(&ci->akickcount, f));
		if (ci->akickcount) {
		    ci->akick = scalloc(ci->akickcount, sizeof(AutoKick));
		    for (j = 0; j < ci->akickcount; j++) {
			SAFE(read_int16(&ci->akick[j].in_use, f));
			if (ci->akick[j].in_use) {
			    SAFE(read_int16(&ci->akick[j].is_nick, f));
			    SAFE(read_string(&s, f));
			    if (ci->akick[j].is_nick) {
				ci->akick[j].u.ni = findnick(s);
				if (!ci->akick[j].u.ni)
				    ci->akick[j].in_use = 0;
				free(s);
			    } else {
				ci->akick[j].u.mask = s;
			    }
			    SAFE(read_string(&s, f));
			    if (ci->akick[j].in_use)
				ci->akick[j].reason = s;
			    else if (s)
				free(s);
			    if (ver >= 8)
			        SAFE(read_buffer(ci->akick[j].who, f));
			    else
			        ci->akick[j].who[0] = '\0';    
			}
		    }
		} else {
		    ci->akick = NULL;
		}

		SAFE(read_int16(&ci->mlock_on, f));
		SAFE(read_int16(&ci->mlock_off, f));
		SAFE(read_int32(&ci->mlock_limit, f));
		SAFE(read_string(&ci->mlock_key, f));

		SAFE(read_int16(&ci->memos.memocount, f));
		SAFE(read_int16(&ci->memos.memomax, f));
		if (ci->memos.memocount) {
		    Memo *memos; 
		    memos = smalloc(sizeof(Memo) * ci->memos.memocount);
		    ci->memos.memos = memos;
		    for (j = 0; j < ci->memos.memocount; j++, memos++) {
			SAFE(read_int32(&memos->number, f));
			SAFE(read_int16(&memos->flags, f));
			SAFE(read_int32(&tmp32, f));
			memos->time = tmp32;
			SAFE(read_buffer(memos->sender, f));
			SAFE(read_string(&memos->text, f));
		    }
		}

		SAFE(read_string(&ci->entry_message, f));

		ci->c = NULL;
		
	    } /* while (getc_db(f) != 0) */

	    *last = NULL;

	} /* for (i) */

	break; /* case 5 and up */

      default:
	fatal("Version no soportada (%d) en %s", ver, ChanDBName);

    } /* switch (version) */

    close_db(f);

    /* Check for non-forbidden channels with no founder */
    for (i = 0; i < 256; i++) {
	ChannelInfo *next;
	for (ci = chanlists[i]; ci; ci = next) {
	    next = ci->next;
	    if (!(ci->flags & CI_VERBOTEN) && !ci->founder) {
//                canalopers(s_ChanServ, "El canal 12%s no tiene founder. Borrando..", ci->name); 
		log("%s: Carga DB: Borrando canal %s por no tener founder",
			s_ChanServ, ci->name);
		delchan(ci);
	    }
	}
    }

}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	restore_db(f);						\
	log_perror("Error de escritura en %s", ChanDBName);		\
	if (time(NULL) - lastwarn > WarningTimeout) {		\
	    canalopers(NULL, "Write error on %s: %s", ChanDBName,	\
			strerror(errno));			\
	    lastwarn = time(NULL);				\
	}							\
	return;							\
    }								\
} while (0)

void save_cs_dbase(void)
{
    dbFILE *f;
    int i, j;
    ChannelInfo *ci;
    Memo *memos;
    static time_t lastwarn = 0;

    if (!(f = open_db(s_ChanServ, ChanDBName, "w")))
	return;

    for (i = 0; i < 256; i++) {
	int16 tmp16;

	for (ci = chanlists[i]; ci; ci = ci->next) {
	    SAFE(write_int8(1, f));
	    SAFE(write_buffer(ci->name, f));
	    if (ci->founder)
		SAFE(write_string(ci->founder->nick, f));
	    else
		SAFE(write_string(NULL, f));
	    if (ci->successor)
		SAFE(write_string(ci->successor->nick, f));
	    else
		SAFE(write_string(NULL, f));
	    SAFE(write_buffer(ci->founderpass, f));
	    SAFE(write_string(ci->desc, f));
	    SAFE(write_string(ci->url, f));
	    SAFE(write_string(ci->email, f));
	    SAFE(write_int32(ci->time_registered, f));
	    SAFE(write_int32(ci->last_used, f));
	    SAFE(write_string(ci->last_topic, f));
	    SAFE(write_buffer(ci->last_topic_setter, f));
	    SAFE(write_int32(ci->last_topic_time, f));
	    SAFE(write_int32(ci->flags, f));
            SAFE(write_string(ci->suspendby, f));
            SAFE(write_string(ci->suspendreason, f));
            SAFE(write_int32(ci->time_suspend, f));
            SAFE(write_int32(ci->time_expiresuspend, f));
            SAFE(write_string(ci->forbidby, f));
            SAFE(write_string(ci->forbidreason, f));                                                
	    tmp16 = CA_SIZE;
	    SAFE(write_int16(tmp16, f));
	    for (j = 0; j < CA_SIZE; j++)
		SAFE(write_int16(ci->levels[j], f));

	    SAFE(write_int16(ci->accesscount, f));
	    for (j = 0; j < ci->accesscount; j++) {
		SAFE(write_int16(ci->access[j].in_use, f));
		if (ci->access[j].in_use) {
		    SAFE(write_int16(ci->access[j].level, f));
		    SAFE(write_string(ci->access[j].ni->nick, f));
		}
	    }

	    SAFE(write_int16(ci->akickcount, f));
	    for (j = 0; j < ci->akickcount; j++) {
		SAFE(write_int16(ci->akick[j].in_use, f));
		if (ci->akick[j].in_use) {
		    SAFE(write_int16(ci->akick[j].is_nick, f));
		    if (ci->akick[j].is_nick)
			SAFE(write_string(ci->akick[j].u.ni->nick, f));
		    else
			SAFE(write_string(ci->akick[j].u.mask, f));
		    SAFE(write_string(ci->akick[j].reason, f));
		    SAFE(write_buffer(ci->akick[j].who, f));
		}
	    }

	    SAFE(write_int16(ci->mlock_on, f));
	    SAFE(write_int16(ci->mlock_off, f));
	    SAFE(write_int32(ci->mlock_limit, f));
	    SAFE(write_string(ci->mlock_key, f));

	    SAFE(write_int16(ci->memos.memocount, f));
	    SAFE(write_int16(ci->memos.memomax, f));
	    memos = ci->memos.memos;
	    for (j = 0; j < ci->memos.memocount; j++, memos++) {
		SAFE(write_int32(memos->number, f));
		SAFE(write_int16(memos->flags, f));
		SAFE(write_int32(memos->time, f));
		SAFE(write_buffer(memos->sender, f));
		SAFE(write_string(memos->text, f));
	    }

	    SAFE(write_string(ci->entry_message, f));

	} /* for (chanlists[i]) */

	SAFE(write_int8(0, f));

    } /* for (i) */

    close_db(f);
}

#undef SAFE

/*************************************************************************/

/* Check the current modes on a channel; if they conflict with a mode lock,
 * fix them. */

void check_modes(const char *chan)
{

    Channel *c = findchan(chan);
    ChannelInfo *ci;
    char newmodes[32], *newkey = NULL;
    int32 newlimit = 0;
    char *end = newmodes; /* *destino;*/
    int modes;
    int set_limit = 0, set_key = 0;
   /* User *u;*/

    if (!c || c->bouncy_modes)
	return;

/* FIXME: old code... delete me when the -r code below is working. -TheShadow
    if (!c || !(ci = c->ci) || c->bouncy_modes)
	return;
*/

    /* Check for mode bouncing */
    if (c->server_modecount >= 3 && c->chanserv_modecount >= 3) {
	canalopers(NULL, "Desincronizacion detectada en el canal %s. "
		"Las U-Lines de los servidores tan bien configuradas?", chan);
	log("%s: Bouncy modes on channel %s", s_ChanServ, c->name);
	c->bouncy_modes = 1;
	return;
    }

    if (c->chanserv_modetime != time(NULL)) {
	c->chanserv_modecount = 0;
	c->chanserv_modetime = time(NULL);
    }
    c->chanserv_modecount++;

    if (!(ci = c->ci)) {
#ifdef IRC_TERRA
	/* Services _always_ knows who should be +r. If a channel tries to be
	 * +r and is not registered, send mode -r. This will compensate for
	 * servers that are split when mode -r is initially sent and then try
	 * to set +r when they rejoin. -TheShadow 
	 */
	if (c->mode & CMODE_r) {
	    c->mode &= ~CMODE_r;
	    send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -r", c->name);
	}
#endif
	return;
    }

/* En canales forbid, no tiene porque
 * ckequear los modos..
 */
    if (ci->flags & CI_VERBOTEN)
        return;
              

    *end++ = '+';
    modes = ~c->mode & ci->mlock_on;
    if (modes & CMODE_I) {
	*end++ = 'i';
	c->mode |= CMODE_I;
    }
    if (modes & CMODE_M) {
	*end++ = 'm';
	c->mode |= CMODE_M;
    }
    if (modes & CMODE_N) {
	*end++ = 'n';
	c->mode |= CMODE_N;
    }
    if (modes & CMODE_P) {
	*end++ = 'p';
	c->mode |= CMODE_P;
    }
    if (modes & CMODE_s) {
	*end++ = 's';
	c->mode |= CMODE_s;
    }
    if (modes & CMODE_T) {
	*end++ = 't';
	c->mode |= CMODE_T;
    }
    if (modes & CMODE_c) {
        *end++ = 'c';
        c->mode |= CMODE_c;
    }
    if (modes & CMODE_C) {
        *end++ = 'C';
        c->mode |= CMODE_C;
    }
    if (modes & CMODE_O) {
        *end++ = 'O';
        c->mode |= CMODE_O;
    }
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
    if (modes & CMODE_R) {
	*end++ = 'R';
	c->mode |= CMODE_R;
    }
    if (modes & CMODE_m) {
    	*end++ = 'M';
	c->mode |= CMODE_m;
    }
    
    if (modes & CMODE_r) {
	*end++ = 'r';
	c->mode |= CMODE_r;
    }
#endif
#ifdef IRC_HISPANO
    if (modes & CMODE_A) {
        *end++ = 'A';
        c->mode |= CMODE_A;
    }
    if (modes & CMODE_S) {
        *end++ = 'S';
        c->mode |= CMODE_S;
    }                        
#endif

    if (ci->mlock_limit && ci->mlock_limit != c->limit) {
	*end++ = 'l';
	newlimit = ci->mlock_limit;
	c->limit = newlimit;
	set_limit = 1;
    }
    if (!c->key && ci->mlock_key) {
	*end++ = 'k';
	newkey = ci->mlock_key;
	c->key = sstrdup(newkey);
	set_key = 1;
    } else if (c->key && ci->mlock_key && strcmp(c->key, ci->mlock_key) != 0) {
	char *av[3];
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -k %s", c->name, c->key);
#else
	send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -k %s", c->name, c->key);
#endif
	av[0] = sstrdup(c->name);
	av[1] = sstrdup("-k");
	av[2] = sstrdup(c->key);
	do_cmode(s_ChanServ, 3, av);
	free(av[0]);
	free(av[1]);
	free(av[2]);
	free(c->key);
	*end++ = 'k';
	newkey = ci->mlock_key;
	c->key = sstrdup(newkey);
	set_key = 1;
    }

    if (end[-1] == '+')
	end--;

    *end++ = '-';
    modes = c->mode & ci->mlock_off;
    if (modes & CMODE_I) {
	*end++ = 'i';
	c->mode &= ~CMODE_I;
    }
    if (modes & CMODE_M) {
	*end++ = 'm';
	c->mode &= ~CMODE_M;
    }
    if (modes & CMODE_N) {
	*end++ = 'n';
	c->mode &= ~CMODE_N;
    }
    if (modes & CMODE_P) {
	*end++ = 'p';
	c->mode &= ~CMODE_P;
    }
    if (modes & CMODE_s) {
	*end++ = 's';
	c->mode &= ~CMODE_s;
    }
    if (modes & CMODE_T) {
	*end++ = 't';
	c->mode &= ~CMODE_T;
    }	
    if (modes & CMODE_c) {
        *end++ = 'c';
        c->mode &= ~CMODE_c;
    }
    if (modes & CMODE_C) {
        *end++ = 'C';
        c->mode &= ~CMODE_C;
    }
    if (modes & CMODE_O) {
        *end++ = 'O';
        c->mode &= ~CMODE_O;
    }
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
    if (modes & CMODE_R) {
    	*end++ = 'R';
    	c->mode &= ~CMODE_R;
    }
    if (modes & CMODE_m) {
    	*end++ = 'M';
	c->mode &= ~CMODE_m;
    }
    if (modes & CMODE_r) {
        *end++ = 'r';
        c->mode &= ~CMODE_r;
    }                           
#endif
#ifdef IRC_HISPANO
    if (modes & CMODE_S) {
        *end++ = 'S';
        c->mode &= ~CMODE_S;
    }
    if (modes & CMODE_S) {
        *end++ = 'S';
        c->mode &= ~CMODE_S;
    }                        
#endif

    if (c->key && (ci->mlock_off & CMODE_K)) {
	*end++ = 'k';
	newkey = sstrdup(c->key);
	free(c->key);
	c->key = NULL;
	set_key = 1;
    }
    if (c->limit && (ci->mlock_off & CMODE_L)) {
	*end++ = 'l';
	c->limit = 0;
    }
#ifdef IRC_HISPANO_NO_CHUTA
    if (!(c->mode & CMODE_r)) {
  
       int hecho = 0;
   /* Elimino los modos +RAS */
       if (c->mode & CMODE_R) {
           *end++ = 'R';
           c->mode &= ~CMODE_R;    
           hecho = 1;
       }
       if (c->mode & CMODE_A) {
           *end++ = 'A';
           c->mode &= ~CMODE_A;
           hecho = 1;
       }
       if (c->mode & CMODE_S) {
           *end++ = 'S';
           c->mode &= ~CMODE_S;
           hecho = 1;
       }           
       if (hecho) {
           send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -RAS", chan);
           notice(s_ChanServ, chan, "Modo de canal no permitido");
           hecho = 0;
       }          
    }
#endif                 
                                           
           

    if (end[-1] == '-')
	end--;

/* Pongo soporte ShadowServ por si en la red hay un Guardián de modos :) 
 *
 * Zoltan - 12 Octubre 2000
 */

 
    /*if (ShadowServ) {
        u = finduser(NickShadow);  Para verificar ke esta en la red :) 
        if (u) {  
#ifdef IRC_UNDERNET_P10
            destino = u->numerico;
#else
            destino = u->nick; 
#endif        */
            if (c->key || c->limit ||
                  (c->mode & (CMODE_I | CMODE_M 
#ifdef IRC_HISPANO
                    | CMODE_R | CMODE_m | CMODE_A | CMODE_S
#elif defined (IRC_TERRA)
                    | CMODE_R
#endif
                    ))) {
              //  send_cmd(s_ShadowServ, "JOIN %s GOD", chan);
		join_shadow_chan(chan);
	    } else {
              //  send_cmd(s_ShadowServ, "PART %s", chan);
	        part_shadow_chan(chan);
            }
        /* } si no esta shadow, se ignora */
   /* }   Shadow */        
    
    if (end == newmodes)
	return;
    *end = 0;
    if (set_limit) {
	if (set_key)
	    send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s %d :%s", c->name,
				newmodes, newlimit, newkey ? newkey : "");
	else
	    send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s %d", c->name,
				newmodes, newlimit);
    } else if (set_key) {
	send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s", c->name,
				newmodes, newkey ? newkey : "");
    } else {
	send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s", c->name, newmodes);
	/* mirar fuente P10 */
    }

    if (newkey && !c->key)
	free(newkey);
/* Activamos el Private
   if (ci->flags & CI_PRIVATE)
       send_cmd(s_ChanServ, "MODE %s +p", ci->name); 
*/       
}

/*************************************************************************/

/* Check whether a user is allowed to be opped on a channel; if they
 * aren't, deop them.  If serverop is 1, the +o was done by a server.
 * Return 1 if the user is allowed to be opped, 0 otherwise. */

int check_valid_op(User *user, const char *chan, int serverop)
{
    ChannelInfo *ci = cs_findchan(chan);

    if (!ci || ((ci->flags & CI_LEAVEOPS) && !is_founder(user, ci)))
	return 1;

    if (is_oper(user->nick) || is_services_oper(user)) 
	return 1;

    if (ci->flags & CI_VERBOTEN) {
	/* check_kick() will get them out; we needn't explain. */
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -o %s", chan, user->numerico);
#else
	send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan, user->nick);
#endif
	return 0;
    }
    if (ci->flags & CI_SUSPEND) {
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -o %s", chan, user->numerico);
#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan, user->nick);    
#endif
        return 0;
    }                        
    if (serverop && time(NULL)-start_time >= CSRestrictDelay
				&& !check_access(user, ci, CA_AUTOOP)) {
/* Quitamos, que floodea mucho e innecesario
	notice_lang(s_ChanServ, user, CHAN_IS_REGISTERED, s_ChanServ);
*/	
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -o %s", chan, user->numerico);
#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan, user->nick);
#endif
	return 0;
    }

    if (check_access(user, ci, CA_AUTODEOP)) {
#if 0	/* This is probably a bad idea.  People could flood other people,
	 * and stuff. */
	notice(s_ChanServ, user->nick, CHAN_NOT_ALLOWED_OP, chan);
#endif
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -o %s", chan, user->numerico);
#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan, user->nick);
#endif
	return 0;
    }

    return 1;
}

/*************************************************************************/
/* La añado soporte para AUTODEVOICE y SECUREVOICES :)
 * Zoltan <zolty@ctv.es> 23/09/2000
 */

int check_valid_voice(User *user, const char *chan, int serverop)
{
    ChannelInfo *ci = cs_findchan(chan);

//    send_cmd(s_ChanServ, "PRIVMSG #opers :DEBUG Pilla la voz!!! eurekaaaaaaaa");

    if (is_oper(user->nick) || is_services_oper(user))
        return 1;
           
    if (ci->flags & CI_VERBOTEN) {
       /* check_kick() will get them out; we needn't explain. */   
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -v %s", chan, user->numerico);
#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -v %s", chan, user->nick);
#endif
        return 0;
    }

    if (ci->flags & CI_SUSPEND) {
//      send_cmd(s_ChanServ, "PRIVMSG #opers :DEBUG canal %s esta suspend, veamos a ver ke pasa", ci->name);
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -v %s", chan, user->numerico);
#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -v %s", chan, user->nick);
#endif
        return 0;
    }
#ifdef prueba    
    if (serverop && time(NULL)-start_time >= CSRestrictDelay
                                && !check_access(user, ci, CA_AUTOVOICE)) {
    /* Quitamos, que floodea mucho e innecesario
        notice_lang(s_ChanServ, user, CHAN_IS_REGISTERED, s_ChanServ);
     */
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -v %s", chan, user->numerico);
#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -v %s", chan, user->nick);
#endif
        return 0;
    }
    if (check_access(user, ci, CA_AUTODEVOICE)) {
#if 0   /* This is probably a bad idea.  People could flood other people,
         * and stuff. */
        notice_lang(s_ChanServ, user->nick, CHAN_NOT_ALLOWED_VOICE, chan);
#endif
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s -v %s", chan, user->numerico);
#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -v %s", chan, user->nick);
#endif
        return 0;
    }
#endif /* prueba */                         
    return 1;
 send_cmd(s_ChanServ, "PRIVMSG #opers :DEBUG cumple para tener voz");    
}                                 


/*************************************************************************/

/* Check whether a user should be opped on a channel, and if so, do it.
 * Return 1 if the user was opped, 0 otherwise.  (Updates the channel's
 * last used time if the user was opped.) */

int check_should_op(User *user, const char *chan)
{
    ChannelInfo *ci = cs_findchan(chan);

    if (is_services_oper(user)) {
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, user->nick);
  //  	return 0;
    }
    
    if (!ci || (ci->flags & CI_VERBOTEN) 
           || (ci->flags & CI_SUSPEND) || *chan == '+')
	return 0;
	            
    if ((ci->flags & CI_SECURE) && !nick_identified(user))
	return 0;

    if (check_access(user, ci, CA_AUTOOP)) {
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s +o %s", chan, user->numerico);
#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, user->nick);
#endif  
	ci->last_used = time(NULL);
	return 1;
    }
   
  if (is_services_oper(user)) {
#ifdef IRC_UNDERNET_P10
       send_cmd(MODE_SENDER(s_ChanServ), "M %s +o %s", chan, user->numerico);
#else
       send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, user->nick);
#endif   
       return 1;
   }
    return 0;
}

/*************************************************************************/

/* Check whether a user should be voiced on a channel, and if so, do it.
 * Return 1 if the user was halfop, 0 otherwise. */

int check_should_halfop(User *user, const char *chan)
{
    ChannelInfo *ci = cs_findchan(chan);

    if (!ci || (ci->flags & CI_VERBOTEN)
              || (ci->flags & CI_SUSPEND) || *chan == '+')
        return 0;

    if (check_access(user, ci, CA_AUTOHALFOP)) {
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +h %s", chan, user->nick);
        return 1;
    }

    return 0;
}

/*************************************************************************/

/* Check whether a user should be voiced on a channel, and if so, do it.
 * Return 1 if the user was voiced, 0 otherwise. */

int check_should_voice(User *user, const char *chan)
{
    ChannelInfo *ci = cs_findchan(chan);
  
    if (!ci || (ci->flags & CI_VERBOTEN) 
              || (ci->flags & CI_SUSPEND) || *chan == '+')
	return 0;

   /* if ((ci->flags & CI_SECURE) && !nick_identified(user))
	return 0;
*/

    if (check_access(user, ci, CA_AUTOVOICE)) {
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "M %s +v %s", chan, user->numerico);

#else
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +v %s", chan, user->nick);
#endif
	return 1;
    }

    return 0;
}

/*************************************************************************/

/* Tiny helper routine to get ChanServ out of a channel after it went in. */

static void timeout_leave(Timeout *to)
{
    char *chan = to->data;
    send_cmd(s_ShadowServ, "PART %s", chan);
    free(to->data);
}


/* Check whether a user is permitted to be on a channel.  If so, return 0;
 * else, kickban the user with an appropriate message (could be either
 * AKICK or restricted access) and return 1.  Note that this is called
 * _before_ the user is added to internal channel lists (so do_kick() is
 * not called).
 */

int check_kick(User *user, const char *chan)
{
    ChannelInfo *ci = cs_findchan(chan);
    AutoKick *akick;
    int i;
    NickInfo *ni;
    char *av[3], *mask;
    const char *reason;
    Timeout *t;
    int stay;

/* Por si se quiere desactivar el akick, quitar los // de abajo */
    //  return 0;

#ifdef NO_USAR
/* Los no admins no pueden entrar en el canal de admins */
//    if (stricmp(chan, "#admins" == 0) {
      if (chan == "#admins") {
        if (!is_services_admin(user)) {
            mask = create_mask(user);
            reason = getstring(user->ni, CHAN_NOT_ALLOWED_TO_JOIN);
            goto kick;
        }
    }
/* Los usuarios normales no pueden entrar en el canal de opers */
//    if (stricmp(chan, "#opers" == 0) {
      if (chan == "#opers") {
        if (!(is_oper(user->nick) || is_services_oper(user))) {
            mask = create_mask(user);
            reason = getstring(user->ni, CHAN_NOT_ALLOWED_TO_JOIN);
            goto kick;
        }
    }
#endif
    if ((ni = findnick(user->nick)) && (ni->status & NS_SUSPENDED)) {
       mask = create_mask(user);
       reason = getstring(user->ni,CHAN_NOT_ALLOWED_TO_JOIN);
       goto kick;
    }

    if (!ci)
	return 0;

    if (is_oper(user->nick) || is_services_oper(user))
	return 0;

/* Si el canal esta suspendido, no deberia rular los akicks
 * por esto lo desactivo
 */
    if (ci->flags & CI_SUSPEND)
        return 0;

    if (ci->flags & CI_VERBOTEN) {
	mask = create_mask(user);
	reason = getstring(user->ni, CHAN_MAY_NOT_BE_USED);
	goto kick;
    }


    if (nick_recognized(user))
	ni = user->ni;
    else
	ni = NULL;



    for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
	if (!akick->in_use)
	    continue;
	if ((akick->is_nick && getlink(akick->u.ni) == ni)
		|| (!akick->is_nick && match_usermask(akick->u.mask, user))
	) {
	    if (debug >= 2) {
		log("debug: %s matched akick %s", user->nick,
			akick->is_nick ? akick->u.ni->nick : akick->u.mask);
	    }
	    //mask = akick->is_nick ? create_mask(user) : sstrdup(akick->u.mask);
	    mask = create_mask(user);
	    reason = akick->reason ? akick->reason : CSAutokickReason;
	    goto kick;
	}
    }

    if (time(NULL)-start_time >= CSRestrictDelay
				&& check_access(user, ci, CA_NOJOIN)) {
	mask = create_mask(user);
	reason = getstring(user->ni, CHAN_NOT_ALLOWED_TO_JOIN);
	goto kick;
    }



    return 0;

kick:
    if (debug) {
	log("debug: channel: AutoKicking %s!%s@%s",
		user->nick, user->username, user->host);
    }
    /* Remember that the user has not been added to our channel user list
     * yet, so we check whether the channel does not exist */
    stay = !findchan(chan);
    av[0] = sstrdup(chan);
    if (stay) {
	send_cmd(s_ShadowServ, "JOIN %s", chan);
    }
    strcpy(av[0], chan);
    av[1] = sstrdup("+b");
    av[2] = mask;
    send_cmd(s_ChanServ, "MODE %s +b %s  %lu", chan, av[2], time(NULL));
    do_cmode(s_ChanServ, 3, av);
    free(av[0]);
    free(av[1]);
    free(av[2]);
#ifdef IRC_UNDERNET_P10
    send_cmd(s_ChanServ, "K %s %s :%s", chan, user->numerico, reason);
#else
    send_cmd(s_ChanServ, "KICK %s %s :%s", chan, user->nick, reason);
#endif
    if (stay) {
	t = add_timeout(CSInhabit, timeout_leave, 0);
	t->data = sstrdup(chan);
    }
    return 1;
}

/*************************************************************************/

/* Record the current channel topic in the ChannelInfo structure. */

void record_topic(const char *chan)
{
    Channel *c;
    ChannelInfo *ci;

    if (readonly)
	return;
    c = findchan(chan);
    if (!c || !(ci = c->ci))
	return;
    /* Canal suspendido o prohibido */
    if ((ci->flags & CI_SUSPEND) || (ci->flags & CI_VERBOTEN))
        return;                
    if (ci->last_topic)
	free(ci->last_topic);
    if (c->topic)
	ci->last_topic = sstrdup(c->topic);
    else
	ci->last_topic = NULL;
    strscpy(ci->last_topic_setter, c->topic_setter, NICKMAX);
    ci->last_topic_time = c->topic_time;
}

/*************************************************************************/

/* Restore the topic in a channel when it's created, if we should. */

void restore_topic(const char *chan)
{
    Channel *c = findchan(chan);
    ChannelInfo *ci;

    if (!c || !(ci = c->ci) || !(ci->flags & CI_KEEPTOPIC) || (ci->flags & CI_SUSPEND))
	return;
    /* Canal suspendido o prohibido */
    if ((ci->flags & CI_SUSPEND) || (ci->flags & CI_VERBOTEN))
        return;
    if (c->topic)
	free(c->topic);
    if (ci->last_topic) {
	c->topic = sstrdup(ci->last_topic);
	strscpy(c->topic_setter, ci->last_topic_setter, NICKMAX);
	c->topic_time = ci->last_topic_time;
    } else {
	c->topic = NULL;
	strscpy(c->topic_setter, s_ChanServ, NICKMAX);
    }
    send_cmd(MODE_SENDER(s_ChanServ), "TOPIC %s :%s", chan,
                c->topic ? c->topic : "");                   
}

/*************************************************************************/

/* See if the topic is locked on the given channel, and return 1 (and fix
 * the topic) if so. */

int check_topiclock(const char *chan)
{
    Channel *c = findchan(chan);
    ChannelInfo *ci;

    if (!c || !(ci = c->ci) || !(ci->flags & CI_TOPICLOCK) || (ci->flags & CI_SUSPEND))
	return 0;
    /* Canal suspendido o prohibido */
    if ((ci->flags & CI_SUSPEND) || (ci->flags & CI_VERBOTEN))
        return 0;                
    if (c->topic)
	free(c->topic);
    if (ci->last_topic)
	c->topic = sstrdup(ci->last_topic);
    else
	c->topic = NULL;
    strscpy(c->topic_setter, ci->last_topic_setter, NICKMAX);
    c->topic_time = ci->last_topic_time;
    send_cmd(MODE_SENDER(s_ChanServ), "TOPIC %s :%s", chan,
                c->topic ? c->topic : "");                    
    return 1;
}

/*************************************************************************/

/* Remove all channels which have expired. */

void expire_chans()
{
    ChannelInfo *ci, *next;
#ifdef IRC_TERRA
    Channel *c;
#endif
    CregInfo *cr;    
    int i;
    time_t now = time(NULL);

    if (!CSExpire)
	return;
    for (i = 0; i < 256; i++) {
	for (ci = chanlists[i]; ci; ci = next) {
	    next = ci->next;
	    if (now - ci->last_used >= CSExpire
			&& !(ci->flags & (CI_VERBOTEN | CI_NO_EXPIRE | CI_SUSPEND))) {
		log("Expirando canal %s", ci->name);
                if ((cr = cr_findcreg(ci->name))) {
                    cr->estado = 0;
                    cr->estado |= CR_EXPIRADO;
                    cr->time_motivo = time(NULL);
                }
                canalopers(s_ChanServ, "Expirando el canal 12%s", ci->name);
		delchan(ci);
	    }
	    /* AQUI LOS EXPIRES DE SUSPENDS */
	}
    }
}

/*************************************************************************/

/* Remove a (deleted or expired) nickname from all channel lists. */

void cs_remove_nick(const NickInfo *ni)
{
    int i, j;
    ChannelInfo *ci, *next;
    ChanAccess *ca;
    AutoKick *akick;
    CregInfo *cr;

    for (i = 0; i < 256; i++) {
	for (ci = chanlists[i]; ci; ci = next) {
	    next = ci->next;
	    if (ci->founder == ni) {
		if (ci->successor) {
		    NickInfo *ni2 = ci->successor;
		    if (ni2->channelcount >= ni2->channelmax) {
		        canalopers(s_ChanServ, "Borrando canal 12%s, sucesor 12%s"
		         " tiene demasiados canales", ci->name, ni2->nick);
			log("%s: Successor (%s) of %s owns too many channels, "
			    "deleting channel",
			    s_ChanServ, ni2->nick, ci->name);
                        if ((cr = cr_findcreg(ci->name))) {
                            cr->estado = 0;
                            cr->estado |= CR_EXPIRADO;
                            cr->time_motivo = time(NULL);
                        }
			delchan(ci);
		    } else {
		        canalopers(s_ChanServ, "Transferiendo el founder de 12%s,"
		        " del nick borrado  12%s al sucesor 12%s", ci->name, ni->nick, ni2->nick);
			log("%s: Transferring foundership of %s from deleted "
			    "nick %s to successor %s",
			    s_ChanServ, ci->name, ni->nick, ni2->nick);
			ci->founder = ni2;
			ci->successor = NULL;
			if (ni2->channelcount+1 > ni2->channelcount)
			    ni2->channelcount++;
		    }
		} else {
                    canalopers(s_ChanServ, "Borrando canal 12%s propiedad del"
                     " nick 12%s", ci->name, ni->nick);
		    log("%s: Deleting channel %s owned by deleted nick %s",
				s_ChanServ, ci->name, ni->nick);
                        if ((cr = cr_findcreg(ci->name))) {
                            cr->estado = 0;
                            cr->estado |= CR_EXPIRADO;
                            cr->time_motivo = time(NULL);
                        }
		    delchan(ci);
		}
		continue;
	    }
	    for (ca = ci->access, j = ci->accesscount; j > 0; ca++, j--) {
		if (ca->in_use && ca->ni == ni) {
		    ca->in_use = 0;
		    ca->ni = NULL;
		}
	    }
	    for (akick = ci->akick, j = ci->akickcount; j > 0; akick++, j--) {
		if (akick->is_nick && akick->u.ni == ni) {
		    akick->in_use = akick->is_nick = 0;
		    akick->u.ni = NULL;
		    if (akick->reason) {
			free(akick->reason);
			akick->reason = NULL;
		    }
		}
	    }
	}
    }
}

/*************************************************************************/

/* Return the ChannelInfo structure for the given channel, or NULL if the
 * channel isn't registered. */

ChannelInfo *cs_findchan(const char *chan)
{
    ChannelInfo *ci;

    for (ci = chanlists[tolower(chan[1])]; ci; ci = ci->next) {
	if (stricmp(ci->name, chan) == 0)
	    return ci;
    }
    return NULL;
}

/*************************************************************************/

/* Return 1 if the user's access level on the given channel falls into the
 * given category, 0 otherwise.  Note that this may seem slightly confusing
 * in some cases: for example, check_access(..., CA_NOJOIN) returns true if
 * the user does _not_ have access to the channel (i.e. matches the NOJOIN
 * criterion). */

int check_access(User *user, ChannelInfo *ci, int what)
{
    int level = get_access(user, ci);
    int limit = ci->levels[what];

    if (level == ACCESS_FOUNDER)
	return (what==CA_AUTODEOP || what==CA_NOJOIN) ? 0 : 1;
    /* Hacks to make flags work */
    if (what == CA_AUTODEOP && (ci->flags & CI_SECUREOPS) && level == 0)
	return 1;
    if (limit == ACCESS_INVALID)
	return 0;
    if (what == CA_AUTODEOP || what == CA_NOJOIN)
	return level <= ci->levels[what];
    else
	return level >= ci->levels[what];
}
/*************************************************************************/

 /* Sacar informacion para el info del nick en NiCK, de los registros,
  * akicks, founders de canales y identificados como founder
  *
  * Zoltan Julio 2000
  */

void registros(User *u, NickInfo *ni)
{

    ChannelInfo *ci;
    int i, y, z;
    int cfounder = 0;
    int cregistros = 0;
    int cakicks = 0;
                    
    ChanAccess *access;
    AutoKick *akick;
        
        
    for (i = 0; i < 256; i++) {
        for (ci = chanlists[i]; ci; ci = ci->next) {
                  
/* Buscar Founders de canales */
           if (ni == ci->founder) {
               privmsg(s_NickServ, u->nick, "   %-20s 12FOUNDER", ci->name);
               cfounder++;
           }

/* Buscar registros en canales */
           for (access = ci->access, y = 0; y < ci->accesscount; access++, y++) {
              if (access->ni->nick == ni->nick) {
                    privmsg(s_NickServ, u->nick, "   %-20s LEVEL 12%d" ,
                    ci->name, access->level);
                    cregistros++;
               }
           }
                                                                                                             
/* Buscar Akicks en canales */
           for (akick = ci->akick, z = 0; z < ci->akickcount; akick++, z++) {
              if (akick->u.ni->nick == ni->nick) {
                     privmsg(s_NickServ, u->nick, "   %-20s 12AKICK", ci->name);
                     cakicks++;
              }
                               
           }
        }
    }
    if (cfounder)
        privmsg(s_NickServ, u->nick, "   Total de FOUNDERS: 12%d", cfounder);
    if (cregistros)
        privmsg(s_NickServ, u->nick, "   Total de registros: 12%d", cregistros);
    if (cakicks)
        privmsg(s_NickServ, u->nick, "   Total de akicks: 12%d", cakicks);
                                          
}

/*************************************************************************/
/* ChanServ entra en los canales que tenga el STAY activado :)
 *                                                                                                
 * Zoltan Agosto 2000
 */

void join_chanserv(void)
{
    ChannelInfo *ci;
    int i;
            
    for (i = 0; i < 256; i++) { 
       for (ci = chanlists[i]; ci; ci = ci->next) {    
         if (ci->flags & CI_STAY) { 
#ifdef IRC_UNDERNET_P10
             send_cmd(s_ChanServ, "J %s", ci->name);
             send_cmd(MODE_SENDER(s_ChanServ), "M %s +o %s", ci->name, s_ChanServP10);
#else
             send_cmd(s_ShadowServ, "JOIN %s", ci->name);
             send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", ci->name, s_ChanServ);
#endif
             check_modes(ci->name);
         } 
       } 
    }
}

/*************************************************************************/
/* Si hay un guardian de modos, el shadowserv, al linkar en la red, tiene
 * que entrar en los canales que tengan ciertos modos :) 
 *
 * Zoltan - 12 Octubre 2000
 */
   
   
void join_shadow(void)
{
    ChannelInfo *ci;
    Channel *c;
    User *u=finduser(s_ShadowServ); 

    char *destino;
    int i;
    
    if (!u) {
        return;
    }        
#ifdef IRC_UNDERNET_P10
    destino = u->numerico;
#else
    destino = u->nick;
#endif

    for (i = 0; i < 256; i++) {
        for (ci = chanlists[i]; ci; ci = ci->next) {
             c = findchan(ci->name);
             if (c) {
                 if (c->key || c->limit ||
                              (c->mode & (CMODE_I | CMODE_M
#ifdef IRC_HISPANO
                              | CMODE_R | CMODE_A | CMODE_S
#elif defined (IRC_TERRA)
                              | CMODE_R
#endif
                            ))) {                 
                     send_cmd(s_ShadowServ, "JOIN %s GOD", ci->name);
/* No le necesita.., si linka, por algo sera xD 
                 } else {  
                     send_cmd(s_OperServ, "PRIVMSG %s :PART %s", destino, ci->name);
*/
                 }                                                                                        
             }    
        }
    }    
}       
/*************************************************************************/
/*********************** ChanServ private routines ***********************/
/*************************************************************************/

/* Insert a channel alphabetically into the database. */

static void alpha_insert_chan(ChannelInfo *ci)
{
    ChannelInfo *ptr, *prev;
    char *chan = ci->name;

    for (prev = NULL, ptr = chanlists[tolower(chan[1])];
			ptr != NULL && stricmp(ptr->name, chan) < 0;
			prev = ptr, ptr = ptr->next)
	;
    ci->prev = prev;
    ci->next = ptr;
    if (!prev)
	chanlists[tolower(chan[1])] = ci;
    else
	prev->next = ci;
    if (ptr)
	ptr->prev = ci;
}

/*************************************************************************/

/* Add a channel to the database.  Returns a pointer to the new ChannelInfo
 * structure if the channel was successfully registered, NULL otherwise.
 * Assumes channel does not already exist. */

static ChannelInfo *makechan(const char *chan)
{
    ChannelInfo *ci;

    ci = scalloc(sizeof(ChannelInfo), 1);
    strscpy(ci->name, chan, CHANMAX);
    ci->time_registered = time(NULL);
    reset_levels(ci);
    alpha_insert_chan(ci);
    return ci;
}

/*************************************************************************/

/* Remove a channel from the ChanServ database.  Return 1 on success, 0
 * otherwise. */

static int delchan(ChannelInfo *ci)
{
    int i;
    NickInfo *ni = ci->founder;

    if (ci->c)
	ci->c->ci = NULL;
    if (ci->next)
	ci->next->prev = ci->prev;
    if (ci->prev)
	ci->prev->next = ci->next;
    else
	chanlists[tolower(ci->name[1])] = ci->next;
    if (ci->desc)
	free(ci->desc);
    if (ci->mlock_key)
	free(ci->mlock_key);
    if (ci->last_topic)
	free(ci->last_topic);
    if (ci->suspendby)
        free (ci->suspendby);
    if (ci->suspendreason)
        free (ci->suspendreason);
    if (ci->forbidby)
        free (ci->forbidby);
    if (ci->forbidreason)
        free (ci->forbidreason);                                
    if (ci->access)
	free(ci->access);
    for (i = 0; i < ci->akickcount; i++) {
	if (!ci->akick[i].is_nick && ci->akick[i].u.mask)
	    free(ci->akick[i].u.mask);
	if (ci->akick[i].reason)
	    free(ci->akick[i].reason);
    }
    if (ci->akick)
	free(ci->akick);
    if (ci->levels)
	free(ci->levels);
    if (ci->memos.memos) {
	for (i = 0; i < ci->memos.memocount; i++) {
	    if (ci->memos.memos[i].text)
		free(ci->memos.memos[i].text);
	}
	free(ci->memos.memos);
    }
    free(ci);
    while (ni) {
	if (ni->channelcount > 0)
	    ni->channelcount--;
	ni = ni->link;
    }
    return 1;
}

/*************************************************************************/

/* Reset channel access level values to their default state. */

static void reset_levels(ChannelInfo *ci)
{
    int i;

    if (ci->levels)
	free(ci->levels);
    ci->levels = smalloc(CA_SIZE * sizeof(*ci->levels));
    for (i = 0; def_levels[i][0] >= 0; i++)
	ci->levels[def_levels[i][0]] = def_levels[i][1];
}

/*************************************************************************/

/* Does the given user have founder access to the channel? */

static int is_founder(User *user, ChannelInfo *ci)
{
    if (user->ni && (user->ni == getlink(ci->founder))) {
/*    if (user->ni == getlink(ci->founder)) { */
	if ((nick_identified(user) ||
		 (nick_recognized(user) && !(ci->flags & CI_SECURE))))
	    return 1;
    }
    if (is_identified(user, ci))
	return 1;
    return 0;
}

/*************************************************************************/

/* Has the given user password-identified as founder for the channel? */

static int is_identified(User *user, ChannelInfo *ci)
{
    struct u_chaninfolist *c;

    for (c = user->founder_chans; c; c = c->next) {
	if (c->chan == ci)
	    return 1;
    }
    return 0;
}

/*************************************************************************/

/* Return the access level the given user has on the channel.  If the
 * channel doesn't exist, the user isn't on the access list, or the channel
 * is CS_SECURE and the user hasn't IDENTIFY'd with NickServ, return 0. */

static int get_access(User *user, ChannelInfo *ci)
{
    NickInfo *ni = user->ni;
    ChanAccess *access;
    int i;

    if (!ci || !ni)
	return 0;
	
/* No he puesto el estilo de iRC-Hispano
 * Admins con nivel 32000 y Opers con nivel 16000
 * porque recibiría avisos de memos en todos los canales 
 * y no mola esto
 */
	
    if (is_founder(user, ci))
	return ACCESS_FOUNDER;
	
    if (nick_identified(user)
	|| (nick_recognized(user) && !(ci->flags & CI_SECURE))
    ) {
	for (access = ci->access, i = 0; i < ci->accesscount; access++, i++) {
	    if (access->in_use && getlink(access->ni) == ni)
		return access->level;
	}
    }
    return 0;
}

/*************************************************************************/
/*********************** ChanServ command routines ***********************/
/*************************************************************************/

static void do_credits(User *u)
{
    notice_lang(s_ChanServ, u, SERVICES_CREDITS);
}
    
/*************************************************************************/
    
static void do_help(User *u)
{
    char *cmd = strtok(NULL, "");

    if (!cmd) {
	notice_help(s_ChanServ, u, CHAN_HELP);
	if (CSExpire >= 86400)
	    notice_help(s_ChanServ, u, CHAN_HELP_EXPIRES, CSExpire/86400);
	if (is_services_oper(u))
	    notice_help(s_ChanServ, u, CHAN_SERVADMIN_HELP);
    } else if (stricmp(cmd, "LEVELS DESC") == 0) {
	int i;
	notice_help(s_ChanServ, u, CHAN_HELP_LEVELS_DESC);
	if (!levelinfo_maxwidth) {
	    for (i = 0; levelinfo[i].what >= 0; i++) {
		int len = strlen(levelinfo[i].name);
		if (len > levelinfo_maxwidth)
		    levelinfo_maxwidth = len;
	    }
	}
	for (i = 0; levelinfo[i].what >= 0; i++) {
	    notice_help(s_ChanServ, u, CHAN_HELP_LEVELS_DESC_FORMAT,
			levelinfo_maxwidth, levelinfo[i].name,
			getstring(u->ni, levelinfo[i].desc));
	}
    } else {
	help_cmd(s_ChanServ, u, cmds, cmd);
    }
}

/*************************************************************************/

static void do_register(User *u)
{
    char *chan = strtok(NULL, " ");
    char *pass = strtok(NULL, " ");
    char *desc = strtok(NULL, "");
    NickInfo *ni = u->ni;
    Channel *c;
    ChannelInfo *ci;
    CregInfo *cr;
    struct u_chaninfolist *uc;
#ifdef USE_ENCRYPTION
    char founderpass[PASSMAX+1];
#endif

    if (readonly) {
	notice_lang(s_ChanServ, u, CHAN_REGISTER_DISABLED);
	return;
    }
    if (!is_services_admin(u)) {
        notice_lang(s_ChanServ, u, ACCESS_DENIED);
        return;
    }    

    if (!desc) {
	syntax_error(s_ChanServ, u, "REGISTER", CHAN_REGISTER_SYNTAX);
    } else if ((*chan == '&') || (*chan == '+')) {
	notice_lang(s_ChanServ, u, CHAN_REGISTER_NOT_LOCAL);
    } else if (!ni) {
	notice_lang(s_ChanServ, u, CHAN_MUST_REGISTER_NICK, s_NickServ);
    } else if (!nick_recognized(u)) {
	notice_lang(s_ChanServ, u, CHAN_MUST_IDENTIFY_NICK,
		s_NickServ, s_NickServ);

    } else if ((ci = cs_findchan(chan)) != NULL) {
	if (ci->flags & CI_VERBOTEN) {
	    log("%s: Attempt to register FORBIDden channel %s by %s!%s@%s",
			s_ChanServ, ci->name, u->nick, u->username, u->host);
	    notice_lang(s_ChanServ, u, CHAN_MAY_NOT_BE_REGISTERED, chan);
	} else {
	    notice_lang(s_ChanServ, u, CHAN_ALREADY_REGISTERED, chan);
	}

    } else if ((cr = cr_findcreg(chan)) != NULL) {
        privmsg(s_ChanServ, u->nick, "El canal está en proceso por %s.", s_CregServ);

    } else if (!is_chanop(u->nick, chan) && !is_services_oper(u)) {
	notice_lang(s_ChanServ, u, CHAN_MUST_BE_CHANOP);

    } else if (ni->channelmax > 0 && ni->channelcount >= ni->channelmax) {
	notice_lang(s_ChanServ, u,
		ni->channelcount > ni->channelmax
					? CHAN_EXCEEDED_CHANNEL_LIMIT
					: CHAN_REACHED_CHANNEL_LIMIT,
		ni->channelmax);

    } else if (!(c = findchan(chan))) {
	log("%s: Channel %s not found for REGISTER", s_ChanServ, chan);
	notice_lang(s_ChanServ, u, CHAN_REGISTRATION_FAILED);

    } else if (!(ci = makechan(chan))) {
	log("%s: makechan() failed for REGISTER %s", s_ChanServ, chan);
	notice_lang(s_ChanServ, u, CHAN_REGISTRATION_FAILED);

#ifdef USE_ENCRYPTION
    } else if (strscpy(founderpass, pass, PASSMAX+1),
               encrypt_in_place(founderpass, PASSMAX) < 0) {
	log("%s: Couldn't encrypt password for %s (REGISTER)",
		s_ChanServ, chan);
	notice_lang(s_ChanServ, u, CHAN_REGISTRATION_FAILED);
	delchan(ci);
#endif

    } else {
	c->ci = ci;
	ci->c = c;
	ci->flags = CI_KEEPTOPIC | CI_SECURE;
#ifdef IRC_TERRA
	ci->mlock_on = CMODE_N | CMODE_T | CMODE_r;
#else
	ci->mlock_on = CMODE_N | CMODE_T;
#endif
	ci->memos.memomax = MSMaxMemos;
	ci->last_used = ci->time_registered;
	ci->founder = u->real_ni;
#ifdef USE_ENCRYPTION
	if (strlen(pass) > PASSMAX)
	    notice_lang(s_ChanServ, u, PASSWORD_TRUNCATED, PASSMAX);
	memset(pass, 0, strlen(pass));
	memcpy(ci->founderpass, founderpass, PASSMAX);
	ci->flags |= CI_ENCRYPTEDPW;
#else
	if (strlen(pass) > PASSMAX-1) /* -1 for null byte */
	    notice_lang(s_ChanServ, u, PASSWORD_TRUNCATED, PASSMAX-1);
	strscpy(ci->founderpass, pass, PASSMAX);
#endif
	ci->desc = sstrdup(desc);
	if (c->topic) {
	    ci->last_topic = sstrdup(c->topic);
	    strscpy(ci->last_topic_setter, c->topic_setter, NICKMAX);
	    ci->last_topic_time = c->topic_time;
	}
	ni = ci->founder;
	while (ni) {
	    if (ni->channelcount+1 > ni->channelcount)  /* Avoid wraparound */
		ni->channelcount++;
	    ni = ni->link;
	}
	ci->entry_message =  DEntryMsg;
	canalopers(s_ChanServ, "Canal 12%s registrado por 12%s", chan, u->nick);
	log("%s: Canal %s registrado por %s!%s@%s", s_ChanServ, chan,
		u->nick, u->username, u->host);
	notice_lang(s_ChanServ, u, CHAN_REGISTERED, chan, u->nick);
#ifndef USE_ENCRYPTION
	notice_lang(s_ChanServ, u, CHAN_PASSWORD_IS, ci->founderpass);
#endif
	uc = smalloc(sizeof(*uc));
	uc->next = u->founder_chans;
	uc->prev = NULL;
	if (u->founder_chans)
	    u->founder_chans->prev = uc;
	u->founder_chans = uc;
	uc->chan = ci;
	/* Implement new mode lock */
	check_modes(ci->name);

    }
}

/*************************************************************************/

int registra_con_creg(User *u, NickInfo *ni, const char *chan, const char *pass, const char *desc)
{
    Channel *c;
    User *u2;
    ChannelInfo *ci;
    struct u_chaninfolist *uc;

    if ((*chan == '&') || (*chan == '+')) {
	return 0;
    } else if (!ni) {
	return 0;
    } else if ((ci = cs_findchan(chan)) != NULL) {
	return 0;
    } else if (!(c = findchan(chan))) {
	log("%s: Channel %s not found for REGISTER", s_ChanServ, chan);
	return 0;
    } else if (!(ci = makechan(chan))) {
	log("%s: makechan() failed for REGISTER %s", s_ChanServ, chan);
	return 0;
    } else {
	c->ci = ci;
	ci->c = c;
	ci->flags = CI_KEEPTOPIC | CI_SECURE;
	ci->mlock_on = CMODE_N | CMODE_T;
	ci->memos.memomax = MSMaxMemos;
	ci->last_used = ci->time_registered;
	ci->founder = ni;
	strscpy(ci->founderpass, pass, PASSMAX);
	ci->desc = sstrdup(desc);
	if (c->topic) {
	    ci->last_topic = sstrdup(c->topic);
	    strscpy(ci->last_topic_setter, c->topic_setter, NICKMAX);
	    ci->last_topic_time = c->topic_time;
	}
	ni = ci->founder;

	if (ni->channelcount+1 > ni->channelcount)  /* Avoid wraparound */
	    ni->channelcount++;

	ci->entry_message =  DEntryMsg;
	canalopers(s_ChanServ, "Canal 12%s aprobado por %s en %s (FUNDADOR: 12%s)", chan, s_ChanServ, u->nick, ni->nick);
	log("%s: Canal %s registrado por %s!%s@%s", s_ChanServ, chan, u->nick, u->username, u->host);

        if ((u2 = finduser(ni->nick))) {
                uc = smalloc(sizeof(*uc));
                uc->next = u2->founder_chans;
                uc->prev = NULL;
                if (u2->founder_chans)
                    u2->founder_chans->prev = uc;
                u2->founder_chans = uc;
                uc->chan = ci;
        }
	/* Implement new mode lock */
	check_modes(ci->name);
	return 1;
    }
}

/*************************************************************************/

static void do_identify(User *u)
{
    char *chan = strtok(NULL, " ");
    char *pass = strtok(NULL, " ");
    ChannelInfo *ci;
    struct u_chaninfolist *uc;

    if (!pass) {
	syntax_error(s_ChanServ, u, "IDENTIFY", CHAN_IDENTIFY_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (ci->flags & CI_SUSPEND) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);            
    } else {
	int res;

	if ((res = check_password(pass, ci->founderpass)) == 1) {
	    if (!is_identified(u, ci)) {
		uc = smalloc(sizeof(*uc));
		uc->next = u->founder_chans;
		uc->prev = NULL;
		if (u->founder_chans)
		    u->founder_chans->prev = uc;
		u->founder_chans = uc;
		uc->chan = ci;
		log("%s: %s!%s@%s identified for %s", s_ChanServ,
			u->nick, u->username, u->host, ci->name);
	    }
            notice(s_ChanServ, chan, "4ATENCION!!! %s se identifica como "
                              "FUNDADOR del canal.", u->nick);                              
	    notice_lang(s_ChanServ, u, CHAN_IDENTIFY_SUCCEEDED, chan);
	} else if (res < 0) {
	    log("%s: check_password failed for %s", s_ChanServ, ci->name);
            notice(s_ChanServ, chan, "4ATENCION!!! Autentificación ilegal de %s", u->nick);
	    notice_lang(s_ChanServ, u, CHAN_IDENTIFY_FAILED);
	} else {
	    log("%s: Failed IDENTIFY for %s by %s!%s@%s",
			s_ChanServ, ci->name, u->nick, u->username, u->host);
            notice(s_ChanServ, chan, "4ATENCION!!! Autentificación con clave "
                             "incorrecta de %s.", u->nick);                              
	    notice_lang(s_ChanServ, u, PASSWORD_INCORRECT);
	    bad_password(u);
	}

    }
}

/*************************************************************************/

static void do_drop(User *u)
{
    char *chan = strtok(NULL, " ");
    char *motivo = strtok(NULL, "");
    ChannelInfo *ci;
    NickInfo *ni;
    CregInfo *cr;

#ifdef IRC_TERRA
    Channel *c;
#endif
    int is_servadmin = is_services_admin(u);

    if (readonly && !is_servadmin) {
	notice_lang(s_ChanServ, u, CHAN_DROP_DISABLED);
	return;
    }

    if (!motivo) {
	syntax_error(s_ChanServ, u, "DROP", CHAN_DROP_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (!is_servadmin & (ci->flags & CI_VERBOTEN)) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_servadmin & (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);            
    } else if (!is_servadmin && !is_identified(u, ci)) {
	notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED, s_ChanServ, chan);
    } else {
	if (readonly)  /* in this case we know they're a Services admin */
	    notice_lang(s_ChanServ, u, READ_ONLY_MODE);
	ni = ci->founder;
	if (ni) {   /* This might be NULL (i.e. after FORBID) */
	    if (ni->channelcount > 0)
		ni->channelcount--;
	    ni = getlink(ni);
	    if (ni != ci->founder && ni->channelcount > 0)
		ni->channelcount--;
	}
        canalopers(s_ChanServ, "12%s elimino el canal 12%s", u->nick, ci->name);
	log("%s: Channel %s dropped by %s!%s@%s", s_ChanServ, ci->name,
			u->nick, u->username, u->host);
        if (cr = cr_findcreg(chan)) {
            cr->estado = 0;
            cr->estado |= CR_DROPADO;
            cr->nickoper = sstrdup(u->nick);
            cr->motivo = sstrdup(motivo);
            cr->time_motivo = time(NULL);
	}
	delchan(ci);
#ifdef IRC_TERRA
	if ((c = findchan(chan))) {
	    c->mode &= ~CMODE_r;
	    send_cmd(s_ChanServ, "MODE %s -r", chan);
	}
#endif
	notice_lang(s_ChanServ, u, CHAN_DROPPED, chan);
    }
}

/*************************************************************************/

/* Main SET routine.  Calls other routines as follows:
 *	do_set_command(User *command_sender, ChannelInfo *ci, char *param);
 * The parameter passed is the first space-delimited parameter after the
 * option name, except in the case of DESC, TOPIC, and ENTRYMSG, in which
 * it is the entire remainder of the line.  Additional parameters beyond
 * the first passed in the function call can be retrieved using
 * strtok(NULL, toks).
 */
static void do_set(User *u)
{
    char *chan = strtok(NULL, " ");
    char *cmd  = strtok(NULL, " ");
    char *param;
    ChannelInfo *ci;
    int is_servoper = is_services_oper(u);

    if (readonly) {
	notice_lang(s_ChanServ, u, CHAN_SET_DISABLED);
	return;
    }

    if (cmd) {
	if (stricmp(cmd, "DESC") == 0 || stricmp(cmd, "TOPIC") == 0
	 || stricmp(cmd, "ENTRYMSG") == 0)
	    param = strtok(NULL, "");
	else
	    param = strtok(NULL, " ");
    } else {
	param = NULL;
    }

    if (!param && (!cmd || (stricmp(cmd, "SUCCESSOR") != 0 &&
                            stricmp(cmd, "URL") != 0 &&
                            stricmp(cmd, "EMAIL") != 0 &&
                            stricmp(cmd, "ENTRYMSG") != 0))) {
	syntax_error(s_ChanServ, u, "SET", CHAN_SET_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_servoper & (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);            
    } else if (!is_servoper && !check_access(u, ci, CA_SET)) {
	notice_lang(s_ChanServ, u, ACCESS_DENIED);
    } else if (stricmp(cmd, "FOUNDER") == 0) {
	if (!is_servoper && get_access(u, ci) < ACCESS_FOUNDER) {
	    notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED, s_ChanServ, chan);
	} else {
	    do_set_founder(u, ci, param);
	}
    } else if (stricmp(cmd, "SUCCESSOR") == 0) {
	if (!is_servoper && get_access(u, ci) < ACCESS_FOUNDER) {
	    notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED, s_ChanServ,chan);
	} else {
	    do_set_successor(u, ci, param);
	}
    } else if (stricmp(cmd, "PASSWORD") == 0) {
	if (!is_servoper && get_access(u, ci) < ACCESS_FOUNDER) {
	    notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED, s_ChanServ,chan);
	} else {
	    do_set_password(u, ci, param);
	}
    } else if (stricmp(cmd, "DESC") == 0) {
	do_set_desc(u, ci, param);
    } else if (stricmp(cmd, "URL") == 0) {
	do_set_url(u, ci, param);
    } else if (stricmp(cmd, "EMAIL") == 0) {
	do_set_email(u, ci, param);
    } else if (stricmp(cmd, "ENTRYMSG") == 0) {
	do_set_entrymsg(u, ci, param);
    } else if (stricmp(cmd, "TOPIC") == 0) {
	do_set_topic(u, ci, param);
    } else if (stricmp(cmd, "MLOCK") == 0) {
	do_set_mlock(u, ci, param);
    } else if (stricmp(cmd, "KEEPTOPIC") == 0) {
	do_set_keeptopic(u, ci, param);
    } else if (stricmp(cmd, "TOPICLOCK") == 0) {
	do_set_topiclock(u, ci, param);
    } else if (stricmp(cmd, "PRIVATE") == 0) {
	do_set_private(u, ci, param);
    } else if (stricmp(cmd, "SECUREVOICES") == 0) {
        do_set_securevoices(u, ci, param);            
    } else if (stricmp(cmd, "SECUREOPS") == 0) {
	do_set_secureops(u, ci, param);
    } else if (stricmp(cmd, "LEAVEOPS") == 0) {
	do_set_leaveops(u, ci, param);
    } else if (stricmp(cmd, "RESTRICTED") == 0) {
	do_set_restricted(u, ci, param);
    } else if (stricmp(cmd, "SECURE") == 0) {
	do_set_secure(u, ci, param);
    } else if (stricmp(cmd, "ISSUED") == 0) {
	do_set_opnotice(u, ci, param);
    } else if (stricmp(cmd, "DEBUG") == 0) {
        do_set_opnotice(u, ci, param);
    } else if (stricmp(cmd, "OPNOTICE") == 0) {
        do_set_opnotice(u, ci, param);
    } else if (stricmp(cmd, "STAY") == 0) {
        do_set_stay(u, ci, param);                                      
    } else if (stricmp(cmd, "NOEXPIRE") == 0) {
	do_set_noexpire(u, ci, param);
    } else {
	notice_lang(s_ChanServ, u, CHAN_SET_UNKNOWN_OPTION, strupper(cmd));
	notice_lang(s_ChanServ, u, MORE_INFO, s_ChanServ, "SET");
    }
}

/*************************************************************************/

static void do_set_founder(User *u, ChannelInfo *ci, char *param)
{
    NickInfo *ni = findnick(param), *ni0 = ci->founder;
    char *antiguo = ci->founder->nick;    

    if (!ni) {
	notice_lang(s_ChanServ, u, NICK_X_NOT_REGISTERED, param);
	return;
    }	
    if (ni->status & NS_VERBOTEN) {
        notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, param);
        return;
    }	   
    if (ni->channelcount >= ni->channelmax && !is_services_oper(u)) {
	notice_lang(s_ChanServ, u, CHAN_SET_FOUNDER_TOO_MANY_CHANS, param);
	return;
    }
    if (ni0->channelcount > 0)  /* Let's be paranoid... */
	ni0->channelcount--;
    ni0 = getlink(ni0);
    if (ni0 != ci->founder && ni0->channelcount > 0)
	ni0->channelcount--;
    ci->founder = ni;
    if (ni->channelcount+1 > ni->channelcount)
	ni->channelcount++;
    ni = getlink(ni);
    if (ni != ci->founder && ni->channelcount+1 > ni->channelcount)
	ni->channelcount++;
    log("%s: Cambiando el founder de %s a %s por %s!%s@%s", s_ChanServ,
		ci->name, param, u->nick, u->username, u->host);
    canalopers(s_ChanServ, "12%s cambia founder canal 12%s a 12%s (Antiguo founder: %s)",
           u->nick, ci->name, ci->founder->nick, antiguo);           		
    notice_lang(s_ChanServ, u, CHAN_FOUNDER_CHANGED, ci->name, param);
}

/*************************************************************************/

static void do_set_successor(User *u, ChannelInfo *ci, char *param)
{
    NickInfo *ni;

    if (param) {
	ni = findnick(param);
	if (!ni) {
	    notice_lang(s_ChanServ, u, NICK_X_NOT_REGISTERED, param);
	    return;
	}
        if (ni->status & NS_VERBOTEN) {
            notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, param);
            return;
        }	
    } else {
	ni = NULL;
    }
    ci->successor = ni;
    if (ni)
	notice_lang(s_ChanServ, u, CHAN_SUCCESSOR_CHANGED, ci->name, param);
    else
	notice_lang(s_ChanServ, u, CHAN_SUCCESSOR_UNSET, ci->name);
}

/*************************************************************************/

static void do_set_password(User *u, ChannelInfo *ci, char *param)
{
#ifdef USE_ENCRYPTION
    int len = strlen(param);
    if (len > PASSMAX) {
	len = PASSMAX;
	param[len] = 0;
	notice_lang(s_ChanServ, u, PASSWORD_TRUNCATED, PASSMAX);
    }
    if (encrypt(param, len, ci->founderpass, PASSMAX) < 0) {
	memset(param, 0, strlen(param));
	log("%s: Failed to encrypt password for %s (set)",
		s_ChanServ, ci->name);
	notice_lang(s_ChanServ, u, CHAN_SET_PASSWORD_FAILED);
	return;
    }
    memset(param, 0, strlen(param));
    notice_lang(s_ChanServ, u, CHAN_PASSWORD_CHANGED, ci->name);
#else /* !USE_ENCRYPTION */
    if (strlen(param) > PASSMAX-1) /* -1 for null byte */
	notice_lang(s_ChanServ, u, PASSWORD_TRUNCATED, PASSMAX-1);
    strscpy(ci->founderpass, param, PASSMAX);
    notice_lang(s_ChanServ, u, CHAN_PASSWORD_CHANGED_TO,
		ci->name, ci->founderpass);
#endif /* USE_ENCRYPTION */
    if (get_access(u, ci) < ACCESS_FOUNDER) {
        canalopers(s_ChanServ, "12%s ha usado SET PASSWORD (como OPER) en el canal 12%s",
                  u->nick, ci->name);
	log("%s: %s!%s@%s set password as Services admin for %s",
		s_ChanServ, u->nick, u->username, u->host, ci->name);
    }
}

/*************************************************************************/

static void do_set_desc(User *u, ChannelInfo *ci, char *param)
{
    if (ci->desc)
	free(ci->desc);
    ci->desc = sstrdup(param);
    notice_lang(s_ChanServ, u, CHAN_DESC_CHANGED, ci->name, param);
}

/*************************************************************************/

static void do_set_url(User *u, ChannelInfo *ci, char *param)
{
    if (ci->url)
	free(ci->url);
    if (param) {
	ci->url = sstrdup(param);
	notice_lang(s_ChanServ, u, CHAN_URL_CHANGED, ci->name, param);
    } else {
	ci->url = NULL;
	notice_lang(s_ChanServ, u, CHAN_URL_UNSET, ci->name);
    }
}

/*************************************************************************/

static void do_set_email(User *u, ChannelInfo *ci, char *param)
{
    if (ci->email)
	free(ci->email);
    if (param) {
	ci->email = sstrdup(param);
	notice_lang(s_ChanServ, u, CHAN_EMAIL_CHANGED, ci->name, param);
    } else {
	ci->email = NULL;
	notice_lang(s_ChanServ, u, CHAN_EMAIL_UNSET, ci->name);
    }
}

/*************************************************************************/

static void do_set_entrymsg(User *u, ChannelInfo *ci, char *param)
{
    if (ci->entry_message)
	free(ci->entry_message);
    if (param) {
	ci->entry_message = sstrdup(param);
	notice_lang(s_ChanServ, u, CHAN_ENTRY_MSG_CHANGED, ci->name, param);
    } else {
	ci->entry_message = NULL;
	notice_lang(s_ChanServ, u, CHAN_ENTRY_MSG_UNSET, ci->name);
    }
}

/*************************************************************************/

static void do_set_topic(User *u, ChannelInfo *ci, char *param)
{
    Channel *c = ci->c;

    if (!c) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, ci->name);
	return;
    }
    if (ci->last_topic)
	free(ci->last_topic);
    if (*param)
	ci->last_topic = sstrdup(param);
    else
	ci->last_topic = NULL;
    if (c->topic) {
	free(c->topic);
	c->topic_time--;	/* to get around TS8 */
    } else
	c->topic_time = time(NULL);
    if (*param)
	c->topic = sstrdup(param);
    else
	c->topic = NULL;
    strscpy(ci->last_topic_setter, u->nick, NICKMAX);
    strscpy(c->topic_setter, u->nick, NICKMAX);
    ci->last_topic_time = c->topic_time;
    send_cmd(MODE_SENDER(s_ChanServ), "TOPIC %s :%s",
                    ci->name, param);                    
}

/*************************************************************************/

static void do_set_mlock(User *u, ChannelInfo *ci, char *param)
{
    char *s, modebuf[32], *end, c;
    int add = -1;	/* 1 if adding, 0 if deleting, -1 if neither */
    int newlock_on = 0, newlock_off = 0, newlock_limit = 0;
    char *newlock_key = NULL;

    while (*param) {
	if (*param != '+' && *param != '-' && add < 0) {
	    param++;
	    continue;
	}
	switch ((c = *param++)) {
	  case '+':
	    add = 1;
	    break;
	  case '-':
	    add = 0;
	    break;
	  case 'i':
	    if (add) {
		newlock_on |= CMODE_I;
		newlock_off &= ~CMODE_I;
	    } else {
		newlock_off |= CMODE_I;
		newlock_on &= ~CMODE_I;
	    }
	    break;
	  case 'k':
	    if (add) {
		if (!(s = strtok(NULL, " "))) {
		    notice_lang(s_ChanServ, u, CHAN_SET_MLOCK_KEY_REQUIRED);
		    return;
		}
		if (newlock_key)
		    free(newlock_key);
		newlock_key = sstrdup(s);
		newlock_off &= ~CMODE_K;
	    } else {
		if (newlock_key) {
		    free(newlock_key);
		    newlock_key = NULL;
		}
		newlock_off |= CMODE_K;
	    }
	    break;
	  case 'l':
	    if (add) {
		if (!(s = strtok(NULL, " "))) {
		    notice_lang(s_ChanServ, u, CHAN_SET_MLOCK_LIMIT_REQUIRED);
		    return;
		}
		if (atol(s) <= 0) {
		    notice_lang(s_ChanServ, u, CHAN_SET_MLOCK_LIMIT_POSITIVE);
		    return;
		}
		newlock_limit = atol(s);
		newlock_off &= ~CMODE_L;
	    } else {
		newlock_limit = 0;
		newlock_off |= CMODE_L;
	    }
	    break;
	  case 'm':
	    if (add) {
		newlock_on |= CMODE_M;
		newlock_off &= ~CMODE_M;
	    } else {
		newlock_off |= CMODE_M;
		newlock_on &= ~CMODE_M;
	    }
	    break;
	  case 'n':
	    if (add) {
		newlock_on |= CMODE_N;
		newlock_off &= ~CMODE_N;
	    } else {
		newlock_off |= CMODE_N;
		newlock_on &= ~CMODE_N;
	    }
	    break;
	  case 'p':
	    if (add) {
		newlock_on |= CMODE_P;
		newlock_off &= ~CMODE_P;
	    } else {
		newlock_off |= CMODE_P;
		newlock_on &= ~CMODE_P;
	    }
	    break;
	  case 's':
	    if (add) {
		newlock_on |= CMODE_s;
		newlock_off &= ~CMODE_s;
	    } else {
		newlock_off |= CMODE_s;
		newlock_on &= ~CMODE_s;
	    }
	    break;
	  case 't':
	    if (add) {
		newlock_on |= CMODE_T;
		newlock_off &= ~CMODE_T;
	    } else {
		newlock_off |= CMODE_T;
		newlock_on &= ~CMODE_T;
	    }
	    break;
          case 'c':
            if (add) {
                newlock_on |= CMODE_c;
                newlock_off &= ~CMODE_c;
            } else {
                newlock_off |= CMODE_c;
                newlock_on &= ~CMODE_c;
            }
            break;
          case 'C':
            if (add) {
                newlock_on |= CMODE_C;
                newlock_off &= ~CMODE_C;
            } else {
                newlock_off |= CMODE_C;
                newlock_on &= ~CMODE_C;
            }
            break;
          case 'O':
            if (add) {
                newlock_on |= CMODE_O;
                newlock_off &= ~CMODE_O;
            } else {
                newlock_off |= CMODE_O;
                newlock_on &= ~CMODE_O;
            }
            break;
/* #if defined (IRC_HISPANO_NOCHUTA) || defined (IRC_TERRA) */
	  case 'R':
	    if (add) {
		newlock_on |= CMODE_R;
		newlock_off &= ~CMODE_R;
	    } else {
		newlock_off |= CMODE_R;
	    	newlock_on &= ~CMODE_R;
	    }
	    break;
	  case 'M':
	    if (add) {
	        newlock_on |= CMODE_m;
		newlock_off &= ~CMODE_m;
	    } else {
	        newlock_off |= CMODE_m;
		newlock_on &= ~CMODE_m;
	    }
	    break;
/* #endif */
#ifdef IRC_HISPANO_NOCHUTA
          case 'A':
            if (add) {
                newlock_on |= CMODE_A;
                newlock_off &= ~CMODE_A;
            } else {
                newlock_off |= CMODE_A;
                newlock_on &= ~CMODE_A;
            }
            break;
                                                                                                                          
          case 'S':
            if (add) {
                newlock_on |= CMODE_S;
                newlock_off &= ~CMODE_S;
            } else {
                newlock_off |= CMODE_S;
                newlock_on &= ~CMODE_S;
            }
            break;
                                                                                                                                                                                                                                                    
#endif
	  default:
	    notice_lang(s_ChanServ, u, CHAN_SET_MLOCK_UNKNOWN_CHAR, c);
	    break;
	} /* switch */
    } /* while (*param) */

    /* Now that everything's okay, actually set the new mode lock. */
    ci->mlock_on = newlock_on;
    ci->mlock_off = newlock_off;
    ci->mlock_limit = newlock_limit;
    if (ci->mlock_key)
	free(ci->mlock_key);
    ci->mlock_key = newlock_key;

    /* Tell the user about it. */
    end = modebuf;
    *end = 0;
    if (ci->mlock_on)
	end += snprintf(end, sizeof(modebuf)-(end-modebuf), 
			"+%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				(ci->mlock_on & CMODE_I) ? "i" : "",
				(ci->mlock_key         ) ? "k" : "",
				(ci->mlock_limit       ) ? "l" : "",
				(ci->mlock_on & CMODE_M) ? "m" : "",
				(ci->mlock_on & CMODE_N) ? "n" : "",
				(ci->mlock_on & CMODE_P) ? "p" : "",
				(ci->mlock_on & CMODE_s) ? "s" : "",
				(ci->mlock_on & CMODE_T) ? "t" : "",
                                (ci->mlock_on & CMODE_c) ? "c" : "",
                                (ci->mlock_on & CMODE_C) ? "C" : "",
                                (ci->mlock_on & CMODE_O) ? "O" : "",
/* #if defined (IRC_HISPANO_NOCHUTA) || defined (IRC_TERRA) */
				(ci->mlock_on & CMODE_m) ? "M" : "",
				(ci->mlock_on & CMODE_R) ? "R" : ""
/* #else
				""
#endif
#ifdef IRC_HISPANO_NOCHUTA
                               ,(ci->mlock_on & CMODE_A) ? "A" : "",
                                (ci->mlock_on & CMODE_S) ? "S" : ""                                
#else */
			       ,"",
			        ""	
/* #endif */
                                );
    if (ci->mlock_off)
	end += snprintf(end, sizeof(modebuf)-(end-modebuf), 
			"-%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				(ci->mlock_off & CMODE_I) ? "i" : "",
				(ci->mlock_off & CMODE_K) ? "k" : "",
				(ci->mlock_off & CMODE_L) ? "l" : "",
				(ci->mlock_off & CMODE_M) ? "m" : "",
				(ci->mlock_off & CMODE_N) ? "n" : "",
				(ci->mlock_off & CMODE_P) ? "p" : "",
				(ci->mlock_off & CMODE_s) ? "s" : "",
				(ci->mlock_off & CMODE_T) ? "t" : "",
                                (ci->mlock_off & CMODE_c) ? "c" : "",
                                (ci->mlock_off & CMODE_C) ? "C" : "",
                                (ci->mlock_off & CMODE_O) ? "O" : "",
/* #if defined (IRC_HISPANO_NOCHUTA) || defined (IRC_TERRA) */                   
				(ci->mlock_off & CMODE_m) ? "M" : "",
				(ci->mlock_off & CMODE_R) ? "R" : ""
/* #else
				""
#endif
#ifdef IRC_HISPANO_NOCHUTA
                               ,(ci->mlock_off & CMODE_R) ? "A" : "",
                                (ci->mlock_off & CMODE_R) ? "S" : ""
#else */
                               ,"",
                                ""
/* #endif */
                                );
    if (*modebuf) {
	notice_lang(s_ChanServ, u, CHAN_MLOCK_CHANGED, ci->name, modebuf);
    } else {
	notice_lang(s_ChanServ, u, CHAN_MLOCK_REMOVED, ci->name);
    }

    /* Implement the new lock. */
    check_modes(ci->name);
}

/*************************************************************************/

static void do_set_keeptopic(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_KEEPTOPIC;
	notice_lang(s_ChanServ, u, CHAN_SET_KEEPTOPIC_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_KEEPTOPIC;
	notice_lang(s_ChanServ, u, CHAN_SET_KEEPTOPIC_OFF);
    } else {
	syntax_error(s_ChanServ, u, "SET KEEPTOPIC", CHAN_SET_KEEPTOPIC_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_topiclock(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_TOPICLOCK;
	notice_lang(s_ChanServ, u, CHAN_SET_TOPICLOCK_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_TOPICLOCK;
	notice_lang(s_ChanServ, u, CHAN_SET_TOPICLOCK_OFF);
    } else {
	syntax_error(s_ChanServ, u, "SET TOPICLOCK", CHAN_SET_TOPICLOCK_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_private(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_PRIVATE;
	notice_lang(s_ChanServ, u, CHAN_SET_PRIVATE_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_PRIVATE;
	notice_lang(s_ChanServ, u, CHAN_SET_PRIVATE_OFF);
    } else {
	syntax_error(s_ChanServ, u, "SET PRIVATE", CHAN_SET_PRIVATE_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_securevoices(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
        ci->flags |= CI_SECUREVOICES;
        notice_lang(s_ChanServ, u, CHAN_SET_SECUREOPS_ON);
        ci->flags &= ~CI_SUSPEND;
    } else if (stricmp(param, "OFF") == 0) {
        ci->flags &= ~CI_SECUREVOICES;
        notice_lang(s_ChanServ, u, CHAN_SET_SECUREOPS_OFF);
    } else {
        syntax_error(s_ChanServ, u, "SET SECUREVOICES", CHAN_SET_SECUREOPS_SYNTAX);
    }
}                                                       

/*************************************************************************/

static void do_set_secureops(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_SECUREOPS;
	notice_lang(s_ChanServ, u, CHAN_SET_SECUREOPS_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_SECUREOPS;
	notice_lang(s_ChanServ, u, CHAN_SET_SECUREOPS_OFF);
    } else {
	syntax_error(s_ChanServ, u, "SET SECUREOPS", CHAN_SET_SECUREOPS_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_leaveops(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_LEAVEOPS;
	notice_lang(s_ChanServ, u, CHAN_SET_LEAVEOPS_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_LEAVEOPS;
	notice_lang(s_ChanServ, u, CHAN_SET_LEAVEOPS_OFF);
    } else {
	syntax_error(s_ChanServ, u, "SET LEAVEOPS", CHAN_SET_LEAVEOPS_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_restricted(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_RESTRICTED;
	if (ci->levels[CA_NOJOIN] < 0)
	    ci->levels[CA_NOJOIN] = 0;
	notice_lang(s_ChanServ, u, CHAN_SET_RESTRICTED_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_RESTRICTED;
	if (ci->levels[CA_NOJOIN] >= 0)
	    ci->levels[CA_NOJOIN] = -1;
	notice_lang(s_ChanServ, u, CHAN_SET_RESTRICTED_OFF);
    } else {
	syntax_error(s_ChanServ,u,"SET RESTRICTED",CHAN_SET_RESTRICTED_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_secure(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_SECURE;
	notice_lang(s_ChanServ, u, CHAN_SET_SECURE_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_SECURE;
	notice_lang(s_ChanServ, u, CHAN_SET_SECURE_OFF);
    } else {
	syntax_error(s_ChanServ, u, "SET SECURE", CHAN_SET_SECURE_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_opnotice(User *u, ChannelInfo *ci, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_OPNOTICE;
	notice_lang(s_ChanServ, u, CHAN_SET_OPNOTICE_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_OPNOTICE;
	notice_lang(s_ChanServ, u, CHAN_SET_OPNOTICE_OFF);
    } else {
	syntax_error(s_ChanServ, u, "SET ISSUED", CHAN_SET_OPNOTICE_SYNTAX);
    }
}
/*************************************************************************/

static void do_set_stay(User *u, ChannelInfo *ci, char *param)
{
    privmsg(s_ChanServ, u->nick, "Opción STAY deshabilitada.");
    return;
    
/*    if (stricmp(param, "ON") == 0) {
        ci->flags |= CI_STAY;
        send_cmd(s_ChanServ, "JOIN %s ", ci->name);
#ifdef IRC_UNDERNET_P10
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", ci->name, s_ChanServP10);
#else        
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", ci->name, s_ChanServ);
#endif
        notice_lang(s_ChanServ, u, CHAN_SET_STAY_ON);
        ci->flags &= ~CI_SUSPEND;
    } else if (stricmp(param, "OFF") == 0) {
        ci->flags &= ~CI_STAY;
        send_cmd(s_ChanServ, "PART %s", ci->name);
        notice_lang(s_ChanServ, u, CHAN_SET_STAY_OFF);
    } else {
        syntax_error(s_ChanServ, u, "SET STAY", CHAN_SET_STAY_SYNTAX);
    }*/
}                                                    
                                            
/*************************************************************************/

static void do_set_noexpire(User *u, ChannelInfo *ci, char *param)
{
    if (!is_services_admin(u)) {
	notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	return;
    }
    if (stricmp(param, "ON") == 0) {
	ci->flags |= CI_NO_EXPIRE;
	notice_lang(s_ChanServ, u, CHAN_SET_NOEXPIRE_ON, ci->name);
    } else if (stricmp(param, "OFF") == 0) {
	ci->flags &= ~CI_NO_EXPIRE;
	notice_lang(s_ChanServ, u, CHAN_SET_NOEXPIRE_OFF, ci->name);
    } else {
	syntax_error(s_ChanServ, u, "SET NOEXPIRE", CHAN_SET_NOEXPIRE_SYNTAX);
    }
}

/*************************************************************************/

/* `last' is set to the last index this routine was called with
 * `perm' is incremented whenever a permission-denied error occurs
 */
#ifdef CAPADO 
static int access_del(User *u, ChanAccess *access, int *perm, int uacc)
{
    if (!access->in_use)
	return 0;
    if (uacc <= access->level) {
	(*perm)++;
	return 0;
    }
    access->ni = NULL;
    access->in_use = 0;
    return 1;
}

static int access_del_callback(User *u, int num, va_list args)
{
    ChannelInfo *ci = va_arg(args, ChannelInfo *);
    int *last = va_arg(args, int *);
    int *perm = va_arg(args, int *);
    int uacc = va_arg(args, int);
    if (num < 1 || num > ci->accesscount)
	return 0;
    *last = num;
    return access_del(u, &ci->access[num-1], perm, uacc);
}
#endif

static int access_list(User *u, int index, ChannelInfo *ci, int *sent_header)
{
    ChanAccess *access = &ci->access[index];
    NickInfo *ni;
    char *s;

    if (!access->in_use)
	return 0;
    if (!*sent_header) {
	notice_lang(s_ChanServ, u, CHAN_ACCESS_LIST_HEADER, ci->name);
	*sent_header = 1;
    }
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
    if ((ni = findnick(access->ni->nick)) && is_services_oper(u))    
#else    
    if ((ni = findnick(access->ni->nick))
			&& !(getlink(ni)->flags & NI_HIDE_MASK))
#endif			
	s = ni->last_usermask;
    else
	s = NULL;
    notice_lang(s_ChanServ, u, CHAN_ACCESS_LIST_FORMAT,
		index+1, access->level, access->ni->nick,
		s ? " (" : "", s ? s : "", s ? ")" : "");
    return 1;
}

static int access_list_callback(User *u, int num, va_list args)
{
    ChannelInfo *ci = va_arg(args, ChannelInfo *);
    int *sent_header = va_arg(args, int *);
    if (num < 1 || num > ci->accesscount)
	return 0;
    return access_list(u, num-1, ci, sent_header);
}

static void do_delaccess(User *u)
{
	char *chan = strtok(NULL, " ");
	ChannelInfo *ci;
	NickInfo *ni;
	int i;
	ChanAccess *access;

	if (!chan) {
		syntax_error(s_ChanServ, u, "DELACCESS", CHAN_SYNTAX_DELACCESS);
	} else if (!(ni = findnick(u->nick))) {
		notice_lang(s_ChanServ, u, NICK_NOT_REGISTERED);
	} else if (!(ci = cs_findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
	} else if (ci->flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	} else if (ci->flags & CI_SUSPEND) {
		notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
	} else if ((get_access(u, ci) <= 0) || (check_access(u, ci, CA_NOJOIN))){
		notice_lang(s_ChanServ, u, CHAN_MSG_DELACCESS_NEG, chan);
	} else if (ni->status & NS_SUSPENDED) {
		notice_lang(s_ChanServ, u, NICK_X_SUSPENDED, u->nick);
	} else if (is_founder(u, ci)) {
		privmsg(s_ChanServ, u->nick, "Como fundador, no puedes hacerte un DELACCESS");
		
	} else {

	for (i = 0; i < ci->accesscount; i++) {
		if (ci->access[i].ni == ni)
		    break;
	 }

		access = &ci->access[i];
		access->ni = NULL;
		access->in_use = 0;
		if (ci->flags & CI_OPNOTICE) {
                    notice(s_ChanServ, chan, "%s borra de %s a %s.",
                    u->nick, chan, u->nick);
                }
		notice_lang(s_ChanServ, u, CHAN_MSG_DELACCESS_SUCCESS, chan);
	}
}
/****************************************************************************************************/
static void do_access(User *u)
{
    char *chan = strtok(NULL, " ");
    char *cmd  = strtok(NULL, " ");
    char *nick = strtok(NULL, " ");
    char *s    = strtok(NULL, " ");
    ChannelInfo *ci;
    NickInfo *ni;
    short level = 0, ulev;
    int i;
    int is_list = (cmd && (stricmp(cmd, "LIST") == 0 || stricmp(cmd, "COUNT") == 0));
    ChanAccess *access;

    /* If LIST/COUNT, we don't *require* any parameters, but we can take any.
     * If DEL, we require a nick and no level.
     * Else (ADD), we require a level (which implies a nick). */
    if (!cmd || ((is_list || !stricmp(cmd, "CLEAR")) ? 0 :
			(stricmp(cmd,"DEL")==0) ? (!nick || s) : !s)) {
	syntax_error(s_ChanServ, u, "ACCESS", CHAN_ACCESS_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if ((stricmp(cmd,"LIST") != 0) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
    } else if (((is_list && !check_access(u, ci, CA_ACCESS_LIST))
                    || (!is_list && !check_access(u, ci, CA_ACCESS_CHANGE)))
                    && !is_services_oper(u))
                
    {
	notice_lang(s_ChanServ, u, ACCESS_DENIED);

    } else if (stricmp(cmd, "ADD") == 0) {

	if (readonly) {
	    notice_lang(s_ChanServ, u, CHAN_ACCESS_DISABLED);
	    return;
	}

	level = atoi(s);
	ulev = get_access(u, ci);
	if ((level >= ulev) && !is_services_oper(u)) {
	    notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	    return;
	}
	if (level == 0) {
	    notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_NONZERO);
	    return;
	} else if (level <= ACCESS_INVALID || level >= ACCESS_FOUNDER) {
	    notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_RANGE,
			ACCESS_INVALID+1, ACCESS_FOUNDER-1);
	    return;
	}
	ni = findnick(nick);
	if (is_a_service(nick)) {
            notice_lang(s_ChanServ, u, NICK_IS_A_SERVICE, nick);
	    return;
	} else if (!ni) {
	    notice_lang(s_ChanServ, u, CHAN_ACCESS_NICKS_ONLY);
	    return;
	} else if (ni->status & NS_VERBOTEN) {
	    notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, ni->nick);
	    return;
	}
	for (access = ci->access, i = 0; i < ci->accesscount; access++, i++) {
	    if (access->ni == ni) {
		/* Don't allow lowering from a level >= ulev */
		if ((access->level >= ulev) && !is_services_oper(u)) {
		    notice_lang(s_ChanServ, u, PERMISSION_DENIED);
		    return;
		}
		if (access->level == level) {
		    notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_UNCHANGED,
			access->ni->nick, chan, level);
		    return;
		}
		access->level = level;
		notice_lang(s_ChanServ, u, CHAN_ACCESS_LEVEL_CHANGED,
			access->ni->nick, chan, level);
                if (!(stricmp(u->nick, ni->nick) == 0) && (ni->status & NS_IDENTIFIED)) 
	          privmsg(s_ChanServ, ni->nick, "%s cambia tu nivel de %s a %d.", u->nick, chan, level);
		
		if (ci->flags & CI_OPNOTICE) {
                    notice(s_ChanServ, chan, "%s cambia en %s nivel de %s a %d.",
                                  u->nick, chan, access->ni->nick, level);
                }                                                           
                return;
	    }
	}
	for (i = 0; i < ci->accesscount; i++) {
	    if (!ci->access[i].in_use)
		break;
	}
	if (i == ci->accesscount) {
	    if (i < CSAccessMax) {
		ci->accesscount++;
		ci->access =
		    srealloc(ci->access, sizeof(ChanAccess) * ci->accesscount);
	    } else {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_REACHED_LIMIT,
			CSAccessMax);
		return;
	    }
	}
	access = &ci->access[i];
	access->ni = ni;
	access->in_use = 1;
	access->level = level;
	notice_lang(s_ChanServ, u, CHAN_ACCESS_ADDED,
		access->ni->nick, chan, level);
	if (!(stricmp(u->nick, ni->nick) == 0) && (ni->status & NS_IDENTIFIED)) 
	          privmsg(s_ChanServ, ni->nick, "%s te registra en %s con nivel %d.", u->nick, chan, level);
	

       if (ci->flags & CI_OPNOTICE) {
           notice(s_ChanServ, chan, "%s registra en %s a %s con nivel %d.",
                      u->nick, chan, access->ni->nick, level);
       }
                                                     
    } else if (stricmp(cmd, "DEL") == 0) {

	if (readonly) {
	    notice_lang(s_ChanServ, u, CHAN_ACCESS_DISABLED);
	    return;
	}

	/* Special case: is it a number/list?  Only do search if it isn't. */
#ifdef CAPADO	
	if (isdigit(*nick) && strspn(nick, "1234567890,-") == strlen(nick)) {
	    int count, deleted, last = -1, perm = 0;
	    deleted = process_numlist(nick, &count, access_del_callback, u,
					ci, &last, &perm, get_access(u, ci));
	    if (!deleted) {
		if (perm) {
		    notice_lang(s_ChanServ, u, PERMISSION_DENIED);
		} else if (count == 1) {
		    notice_lang(s_ChanServ, u, CHAN_ACCESS_NO_SUCH_ENTRY,
				last, ci->name);
		} else {
		    notice_lang(s_ChanServ, u, CHAN_ACCESS_NO_MATCH, ci->name);
		}
	    } else if (deleted == 1) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_DELETED_ONE, ci->name);
	    } else {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_DELETED_SEVERAL,
				deleted, ci->name);
	    }
	} else {
#endif	
	    ni = findnick(nick);
	    if (!ni) {
		notice_lang(s_ChanServ, u, NICK_X_NOT_REGISTERED, nick);
		return;
	    }
	    for (i = 0; i < ci->accesscount; i++) {
		if (ci->access[i].ni == ni)
		    break;
	    }
	    if (i == ci->accesscount) {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_NOT_FOUND, nick, chan);
		return;
	    }
	    access = &ci->access[i];
	    if ((get_access(u, ci) <= access->level) && !is_services_oper(u)) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	    } else {
		notice_lang(s_ChanServ, u, CHAN_ACCESS_DELETED,
				access->ni->nick, ci->name);
               
	       if (!(stricmp(u->nick, ni->nick) == 0) && (ni->status & NS_IDENTIFIED)) 
	          privmsg(s_ChanServ, ni->nick, "%s elimina tu registro de %s.", u->nick, chan);
	       
	        if (ci->flags & CI_OPNOTICE) {
                    notice(s_ChanServ, chan, "%s borra de %s a %s.",
                    u->nick, chan, access->ni->nick);
                }                                                                   
		access->ni = NULL;
		access->in_use = 0;
	    }
//	}

    } else if (stricmp(cmd, "LIST") == 0) {
	int sent_header = 0;

	if (ci->accesscount == 0) {
	    notice_lang(s_ChanServ, u, CHAN_ACCESS_LIST_EMPTY, chan);
	    return;
	}
	if (nick && strspn(nick, "1234567890,-") == strlen(nick)) {
	    process_numlist(nick, NULL, access_list_callback, u, ci,
								&sent_header);
	} else {
	    for (i = 0; i < ci->accesscount; i++) {
		if (nick && ci->access[i].ni
			 && !match_wild_nocase(nick, ci->access[i].ni->nick))
		    continue;
		access_list(u, i, ci, &sent_header);
	    }
//            privmsg(s_ChanServ, u->nick, "Total de registros del canal 12%s: 12%d",
//                          ci->name, ci->accesscount);
	}
	if (!sent_header)
	    notice_lang(s_ChanServ, u, CHAN_ACCESS_NO_MATCH, chan);

    } else if (stricmp(cmd, "COUNT") ==0) {
            privmsg(s_ChanServ, u->nick, "Número de registros del canal 12%s: 12%d",
                                ci->name, ci->accesscount);
                                    

    } else if (stricmp(cmd, "CLEAR") ==0) {
            
        if (readonly) {
            notice_lang(s_ChanServ, u, CHAN_ACCESS_DISABLED);
            return;
        }
        
        if (!is_founder(u, ci)) {
            notice_lang(s_ChanServ, u, PERMISSION_DENIED);
            return;
        }
        
        free(ci->access);
        ci->access = NULL;
        ci->accesscount = 0;
        
        //notice_lang(s_ChanServ, u, CHAN_ACCESS_CLEAR);
        privmsg(s_ChanServ, u->nick, "Los registros del canal %s han sido borrados", ci->name);
            

    } else {
	syntax_error(s_ChanServ, u, "ACCESS", CHAN_ACCESS_SYNTAX);
    }
}


/*************************************************************************/

/* `last' is set to the last index this routine was called with */

static int akick_del(User *u, AutoKick *akick)
{
    if (!akick->in_use)
	return 0;
    if (akick->is_nick) {
	akick->u.ni = NULL;
    } else {
	free(akick->u.mask);
	akick->u.mask = NULL;
    }
    if (akick->reason) {
	free(akick->reason);
	akick->reason = NULL;
    }
    akick->in_use = akick->is_nick = 0;
    return 1;
}
#ifdef CAPADO
static int akick_del_callback(User *u, int num, va_list args)
{
    ChannelInfo *ci = va_arg(args, ChannelInfo *);
    int *last = va_arg(args, int *);
    if (num < 1 || num > ci->akickcount)
	return 0;
    *last = num;
    return akick_del(u, &ci->akick[num-1]);
}
#endif

static int akick_list(User *u, int index, ChannelInfo *ci, int *sent_header)
{
    AutoKick *akick = &ci->akick[index];
    char buf[BUFSIZE], buf2[BUFSIZE];

    if (!akick->in_use)
	return 0;
    if (!*sent_header) {
	notice_lang(s_ChanServ, u, CHAN_AKICK_LIST_HEADER, ci->name);
	*sent_header = 1;
    }
    if (akick->is_nick) {
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
        if (is_services_oper(u))
#else    
	if (akick->u.ni->flags & NI_HIDE_MASK)
#endif	
	    strscpy(buf, akick->u.ni->nick, sizeof(buf));
	else
	    snprintf(buf, sizeof(buf), "%s (%s)",
			akick->u.ni->nick, akick->u.ni->last_usermask);
    } else {
	strscpy(buf, akick->u.mask, sizeof(buf));
    }
    if (akick->reason)
	snprintf(buf2, sizeof(buf2), " (%s)", akick->reason);
    else
	*buf2 = 0;
    notice_lang(s_ChanServ, u, CHAN_AKICK_LIST_FORMAT, index+1, buf, buf2);
    return 1;
}

static int akick_list_callback(User *u, int num, va_list args)
{
    ChannelInfo *ci = va_arg(args, ChannelInfo *);
    int *sent_header = va_arg(args, int *);
    if (num < 1 || num > ci->akickcount)
	return 0;
    return akick_list(u, num-1, ci, sent_header);
}


static void do_akick(User *u)
{
    char *chan   = strtok(NULL, " ");
    char *cmd    = strtok(NULL, " ");
    char *mask   = strtok(NULL, " ");
    char *reason = strtok(NULL, "");
    ChannelInfo *ci;
    int i;
    AutoKick *akick;    

    if (!cmd || (!mask && 
		(stricmp(cmd, "ADD") == 0 || stricmp(cmd, "DEL") == 0))) {
	syntax_error(s_ChanServ, u, "AKICK", CHAN_AKICK_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
   } else if ((stricmp(cmd,"LIST") != 0) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);          
    } else if (!check_access(u, ci, CA_AKICK) && !is_services_oper(u)) {
	if (ci->founder && getlink(ci->founder) == u->ni)
	    notice_lang(s_ChanServ, u, CHAN_IDENTIFY_REQUIRED, s_ChanServ,chan);
	else
	    notice_lang(s_ChanServ, u, ACCESS_DENIED);

    } else if (stricmp(cmd, "ADD") == 0) {

	NickInfo *ni = findnick(mask);
	char *nick, *user, *host;
//	User *u2 = NULL;


	if (readonly) {
	    notice_lang(s_ChanServ, u, CHAN_AKICK_DISABLED);
	    return;
	}

	if (!ni) {
	    split_usermask(mask, &nick, &user, &host);
	    mask = smalloc(strlen(nick)+strlen(user)+strlen(host)+3);
	    sprintf(mask, "%s!%s@%s", nick, user, host);
	    free(nick);
	    free(user);
	    free(host);

	} else if (ni->status & NS_VERBOTEN) {
	    notice_lang(s_ChanServ, u, NICK_X_FORBIDDEN, ni->nick);
	    return;
	}

	for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
	    if (!akick->in_use)
		continue;
	    if (akick->is_nick ? akick->u.ni == ni
	                       : stricmp(akick->u.mask,mask) == 0) {
		notice_lang(s_ChanServ, u, CHAN_AKICK_ALREADY_EXISTS,
			akick->is_nick ? akick->u.ni->nick : akick->u.mask,
			chan);
		return;
	    }
	}

	for (i = 0; i < ci->akickcount; i++) {
	    if (!ci->akick[i].in_use)
		break;
	}

	if (i == ci->akickcount) {
	    if (ci->akickcount >= CSAutokickMax) {
		notice_lang(s_ChanServ, u, CHAN_AKICK_REACHED_LIMIT,
			CSAutokickMax);
		return;
	    }
	    ci->akickcount++;
	    ci->akick = srealloc(ci->akick, sizeof(AutoKick) * ci->akickcount);

	}
	akick = &ci->akick[i];
	akick->in_use = 1;
	if (ni) {
	    akick->is_nick = 1;
	    akick->u.ni = ni;
	} else {
	    akick->is_nick = 0;
	    akick->u.mask = mask;
	}

	if (reason) {
            char buf[BUFSIZE];
            snprintf(buf, sizeof(buf), "(%s) -> %s", u->nick, reason);
	    send_cmd(MODE_SENDER(s_ChanServ), "KICK %s %s :(%s) -> %s", chan, mask, u->nick, reason);
	    akick->reason = sstrdup(buf);
	} else {
            char buf[BUFSIZE];
            snprintf(buf, sizeof(buf), "(%s) -> %s", u->nick, CSAutokickReason);
            send_cmd(MODE_SENDER(s_ChanServ), "KICK %s %s :(%s) -> %s", chan, mask, u->nick, CSAutokickReason);
	    akick->reason = sstrdup(buf);
        }
//        strscpy(akick->who, u->nick, NICKMAX);
/* Kickeamos al momento :) */
#ifdef DESACTIVADO
        User *u2;
	u2 = finduser(mask);
        if (u2) {
            if (!is_services_oper(u2)) {
#ifdef IRC_UNDERNET_P10
                char *destino = u2->numerico;
#else
                char *destino = u2->nick;
#endif
                char *av[3];
                send_cmd(MODE_SENDER(s_ChanServ), "KICK %s %s :%s", chan, destino, akick->reason);
                av[0] = sstrdup(chan);
                av[1] = sstrdup(destino);
                av[2] = sstrdup(akick->reason);
                do_kick(s_ChanServ, 3, av);
//                free(av[0]);
                free(av[1]);
//                free(av[2]);
            }
        }
#endif
	notice_lang(s_ChanServ, u, CHAN_AKICK_ADDED, mask, chan);

    } else if (stricmp(cmd, "DEL") == 0) {
NickInfo *ni = findnick(mask);

	if (readonly) {
	    notice_lang(s_ChanServ, u, CHAN_AKICK_DISABLED);
	    return;
	}
#ifdef CAPADO
	/* Special case: is it a number/list?  Only do search if it isn't. */
	if (isdigit(*mask) && strspn(mask, "1234567890,-") == strlen(mask)) {
	    int count, deleted, last = -1;
	    deleted = process_numlist(mask, &count, akick_del_callback, u,
					ci, &last);
	    if (!deleted) {
		if (count == 1) {
		    notice_lang(s_ChanServ, u, CHAN_AKICK_NO_SUCH_ENTRY,
				last, ci->name);
		} else {
		    notice_lang(s_ChanServ, u, CHAN_AKICK_NO_MATCH, ci->name);
		}
	    } else if (deleted == 1) {
		notice_lang(s_ChanServ, u, CHAN_AKICK_DELETED_ONE, ci->name);
	    } else {
		notice_lang(s_ChanServ, u, CHAN_AKICK_DELETED_SEVERAL,
				deleted, ci->name);
	    }
	} else {
#endif
//	    NickInfo *ni = findnick(mask);

	    for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
		if (!akick->in_use)
		    continue;
		if ((akick->is_nick && akick->u.ni == ni)
		 || (!akick->is_nick && stricmp(akick->u.mask, mask) == 0))
		    break;
	    }
	    if (i == ci->akickcount) {
		notice_lang(s_ChanServ, u, CHAN_AKICK_NOT_FOUND, mask, chan);
		return;
	    }
	    notice_lang(s_ChanServ, u, CHAN_AKICK_DELETED, mask, chan);
	    akick_del(u, akick);
//	}

    } else if (stricmp(cmd, "LIST") == 0) {
	int sent_header = 0;

	if (ci->akickcount == 0) {
	    notice_lang(s_ChanServ, u, CHAN_AKICK_LIST_EMPTY, chan);
	    return;
	}
	if (mask && isdigit(*mask) &&
			strspn(mask, "1234567890,-") == strlen(mask)) {
	    process_numlist(mask, NULL, akick_list_callback, u, ci,
								&sent_header);
	} else {
	    for (akick = ci->akick, i = 0; i < ci->akickcount; akick++, i++) {
		if (!akick->in_use)
		    continue;
		if (mask) {
		    if (!akick->is_nick && !match_wild(mask, akick->u.mask))
			continue;
		    if (akick->is_nick && !match_wild(mask, akick->u.ni->nick))
			continue;
		}
		akick_list(u, i, ci, &sent_header);
	    }
	}
	if (!sent_header)
	    notice_lang(s_ChanServ, u, CHAN_AKICK_NO_MATCH, chan);

    } else if (stricmp(cmd, "ENFORCE") == 0) {
	Channel *c = findchan(ci->name);
	struct c_userlist *cu = NULL;
	struct c_userlist *next;
	char *argv[3];
	int i;
	int count = 0;

	if (!c) {
	    notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, ci->name);
	    return;
	}

	for (i = 0; i < 3; i++) {
	    switch (i) {
		case 0: cu = c->chanops; break;
		case 1: cu = c->voices; break;
		case 2: cu = c->users; break;
	    }

	    while (cu) {
		next = cu->next;
		if (check_kick(cu->user, c->name)) {
		    argv[0] = sstrdup(c->name);
		    argv[1] = sstrdup(cu->user->nick);
		    argv[2] = sstrdup(CSAutokickReason);

		    do_kick(s_ChanServ, 3, argv);

		    free(argv[2]);
		    free(argv[1]);
		    free(argv[0]);

		    count++;
		}
		cu = next;
	    }
	}

	notice_lang(s_ChanServ, u, CHAN_AKICK_ENFORCE_DONE, chan, count);

    } else {
	syntax_error(s_ChanServ, u, "AKICK", CHAN_AKICK_SYNTAX);
    }
}

/*************************************************************************/

static void do_levels(User *u)
{
    char *chan = strtok(NULL, " ");
    char *cmd  = strtok(NULL, " ");
    char *what = strtok(NULL, " ");
    char *s    = strtok(NULL, " ");
    ChannelInfo *ci;
    short level;
    int i;

    /* If SET, we want two extra parameters; if DIS[ABLE], we want only
     * one; else, we want none.
     */
    if (!cmd || ((stricmp(cmd,"SET")==0) ? !s :
			(strnicmp(cmd,"DIS",3)==0) ? (!what || s) : !!what)) {
	syntax_error(s_ChanServ, u, "LEVELS", CHAN_LEVELS_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if ((stricmp(cmd,"LIST") != 0) && (ci->flags & CI_SUSPEND)) {
            notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);            	
/*    } else if (!is_founder(u, ci) && !is_services_oper(u)) { */
    } else if ((!is_founder(u, ci) && !is_services_oper(u)) && !(stricmp(cmd, "LIST") == 0)) {
	notice_lang(s_ChanServ, u, ACCESS_DENIED);

    } else if (stricmp(cmd, "SET") == 0) {
	level = atoi(s);
	if (level <= ACCESS_INVALID || level >= ACCESS_FOUNDER) {
	    notice_lang(s_ChanServ, u, CHAN_LEVELS_RANGE,
			ACCESS_INVALID+1, ACCESS_FOUNDER-1);
	    return;
	}
	for (i = 0; levelinfo[i].what >= 0; i++) {
	    if (stricmp(levelinfo[i].name, what) == 0) {
		ci->levels[levelinfo[i].what] = level;
		notice_lang(s_ChanServ, u, CHAN_LEVELS_CHANGED,
			levelinfo[i].name, chan, level);
		return;
	    }
	}
	notice_lang(s_ChanServ, u, CHAN_LEVELS_UNKNOWN, what, s_ChanServ);

    } else if (stricmp(cmd, "DIS") == 0 || stricmp(cmd, "DISABLE") == 0) {
	for (i = 0; levelinfo[i].what >= 0; i++) {
	    if (stricmp(levelinfo[i].name, what) == 0) {
		ci->levels[levelinfo[i].what] = ACCESS_INVALID;
		notice_lang(s_ChanServ, u, CHAN_LEVELS_DISABLED,
			levelinfo[i].name, chan);
		return;
	    }
	}
	notice_lang(s_ChanServ, u, CHAN_LEVELS_UNKNOWN, what, s_ChanServ);

    } else if (stricmp(cmd, "LIST") == 0) {
	int i;
	notice_lang(s_ChanServ, u, CHAN_LEVELS_LIST_HEADER, chan);
	if (!levelinfo_maxwidth) {
	    for (i = 0; levelinfo[i].what >= 0; i++) {
		int len = strlen(levelinfo[i].name);
		if (len > levelinfo_maxwidth)
		    levelinfo_maxwidth = len;
	    }
	}
	for (i = 0; levelinfo[i].what >= 0; i++) {
	    int j = ci->levels[levelinfo[i].what];
	    if (j == ACCESS_INVALID) {
		j = levelinfo[i].what;
		if (j == CA_AUTOOP || j == CA_AUTODEOP
		|| j == CA_AUTOVOICE || j == CA_AUTOHALFOP || j == CA_AUTODEVOICE || j == CA_NOJOIN)
		{
		    notice_lang(s_ChanServ, u, CHAN_LEVELS_LIST_DISABLED,
				levelinfo_maxwidth, levelinfo[i].name);
		} else {
		    notice_lang(s_ChanServ, u, CHAN_LEVELS_LIST_DISABLED,
				levelinfo_maxwidth, levelinfo[i].name);
		}
	    } else {
		notice_lang(s_ChanServ, u, CHAN_LEVELS_LIST_NORMAL,
				levelinfo_maxwidth, levelinfo[i].name, j);
	    }
	}

    } else if (stricmp(cmd, "RESET") == 0) {
	reset_levels(ci);
	notice_lang(s_ChanServ, u, CHAN_LEVELS_RESET, chan);

    } else {
	syntax_error(s_ChanServ, u, "LEVELS", CHAN_LEVELS_SYNTAX);
    }
}

/*************************************************************************/

/* SADMINS and users, who have identified for a channel, can now cause it's
 * Enstry Message and Successor to be displayed by supplying the ALL parameter.
 * Syntax: INFO channel [ALL]
 * -TheShadow (29 Mar 1999)
 */

static void do_info(User *u)
{
    char *chan = strtok(NULL, " ");
    char *param = strtok(NULL, " ");
    ChannelInfo *ci;
    NickInfo *ni;
    char buf[BUFSIZE], *end;
    struct tm *tm;
    int need_comma = 0;
    const char *commastr = getstring(u->ni, COMMA_SPACE);
    int is_servoper = is_services_oper(u);
    int show_all = 0;

    if (!chan) {
        syntax_error(s_ChanServ, u, "INFO", CHAN_INFO_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
	 if (is_services_oper(u)) {
                privmsg(s_ChanServ, u->nick, "Forbideado por: 12%s", ci->forbidby);
    		privmsg(s_ChanServ, u->nick, "Motivo: 12%s", ci->forbidreason);
           }
    } else if (!ci->founder) {
        /* Paranoia... this shouldn't be able to happen */
        delchan(ci);
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else {

        /* Should we show all fields? Only for sadmins and identified users */
        if (param && stricmp(param, "ALL") == 0 && 
	    				(is_identified(u, ci) || is_servoper))
            show_all = 1;

        notice_lang(s_ChanServ, u, CHAN_INFO_HEADER, chan);
        if (ci->flags & CI_SUSPEND) {
            notice_lang(s_ChanServ, u, CHAN_INFO_SUSPENDED);
            privmsg(s_ChanServ, u->nick, "Motivo: 12%s", ci->suspendreason);
            if (show_all) {
                char timebuf[32], expirebuf[256];
//                time_t now = time(NULL);
                
                privmsg(s_ChanServ, u->nick, "Suspendido por: 12%s", ci->suspendby);
   tm = localtime(&ci->time_suspend);                        
   strftime_lang(timebuf, sizeof(timebuf), u, STRFTIME_DATE_TIME_FORMAT, tm);
                privmsg(s_ChanServ, u->nick, "Fecha de la suspensión: 12%s", timebuf);                
/*    
                if (ci->time_expiresuspend == 0) {
                    snprintf(expirebuf, sizeof(expirebuf),
                                getstring(u->ni, OPER_AKILL_NO_EXPIRE));

                } else if (ci->time_expiresuspend <= now) {                                 
                    snprintf(expirebuf, sizeof(expirebuf),
                                getstring(u->ni, OPER_AKILL_EXPIRES_SOON));                
                } else {
                    expires_in_lang(expirebuf, sizeof(expirebuf), u,
                                ci->time_expiresuspend - now + 59);
                }                 
                */
                privmsg(s_ChanServ, u->nick, "La suspensión expira en: 12%s", expirebuf);
            }
            
        } else {
            notice_lang(s_ChanServ, u, CHAN_INFO_ACTIVED);
        }                                                 
        ni = ci->founder;
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
        if (ni->last_usermask && is_servoper)        
#else        
        if (ni->last_usermask && (is_servoper || !(ni->flags & NI_HIDE_MASK)))
#endif
        {
            notice_lang(s_ChanServ, u, CHAN_INFO_FOUNDER, ni->nick,
                        ni->last_usermask);
        } else {
            notice_lang(s_ChanServ, u, CHAN_INFO_NO_FOUNDER, ni->nick);
        }

//	if (show_all) {
	    ni = ci->successor;
	    if (ni) {
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
               if (ni->last_usermask && is_servoper) {
#else	    
		if (ni->last_usermask && (is_servoper ||
				!(ni->flags & NI_HIDE_MASK))) {
#endif				
		    notice_lang(s_ChanServ, u, CHAN_INFO_SUCCESSOR, ni->nick, 
				ni->last_usermask);
		} else {
		    notice_lang(s_ChanServ, u, CHAN_INFO_NO_SUCCESSOR,
				ni->nick);
		}
	    }
//	}

	notice_lang(s_ChanServ, u, CHAN_INFO_DESCRIPTION, ci->desc);
	tm = localtime(&ci->time_registered);
	strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
	notice_lang(s_ChanServ, u, CHAN_INFO_TIME_REGGED, buf);
	tm = localtime(&ci->last_used);
	strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
	notice_lang(s_ChanServ, u, CHAN_INFO_LAST_USED, buf);
	if (ci->last_topic) {
	    notice_lang(s_ChanServ, u, CHAN_INFO_LAST_TOPIC, ci->last_topic);
	    notice_lang(s_ChanServ, u, CHAN_INFO_TOPIC_SET_BY,
			ci->last_topic_setter);
	}

	if (ci->entry_message && show_all)
	    notice_lang(s_ChanServ, u, CHAN_INFO_ENTRYMSG, ci->entry_message);
	if (ci->url)
	    notice_lang(s_ChanServ, u, CHAN_INFO_URL, ci->url);
	if (ci->email)
	    notice_lang(s_ChanServ, u, CHAN_INFO_EMAIL, ci->email);
	end = buf;
	*end = 0;
	if (ci->flags & CI_PRIVATE) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s",
			getstring(u->ni, CHAN_INFO_OPT_PRIVATE));
	    need_comma = 1;
	}
	if (ci->flags & CI_KEEPTOPIC) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
			need_comma ? commastr : "",
			getstring(u->ni, CHAN_INFO_OPT_KEEPTOPIC));
	    need_comma = 1;
	}
	if (ci->flags & CI_TOPICLOCK) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
			need_comma ? commastr : "",
			getstring(u->ni, CHAN_INFO_OPT_TOPICLOCK));
	    need_comma = 1;
	}
/**        if (ci->flags & CI_SECUREVOICES) {
            end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
                        need_comma ? commastr : "",
                        getstring(u->ni, "Secure Voices"));
            need_comma = 1;
        }                                                                                        
*/	if (ci->flags & CI_SECUREOPS) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
			need_comma ? commastr : "",
			getstring(u->ni, CHAN_INFO_OPT_SECUREOPS));
	    need_comma = 1;
	}
	if (ci->flags & CI_LEAVEOPS) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
			need_comma ? commastr : "",
			getstring(u->ni, CHAN_INFO_OPT_LEAVEOPS));
	    need_comma = 1;
	}
	if (ci->flags & CI_RESTRICTED) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
			need_comma ? commastr : "",
			getstring(u->ni, CHAN_INFO_OPT_RESTRICTED));
	    need_comma = 1;
	}
	if (ci->flags & CI_SECURE) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
			need_comma ? commastr : "",
			getstring(u->ni, CHAN_INFO_OPT_SECURE));
	    need_comma = 1;
	}
	notice_lang(s_ChanServ, u, CHAN_INFO_OPTIONS,
		*buf ? buf : getstring(u->ni, CHAN_INFO_OPT_NONE));
	end = buf;
	*end = 0;
	if (ci->mlock_on || ci->mlock_key || ci->mlock_limit)
	    end += snprintf(end, sizeof(buf)-(end-buf), "+%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				(ci->mlock_on & CMODE_I) ? "i" : "",
				(ci->mlock_key         ) ? "k" : "",
				(ci->mlock_limit       ) ? "l" : "",
				(ci->mlock_on & CMODE_M) ? "m" : "",
				(ci->mlock_on & CMODE_N) ? "n" : "",
				(ci->mlock_on & CMODE_P) ? "p" : "",
				(ci->mlock_on & CMODE_s) ? "s" : "",
				(ci->mlock_on & CMODE_T) ? "t" : "",
                                (ci->mlock_on & CMODE_c) ? "c" : "",
                                (ci->mlock_on & CMODE_C) ? "C" : "",
                                (ci->mlock_on & CMODE_O) ? "O" : "",
/* #if defined (IRC_HISPANO) || defined (IRC_TERRA) */
	    			(ci->mlock_on & CMODE_m) ? "M" : "",
				(ci->mlock_on & CMODE_R) ? "R" : ""
/* #else
	    			""
#endif
#ifdef IRC_HISPANO
                                ,
                                (ci->mlock_on & CMODE_A) ? "A" : "",
                                (ci->mlock_on & CMODE_S) ? "S" : ""
#else */
                                ,
                                "",
                                ""
/* #endif */
                                );
	if (ci->mlock_off)
	    end += snprintf(end, sizeof(buf)-(end-buf), "-%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				(ci->mlock_off & CMODE_I) ? "i" : "",
				(ci->mlock_off & CMODE_K) ? "k" : "",
				(ci->mlock_off & CMODE_L) ? "l" : "",
				(ci->mlock_off & CMODE_M) ? "m" : "",
				(ci->mlock_off & CMODE_N) ? "n" : "",
				(ci->mlock_off & CMODE_P) ? "p" : "",
				(ci->mlock_off & CMODE_s) ? "s" : "",
				(ci->mlock_off & CMODE_T) ? "t" : "",
                                (ci->mlock_off & CMODE_c) ? "c" : "",
                                (ci->mlock_off & CMODE_C) ? "C" : "",
                                (ci->mlock_off & CMODE_O) ? "O" : "",
/* #if defined (IRC_HISPANO) || defined (IRC_TERRA) */
	    			(ci->mlock_off & CMODE_m) ? "M" : "",
				(ci->mlock_off & CMODE_R) ? "R" : ""
/* #else
	    			""
#endif
#ifdef IRC_HISPANO
                                ,
                                (ci->mlock_off & CMODE_A) ? "A" : "",
                                (ci->mlock_off & CMODE_S) ? "S" : ""
#else */
                                ,
                                "",
                                ""
/* #endif */
                                );
	if (*buf)
	    notice_lang(s_ChanServ, u, CHAN_INFO_MODE_LOCK, buf);

        if ((ci->flags & CI_NO_EXPIRE) && show_all)
	                notice_lang(s_ChanServ, u, CHAN_INFO_NO_EXPIRE);
    }
}

/*************************************************************************/

static void do_list(User *u)
{
    char *pattern = strtok(NULL, " ");
    char *keyword;
    ChannelInfo *ci;
    int nchans, i;
    char buf[BUFSIZE];
    int is_servoper = is_services_oper(u);
    int16 matchflags = 0;

    if (CSListOpersOnly && (!u || !(u->mode & UMODE_O))) {
	notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	return;
    }

    if (!pattern) {
	syntax_error(s_ChanServ, u, "LIST", CHAN_LIST_SYNTAX);
    } else {
	nchans = 0;
	
	while (is_servoper && (keyword = strtok(NULL, " "))) {
	    if (stricmp(keyword, "FORBIDDEN") == 0)
	        matchflags |= CI_VERBOTEN;
	    if (stricmp(keyword, "SUSPENDED") == 0)
	        matchflags |= CI_SUSPEND;
	    if (stricmp(keyword, "NOEXPIRE") == 0)
	        matchflags |= CI_NO_EXPIRE;
	}                
	notice_lang(s_ChanServ, u, CHAN_LIST_HEADER, pattern);
	for (i = 0; i < 256; i++) {
	    for (ci = chanlists[i]; ci; ci = ci->next) {
		if (!is_servoper & ((ci->flags & CI_PRIVATE)
		                              || (ci->flags & CI_VERBOTEN)))
		    continue;
		if ((matchflags != 0) && !(ci->flags & matchflags))
		    continue;
		        
		snprintf(buf, sizeof(buf), "%-20s  %s", ci->name,
						ci->desc ? ci->desc : "");
		if (stricmp(pattern, ci->name) == 0 ||
					match_wild_nocase(pattern, buf)) {
		    if (++nchans <= CSListMax) {
			char noexpire_char = ' ';
			if (is_servoper && (ci->flags & CI_NO_EXPIRE))
			    noexpire_char = '!';
		        if (ci->flags & CI_VERBOTEN) {
		            snprintf(buf, sizeof(buf), "%-20s [PROHIBIDO]", ci->name);
		        }
                        if (ci->flags & CI_SUSPEND) {
                            snprintf(buf, sizeof(buf), "%-20s [SUSPENDIDO]", ci->name);
                        }		                       
			privmsg(s_ChanServ, u->nick, "  %c%s",
						noexpire_char, buf);
		    }
		}
	    }
	}
	notice_lang(s_ChanServ, u, CHAN_LIST_END,
			nchans>CSListMax ? CSListMax : nchans, nchans);
    }

}

/*************************************************************************/

static void do_invite(User *u)
{
    char *chan = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;

    if (!chan) {
	syntax_error(s_ChanServ, u, "INVITE", CHAN_INVITE_SYNTAX);
    } else if (!(c = findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (!(ci = c->ci)) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_services_oper(u) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);             	
    } else if (!u || (!check_access(u, ci, CA_INVITE)  && !is_services_oper(u))) {
	notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
        if (ci->flags & CI_OPNOTICE) {
            notice(s_ChanServ, chan, "%s se invita al canal.", u->nick);
        }
       	send_cmd(s_ChanServ, "INVITE %s %s", u->nick, chan);
    }
}

/*************************************************************************/
 
static void do_voice(User *u)
{
    char *chan = strtok(NULL, " ");
    char *voice_params = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;
               
    if (!chan || !voice_params) {
        syntax_error(s_ChanServ, u, "VOICE", CHAN_OP_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
/*    } else if (!(ci = c->ci) && is_services_oper(u)) {
        send_cmd(s_ChanServ, "MODE %s +v %s", chan, voice_params); */
    } else if (!(ci = c->ci)) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_services_oper(u) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
    } else if (!u || (!check_access(u, ci, CA_VOICEDEVOICE) && 
            !is_services_oper(u))) {
        notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
        char *av[3];
        User *u2 = finduser(voice_params);
        char *destino;
        if (u2) {
#ifdef IRC_UNDERNET_P10  
            destino = u2->numerico;     
#else            
            destino = u2->nick;
#endif            
            send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +v %s", chan, destino);
            av[0] = chan;
            av[1] = sstrdup("+v");
            av[2] = destino;
            do_cmode(s_ChanServ, 3, av);
            free(av[1]);     
            if (ci->flags & CI_OPNOTICE) {
                notice(s_ChanServ, chan, "%s hace VOICE a %s.",
                         u->nick, voice_params);         
            }     
        } else {
            notice_lang(s_ChanServ, u, NICK_X_NOT_IN_USE, voice_params);
        }    
    }
                                     
}
                                                                                                                                                       
/*************************************************************************/

static void do_devoice(User *u)
{
    char *chan = strtok(NULL, " ");
    char *devoice_params = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;
 
    if (!chan || !devoice_params) {
        syntax_error(s_ChanServ, u, "DEVOICE", CHAN_DEOP_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
/*    } else if (!(ci = c->ci) && is_services_oper(u)) {
        send_cmd(s_ChanServ, "MODE %s -v %s", chan, devoice_params); */
    } else if (!(ci = c->ci)) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_services_oper(u) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
    } else if (!u || (!check_access(u, ci, CA_VOICEDEVOICE) && 
              !is_services_oper(u))) {
        notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
        char *av[3];
        User *u2 = finduser(devoice_params);
        char *destino;
        if (u2) {
#ifdef IRC_UNDERNET_P10
            destino = u2->numerico;
#else
            destino = u2->nick;
#endif    
            send_cmd(s_ChanServ, "MODE %s -v %s", chan, destino);
            av[0] = chan;
            av[1] = sstrdup("-v");
            av[2] = destino;
            do_cmode(s_ChanServ, 3, av);
            free(av[1]);        
            if (ci->flags & CI_OPNOTICE) {              
                notice(s_ChanServ, chan, "%s hace DEVOICE a %s.",
                        u->nick, devoice_params);
            }    
        } else {
            notice_lang(s_ChanServ, u, NICK_X_NOT_IN_USE, devoice_params);
        }                                                                                   
    }
    
}
                                                                                                      
/*************************************************************************/

static void do_halfop(User *u)
{
    char *chan = strtok(NULL, " ");
    char *voice_params = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;

    if (!chan || !voice_params) {
        syntax_error(s_ChanServ, u, "HALFOP", CHAN_OP_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
/*    } else if (!(ci = c->ci) && is_services_oper(u)) {
        send_cmd(s_ChanServ, "MODE %s +v %s", chan, voice_params); */
    } else if (!(ci = c->ci)) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_services_oper(u) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
    } else if (!u || (!check_access(u, ci, CA_HALFOPDEHALFOP) &&
            !is_services_oper(u))) {
        notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
        char *av[3];
        User *u2 = finduser(voice_params);
        char *destino;
        if (u2) {
            destino = u2->nick;
            send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +h %s", chan, destino);
            av[0] = chan;
            av[1] = sstrdup("+h");
            av[2] = destino;
            do_cmode(s_ChanServ, 3, av);
            free(av[1]);
            if (ci->flags & CI_OPNOTICE) {
                notice(s_ChanServ, chan, "%s hace HALFOP a %s.",
                         u->nick, voice_params);
            }
        } else {
            notice_lang(s_ChanServ, u, NICK_X_NOT_IN_USE, voice_params);
        }
    }
}

static void do_dehalfop(User *u)
{
    char *chan = strtok(NULL, " ");
    char *devoice_params = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;

    if (!chan || !devoice_params) {
        syntax_error(s_ChanServ, u, "DEHALFOP", CHAN_DEOP_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
/*    } else if (!(ci = c->ci) && is_services_oper(u)) {
        send_cmd(s_ChanServ, "MODE %s -h %s", chan, devoice_params); */
    } else if (!(ci = c->ci)) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_services_oper(u) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
    } else if (!u || (!check_access(u, ci, CA_HALFOPDEHALFOP) &&
              !is_services_oper(u))) {
        notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
        char *av[3];
        User *u2 = finduser(devoice_params);
        char *destino;
        if (u2) {
            destino = u2->nick;
            send_cmd(s_ChanServ, "MODE %s -h %s", chan, destino);
            av[0] = chan;
            av[1] = sstrdup("-h");
            av[2] = destino;
            do_cmode(s_ChanServ, 3, av);
            free(av[1]);
            if (ci->flags & CI_OPNOTICE) {
                notice(s_ChanServ, chan, "%s hace DEHALFOP a %s.",
                        u->nick, devoice_params);
            }
        } else {
            notice_lang(s_ChanServ, u, NICK_X_NOT_IN_USE, devoice_params);
        }
    }
}

/*************************************************************************/

 /*static void do_op(User *u)
{
	char *chan = strtok(NULL, " ");
	char *op_params = strtok(NULL, " ");
	Channel *c;
	ChannelInfo *ci;
	
	if (!chan || !op_params) {
		syntax_error(s_ChanServm u, "OP", CHAN_OP_SYNTAX);
	} else if (!(c = findchan(chan))) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE);
	} else if (!(ci = c->ci)) {
		notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED);
	} else if (ci->flags & CI_VERBOTEN) {
		notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);            	
	} else if (!u || (!check_access(u, ci, CA_OPDEOP) && !is_services_oper(u))) {
		notice_lang(s_ChanServ, u, PERMISSION_DENIED);
	} else { */
static void do_op(User *u)
{
    char *chan = strtok(NULL, " ");
    char *op_params = strtok(NULL, " ");  
    Channel *c;
    ChannelInfo *ci;

    if (!chan || !op_params) {
	syntax_error(s_ChanServ, u, "OP", CHAN_OP_SYNTAX);
    } else if (!(c = findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
//    } else if (!(ci = c->ci) && is_services_oper(u)) {
//	  send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, u->nick);
//        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, op_params);  
//        goto op;
    } else if (!(ci = c->ci)) {            
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_services_oper(u) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);            	
    } else if (!u || (!check_access(u, ci, CA_OPDEOP) && !is_services_oper(u))) {
	notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
// op:    
	char *av[3];
        User *u2 = finduser(op_params);
        char *destino;
	if (u2) {
#ifdef IRC_UNDERNET_P10
            destino = u2->numerico;
#else
            //destino = u2->nick;
	    destino = u2->nick;
#endif	
            send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, destino);
            av[0] = chan;
	    av[1] = sstrdup("+o");
	    av[2] = destino;
	    do_cmode(s_ChanServ, 3, av);
            free(av[1]);     	
	    if (ci->flags & CI_OPNOTICE) {        
                notice(s_ChanServ, chan, "%s hace OP a %s.",
                        u->nick, destino);
            }    
	} else {
	    notice_lang(s_ChanServ, u, NICK_X_NOT_IN_USE, op_params);
	}    
    }
}

/*************************************************************************/

static void do_deop(User *u)
{
    char *chan = strtok(NULL, " ");
    char *deop_params = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;

    if (!chan || !deop_params) {
	syntax_error(s_ChanServ, u, "DEOP", CHAN_DEOP_SYNTAX);
    } else if (!(c = findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
/*    } else if (!(ci = c->ci) && is_services_oper(u)) {
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan, deop_params);             */
    } else if (!(ci = c->ci)) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_services_oper(u) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);             	
    } else if (!u || (!check_access(u, ci, CA_OPDEOP) && !is_services_oper(u))) {
	notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
	char *av[3];
        User *u2 = finduser(deop_params);
        char *destino;
        if (u2) {
#ifdef IRC_UNDERNET_P10
            destino = u2->numerico;
#else
            destino = u2->nick;
#endif
            if (is_services_oper(u2) && (stricmp(u2->nick, u->nick) != 0)) {
	          notice_lang(s_ChanServ, u, PERMISSION_DENIED);
		  privmsg(s_ChanServ, destino, "%s ha intentado hacerte DEOP el canal %s", u->nick, chan);
	          return;
            }
	    
	    send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -o %s", chan, destino);
            av[0] = chan;
	    av[1] = sstrdup("-o");
	    av[2] = destino;
	    do_cmode(s_ChanServ, 3, av);
            free(av[1]);	
	    if (ci->flags & CI_OPNOTICE) {
	        notice(s_ChanServ, chan, "%s hace DEOP a %s.",
                        u->nick, deop_params);                        	        
            }	        
	} else {
	    notice_lang(s_ChanServ, u, NICK_X_NOT_IN_USE, deop_params);
	}    
    }
}

/*************************************************************************/

static void do_ckick(User *u)
{

    char *chan = strtok(NULL, " ");
    char *nick = strtok(NULL, " ");
    char *reason = strtok(NULL, "");
            
    Channel *c;
    ChannelInfo *ci;
    char *av[3];
        
    if (!nick) {
        syntax_error(s_ChanServ, u, "KICK", CHAN_KICK_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (!(ci = c->ci)) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (ci->flags & CI_SUSPEND) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
    } else if (!u || (!check_access(u, ci, CA_KICK) && !is_services_oper(u))) {
        notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
        char buf[BUFSIZE];
        User *u2 = finduser(nick); 
        char *destino;
        if (u2) {
/*        me peta :( */
#ifdef IRC_UNDERNET_P10
            destino = u2->numerico;
            
#else
/*            destino = u2->nick; */
              destino = nick;
#endif
            if (reason) 
                snprintf(buf, sizeof(buf), "Kick ordenado -> %s", reason);     
            else 
                snprintf(buf, sizeof(buf), "Kick ordenado.");

            send_cmd(MODE_SENDER(s_ChanServ), "KICK %s %s :%s", chan, destino, buf);
            av[0] = sstrdup(chan);
            av[1] = destino;
            av[2] = sstrdup(buf);
            do_kick(s_ChanServ, 3, av);
            free(av[2]);                                                                                          
            if (ci->flags & CI_OPNOTICE)
                notice(s_ChanServ, chan, "%s ha KICKeado a %s.", u->nick, nick);
                
        } else {
            notice_lang(s_ChanServ, u, NICK_X_NOT_IN_USE, nick);
        }    
    }                   
}

/*************************************************************************/
#ifdef DESACTIVADO
static void do_ban(User *u)
{

    char *chan = strtok(NULL, " ");
    char *mask = strtok(NULL, " ");
        
    Channel *c;
    ChannelInfo *ci;
                                                                                                                                                                                             
    if (!mask) {
        syntax_error(s_ChanServ, u, "BAN", CHAN_BAN_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (!(ci = c->ci)) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (ci->flags & CI_SUSPEND) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
    } else if (!u || (!check_access(u, ci, CA_BAN) && !is_services_oper(u))) {
        notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
        char *av[3];
      
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +b  %s %lu", chan, mask, time(NULL));

        av[0] = chan;
        av[1] = sstrdup("+b");
        av[2] = mask;
        do_cmode(s_ChanServ, 3, av);
        free(av[0]);                                                        
        free(av[1]);
        free(av[2]);                    
        if (ci->flags & CI_OPNOTICE)
            notice(s_ChanServ, chan, "%s ha BANeado en el canal", u->nick);
        }
}
#endif                                               
                                                           
/*************************************************************************/

static void do_unban(User *u)
{
    char *chan = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;
    int i;
    char *av[3];

    if (!chan) {
	syntax_error(s_ChanServ, u, "UNBAN", CHAN_UNBAN_SYNTAX);
    } else if (!(c = findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (!(ci = c->ci)) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (ci->flags & CI_SUSPEND) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);            
    } else if (!u || (!check_access(u, ci, CA_UNBAN) && !is_services_oper(u))) {    
	notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
	/* Save original ban info */
	int desban = 0;
	int count = c->bancount;
	char **bans = smalloc(sizeof(char *) * count);
	memcpy(bans, c->bans, sizeof(char *) * count);

	av[0] = chan;
	av[1] = sstrdup("-b");
	for (i = 0; i < count; i++) {
	    if (match_usermask(bans[i], u)) {
		send_cmd(MODE_SENDER(s_ChanServ), "MODE %s -b %s",
			chan, bans[i]);
		av[2] = sstrdup(bans[i]);
		do_cmode(s_ChanServ, 3, av);
		desban = 1;
		free(av[2]);
	    }
	}
        free(av[1]);       	
	free(bans);
	if (desban)
	    notice_lang(s_ChanServ, u, CHAN_UNBANNED, chan);
	else
	    privmsg(s_ChanServ, u->nick, "No se ha podido desbanearte de %s", chan);
    }
}

/*************************************************************************/

static void do_clear(User *u)
{
    char *chan = strtok(NULL, " ");
    char *what = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;

    if (!what) {
	syntax_error(s_ChanServ, u, "CLEAR", CHAN_CLEAR_SYNTAX);
    } else if (!(c = findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (!(ci = c->ci)) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (!is_services_oper(u) && (ci->flags & CI_SUSPEND)) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);            
    } else if (!u || (!check_access(u, ci, CA_CLEAR)  && !is_services_oper(u))) {
	notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else if (stricmp(what, "bans") == 0) {
	char *av[3];
	int i;

	/* Save original ban info */
	int count = c->bancount;
	char **bans = smalloc(sizeof(char *) * count);
	for (i = 0; i < count; i++)
	    bans[i] = sstrdup(c->bans[i]);

	for (i = 0; i < count; i++) {
	    av[0] = sstrdup(chan);
	    av[1] = sstrdup("-b");
	    av[2] = bans[i];
	    send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
			av[0], av[1], av[2]);
	    do_cmode(s_ChanServ, 3, av);
	    free(av[2]);
	    free(av[1]);
	    free(av[0]);
	}
	notice_lang(s_ChanServ, u, CHAN_CLEARED_BANS, chan);
	free(bans);
    } else if (stricmp(what, "modes") == 0) {
	char *av[3];

	av[0] = chan;
	av[1] = sstrdup("-mintpslk");
	if (c->key)
	    av[2] = sstrdup(c->key);
	else
	    av[2] = sstrdup("");
	send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
			av[0], av[1], av[2]);
	do_cmode(s_ChanServ, 3, av);
	free(av[2]);
	free(av[1]);      	
	check_modes(chan);
	notice_lang(s_ChanServ, u, CHAN_CLEARED_MODES, chan);
    } else if (stricmp(what, "ops") == 0) {
	char *av[3];
	struct c_userlist *cu, *next;

	for (cu = c->chanops; cu; cu = next) {
	    next = cu->next;
	    av[0] = sstrdup(chan);
	    av[1] = sstrdup("-o");
#ifdef IRC_UNDERNET_P10
            av[2] = sstrdup(cu->user->numerico);
#else
	    av[2] = sstrdup(cu->user->nick);
#endif	    
	    send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
			av[0], av[1], av[2]);
	    do_cmode(s_ChanServ, 3, av);
	    free(av[2]);
	    free(av[1]);
	    free(av[0]);
	}
	notice_lang(s_ChanServ, u, CHAN_CLEARED_OPS, chan);
    } else if (stricmp(what, "halfops") == 0) {
	char *av[3];
	struct c_userlist *cu, *next;

	for (cu = c->halfops; cu; cu = next) {
	    next = cu->next;
	    av[0] = sstrdup(chan);
	    av[1] = sstrdup("-h");
#ifdef IRC_UNDERNET_P10
            av[2] = sstrdup(cu->user->numerico);
#else	    
	    av[2] = sstrdup(cu->user->nick);
#endif	    
	    send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
			av[0], av[1], av[2]);
	    do_cmode(s_ChanServ, 3, av);
	    free(av[2]);
	    free(av[1]);
	    free(av[0]);
	}
	notice_lang(s_ChanServ, u, CHAN_CLEARED_OPS, chan);
    } else if (stricmp(what, "voices") == 0) {
	char *av[3];
	struct c_userlist *cu, *next;

	for (cu = c->voices; cu; cu = next) {
	    next = cu->next;
	    av[0] = sstrdup(chan);
	    av[1] = sstrdup("-v");
#ifdef IRC_UNDERNET_P10
            av[2] = sstrdup(cu->user->numerico);
#else	    
	    av[2] = sstrdup(cu->user->nick);
#endif	    
	    send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
			av[0], av[1], av[2]);
	    do_cmode(s_ChanServ, 3, av);
	    free(av[2]);
	    free(av[1]);
	    free(av[0]);
	}
	notice_lang(s_ChanServ, u, CHAN_CLEARED_VOICES, chan);
    } else if (stricmp(what, "users") == 0) {
	char *av[3];
	struct c_userlist *cu, *next;
	char buf[256];

	snprintf(buf, sizeof(buf), "%s ha usado 12CLEAR USERS", u->nick);

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
	notice_lang(s_ChanServ, u, CHAN_CLEARED_USERS, chan);
    } else if (stricmp(what, "topic") == 0) {
        send_cmd(MODE_SENDER(s_ChanServ), "TOPIC %s :%s", chan, ci->last_topic);
        notice_lang(s_ChanServ, u, CHAN_CLEARED_USERS, chan);            	
    } else {
	syntax_error(s_ChanServ, u, "CLEAR", CHAN_CLEAR_SYNTAX);
    }
}

/*************************************************************************/
static void do_reset(User *u)
{
    char *chan = strtok(NULL, " ");
    Channel *c;
    ChannelInfo *ci;
                 
    if (!chan) {
        syntax_error(s_ChanServ, u, "RESET", CHAN_CLEAR_SYNTAX);
    } else if (!(c = findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
    } else if (!(ci = c->ci)) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
        notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else if (ci->flags & CI_SUSPEND) {
        notice_lang(s_ChanServ, u, CHAN_X_SUSPENDED, chan);
    } else if (!u || (!check_access(u, ci, CA_RESET) && !is_services_oper(u))) {
        notice_lang(s_ChanServ, u, PERMISSION_DENIED);
    } else {
                                                                                                                                                                  
        char *av[3];
        int i;
        struct c_userlist *cu, *next; 
        int count = c->bancount;
        char **bans = smalloc(sizeof(char *) * count);
/*
        c->bouncy_modes=0;
        c->server_modecount=0;
*/        
        
        if (ci->flags & CI_OPNOTICE) 
            notice(s_ChanServ, chan, "%s resetea el canal.", u->nick);
        if (ci->flags & CI_STAY) {
            send_cmd(s_ChanServ, "PART %s ", chan);
            send_cmd(s_ChanServ, "JOIN %s GOD", chan);
#ifdef IRC_UNDERNET_P10            
            send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, s_ChanServP10);
#else
            send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, s_ChanServ);
            
#endif            
        }
             /*** Clear Bans ***/
        for (i = 0; i < count; i++)
             bans[i] = sstrdup(c->bans[i]);
             for (i = 0; i < count; i++) {
                  av[0] = sstrdup(chan);
                  av[1] = sstrdup("-b");
                  av[2] = bans[i];
                  send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
                          av[0], av[1], av[2]);
                  do_cmode(s_ChanServ, 3, av);
                  free(av[2]);
                  free(av[1]);
                  free(av[0]);
              }
        free(bans);
                                                           
              /*** Clear Modes ***/
    
        av[0] = sstrdup(chan);
        av[1] = sstrdup("-mintpslkRAS");
        if (c->key)
            av[2] = sstrdup(c->key);
        else
        av[2] = sstrdup("");
        send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
                av[0], av[1], av[2]);
        do_cmode(s_ChanServ, 3, av);
        free(av[2]);
        free(av[1]);
        free(av[0]);                 
        check_modes(chan);

           /*** Clear Ops ***/               

        for (cu = c->chanops; cu; cu = next) {
             next = cu->next;
             av[0] = sstrdup(chan);
             av[1] = sstrdup("-o");
#ifdef IRC_UNDERNET_P10
             av[2] = sstrdup(cu->user->numerico);
#else             
             av[2] = sstrdup(cu->user->nick);
#endif             
             send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
                      av[0], av[1], av[2]);
             do_cmode(s_ChanServ, 3, av);
             free(av[2]);
             free(av[1]);
             free(av[0]);
        }  
                                      
          /***  Clear Halfops ***/                                                          
                          
        for (cu = c->halfops; cu; cu = next) {
             next  = cu->next;
             av[0] = sstrdup(chan);
             av[1] = sstrdup("-h");
#ifdef IRC_UNDERNET_P10
             av[2] = sstrdup(cu->user->numerico);
#else
             av[2] = sstrdup(cu->user->nick);
#endif             
             send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
                       av[0], av[1], av[2]);
             do_cmode(s_ChanServ, 3, av);
             free(av[2]);
             free(av[1]);
             free(av[0]);
        }  

          /***  Clear Voices ***/                                                          
                          
        for (cu = c->voices; cu; cu = next) {
             next  = cu->next;
             av[0] = sstrdup(chan);
             av[1] = sstrdup("-v");
#ifdef IRC_UNDERNET_P10
             av[2] = sstrdup(cu->user->numerico);
#else
             av[2] = sstrdup(cu->user->nick);
#endif             
             send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s",
                       av[0], av[1], av[2]);
             do_cmode(s_ChanServ, 3, av);
             free(av[2]);
             free(av[1]);
             free(av[0]);
        }  
        if (ci->last_topic) 
            send_cmd(MODE_SENDER(s_ChanServ), "TOPIC %s :%s", chan, ci->last_topic); 
    }
}

/*************************************************************************/

static void do_verify(User *u)
{
    char *nick = strtok(NULL, " ");
    NickInfo *ni;
       
    if (!nick) {
        syntax_error(s_ChanServ, u, "VERIFY", CHAN_INFO_SYNTAX);
        return;
    }
    ni = findnick(nick);
    
    if (stricmp(nick, s_ChanServ) == 0)
        privmsg(s_ChanServ, u->nick, "12%s es un BOT autorizado de la RED", s_ChanServ);
    else if (stricmp(nick, s_NickServ) == 0)
        privmsg(s_ChanServ, u->nick, "12%s es un BOT autorizado de la RED", s_NickServ);
    else if (stricmp(nick, s_MemoServ) == 0)
        privmsg(s_ChanServ, u->nick, "12%s es un BOT autorizado de la RED", s_MemoServ);
    else if (stricmp(nick, s_OperServ) == 0)
        privmsg(s_ChanServ, u->nick, "12%s es un BOT autorizado de la RED", s_OperServ);
    else if (stricmp(nick, s_HelpServ) == 0)
        privmsg(s_ChanServ, u->nick, "12%s es un BOT autorizado de la RED", s_HelpServ);
    else if (stricmp(nick, s_GlobalNoticer) == 0)
        privmsg(s_ChanServ, u->nick, "12%s es un BOT autorizado de la RED", s_GlobalNoticer);
            
/****  Bots de upworld *****/
    else if (stricmp(nick, "SHaDoW") == 0)
        privmsg(s_ChanServ, u->nick, "12SHaDoW es un BOT autorizado de la RED"); 
    else if (stricmp(nick, "ToDo") == 0)
        privmsg(s_ChanServ, u->nick, "12ToDo es un BOT autorizado de la RED");
    else if (stricmp(nick, ServicesRoot) ==0) {                                                                                                                                                                                                            
        privmsg(s_ChanServ, u->nick, "12%s es el 12ROOT de la RED", ServicesRoot);
        return;
     } else if (nick_is_services_admin(ni)) {
        privmsg(s_ChanServ, u->nick, "12%s es un 12ADMINinstrador de la RED.", ni->nick);
        return;
    } else if (nick_is_services_oper(ni)) {
        privmsg(s_ChanServ, u->nick, "12%s es un 12OPERador de la RED.", ni->nick);
        return;
    } else
        privmsg(s_ChanServ, u->nick, "12%s NO es un representante de la RED.", nick);
}
                                                                                      

/*************************************************************************/

static void do_ircops(User *u)
{
    ircops(u);
}
    

/*************************************************************************/

static void do_getpass(User *u)
{
#ifndef USE_ENCRYPTION
    char *chan = strtok(NULL, " ");
    ChannelInfo *ci;
#endif

    /* Assumes that permission checking has already been done. */
#ifdef USE_ENCRYPTION
    notice_lang(s_ChanServ, u, CHAN_GETPASS_UNAVAILABLE);
#else
    if (!chan) {
	syntax_error(s_ChanServ, u, "GETPASS", CHAN_GETPASS_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
	notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
    } else if (ci->flags & CI_VERBOTEN) {
	notice_lang(s_ChanServ, u, CHAN_X_FORBIDDEN, chan);
    } else {
	log("%s: %s!%s@%s uso GETPASS sobre %s",
		s_ChanServ, u->nick, u->username, u->host, ci->name);
        canalopers(s_ChanServ, "12%s ha usado GETPASS en 12%s (Founder: %s).",
             u->nick, chan, ci->founder->nick);         
	notice_lang(s_ChanServ, u, CHAN_GETPASS_PASSWORD_IS,
		chan, ci->founderpass);
    }
#endif
}


/*************************************************************************/

static void do_suspend(User *u)
{

    ChannelInfo *ci;
    char *av[3];
    char *chan, *expiry, *reason;
    Channel *c;
    struct c_userlist *cu, *next;        
    time_t expires;
    
    chan = strtok(NULL, " ");
/* Por el momento, nada de expiraciones
    if (chan && *chan == '+') {
        expiry = chan;
        chan = strtok(NULL, " ");
    } else {
        expiry = NULL;
    }            
*/
    expiry = NULL;
    reason = strtok(NULL, "");
    
    if (!reason) {
        syntax_error(s_ChanServ, u, "SUSPEND", CHAN_SUSPEND_SYNTAX);
    } else if (!(ci = cs_findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
             
    } else if (ci->flags & CI_VERBOTEN) {
        privmsg(s_ChanServ, u->nick, "No se puede suspender un canal forbideado");
                      
    } else if (ci->flags & CI_SUSPEND) {
        privmsg(s_ChanServ, u->nick, "El canal ya estaba suspendido");
                                                     
    } else {
        /*if (expiry) {
            expires = dotime(expiry);
            if (expires < 0) {
                notice_lang(s_ChanServ, u, BAD_EXPIRY_TIME);
                return;
            } else if (expires > 0) {
                expires += time(NULL);
            }
        } else {*/
//            expires = time(NULL) + CSSuspendExpire;
            expires = 0; /* suspension indefinida */
//        }    
        log("%s: %s!%s@%s SUSPENDió el canal %s, Motivo: %s", s_ChanServ, u->nick, u->username, u->host, chan, reason);                              
        ci->suspendby = sstrdup(u->nick);
        ci->suspendreason = sstrdup(reason);
        ci->time_suspend = time(NULL);
        ci->time_expiresuspend = expires;
        ci->flags |= CI_SUSPEND;
                
        notice_lang(s_ChanServ, u, CHAN_SUSPEND_SUCCEEDED, chan);
      
/*        if (!(ci->flags & CI_STAY)) {
              send_cmd(s_ChanServ, "JOIN %s", chan);
              send_cmd(MODE_SENDER(s_ChanServ), "MODE %s +o %s", chan, s_ChanServ);
        }
*/       
        send_cmd(s_ChanServ,"TOPIC %s :Este canal ha sido temporalmente SUSPENDIDO", chan);
        canalopers(s_ChanServ, "12%s ha 12SUSPENDido el canal 12%s, motivo %s", u->nick, chan, reason);

	c = findchan(chan);
	for (cu = c->chanops; cu; cu = next) {
            next = cu->next;
            av[0] = sstrdup(chan);
            av[1] = sstrdup("-o");
            av[2] = sstrdup(cu->user->nick);
            send_cmd(MODE_SENDER(s_ChanServ), "MODE %s %s :%s", av[0], av[1], av[2]);
            do_cmode(s_ChanServ, 3, av);
            free(av[2]);
            free(av[1]);
            free(av[0]);
	}
    }                                                                                         
}        

/*************************************************************************/

static void do_unsuspend(User *u)
{
    ChannelInfo *ci;
    char *chan = strtok(NULL, " ");

    if (!chan) {
        syntax_error(s_ChanServ, u, "UNSUSPEND", CHAN_SUSPEND_SYNTAX);
        return;
    }
    if (readonly)
        notice_lang(s_ChanServ, u, READ_ONLY_MODE);

    if (!(ci = cs_findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_REGISTERED, chan);
            
    } else if (!(ci->flags & CI_SUSPEND)) {
        privmsg(s_ChanServ, u->nick, "El canal no esta suspendido");
//    } else if (nick_is_services_admin(ci->suspendby) && !is_services_admin(u)) {
    } else {
        log("%s: %s!%s@%s ha usado UNSUSPEND on %s",
                s_ChanServ, u->nick, u->username, u->host, chan);
        free(ci->suspendby);
        free(ci->suspendreason);
        ci->time_suspend = 0;
        ci->time_expiresuspend = 0;
        ci->flags &= ~CI_SUSPEND;
        privmsg(s_ChanServ, u->nick, "Canal 12%s ha sido reactivado", chan);      
        canalopers(s_ChanServ, "12%s ha reactivado el canal 12%s", u->nick, chan);
     /*   if (!(ci->flags & CI_STAY)) 
            send_cmd(s_ChanServ, "PART %s", chan); */                                       
        send_cmd(MODE_SENDER(s_ChanServ), "TOPIC %s :%s", chan, ci->last_topic ? ci->last_topic : "");
    } 
    
}                                    
                                                                    
        
/*************************************************************************/

static void do_forbid(User *u)
{
    ChannelInfo *ci;
    char *chan = strtok(NULL, " ");
    char *reason = strtok(NULL, "");

    /* Assumes that permission checking has already been done. */
    if (!reason) {
	syntax_error(s_ChanServ, u, "FORBID", CHAN_FORBID_SYNTAX);
    } else if (*chan == '&') {
        privmsg(s_ChanServ, u->nick, "No puedes forbidear canales locales");
    } else if (!((*chan == '#') || (*chan == '+'))) {
        privmsg(s_ChanServ, u->nick, "Canal no valido para forbidear");
    } else if (strlen(chan) >= 64) {
        privmsg(s_ChanServ, u->nick, "No puedes forbidear canales con más de 64 caracteres");
    } else {
                                        
    if (readonly)
	notice_lang(s_ChanServ, u, READ_ONLY_MODE);
    if ((ci = cs_findchan(chan)) != NULL)
	delchan(ci);
    ci = makechan(chan);
    if (ci) {
	log("%s: %s set FORBID for channel %s", s_ChanServ, u->nick,
		ci->name);
	ci->flags |= CI_VERBOTEN;
        ci->forbidby = sstrdup(u->nick);
        ci->forbidreason = sstrdup(reason);                    	
	notice_lang(s_ChanServ, u, CHAN_FORBID_SUCCEEDED, chan);
        canalopers(s_ChanServ, "12%s ha FORBIDeado el canal 12%s",
                  u->nick, ci->name);                  	
    } else {
	log("%s: Valid FORBID for %s by %s failed", s_ChanServ, ci->name,
		u->nick);
	notice_lang(s_ChanServ, u, CHAN_FORBID_FAILED, chan);
    }
    }
}

/****************************************************************************/
static void do_unforbid(User *u)
{
    ChannelInfo *ci;
    char *chan = strtok(NULL, " ");

    if (!chan) {
        syntax_error(s_ChanServ, u, "UNFORBID", CHAN_FORBID_SYNTAX);
        return;
    }
    if (readonly)
        notice_lang(s_ChanServ, u, READ_ONLY_MODE);

    if ((ci = cs_findchan(chan)) != NULL && (ci->flags & CI_VERBOTEN)) {
        delchan(ci);
        log("%s: %s!%s@%s used UNFORBID on %s",
                      s_ChanServ, u->nick, u->username, u->host, chan);                      
        privmsg(s_ChanServ, u->nick, "Canal 12%s UNFORBIDeado.", chan);
        canalopers(s_ChanServ, "12%s ha UNFORBIDeado el canal 12%s",
                u->nick, chan);
    } else {
        log("%s: Valid UNFORBID for %s by %s failed", s_ChanServ, chan,
                        u->nick);
        privmsg(s_ChanServ, u->nick, "UNFORBID para 12%s fallido", chan);   
    }
}                                                                                                         

/*************************************************************************/

static void do_status(User *u)
{
    ChannelInfo *ci;
    User *u2;
    char *nick, *chan;

    chan = strtok(NULL, " ");
    nick = strtok(NULL, " ");
    if (!nick || strtok(NULL, " ")) {
	privmsg(s_ChanServ, u->nick, "STATUS ERROR Error de sintaxis");
	return;
    }
    if (!(ci = cs_findchan(chan))) {
	char *temp = chan;
	chan = nick;
	nick = temp;
	ci = cs_findchan(chan);
    }
    if (!ci) {
	privmsg(s_ChanServ, u->nick, "STATUS ERROR El canal %s no esta registrado",
		chan);
    } else if (ci->flags & CI_VERBOTEN) {
	privmsg(s_ChanServ, u->nick, "STATUS ERROR El canal %s esta FORBIDeado", chan);
	return;
    } else if (ci->flags & CI_SUSPEND) {
        privmsg(s_ChanServ, u->nick, "STATUS El canal %s esta SUSPENDido.", chan);
    } else if ((u2 = finduser(nick)) != NULL) {
	privmsg(s_ChanServ, u->nick, "STATUS en %s de %s es %d",
		chan, nick, get_access(u2, ci));
    } else { /* !u2 */
	privmsg(s_ChanServ, u->nick, "STATUS ERROR El nick %s no esta on-line", nick);
    }
}


/*************************************************************************/
static void do_say(User *u)
{

    char *chan = strtok(NULL, " ");
    char *text = strtok(NULL, "");
    Channel *c;

    if (!text) {
        privmsg(s_ChanServ, u->nick, "Sintaxis: 12SAY <canal> <mensaje>");
    } else if (!(c = findchan(chan))) {
        notice_lang(s_ChanServ, u, CHAN_X_NOT_IN_USE, chan);
    } else {
        log("%s: %s!%s@%s usado SAY on %s (%s)",
                      s_ChanServ, u->nick, u->username, u->host, chan, text);                            
        send_cmd(s_ChanServ, "PRIVMSG %s :%s", chan, text);
        canalopers(s_ChanServ, "12%s ha usado SAY en 12%s (%s)", u->nick, chan, text);
    }
}
                                                       
