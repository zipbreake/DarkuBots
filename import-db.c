/* Convert other programs' databases to Services format.
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
#include "datafiles.h"

/*************************************************************************/

struct akill {
    char *mask;
    char *reason;
    char who[NICKMAX];
    time_t time;
    time_t expires;
};
typedef struct {
    int16 type;
    int32 num;
    char *text;
    char who[NICKMAX];
    time_t time;
} NewsItem;

/* All this is initialized to zeros */
NickInfo *nicklists[256];
ChannelInfo *chanlists[256];
NickInfo *services_admins[MAX_SERVADMINS];
NickInfo *services_opers[MAX_SERVOPERS];
int nakill;
struct akill *akills;
int nnews;
NewsItem *news;
int32 maxusercnt;
time_t maxusertime;

/*************************************************************************/

/* Generic routine to make a backup copy of a file. */

void make_backup(const char *name)
{
    char buf[PATH_MAX];
    FILE *in, *out;
    int n;

    snprintf(buf, sizeof(buf), "%s~", name);
    if (strcmp(buf, name) == 0) {
	fprintf(stderr, "Can't back up %s: Path too long\n", name);
	exit(1);
    }
    in = fopen(name, "rb");
    if (!in) {
	fprintf(stderr, "Can't open %s for writing", buf);
	perror("");
	exit(1);
    }
    out = fopen(buf, "wb");
    if (!out) {
	fprintf(stderr, "Can't open %s for writing", buf);
	perror("");
	exit(1);
    }
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
	if (fwrite(buf, 1, n, out) != n) {
	    fprintf(stderr, "Write error on %s", buf);
	    perror("");
	    exit(1);
	}
    }
    fclose(in);
    fclose(out);
}

/*************************************************************************/

/* Find a nickname or channel. */

NickInfo *findnick(const char *nick)
{
    NickInfo *ni;

    for (ni = nicklists[tolower(*nick)]; ni; ni = ni->next) {
	if (stricmp(ni->nick, nick) == 0)
	    return ni;
    }
    return NULL;
}


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

/* Safe memory allocation. */

void *smalloc(long size)
{
    void *ptr = malloc(size);
    if (size && !ptr) {
	fprintf(stderr, "Out of memory\n");
	exit(1);
    }
    return ptr;
}

/*************************************************************************/
/********************* Database loading: Magick 1.4b2 ********************/
/*************************************************************************/

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	fprintf(stderr, "Read error on %s\n", filename);	\
	exit(1);						\
    }								\
} while (0)

/*************************************************************************/

