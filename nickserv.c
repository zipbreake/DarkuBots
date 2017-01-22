/* NickServ functions.
 *
 * Services is copyright (c) 1996-1999 Andy Church.
 *     E-mail: <achurch@dragonfire.net>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 *
 * DarkuBots es una adaptaci髇 de Javier Fern醤dez Vi馻, ZipBreake.
 * E-Mail: javier@jfv.es || Web: http://jfv.es/
 *
 */

#include "services.h"
#include "pseudo.h"

/*************************************************************************/

static NickInfo *nicklists[256];	/* One for each initial character */

int numkills = 10;
#define TO_COLLIDE   0			/* Collide the user with this nick */
#define TO_RELEASE   1			/* Release a collided nick */

/*************************************************************************/

static int is_on_access(User *u, NickInfo *ni);
static void alpha_insert_nick(NickInfo *ni);
static NickInfo *makenick(const char *nick);
static int delnick(NickInfo *ni);
static void remove_links(NickInfo *ni);
static void delink(NickInfo *ni);

static void collide(NickInfo *ni, int from_timeout);
static void release(NickInfo *ni, int from_timeout);
static void add_ns_timeout(NickInfo *ni, int type, time_t delay);
static void del_ns_timeout(NickInfo *ni, int type);

static void do_credits(User *u);
static void do_help(User *u);
static void do_register(User *u);
static void do_identify(User *u);
static void do_drop(User *u);
static void do_set(User *u);
static void do_set_password(User *u, NickInfo *ni, char *param);
static void do_set_language(User *u, NickInfo *ni, char *param);
static void do_set_url(User *u, NickInfo *ni, char *param);
static void do_set_email(User *u, NickInfo *ni, char *param);
static void do_set_kill(User *u, NickInfo *ni, char *param);
static void do_set_secure(User *u, NickInfo *ni, char *param);
static void do_set_private(User *u, NickInfo *ni, char *param);
static void do_set_hide(User *u, NickInfo *ni, char *param);
static void do_set_noexpire(User *u, NickInfo *ni, char *param);
static void do_set_vhost(User *u, NickInfo *ni, char *param);
//static void do_access(User *u);
static void do_info(User *u);
static void do_userip(User *u);
static void do_list(User *u);
static void do_ghost(User *u);
static void do_status(User *u);
static void do_sendpass(User *u);
static void do_getpass(User *u);
static void do_suspend(User *u);
static void do_unsuspend(User *u);
static void do_forbid(User *u);
static void do_unforbid(User *u);
static void do_rename(User *u);


/*************************************************************************/

static Command cmds[] = {
    { "CREDITS",  do_credits,  NULL,  -1,                     -1,-1,-1,-1 },
    { "CREDITOS", do_credits,  NULL,  -1,                     -1,-1,-1,-1 },        
    { "HELP",     do_help,     NULL,  -1,                     -1,-1,-1,-1 },
    { "AYUDA",    do_help,     NULL,  -1,                     -1,-1,-1,-1 },
    { "REGISTER", do_register, NULL,  NICK_HELP_REGISTER,     -1,-1,-1,-1 },
    { "REGISTRA", do_register, NULL,  NICK_HELP_REGISTER,     -1,-1,-1,-1 },
//    { "IDENTIFY", do_identify, NULL,  NICK_HELP_IDENTIFY,     -1,-1,-1,-1 },
//    { "AUTH",     do_identify, NULL,  NICK_HELP_IDENTIFY,     -1,-1,-1,-1 },    
    { "USERIP",	  do_userip,   is_services_oper,  NICK_SERVADMIN_HELP_USERIP,         -1,-1,-1,-1 }, 
    
    { "DROP",     do_drop,     is_services_oper,  -1,
		NICK_HELP_DROP, NICK_SERVADMIN_HELP_DROP,
		NICK_SERVADMIN_HELP_DROP, NICK_SERVADMIN_HELP_DROP },


//    { "ACCESS",   do_access,   NULL,  NICK_HELP_ACCESS,       -1,-1,-1,-1 },


    { "SET",      do_set,      NULL,  NICK_HELP_SET,
		-1, NICK_SERVADMIN_HELP_SET,
		NICK_SERVADMIN_HELP_SET, NICK_SERVADMIN_HELP_SET },
                                    		
    { "SET PASSWORD", NULL,    NULL,  NICK_HELP_SET_PASSWORD, -1,-1,-1,-1 },
    { "SET PASS",     NULL,    NULL,  NICK_HELP_SET_PASSWORD, -1,-1,-1,-1 },    
    { "SET URL",      NULL,    NULL,  NICK_HELP_SET_URL,      -1,-1,-1,-1 },
    { "SET EMAIL",    NULL,    NULL,  NICK_HELP_SET_EMAIL,    -1,-1,-1,-1 },
    { "SET KILL",     NULL,    NULL,  NICK_HELP_SET_KILL,     -1,-1,-1,-1 },
    { "SET SECURE",   NULL,    NULL,  NICK_HELP_SET_SECURE,   -1,-1,-1,-1 },
    { "SET PRIVATE",  NULL,    NULL,  NICK_HELP_SET_PRIVATE,  -1,-1,-1,-1 },
    { "SET HIDE",     NULL,    NULL,  NICK_HELP_SET_HIDE,     -1,-1,-1,-1 },
    { "SET NOEXPIRE", NULL,    NULL,  -1, -1,
		NICK_SERVADMIN_HELP_SET_NOEXPIRE,
		NICK_SERVADMIN_HELP_SET_NOEXPIRE,
		NICK_SERVADMIN_HELP_SET_NOEXPIRE },
    { "SET VHOST", NULL,       NULL,  NICK_HELP_SET_VHOST,    -1,-1,-1,-1 },
    { "SET IPVIRTUAL", NULL,       NULL,  NICK_HELP_SET_VHOST,    -1,-1,-1,-1 },

//    { "RECOVER",  do_ghost,  NULL,  NICK_HELP_RECOVER,      -1,-1,-1,-1 },
//    { "RELEASE",  do_ghost,  NULL,  NICK_HELP_RELEASE,      -1,-1,-1,-1 },
//    { "GHOST",    do_ghost,    NULL,  NICK_HELP_GHOST,        -1,-1,-1,-1 },

    { "INFO",     do_info,     NULL,  NICK_HELP_INFO,
		-1, NICK_HELP_INFO, NICK_SERVADMIN_HELP_INFO,
		NICK_SERVADMIN_HELP_INFO },
    { "LIST",     do_list,     NULL,  -1,
		NICK_HELP_LIST, NICK_SERVADMIN_HELP_LIST,
		NICK_SERVADMIN_HELP_LIST, NICK_SERVADMIN_HELP_LIST },
    { "STATUS",   do_status,   NULL,  NICK_HELP_STATUS,       -1,-1,-1,-1 },


    { "SENDPASS",  do_sendpass,  is_services_oper,  -1,
                -1, NICK_SERVADMIN_HELP_SENDPASS,
                NICK_SERVADMIN_HELP_SENDPASS, NICK_SERVADMIN_HELP_SENDPASS },

    { "RENAME",  do_rename, is_services_oper,   -1,	      -1,-1,-1,-1 }, 

    { "GETPASS",  do_getpass,  is_services_admin,  -1,
                -1, NICK_SERVADMIN_HELP_GETPASS,
                NICK_SERVADMIN_HELP_GETPASS, NICK_SERVADMIN_HELP_GETPASS },
    { "SUSPEND",  do_suspend,  is_services_oper,  -1,
                -1, NICK_SERVADMIN_HELP_SUSPEND,
                NICK_SERVADMIN_HELP_SUSPEND, NICK_SERVADMIN_HELP_SUSPEND },
    { "UNSUSPEND",  do_unsuspend,  is_services_oper,  -1,
                -1, NICK_SERVADMIN_HELP_UNSUSPEND,
                NICK_SERVADMIN_HELP_UNSUSPEND, NICK_SERVADMIN_HELP_UNSUSPEND },
    { "FORBID",   do_forbid,   is_services_admin,  -1,
                -1, NICK_SERVADMIN_HELP_FORBID,
                NICK_SERVADMIN_HELP_FORBID, NICK_SERVADMIN_HELP_FORBID },
    { "UNFORBID",   do_unforbid,   is_services_admin,  -1,
                -1, NICK_SERVADMIN_HELP_UNFORBID,
                NICK_SERVADMIN_HELP_UNFORBID, NICK_SERVADMIN_HELP_UNFORBID },
    { NULL }
};

/*************************************************************************/
/*************************************************************************/

/* Return information on memory use.  Assumes pointers are valid. */

void get_nickserv_stats(long *nrec, long *memuse)
{
    long count = 0, mem = 0;
    int i, j;
    NickInfo *ni;
    char **accptr;

    for (i = 0; i < 256; i++) {
	for (ni = nicklists[i]; ni; ni = ni->next) {
	    count++;
	    mem += sizeof(*ni);
	    if (ni->url)
		mem += strlen(ni->url)+1;
	    if (ni->email)
		mem += strlen(ni->email)+1;
	    if (ni->last_usermask)
		mem += strlen(ni->last_usermask)+1;
	    if (ni->last_realname)
		mem += strlen(ni->last_realname)+1;
	    if (ni->last_quit)
		mem += strlen(ni->last_quit)+1;
            if (ni->suspendby)
                mem += strlen(ni->suspendby)+1;
            if (ni->suspendreason)
                mem += strlen(ni->suspendreason)+1;
            if (ni->forbidby)
                mem += strlen(ni->forbidby)+1;
            if (ni->forbidreason)
                mem += strlen(ni->forbidreason)+1;		
	    mem += sizeof(char *) * ni->accesscount;
	    for (accptr=ni->access, j=0; j < ni->accesscount; accptr++, j++) {
		if (*accptr)
		    mem += strlen(*accptr)+1;
	    }
	    mem += ni->memos.memocount * sizeof(Memo);
	    for (j = 0; j < ni->memos.memocount; j++) {
		if (ni->memos.memos[j].text)
		    mem += strlen(ni->memos.memos[j].text)+1;
	    }
	}
    }
    *nrec = count;
    *memuse = mem;
}

/*************************************************************************/
/*************************************************************************/

/* NickServ initialization. */

void ns_init(void)
{
}

/*************************************************************************/

/* Main NickServ routine. */

void nickserv(const char *source, char *buf)
{
    char *cmd, *s;
    User *u = finduser(source);

    if (!u) {
	log("%s: user record for %s not found", s_NickServ, source);
#ifndef IRC_UNDERNET_P10
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
        notice(s_NickServ, u->numerico, "\1PING %s", s);
#else
	notice(s_NickServ, source, "\1PING %s", s);
#endif	
    } else if (stricmp(cmd, "\1VERSION\1") == 0) {
        notice(s_NickServ, source, "\1VERSION %s %s -- %s\1",
                PNAME, s_NickServ, version_build);
                               
    } else if (skeleton) {
	notice_lang(s_NickServ, u, SERVICE_OFFLINE, s_NickServ);
    } else {
	run_cmd(s_NickServ, u, cmds, cmd);
    }

}

/*************************************************************************/

/* Load/save data files. */


#define SAFE(x) do {					\
    if ((x) < 0) {					\
	if (!forceload)					\
	    fatal("Error de lectura en %s", NickDBName);	\
	failed = 1;					\
	break;						\
    }							\
} while (0)

