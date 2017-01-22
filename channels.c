/* Channel-handling routines.
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

#define HASH(chan)	((chan)[1] ? ((chan)[1]&31)<<5 | ((chan)[2]&31) : 0)
static Channel *chanlist[1024];

/*************************************************************************/

/* Return statistics.  Pointers are assumed to be valid. */

void get_channel_stats(long *nrec, long *memuse)
{
    long count = 0, mem = 0;
    Channel *chan;
    struct c_userlist *cu;
    int i, j;

    for (i = 0; i < 1024; i++) {
	for (chan = chanlist[i]; chan; chan = chan->next) {
	    count++;
	    mem += sizeof(*chan);
	    if (chan->topic)
		mem += strlen(chan->topic)+1;
	    if (chan->key)
		mem += strlen(chan->key)+1;
	    mem += sizeof(char *) * chan->bansize;
	    for (j = 0; j < chan->bancount; j++) {
		if (chan->bans[j])
		    mem += strlen(chan->bans[j])+1;
	    }
	    for (cu = chan->users; cu; cu = cu->next)
		mem += sizeof(*cu);
	    for (cu = chan->chanops; cu; cu = cu->next)
		mem += sizeof(*cu);
	    for (cu = chan->voices; cu; cu = cu->next)
		mem += sizeof(*cu);
	    for (cu = chan->halfops; cu; cu = cu->next)
		mem += sizeof(*cu);
	}
    }
    *nrec = count;
    *memuse = mem;
}

/*************************************************************************/

#ifdef DEBUG_COMMANDS

/* Send the current list of channels to the named user. */

void send_channel_list(User *user)
{
    Channel *c;
    char s[16], buf[512], *end;
    struct c_userlist *u, *u2;
    int isop, isvoice, ishalfop;
#ifdef IRC_UNDERNET_P10
    const char *source = user->numerico;
#else    
    const char *source = user->nick;
#endif

    for (c = firstchan(); c; c = nextchan()) {
	snprintf(s, sizeof(s), " %d", c->limit);
	privmsg(s_OperServ, source, "%s %lu +%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s %s",
				c->name, c->creation_time,
				(c->mode&CMODE_I) ? "i" : "",
				(c->mode&CMODE_M) ? "m" : "",
				(c->mode&CMODE_N) ? "n" : "",
				(c->mode&CMODE_P) ? "p" : "",
				(c->mode&CMODE_s) ? "s" : "",
				(c->mode&CMODE_T) ? "t" : "",

                                (c->mode&CMODE_c) ? "c" : "",
                                (c->mode&CMODE_C) ? "C" : "",
                                (c->mode&CMODE_O) ? "O" : "",
                                				
#if defined (IRC_TERRA) || defined (IRC_HISPANO)
				(c->mode&CMODE_r) ? "r" : "",
                                (c->mode&CMODE_R) ? "R" : "",				
#else
				"",
				"",
#endif
#ifdef IRC_HISPANO
                                (c->mode&CMODE_A) ? "A" : "",
                                (c->mode&CMODE_S) ? "S" : "",                                
#else
                                "",
                                "",                                
#endif
				(c->limit)        ? "l" : "",
				(c->key)          ? "k" : "",
				(c->limit)        ?  s  : "",
				(c->key)          ? " " : "",
				(c->key)          ? c->key : "",
				c->topic ? c->topic : "");
	end = buf;
	end += snprintf(end, sizeof(buf)-(end-buf), "%s", c->name);
	for (u = c->users; u; u = u->next) {
	    isop = isvoice = ishalfop = 0;
	    for (u2 = c->chanops; u2; u2 = u2->next) {
		if (u2->user == u->user) {
		    isop = 1;
		    break;
		}
	    }
	    for (u2 = c->voices; u2; u2 = u2->next) {
		if (u2->user == u->user) {
		    isvoice = 1;
		    break;
		}
	    }
	    for (u2 = c->halfops; u2; u2 = u2->next) {
		if (u2->user == u->user) {
		    ishalfop = 1;
		    break;
		}
	    }
	    end += snprintf(end, sizeof(buf)-(end-buf),
					" Modos en %s Ops:%s HalfOps:%s Voices:%s", isvoice ? "+" : "",
					ishalfop ? "%" : "", isop ? "@" : "", u->user->nick);
	}
	privmsg(s_OperServ, source, buf);
    }
}


