/* Main header for Services.
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

#ifndef SERVICES_H
#define SERVICES_H

/*************************************************************************/

#include "sysconf.h"
#include "config.h"

/* Some Linux boxes (or maybe glibc includes) require this for the
 * prototype of strsignal(). */
#define _GNU_SOURCE

/* Some AIX boxes define int16 and int32 on their own.  Blarph. */
#if INTTYPE_WORKAROUND
# define int16 builtin_int16
# define int32 builtin_int32
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <grp.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <sys/socket.h>
#include <sys/stat.h>	/* for umask() on some systems */
#include <sys/types.h>
#include <sys/time.h>

#if HAVE_STRINGS_H
# include <strings.h>
#endif

#if HAVE_SYS_SELECT_H
# include <sys/select.h>
#endif

#ifdef _AIX
/* Some AIX boxes seem to have bogus includes that don't have these
 * prototypes. */
extern int strcasecmp(const char *, const char *);
extern int strncasecmp(const char *, const char *, size_t);
# if 0	/* These break on some AIX boxes (4.3.1 reported). */
extern int gettimeofday(struct timeval *, struct timezone *);
extern int socket(int, int, int);
extern int bind(int, struct sockaddr *, int);
extern int connect(int, struct sockaddr *, int);
extern int shutdown(int, int);
# endif
# undef FD_ZERO
# define FD_ZERO(p) memset((p), 0, sizeof(*(p)))
#endif /* _AIX */

/* Alias stricmp/strnicmp to strcasecmp/strncasecmp if we have the latter
 * but not the former. */
#if !HAVE_STRICMP && HAVE_STRCASECMP
# define stricmp strcasecmp
# define strnicmp strncasecmp
#endif

/* We have our own versions of toupper()/tolower(). */
#include <ctype.h>
#undef tolower
#undef toupper
#define tolower tolower_
#define toupper toupper_
extern int toupper(char), tolower(char);

/* We also have our own encrypt(). */
#define encrypt encrypt_


#if INTTYPE_WORKAROUND
# undef int16
# undef int32
#endif


/* Miscellaneous definitions. */
#include "defs.h"

/*************************************************************************/

/* Configuration sanity-checking: */

#if CHANNEL_MAXREG > 32767
# undef CHANNEL_MAXREG
# define CHANNEL_MAXREG	0
#endif
#if DEF_MAX_MEMOS > 32767
# undef DEF_MAX_MEMOS
# define DEF_MAX_MEMOS	0
#endif
#if MAX_SERVOPERS > 32767
# undef MAX_SERVOPERS
# define MAX_SERVOPERS 32767
#endif
#if MAX_SERVADMINS > 32767
# undef MAX_SERVADMINS
# define MAX_SERVADMINS 32767
#endif

/*************************************************************************/
/*************************************************************************/

/* Version number for data files; if structures below change, increment
 * this.  (Otherwise -very- bad things will happen!) */

#define FILE_VERSION	9

/*************************************************************************/

typedef struct server_ Server;

struct server_ {
    Server *next, *prev;
    Server *hub;
    Server *hijo, *rehijo;
    int parent;
    char *name;
    int  users;
    char *numerico;
};

/*************************************************************************/
/* Memo info structures.  Since both nicknames and channels can have memos,
 * we encapsulate memo data in a MemoList to make it easier to handle. */

typedef struct memo_ Memo;
struct memo_ {
    uint32 number;	/* Index number -- not necessarily array position! */
    int16 flags;
    time_t time;	/* When it was sent */
    char sender[NICKMAX];
    char *text;
};

#define MF_UNREAD	0x0001	/* Memo has not yet been read */

typedef struct {
    int16 memocount, memomax;
    Memo *memos;
} MemoInfo;

/*************************************************************************/

/* Nickname info structure.  Each nick structure is stored in one of 256
 * lists; the list is determined by the first character of the nick.  Nicks
 * are stored in alphabetical order within lists. */


typedef struct nickinfo_ NickInfo;

struct nickinfo_ {
    NickInfo *next, *prev;
    char nick[NICKMAX];
    char pass[PASSMAX];
    char *url;
    char *email;
    char *vhost;

    char *last_usermask;
    char *last_realname;
    char *last_quit;
    time_t time_registered;
    time_t last_seen;
    int16 status;	/* See NS_* below */
    char *suspendby;         /* Quien lo suspendio */
    char *suspendreason;     /* Motivo de la suspension */
    time_t time_suspend;     /* Tiempo cuando suspendio el nick */
    time_t time_expiresuspend; /* Expiracion suspension */
    char *forbidby;          /* Quien lo forbideo */
    char *forbidreason;      /* Motivo del forbid */

    NickInfo *link;	/* If non-NULL, nick to which this one is linked */
    int16 linkcount;	/* Number of links to this nick */

