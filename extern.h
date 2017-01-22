/* Prototypes and external variable declarations.
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

#ifndef EXTERN_H
#define EXTERN_H

#define E extern


/**** actions.c ****/

E void kill_user(const char *source, const char *user, const char *reason);
E void bad_password(User *u);


/**** akill.c ****/

E void get_akill_stats(long *nrec, long *memuse);
E int num_akills(void);
E void load_akill(void);
E void save_akill(void);
E int check_akill(const char *nick, const char *username, const char *host);
E void expire_akills(void);
E void do_akill(User *u);
E void add_akill(const char *mask, const char *reason, const char *who,
			const time_t expiry);

/**** channels.c ****/

#ifdef DEBUG_COMMANDS
E void send_channel_list(User *user);
E void send_channel_users(User *user);
#endif

E void get_channel_stats(long *nrec, long *memuse);
E Channel *findchan(const char *chan);
E Channel *firstchan(void);
E Channel *nextchan(void);

E void chan_adduser(User *user, const char *chan);
E void chan_deluser(User *user, Channel *c);

#ifdef IRC_UNDERNET_P10
E void do_burst(const char *source, int ac, char **av);
E void do_create(const char *source, int ac, char **av);
#endif
E void do_cmode(const char *source, int ac, char **av);
E void do_topic(const char *source, int ac, char **av);

E int only_one_user(const char *chan);


/**** chanserv.c ****/

E void get_chanserv_stats(long *nrec, long *memuse);

E void cs_init(void);
E void chanserv(const char *source, char *buf);
E void load_cs_dbase(void);
E void save_cs_dbase(void);
E void check_modes(const char *chan);
E int check_valid_op(User *user, const char *chan, int newchan);
E int check_valid_voice(User *user, const char *chan, int newchan);
E int check_should_op(User *user, const char *chan);
E int check_should_halfop(User *user, const char *chan);
E int check_should_voice(User *user, const char *chan);
E int check_kick(User *user, const char *chan);
E void record_topic(const char *chan);
E void restore_topic(const char *chan);
E int check_topiclock(const char *chan);
E void expire_chans(void);
E void cs_remove_nick(const NickInfo *ni);

E ChannelInfo *cs_findchan(const char *chan);
E int check_access(User *user, ChannelInfo *ci, int what);
E void registros(User *u, NickInfo *ni);
E void join_chanserv();
E void join_shadow();
E int registra_con_creg(User *u, NickInfo *ni, const char *chan, const char *pass, const char *desc);

/**** cregserv.c ****/

E void cr_init(void);
E void cregserv(const char *source, char *buf);
E void load_cr_dbase(void);
E void save_cr_dbase(void);
E void get_cregserv_stats(long *nrec, long *memuse);
E CregInfo *cr_findcreg(const char *chan);
E void expire_chans(void);


/**** compat.c ****/

#if !HAVE_SNPRINTF
# if BAD_SNPRINTF
#  define snprintf my_snprintf
# endif
# define vsnprintf my_vsnprintf
E int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
E int snprintf(char *buf, size_t size, const char *fmt, ...);
#endif
#if !HAVE_STRICMP && !HAVE_STRCASECMP
E int stricmp(const char *s1, const char *s2);
E int strnicmp(const char *s1, const char *s2, size_t len);
#endif
#if !HAVE_STRDUP
E char *strdup(const char *s);
#endif
#if !HAVE_STRSPN
E size_t strspn(const char *s, const char *accept);
#endif
#if !HAVE_STRERROR
E char *strerror(int errnum);
#endif
#if !HAVE_STRSIGNAL
char *strsignal(int signum);
#endif


/**** config.c ****/

E char *RemoteServer;
E int   RemotePort;
E char *RemotePassword;
E char *LocalHost;
E int   LocalPort;