static void m14_load_nick(const char *sourcedir)
{
    char filename[PATH_MAX], *s;
    dbFILE *f;
    int i, j;
    int32 tmp32;
    NickInfo *ni, *ni2, **last, *prev;
    struct oldni_ {
	struct oldni_ *next, *prev;
	char nick[32];
	char pass[32];
	char *email;
	char *url;
	char *usermask;
	char *realname;
	time_t reg;
	time_t seen;
	long naccess;
	char **access;
	long nignore;
	char **ignore;
	long flags;
	long resv[4];
    } oldni;

    snprintf(filename, sizeof(filename), "%s/nick.db", sourcedir);
    make_backup(filename);
    f = open_db(NULL, filename, "r");
    if (!f) {
	fprintf(stderr, "Can't open %s for reading\n", filename);
	perror("");
	exit(1);
    }
    SAFE(read_int32(&tmp32, f));
    if (tmp32 != 5) {
	fprintf(stderr, "Wrong version number on %s\n", filename);
	exit(1);
    }
    for (i = 33; i < 256; i++) {
	last = &nicklists[i];
	prev = NULL;
	while (getc_db(f)) {
	    SAFE(read_variable(oldni, f));
	    if (oldni.email)
		SAFE(read_string(&oldni.email, f));
	    if (oldni.url)
		SAFE(read_string(&oldni.url, f));
	    SAFE(read_string(&oldni.usermask, f));
	    SAFE(read_string(&oldni.realname, f));
	    ni = smalloc(sizeof(*ni));
	    ni->next = NULL;
	    ni->prev = prev;
	    *last = ni;
	    last = &(ni->next);
	    prev = ni;
	    strscpy(ni->nick, oldni.nick, NICKMAX);
	    strscpy(ni->pass, oldni.pass, PASSMAX);
	    ni->url = oldni.url;
	    ni->email = oldni.email;
	    ni->last_usermask = oldni.usermask;
	    ni->last_realname = oldni.realname;
	    ni->last_quit = NULL;
	    ni->time_registered = oldni.reg;
	    ni->last_seen = oldni.seen;
	    ni->link = NULL;
	    ni->linkcount = 0;
	    ni->accesscount = oldni.naccess;
	    ni->memos.memocount = 0;
	    ni->memos.memomax = MSMaxMemos;
	    ni->memos.memos = NULL;
	    ni->channelcount = 0;
	    ni->channelmax = CSMaxReg;
	    ni->language = DEF_LANGUAGE;
	    ni->status = 0;
	    ni->flags = 0;
	    if (oldni.flags & 0x00000001)
		ni->flags |= NI_KILLPROTECT;
	    if (oldni.flags & 0x00000002)
		ni->flags |= NI_SECURE;
	    if (oldni.flags & 0x00000004)
		ni->status |= NS_VERBOTEN;
	    if (oldni.flags & 0x00000008)
		ni->status |= NS_NO_EXPIRE;
	    if (oldni.flags & 0x00000020)
	        ni->status |= NI_ON_BDD;
	    if (oldni.flags & 0x00001000)
		ni->flags |= NI_PRIVATE;
	    if (oldni.flags & 0x00000080)
		ni->link = (NickInfo *) -1;  /* Flag: this is a linked nick */
	    if (ni->accesscount > NSAccessMax)
		ni->accesscount = NSAccessMax;
	    ni->access = smalloc(ni->accesscount * sizeof(char *));
	    for (j = 0; j < ni->accesscount; j++)
		SAFE(read_string(&ni->access[j], f));
	    while (j < oldni.naccess) {
		SAFE(read_string(&s, f));
		if (s)
		    free(s);
		j++;
	    }
	    for (j = 0; j < oldni.nignore; j++) {
		SAFE(read_string(&s, f));
		if (s)
		    free(s);
		j++;
	    }
	}
    }
    close_db(f);
    /* Resolve links */
    for (i = 33; i < 256; i++) {
	for (ni = nicklists[i]; ni; ni = ni->next) {
	    if (ni->link) {
		int c = (unsigned char) tolower(ni->last_usermask[0]);
		for (ni2 = nicklists[c]; ni2; ni2 = ni2->next) {
		    if (stricmp(ni2->nick, ni->last_usermask) == 0)
			break;
		}
		if (ni2) {
		    ni->link = ni2;
		    strscpy(ni->pass, ni2->pass, PASSMAX);
		} else {
		    fprintf(stderr, "Warning: dropping nick %s linked to nonexistent nick %s\n",
				ni->nick, ni->last_usermask);
		    if (ni->prev)
			ni->prev->next = ni->next;
		    else
			nicklists[i] = ni->next;
		    if (ni->next)
			ni->next->prev = ni->prev;
		}
		free(ni->last_usermask);
		ni->last_usermask = NULL;
	    }
	}
    }
}

/*************************************************************************/

