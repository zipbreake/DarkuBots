/* HelpServ functions.
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
#include "language.h"
#include <sys/stat.h>

static void do_help(const char *whoami, const char *source, char *topic);

/*************************************************************************/

/* helpserv:  Main HelpServ routine.  `whoami' is what nick we should send
 * messages as: this won't necessarily be s_HelpServ, because other
 * routines call this one to display help files. */

void helpserv(const char *whoami, const char *source, char *buf)
{
    char *cmd, *topic, *s;
#ifdef IRC_UNDERNET_P10    
    User *u = finduser(source);
    
    if (!u)
        return;
#endif        

    topic = buf ? sstrdup(buf) : NULL;
    cmd = strtok(buf, " ");
    if (cmd && stricmp(cmd, "\1PING") == 0) {
	if (!(s = strtok(NULL, "")))
	    s = "\1";
#ifdef IRC_UNDERNET_P10
	notice(s_HelpServ, u->numerico, "\1PING %s", s);
    } else {
        do_help(whoami, u->numerico, topic);    
#else
        notice(s_HelpServ, source, "\1PING %s", s);
    } else if (stricmp(cmd, "\1VERSION\1") == 0) {
        notice(s_HelpServ, source, "\1VERSION %s %s -- %s\1",
        PNAME, s_HelpServ, version_build);
            
    } else {  
	do_help(whoami, source, topic);
#endif	
    }
    if (topic)
	free(topic);
}

/*************************************************************************/
/*********************** HelpServ command routines ***********************/
/*************************************************************************/

/* Return a help message. */

static void do_help(const char *whoami, const char *source, char *topic)
{
    FILE *f;
    struct stat st;
    char buf[256], *ptr, *s;
    char *old_topic;	/* an unclobbered (by strtok) copy */
    User *u = finduser(source);

    if (!topic || !*topic)
	topic = "help";
    old_topic = sstrdup(topic);

    /* As we copy path parts, (1) lowercase everything and (2) make sure
     * we don't let any special characters through -- this includes '.'
     * (which could get parent dir) or '/' (which couldn't _really_ do
     * anything nasty if we keep '.' out, but better to be on the safe
     * side).  Special characters turn into '_'.
     */
    strscpy(buf, HelpDir, sizeof(buf));
    ptr = buf + strlen(buf);
    for (s = strtok(topic, " "); s && ptr-buf < sizeof(buf)-1;
						s = strtok(NULL, " ")) {
	*ptr++ = '/';
	while (*s && ptr-buf < sizeof(buf)-1) {
	    if (*s == '.' || *s == '/')
		*ptr++ = '_';
	    else
		*ptr++ = tolower(*s);
	    ++s;
	}
	*ptr = 0;
    }

    /* If we end up at a directory, go for an "index" file/dir if
     * possible.
     */
    while (ptr-buf < sizeof(buf)-1
		&& stat(buf, &st) == 0 && S_ISDIR(st.st_mode)) {
	*ptr++ = '/';
	strscpy(ptr, "index", sizeof(buf) - (ptr-buf));
	ptr += strlen(ptr);
    }

    /* Send the file, if it exists.
     */
    if (!(f = fopen(buf, "r"))) {
	if (debug)
	    log_perror("debug: No encuentra el archivo de ayuda %s", buf);
	if (u) {
	    notice_lang(whoami, u, NO_HELP_AVAILABLE, old_topic);
#ifndef IRC_UNDERNET_P10
	} else {
	    privmsg(whoami, source,
			"Lo siento, no hay ayuda disponible para 12%s.", old_topic);
#endif			
	}
	free(old_topic);
	return;
    }
    while (fgets(buf, sizeof(buf), f)) {
	s = strtok(buf, "\n");
	/* Use this odd construction to prevent any %'s in the text from
	 * doing weird stuff to the output.  Also replace blank lines by
	 * spaces (see send.c/notice_list() for an explanation of why).
	 */
#ifdef IRC_UNDERNET_P10
	privmsg(whoami, u->numerico, "%s", s ? s : " ");
#else
        privmsg(whoami, source, "%s", s ? s : " ");
#endif	
    }
    fclose(f);
    free(old_topic);
}

/*************************************************************************/