E char *ServerName;
#ifdef IRC_UNDERNET_P10
E int   ServerNumerico;
#endif 
E char *ServerHUB;
E char *ServerDesc;
E char *ServiceUser;
E char *ServiceHost;
E char *s_NickServ;
E char *s_ChanServ;
E char *s_CregServ;
E char *s_MemoServ;
E char *s_HelpServ;
E char *s_OperServ;
E char *s_GlobalNoticer;
E char *s_NewsServ;
E char *s_BddServ;
E char *s_ShadowServ;
E char *DEntryMsg;
E int  CregApoyos;

#ifdef IRC_UNDERNET_P10
E char s_NickServP10[4];
E char s_ChanServP10[4];
E char s_CregServP10[4];
E char s_MemoServP10[4];
E char s_HelpServP10[4];
E char s_OperServP10[4];
E char s_GlobalNoticerP10[4];
E char s_NewsServP10[4];
#endif
E char *desc_NickServ;
E char *desc_ChanServ;
E char *desc_CregServ;
E char *desc_MemoServ;
E char *desc_HelpServ;
E char *desc_OperServ;
E char *desc_GlobalNoticer;
E char *desc_NewsServ;
E char *desc_ShadowServ;
E char *desc_BddServ;

E char *PIDFilename;
E char *MOTDFilename;
E char *HelpDir;
E char *NickDBName;
E char *ChanDBName;
E char *CregDBName;
E char *OperDBName;
E char *AutokillDBName;
E char *NewsDBName;

#ifdef REG_NICK_MAIL
E int  NicksMail;
E char *SendMailPatch;
E char *ServerSMTP;
E int  PortSMTP;
E char *SendFrom;
E char *WebNetwork;
#endif

E int   NoBackupOkay;
E int   NoSplitRecovery;
E int   StrictPasswords;
E int   BadPassLimit;
E int   BadPassTimeout;
E int   UpdateTimeout;
E int   ExpireTimeout;
E int   ReadTimeout;
E int   WarningTimeout;
E int   TimeoutCheck;

#define PNAME "DarkuBots"
#define DNAME "DarkuBots"

E int   NSForceNickChange; 
E char *NSGuestNickPrefix;
E int   NSDefKill;
E int   NSDefKillQuick;
E int   NSDefSecure;
E int   NSDefPrivate;
E int   NSDefHideEmail;
E int   NSDefHideUsermask;
E int   NSDefHideQuit;
E int   NSDefMemoSignon;
E int   NSDefMemoReceive;
E int   NSRegDelay;
E int   NSExpire;
E int   NSAccessMax;
E char *NSEnforcerUser;
E char *NSEnforcerHost;
E int   NSReleaseTimeout;
E int   NSAllowKillImmed;
E int   NSDisableLinkCommand;
E int   NSListOpersOnly;
E int   NSListMax;
E int   NSSecureAdmins;

E int   CSMaxReg;
E int   CSExpire;
E int   CSAccessMax;
E int   CSAutokickMax;
E char *CSAutokickReason;
E int   CSInhabit;
E int   CSRestrictDelay;
E int   CSListOpersOnly;
E int   CSListMax;

/*E int   ShadowServ;
E char *NickShadow;
*/
E int   MSMaxMemos;
E int   MSSendDelay;
E int   MSNotifyAll;

E char *CanalAdmins;
E char *CanalOpers;
E char *ServicesRoot;
E int   LogMaxUsers;
E int   AutokillExpiry;

E int   KillClonesAkillExpire;

E int read_config(void);

/**** correo.c ****/

E int enviar_correo(const char * destino, const char *subject, const char *body);


/**** helpserv.c ****/

E void helpserv(const char *whoami, const char *source, char *buf);


/**** init.c ****/

E void introduce_user(const char *user);
E int init(int ac, char **av);


/**** language.c ****/

E char **langtexts[NUM_LANGS];
E char *langnames[NUM_LANGS];
E int langlist[NUM_LANGS];

E void lang_init(void);
#define getstring(ni,index) \
	(langtexts[((ni)?((NickInfo*)ni)->language:DEF_LANGUAGE)][(index)])
E int strftime_lang(char *buf, int size, User *u, int format, struct tm *tm);
E void syntax_error(const char *service, User *u, const char *command,
		int msgnum);