static void m14_load_chan(const char *sourcedir)
{
    char filename[PATH_MAX], *s;
    dbFILE *f;
    int i, j;
    int16 tmp16;
    int32 tmp32;
    ChannelInfo *ci, **last, *prev;
    struct access_ {
	short level;
	short is_nick;
	char *name;
    } access;
    struct akick_ {
	short is_nick;
	short pad;
	char *name;
	char *reason;
    } akick;
    struct oldci_ {
	struct oldci_ *next, *prev;
	char name[64];
	char founder[32];
	char pass[32];
	char *desc;
	char *url;
	time_t reg;
	time_t used;
	long naccess;
	struct access_ *access;
	long nakick;
	struct akick_ *akick;
	char mlock_on[64], mlock_off[64];
	long mlock_limit;
	char *mlock_key;
	char *topic;
	char topic_setter[32];
	time_t topic_time;
	long flags;
	short *levels;
	long resv[3];
    } oldci;

    snprintf(filename, sizeof(filename), "%s/chan.db", sourcedir);
    make_backup(filename);
    f = open_db(NULL, filename, "r");
    if (!f) {
	fprintf(stderr, "Can't open %s for reading\n", filename);
	perror("");
	exit(1);
    }
    SAFE(read_int32(&tmp32, f));
    if (tmp32 != 5) {
	fprintf(stderr, "Wrong version number on %s\n", filename);
	exit(1);
    }
    for (i = 33; i < 256; i++) {
	last = &chanlists[i];
	prev = NULL;
	while (getc_db(f)) {
	    SAFE(read_variable(oldci, f));
	    SAFE(read_string(&oldci.desc, f));
	    if (oldci.url)
		SAFE(read_string(&oldci.url, f));
	    if (oldci.mlock_key)
		SAFE(read_string(&oldci.mlock_key, f));
	    if (oldci.topic)
		SAFE(read_string(&oldci.topic, f));
	    ci = smalloc(sizeof(*ci));
	    strscpy(ci->name, oldci.name, CHANMAX);
	    ci->founder = findnick(oldci.founder);
	    strscpy(ci->founderpass, oldci.pass, PASSMAX);
	    ci->desc = oldci.desc;
	    ci->url = oldci.url;
	    ci->time_registered = oldci.reg;
	    ci->last_used = oldci.used;
	    ci->accesscount = oldci.naccess;
	    ci->akickcount = oldci.nakick;
	    ci->mlock_limit = oldci.mlock_limit;
	    ci->mlock_key = oldci.mlock_key;
	    ci->last_topic = oldci.topic;
	    strscpy(ci->last_topic_setter, oldci.topic_setter, NICKMAX);
	    ci->last_topic_time = oldci.topic_time;
	    ci->memos.memocount = 0;
	    ci->memos.memomax = MSMaxMemos;
	    ci->memos.memos = NULL;
	    ci->flags = oldci.flags & 0x000000FF;
	    if (oldci.naccess > CSAccessMax)
		ci->accesscount = CSAccessMax;
	    else
		ci->accesscount = oldci.naccess;
	    if (oldci.nakick > CSAutokickMax)
		ci->akickcount = CSAutokickMax;
	    else
		ci->akickcount = oldci.nakick;
	    ci->access = smalloc(sizeof(ChanAccess) * ci->accesscount);
	    for (j = 0; j < oldci.naccess; j++) {
		SAFE(read_variable(access, f));
		if (j < ci->accesscount) {
		    ci->access[j].in_use = (access.is_nick == 1);
		    ci->access[j].level = access.level;
		}
	    }
	    for (j = 0; j < oldci.naccess; j++) {
		SAFE(read_string(&s, f));
		if (!s)
		    continue;
		if (j < ci->accesscount && ci->access[j].in_use) {
		    ci->access[j].ni = findnick(s);
		    if (!ci->access[j].ni)
			ci->access[j].in_use = 0;
		}
		free(s);
	    }
	    ci->akick = smalloc(sizeof(AutoKick) * ci->akickcount);
	    for (j = 0; j < oldci.nakick; j++) {
		SAFE(read_variable(akick, f));
		if (j < ci->akickcount) {
		    if (access.is_nick >= 0) {
			ci->akick[j].in_use = 1;
			ci->akick[j].is_nick = akick.is_nick;
		    } else {
			ci->akick[j].in_use = 0;
			ci->akick[j].is_nick = 0;
		    }
		    ci->akick[j].reason = akick.reason;
		}
	    }
	    for (j = 0; j < oldci.nakick; j++) {
		SAFE(read_string(&s, f));
		if (s) {
		    if (j < ci->akickcount && ci->akick[j].in_use) {
			if (ci->akick[j].is_nick) {
			    ci->akick[j].u.ni = findnick(s);
			    if (!ci->akick[j].u.ni) {
				ci->akick[j].in_use = 0;
				ci->akick[j].is_nick = 0;
			    }
			    free(s);
			} else {
			    ci->akick[j].u.mask = s;
			}
		    } else {
			free(s);
		    }
		}
		/* XXX this could fail if there are a lot of akicks with
		 * reasons */
		if (j < ci->akickcount && ci->akick[j].reason) {
		    SAFE(read_string(&ci->akick[j].reason, f));
		    if (!ci->akick[j].in_use && ci->akick[j].reason) {
			free(ci->akick[j].reason);
			ci->akick[j].reason = NULL;
		    }
		}
	    }
	    ci->levels = smalloc(CA_SIZE * sizeof(*ci->levels));
	    ci->levels[CA_AUTOOP]	=  5;
	    ci->levels[CA_AUTOVOICE]	=  3;
	    ci->levels[CA_AUTODEOP]	= -1;
	    ci->levels[CA_NOJOIN]	= -1;
	    ci->levels[CA_INVITE]	=  5;
	    ci->levels[CA_AKICK]	= 10;
	    ci->levels[CA_SET]		= ACCESS_INVALID;
	    ci->levels[CA_CLEAR]	= ACCESS_INVALID;
	    ci->levels[CA_UNBAN]	=  5;
	    ci->levels[CA_OPDEOP]	=  5;
	    ci->levels[CA_ACCESS_LIST]	=  0;
	    ci->levels[CA_ACCESS_CHANGE]=  1;
	    ci->levels[CA_MEMO]		= 10;
	    if (oldci.levels) {
		SAFE(read_int16(&tmp16, f));
		for (j = 0; j < tmp16; j++) {
		    short lev;
		    SAFE(read_variable(lev, f));
		    switch (j) {
			case  0: ci->levels[CA_AUTODEOP]      = lev; break;
			case  1: ci->levels[CA_AUTOVOICE]     = lev; break;
			case  2: ci->levels[CA_AUTOOP]        = lev; break;
			case  6: ci->levels[CA_AKICK]         = lev; break;
			case  9: ci->levels[CA_ACCESS_CHANGE] = lev; break;
			case 10: ci->levels[CA_SET]           = lev; break;
			case 11: ci->levels[CA_INVITE]        = lev; break;
			case 12: ci->levels[CA_UNBAN]         = lev; break;
			case 14: ci->levels[CA_OPDEOP]        = lev; break;
			case 15: ci->levels[CA_CLEAR]         = lev; break;
		    }
		}
	    }
	    /* Only insert in list if founder is found */
	    if (ci->founder) {
		ci->prev = prev;
		ci->next = NULL;
		*last = ci;
		last = &(ci->next);
		prev = ci;
	    } else {
		/* Yeah, it's a memory leak, I know.  Shouldn't matter for
		 * this program. */
	    }
	} /* while more entries */
    } /* for 33..256 */
    close_db(f);
}