    /* All information from this point down is governed by links.  Note,
     * however, that channelcount is always saved, even for linked nicks
     * (thus removing the need to recount channels when unlinking a nick). */

    int32 flags;	/* See NI_* below */

    int16 accesscount;	/* # of entries */
    char **access;	/* Array of strings */

    MemoInfo memos;

    uint16 channelcount;/* Number of channels currently registered */
    uint16 channelmax;	/* Maximum number of channels allowed */

    uint16 language;	/* Language selected by nickname owner (LANG_*) */

    time_t id_timestamp;/* TS8 timestamp of user who last ID'd for nick */
/*     char *msuspend; */
};


/* Nickname status flags: */
#define NS_ENCRYPTEDPW	0x0001      /* Nickname password is encrypted */
#define NS_VERBOTEN	0x0002      /* Nick may not be registered or used */
#define NS_NO_EXPIRE	0x0004      /* Nick never expires */
#define NS_SUSPENDED	0x0008      /* Nick suspendido */
#define NI_ON_BDD	0x0010      /* Nick en la BDD */

#define NS_IDENTIFIED	0x8000      /* User has IDENTIFY'd */
#define NS_RECOGNIZED	0x4000      /* ON_ACCESS true && SECURE flag not set */
#define NS_ON_ACCESS	0x2000      /* User comes from a known address */
#define NS_KILL_HELD	0x1000      /* Nick is being held after a kill */
#define NS_GUESTED	0x0100	    /* SVSNICK has been sent but nick has not
				     * yet changed. An enforcer will be
				     * introduced when it does change. */
#define NS_TEMPORARY	0xFF00      /* All temporary status flags */


/* Nickname setting flags: */
#define NI_KILLPROTECT	0x00000001  /* Kill others who take this nick */
#define NI_SECURE	0x00000002  /* Don't recognize unless IDENTIFY'd */
#define NI_MEMO_HARDMAX	0x00000008  /* Don't allow user to change memo limit */
#define NI_MEMO_SIGNON	0x00000010  /* Notify of memos at signon and un-away */
#define NI_MEMO_RECEIVE	0x00000020  /* Notify of new memos when sent */
#define NI_PRIVATE	0x00000040  /* Don't show in LIST to non-servadmins */
#define NI_HIDE_EMAIL	0x00000080  /* Don't show E-mail in INFO */
#define NI_HIDE_MASK	0x00000100  /* Don't show last seen address in INFO */
#define NI_HIDE_QUIT	0x00000200  /* Don't show last quit message in INFO */
#define NI_KILL_QUICK	0x00000400  /* Kill in 20 seconds instead of 60 */
#define NI_KILL_IMMED	0x00000800  /* Kill immediately instead of in 60 sec */
#define NI_IPVIRTUALV	0x00001000  /* Tiene una IPvirtual por OPER */

/* Languages.  Never insert anything in the middle of this list, or
 * everybody will start getting the wrong language!  If you want to change
 * the order the languages are displayed in for NickServ HELP SET LANGUAGE,
 * do it in language.c.
 */
#define LANG_ES		0	/* Español */
#define LANG_CA		1	/* Catalan */
#define LANG_EN_US	2	/* United States English */
#define LANG_JA_JIS	3	/* Japanese (JIS encoding) */
#define LANG_JA_EUC	4	/* Japanese (EUC encoding) */
#define LANG_JA_SJIS	5	/* Japanese (SJIS encoding) */
#define LANG_PT		6	/* Portugese */
#define LANG_FR		7	/* French */
#define LANG_TR		8	/* Turkish */
#define LANG_IT		9	/* Italian */
#define LANG_GA		10	/* Gallego */

#define NUM_LANGS	11	/* Number of languages */

#define COMPENSACION 439387563

/* Sanity-check on default language value */
#if DEF_LANGUAGE < 0 || DEF_LANGUAGE >= NUM_LANGS
# error Invalid value for DEF_LANGUAGE: must be >= 0 and < NUM_LANGS
#endif

/*************************************************************************/

/* Channel info structures.  Stored similarly to the nicks, except that
 * the second character of the channel name, not the first, is used to
 * determine the list.  (Hashing based on the first character of the name
 * wouldn't get very far. ;) ) */

/* Access levels for users. */
typedef struct {
    int16 in_use;	/* 1 if this entry is in use, else 0 */
    int16 level;
    NickInfo *ni;	/* Guaranteed to be non-NULL if in use, NULL if not */
} ChanAccess;

/* Note that these two levels also serve as exclusive boundaries for valid
 * access levels.  ACCESS_FOUNDER may be assumed to be strictly greater
 * than any valid access level, and ACCESS_INVALID may be assumed to be
 * strictly less than any valid access level.
 */
#define ACCESS_FOUNDER	500	/* Numeric level indicating founder access */
#define ACCESS_INVALID	-3	/* Used in levels[] for disabled settings */

