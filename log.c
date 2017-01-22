/* Logging routines.
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

static FILE *logfile;

/*************************************************************************/

/* Open the log file.  Return -1 if the log file could not be opened, else
 * return 0. */

int open_log(void)
{
    if (logfile)
	return 0;
    logfile = fopen(log_filename, "a");
    if (logfile)
	setbuf(logfile, NULL);
    return logfile!=NULL ? 0 : -1;
}

/* Close the log file. */

void close_log(void)
{
    if (!logfile)
	return;
    fclose(logfile);
    logfile = NULL;
}

/*************************************************************************/

/* Rename the log file. Errors are wallop'ed if *u is NULL. Returns 1 if the
 * log file is successfully renamed otherwise 0 is returned. newname should 
 * not be user-supplied - rather use the date or something. Pass NULL for *u 
 * if called automatically. 
 * There is lots of room for improvement in the use of this function!
 *
 * -TheShadow (15 May 1999)
 */

int rename_log(User *u, const char *newname)
{
    FILE *temp_fp;
    int success = 1;

    if ((temp_fp = fopen(newname, "r"))) {
 	fclose(temp_fp);
	if (u) {
	    notice_lang(s_OperServ, u, OPER_ROTATELOG_FILE_EXISTS, newname);
	} else {
            canalopers(NULL, "WARNING: Could not rename logfile, a file with "
			"the name \2%s\2 already exists!", newname);
	}
	log("ERROR: Could not rename logfile, a file with the name \2%s\2 "
			"already exists!", newname);
	return 0;
    }

    if (debug)
	log("Renaming logfile from %s to %s ...", log_filename, newname);

    close_log();

    /* Note: If something goes wrong here, there is no way for us to log it. 
     * We have to rely on the notice or an oper seeing the wallop. -TheShadow 
     */
    if (rename(log_filename, newname) == 0) {
	if (u) {
	    notice_lang(s_OperServ, u, OPER_ROTATELOG_RENAME_DONE, newname);
	}

    } else {
	if (u) {
	    notice_lang(s_OperServ, u, OPER_ROTATELOG_RENAME_FAIL, 
			strerror(errno));
	} else {
	    canalopers(NULL, "WARNING: Logfile could not be renamed: %s", 
			strerror(errno));
	}
	success = 0;
    }

    if (open_log() == -1) {
        if (u) {
            notice_lang(s_OperServ, u, OPER_ROTATELOG_OPEN_LOG_FAIL,
			strerror(errno));
        } else {
	    /* If we ever get here, then we're pretty stuffed. Services is
	     * probably going to have to be restarted. All we can do is yell
	     * for help and tell who ever is listening to find the services
	     * administrator (root) and notify them. -TheShadow */

            canalopers(NULL, "WARNING: Logging could not be restarted: \2%s\2",
			strerror(errno));
	    canalopers(NULL, "Please notify your services administrator, \2%s\2, "
			"and include the above error message.", ServicesRoot);
	    success = 0;
	}
    }

    if (debug && !success)
	log("Logfile could not be renamed: %s", strerror(errno));

    if (success) {
	log("Fresh logfile started. Previous logfile has been renamed to: %s",
			 newname);
    } else if (debug) {
	/* pointless if open_log() fails */
	log("ERROR: Logfile could not be renamed: %s", strerror(errno));
    }

    return success;
}

/* This is the function that should be called by a user, and currently is :) 
 * Basically we make a nice new log filename and then call the rename_log
 * function. Due to the nature of life, we have to pass the User structure 
 * through to the rename code. This is the only sain way of being able to
 * notify users of problems while renaming the log file - we might not be able
 * to log the problem.
 * -TheShadow (15 May 1999) 
 */

void rotate_log(User *u)
{
    time_t t;
    struct tm tm;
    char newname[23];

    time(&t);
    tm = *localtime(&t);
    strftime(newname, sizeof(newname)-1, "services-%Y%m%d.log", &tm);

    rename_log(u, newname);
}

/*************************************************************************/

/* Log stuff to the log file with a datestamp.  Note that errno is
 * preserved by this routine and log_perror().
 */