/*************************************************************************/

static void m14_load_memo(const char *sourcedir)
{
    char filename[PATH_MAX], *s;
    dbFILE *f;
    int32 tmp32;
    struct memo_ {
	char sender[32];
	long number;
	time_t time;
	char *text;
	long resv[4];
    } memo;
    struct memolist_ {
	struct memolist_ *next, *prev;
	char nick[32];
	long n_memos;
	Memo *memos;
	long reserved[4];
    } memolist;
    NickInfo *ni;
    Memo *m = NULL;
    int i, j;

    snprintf(filename, sizeof(filename), "%s/memo.db", sourcedir);
    make_backup(filename);
    f = open_db(NULL, filename, "r");
    if (!f) {
	fprintf(stderr, "Can't open %s for reading\n", filename);
	perror("");
	exit(1);
    }
    SAFE(read_int32(&tmp32, f));
    if (tmp32 != 5) {
	fprintf(stderr, "Wrong version number on %s\n", filename);
	exit(1);
    }
    for (i = 33; i < 256; i++) {
	while (getc_db(f)) {
	    SAFE(read_variable(memolist, f));
	    ni = findnick(memolist.nick);
	    if (ni) {
		ni->memos.memocount = memolist.n_memos;
		m = smalloc(sizeof(Memo) * ni->memos.memocount);
		ni->memos.memos = m;
	    }
	    for (j = 0; j < memolist.n_memos; j++) {
		SAFE(read_variable(memo, f));
		if (ni) {
		    m[j].number = memo.number;
		    m[j].flags = 0;
		    m[j].time = memo.time;
		    strscpy(m[j].sender, memo.sender, NICKMAX);
		}
	    }
	    for (j = 0; j < memolist.n_memos; j++) {
		SAFE(read_string(&s, f));
		if (ni)
		    m[j].text = s;
		else if (s)
		    free(s);
	    }
	}
    }
    close_db(f);
}