void load_ns_dbase(void)
{
    dbFILE *f;
    int ver, i, j, c;
    NickInfo *ni, **last, *prev;
    int failed = 0;

    if (!(f = open_db(s_NickServ, NickDBName, "r")))
	return;

    switch (ver = get_file_version(f)) {
      case 9:
      case 8:
	for (i = 0; i < 256 && !failed; i++) {
	    int32 tmp32;
	    last = &nicklists[i];
	    prev = NULL;
	    while ((c = getc_db(f)) == 1) {
		if (c != 1)
		    fatal("Invalid format in %s", NickDBName);
		ni = scalloc(sizeof(NickInfo), 1);
		*last = ni;
		last = &ni->next;
		ni->prev = prev;
		prev = ni;
		SAFE(read_buffer(ni->nick, f));
		SAFE(read_buffer(ni->pass, f));
		SAFE(read_string(&ni->url, f));
		SAFE(read_string(&ni->email, f));
                if (ver >= 9) {
			SAFE(read_string(&ni->vhost, f));
		}
		SAFE(read_string(&ni->last_usermask, f));
		if (!ni->last_usermask)
		    ni->last_usermask = sstrdup("@");
		SAFE(read_string(&ni->last_realname, f));
		if (!ni->last_realname)
		    ni->last_realname = sstrdup("");
		SAFE(read_string(&ni->last_quit, f));
		SAFE(read_int32(&tmp32, f));
		ni->time_registered = tmp32;
		SAFE(read_int32(&tmp32, f));
		ni->last_seen = tmp32;
		SAFE(read_int16(&ni->status, f));
		ni->status &= ~NS_TEMPORARY;
#ifdef USE_ENCRYPTION
		if (!(ni->status & (NS_ENCRYPTEDPW | NS_VERBOTEN))) {
		    if (debug)
			log("debug: %s: encrypting password for `%s' on load",
				s_NickServ, ni->nick);
		    if (encrypt_in_place(ni->pass, PASSMAX) < 0)
			fatal("%s: Can't encrypt `%s' nickname password!",
				s_NickServ, ni->nick);
		    ni->status |= NS_ENCRYPTEDPW;
		}
#else
		if (ni->status & NS_ENCRYPTEDPW) {
		    /* Bail: it makes no sense to continue with encrypted
		     * passwords, since we won't be able to verify them */
		    fatal("%s: load database: password for %s encrypted "
		          "but encryption disabled, aborting",
		          s_NickServ, ni->nick);
		}
#endif
                if (ver >= 8) {
                    SAFE(read_string(&ni->suspendby, f));
                    SAFE(read_string(&ni->suspendreason, f));
                    SAFE(read_int32(&tmp32, f));
                    ni->time_suspend = tmp32;
                    SAFE(read_int32(&tmp32, f));
                    ni->time_expiresuspend = tmp32;
                    SAFE(read_string(&ni->forbidby, f));
                    SAFE(read_string(&ni->forbidreason, f));
                } else {
                    ni->suspendby = NULL;
                    ni->suspendreason = NULL;
                    ni->time_suspend = 0;
                    ni->time_expiresuspend = 0;
                    ni->forbidby = NULL;
                    ni->forbidreason = NULL;
                }    
		/* Store the _name_ of the link target in ni->link for now;
		 * we'll resolve it after we've loaded all the nicks */
		SAFE(read_string((char **)&ni->link, f));
		SAFE(read_int16(&ni->linkcount, f));
		if (ni->link) {
		    SAFE(read_int16(&ni->channelcount, f));
		    /* No other information saved for linked nicks, since
		     * they get it all from their link target */
		    ni->flags = 0;
		    ni->accesscount = 0;
		    ni->access = NULL;
		    ni->memos.memocount = 0;
		    ni->memos.memomax = MSMaxMemos;
		    ni->memos.memos = NULL;
		    ni->channelmax = CSMaxReg;
		    ni->language = DEF_LANGUAGE;
		} else {
		    SAFE(read_int32(&ni->flags, f));
		    if (!NSAllowKillImmed)
			ni->flags &= ~NI_KILL_IMMED;
		    SAFE(read_int16(&ni->accesscount, f));
		    if (ni->accesscount) {
			char **access;
			access = smalloc(sizeof(char *) * ni->accesscount);
			ni->access = access;
			for (j = 0; j < ni->accesscount; j++, access++)
			    SAFE(read_string(access, f));
		    }
		    SAFE(read_int16(&ni->memos.memocount, f));
		    SAFE(read_int16(&ni->memos.memomax, f));
		    if (ni->memos.memocount) {
			Memo *memos;
			memos = smalloc(sizeof(Memo) * ni->memos.memocount);
			ni->memos.memos = memos;
			for (j = 0; j < ni->memos.memocount; j++, memos++) {
			    SAFE(read_int32(&memos->number, f));
			    SAFE(read_int16(&memos->flags, f));
			    SAFE(read_int32(&tmp32, f));
			    memos->time = tmp32;
			    SAFE(read_buffer(memos->sender, f));
			    SAFE(read_string(&memos->text, f));
			}
		    }
		    SAFE(read_int16(&ni->channelcount, f));
		    SAFE(read_int16(&ni->channelmax, f));
		    if (ver == 5) {
			/* Fields not initialized properly for new nicks */
			/* These will be updated by load_cs_dbase() */
			ni->channelcount = 0;
			ni->channelmax = CSMaxReg;
		    }
		    SAFE(read_int16(&ni->language, f));
		}
		ni->id_timestamp = 0;
	    } /* while (getc_db(f) != 0) */
	    *last = NULL;
	} /* for (i) */

	/* Now resolve links */
	for (i = 0; i < 256; i++) {
	    for (ni = nicklists[i]; ni; ni = ni->next) {
		if (ni->link)
		    ni->link = findnick((char *)ni->link);
	    }
	}

	break;

      default:
	fatal("Unsupported version number (%d) on %s", ver, NickDBName);

    } /* switch (version) */

    close_db(f);
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	restore_db(f);						\
	log_perror("Write error on %s", NickDBName);		\
	if (time(NULL) - lastwarn > WarningTimeout) {		\
	    canalopers(NULL, "Write error on %s: %s", NickDBName,	\
			strerror(errno));			\
	    lastwarn = time(NULL);				\
	}							\
	return;							\
    }								\
} while (0)

void save_ns_dbase(void)
{
    dbFILE *f;
    int i, j;
    NickInfo *ni;
    char **access;
    Memo *memos;
    static time_t lastwarn = 0;

    if (!(f = open_db(s_NickServ, NickDBName, "w")))
	return;
    for (i = 0; i < 256; i++) {
	for (ni = nicklists[i]; ni; ni = ni->next) {
	    SAFE(write_int8(1, f));
	    SAFE(write_buffer(ni->nick, f));
	    SAFE(write_buffer(ni->pass, f));
	    SAFE(write_string(ni->url, f));
	    SAFE(write_string(ni->email, f));
	    SAFE(write_string(ni->vhost, f));
	    SAFE(write_string(ni->last_usermask, f));
	    SAFE(write_string(ni->last_realname, f));
	    SAFE(write_string(ni->last_quit, f));
	    SAFE(write_int32(ni->time_registered, f));
	    SAFE(write_int32(ni->last_seen, f));
	    SAFE(write_int16(ni->status, f));
            SAFE(write_string(ni->suspendby, f));
            SAFE(write_string(ni->suspendreason, f));
            SAFE(write_int32(ni->time_suspend, f));
            SAFE(write_int32(ni->time_expiresuspend, f));
            SAFE(write_string(ni->forbidby, f));
            SAFE(write_string(ni->forbidreason, f));                                                              
	    if (ni->link) {
		SAFE(write_string(ni->link->nick, f));
		SAFE(write_int16(ni->linkcount, f));
		SAFE(write_int16(ni->channelcount, f));
	    } else {
		SAFE(write_string(NULL, f));
		SAFE(write_int16(ni->linkcount, f));
		SAFE(write_int32(ni->flags, f));
		SAFE(write_int16(ni->accesscount, f));
		for (j=0, access=ni->access; j<ni->accesscount; j++, access++)
		    SAFE(write_string(*access, f));
		SAFE(write_int16(ni->memos.memocount, f));
		SAFE(write_int16(ni->memos.memomax, f));
		memos = ni->memos.memos;
		for (j = 0; j < ni->memos.memocount; j++, memos++) {
		    SAFE(write_int32(memos->number, f));
		    SAFE(write_int16(memos->flags, f));
		    SAFE(write_int32(memos->time, f));
		    SAFE(write_buffer(memos->sender, f));
		    SAFE(write_string(memos->text, f));
		}
		SAFE(write_int16(ni->channelcount, f));
		SAFE(write_int16(ni->channelmax, f));
		SAFE(write_int16(ni->language, f));
	    }
	} /* for (ni) */
	SAFE(write_int8(0, f));
    } /* for (i) */
    close_db(f);
}

#undef SAFE

/*************************************************************************/

/* Check whether a user is on the access list of the nick they're using, or
 * if they're the same user who last identified for the nick.  If not, send
 * warnings as appropriate.  If so (and not NI_SECURE), update last seen
 * info.  Return 1 if the user is valid and recognized, 0 otherwise (note
 * that this means an NI_SECURE nick will return 0 from here unless the
 * user's timestamp matches the last identify timestamp).  If the user's
 * nick is not registered, 0 is returned.
 */

int validate_user(User *u)
{
    NickInfo *ni;
    int on_access;

    if (!(ni = u->real_ni))
	return 0;
	
    if (ni->status & NS_IDENTIFIED)
        return 1;
    
    if (ni->status & NI_ON_BDD)
        return 1;

    if (ni->status & NS_VERBOTEN) {
	notice_lang(s_NickServ, u, NICK_MAY_NOT_BE_USED);
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
	if (NSForceNickChange)
	    notice_lang(s_NickServ, u, FORCENICKCHANGE_IN_1_MINUTE);
	else
#endif
	    notice_lang(s_NickServ, u, DISCONNECT_IN_1_MINUTE);
	add_ns_timeout(ni, TO_COLLIDE, 60);
	return 0;
    }

    if (ni->status & NS_SUSPENDED) {
        notice_lang(s_NickServ, u, NICK_SUSPENDED, ni->suspendreason);
        return 0;
    }
#ifdef OBSOLETO                      
    if (!NoSplitRecovery) {
	/* XXX: This code should be checked to ensure it can't be fooled */
	if (ni->id_timestamp != 0 && u->signon == ni->id_timestamp) {
	    char buf[256];
	    snprintf(buf, sizeof(buf), "%s@%s", u->username, u->host);
	    if (strcmp(buf, ni->last_usermask) == 0) {
		ni->status |= NS_IDENTIFIED;
		return 1;
	    }
	}
    }   
#endif

    on_access = is_on_access(u, u->ni);
    if (on_access)
	ni->status |= NS_ON_ACCESS;

    if (!(u->ni->flags & NI_SECURE) && on_access) {
	ni->status |= NS_RECOGNIZED;
	ni->last_seen = time(NULL);
	if (ni->last_usermask)
	    free(ni->last_usermask);
	ni->last_usermask = smalloc(strlen(u->username)+strlen(u->host)+2);
	sprintf(ni->last_usermask, "%s@%s", u->username, u->host);
	if (ni->last_realname)
	    free(ni->last_realname);
	ni->last_realname = sstrdup(u->realname);
	return 1;
    }

    if (on_access || !(u->ni->flags & NI_KILL_IMMED)) {
	if (u->ni->flags & NI_SECURE)
	    notice_lang(s_NickServ, u, NICK_IS_SECURE, s_NickServ);
	else
	    notice_lang(s_NickServ, u, NICK_IS_REGISTERED, s_NickServ);
    }

    if ((u->ni->flags & NI_KILLPROTECT) && !on_access) {
	if (u->ni->flags & NI_KILL_IMMED) {
	    collide(ni, 0);
	} else if (u->ni->flags & NI_KILL_QUICK) {
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
	    if (NSForceNickChange)
	    	notice_lang(s_NickServ, u, FORCENICKCHANGE_IN_20_SECONDS);
	    else
#endif
	    	notice_lang(s_NickServ, u, DISCONNECT_IN_20_SECONDS);
	    add_ns_timeout(ni, TO_COLLIDE, 20);
	} else {
#if defined (IRC_HISPANO) || defined (IRC_TERRA)
	    if (NSForceNickChange)
	    	notice_lang(s_NickServ, u, FORCENICKCHANGE_IN_1_MINUTE);
	    else
#endif
	    	notice_lang(s_NickServ, u, DISCONNECT_IN_1_MINUTE);
	    add_ns_timeout(ni, TO_COLLIDE, 60);
	}
    }

    return 0;
}

/*************************************************************************/

/* Cancel validation flags for a nick (i.e. when the user with that nick
 * signs off or changes nicks).  Also cancels any impending collide. */

