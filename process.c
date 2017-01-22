/* Main processing code for Services.
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
#include "messages.h"

/*************************************************************************/
/*************************************************************************/

/* Use ignore code? */
int allow_ignore = 1;

/* People to ignore (hashed by first character of nick). */
IgnoreData *ignore[256];

/*************************************************************************/

/* add_ignore: Add someone to the ignorance list for the next `delta'
 *             seconds.
 */

void add_ignore(const char *nick, time_t delta)
{
    IgnoreData *ign;
    char who[NICKMAX];
    time_t now = time(NULL);
    IgnoreData **whichlist = &ignore[tolower(nick[0])];

    strscpy(who, nick, NICKMAX);
    for (ign = *whichlist; ign; ign = ign->next) {
	if (stricmp(ign->who, who) == 0)
	    break;
    }
    if (ign) {
	if (ign->time > now)
	    ign->time += delta;
	else
	    ign->time = now + delta;
    } else {
	ign = smalloc(sizeof(*ign));
	strscpy(ign->who, who, sizeof(ign->who));
	ign->time = now + delta;
	ign->next = *whichlist;
	*whichlist = ign;
    }
}

/*************************************************************************/

/* get_ignore: Retrieve an ignorance record for a nick.  If the nick isn't
 *             being ignored, return NULL and flush the record from the
 *             in-core list if it exists (i.e. ignore timed out).
 */

IgnoreData *get_ignore(const char *nick)
{
    IgnoreData *ign, *prev;
    time_t now = time(NULL);
    IgnoreData **whichlist = &ignore[tolower(nick[0])];

    for (ign = *whichlist, prev = NULL; ign; prev = ign, ign = ign->next) {
	if (stricmp(ign->who, nick) == 0)
	    break;
    }
    if (ign && ign->time <= now) {
	if (prev)
	    prev->next = ign->next;
	else
	    *whichlist = ign->next;
	free(ign);
	ign = NULL;
    }
    return ign;
}

/*************************************************************************/
/*************************************************************************/

/* split_buf:  Split a buffer into arguments and store the arguments in an
 *             argument vector pointed to by argv (which will be malloc'd
 *             as necessary); return the argument count.  If colon_special
 *             is non-zero, then treat a parameter with a leading ':' as
 *             the last parameter of the line, per the IRC RFC.  Destroys
 *             the buffer by side effect.
 */

int split_buf(char *buf, char ***argv, int colon_special)
{
    int argvsize = 8;
    int argc;
    char *s;

    *argv = smalloc(sizeof(char *) * argvsize);
    argc = 0;
    while (*buf) {
	if (argc == argvsize) {
	    argvsize += 8;
	    *argv = srealloc(*argv, sizeof(char *) * argvsize);
	}
	if (*buf == ':') {
	    (*argv)[argc++] = buf+1;
	    buf = "";
	} else {
	    s = strpbrk(buf, " ");
	    if (s) {
		*s++ = 0;
		while (isspace(*s))
		    s++;
	    } else {
		s = buf + strlen(buf);
	    }
	    (*argv)[argc++] = buf;
	    buf = s;
	}
    }
    return argc;
}

/*************************************************************************/

/* process:  Main processing routine.  Takes the string in inbuf (global
 *           variable) and does something appropriate with it. */

void process()
{
    char source[64];
    char cmd[64];
    char buf[512];		/* Longest legal IRC command line */
    char *s;
    int ac;			/* Parameters for the command */
    char **av;
    Message *m;
#ifdef IRC_UNDERNET_P10
    User *u=NULL;
#endif    

    /* If debugging, log the buffer. */
    if (debug)
	log("debug: Received: %s", inbuf);

    /* First make a copy of the buffer so we have the original in case we
     * crash - in that case, we want to know what we crashed on. */
    strscpy(buf, inbuf, sizeof(buf));

    /* Split the buffer into pieces. */
    if (*buf == ':') {
	s = strpbrk(buf, " ");
	if (!s)
	    return;
	*s = 0;
	while (isspace(*++s))
	    ;
	strscpy(source, buf+1, sizeof(source));
	memmove(buf, s, strlen(s)+1);
    } else {
#ifdef IRC_UNDERNET_P10
        if (strncmp(buf, "SERVER" ,strlen("SERVER"))==0) {
            s = strdup(buf);
            source[0] = '\0';
        } else {
            s = strpbrk(buf, " ");
            if (!s)
                return;
            *s = 0;
            while (isspace(*++s));
            strscpy(source, buf, sizeof(source));
            if (strlen(source) == 3) {
                u = finduserP10(source); 
                if(u != NULL) strscpy(source, u->nick, sizeof(source));
                else return; 
            }  
            memmove(buf, s, strlen(s)+1);
        }            
#else            
	*source = 0;
#endif
    }
    
    if (!*buf)
	return;
    s = strpbrk(buf, " ");
    if (s) {
	*s = 0;
	while (isspace(*++s))
	    ;
    } else
	s = buf + strlen(buf);
    strscpy(cmd, buf, sizeof(cmd));
    ac = split_buf(s, &av, 1);

    /* Do something with the message. */
    m = find_message(cmd);
    if (m) {
	if (m->func)
	    m->func(source, ac, av);
    } else {
       log("unknown message from server (%s)", inbuf); 
//       send_cmd(ServerName, "PRIVMSG #devels :DEBUG: %s", inbuf);
    }

    /* Free argument list we created */
    free(av);
}

/*************************************************************************/