/* AutoKick data. */
typedef struct {
    int16 in_use;
    int16 is_nick;	/* 1 if a regged nickname, 0 if a nick!user@host mask */
			/* Always 0 if not in use */
    union {
	char *mask;	/* Guaranteed to be non-NULL if in use, NULL if not */
	NickInfo *ni;	/* Same */
    } u;
    char *reason;
    char *who[NICKMAX]; /* Quien ha metido el akick */
} AutoKick;

typedef struct chaninfo_ ChannelInfo;
struct chaninfo_ {
    ChannelInfo *next, *prev;
    char name[CHANMAX];
    NickInfo *founder;
    NickInfo *successor;		/* Who gets the channel if the founder
					 * nick is dropped or expires */
    char founderpass[PASSMAX];
    char *desc;
    char *url;
    char *email;

    time_t time_registered;
    time_t last_used;
    char *last_topic;			/* Last topic on the channel */
    char last_topic_setter[NICKMAX];	/* Who set the last topic */
    time_t last_topic_time;		/* When the last topic was set */

    int32 flags;			/* See below */
    char *suspendby;                    /* Quien lo suspendio */
    char *suspendreason;                /* Motivo de la suspension */
    time_t time_suspend;                /* Tiempo cuando suspendio el canal */
    time_t time_expiresuspend;          /* Expiracion suspension */
    char *forbidby;                     /* Quien lo forbideo */
    char *forbidreason;                 /* Motivo del forbid */                        

    int16 *levels;			/* Access levels for commands */

    int16 accesscount;
    ChanAccess *access;			/* List of authorized users */
    int16 akickcount;
    AutoKick *akick;			/* List of users to kickban */

    int16 mlock_on, mlock_off;		/* See channel modes below */
    int32 mlock_limit;			/* 0 if no limit */
    char *mlock_key;			/* NULL if no key */

    char *entry_message;		/* Notice sent on entering channel */

    MemoInfo memos;

    struct channel_ *c;			/* Pointer to channel record (if   *
					 *    channel is currently in use) */
};

/* Retain topic even after last person leaves channel */
#define CI_KEEPTOPIC	0x00000001
/* Don't allow non-authorized users to be opped */
#define CI_SECUREOPS	0x00000002
/* Hide channel from ChanServ LIST command */
#define CI_PRIVATE	0x00000004
/* Topic can only be changed by SET TOPIC */
#define CI_TOPICLOCK	0x00000008
/* Those not allowed ops are kickbanned */
#define CI_RESTRICTED	0x00000010
/* Don't auto-deop anyone */
#define CI_LEAVEOPS	0x00000020
/* Don't allow any privileges unless a user is IDENTIFY'd with NickServ */
#define CI_SECURE	0x00000040
/* Don't allow the channel to be registered or used */
#define CI_VERBOTEN	0x00000080
/* Channel password is encrypted */
#define CI_ENCRYPTEDPW	0x00000100
/* Channel does not expire */
#define CI_NO_EXPIRE	0x00000200
/* Channel memo limit may not be changed */
#define CI_MEMO_HARDMAX	0x00000400
/* Send notice to channel on use of OP/DEOP */
#define CI_OPNOTICE	0x00000800
/* Canal Suspendido */
#define CI_SUSPEND	0x00001000
/* Chanserv dentro o fuera del canal */
#define CI_STAY		0x00002000
/* No registrados no pueden tener voz */
#define CI_SECUREVOICES	0x00004000
 


/* Indices for cmd_access[]: */
#define CA_INVITE	0
#define CA_AKICK	1
#define CA_SET		2	/* but not FOUNDER or PASSWORD */
#define CA_UNBAN	3
#define CA_AUTOOP	4
#define CA_AUTODEOP	5	/* Maximum, not minimum */
#define CA_AUTOVOICE	6
#define CA_OPDEOP	7	/* ChanServ commands OP and DEOP */
#define CA_ACCESS_LIST	8
#define CA_CLEAR	9
#define CA_NOJOIN	10	/* Maximum */
#define CA_ACCESS_CHANGE 11
#define CA_MEMO		12
#define CA_VOICEDEVOICE 13
#define CA_AUTODEVOICE  14
#define CA_KICK         15
#define CA_BAN          16
#define CA_RESET        17
#define CA_AUTOHALFOP   18
#define CA_HALFOPDEHALFOP 19

#define CA_SIZE         20

/*************************************************************************/

/* Online user and channel data. */

typedef struct user_ User;
typedef struct channel_ Channel;

