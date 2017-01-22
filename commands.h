/* Declarations for command data.
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

/* Structure for information about a *Serv command. */

typedef struct {
    const char *name;
    void (*routine)(User *u);
    int (*has_priv)(User *u);	/* Returns 1 if user may use command, else 0 */

    /* Regrettably, these are hard-coded to correspond to current privilege
     * levels (v4.0).  Suggestions for better ways to do this are
     * appreciated.
     */
    int helpmsg_all;	/* Displayed to all users; -1 = no message */
    int helpmsg_reg;	/* Displayed to regular users only */
    int helpmsg_oper;	/* Displayed to Services operators only */
    int helpmsg_admin;	/* Displayed to Services admins only */
    int helpmsg_root;	/* Displayed to Services root only */
    const char *help_param1;
    const char *help_param2;
    const char *help_param3;
    const char *help_param4;
} Command;

/*************************************************************************/

/* Routines for looking up commands.  Command lists are arrays that must be
 * terminated with a NULL name.
 */

extern Command *lookup_cmd(Command *list, const char *name);
extern void run_cmd(const char *service, User *u, Command *list,
		const char *name);
extern void help_cmd(const char *service, User *u, Command *list,
		const char *name);

/*************************************************************************/
