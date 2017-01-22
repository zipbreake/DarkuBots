/* Set default values for any constants that should be in include files but
 * aren't, or that have wacky values.
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

/*************************************************************************/

#ifndef NAME_MAX
# define NAME_MAX 255
#endif

#ifndef BUFSIZ
# define BUFSIZ 256
#else
# if BUFSIZ < 256
#  define BUFSIZ 256
# endif
#endif

/* Length of an array: */
#define lenof(a)	(sizeof(a) / sizeof(*(a)))

/* Telling compilers about printf()-like functions: */
#ifdef __GNUC__
# define FORMAT(type,fmt,start) __attribute__((format(type,fmt,start)))
#else
# define FORMAT(type,fmt,start)
#endif

/*************************************************************************/