/*************************************************************************/

static void m14_load_sop(const char *sourcedir)
{
    char filename[PATH_MAX], *s;
    char buf[32];
    dbFILE *f;
    int32 tmp32;
    int16 n, i;

    snprintf(filename, sizeof(filename), "%s/sop.db", sourcedir);
    make_backup(filename);
    f = open_db(NULL, filename, "r");
    if (!f) {
	fprintf(stderr, "Can't open %s for reading\n", filename);
	perror("");
	exit(1);
    }
    SAFE(read_int32(&tmp32, f));
    if (tmp32 != 5) {
	fprintf(stderr, "Wrong version number on %s\n", filename);
	exit(1);
    }
    SAFE(read_int16(&n, f));
    if (n > MAX_SERVOPERS)
	n = MAX_SERVOPERS;
    for (i = 0; i < n; i++) {
	SAFE(read_buffer(buf, f));
	services_opers[i] = findnick(buf);
    }
    close_db(f);
}

/*************************************************************************/

static void m14_load_akill(const char *sourcedir)
{
    char filename[PATH_MAX];
    dbFILE *f;
    int32 tmp32;
    int16 i, n;
    struct akill_ {
	char *mask;
	char *reason;
	char who[32];
	time_t time;
    } akill;

    snprintf(filename, sizeof(filename), "%s/akill.db", sourcedir);
    make_backup(filename);
    f = open_db(NULL, filename, "r");
    if (!f) {
	fprintf(stderr, "Can't open %s for reading\n", filename);
	perror("");
	exit(1);
    }
    SAFE(read_int32(&tmp32, f));
    if (tmp32 != 5) {
	fprintf(stderr, "Wrong version number on %s\n", filename);
	exit(1);
    }
    SAFE(read_int16(&n, f));
    nakill = n;
    akills = smalloc(n * sizeof(*akills));
    for (i = 0; i < n; i++) {
	SAFE(read_variable(akill, f));
	strscpy(akills[i].who, akill.who, NICKMAX);
	akills[i].time = akill.time;
	akills[i].expires = 0;
    }
    for (i = 0; i < n; i++) {
	SAFE(read_string(&akills[i].mask, f));
	SAFE(read_string(&akills[i].reason, f));
    }
    close_db(f);
}

/*************************************************************************/

static void m14_load_clone(const char *sourcedir)
{
    char filename[PATH_MAX];
    dbFILE *f;
    int32 tmp32;

    snprintf(filename, sizeof(filename), "%s/clone.db", sourcedir);
    make_backup(filename);
    f = open_db(NULL, filename, "r");
    if (!f) {
	fprintf(stderr, "Can't open %s for reading\n", filename);
	perror("");
	exit(1);
    }
    SAFE(read_int32(&tmp32, f));
    if (tmp32 != 5) {
	fprintf(stderr, "Wrong version number on %s\n", filename);
	exit(1);
    }
    close_db(f);
}

/*************************************************************************/

static void m14_load_message(const char *sourcedir)
{
    char filename[PATH_MAX];
    dbFILE *f;
    int32 tmp32;

    snprintf(filename, sizeof(filename), "%s/message.db", sourcedir);
    make_backup(filename);
    f = open_db(NULL, filename, "r");
    if (!f) {
	fprintf(stderr, "Can't open %s for reading\n", filename);
	perror("");
	exit(1);
    }
    SAFE(read_int32(&tmp32, f));
    if (tmp32 != 5) {
	fprintf(stderr, "Wrong version number on %s\n", filename);
	exit(1);
    }
    close_db(f);
}

/*************************************************************************/