void _log(const char *fmt, ...)
{
    va_list args;
    time_t t;
    struct tm tm;
    char buf[256];
    int errno_save = errno;

    va_start(args, fmt);
    time(&t);
    tm = *localtime(&t);
#if HAVE_GETTIMEOFDAY
    if (debug) {
	char *s;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	strftime(buf, sizeof(buf)-1, "[%b %d %H:%M:%S", &tm);
	s = buf + strlen(buf);
	s += snprintf(s, sizeof(buf)-(s-buf), ".%06d", tv.tv_usec);
	strftime(s, sizeof(buf)-(s-buf)-1, " %Y] ", &tm);
    } else {
#endif
	strftime(buf, sizeof(buf)-1, "[%b %d %H:%M:%S %Y] ", &tm);
#if HAVE_GETTIMEOFDAY
    }
#endif
    if (logfile) {
	fputs(buf, logfile);
	vfprintf(logfile, fmt, args);
	fputc('\n', logfile);
    }
    if (nofork) {
	fputs(buf, stderr);
	vfprintf(stderr, fmt, args);
	fputc('\n', stderr);
    }
    errno = errno_save;
}


/* Like log(), but tack a ": " and a system error message (as returned by
 * strerror()) onto the end.
 */

void log_perror(const char *fmt, ...)
{
    va_list args;
    time_t t;
    struct tm tm;
    char buf[256];
    int errno_save = errno;

    va_start(args, fmt);
    time(&t);
    tm = *localtime(&t);
#if HAVE_GETTIMEOFDAY
    if (debug) {
	char *s;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	strftime(buf, sizeof(buf)-1, "[%b %d %H:%M:%S", &tm);
	s = buf + strlen(buf);
	s += snprintf(s, sizeof(buf)-(s-buf), ".%06d", tv.tv_usec);
	strftime(s, sizeof(buf)-(s-buf)-1, " %Y] ", &tm);
    } else {
#endif
	strftime(buf, sizeof(buf)-1, "[%b %d %H:%M:%S %Y] ", &tm);
#if HAVE_GETTIMEOFDAY
    }
#endif
    if (logfile) {
	fputs(buf, logfile);
	vfprintf(logfile, fmt, args);
	fprintf(logfile, ": %s\n", strerror(errno_save));
    }
    if (nofork) {
	fputs(buf, stderr);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, ": %s\n", strerror(errno_save));
    }
    errno = errno_save;
}

/*************************************************************************/

/* We've hit something we can't recover from.  Let people know what
 * happened, then go down.
 */

void fatal(const char *fmt, ...)
{
    va_list args;
    time_t t;
    struct tm tm;
    char buf[256], buf2[4096];

    va_start(args, fmt);
    time(&t);
    tm = *localtime(&t);
#if HAVE_GETTIMEOFDAY
    if (debug) {
	char *s;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	strftime(buf, sizeof(buf)-1, "[%b %d %H:%M:%S", &tm);
	s = buf + strlen(buf);
	s += snprintf(s, sizeof(buf)-(s-buf), ".%06d", tv.tv_usec);
	strftime(s, sizeof(buf)-(s-buf)-1, " %Y] ", &tm);
    } else {
#endif
	strftime(buf, sizeof(buf)-1, "[%b %d %H:%M:%S %Y] ", &tm);
#if HAVE_GETTIMEOFDAY
    }
#endif
    vsnprintf(buf2, sizeof(buf2), fmt, args);
    if (logfile)
	fprintf(logfile, "%sFATAL: %s\n", buf, buf2);
    if (nofork)
	fprintf(stderr, "%sFATAL: %s\n", buf, buf2);
    if (servsock >= 0)
	canalopers(NULL, "FATAL ERROR!  %s", buf2);
    exit(1);
}


/* Same thing, but do it like perror(). */

void fatal_perror(const char *fmt, ...)
{
    va_list args;
    time_t t;
    struct tm tm;
    char buf[256], buf2[4096];
    int errno_save = errno;

    va_start(args, fmt);
    time(&t);
    tm = *localtime(&t);
#if HAVE_GETTIMEOFDAY
    if (debug) {
	char *s;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	strftime(buf, sizeof(buf)-1, "[%b %d %H:%M:%S", &tm);
	s = buf + strlen(buf);
	s += snprintf(s, sizeof(buf)-(s-buf), ".%06d", tv.tv_usec);
	strftime(s, sizeof(buf)-(s-buf)-1, " %Y] ", &tm);
    } else {
#endif
	strftime(buf, sizeof(buf)-1, "[%b %d %H:%M:%S %Y] ", &tm);
#if HAVE_GETTIMEOFDAY
    }
#endif
    vsnprintf(buf2, sizeof(buf2), fmt, args);
    if (logfile)
	fprintf(logfile, "%sFATAL: %s: %s\n", buf, buf2, strerror(errno_save));
    if (stderr)
	fprintf(stderr, "%sFATAL: %s: %s\n", buf, buf2, strerror(errno_save));
    if (servsock >= 0)
	canalopers(NULL, "FATAL ERROR!  %s: %s", buf2, strerror(errno_save));
    exit(1);
}

/*************************************************************************/