/* Send list of users on a single channel, taken from strtok(). */

void send_channel_users(User *user)
{
    char *chan = strtok(NULL, " ");
    Channel *c = chan ? findchan(chan) : NULL;
    struct c_userlist *u;
#ifdef IRC_UNDERNET_P10
    const char *source = user->numerico;
#else    
    const char *source = user->nick;
#endif

    if (!c) {
	privmsg(s_OperServ, source, "Canal %s no encontrado!",
		chan ? chan : "(null)");
	return;
    }
    privmsg(s_OperServ, source, "Canal %s usuarios:", chan);
    for (u = c->users; u; u = u->next)
	privmsg(s_OperServ, source, "%s", u->user->nick);
    privmsg(s_OperServ, source, "Canal %s operadores:", chan);
    for (u = c->chanops; u; u = u->next)
	privmsg(s_OperServ, source, "%s", u->user->nick);
    privmsg(s_OperServ, source, "Canal %s semi-operadores:", chan);
    for (u = c->halfops; u; u = u->next)
	privmsg(s_OperServ, source, "%s", u->user->nick);
    privmsg(s_OperServ, source, "Canal %s moderadores:", chan);
    for (u = c->voices; u; u = u->next)
	privmsg(s_OperServ, source, "%s", u->user->nick);
}

#endif	/* DEBUG_COMMANDS */

/*************************************************************************/

/* Return the Channel structure corresponding to the named channel, or NULL
 * if the channel was not found.  chan is assumed to be non-NULL and valid
 * (i.e. pointing to a channel name of 2 or more characters). */

Channel *findchan(const char *chan)
{
    Channel *c;

    if (debug >= 3)
	log("debug: findchan(%p)", chan);
    c = chanlist[HASH(chan)];
    while (c) {
	if (stricmp(c->name, chan) == 0)
	    return c;
	c = c->next;
    }
    if (debug >= 3)
	log("debug: findchan(%s) -> %p", chan, c);
    return NULL;
}

/*************************************************************************/

/* Iterate over all channels in the channel list.  Return NULL at end of
 * list.
 */

static Channel *current;
static int next_index;

Channel *firstchan(void)
{
    next_index = 0;
    while (next_index < 1024 && current == NULL)
	current = chanlist[next_index++];
    if (debug >= 3)
	log("debug: firstchan() returning %s",
			current ? current->name : "NULL (fin de la lista)");
    return current;
}

Channel *nextchan(void)
{
    if (current)
	current = current->next;
    if (!current && next_index < 1024) {
	while (next_index < 1024 && current == NULL)
	    current = chanlist[next_index++];
    }
    if (debug >= 3)
	log("debug: nextchan() returning %s",
			current ? current->name : "NULL (fin de la lista)");
    return current;
}

/*************************************************************************/
/*************************************************************************/

/* Add/remove a user to/from a channel, creating or deleting the channel as
 * necessary.  If creating the channel, restore mode lock and topic as
 * necessary.  Also check for auto-opping and auto-voicing. */