void cancel_user(User *u)
{
    NickInfo *ni = u->real_ni;
    if (ni) {

#ifdef IRC_TERRA
	if (ni->status & NS_GUESTED) {
	    send_cmd(NULL, "NICK %s %ld 1 %s %s %s :%s Enforcement",
			u->nick, time(NULL), NSEnforcerUser, NSEnforcerHost, 
			ServerName, s_NickServ);
	    add_ns_timeout(ni, TO_RELEASE, NSReleaseTimeout);
	    ni->status &= ~NS_TEMPORARY;
	    ni->status |= NS_KILL_HELD;
	} else {
#endif
	    ni->status &= ~NS_TEMPORARY;
#ifdef IRC_TERRA
	}
#endif
	del_ns_timeout(ni, TO_COLLIDE);
    }
}

/*************************************************************************/

/* Return whether a user has identified for their nickname. */

int nick_identified(User *u)
{
    return u->real_ni && (u->real_ni->status & NS_IDENTIFIED);
}

/*************************************************************************/

/* Return whether a user is recognized for their nickname. */

int nick_recognized(User *u)
{
    return u->real_ni && (u->real_ni->status & (NS_IDENTIFIED | NS_RECOGNIZED));
}
/*************************************************************************/

/* Nick suspendido. */

int nick_suspendied(User *u)
{
    return u->real_ni && (u->real_ni->status & NS_SUSPENDED);
}
    

/*************************************************************************/

/* Remove all nicks which have expired.  Also update last-seen time for all
 * nicks.
 */

void expire_nicks()
{
    User *u;
    NickInfo *ni, *next;
    int i;
    time_t now = time(NULL);

    /* Assumption: this routine takes less than NSExpire seconds to run.
     * If it doesn't, some users may end up with invalid user->ni pointers. */
    for (u = firstuser(); u; u = nextuser()) {
	if (u->real_ni) {
	    if (debug >= 2)
		log("debug: NickServ: updating last seen time for %s", u->nick);
	    u->real_ni->last_seen = time(NULL);
	}
    }
    if (!NSExpire)
	return;
    for (i = 0; i < 256; i++) {
	for (ni = nicklists[i]; ni; ni = next) {
	    next = ni->next;
	    if (now - ni->last_seen >= NSExpire
			&& !(ni->status & (NS_VERBOTEN | NS_NO_EXPIRE | NS_SUSPENDED))) {
		log("Expirando Nick %s", ni->nick);
		canalopers(s_NickServ, "El nick 12%s ha expirado", ni->nick);
		delnick(ni);
	    }
	    /* AQUI EXPIRACION NICKS SUSPENDIDOS */
	}
    }
}

/*************************************************************************/
/*************************************************************************/

/* Return the NickInfo structure for the given nick, or NULL if the nick
 * isn't registered. */

NickInfo *findnick(const char *nick)
{
    NickInfo *ni;

#ifdef IRC_UNDERNET
/* A帽adido soporte toLower, toUpper y strCasecmp para evitar conflictos
 * con nicks, debido a la arquitectura de undernet, que considera
 * como equivalentes los nicks [zoltan] y {zoltan} entre otros signos
 * As铆 los Services, lo considerar谩 como el mismo nick, impidiendo que
 * se pueda registrar {zoltan} existiendo [zoltan]
 *
 * Signos equivalentes
 * Min煤sculas == May煤sculas (esto ya estaba antes con tolower)
 * [ == {
 * ] == }
 * ^ == ~
 * \ == |
 *
 * Copiado codigo common.c y common.h del ircu de Undernet
 *
 * zoltan 1/11/2000
 */
    for (ni = nicklists[toLower(*nick)]; ni; ni = ni->next) {
        if (strCasecmp(ni->nick, nick) == 0)
            return ni;
    }

    for (ni = nicklists[toUpper(*nick)]; ni; ni = ni->next) {
        if (strCasecmp(ni->nick, nick) == 0)
            return ni;
    }
#else

    for (ni = nicklists[tolower(*nick)]; ni; ni = ni->next) {
	if (stricmp(ni->nick, nick) == 0)
	    return ni;
    }
#endif
    
    return NULL;
}

/*************************************************************************/

/* Return the "master" nick for the given nick; i.e., trace the linked list
 * through the `link' field until we find a nickname with a NULL `link'
 * field.  Assumes `ni' is not NULL.
 *
 * Note that we impose an arbitrary limit of 512 nested links.  This is to
 * prevent infinite loops in case someone manages to create a circular
 * link.  If we pass this limit, we arbitrarily cut off the link at the
 * initial nick.
 */

NickInfo *getlink(NickInfo *ni)
{
    return ni;
}

/*************************************************************************/
/*********************** NickServ private routines ***********************/
/*************************************************************************/

/* Is the given user's address on the given nick's access list?  Return 1
 * if so, 0 if not. */

static int is_on_access(User *u, NickInfo *ni)
{
    int i;
    char *buf;

    if (ni->accesscount == 0)
	return 0;
    i = strlen(u->username);
    buf = smalloc(i + strlen(u->host) + 2);
    sprintf(buf, "%s@%s", u->username, u->host);
    strlower(buf+i+1);
    for (i = 0; i < ni->accesscount; i++) {
	if (match_wild_nocase(ni->access[i], buf)) {
	    free(buf);
	    return 1;
	}
    }
    free(buf);
    return 0;
}

/*************************************************************************/

/* Insert a nick alphabetically into the database. */

static void alpha_insert_nick(NickInfo *ni)
{
    NickInfo *ptr, *prev;
    char *nick = ni->nick;

    for (prev = NULL, ptr = nicklists[tolower(*nick)];
			ptr && stricmp(ptr->nick, nick) < 0;
			prev = ptr, ptr = ptr->next)
	;
    ni->prev = prev;
    ni->next = ptr;
    if (!prev)
	nicklists[tolower(*nick)] = ni;
    else
	prev->next = ni;
    if (ptr)
	ptr->prev = ni;
}

/*************************************************************************/

/* Add a nick to the database.  Returns a pointer to the new NickInfo
 * structure if the nick was successfully registered, NULL otherwise.
 * Assumes nick does not already exist.
 */

static NickInfo *makenick(const char *nick)
{
    NickInfo *ni;

    ni = scalloc(sizeof(NickInfo), 1);
    strscpy(ni->nick, nick, NICKMAX);
    alpha_insert_nick(ni);
    ni->status |= NI_ON_BDD;
    return ni;
   
}

/*************************************************************************/

/* Remove a nick from the NickServ database.  Return 1 on success, 0
 * otherwise.  Also deletes the nick from any ChanServ/OperServ lists it is
 * on.
 */

static int delnick(NickInfo *ni)
{
    int i;

    if (ni->status & NI_ON_BDD) {
	do_write_bdd(ni->nick, 15, "");
	do_write_bdd(ni->nick, 2, "");
	do_write_bdd(ni->nick, 3, "");
	do_write_bdd(ni->nick, 4, "");
    }

    cs_remove_nick(ni);
    os_remove_nick(ni);
    if (ni->linkcount)
	remove_links(ni);
    if (ni->link)
	ni->link->linkcount--;
    if (ni->next)
	ni->next->prev = ni->prev;
    if (ni->prev)
	ni->prev->next = ni->next;
    else
	nicklists[tolower(*ni->nick)] = ni->next;
    if (ni->last_usermask)
	free(ni->last_usermask);
    if (ni->last_realname)
	free(ni->last_realname);
    if (ni->email)
        free (ni->email);	
    if (ni->url)
        free (ni->url);    
    if (ni->suspendby)
        free (ni->suspendby);
    if (ni->suspendreason)
        free (ni->suspendreason); 
    if (ni->forbidby)
        free (ni->forbidby);        
    if (ni->forbidreason)
        free (ni->forbidreason);        
    if (ni->access) {
	for (i = 0; i < ni->accesscount; i++) {
	    if (ni->access[i])
		free(ni->access[i]);
	}
	free(ni->access);
    }
    if (ni->memos.memos) {
	for (i = 0; i < ni->memos.memocount; i++) {
	    if (ni->memos.memos[i].text)
		free(ni->memos.memos[i].text);
	}
	free(ni->memos.memos);
    }

    free(ni);
    return 1;
}

/*************************************************************************/

/* Remove any links to the given nick (i.e. prior to deleting the nick).
 * Note this is currently linear in the number of nicks in the database--
 * that's the tradeoff for the nice clean method of keeping a single parent
 * link in the data structure.
 */

static void remove_links(NickInfo *ni)
{
    int i;
    NickInfo *ptr;

    for (i = 0; i < 256; i++) {
	for (ptr = nicklists[i]; ptr; ptr = ptr->next) {
	    if (ptr->link == ni) {
		if (ni->link) {
		    ptr->link = ni->link;
		    ni->link->linkcount++;
		} else
		    delink(ptr);
	    }
	}
    }
}

/*************************************************************************/

/* Break a link from the given nick to its parent. */

static void delink(NickInfo *ni)
{
    NickInfo *link;

    link = ni->link;
    ni->link = NULL;
    do {
	link->channelcount -= ni->channelcount;
	if (link->link)
	    link = link->link;
    } while (link->link);
    ni->status = link->status;
    link->status &= ~NS_TEMPORARY;
    ni->flags = link->flags;
    ni->channelmax = link->channelmax;
    ni->memos.memomax = link->memos.memomax;
    ni->language = link->language;
    if (link->accesscount > 0) {
	char **access;
	int i;

	ni->accesscount = link->accesscount;
	access = smalloc(sizeof(char *) * ni->accesscount);
	ni->access = access;
	for (i = 0; i < ni->accesscount; i++, access++)
	    *access = sstrdup(link->access[i]);
    }
    link->linkcount--;
}

/*************************************************************************/
/*************************************************************************/

/* Collide a nick. 
 *
 * When connected to a network using DALnet servers, version 4.4.15 and above, 
 * Services is now able to force a nick change instead of killing the user. 
 * The new nick takes the form "Guest######". If a nick change is forced, we
 * do not introduce the enforcer nick until the user's nick actually changes. 
 * This is watched for and done in cancel_user(). -TheShadow 
 */