struct user_ {
    User *next, *prev;
    char nick[NICKMAX];
#ifdef IRC_UNDERNET_P10
    char *numerico;			/* Numerico del nick en Redes P10 */
#endif    
    NickInfo *ni;			/* Effective NickInfo (not a link) */
    NickInfo *real_ni;			/* Real NickInfo (ni.nick==user.nick) */
    char *username;
    char *host;				/* User's hostname */
    char *realname;
    char *server;			/* Name of server user is on */
    time_t signon;			/* Time of signon (NOT nick change) */
    time_t my_signon;			/* When did _we_ see the user? */
    int32 mode;				/* See below */
    struct u_chanlist {
	struct u_chanlist *next, *prev;
	Channel *chan;
    } *chans;				/* Channels user has joined */
    struct u_chaninfolist {
	struct u_chaninfolist *next, *prev;
	ChannelInfo *chan;
    } *founder_chans;			/* Channels user has identified for */
    short invalid_pw_count;		/* # of invalid password attempts */
    time_t invalid_pw_time;		/* Time of last invalid password */
    time_t lastmemosend;		/* Last time MS SEND command used */
    time_t lastnickreg;			/* Last time NS REGISTER cmd used */
    char creg_apoyo[PASSMAX];		/* token del apoyo por creg */
};

#define UMODE_O 0x00000001
#define UMODE_I 0x00000002
#define UMODE_S 0x00000004
#define UMODE_W 0x00000008
#define UMODE_G 0x00000010
#define UMODE_H 0x00000020
#define UMODE_R 0x00000040
#define UMODE_X 0x00000080
#define UMODE_Z 0x00000100
#define UMODE_K 0x00000200
#define UMODE_P 0x00000400
#define UMODE_C 0x00000800
#define UMODE_A 0x00001000
#define UMODE_B 0x00002000


struct channel_ {
    Channel *next, *prev;
    char name[CHANMAX];
    ChannelInfo *ci;			/* Corresponding ChannelInfo */
    time_t creation_time;		/* When channel was created */

    char *topic;
    char topic_setter[NICKMAX];		/* Who set the topic */
    time_t topic_time;			/* When topic was set */

    int32 mode;				/* Binary modes only */
    int32 limit;			/* 0 if none */
    char *key;				/* NULL if none */

    int32 bancount, bansize;
    char **bans;

    struct c_userlist {
	struct c_userlist *next, *prev;
	User *user;
    } *users, *chanops, *voices, *halfops;

    time_t server_modetime;		/* Time of last server MODE */
    time_t chanserv_modetime;		/* Time of last check_modes() */
    int16 server_modecount;		/* Number of server MODEs this second */
    int16 chanserv_modecount;		/* Number of check_mode()'s this sec */
    int16 bouncy_modes;			/* Did we fail to set modes here? */
};

#define CMODE_I 0x00000001
#define CMODE_M 0x00000002
#define CMODE_N 0x00000004
#define CMODE_P 0x00000008
#define CMODE_s 0x00000010
#define CMODE_T 0x00000020
#define CMODE_K 0x00000040		/* These two used only by ChanServ */
#define CMODE_L 0x00000080

/* Tambien Modos Hispano y Terra */
#define CMODE_R 0x00000100		/* Only identified users can join */
#define CMODE_r 0x00000200		/* Set for all registered channels */
/* Modos hispano */
#define CMODE_A 0x00000400		/* Modo AutoOP */
#define CMODE_S 0x00000800		/* Modo +S SecureOps */
#define CMODE_m 0x00001000		/* Solo nicks +r habln en canal (sin +o,v) */

#define CMODE_c 0x00002000              /* Canal que no admite colores */
#define CMODE_C 0x00004000              /* Canal que no admite CTCPs */
#define CMODE_O 0x00008000              /* Solo entran opers/ircops */


/* Who sends channel MODE (and KICK) commands? */
#if defined(IRC_HISPANO) || defined(IRC_TERRA)
# define MODE_SENDER(service) service
#else
# define MODE_SENDER(service) ServerName
#endif

/* P10 */

#define NUMNICKBASE 64
#define NUMNICKMAXCHAR 'z'

/*************************************************************************/

/* Soporte P10 */

#define NUMNICKBASE 64
#define NUMNICKMAXCHAR 'z'

#define toLower(c)      (NTL_tolower_tab[(c)-CHAR_MIN])
#define toUpper(c)      (NTL_toupper_tab[(c)-CHAR_MIN])

/* Usamos nuestras propias versiones de strtTok y strToken */
#undef strtok
#undef strtoken

#define strtok strTok
#define strtoken strToken

/*************************************************************************/

/* Constants for news types. */

#define NEWS_LOGON	0
#define NEWS_OPER	1

/*************************************************************************/

/* Ignorance list data. */

typedef struct ignore_data {
    struct ignore_data *next;
    char who[NICKMAX];
    time_t time;	/* When do we stop ignoring them? */
} IgnoreData;

/*************************************************************************/
/*************************************************************************/

#include "cregserv.h"
#include "extern.h"

/*************************************************************************/

#endif	/* SERVICES_H */