void chan_adduser(User *user, const char *chan)
{
    Channel *c = findchan(chan);
    Channel **list;
    int newchan = !c;
    struct c_userlist *u;

    if (newchan) {
	if (debug)
	    log("debug: Creando canal %s", chan);
	/* Allocate pre-cleared memory */
	c = scalloc(sizeof(Channel), 1);
	strscpy(c->name, chan, sizeof(c->name));
	list = &chanlist[HASH(c->name)];
	c->next = *list;
	if (*list)
	    (*list)->prev = c;
	*list = c;
	c->creation_time = time(NULL);
	/* Store ChannelInfo pointer in channel record */
	c->ci = cs_findchan(chan);
	if (c->ci) {
#ifdef IRC_TERRA
	    /* This is a registered channel, ensure it's mode locked +r */
	    c->ci->mlock_on |= CMODE_r;
	    c->ci->mlock_off &= ~CMODE_r;	/* just to be safe */
#endif

	    /* Store return pointer in ChannelInfo record */
	    c->ci->c = c;
	}
	/* Restaura el modos de candado y topic en canales reg */
	check_modes(chan);
	restore_topic(chan);
    }
    if (check_should_op(user, chan)) {
	u = smalloc(sizeof(struct c_userlist));
	u->next = c->chanops;
	u->prev = NULL;
	if (c->chanops)
	    c->chanops->prev = u;
	c->chanops = u;
	u->user = user;
    } else if (check_should_halfop(user, chan)) {
	u = smalloc(sizeof(struct c_userlist));
	u->next = c->halfops;
	u->prev = NULL;
	if (c->halfops)
	    c->halfops->prev = u;
	c->halfops = u;
	u->user = user;
    } else if (check_should_voice(user, chan)) {
	u = smalloc(sizeof(struct c_userlist));
	u->next = c->voices;
	u->prev = NULL;
	if (c->voices)
	    c->voices->prev = u;
	c->voices = u;
	u->user = user;
    }
    u = smalloc(sizeof(struct c_userlist));
    u->next = c->users;
    u->prev = NULL;
    if (c->users)
	c->users->prev = u;
    c->users = u;
    u->user = user;
}


void chan_deluser(User *user, Channel *c)
{
    struct c_userlist *u;
    int i;

    for (u = c->users; u && u->user != user; u = u->next)
	;
    if (!u)
	return;
    if (u->next)
	u->next->prev = u->prev;
    if (u->prev)
	u->prev->next = u->next;
    else
	c->users = u->next;
    free(u);
    for (u = c->chanops; u && u->user != user; u = u->next)
	;
    if (u) {
	if (u->next)
	    u->next->prev = u->prev;
	if (u->prev)
	    u->prev->next = u->next;
	else
	    c->chanops = u->next;
	free(u);
    }
    for (u = c->halfops; u && u->user != user; u = u->next)
	;
    if (u) {
	if (u->next)
	    u->next->prev = u->prev;
	if (u->prev)
	    u->prev->next = u->next;
	else
	    c->halfops = u->next;
	free(u);
    }
    for (u = c->voices; u && u->user != user; u = u->next)
	;
    if (u) {
	if (u->next)
	    u->next->prev = u->prev;
	if (u->prev)
	    u->prev->next = u->next;
	else
	    c->voices = u->next;
	free(u);
    }
    if (!c->users) {
	if (debug)
	    log("debug: Borrando canal %s", c->name);
	if (c->ci)
	    c->ci->c = NULL;
	if (c->topic)
	    free(c->topic);
	if (c->key)
	    free(c->key);
	for (i = 0; i < c->bancount; ++i) {
	    if (c->bans[i])
		free(c->bans[i]);
	    else
		log("Canales: BUG freeing %s: bans[%d] es NULL!", c->name, i);
	}
	if (c->bansize)
	    free(c->bans);
	if (c->chanops || c->voices || c->halfops)
	    log("Canales: Memory leak freeing %s: %s%s%s %s no es NULL!",
			c->name,
			c->chanops ? "c->chanops" : "",
			c->chanops && c->voices ? " y " : "",
			c->voices ? "c->voices" : "",
			c->chanops && c->voices ? "son" : "es");
	if (c->next)
	    c->next->prev = c->prev;
	if (c->prev)
	    c->prev->next = c->next;
	else
	    chanlist[HASH(c->name)] = c->next;
	free(c);
    }
}

/*************************************************************************/

/* Handle a channel CREATE command. */
/*
 *  Source = Numerico
 *   av[0] = Canal
 *   av[1] = Tiempo creacion canal
 *
 * Ejemplo:
 *
 *   0AI CREATE #vigo 973551641
 */