static void collide(NickInfo *ni, int from_timeout)
{
    User *u;
#ifdef IRC_UNDERNET_P10
    int numeroa, numerob;    
#endif
    u = finduser(ni->nick);


#ifdef IRC_UNDERNET_P10
    numkills++;
    
    if (numkills > 4095)
        numkills = 10;
    numeroa = (numkills / 64);
    numerob = numkills - (64 * numeroa);
#endif

    if (!from_timeout)
	del_ns_timeout(ni, TO_COLLIDE);

#ifdef IRC_TERRA
    if (NSForceNickChange) {
	struct timeval tv;
	char guestnick[NICKMAX];

	gettimeofday(&tv, NULL);
	snprintf(guestnick, sizeof(guestnick), "%s%ld%ld", NSGuestNickPrefix,
			tv.tv_usec / 10000, tv.tv_sec % (60*60*24));

        notice_lang(s_NickServ, u, FORCENICKCHANGE_NOW, guestnick);

	send_cmd(NULL, "SVSNICK %s %s :%ld", ni->nick, guestnick, time(NULL));
	ni->status |= NS_GUESTED;
    } else {
#elif defined (IRC_HISPANO)
    if (NSForceNickChange) {
        send_cmd(ServerName, "RENAME %s", ni->nick);
    } else {
#endif                
	notice_lang(s_NickServ, u, DISCONNECT_NOW);
#ifdef IRC_UNDERNET_P10
        
        kill_user(s_NickServ, u->numerico, "Protecci贸n de Nick Registrado");
    	send_cmd(NULL, "%c NICK %s 1 %lu  %s %s AAAAAA %c%c%c :%s protegiendo a %s",
           convert2y[ServerNumerico], ni->nick, time(NULL), NSEnforcerUser,
           ServiceHost, convert2y[ServerNumerico], convert2y[numeroa],
           convert2y[numerob], s_NickServ, ni->nick); 
#else
        kill_user(s_NickServ, ni->nick, "Protecci贸n de Nick Registrado");
        send_cmd(NULL, "NICK %s %ld 1 %s %s %s :%s protegiendo a %s",
                ni->nick, time(NULL), NSEnforcerUser, NSEnforcerHost,
                ServerName, s_NickServ, ni->nick);
#endif		
	ni->status |= NS_KILL_HELD;
	add_ns_timeout(ni, TO_RELEASE, NSReleaseTimeout);
#if defined (IRC_TERRA) || defined (IRC_HISPANO)
    }
#endif
}

/*************************************************************************/

/* Release hold on a nick. */

static void release(NickInfo *ni, int from_timeout)
{
    if (!from_timeout)
	del_ns_timeout(ni, TO_RELEASE);
    send_cmd(ni->nick, "QUIT");
    ni->status &= ~NS_KILL_HELD;
}

/*************************************************************************/
/*************************************************************************/

static struct my_timeout {
    struct my_timeout *next, *prev;
    NickInfo *ni;
    Timeout *to;
    int type;
} *my_timeouts;

/*************************************************************************/

/* Remove a collide/release timeout from our private list. */

static void rem_ns_timeout(NickInfo *ni, int type)
{
    struct my_timeout *t, *t2;

    t = my_timeouts;
    while (t) {
	if (t->ni == ni && t->type == type) {
	    t2 = t->next;
	    if (t->next)
		t->next->prev = t->prev;
	    if (t->prev)
		t->prev->next = t->next;
	    else
		my_timeouts = t->next;
	    free(t);
	    t = t2;
	} else {
	    t = t->next;
	}
    }
}

/*************************************************************************/

/* Collide a nick on timeout. */

static void timeout_collide(Timeout *t)
{
    NickInfo *ni = t->data;
    User *u;

    rem_ns_timeout(ni, TO_COLLIDE);
    /* If they identified or don't exist anymore, don't kill them. */
    if ((ni->status & NS_IDENTIFIED)
		|| !(u = finduser(ni->nick))
		|| u->my_signon > t->settime)
	return;
    /* The RELEASE timeout will always add to the beginning of the
     * list, so we won't see it.  Which is fine because it can't be
     * triggered yet anyway. */
    collide(ni, 1);
}

/*************************************************************************/

/* Release a nick on timeout. */

static void timeout_release(Timeout *t)
{
    NickInfo *ni = t->data;

    rem_ns_timeout(ni, TO_RELEASE);
    release(ni, 1);
}

/*************************************************************************/

/* Add a collide/release timeout. */

void add_ns_timeout(NickInfo *ni, int type, time_t delay)
{
    Timeout *to;
    struct my_timeout *t;
    void (*timeout_routine)(Timeout *);

    if (type == TO_COLLIDE)
	timeout_routine = timeout_collide;
    else if (type == TO_RELEASE)
	timeout_routine = timeout_release;
    else {
	log("NickServ: unknown timeout type %d!  ni=%p (%s), delay=%ld",
		type, ni, ni->nick, delay);
	return;
    }
    to = add_timeout(delay, timeout_routine, 0);
    to->data = ni;
    t = smalloc(sizeof(*t));
    t->next = my_timeouts;
    my_timeouts = t;
    t->prev = NULL;
    t->ni = ni;
    t->to = to;
    t->type = type;
}

/*************************************************************************/

/* Delete a collide/release timeout. */

static void del_ns_timeout(NickInfo *ni, int type)
{
    struct my_timeout *t, *t2;

    t = my_timeouts;
    while (t) {
	if (t->ni == ni && t->type == type) {
	    t2 = t->next;
	    if (t->next)
		t->next->prev = t->prev;
	    if (t->prev)
		t->prev->next = t->next;
	    else
		my_timeouts = t->next;
	    del_timeout(t->to);
	    free(t);
	    t = t2;
	} else {
	    t = t->next;
	}
    }
}

/*************************************************************************/
/*********************** NickServ command routines ***********************/
/*************************************************************************/

static void do_credits(User *u)
{
    notice_lang(s_NickServ, u, SERVICES_CREDITS);
}    

/*************************************************************************/    

/* Return a help message. */

static void do_help(User *u)
{
    char *cmd = strtok(NULL, "");

    if (!cmd) {
	if (NSExpire >= 86400)
	    notice_help(s_NickServ, u, NICK_HELP, NSExpire/86400);
	else
	    notice_help(s_NickServ, u, NICK_HELP_EXPIRE_ZERO);
	if (is_services_oper(u))
	    notice_help(s_NickServ, u, NICK_SERVADMIN_HELP);
    } else if (stricmp(cmd, "SET LANGUAGE") == 0) {
	int i;
	notice_help(s_NickServ, u, NICK_HELP_SET_LANGUAGE);
	for (i = 0; i < NUM_LANGS && langlist[i] >= 0; i++) {
#ifdef IRC_UNDERNET_P10
	    privmsg(s_NickServ, u->numerico, "    %2d) %s",
#else
            privmsg(s_NickServ, u->nick, "    %2d) %s",
#endif	    
			i+1, langnames[langlist[i]]);
	}
    } else {
	help_cmd(s_NickServ, u, cmds, cmd);
    }
}

/*************************************************************************/

/* Register a nick. */

static void do_register(User *u)
{

    NickInfo *ni;
#ifdef REG_NICK_MAIL
    char *email = strtok(NULL, " ");
    int i, nicksmail = 0;
#else
    char *pass = strtok(NULL, " ");
#endif
 /*   static char saltChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char salt[3];
    char * plaintext;
    *
    srandom(time(0));		 may not be the BEST salt, but its close
    salt[0] = saltChars[random() % 64];
    salt[1] = saltChars[random() % 64];
    salt[2] = 0;
    */
    if (readonly) {
	notice_lang(s_NickServ, u, NICK_REGISTRATION_DISABLED);
	return;
    }

#if defined (IRC_HISPANO) || defined (IRC_TERRA)
    /* Prevent "Guest" nicks from being registered. -TheShadow */
    if (NSForceNickChange) {
	int prefixlen = strlen(NSGuestNickPrefix);
	int nicklen = strlen(u->nick);

	/* A guest nick is defined as a nick...
	 * 	- starting with NSGuestNickPrefix
	 * 	- with a series of between, and including, 2 and 7 digits
	 * -TheShadow
	 */
	if (nicklen <= prefixlen+13 && nicklen >= prefixlen+2 &&
			stristr(u->nick, NSGuestNickPrefix) == u->nick &&
			strspn(u->nick+prefixlen, "1234567890") ==
							nicklen-prefixlen) {
	    notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED, u->nick);
	    return;
	}
    }
#endif

#ifdef REG_NICK_MAIL
    if (!email || (stricmp(email, u->nick) == 0 && strtok(NULL, " "))) {
	syntax_error(s_NickServ, u, "REGISTER", NICK_REGISTER_SYNTAX);
#else
    if (!pass || (stricmp(pass, u->nick) == 0 && strtok(NULL, " "))) {
        syntax_error(s_NickServ, u, "REGISTER", NICK_REGISTER_SYNTAX);
#endif
    } else if (time(NULL) < u->lastnickreg + NSRegDelay) {
	notice_lang(s_NickServ, u, NICK_REG_PLEASE_WAIT, NSRegDelay);

    } else if (u->real_ni) {	/* i.e. there's already such a nick regged */
	if (u->real_ni->status & NS_VERBOTEN) {
	    log("%s: %s@%s tried to register FORBIDden nick %s", s_NickServ,
			u->username, u->host, u->nick);
	    notice_lang(s_NickServ, u, NICK_CANNOT_BE_REGISTERED, u->nick);
	} else {
	    notice_lang(s_NickServ, u, NICK_ALREADY_REGISTERED, u->nick);
	}
#ifdef REG_NICK_MAIL
    } else if (
          !strchr(email,'@') ||
               strchr(email,'@') != strrchr(email,'@') ||
                   !strchr(email,'.') ||
                               strchr(email,'|') ) {
        notice_lang(s_NickServ, u, NICK_MAIL_INVALID);
        syntax_error(s_NickServ, u, "REGISTER", NICK_REGISTER_SYNTAX);
    } else {
        strlower(email);
        for(i=0; i < 256; i++)
          for (ni = nicklists[i]; ni; ni = ni->next)
             if(ni->email && !strcmp(email,ni->email))
                nicksmail++;
        if (nicksmail > NicksMail) {
            notice_lang(s_NickServ, u, NICK_MAIL_ABUSE, NicksMail);
            return;
        }
  
#else
    } else if (stricmp(u->nick, pass) == 0
            || (StrictPasswords && strlen(pass) < 5)) {
        notice_lang(s_NickServ, u, MORE_OBSCURE_PASSWORD);
    } else {

#endif
	ni = makenick(u->nick);
	if (ni) {
#ifdef REG_NICK_MAIL
/*** Registro de nicks por mail by Zoltan ***/

            char pass[255];
            static char saltChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]";
	    char salt[13];
	    int cnt = 0;
	    unsigned long long xi;
	    salt[12] = '\0';
            __asm__ volatile (".byte 0x0f, 0x31" : "=A" (xi));
	    srandom(xi);

	    for(cnt = 0; cnt < 12; ++cnt)
		  salt[cnt] = saltChars[random() % 64];

            sprintf(pass,"%s",salt);

            strscpy(ni->pass, pass, PASSMAX);
            ni->email = sstrdup(email);

            {
            char *buf;
            char subject[BUFSIZE];                        
            if (fork()==0) {

               buf = smalloc(sizeof(char *) * 1024);
               sprintf(buf,"\n    NiCK: %s\n"
                             "Password: %s\n\n"
                             "Para identificarte   -> /nick %s:%s\n"
                             "Para cambio de clave -> /msg %s SET PASSWORD nueva_password\n\n"
                             "P谩gina de Informaci贸n %s\n",
                       ni->nick, ni->pass, ni->nick, ni->pass, s_NickServ, WebNetwork);
       
               snprintf(subject, sizeof(subject), "Registro del NiCK '%s'", ni->nick);
               
               enviar_correo(ni->email, subject, buf);
	       do_write_bdd(ni->nick, 1, ni->pass);
	       ni->status |= NI_ON_BDD;
	       notice_lang(s_NickServ, u, NICK_BDD_NEW_REG,ni->pass, ni->nick, ni->pass);
	       send_cmd(NULL, "RENAME %s", ni->nick);
               exit(0);
            }
           }                                                                                                                                                                      

#else	
#ifdef USE_ENCRYPTION
	    int len = strlen(pass);
	    if (len > PASSMAX) {
		len = PASSMAX;
		pass[len] = 0;
		notice_lang(s_NickServ, u, PASSWORD_TRUNCATED, PASSMAX);
	    }
	    if (encrypt(pass, len, ni->pass, PASSMAX) < 0) {
		memset(pass, 0, strlen(pass));
		log("%s: Failed to encrypt password for %s (register)",
			s_NickServ, u->nick);
		notice_lang(s_NickServ, u, NICK_REGISTRATION_FAILED);
		return;
	    }
	    memset(pass, 0, strlen(pass));
	    ni->status = NS_ENCRYPTEDPW | NS_IDENTIFIED | NS_RECOGNIZED;
#else
	    if (strlen(pass) > PASSMAX-1) /* -1 for null byte */
		notice_lang(s_NickServ, u, PASSWORD_TRUNCATED, PASSMAX-1);
	    strscpy(ni->pass, pass, PASSMAX);
	    ni->status = NS_IDENTIFIED | NS_RECOGNIZED;
#endif
#endif /* REG_NICK_MAIL */

	    ni->flags = 0;
	    if (NSDefKill)
		ni->flags |= NI_KILLPROTECT;
	    if (NSDefKillQuick)
		ni->flags |= NI_KILL_QUICK;
	    if (NSDefSecure)
		ni->flags |= NI_SECURE;
	    if (NSDefPrivate)
		ni->flags |= NI_PRIVATE;
	    if (NSDefHideEmail)
		ni->flags |= NI_HIDE_EMAIL;
	    if (NSDefHideUsermask)
		ni->flags |= NI_HIDE_MASK;
	    if (NSDefHideQuit)
		ni->flags |= NI_HIDE_QUIT;
	    if (NSDefMemoSignon)
		ni->flags |= NI_MEMO_SIGNON;
	    if (NSDefMemoReceive)
		ni->flags |= NI_MEMO_RECEIVE;
	    ni->memos.memomax = MSMaxMemos;
	    ni->channelcount = 0;
	    ni->channelmax = CSMaxReg;
	    ni->last_usermask = smalloc(strlen(u->username)+strlen(u->host)+2);
	    sprintf(ni->last_usermask, "%s@%s", u->username, u->host);
	    ni->last_realname = sstrdup(u->realname);
	    ni->time_registered = ni->last_seen = time(NULL);
	    ni->accesscount = 1;
	    ni->access = smalloc(sizeof(char *));
	    ni->access[0] = create_mask(u);
	    ni->language = DEF_LANGUAGE;
	    ni->link = NULL;
	    u->ni = u->real_ni = ni;
#ifdef REG_NICK_MAIL
            log("%s: %s' registered by %s@%s Email: %s Pass: %s", s_NickServ,
                   u->nick, u->username, u->host, ni->email, ni->pass);                
            notice_lang(s_NickServ, u, NICK_REGISTERED, u->nick, ni->email);
            notice_lang(s_NickServ, u, NICK_IN_MAIL);
#else                                            
	    log("%s: `%s' registered by %s@%s", s_NickServ,
			u->nick, u->username, u->host);
	    notice_lang(s_NickServ, u, NICK_REGISTERED, u->nick, ni->access[0]);
	    
#ifndef USE_ENCRYPTION
	    notice_lang(s_NickServ, u, NICK_PASSWORD_IS, ni->pass);
#endif
#endif
	    u->lastnickreg = time(NULL);
#if defined (IRC_DAL4_4_15) || defined (IRC_BAHAMUT)
	    send_cmd(ServerName, "SVSMODE %s +r", u->nick);
#endif
	} else {
	    log("%s: makenick(%s) failed", s_NickServ, u->nick);
	    notice_lang(s_NickServ, u, NICK_REGISTRATION_FAILED);
	}

    }

}

/*************************************************************************/

static void do_identify(User *u)
{
    char *pass = strtok(NULL, " ");
    NickInfo *ni;
    int res;

    if (!pass) {
	syntax_error(s_NickServ, u, "IDENTIFY", NICK_IDENTIFY_SYNTAX);

    } else if (!(ni = u->real_ni)) {
        privmsg(s_NickServ, u->nick, "Tu nick no esta registrado");
    } else if (ni->status & NS_SUSPENDED) {
        notice_lang(s_NickServ, u, NICK_SUSPENDED, ni->suspendreason);
    } else if (!(res = check_password(pass, ni->pass))) {
	log("%s: Failed IDENTIFY for %s!%s@%s",
		s_NickServ, u->nick, u->username, u->host);
	notice_lang(s_NickServ, u, PASSWORD_INCORRECT);
	bad_password(u);

    } else if (res == -1) {
	notice_lang(s_NickServ, u, NICK_IDENTIFY_FAILED);

    } else {
        if (ni->status & NS_IDENTIFIED)
           notice_lang(s_NickServ, u, NICK_IS_IDENTIFIED);
	ni->id_timestamp = u->signon;
	if (!(ni->status & NS_RECOGNIZED)) {
	    ni->last_seen = time(NULL);
	    if (ni->last_usermask)
		free(ni->last_usermask);
	    ni->last_usermask = smalloc(strlen(u->username)+strlen(u->host)+2);
	    sprintf(ni->last_usermask, "%s@%s", u->username, u->host);
	    if (ni->last_realname)
		free(ni->last_realname);
	    ni->last_realname = sstrdup(u->realname);
	}
#if defined (IRC_TERRA)
	send_cmd(ServerName, "SVSMODE %s +r", u->nick);
#endif
	log("%s: %s!%s@%s identified for nick %s", s_NickServ,
			u->nick, u->username, u->host, u->nick);
        if (!(ni->status & NS_IDENTIFIED))
          notice_lang(s_NickServ, u, NICK_IDENTIFY_SUCCEEDED);
        ni->status |= NS_IDENTIFIED;
	if (!(ni->status & NS_RECOGNIZED))
	    check_memos(u);
	strcpy(ni->nick, u->nick);    

    }
}

/*************************************************************************/

static void do_drop(User *u)
{
    char *nick = strtok(NULL, " ");
    NickInfo *ni;
    User *u2;

    if (readonly && !is_services_oper(u)) {
	notice_lang(s_NickServ, u, NICK_DROP_DISABLED);
	return;
    }

    if (!is_services_oper(u) && nick) {
	syntax_error(s_NickServ, u, "DROP", NICK_DROP_SYNTAX);

    } else if (!(ni = (nick ? findnick(nick) : u->real_ni))) {
	if (nick)
	    notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
	else
	    notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);

    } else if (NSSecureAdmins && nick && (nick_is_services_admin(ni) 
            || nick_is_services_oper(ni)) && !is_services_root(u)) {
	notice_lang(s_NickServ, u, PERMISSION_DENIED);
	
    } else if (!nick && !nick_identified(u)) {
	notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED, s_NickServ);

    } else if (ni->status & NS_VERBOTEN) {
        notice_lang(s_NickServ, u, NICK_DROP_FORBIDDEN);
            

    } else {
	if (readonly)
	    notice_lang(s_NickServ, u, READ_ONLY_MODE);
	delnick(ni);
	canalopers(s_NickServ, "12%s elimin贸 el nick 12%s", u->nick, nick);
	log("%s: %s!%s@%s dropped nickname %s", s_NickServ, u->nick, u->username, u->host, nick ? nick : u->nick);
	if (nick)
	    notice_lang(s_NickServ, u, NICK_X_DROPPED, nick);
	else
	    notice_lang(s_NickServ, u, NICK_DROPPED);
	if (nick && (u2 = finduser(nick)))
	    u2->ni = u2->real_ni = NULL;
	else if (!nick)
	    u->ni = u->real_ni = NULL;
    }
}

