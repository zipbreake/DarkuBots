/* Services configuration.
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

#ifndef CONFIG_H
#define CONFIG_H

/* Note that most of the options which used to be here have been moved to
 * services.conf. */

/*************************************************************************/

/* Should we try and deal with old (v2.x) databases?  Define this if you're
 * upgrading from v2.2.26 or earlier. */
/* #define COMPATIBILITY_V2 */


/******* General configuration *******/

/* Name of configuration file (in Services directory) */
#define SERVICES_CONF	"services.conf"

/* Name of log file (in Services directory) */
#define LOG_FILENAME	"services.log"

/* Maximum amount of data from/to the network to buffer (bytes). */
#define NET_BUFSIZE	4096

/******* NickServ configuration *******/

/* Default language for newly registered nicks (and nicks imported from
 * old databases); see services.h for available languages (search for
 * "LANG_").  Unless you're running a regional network, you should probably
 * leave this at LANG_EN_US. */
#define DEF_LANGUAGE	LANG_ES


/******* OperServ configuration *******/

/* What is the maximum number of Services admins we will allow? */
#define MAX_SERVADMINS	32

/* What is the maximum number of Services operators we will allow? */
#define MAX_SERVOPERS	64

/* How big a hostname list do we keep for clone detection?  On large nets
 * (over 500 simultaneous users or so), you may want to increase this if
 * you want a good chance of catching clones. */
#define CLONE_DETECT_SIZE 16

/* Define this to enable OperServ's debugging commands (Services root
 * only).  These commands are undocumented; "use the source, Luke!" */
/* #define DEBUG_COMMANDS */
/* #define DEBUG_COMMANDS */

/******************* END OF USER-CONFIGURABLE SECTION ********************/


/* Size of input buffer (note: this is different from BUFSIZ)
 * This must be big enough to hold at least one full IRC message, or messy
 * things will happen. */
#define BUFSIZE		512


/* Extra warning:  If you change these, your data files will be unusable! */

/* Maximum length of a channel name, including the trailing null.  Any
 * channels with a length longer than (CHANMAX-1) including the leading #
 * will not be usable with ChanServ. */
#define CHANMAX		255

/* Maximum length of a nickname, including the trailing null.  This MUST be
 * at least one greater than the maximum allowable nickname length on your
 * network, or people will run into problems using Services!  The default
 * (32) works with all servers I know of. */
#define NICKMAX		32

/* Maximum length of a password */
#define PASSMAX		32

/**************************************************************************/

#endif	/* CONFIG_H */