#ifdef IRC_UNDERNET_P10
void do_create(const char *source, int ac, char **av)
{
   User *user;
   
   u = finduser(source);

   if (debug)
       log("debug: %s crea el canal %s", user->nick, av[0]);
          
   do_join(user->nick, ac, av);
   av[0] = sstrdup(av[0]);
   av[1] = sstrdup("+o");
   av[2] = sstrdup(user->numerico);
/* Al entrar el tio, le pongo op */
   do_cmode(user->nick, 3, av);
   free(av[0]);
   free(av[1]);
   free(av[2]);
                              
}

/*************************************************************************/

/* Handle a channel BURST command. */

/* Codigo copiado del ircu P10 de undernet
 *
 *      source = Numerico del servidor
 *      av[0]  = Canal
 *      av[1]  = Hora de creacion canal
 *      av[2]  = Modos canal
 *
 *  Si tiene modo +k y +l
 *      av[3]  = Key
 *      av[4]  = Limite usuarios
 *      av[5]  = usuarios y los modos
 *      av[6..]  = bans....
 *
 *  Si solo tiene modo +k o +l
 *      av[3]  = key o limite
 *      av[4]  = usuarios y los modos
 *      av[5..]  = bans....
 *
 *  Si no tiene modo +k y +l
 *      av[3]  = usuarios y los modos
 *      av[4..]  = bans....
 *
 * Ejemplo:
 *   "E BURST #zoltan 93422742 +ntkli lere 12 EMS,TEJ:o,FWE:ov,JET:v,EJS,JRT :%*!*@jet.es *!*@*.lnst.es"
 */
void do_burst(const char *source, int ac, char **av)
{                    
    char **modes = NULL;
    char *ax[3];
    int n = 0, first = 1, mod_num = 0;
    User *u = NULL;
                        
    /* Run over all remaining parameters */
    for (n = 2; n < ac; n++) {
        switch (av[n][0]) {        /* What type is it ¿mode, nicks or bans? */
            case '+':        /* modes */
            {
                modes = smalloc(sizeof(char*) * 2);
                modes[mod_num++] = sstrdup(av[0]);
                modes[mod_num++] = sstrdup(av[n]);
                if (strchr(modes[1], 'l')) {
                    realloc(modes, (sizeof(modes) +sizeof(char *)));
                    modes[mod_num++] = sstrdup(av[++n]);
                }
                if (strchr(modes[1], 'k')) {
                    realloc(modes, (sizeof(modes) +sizeof(char *)));
                    modes[mod_num++] = sstrdup(av[++n]);
                }                                                                                                    
                break;
            }
             
            case '%':        /* bans */
            {                
                char *pv, *p = NULL, *ban;
                /* Run over all bans */
                for (pv = av[n] +1; (ban = strtoken(&p, pv, " ")); pv = NULL)
                {                       
                    ax[0] = sstrdup(av[0]);
                    ax[1] = sstrdup("+b");
                    ax[2] = sstrdup(ban);
                    /* Como no sabemos quien ha baneado, ponemos que
                     * ha sido ChanServ :)
                     */                
                    do_cmode(s_ChanServ, 3, ax);
//                    free(ax);
                }
                break;                /* Done bans part */
            }                     

            default:                  /* nicks */
            {                        
                char *pv, *p = NULL, *nick, *ptr;
                /* Default mode: */
                char *default_mode = NULL;
                /* Run over all nicks */
                for (pv = av[n]; (nick = strtoken(&p, pv, ",")); pv = NULL)
                {            
                    if ((ptr = strchr(nick, ':')))        /* New default mode ? */
                    {
                        default_mode = NULL;
                        *ptr = '\0';        /* Fix 'nick' */
                        u = finduserP10(nick);

                        /*Calculate new mode change: */
                        while (*(++ptr)) {
                            if (*ptr == 'o')
                            {
                                if (default_mode)
                                    strcat(default_mode, "o");
                                else
                                    default_mode = sstrdup("o");
                            }
                            else if (*ptr == 'v')
                            {
                                if (default_mode)
                                    strcat(default_mode, "v");
                                else
                                    default_mode = sstrdup("v");                                                                                                                                        
                            }
                            else
                              break;                                                                                                                                       
                        }
                    }
                    else 
                        u = finduserP10(nick);                                                                                                                                                                                                                                                                                                           

                    if (u) {
                        if (debug)
                            log("Canales: Usuario %s entra al canal %s", u->nick, av[0]);
                        do_join(u->nick, 1, av);
                        if (first && mod_num) {
                            do_cmode(source, mod_num, modes);
//                            free(modes);
                        }                                                                                                            
                        if (first)
                            first = 0;
                              
                        if (default_mode) {                    
                            if (stricmp(default_mode, "ov") == 0) {
                                ax[0] = sstrdup(av[0]);
                                ax[1] = sstrdup("o");
                                ax[2] = sstrdup(u->numeric);
                                do_cmode(s_ChanServ, 3, ax);
                                ax[1] = sstrdup("v");
                                do_cmode(s_ChanServ, 3, ax);
                                free(ax);
                            } else {
                                ax[0] = sstrdup(av[0]);
                                ax[1] = sstrdup(default_mode);
                                ax[2] = sstrdup(u->numerico);                                                                                        
                                /* Como no sabemos quien ha dado los modos,
                                 * ponemos que ha sido ChanServ :)
                                 */                                
                                do_cmode(s_ChanServ, 3, ax);
                                free(ax);                                
                            }
                        }    
                    } else
                        log("channel: No se encuentra user %s en canal %s", nick, av[0]);
                }
            }                         /* <-- Next parameter if any */    
        }    /* Swith de la linea de burst */
    }  /* Fin del for */

}
#endif /* IRC_UNDERNET_P10 */
                        