/*************************************************************************/

static void do_set(User *u)
{
    char *cmd    = strtok(NULL, " ");
    char *param  = strtok(NULL, " ");
    NickInfo *ni;
    int is_servadmin = is_services_admin(u);
    int set_nick = 0;

    if (readonly) {
	notice_lang(s_NickServ, u, NICK_SET_DISABLED);
	return;
    }

    if (is_servadmin && cmd && (ni = findnick(cmd))) {
        if (nick_is_services_admin(ni) && !is_services_root(u)) {
            notice_lang(s_NickServ, u, PERMISSION_DENIED);
            return;
        }
                                        
	cmd = param;
	param = strtok(NULL, " ");
	set_nick = 1;
    } else {
	ni = u->ni;
    }
    if (!param && (!cmd || (stricmp(cmd,"URL")!=0 && stricmp(cmd,"EMAIL")!=0))){
	if (is_servadmin) {
	    syntax_error(s_NickServ, u, "SET", NICK_SET_SERVADMIN_SYNTAX);
	} else {
	    syntax_error(s_NickServ, u, "SET", NICK_SET_SYNTAX);
	}
	notice_lang(s_NickServ, u, MORE_INFO, s_NickServ, "SET");
    } else if (!ni) {
	notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);
    } else if (!is_servadmin && !nick_identified(u)) {
	notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED, s_NickServ);
    } else if (stricmp(cmd, "PASSWORD") == 0) {
	do_set_password(u, set_nick ? ni : u->real_ni, param);
    } else if (stricmp(cmd, "PASS") == 0) {
        do_set_password(u, set_nick ? ni : u->real_ni, param);            	
    } else if (stricmp(cmd, "LANGUAGE") == 0) {
	do_set_language(u, ni, param);
    } else if (stricmp(cmd, "URL") == 0) {
	do_set_url(u, set_nick ? ni : u->real_ni, param);
    } else if ((stricmp(cmd, "EMAIL") == 0) && is_services_admin(u)) {
	do_set_email(u, set_nick ? ni : u->real_ni, param);
    } else if (stricmp(cmd, "KILL") == 0) {
	do_set_kill(u, ni, param);
    } else if (stricmp(cmd, "SECURE") == 0) {
	do_set_secure(u, ni, param);
    } else if (stricmp(cmd, "PRIVATE") == 0) {
	do_set_private(u, ni, param);
    } else if (stricmp(cmd, "HIDE") == 0) {
	do_set_hide(u, ni, param);
    } else if (stricmp(cmd, "NOEXPIRE") == 0) {
	do_set_noexpire(u, ni, param);
    } else if (stricmp(cmd, "VHOST") == 0) {
        do_set_vhost(u, ni, param);
    } else if (stricmp(cmd, "IPVIRTUAL") == 0) {
        do_set_vhost(u, ni, param);
	
    } else {
	if (is_servadmin)
	    notice_lang(s_NickServ, u, NICK_SET_UNKNOWN_OPTION_OR_BAD_NICK,
			strupper(cmd));
	else
	    notice_lang(s_NickServ, u, NICK_SET_UNKNOWN_OPTION, strupper(cmd));
    }
}

/*************************************************************************/
    
static void do_set_password(User *u, NickInfo *ni, char *param)
{
    int len = strlen(param);

    if (NSSecureAdmins && u->real_ni != ni && nick_is_services_admin(ni) && 
    							!is_services_root(u)) {
	notice_lang(s_NickServ, u, PERMISSION_DENIED);
	return;
    } else if (stricmp(ni->nick, param) == 0 || (StrictPasswords && len < 5)) {
	notice_lang(s_NickServ, u, MORE_OBSCURE_PASSWORD);
	return;
    }

#ifdef USE_ENCRYPTION
    if (len > PASSMAX) {
	len = PASSMAX;
	param[len] = 0;
	notice_lang(s_NickServ, u, PASSWORD_TRUNCATED, PASSMAX);
    }
    if (encrypt(param, len, ni->pass, PASSMAX) < 0) {
	memset(param, 0, strlen(param));
	log("%s: Failed to encrypt password for %s (set)",
		s_NickServ, ni->nick);
	notice_lang(s_NickServ, u, NICK_SET_PASSWORD_FAILED);
	return;
    }
    memset(param, 0, strlen(param));
    notice_lang(s_NickServ, u, NICK_SET_PASSWORD_CHANGED);
#else
    if (strlen(param) > PASSMAX-1) /* -1 for null byte */
	notice_lang(s_NickServ, u, PASSWORD_TRUNCATED, PASSMAX-1);
    strscpy(ni->pass, param, PASSMAX);
    notice_lang(s_NickServ, u, NICK_SET_PASSWORD_CHANGED_TO, ni->pass);
  
    if (ni->status & NI_ON_BDD)
       do_write_bdd(ni->nick, 1, ni->pass);
    
#endif
    if (u->real_ni != ni) {
        canalopers(s_NickServ, "12%s  ha usado SET PASSWORD sobre 12%s",
         u->nick, ni->nick);
	log("%s: %s!%s@%s used SET PASSWORD as Services admin on %s",
		s_NickServ, u->nick, u->username, u->host, ni->nick);	
    }
}

/*************************************************************************/

static void do_set_language(User *u, NickInfo *ni, char *param)
{
    int langnum;

    if (param[strspn(param, "0123456789")] != 0) {  /* i.e. not a number */
	syntax_error(s_NickServ, u, "SET LANGUAGE", NICK_SET_LANGUAGE_SYNTAX);
	return;
    }
    langnum = atoi(param)-1;
    if (langnum < 0 || langnum >= NUM_LANGS || langlist[langnum] < 0) {
	notice_lang(s_NickServ, u, NICK_SET_LANGUAGE_UNKNOWN,
		langnum+1, s_NickServ);
	return;
    }
    ni->language = langlist[langnum];
    notice_lang(s_NickServ, u, NICK_SET_LANGUAGE_CHANGED);
}

/*************************************************************************/

static void do_set_url(User *u, NickInfo *ni, char *param)
{
    if (ni->url)
	free(ni->url);
    if (param) {
	ni->url = sstrdup(param);
	notice_lang(s_NickServ, u, NICK_SET_URL_CHANGED, param);
    } else {
	ni->url = NULL;
	notice_lang(s_NickServ, u, NICK_SET_URL_UNSET);
    }
}