/**** log.c ****/

E int open_log(void);
E void close_log(void);
#define log _log
E void _log(const char *fmt, ...)		FORMAT(printf,1,2);
E void log_perror(const char *fmt, ...)		FORMAT(printf,1,2);
E void fatal(const char *fmt, ...)		FORMAT(printf,1,2);
E void fatal_perror(const char *fmt, ...)	FORMAT(printf,1,2);

E void rotate_log(User *u);


/**** main.c ****/

E const char version_branchstatus[];
E const char version_number[];
E const char version_upworld[];
E const char version_darku[];
E const char version_build[];
E const char version_protocol[];
E const char *info_text[];

E char *services_dir;
E char *log_filename;
E int   debug;
E int   readonly;
E int   skeleton;
E int   nofork;
E int   forceload;

E int   quitting;
E int   delayed_quit;
E char *quitmsg;
E char  inbuf[BUFSIZE];
E int   servsock;
E int   save_data;
E int   got_alarm;
E time_t start_time;


/**** memory.c ****/

E void *smalloc(long size);
E void *scalloc(long elsize, long els);
E void *srealloc(void *oldptr, long newsize);
E char *sstrdup(const char *s);


/**** memoserv.c ****/

E void ms_init(void);
E void memoserv(const char *source, char *buf);
E void load_old_ms_dbase(void);
E void check_memos(User *u);
E void check_cs_memos(User *u, ChannelInfo *ci);

/********** messages.c ********/


/**** misc.c ****/

E char *strscpy(char *d, const char *s, size_t len);
E char *stristr(char *s1, char *s2);
E char *strupper(char *s);
E char *strlower(char *s);
E char *strnrepl(char *s, int32 size, const char *old, const char *new);
#ifdef IRC_UNDERNET
E int strCasecmp(const char *a, const char *b);
E int NTL_tolower_tab[];
E int NTL_toupper_tab[];
E char *strToken(char **save, char *str, char *fs);
E char *strTok(char *str, char *fs);

#endif /* IRC_UNDERNET */

E char *merge_args(int argc, char **argv);

E int match_wild(const char *pattern, const char *str);
E int match_wild_nocase(const char *pattern, const char *str);

/* E char saltChars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]"; */

typedef int (*range_callback_t)(User *u, int num, va_list args);
E int process_numlist(const char *numstr, int *count_ret,
		range_callback_t callback, User *u, ...);
E int dotime(const char *s);
E int valid_ipvirtual(const char *str);
E int valid_domain(const char *str);

/**** news.c ****/

E void get_news_stats(long *nrec, long *memuse);
E void load_news(void);
E void save_news(void);
E void display_news(User *u, int16 type);
E void do_logonnews(User *u);
E void do_opernews(User *u);

E void newsserv(const char *source, char *buf);

/**** nickserv.c ****/

E void get_nickserv_stats(long *nrec, long *memuse);

E void ns_init(void);
E void nickserv(const char *source, char *buf);
E void load_ns_dbase(void);
E void save_ns_dbase(void);
E int validate_user(User *u);
E void cancel_user(User *u);
E int nick_identified(User *u);
E int nick_recognized(User *u);
E void expire_nicks(void);

E NickInfo *findnick(const char *nick);
E NickInfo *getlink(NickInfo *ni);


/**** operserv.c ****/

E void operserv(const char *source, char *buf);

E void os_init(void);
E void load_os_dbase(void);
E void save_os_dbase(void);
E int is_services_root(User *u);
E int is_services_bot(User *u);
E int is_services_admin(User *u);
E int is_services_coadmin(User *u);
E int is_services_cregadmin(User *u);
E int is_services_oper(User *u);
E int is_services_preoper(User *u);
E int is_a_service(char *nick);
E void ircops(User *u);
E int nick_is_services_admin(NickInfo *ni);
E int nick_is_services_coadmin(NickInfo *ni);
E int nick_is_services_oper(NickInfo *ni);
E int nick_is_services_preoper(NickInfo *ni);
E void os_remove_nick(const NickInfo *ni);