/*************************************************************************/

/* Handle a channel MODE command. */

void do_cmode(const char *source, int ac, char **av)
{
    Channel *chan;
    struct c_userlist *u;
    User *user;
    char *s, *nick;
    int add = 1;		/* 1 if adding modes, 0 if deleting */
    char *modestr = av[1];

    chan = findchan(av[0]);
    if (!chan) {
	log("Canales: MODE %s para canal %s no existente",
					merge_args(ac-1, av+1), av[0]);
	return;
    }

    /* This shouldn't trigger on +o, etc. */
    if (strchr(source, '.') && !modestr[strcspn(modestr, "bov")]) {
	if (time(NULL) != chan->server_modetime) {
	    chan->server_modecount = 0;
	    chan->server_modetime = time(NULL);
	}
	chan->server_modecount++;
    }

    s = modestr;
    ac -= 2;
    av += 2;

    while (*s) {

	switch (*s++) {

	case '+':
	    add = 1; break;

	case '-':
	    add = 0; break;

	case 'i':
	    if (add)
		chan->mode |= CMODE_I;
	    else
		chan->mode &= ~CMODE_I;
	    break;

	case 'm':
	    if (add)
		chan->mode |= CMODE_M;
	    else
		chan->mode &= ~CMODE_M;
	    break;

	case 'n':
	    if (add)
		chan->mode |= CMODE_N;
	    else
		chan->mode &= ~CMODE_N;
	    break;

	case 'p':
	    if (add)
		chan->mode |= CMODE_P;
	    else
		chan->mode &= ~CMODE_P;
	    break;

	case 's':
	    if (add)
		chan->mode |= CMODE_s;
	    else
		chan->mode &= ~CMODE_s;
	    break;

	case 't':
	    if (add)
		chan->mode |= CMODE_T;
	    else
		chan->mode &= ~CMODE_T;
	    break;

        case 'c':
            if (add)
                chan->mode |= CMODE_c;
            else
                chan->mode &= ~CMODE_c;
            break;

        case 'C':
            if (add)
                chan->mode |= CMODE_C;
            else
                chan->mode &= ~CMODE_C;
            break;

        case 'O':
            if (add)
                chan->mode |= CMODE_O;
            else
                chan->mode &= ~CMODE_O;
            break;

#if defined (IRC_HISPANO) || defined (IRC_TERRA)
/* Soporte para redes con BDD como Hispano, Globalchat o Upworld */
	case 'R':
	    if (add)
		chan->mode |= CMODE_R;
	    else
		chan->mode &= ~CMODE_R;
	    break;
	
	case 'M':
	    if (add)
		chan->mode |= CMODE_m;
	    else
		chan->mode &= ~CMODE_m;
	    break;

	case 'r':
	    if (add)
		chan->mode |= CMODE_r;
	    else
		chan->mode &= ~CMODE_r;
	    break;
#endif
#ifdef IRC_HISPANO
/* Soporte para redes con BDD como Hispano, Globalchat o Upworld */

        case 'A':
            if (add)
                chan->mode |= CMODE_A;
            else
                chan->mode &= ~CMODE_A;
            break;

        case 'S':
            if (add)
                chan->mode |= CMODE_S;
            else
                chan->mode &= ~CMODE_S;
            break;
#endif
	case 'k':
	    if (--ac < 0) {
		log("Canales: MODE %s %s: falta parametro para %ck",
					chan->name, modestr, add ? '+' : '-');
		break;
	    }
	    if (chan->key) {
		free(chan->key);
		chan->key = NULL;
	    }
	    if (add)
		chan->key = sstrdup(*av++);
	    break;

	case 'l':
	    if (add) {
		if (--ac < 0) {
		    log("Canales: MODE %s %s: falta parametro para +l",
							chan->name, modestr);
		    break;
		}
		chan->limit = atoi(*av++);
	    } else {
		chan->limit = 0;
	    }
	    break;

	case 'b':
	    if (--ac < 0) {
		log("Canales: MODE %s %s: falta parametro para %cb",
					chan->name, modestr, add ? '+' : '-');
		break;
	    }
	    if (add) {
		if (chan->bancount >= chan->bansize) {
		    chan->bansize += 8;
		    chan->bans = srealloc(chan->bans,
					sizeof(char *) * chan->bansize);
		}
		chan->bans[chan->bancount++] = sstrdup(*av++);
	    } else {
		char **s = chan->bans;
		int i = 0;
		while (i < chan->bancount && strcmp(*s, *av) != 0) {
		    i++;
		    s++;
		}
		if (i < chan->bancount) {
		    chan->bancount--;
		    if (i < chan->bancount)
			memmove(s, s+1, sizeof(char *) * (chan->bancount-i));
		}
		av++;
	    }
	    break;

	case 'o':
	    if (--ac < 0) {
		log("Canales: MODE %s %s: falta parametro para %co",
					chan->name, modestr, add ? '+' : '-');
		break;
	    }
	    nick = *av++;
	    if (add) {
		for (u = chan->chanops; u && stricmp(u->user->nick, nick) != 0;
								u = u->next)
		    ;
		if (u)
		    break;
#ifdef IRC_UNDERNET_P10
                user = finduserP10(sstrdup(nick));
#else
		user = finduser(nick);
#endif 		
		if (!user) {
		    log("Canales: MODE %s +o para usuario inexistente %s",
							chan->name, nick);
		    break;
		}
		if (debug)
		    log("debug: Setting +o on %s for %s", chan->name, user->nick);
		if (!check_valid_op(user, chan->name, !!strchr(source, '.')))
		    break;
		u = smalloc(sizeof(*u));
		u->next = chan->chanops;
		u->prev = NULL;
		if (chan->chanops)
		    chan->chanops->prev = u;
		chan->chanops = u;
		u->user = user;
	    } else {
		for (u = chan->chanops; u && stricmp(u->user->nick, nick);
								u = u->next)
		    ;
                /* Poner aqui leaveops :??? */
		if (!u)
		    break;
		if (u->next)
		    u->next->prev = u->prev;
		if (u->prev)
		    u->prev->next = u->next;
		else
		    chan->chanops = u->next;
		free(u);
	    }
	    break;

	case 'h':
	    if (--ac < 0) {
		log("Canales: MODE %s %s: falta parametro para %cv",
					chan->name, modestr, add ? '+' : '-');
		break;
	    }
	    nick = *av++;
	    if (add) {
		for (u = chan->halfops; u && stricmp(u->user->nick, nick) != 0;
								u = u->next)
		    ;
		if (u)
		    break;
#ifdef IRC_UNDERNET_P10
                user = finduserP10(nick);
#else                
		user = finduser(nick);
#endif		
		if (!user) {
		    log("channe: MODE %s +h para usuario inexistente %s",
							chan->name, nick);
		    break;
		}
		if (debug)
		    log("debug: Setting +h on %s for %s", chan->name, user->nick);
/* Aun no implementado Securehalfops y Autodeop
                if (!check_valid_halfop(user, chan->name, !!strchr(source, '.')))
                    break;
 */                    
		u = smalloc(sizeof(*u));
		u->next = chan->halfops;
		u->prev = NULL;
		if (chan->halfops)
		    chan->halfops->prev = u;
		chan->halfops = u;
		u->user = user;
	    } else {
		for (u = chan->halfops; u && stricmp(u->user->nick, nick);
								u = u->next)
		    ;
		if (!u)
		    break;
		if (u->next)
		    u->next->prev = u->prev;
		if (u->prev)
		    u->prev->next = u->next;
		else
		    chan->halfops = u->next;
		free(u);
	    }
	    break;


	case 'v':
	    if (--ac < 0) {
		log("Canales: MODE %s %s: falta parametro para %cv",
					chan->name, modestr, add ? '+' : '-');
		break;
	    }
	    nick = *av++;
	    if (add) {
		for (u = chan->voices; u && stricmp(u->user->nick, nick) != 0;
								u = u->next)
		    ;
		if (u)
		    break;
#ifdef IRC_UNDERNET_P10
                user = finduserP10(nick);
#else                
		user = finduser(nick);
#endif		
		if (!user) {
		    log("channe: MODE %s +v para usuario inexistente %s",
							chan->name, nick);
		    break;
		}
		if (debug)
		    log("debug: Setting +v on %s for %s", chan->name, user->nick);
/* Aun no implementado Securevoices y Autodeop
                if (!check_valid_voice(user, chan->name, !!strchr(source, '.')))
                    break;
 */                    
		u = smalloc(sizeof(*u));
		u->next = chan->voices;
		u->prev = NULL;
		if (chan->voices)
		    chan->voices->prev = u;
		chan->voices = u;
		u->user = user;
	    } else {
		for (u = chan->voices; u && stricmp(u->user->nick, nick);
								u = u->next)
		    ;
		if (!u)
		    break;
		if (u->next)
		    u->next->prev = u->prev;
		if (u->prev)
		    u->prev->next = u->next;
		else
		    chan->voices = u->next;
		free(u);
	    }
	    break;

	} /* switch */

    } /* while (*s) */

    /* Check modes against ChanServ mode lock */
    check_modes(chan->name);
}