/*************************************************************************/

static void do_set_vhost(User *u, NickInfo *ni, char *param)
{
    if (ni->flags & NI_IPVIRTUALV) {
        privmsg(s_NickServ, u->nick, "No puedes usar este comando. Tienes una IPvirtual asignada por %s", s_OperServ);
        return;
    }
    if (stricmp(param, "OFF") == 0) {
	 do_write_bdd(ni->nick, 4, "");
	 free(ni->vhost);
	 notice_lang(s_NickServ, u, NICK_SET_VHOST_OFF);
    } else {
        if ( (strlen(param) > 45) || (strlen(param) < 3) ) {
            privmsg(s_NickServ, u->nick, "Tu IP virtual debe estar entre 3 y 45 caracteres.");
            return;
        }
        if (!valid_ipvirtual(param)) {
            privmsg(s_NickServ, u->nick, "La IP virtual que intenta ponerse contiene caracteres ilegales.");
            privmsg(s_NickServ, u->nick, "Recuerde que los car醕teres v醠idos son 'A'-'Z', 'a'-'z', '0'-'9', '.' y '-', y no puede terminar en ninguno de los dos 鷏timos.");
            return;
        }
        do_write_bdd(ni->nick, 4, param);
        ni->vhost = sstrdup(param);
	notice_lang(s_NickServ, u, NICK_SET_VHOST_ON, param);
    }
}
	
static void do_set_email(User *u, NickInfo *ni, char *param)
{
    if (ni->email)
	free(ni->email);
    if (param) {
	ni->email = sstrdup(param);
	notice_lang(s_NickServ, u, NICK_SET_EMAIL_CHANGED, param);
    } else {
	ni->email = NULL;
	notice_lang(s_NickServ, u, NICK_SET_EMAIL_UNSET);
    }
}

/*************************************************************************/

