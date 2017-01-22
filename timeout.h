/* Time-delay routine include stuff.
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

#ifndef TIMEOUT_H
#define TIMEOUT_H

#include <time.h>


/* Definitions for timeouts: */
typedef struct timeout_ Timeout;
struct timeout_ {
    Timeout *next, *prev;
    time_t settime, timeout;
    int repeat;			/* Does this timeout repeat indefinitely? */
    void (*code)(Timeout *);	/* This structure is passed to the code */
    void *data;			/* Can be anything */
};


/* Check the timeout list for any pending actions. */
extern void check_timeouts(void);

/* Add a timeout to the list to be triggered in `delay' seconds.  Any
 * timeout added from within a timeout routine will not be checked during
 * that run through the timeout list.
 */
extern Timeout *add_timeout(int delay, void (*code)(Timeout *), int repeat);

/* Remove a timeout from the list (if it's there). */
extern void del_timeout(Timeout *t);

#ifdef DEBUG_COMMANDS
/* Send the list of timeouts to the given user. */
extern void send_timeout_list(User *u);
#endif


#endif	/* TIMEOUT_H */