static void m14_load_news(const char *sourcedir)
{
    char filename[PATH_MAX];
    dbFILE *f;
    int32 tmp32;
    int16 i, n;
    struct akill_ {
	char *text;
	int type;
	char who[32];
	time_t time;
    } msg;

    snprintf(filename, sizeof(filename), "%s/news.db", sourcedir);
    make_backup(filename);
    f = open_db(NULL, filename, "r");
    if (!f) {
	fprintf(stderr, "Can't open %s for reading\n", filename);
	perror("");
	exit(1);
    }
    SAFE(read_int32(&tmp32, f));
    if (tmp32 != 5) {
	fprintf(stderr, "Wrong version number on %s\n", filename);
	exit(1);
    }
    SAFE(read_int16(&n, f));
    nnews = n;
    news = smalloc(n * sizeof(*news));
    for (i = 0; i < n; i++) {
	SAFE(read_variable(msg, f));
	news[i].type = msg.type;
	strscpy(news[i].who, msg.who, NICKMAX);
	news[i].time = msg.time;
    }
    for (i = 0; i < n; i++)
	SAFE(read_string(&news[i].text, f));
    close_db(f);
}

/*************************************************************************/

#undef SAFE

void load_magick_14b2(const char *sourcedir, int verbose)
{
    if (verbose)
	printf("Loading nick.db...\n");
    m14_load_nick(sourcedir);
    if (verbose)
	printf("Loading chan.db...\n");
    m14_load_chan(sourcedir);
    if (verbose)
	printf("Loading memo.db...\n");
    m14_load_memo(sourcedir);
    if (verbose)
	printf("Loading sop.db...\n");
    m14_load_sop(sourcedir);
    if (verbose)
	printf("Loading akill.db...\n");
    m14_load_akill(sourcedir);
    if (verbose)
	printf("Loading clone.db...\n");
    m14_load_clone(sourcedir);
    if (verbose)
	printf("Loading message.db...\n");
    m14_load_message(sourcedir);
    if (verbose)
	printf("Loading news.db...\n");
    m14_load_news(sourcedir);
    if (verbose)
	printf("Data files successfully loaded.\n");
}

/*************************************************************************/
/***************************** Database saving ***************************/
/*************************************************************************/

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	fprintf(stderr, "Write error on " SERVICES_DIR "/");	\
	perror(NickDBName);					\
	exit(1);						\
    }								\
} while (0)

void save_ns_dbase(void)
{
    char buf[256];
    dbFILE *f;
    int i, j;
    NickInfo *ni;
    char **access;
    Memo *memos;

    snprintf(buf, sizeof(buf), "%s/%s", SERVICES_DIR, NickDBName);
    if (!(f = open_db(NULL, buf, "w")))
	return;
    for (i = 0; i < 256; i++) {
	for (ni = nicklists[i]; ni; ni = ni->next) {
	    SAFE(write_int8(1, f));
	    SAFE(write_buffer(ni->nick, f));
	    SAFE(write_buffer(ni->pass, f));
	    SAFE(write_string(ni->url, f));
	    SAFE(write_string(ni->email, f));
	    SAFE(write_string(ni->last_usermask, f));
	    SAFE(write_string(ni->last_realname, f));
	    SAFE(write_string(ni->last_quit, f));
	    SAFE(write_int32(ni->time_registered, f));
	    SAFE(write_int32(ni->last_seen, f));
	    SAFE(write_int16(ni->status, f));
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

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	fprintf(stderr, "Write error on " SERVICES_DIR "/");	\
	perror(ChanDBName);					\
	exit(1);						\
    }								\
} while (0)
void save_cs_dbase(void)
{
    char buf[256];
    dbFILE *f;
    int i, j;
    ChannelInfo *ci;
    Memo *memos;

    snprintf(buf, sizeof(buf), "%s/%s", SERVICES_DIR, ChanDBName);
    if (!(f = open_db(NULL, buf, "w")))
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

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	fprintf(stderr, "Write error on " SERVICES_DIR "/");	\
	perror(OperDBName);					\
	exit(1);						\
    }								\
} while (0)

void save_os_dbase(void)
{
    char buf[256];
    dbFILE *f;
    int16 i, count = 0;

    snprintf(buf, sizeof(buf), "%s/%s", SERVICES_DIR, OperDBName);
    if (!(f = open_db(NULL, buf, "w")))
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
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_opers[i])
	    count++;
    }
    SAFE(write_int16(count, f));
    for (i = 0; i < MAX_SERVOPERS; i++) {
	if (services_opers[i])
	    SAFE(write_string(services_opers[i]->nick, f));
    }
    SAFE(write_int32(maxusercnt, f));
    SAFE(write_int32(maxusertime, f));
    close_db(f);
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	fprintf(stderr, "Write error on " SERVICES_DIR "/");	\
	perror(AutokillDBName);					\
	exit(1);						\
    }								\
} while (0)