static void do_set_kill(User *u, NickInfo *ni, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ni->flags |= NI_KILLPROTECT;
	ni->flags &= ~(NI_KILL_QUICK | NI_KILL_IMMED);
	notice_lang(s_NickServ, u, NICK_SET_KILL_ON);
    } else if (stricmp(param, "QUICK") == 0) {
	ni->flags |= NI_KILLPROTECT | NI_KILL_QUICK;
	ni->flags &= ~NI_KILL_IMMED;
	notice_lang(s_NickServ, u, NICK_SET_KILL_QUICK);
    } else if (stricmp(param, "IMMED") == 0) {
	if (NSAllowKillImmed) {
	    ni->flags |= NI_KILLPROTECT | NI_KILL_IMMED;
	    ni->flags &= ~NI_KILL_QUICK;
	    notice_lang(s_NickServ, u, NICK_SET_KILL_IMMED);
	} else {
	    notice_lang(s_NickServ, u, NICK_SET_KILL_IMMED_DISABLED);
	}
    } else if (stricmp(param, "OFF") == 0) {
	ni->flags &= ~(NI_KILLPROTECT | NI_KILL_QUICK | NI_KILL_IMMED);
	notice_lang(s_NickServ, u, NICK_SET_KILL_OFF);
    } else {
	syntax_error(s_NickServ, u, "SET KILL",
		NSAllowKillImmed ? NICK_SET_KILL_IMMED_SYNTAX
		                 : NICK_SET_KILL_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_secure(User *u, NickInfo *ni, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ni->flags |= NI_SECURE;
	notice_lang(s_NickServ, u, NICK_SET_SECURE_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ni->flags &= ~NI_SECURE;
	notice_lang(s_NickServ, u, NICK_SET_SECURE_OFF);
    } else {
	syntax_error(s_NickServ, u, "SET SECURE", NICK_SET_SECURE_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_private(User *u, NickInfo *ni, char *param)
{
    if (stricmp(param, "ON") == 0) {
	ni->flags |= NI_PRIVATE;
	notice_lang(s_NickServ, u, NICK_SET_PRIVATE_ON);
    } else if (stricmp(param, "OFF") == 0) {
	ni->flags &= ~NI_PRIVATE;
	notice_lang(s_NickServ, u, NICK_SET_PRIVATE_OFF);
    } else {
	syntax_error(s_NickServ, u, "SET PRIVATE", NICK_SET_PRIVATE_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_hide(User *u, NickInfo *ni, char *param)
{
    int flag, onmsg, offmsg;

    if (stricmp(param, "EMAIL") == 0) {
	flag = NI_HIDE_EMAIL;
	onmsg = NICK_SET_HIDE_EMAIL_ON;
	offmsg = NICK_SET_HIDE_EMAIL_OFF;
    } else if (stricmp(param, "USERMASK") == 0) {
	flag = NI_HIDE_MASK;
	onmsg = NICK_SET_HIDE_MASK_ON;
	offmsg = NICK_SET_HIDE_MASK_OFF;
    } else if (stricmp(param, "QUIT") == 0) {
	flag = NI_HIDE_QUIT;
	onmsg = NICK_SET_HIDE_QUIT_ON;
	offmsg = NICK_SET_HIDE_QUIT_OFF;
    } else {
	syntax_error(s_NickServ, u, "SET HIDE", NICK_SET_HIDE_SYNTAX);
	return;
    }
    param = strtok(NULL, " ");
    if (!param) {
	syntax_error(s_NickServ, u, "SET HIDE", NICK_SET_HIDE_SYNTAX);
    } else if (stricmp(param, "ON") == 0) {
	ni->flags |= flag;
	notice_lang(s_NickServ, u, onmsg, s_NickServ);
    } else if (stricmp(param, "OFF") == 0) {
	ni->flags &= ~flag;
	notice_lang(s_NickServ, u, offmsg, s_NickServ);
    } else {
	syntax_error(s_NickServ, u, "SET HIDE", NICK_SET_HIDE_SYNTAX);
    }
}

/*************************************************************************/

static void do_set_noexpire(User *u, NickInfo *ni, char *param)
{
    if (!is_services_admin(u)) {
	notice_lang(s_NickServ, u, PERMISSION_DENIED);
	return;
    }
    if (!param) {
	syntax_error(s_NickServ, u, "SET NOEXPIRE", NICK_SET_NOEXPIRE_SYNTAX);
	return;
    }
    if (stricmp(param, "ON") == 0) {
	ni->status |= NS_NO_EXPIRE;
	notice_lang(s_NickServ, u, NICK_SET_NOEXPIRE_ON, ni->nick);
    } else if (stricmp(param, "OFF") == 0) {
	ni->status &= ~NS_NO_EXPIRE;
	notice_lang(s_NickServ, u, NICK_SET_NOEXPIRE_OFF, ni->nick);
    } else {
	syntax_error(s_NickServ, u, "SET NOEXPIRE", NICK_SET_NOEXPIRE_SYNTAX);
    }
}

/*************************************************************************/

static void do_userip(User *u)
{
    char *nick = strtok(NULL, " ");
    User *u2;
    struct hostent *hp;
    //struct in_addr inaddr;

   if (!nick) {
     	syntax_error(s_NickServ,u, "USERIP", NICK_USERIP_SYNTAX);
     } else if (!(u2 = finduser(nick))) {
     	  notice_lang(s_NickServ, u, NICK_USERIP_CHECK_NO, nick);
     } else {
	  if ((hp=gethostbyname(u2->host)) == NULL) {
	  privmsg(s_NickServ, u->nick, "Fallo al intentar resolver %s", u2->host);
	  return;
	  }
	  notice_lang(s_NickServ, u, NICK_USERIP_CHECK_OK, nick, u2->host, inet_ntoa(*((struct in_addr *)hp->h_addr)));
	  canalopers(s_NickServ, "12%s uso USERIP sobre 12%s.",u->nick, nick);
     }
}


/**************************************************************************/ 
      
#ifdef quitar
static void do_access(User *u)
{
    char *cmd = strtok(NULL, " ");
    char *mask = strtok(NULL, " ");
    NickInfo *ni;
    int i;
    char **access;

    if (cmd && stricmp(cmd, "LIST") == 0 && mask && is_services_oper(u)
			&& (ni = findnick(mask))) {
	ni = getlink(ni);
	notice_lang(s_NickServ, u, NICK_ACCESS_LIST_X, mask);
	mask = strtok(NULL, " ");
	for (access = ni->access, i = 0; i < ni->accesscount; access++, i++) {
	    if (mask && !match_wild(mask, *access))
		continue;
#ifdef IRC_UNDERNET_P10
	    privmsg(s_NickServ, u->numerico, "    %s", *access);
#else
            notice(s_NickServ, u->nick, "    %s", *access);
#endif	    
	}

    } else if (!cmd || ((stricmp(cmd,"LIST")==0) ? !!mask : !mask)) {
	syntax_error(s_NickServ, u, "ACCESS", NICK_ACCESS_SYNTAX);

    } else if (mask && !strchr(mask, '@')) {
	notice_lang(s_NickServ, u, BAD_USERHOST_MASK);
	notice_lang(s_NickServ, u, MORE_INFO, s_NickServ, "ACCESS");

    } else if (!(ni = u->ni)) {
	notice_lang(s_NickServ, u, NICK_NOT_REGISTERED);

    } else if (!nick_identified(u)) {
	notice_lang(s_NickServ, u, NICK_IDENTIFY_REQUIRED, s_NickServ);

    } else if (stricmp(cmd, "ADD") == 0) {
	if (ni->accesscount >= NSAccessMax) {
	    notice_lang(s_NickServ, u, NICK_ACCESS_REACHED_LIMIT, NSAccessMax);
	    return;
	}
	for (access = ni->access, i = 0; i < ni->accesscount; access++, i++) {
	    if (strcmp(*access, mask) == 0) {
		notice_lang(s_NickServ, u,
			NICK_ACCESS_ALREADY_PRESENT, *access);
		return;
	    }
	}
	ni->accesscount++;
	ni->access = srealloc(ni->access, sizeof(char *) * ni->accesscount);
	ni->access[ni->accesscount-1] = sstrdup(mask);
	notice_lang(s_NickServ, u, NICK_ACCESS_ADDED, mask);

    } else if (stricmp(cmd, "DEL") == 0) {
	/* First try for an exact match; then, a case-insensitive one. */
	for (access = ni->access, i = 0; i < ni->accesscount; access++, i++) {
	    if (strcmp(*access, mask) == 0)
		break;
	}
	if (i == ni->accesscount) {
	    for (access = ni->access, i = 0; i < ni->accesscount;
							access++, i++) {
		if (stricmp(*access, mask) == 0)
		    break;
	    }
	}
	if (i == ni->accesscount) {
	    notice_lang(s_NickServ, u, NICK_ACCESS_NOT_FOUND, mask);
	    return;
	}
	notice_lang(s_NickServ, u, NICK_ACCESS_DELETED, *access);
	free(*access);
	ni->accesscount--;
	if (i < ni->accesscount)	/* if it wasn't the last entry... */
	    memmove(access, access+1, (ni->accesscount-i) * sizeof(char *));
	if (ni->accesscount)		/* if there are any entries left... */
	    ni->access = srealloc(ni->access, ni->accesscount * sizeof(char *));
	else {
	    free(ni->access);
	    ni->access = NULL;
	}

    } else if (stricmp(cmd, "LIST") == 0) {
	notice_lang(s_NickServ, u, NICK_ACCESS_LIST);
	for (access = ni->access, i = 0; i < ni->accesscount; access++, i++) {
	    if (mask && !match_wild(mask, *access))
		continue;
	    privmsg(s_NickServ, u->nick, "    %s", *access);
	}

    } else {
	syntax_error(s_NickServ, u, "ACCESS", NICK_ACCESS_SYNTAX);

    }
}
#endif


/*************************************************************************/

static void do_info(User *u)
{
    char *nick = strtok(NULL, " ");
    char *param = strtok(NULL, " ");
    NickInfo *ni, *ni2, *real;
    int is_servoper = is_services_oper(u);
    int i;

    if (!nick) {
    	syntax_error(s_NickServ, u, "INFO", NICK_INFO_SYNTAX);
    } else if (is_a_service(nick)) {
    	notice_lang(s_NickServ, u, NICK_IS_A_SERVICE, nick);
    } else if (!(ni = findnick(nick))) {
	notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
    } else if (ni->status & NS_VERBOTEN) {
	notice_lang(s_NickServ, u, NICK_X_FORBIDDEN, nick);
        privmsg(s_NickServ, u->nick, "Motivo: 12%s", ni->forbidreason);
        if (is_services_oper(u))
            privmsg(s_NickServ, u->nick, "Forbideado por: 12%s", ni->forbidby);

    } else {
	struct tm *tm;
	char buf[BUFSIZE], *end;
	const char *commastr = getstring(u->ni, COMMA_SPACE);
	int need_comma = 0;
	int nick_online = 0;
	int show_hidden = 0;

	/* Is the real owner of the nick we're looking up online? -TheShadow */
	if (ni->status & NS_IDENTIFIED)
	    nick_online = 0;

        /* Only show hidden fields to owner and sadmins and only when the ALL
	 * parameter is used. -TheShadow */
        if (param && stricmp(param, "ALL") == 0 &&
			((nick_online && (stricmp(u->nick, nick) == 0)) ||
                        	is_services_oper(u)))
            show_hidden = 1;

	real = getlink(ni);

        if(stricmp(ni->nick,real->nick)!=0)
            notice_lang(s_NickServ, u, NICK_INFO_LINKED, real->nick);

        if (ni->status & NS_SUSPENDED) {
            notice_lang(s_NickServ, u, NICK_INFO_SUSPENDED, ni->suspendreason);
            if (show_hidden) {
                char timebuf[32], expirebuf[256];
//                time_t now = time(NULL);            
                privmsg(s_NickServ, u->nick, "Suspendido por: 12%s", ni->suspendby);
                tm = localtime(&ni->time_suspend);
                strftime_lang(timebuf, sizeof(timebuf), u, STRFTIME_DATE_TIME_FORMAT, tm);
                privmsg(s_NickServ, u->nick, "Fecha de la suspensi贸n: 12%s", timebuf);
                if (ni->time_expiresuspend == 0) {
                    snprintf(expirebuf, sizeof(expirebuf),
                           getstring(u->ni, OPER_AKILL_NO_EXPIRE));
/***
                } else if (ni->time_expiresuspend <= now) {
                    snprintf(expirebuf, sizeof(expirebuf),
                              getstring(u->ni, OPER_AKILL_EXPIRES_SOON));
                } else {
                    expires_in_lang(expirebuf, sizeof(expirebuf), u,
                              ni->time_expiresuspend - now + 59);
  */            }                           
                privmsg(s_NickServ, u->nick, "La suspensi贸n expira en: 12%s", expirebuf);
            }  
                                                            
        }    
	notice_lang(s_NickServ, u, NICK_INFO_REALNAME,
		ni->nick, ni->last_realname);
	
	tm = localtime(&ni->time_registered);
	strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
	notice_lang(s_NickServ, u, NICK_INFO_TIME_REGGED, buf);
	
	if (nick_online) {
		 if (is_services_admin(u))
		   notice_lang(s_NickServ, u, NICK_INFO_ADDRESS_ONLINE, ni->last_usermask);
	} else {
		if (is_services_admin(u))
			notice_lang(s_NickServ, u, NICK_INFO_ADDRESS, ni->last_usermask);
	}
				  /*			
#ifdef DB_NETWORKS
            if (show_hidden)
#else
	    if (show_hidden || !(real->flags & NI_HIDE_MASK))
#endif	    
		 notice_lang(s_NickServ, u, NICK_INFO_ADDRESS_ONLINE,
			ni->last_usermask); 
	    else
		 notice_lang(s_NickServ, u, NICK_INFO_ADDRESS_ONLINE_NOHOST,
			ni->nick); 

	}  else {
#ifdef DB_NETWORKS	
	    if (show_hidden)
#else
            if (show_hidden || !(real->flags & NI_HIDE_MASK))
#endif	    
		notice_lang(s_NickServ, u, NICK_INFO_ADDRESS,
			ni->last_usermask);
*/
            tm = localtime(&ni->last_seen);
            strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
            notice_lang(s_NickServ, u, NICK_INFO_LAST_SEEN, buf);
//	} 	    
	    
/***/
	if ((ni->last_quit && (show_hidden || !(real->flags & NI_HIDE_QUIT))) && !(ni->status & NS_SUSPENDED))
	    notice_lang(s_NickServ, u, NICK_INFO_LAST_QUIT, ni->last_quit);
	if (ni->url)
	    notice_lang(s_NickServ, u, NICK_INFO_URL, ni->url);
/**	if (ni->email && (show_hidden || !(real->flags & NI_HIDE_EMAIL))) **/
        
        if (is_services_oper(u))        
	    notice_lang(s_NickServ, u, NICK_INFO_EMAIL, ni->email);

/*	if (ni->status & NI_ON_BDD)
	    notice_lang(s_NickServ, u, NICK_INFO_OPT_BDD);
*/

	/* if ((u->status & is_services_oper) && !(u->status & is_services_admin)
	    privmsg(s_NickServ, u->nick, "Es un OPERador de la RED"); */
	if (nick_is_services_oper(ni) && !(stricmp(ni->nick, u->nick) == 0))
	    privmsg(s_NickServ, ni->nick, "%s ha utilizado %s INFO sobre ti", u->nick, s_NickServ);
	    
	if (nick_is_services_oper(ni) && !nick_is_services_admin(ni) && !(stricmp(nick, ServicesRoot) ==0))
	    notice_lang(s_NickServ, u, NICK_INFO_SERV_OPER);
	       
	if (nick_is_services_admin(ni) && !(stricmp(nick, ServicesRoot) ==0))
	    notice_lang(s_NickServ, u, NICK_INFO_SERV_ADMIN);
	 
	if (stricmp(nick, ServicesRoot) ==0)
	    notice_lang(s_NickServ, u, NICK_INFO_SERV_ROOT);

	*buf = 0;
	end = buf;
	if (real->flags & NI_KILLPROTECT) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s",
			getstring(u->ni, NICK_INFO_OPT_KILL));
	    need_comma = 1;
	}
	if (real->flags & NI_SECURE) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
			need_comma ? commastr : "",
			getstring(u->ni, NICK_INFO_OPT_SECURE));
	    need_comma = 1;
	}
	if (real->flags & NI_ON_BDD) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s", need_comma ? commastr : "", getstring(u->ni, NICK_INFO_OPT_BDD));
	    need_comma = 1;
	}
	
	if (real->flags & NI_PRIVATE) {
	    end += snprintf(end, sizeof(buf)-(end-buf), "%s%s",
			need_comma ? commastr : "",
			getstring(u->ni, NICK_INFO_OPT_PRIVATE));
	    need_comma = 1;
	}
	notice_lang(s_NickServ, u, NICK_INFO_OPTIONS,
		*buf ? buf : getstring(u->ni, NICK_INFO_OPT_NONE));

	    
	if ((ni->status & NS_NO_EXPIRE) && (real == u->ni || is_servoper))
	    notice_lang(s_NickServ, u, NICK_INFO_NO_EXPIRE);
  
        if (is_services_oper(u) || (ni == u->ni) || (ni == u->real_ni) ) {
            registros(u, ni);

           
           for (i = 0; i < 256; i++)
              for (ni2 = nicklists[i]; ni2; ni2 = ni2->next)
                 if( ni2->link == ni ) {
                        notice_lang(s_NickServ, u, NICK_INFO_LINKS, ni2->nick, ni2->email);
                        registros(u, ni2);
                 }
        }
    }
}

/*************************************************************************/

/* SADMINS can search for nicks based on their NS_VERBOTEN and NS_NO_EXPIRE
 * status. The keywords FORBIDDEN and NOEXPIRE represent these two states
 * respectively. These keywords should be included after the search pattern.
 * Multiple keywords are accepted and should be separated by spaces. Only one
 * of the keywords needs to match a nick's state for the nick to be displayed.
 * Forbidden nicks can be identified by "[Forbidden]" appearing in the last
 * seen address field. Nicks with NOEXPIRE set are preceeded by a "!". Only
 * SADMINS will be shown forbidden nicks and the "!" indicator.
 * Syntax for sadmins: LIST pattern [FORBIDDEN] [NOEXPIRE]
 * -TheShadow
 */

static void do_list(User *u)
{
    char *pattern = strtok(NULL, " ");
    char *keyword;
    NickInfo *ni;
    int nnicks, i;
    char buf[BUFSIZE];
    int is_servadmin = is_services_admin(u);
    int16 matchflags = 0; /* NS_ flags a nick must match one of to qualify */

    if (NSListOpersOnly && !(u->mode & UMODE_O)) {
	notice_lang(s_NickServ, u, PERMISSION_DENIED);
	return;
    }

    if (!pattern) {
	syntax_error(s_NickServ, u, "LIST",
		is_servadmin ? NICK_LIST_SERVADMIN_SYNTAX : NICK_LIST_SYNTAX);
    } else {
	nnicks = 0;

	while (is_servadmin && (keyword = strtok(NULL, " "))) {
	    if (stricmp(keyword, "FORBIDDEN") == 0)
		matchflags |= NS_VERBOTEN;
	    if (stricmp(keyword, "NOEXPIRE") == 0)
		matchflags |= NS_NO_EXPIRE;
            if (stricmp(keyword, "SUSPENDED") == 0)
                matchflags |= NS_SUSPENDED;                            		
	}

	notice_lang(s_NickServ, u, NICK_LIST_HEADER, pattern);
	for (i = 0; i < 256; i++) {
	    for (ni = nicklists[i]; ni; ni = ni->next) {
		if (!is_servadmin && ((ni->flags & NI_PRIVATE)
           || (ni->status & NS_VERBOTEN) || (ni->status & NS_SUSPENDED)))
		    continue;
		if ((matchflags != 0) && !(ni->status & matchflags))
		    continue;

		/* We no longer compare the pattern against the output buffer.
		 * Instead we build a nice nick!user@host buffer to compare.
		 * The output is then generated separately. -TheShadow */
		snprintf(buf, sizeof(buf), "%s!%s", ni->nick,
				ni->last_usermask ? ni->last_usermask : "*@*");
		if (stricmp(pattern, ni->nick) == 0 ||
					match_wild_nocase(pattern, buf)) {
		    if (++nnicks <= NSListMax) {
			char noexpire_char = ' ';
			if (is_servadmin && (ni->status & NS_NO_EXPIRE))
			    noexpire_char = '!';
			if (!is_servadmin && (ni->flags & NI_HIDE_MASK)) {
			    snprintf(buf, sizeof(buf), "%-20s  [Oculto]",
						ni->nick);
			} else if (ni->status & NS_VERBOTEN) {
			    snprintf(buf, sizeof(buf), "%-20s  [Forbideado]",
						ni->nick);
                        } else if (ni->status & NS_SUSPENDED) {
                            snprintf(buf, sizeof(buf), "%-20s  [Suspendido]",
                                                ni->nick);                                                                                                    
			} else {
			    snprintf(buf, sizeof(buf), "%-20s  %s",
						ni->nick, ni->last_usermask);
			}
#ifdef IRC_UNDERNET_P10
			privmsg(s_NickServ, u->numerico, "   %c%s",
#else
                        privmsg(s_NickServ, u->nick, "   %c%s",
#endif			
						noexpire_char, buf);
		    }
		}
	    }
	}
	notice_lang(s_NickServ, u, NICK_LIST_RESULTS,
			nnicks>NSListMax ? NSListMax : nnicks, nnicks);
    }
}

/*************************************************************************/
static void do_ghost(User *u)
{
    privmsg(s_NickServ, u->nick, "Los comandos GHOST, RECOVER y RELEASE han sido deshabilitados ya que estas");    
    privmsg(s_NickServ, u->nick, "funciones pueden realizarse directamente sin tener que utilizar los bots para");    
    privmsg(s_NickServ, u->nick, "ello. Si desea recuperar el control sobre su nick teclee directamente el");    
    privmsg(s_NickServ, u->nick, "comando /NICK <su_nick>!<su_clave> para ponerse su nick o /GHOST <nick>:<clave>");    
    privmsg(s_NickServ, u->nick, "para desconectar un nick que se haya quedado conectado.");    
}

/*************************************************************************/

static void do_status(User *u)
{
    NickInfo *ni;

    char *nick;
    User *u2;
    int i = 0;
    /* NickInfo *u2; */


    while ((nick = strtok(NULL, " ")) && (i++ < 16)) {
       if (is_a_service(nick))
           notice_lang(s_NickServ, u, NICK_IS_A_SERVICE, nick);
       else if (!(u2 = finduser(nick)))
           notice_lang(s_NickServ, u, NICK_STATUS_OFFLINE, nick);
       else if (!(findnick(nick)))
           notice_lang(s_NickServ, u, NICK_STATUS_NOT_REGISTRED, nick);
       else if (nick_suspendied(u2))
           notice_lang(s_NickServ, u, NICK_STATUS_SUSPENDED, nick);
       else if (nick_identified(u2))
           notice_lang(s_NickServ, u, NICK_STATUS_IDENTIFIED, nick);
       else if (nick_recognized(u2))
           notice_lang(s_NickServ, u, NICK_STATUS_RECOGNIZED, nick);
       else
           notice_lang(s_NickServ, u, NICK_STATUS_NOT_IDENTIFIED, nick);

       
       if ((ni = findnick(nick)) && (u2 = finduser(nick))) {
       if ((ni->status & NI_ON_BDD) && nick_identified(u2)) {
      	  notice_lang(s_NickServ, u, NICK_STATUS_ID_BDD);
       }
      }
   }
}


/*************************************************************************/

static void do_sendpass(User *u)
{
#ifdef REG_NICK_MAIL
    char *nick = strtok(NULL, " ");
    NickInfo *ni;
        
    if (!nick){
        syntax_error(s_NickServ, u, "SENDPASS", NICK_SENDPASS_SYNTAX);
    } else if (!(ni = findnick(nick))) {
        notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
    } else {
        {
         char *buf;
         char subject[BUFSIZE];
                       
         if (fork()==0) {
                       
             buf = smalloc(sizeof(char *) * 1024);
             sprintf(buf,"\n    NiCK: %s\n"
                           "Password: %s\n\n"
                           "Para identificarte   -> /nick %s:%s\n"
                           "Para cambio de clave -> /msg %s SET PASSWORD nueva_contrase帽a\n\n"
                           "P谩gina de Informaci贸n %s\n",                           
                  ni->nick, ni->pass, ni->nick, ni->pass, s_NickServ, WebNetwork);
                    
             snprintf(subject, sizeof(subject), "Contrase帽a del NiCK '%s'", ni->nick);
       
             enviar_correo(ni->email, subject, buf);             
             exit(0);
         }
        }                           
         notice_lang(s_NickServ, u, NICK_SENDPASS_SUCCEEDED, nick, ni->email);
         canalopers(s_NickServ, "12%s ha usado 12SENDPASS sobre 12%s", u->nick, nick);
    }
#else
         privmsg(s_NickServ, u->nick, "En esta red no esta disponible el SENDPASS");    
#endif    
}                                                                                                               
/***************************************************************************/                 
/* El RENAME */
static void do_rename(User *u)
{
   char *nick = strtok(NULL, " ");
   NickInfo *ni;
      
   if (!nick) {
       privmsg(s_NickServ, u->nick, "No se ha especificado un nick");
   } else if (!(ni = findnick(nick))) {
       return;
   } else if (nick_is_services_admin(ni) && !is_services_root(u)) {
       notice_lang(s_NickServ, u, PERMISSION_DENIED);
   } else {
       send_cmd(NULL, "RENAME %s", ni->nick);
   }
}

/***************************************************************************/

static void do_getpass(User *u)
{
#ifndef USE_ENCRYPTION
    char *nick = strtok(NULL, " ");
    NickInfo *ni;
#endif

    /* Assumes that permission checking has already been done. */
#ifdef USE_ENCRYPTION
    notice_lang(s_NickServ, u, NICK_GETPASS_UNAVAILABLE);
#else
    if (!nick) {
	syntax_error(s_NickServ, u, "GETPASS", NICK_GETPASS_SYNTAX);
    } else if (!(ni = findnick(nick))) {
	notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
    } else if (nick_is_services_admin(ni) && !is_services_root(u)) {
	notice_lang(s_NickServ, u, PERMISSION_DENIED);
    } else {
        canalopers(s_NickServ, "12%s Uso GETPASS sobre 12%s", u->nick, ni->nick);
	log("%s: %s!%s@%s used GETPASS on %s",
		s_NickServ, u->nick, u->username, u->host, nick);
	notice_lang(s_NickServ, u, NICK_GETPASS_PASSWORD_IS, nick, ni->pass);
    }
#endif
}


/*************************************************************************/

static void do_suspend(User *u)
{

    NickInfo *ni;
    char *nick, *expiry, *reason;                        
    time_t expires;

    nick = strtok(NULL, " ");
    /* Por el momento, nada de expiraciones
    if (nick && *nick == '+') {
        expiry = nick;
        nick = strtok(NULL, " ");
    } else { */
        expiry = NULL;
/*    }
 */
    reason = strtok(NULL, "");    
            
    if (!reason) {
        syntax_error(s_NickServ, u, "SUSPEND", NICK_SUSPEND_SYNTAX);
    } else if (!(ni = findnick(nick))) {
        notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
    } else if (ni->status & NS_VERBOTEN) {
        notice_lang(s_NickServ, u, NICK_SUSPEND_FORBIDDEN);
    } else if (ni->status & NS_SUSPENDED) {
        notice_lang(s_NickServ, u, NICK_SUSPEND_SUSPENDED, nick);
    } else if (nick_is_services_admin(ni)) {
        notice_lang(s_NickServ, u, NICK_SUSPEND_OPER);
        canalopers(s_NickServ, "12%s ha intentado 12SUSPENDer al admin 12%s (%s)",
                                        u->nick, ni->nick, reason);
    } else if (nick_is_services_oper(ni)) {
        notice_lang(s_NickServ, u, NICK_SUSPEND_OPER);
        canalopers(s_NickServ, "12%s ha intentado 12SUSPENDer al oper 12%s (%s)",
              u->nick, ni->nick, reason);
    } else {
        if (expiry) {
            expires = dotime(expiry);
            if (expires < 0) {
                notice_lang(s_ChanServ, u, BAD_EXPIRY_TIME);
                return;
            } else if (expires > 0) {
                expires += time(NULL);
            }
        } else {
//            expires = time(NULL) + CSSuspendExpire;
            expires = 0; /* suspension indefinida */
        }                                    
        log("%s: %s!%s@%s usado SUSPEND on %s (%s)",
             s_NickServ, u->nick, u->username, u->host, nick, reason);
        ni->suspendby = sstrdup(u->nick);
        ni->suspendreason = sstrdup(reason);
        ni->time_suspend = time(NULL);
        ni->time_expiresuspend = expires;
        ni->status |= NS_SUSPENDED;
        ni->status &= ~NS_IDENTIFIED;
	
	if (ni->status & NI_ON_BDD)
		do_write_bdd(ni->nick, 16, ni->pass); 
        
	canalopers(s_NickServ, "12%s ha 12SUSPENDido el nick 12%s (%s)",
          u->nick, nick, reason); 
        notice_lang(s_NickServ, u, NICK_SUSPEND_SUCCEEDED, nick);

        if (finduser(nick)) {
            privmsg (s_NickServ, ni->nick, "Tu nick 12%s ha sido 12SUSPENDido"
                 " temporalmente.", ni->nick);
            privmsg (s_NickServ, ni->nick, "Causa de suspension: %s.", reason);
        }
                          
    }
                                                     
}
                                                                                                                                                           
/*************************************************************************/
static void do_unsuspend(User *u)
{
    char *nick = strtok(NULL, " ");
    NickInfo *ni;
        
        
    /* Assumes that permission checking has already been done. */
    if (!nick) {
        syntax_error(s_NickServ, u, "UNSUSPEND", NICK_UNSUSPEND_SYNTAX);
    } else if (!(ni = findnick(nick))) {
        notice_lang(s_NickServ, u, NICK_X_NOT_REGISTERED, nick);
    } else if (!(ni->status & NS_SUSPENDED)) {
        notice_lang(s_NickServ, u, NICK_SUSPEND_NOT_SUSPEND, ni->nick);
    } else {
         log("%s: %s!%s@%s usado UNSUSPEND on %s",
                s_NickServ, u->nick, u->username, u->host, nick);
          ni->status &= ~NS_SUSPENDED;
          free(ni->suspendby);
          free(ni->suspendreason);
          ni->time_suspend = 0;
          ni->time_expiresuspend = 0;
	  canalopers(s_NickServ, "12%s ha reactivado el nick 12%s", u->nick, nick);

	  if (ni->status & NI_ON_BDD)
		  do_write_bdd(ni->nick, 1, ni->pass);

          notice_lang(s_NickServ, u, NICK_UNSUSPEND_SUCCEEDED, nick);
          if (finduser(nick)) {
              privmsg (s_NickServ, ni->nick, "Tu nick 12%s ha sido reactivado.", ni->nick);
              privmsg (s_NickServ, ni->nick, "Vuelve a identificarte con tu nick.");
//            send_cmd(NULL, "RENAME %s", ni->nick);
	  }
    }
}


/*************************************************************************/

static void do_forbid(User *u)
{
    NickInfo *ni;
    char *nick = strtok(NULL, " ");
    char *reason = strtok(NULL, "");    

    /* Assumes that permission checking has already been done. */
    if (!reason) {
	syntax_error(s_NickServ, u, "FORBID", NICK_FORBID_SYNTAX);
	return;
    }
/***    } else if (ni->status & NS_VERBOTEN) {
        notice_lang(s_NickServ, u, NICK_FORBID_FORBIDDEN);
    } else {        ***/
     
    if (nick_is_services_oper(findnick(nick))) {
        notice_lang(s_NickServ, u, PERMISSION_DENIED);
        canalopers(s_NickServ, "%s ha intentado FORBIDear el nick %s", u->nick, nick);
        return;
    }            
    if (readonly)
	notice_lang(s_NickServ, u, READ_ONLY_MODE);
    if ((ni = findnick(nick)) != NULL)
	delnick(ni);
    ni = makenick(nick);
    if (ni) {
        ni->forbidby = sstrdup(u->nick);
        ni->forbidreason = sstrdup(reason);    
	ni->status |= NS_VERBOTEN;
	log("%s: %s set FORBID for nick %s", s_NickServ, u->nick, nick);
	do_write_bdd(nick, 15, "*");
	notice_lang(s_NickServ, u, NICK_FORBID_SUCCEEDED, nick);
    } else {
	log("%s: Valid FORBID for %s by %s failed", s_NickServ,
		nick, u->nick);
	notice_lang(s_NickServ, u, NICK_FORBID_FAILED, nick);
    }
}


/*************************************************************************/

static void do_unforbid(User *u)
{
    NickInfo *ni;
    char *nick = strtok(NULL, " ");
    User *u2;
  
    /* Assumes that permission checking has already been done. */
    if (!nick) {
        syntax_error(s_NickServ, u, "UNFORBID", NICK_UNFORBID_SYNTAX);
    } else if (!((ni = findnick(nick)) && (ni->status & NS_VERBOTEN))) {
        notice_lang(s_NickServ, u, NICK_UNFORBID_NOT_FORBID, nick);
    } else {
        if (readonly)
             notice_lang(s_NickServ, u, READ_ONLY_MODE);
                 
        delnick(ni);
        log("%s: %s set UNFORBID for nick %s", s_NickServ, u->nick, nick);
       	do_write_bdd(nick, 15, "");
	
	notice_lang(s_NickServ, u, NICK_UNFORBID_SUCCEEDED, nick);
        canalopers(s_NickServ, "12%s ha 12UNFORBIDeado el nick 12%s",
                           u->nick, nick);
        if (nick && (u2 = finduser(nick)))
             u2->ni = u2->real_ni = NULL;
    }
                                                                                                  
}

/*************************************************************************/