E void check_clones(User *user);

/**** bdd.c ***/
E void do_write_bdd(char *entrada, int tabla, const char *valor, ...);
E void do_count_bdd(int tabla, unsigned int valor);
E void bddserv(const char *source, char *buf);
E void bdd_init(void);
E char *gen_nice_key(unsigned int ilevel);

/**** process.c ****/

E int allow_ignore;
E IgnoreData *ignore[];

E void add_ignore(const char *nick, time_t delta);
E IgnoreData *get_ignore(const char *nick);

E int split_buf(char *buf, char ***argv, int colon_special);
E void process(void);


/**** send.c ****/

E void send_cmd(const char *source, const char *fmt, ...)
	FORMAT(printf,2,3);
E void vsend_cmd(const char *source, const char *fmt, va_list args)
	FORMAT(printf,2,0);
E void canalopers(const char *source, const char *fmt, ...);
E void canaladmins(const char *source, const char *fmt, ...)
        FORMAT(printf,2,3);        	
E void notice(const char *source, const char *dest, const char *fmt, ...)
	FORMAT(printf,3,4);
E void notice_list(const char *source, const char *dest, const char **text);
E void notice_lang(const char *source, User *dest, int message, ...);
E void notice_help(const char *source, User *dest, int message, ...);
E void privmsg(const char *source, const char *dest, const char *fmt, ...)
	FORMAT(printf,3,4);


/**** servers.c ****/

E void get_server_stats(long *nrec, long *memuse);
E void do_server(const char *source, int ac, char **av);
E void do_squit(const char *source, int ac, char **av);
E Server *find_servername(const char *servername);
E Server *find_servernumeric(const char *numeric);
E Server *add_server(const char *servername);
E void del_server(Server *server);
E void recursive_squit(Server *parent, const char *razon);
E void del_users_server(Server *server);
E void do_servers(User *u);

	
/**** sockutil.c ****/

E int32 total_read, total_written;
E int32 read_buffer_len(void);
E int32 write_buffer_len(void);

E int sgetc(int s);
E char *sgets(char *buf, int len, int s);
E char *sgets2(char *buf, int len, int s);
E int sread(int s, char *buf, int len);
E int sputs(char *str, int s);
E int sockprintf(int s, char *fmt,...);
E int conn(const char *host, int port, const char *lhost, int lport);
E void disconn(int s);


/**** P10.c ****/

#ifdef IRC_UNDERNET_P10
E char convert2y[];
E unsigned int base64toint(const char *s);
E const char *inttobase64(unsigned int i); 
#endif


/**** users.c ****/

E int usercnt, opcnt, maxusercnt, servercnt;
E time_t maxusertime;

#ifdef DEBUG_COMMANDS
E void send_user_list(User *user);
E void send_user_info(User *user);
#endif

E void get_user_stats(long *nusers, long *memuse);
E User *finduser(const char *nick);
#ifdef IRC_UNDERNET_P10
E User *finduserP10(const char *nick);
#endif
E User *firstuser(void);
E User *nextuser(void);

E void do_nick(const char *source, int ac, char **av);
E void do_join(const char *source, int ac, char **av);
E void do_part(const char *source, int ac, char **av);
E void do_kick(const char *source, int ac, char **av);
E void do_umode(const char *source, int ac, char **av);
E void do_quit(const char *source, int ac, char **av);
E void do_kill(const char *source, int ac, char **av);

E int is_oper(const char *nick);
E int is_on_chan(const char *nick, const char *chan);
E int is_chanop(const char *nick, const char *chan);
E int is_voiced(const char *nick, const char *chan);

E int match_usermask(const char *mask, User *user);
E void split_usermask(const char *mask, char **nick, char **user, char **host);
E char *create_mask(User *u);
E void join_shadow_chan(const char *canal);
E void part_shadow_chan(const char *canal);
#endif	/* EXTERN_H */