/*************************************************************************/

/* Handle a TOPIC command. */

/* En Undernet P9 y P10 el comando topic es asi
 *
 *      source = Nick de kien pone el topic
 *      av[0]  = Canal
 *      av[1]  = Topic
 *
 * En el resto de ircds
 *
 *      source = ??
 *      av[0]  = Canal
 *      av[1]  = Nick de kien pone el topic
 *      av[2]  = Tiempo ke fue cambiado el topic
 *      av[3]  = Topic
 * 
 * Por este motivo he hecho cambios en do_topic :)
 *
 * Toni García  -Zoltan-  1- Septiembre- 2000
 */

void do_topic(const char *source, int ac, char **av)
{
    Channel *c = findchan(av[0]);

    if (!c) {
	log("Canales: TOPIC %s para canal %s inexistente",
						merge_args(ac-1, av+1), av[0]);
	return;
    }
    if (check_topiclock(av[0]))
	return;
    strscpy(c->topic_setter, source, sizeof(c->topic_setter));
    c->topic_time = time(NULL);    
    if (c->topic) {
	free(c->topic);
	c->topic = NULL;
    }
    if (ac > 1 && *av[1])
        c->topic = sstrdup(av[1]);
    record_topic(av[0]);
}

/*************************************************************************/

/* Does the given channel have only one user? */

/* Note:  This routine is not currently used, but is kept around in case it
 * might be handy someday. */

#if 0
int only_one_user(const char *chan)
{
    Channel *c = findchan(chan);
    return (c && c->users && !c->users->next) ? 1 : 0;
}
#endif

/*************************************************************************/