void save_akill(void)
{
    char buf[256];
    dbFILE *f;
    int i;

    snprintf(buf, sizeof(buf), "%s/%s", SERVICES_DIR, AutokillDBName);
    if (!(f = open_db(NULL, buf, "w")))
	return;
    write_int16(nakill, f);
    for (i = 0; i < nakill; i++) {
	SAFE(write_string(akills[i].mask, f));
	SAFE(write_string(akills[i].reason, f));
	SAFE(write_buffer(akills[i].who, f));
	SAFE(write_int32(akills[i].time, f));
	SAFE(write_int32(akills[i].expires, f));
    }
    close_db(f);
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {						\
    if ((x) < 0) {						\
	fprintf(stderr, "Write error on " SERVICES_DIR "/");	\
	perror(NewsDBName);					\
	exit(1);						\
    }								\
} while (0)

void save_news()
{
    char buf[256];
    dbFILE *f;
    int i;

    snprintf(buf, sizeof(buf), "%s/%s", SERVICES_DIR, NewsDBName);
    if (!(f = open_db(NULL, buf, "w")))
	return;
    SAFE(write_int16(nnews, f));
    for (i = 0; i < nnews; i++) {
	SAFE(write_int16(news[i].type, f));
	SAFE(write_int32(news[i].num, f));
	SAFE(write_string(news[i].text, f));
	SAFE(write_buffer(news[i].who, f));
	SAFE(write_int32(news[i].time, f));
    }
    close_db(f);
}

#undef SAFE

/*************************************************************************/
/****************************** Main program *****************************/
/*************************************************************************/

void usage(const char *progname)
{
    fprintf(stderr, "Usage: %s [-v] [-d sourcedir] [+program-name]\n"
		    "The following program names are known:\n"
		    "    magick-1.4b2\n", progname);
    exit(1);
}

/*************************************************************************/

int main(int ac, char **av)
{
    char *sourcedir = NULL;	/* Source data file directory */
    int verbose = 0;		/* Verbose output? */
    void (*load)(const char *dir, int verbose) = NULL;
    int i;

    for (i = 1; i < ac; i++) {
	if (av[i][0] == '-') {
	    if (av[i][1] == 'd') {
		if (av[i][2])
		    sourcedir = av[i]+2;
		else if (i+1 >= ac)
		    usage(av[0]);
		else
		    sourcedir = av[++i];
		if (*sourcedir != '/') {
		    fprintf(stderr, "Source directory path must be absolute\n");
		    usage(av[0]);
		}
	    } else if (av[i][1] == 'v') {
		verbose++;
	    } else {
		if (av[i][1] != 'h')
		    fprintf(stderr, "Unknown option -%c\n", av[i][1]);
		usage(av[0]);
	    }
	} else if (av[i][0] == '+') {
	    if (strcmp(av[i]+1, "magick-1.4b2") == 0)
		load = load_magick_14b2;
	    else {
		fprintf(stderr, "Unknown program name `%s'\n", av[i]+1);
		usage(av[0]);
	    }
	} else {
	    usage(av[0]);
	}
    }

    chdir(SERVICES_DIR);
    if (!read_config())
	return 1;

    if (!load) {
	if (!sourcedir) {
	    fprintf(stderr, "At least one of -d and +name must be specified\n");
	    usage(av[0]);
	}
	fprintf(stderr, "Can't determine data file type; use +name option\n");
	usage(av[0]);
    }

    load(sourcedir, verbose);
    if (verbose)
	printf("Saving new NickServ database\n");
    save_ns_dbase();
    if (verbose)
	printf("Saving new ChanServ database\n");
    save_cs_dbase();
    if (verbose)
	printf("Saving new OperServ database\n");
    save_os_dbase();
    if (verbose)
	printf("Saving new AKILL database\n");
    save_akill();
    if (verbose)
	printf("Saving new news database\n");
    save_news();
    return 0;
}

/*************************************************************************/
