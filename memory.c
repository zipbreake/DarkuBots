/* Memory management routines.
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

/*************************************************************************/
/*************************************************************************/

/* smalloc, scalloc, srealloc, sstrdup:
 *	Versions of the memory allocation functions which will cause the
 *	program to terminate with an "Out of memory" error if the memory
 *	cannot be allocated.  (Hence, the return value from these functions
 *	is never NULL.)
 */

void *smalloc(long size)
{
    void *buf;

    if (!size) {
	log("smalloc: Illegal attempt to allocate 0 bytes");
	size = 1;
    }
    buf = malloc(size);
    if (!buf)
	raise(SIGUSR1);
    return buf;
}

void *scalloc(long elsize, long els)
{
    void *buf;

    if (!elsize || !els) {
	log("scalloc: Illegal attempt to allocate 0 bytes");
	elsize = els = 1;
    }
    buf = calloc(elsize, els);
    if (!buf)
	raise(SIGUSR1);
    return buf;
}

void *srealloc(void *oldptr, long newsize)
{
    void *buf;

    if (!newsize) {
	log("srealloc: Illegal attempt to allocate 0 bytes");
	newsize = 1;
    }
    buf = realloc(oldptr, newsize);
    if (!buf)
	raise(SIGUSR1);
    return buf;
}

char *sstrdup(const char *s)
{
    char *t = strdup(s);
    if (!t)
	raise(SIGUSR1);
    return t;
}

/*************************************************************************/
/*************************************************************************/

/* In the future: malloc() replacements that tell us if we're leaking and
 * maybe do sanity checks too... */

/*************************************************************************/
