/* CregServ functions.
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

static CregInfo *creglists[256]; 

static void do_help(User *u);
static void do_registra(User *u);
static void do_acepta(User *u);
static void do_drop(User *u);
static void do_deniega(User *u);
static void do_cancela(User *u);
static void do_apoya(User *u);
static void do_estado(User *u);
static void do_apoyos(User *u);
static void do_fuerza(User *u);
static void do_list(User *u);
static void do_info(User *u);


static Command cmds[] = {
    { "HELP",       do_help,     NULL,  -1,                 -1,-1,-1,-1 },
    { "AYUDA",      do_help,     NULL,  -1,                 -1,-1,-1,-1 },
    { "REGISTRA",   do_registra, NULL,  CREG_HELP_REGISTRA, -1,-1,-1,-1 },
    { "REGISTER",   do_registra, NULL,  CREG_HELP_REGISTRA, -1,-1,-1,-1 },
    { "CANCELA",    do_cancela,  NULL,  CREG_HELP_CANCELA,  -1,-1,-1,-1 },
    { "APOYA",      do_apoya,    NULL,  CREG_HELP_APOYA,    -1,-1,-1,-1 },
    { "ESTADO",     do_estado,   NULL,  CREG_HELP_ESTADO,   -1,-1,-1,-1 },
    { "APOYOS",     do_apoyos,   NULL,  CREG_HELP_APOYOS,   -1,-1,-1,-1 },        

    { "INFO",     do_info,    is_services_preoper,   -1,-1,-1,-1,-1 },        
    { "LISTA",    do_list,    is_services_preoper,   -1,-1,-1,-1,-1 },
    { "LIST",     do_list,    is_services_preoper,   -1,-1,-1,-1,-1 },
    { "ACEPTA",   do_acepta,  is_services_cregadmin, -1,-1,-1,-1,-1 },
    { "DENIEGA",  do_deniega, is_services_cregadmin, -1,-1,-1,-1,-1 },
    { "DROP",     do_drop,    is_services_cregadmin, -1,-1,-1,-1,-1 },
    { "FUERZA",   do_fuerza,  is_services_cregadmin,      -1,-1,-1,-1,-1 },
    { NULL }
};

static void alpha_insert_creg(CregInfo *cr);
static int delcreg(CregInfo *cr);

/***********************************************************************/
/*                  RUTINAS INTERNAS DEL BOT CREG                      */ 
/***********************************************************************/

/* Devuelve la información de la memoria utilizada. */

void get_cregserv_stats(long *nrec, long *memuse)
{
    long count = 0, mem = 0;
    int i;
    CregInfo *cr;
                        
    for (i = 0; i < 256; i++) {
        for (cr = creglists[i]; cr; cr = cr->next) {
            count++;
            mem += sizeof(*cr);
            if (cr->nickoper)
                mem += strlen(cr->nickoper)+1;
            if (cr->motivo)
                mem += strlen(cr->motivo)+1;
            mem += cr->apoyoscount * sizeof(ApoyosCreg);
        }
    }
    *nrec = count;
    *memuse = mem;
}            
/*************************************************************************/
/* Inicialización del bot creg. */

void cr_init(void)
{
    Command *cmd;
    
    cmd = lookup_cmd(cmds, "APOYA");
    if (cmd)
         cmd->help_param1 = s_CregServ;
}                                                                                                                                            

/****************CUANDO EXPIRAN LOS CANALES*************************/
void expire_creg()
{
  CregInfo *cr, *next;
  int i;
  time_t now = time(NULL);

    for (i = 0; i < 256; i++) {
	for (cr = creglists[i]; cr; cr = next) {
	  next = cr->next;
	  if (now - cr->time_peticion >= 604800 && (cr->estado & CR_PROCESO_REG)) {
	   canalopers(s_CregServ, "Expirado el proceso de registro de %s", cr->name);
	   delcreg(cr);
	  }
          if (now - cr->time_motivo >= 172800 && !(cr->estado & (CR_PROCESO_REG | CR_REGISTRADO | CR_ACEPTADO))) {
	   canalopers(s_CregServ, "Expirado el canal no registrado %s", cr->name);
	   delcreg(cr);
	  }
	}
     }
}

/*************************************************************************/

/* Load/save data files. */


#define SAFE(x) do {                                    \
    if ((x) < 0) {                                      \
        if (!forceload)                                 \
            fatal("Error de Lectura en %s", CregDBName);      \
        failed = 1;                                     \
        break;                                          \
    }                                                   \
} while (0)
 
                                            
void load_cr_dbase(void)
{
    dbFILE *f;
    int ver, i, j, c;
    CregInfo *cr, **last, *prev;
    int failed = 0;
                
    if (!(f = open_db(s_CregServ, CregDBName, "r")))
        return;
                            
   switch (ver = get_file_version(f)) {
      case 9:
      case 8:
        for (i = 0; i < 256 && !failed; i++) {
            int32 tmp32;
            last = &creglists[i];
            prev = NULL;
            while ((c = getc_db(f)) != 0) {
                if (c != 1)
                    fatal("Invalid format in %s", CregDBName); 
                cr = smalloc(sizeof(CregInfo));
                *last = cr;
                last = &cr->next;
                cr->prev = prev;
                prev = cr;
                SAFE(read_buffer(cr->name, f));
                SAFE(read_string(&cr->founder, f));

                SAFE(read_buffer(cr->founderpass, f));
                SAFE(read_string(&cr->desc, f));
                SAFE(read_string(&cr->email, f));
                                                                                                                                                                                    
                SAFE(read_int32(&tmp32, f));
                cr->time_peticion = tmp32;                

                SAFE(read_string(&cr->nickoper, f));
                SAFE(read_string(&cr->motivo, f));                    
                SAFE(read_int32(&tmp32, f));
                cr->time_motivo = tmp32;                                                                                                

                SAFE(read_int32(&cr->estado, f));                                                
                SAFE(read_int16(&cr->apoyoscount, f));
                if (cr->apoyoscount) {
                    cr->apoyos = scalloc(cr->apoyoscount, sizeof(ApoyosCreg));
                    for (j = 0; j < cr->apoyoscount; j++) {                        
                        SAFE(read_string(&cr->apoyos[j].nickapoyo, f));
                        SAFE(read_string(&cr->apoyos[j].emailapoyo, f));                               
                        SAFE(read_int32(&tmp32, f));
                        cr->apoyos[j].time_apoyo = tmp32;                                                              
                    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
                } else {
                    cr->apoyos = NULL;
                }                
            }  /* while (getc_db(f) != 0) */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
             *last = NULL;

        }  /* for (i) */ 
        break;                                                                                                   
     default:
        fatal("Unsupported version number (%d) on %s", ver, CregDBName);

    }  /* switch (version) */
    
    close_db(f);
}

#undef SAFE

/*************************************************************************/

#define SAFE(x) do {                                            \
    if ((x) < 0) {                                              \
        restore_db(f);                                          \
        log_perror("Write error on %s", CregDBName);            \
        if (time(NULL) - lastwarn > WarningTimeout) {           \
            canalopers(NULL, "Write error on %s: %s", CregDBName,  \
                           strerror(errno));                       \
            lastwarn = time(NULL);                              \
        }                                                       \
        return;                                                 \
    }                                                           \
} while (0)
                                                                                                                                            
void save_cr_dbase(void)
{
    dbFILE *f;
    int i, j;
    CregInfo *cr;
    static time_t lastwarn = 0;

    if (!(f = open_db(s_CregServ, CregDBName, "w")))
        return;
           
    for (i = 0; i < 256; i++) {
                                            
        for (cr = creglists[i]; cr; cr = cr->next) {
            SAFE(write_int8(1, f));
            SAFE(write_buffer(cr->name, f));
            SAFE(write_string(cr->founder, f));
                                
            SAFE(write_buffer(cr->founderpass, f));
            SAFE(write_string(cr->desc, f));
            SAFE(write_string(cr->email, f));
                                                                                                    
            SAFE(write_int32(cr->time_peticion, f));
            SAFE(write_string(cr->nickoper, f));
            SAFE(write_string(cr->motivo, f));
            SAFE(write_int32(cr->time_motivo, f));

            SAFE(write_int32(cr->estado, f));                                                                                                                
            SAFE(write_int16(cr->apoyoscount, f));
            for (j = 0; j < cr->apoyoscount; j++) {            
                SAFE(write_string(cr->apoyos[j].nickapoyo, f));
                SAFE(write_string(cr->apoyos[j].emailapoyo, f));
                SAFE(write_int32(cr->apoyos[j].time_apoyo, f));
            }                     
        } /* for (creglists[i]) */
        
        SAFE(write_int8(0, f));
                
    } /* for (i) */
                    
    close_db(f);
}

#undef SAFE

/*************************************************************************/


/*************************************************************************/


void cregserv(const char *source, char *buf)
{
    char *cmd, *s;
    User *u = finduser(source);
        
    if (!u) {
        log("%s: user record for %s not found", s_CregServ, source);
        notice(s_CregServ, source,
            getstring((NickInfo *)NULL, USER_RECORD_NOT_FOUND));
        return;
    }
    
    /*if (!is_moder(u->nick))
        return;*/

    cmd = strtok(buf, " ");
    if (!cmd) {
        return;
    } else if (stricmp(cmd, "\1PING") == 0) {
        if (!(s = strtok(NULL, "")))
            s = "\1";
        notice(s_CregServ, source, "\1PING %s", s);
    } else if (skeleton) {
        notice_lang(s_CregServ, u, SERVICE_OFFLINE, s_CregServ);
    } else {
        if (!u->ni && stricmp(cmd, "HELP") != 0)
            notice_lang(s_CregServ, u, NICK_NOT_REGISTERED_HELP, s_NickServ);
        else
            run_cmd(s_CregServ, u, cmds, cmd);
    }
}

/*************************************************************************/

/* Return the ChannelInfo structure for the given channel, or NULL if the
 * channel isn't registered. */

CregInfo *cr_findcreg(const char *chan)
{
     CregInfo *cr;
     
     for (cr = creglists[tolower(chan[1])]; cr; cr = cr->next) {
         if (stricmp(cr->name, chan) == 0)
             return cr;
     }
     return NULL;
}

/*************************************************************************/

/*************************************************************************/
/*********************** CregServ command routines ***********************/
/*************************************************************************/
/* Insert a channel alphabetically into the database. */

static void alpha_insert_creg(CregInfo *cr)
{
    CregInfo *ptr, *prev;
    char *chan = cr->name;
        
    for (prev = NULL, ptr = creglists[tolower(chan[1])];
                        ptr != NULL && stricmp(ptr->name, chan) < 0;
                        prev = ptr, ptr = ptr->next);
    cr->prev = prev;
    cr->next = ptr;
    if (!prev)
        creglists[tolower(chan[1])] = cr;
    else
        prev->next = cr;
    if (ptr)
        ptr->prev = cr;
}

/*************************************************************************/

/* Add a channel to the database.  Returns a pointer to the new ChannelInfo
 * structure if the channel was successfully registered, NULL otherwise.
  * Assumes channel does not already exist. */
  
static CregInfo *makecreg(const char *chan)
{
    CregInfo *cr;
    
    cr = scalloc(sizeof(CregInfo), 1);
    /*strscpy(cr->name, chan, CHANMAX);*/
    strscpy(cr->name, chan, CHANMAX);
    alpha_insert_creg(cr);
    return cr;
}

/*************************************************************************/

/* Remove a channel from the ChanServ database.  Return 1 on success, 0
 * otherwise. */
                                                                                                                                                                         
static int delcreg(CregInfo *cr)
{
  
    if (cr->next)
	cr->next->prev = cr->prev;
    if (cr->prev)
	cr->prev->next = cr->next;
    else
	creglists[tolower(cr->name[1])] = cr->next;
    /*if (cr->founder)
	free(cr->founder);*/
    if (cr->desc)
	free(cr->desc);
    if (cr->email)
	free(cr->email);
    /*if (cr->nickoper)
	free(cr->nickoper);*/
    if (cr->motivo)
	free(cr->motivo);
    if (cr->apoyos)
	free(cr->apoyos);
    cr = NULL; 
 
    free(cr);
    
    return 1;
    
}            
/*************************************************************************/

/* Return a help message. */
static void do_help(User *u)
{
    char *cmd = strtok(NULL, "");
    
    if (!cmd) {
        notice_help(s_CregServ, u, CREG_HELP, CregApoyos);
        
    if (is_services_preoper(u))
        notice_help(s_CregServ, u, CREG_SERVADMIN_HELP);
    } else {
        help_cmd(s_CregServ, u, cmds, cmd);
    }
}

/*************************************************************************/

static void do_registra(User *u)
{
    char *chan = strtok(NULL, " ");
    char *pass = strtok(NULL, " ");
    char *desc = strtok(NULL, "");

    NickInfo *ni = u->ni;
    CregInfo *cr, *cr2;
    ChannelInfo *ci;

    char passtemp[255];

                        
    if (!desc) {
        privmsg(s_CregServ, u->nick, "Sintaxis: 12REGISTRA <canal> <clave> <descripción>");
    } else if ((*chan == '&') || (*chan == '+')) {
        notice_lang(s_ChanServ, u, CHAN_REGISTER_NOT_LOCAL);
    } else if (!(*chan == '#')) {
        privmsg(s_CregServ, u->nick, "Canal no valido para registrar");
    } else if (!ni) {
        notice_lang(s_CregServ, u, CHAN_MUST_REGISTER_NICK, s_NickServ);
    } else if (!nick_identified(u)) {
        notice_lang(s_CregServ, u, CHAN_MUST_IDENTIFY_NICK, s_NickServ, s_NickServ);                                                    
    } else if ((ci = cs_findchan(chan)) != NULL) {
        if (ci->flags & CI_VERBOTEN) {
            notice_lang(s_CregServ, u, CHAN_MAY_NOT_BE_REGISTERED, chan);
        } else {
            notice_lang(s_CregServ, u, CHAN_ALREADY_REGISTERED, chan);
        }
    } else if ((cr2 = cr_findcreg(chan))) {
        privmsg(s_CregServ, u->nick, "El canal ya existe en %s. Teclee \00312/msg %s ESTADO %s\003 para ver su estado.", s_CregServ, s_CregServ, chan);     
    } else if (ni->channelmax > 0 && ni->channelcount >= ni->channelmax) {
        notice_lang(s_CregServ, u, ni->channelcount > ni->channelmax ? CHAN_EXCEEDED_CHANNEL_LIMIT : CHAN_REACHED_CHANNEL_LIMIT, ni->channelmax);

    } else if (!(cr = makecreg(chan))) {
        log("%s: makecreg() failed for REGISTER %s", s_CregServ, chan);
        notice_lang(s_CregServ, u, CHAN_REGISTRATION_FAILED);                             
    } else {
        cr->founder =  ni->nick; 
        if (strlen(pass) > PASSMAX-1) /* -1 for null byte */
            notice_lang(s_CregServ, u, PASSWORD_TRUNCATED, PASSMAX-1);
        strscpy(cr->founderpass, pass, PASSMAX);
                                    
        cr->time_lastapoyo = time(NULL);
        cr->email = sstrdup(ni->email);
        cr->desc = sstrdup(desc);
        cr->time_peticion = time(NULL);

        srand(time(NULL));
        sprintf(passtemp, "%05u",1+(int)(rand()%99999) );
        strscpy(cr->passapoyo, passtemp, PASSMAX);

       	privmsg(s_CregServ, u->nick, "El canal 12%s ha sido aceptado para el registro.", chan); 
        if (CregApoyos) {
	        cr->estado = CR_PROCESO_REG;
        	privmsg(s_CregServ, u->nick,"En los proximos 7 días necesitara %i apoyos para que sea registrado.", CregApoyos);
	} else {
                cr->estado = CR_ACEPTADO;
        	privmsg(s_CregServ, u->nick,"En breve la comisión de canales lo evaluará y registrará o denegará.");
	}
       	canalopers(s_CregServ, "12%s ha solicitado el registro del canal 12%s", u->nick, chan);
    } 
}                                                                        
                                                        

/*************************************************************************/

static void do_cancela(User *u)
{
    char *chan = strtok(NULL, " ");
    CregInfo *cr;
    NickInfo *ni = u->ni;
    
         
    if (!chan) {
       privmsg(s_CregServ, u->nick, "Sintaxis: 12CANCELA <canal>");
    } else if (!ni) {
         notice_lang(s_CregServ, u, CHAN_MUST_REGISTER_NICK, s_NickServ);
    } else if (!nick_identified(u)) {
         notice_lang(s_CregServ, u, CHAN_MUST_IDENTIFY_NICK, s_NickServ, s_NickServ);
    } else {
       if (!(cr = cr_findcreg(chan))) {
           privmsg(s_CregServ, u->nick, "El canal 12%s no existe", chan);  
       } else {
           if (!(cr->estado & CR_PROCESO_REG)) {
               privmsg(s_CregServ, u->nick, "El canal 12%s no puede ser eliminado. Solicitelo a un Administrador u Operador", chan);
               return;
           }
           if (stricmp(u->nick, cr->founder) == 0) {
               delcreg(cr);                                      
               privmsg(s_CregServ, u->nick, "El canal 12%s ha sido cancelado", chan);
               canalopers(s_CregServ, "12%s ha cancelado la peticion de registro para 12%s", u->nick, chan);
           } else {
               privmsg(s_CregServ, u->nick, "4Acceso denegado!!");
	   }
       } 
    }
}

                                                        
/*************************************************************************/
static void do_apoya(User *u)
{
    char *chan = strtok(NULL, " ");
    char *pass = strtok(NULL, " "); 
    NickInfo *ni = u->ni;
    CregInfo *cr; 
    ApoyosCreg *apoyos; 
    int i; 
        
    char passtemp[255];            
    
    if (!chan) { 
       privmsg(s_CregServ, u->nick, "Sintaxis: 12APOYA <canal>");
    } else if (!ni) {
        notice_lang(s_CregServ, u, CHAN_MUST_REGISTER_NICK, s_NickServ);
    } else if (!nick_identified(u)) {
        notice_lang(s_CregServ, u, CHAN_MUST_IDENTIFY_NICK, s_NickServ, s_NickServ);
    } else if (!(cr = cr_findcreg(chan))) {
        privmsg(s_CregServ, u->nick, "El canal 12%s no esta en proceso de registro", chan);
    } else if (!(cr->estado & CR_PROCESO_REG)) {                                           
        privmsg(s_CregServ, u->nick, "El canal 12%s no esta pendiente de apoyos", chan); 
    } else {
        /* Controles de apoyos por nick/e-mail */
        if (stricmp(u->nick, cr->founder) == 0) {
            privmsg(s_CregServ, u->nick, "Ya eres el fundador del canal");
            return;
        }
        for (apoyos = cr->apoyos, i = 0; i < cr->apoyoscount; apoyos++, i++) {
            if (stricmp(u->nick, apoyos->nickapoyo) == 0) {
                privmsg(s_CregServ, u->nick, "Ya has apoyado este canal una vez.");
                return;
            }
        }
        for (apoyos = cr->apoyos, i = 0; i < cr->apoyoscount; apoyos++, i++) {
            if (stricmp(ni->email, apoyos->emailapoyo) == 0) {
                privmsg(s_CregServ, u->nick, "Ya ha apoyado alguien este canal con su cuenta de correo.");
                return;
            } if (stricmp(ni->email, cr->email) == 0) {
                privmsg(s_CregServ, u->nick, "Ya ha apoyado alguien este canal con su cuenta de correo.");
                return;
            }
        }
        
        if (!pass || !u->creg_apoyo) {
             srand(time(NULL));
             sprintf(passtemp, "%05u",1+(int)(rand()%99999) );
             strscpy(u->creg_apoyo, passtemp, PASSMAX);
	     privmsg(s_CregServ, u->nick, "No apoye el canal solamente porque le hayan dicho que");
	     privmsg(s_CregServ, u->nick, "escriba 12/msg %s APOYA %s. Infórmese sobre la temática", s_CregServ, chan); 
	     privmsg(s_CregServ, u->nick, "del canal, sus usuarios, etc.");
	     privmsg(s_CregServ, u->nick, "Si decide finalmente apoyar al canal 12%s hágalo escribiendo:", chan);
	     privmsg(s_CregServ, u->nick, "12/msg %s APOYA %s %s", s_CregServ, chan, u->creg_apoyo); 
             return;
        } else if (stricmp(pass, u->creg_apoyo) != 0) {
            privmsg(s_CregServ, u->nick, "4Clave inválida");
            privmsg(s_CregServ, u->nick, "Para confirmar tu apoyo al canal 12%s hágalo escribiendo:", chan);
            privmsg(s_CregServ, u->nick, "12/msg %s APOYA %s %s", s_CregServ, chan, u->creg_apoyo);
            return;
        }

        cr->apoyoscount++;
        cr->apoyos = srealloc(cr->apoyos, sizeof(ApoyosCreg) * cr->apoyoscount);
        apoyos = &cr->apoyos[cr->apoyoscount-1];
        apoyos->nickapoyo = sstrdup(u->nick);
        apoyos->emailapoyo = sstrdup(ni->email);
        apoyos->time_apoyo = time(NULL); 
        cr->time_lastapoyo = time(NULL);
        privmsg(s_CregServ, u->nick, "Anotado tu apoyo al canal 12%s", chan);             

        if ((cr->apoyoscount) >= CregApoyos) {
            cr->time_motivo = time(NULL);
            cr->estado = 0;
            cr->estado |= CR_ACEPTADO;
            canalopers(s_CregServ, "12%s ya tiene todos los apoyos", chan);
        }
    } 
}                                                                                                                          
                                                        
/*************************************************************************/

static void do_estado(User *u)
{
    char *chan = strtok(NULL, " ");
    CregInfo *cr;
    ChannelInfo *cr2;

    if (!chan) {
       privmsg(s_CregServ, u->nick, "Sintaxis: 12ESTADO <canal>");
       return;
    }                  
    if (!(cr = cr_findcreg(chan))) {
        if (!(cr2 = cs_findchan(chan)))
          privmsg(s_CregServ, u->nick, "El canal 12%s no esta registrado", chan);
        else
         if (cr2->flags & CI_VERBOTEN)
          privmsg(s_CregServ, u->nick, "El canal 12%s no puede ser registrado ni utilizado.", chan);
         else if (cr2->flags & CI_SUSPEND)
          privmsg(s_CregServ, u->nick, "El canal 12%s esta 12SUSPENDIDO temporalmente", chan);
         else
          privmsg(s_CregServ, u->nick, "El canal 12%s existe en 12%s pero no esta registrado con 12%s", chan, s_ChanServ, s_CregServ);
    } else if (cr->estado & CR_REGISTRADO) {
        if ((cr2 = cs_findchan(chan))) {
             if (cr2->flags & CI_SUSPEND)
          	privmsg(s_CregServ, u->nick, "El canal 12%s esta 12SUSPENDIDO temporalmente", chan);
             else        
          	privmsg(s_CregServ, u->nick, "El canal 12%s esta 12REGISTRADO", chan);
        } else {
             privmsg(s_CregServ, u->nick, "El canal 12%s esta actualmente en estado 12DESCONOCIDO", chan);
	}
    } else if (cr->estado & CR_PROCESO_REG) {
        privmsg(s_CregServ, u->nick, "El canal 12%s esta pendiente de 12APOYOS", chan);
    } else if (cr->estado & CR_ACEPTADO) {
        privmsg(s_CregServ, u->nick, "El canal 12%s esta pendiente de 12APROBACION por parte de la administración de la red", chan);
    } else if (cr->estado & CR_DROPADO) {
        privmsg(s_CregServ, u->nick, "El canal 12%s esta dropado de %s", chan, s_ChanServ);
    } else if (cr->estado & CR_RECHAZADO) {
        privmsg(s_CregServ, u->nick, "El canal 12%s ha sido 12DENEGADO", chan);
        if (cr->motivo)        
             privmsg(s_CregServ, u->nick, "Motivo: 12%s", cr->motivo);
    } else if (cr->estado & CR_EXPIRADO) {
        privmsg(s_CregServ, u->nick, "El canal 12%s es un canal 12EXPIRADO en %s", chan, s_ChanServ);
    } else {
        privmsg(s_CregServ, u->nick, "El canal 12%s esta actualmente en estado 12DESCONOCIDO", chan);
    }
    
}
                
/*************************************************************************
  Gestion para la visualización de apoyos de nicks y canales
    
               Implementado por ZipBreake :)
************************************************************************/
static void do_apoyos(User *u)
{
    char *chan = strtok(NULL, " ");
    CregInfo *cr; 
    ApoyosCreg *apoyos; 
    int i, f;
    struct tm *tm;
    char buf[BUFSIZE];
    int numero = 0; 
        
    if (!chan) { 
       privmsg(s_CregServ, u->nick, "Sintaxis: 12APOYOS <canal> | <nick>");
       return;
    } else if (!nick_identified(u)) {
        notice_lang(s_CregServ, u, CHAN_MUST_IDENTIFY_NICK, s_NickServ, s_NickServ);
        return;
    }
    
    if (*chan == '#') {

    if (!(cr = cr_findcreg(chan))) {
        privmsg(s_CregServ, u->nick, "El canal 12%s no esta registrado ni en proceso de registro por CReG",chan);
    } else {

                                  
        if ((stricmp(u->nick, cr->founder) == 0) || is_services_preoper(u)) {
	    privmsg(s_CregServ, u->nick, "Lista de apoyos de 12%s", chan);
	    for (apoyos = cr->apoyos, i = 0; i < cr->apoyoscount; apoyos++, i++) {
	          tm = localtime(&apoyos->time_apoyo);
	          strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
	          privmsg(s_CregServ, u->nick, "- %s (%s)", apoyos->nickapoyo, buf);
	          numero++;
	    }
	    privmsg(s_CregServ, u->nick, "Actualmente 12%d apoyos", numero);
        }
        else 
           privmsg(s_CregServ, u->nick, "4Acceso denegado!!");
    }        
    } else {

    if (!(stricmp(chan, u->nick) == 0) && !is_services_preoper(u)) {
	privmsg(s_CregServ, u->nick, "4Acceso denegado!!");
	return;
    }

    privmsg(s_CregServ, u->nick, "Lista de apoyos para 12%s", chan);
      
       for (i = 0; i < 256; i++) {
	    for (cr = creglists[i]; cr; cr = cr->next) {

            if (stricmp(chan, cr->founder) == 0) {
            privmsg(s_CregServ, u->nick, "- %s (Como fundador)", cr->name);
            }
            for (apoyos = cr->apoyos, f = 0; f < cr->apoyoscount; apoyos++, f++) {
             if (stricmp(chan, apoyos->nickapoyo) == 0) {
        tm = localtime(&apoyos->time_apoyo);
        strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
        
             privmsg(s_CregServ, u->nick, "- %s (%s)", cr->name, buf);
             }
            }
           }
       }	

           privmsg(s_CregServ, u->nick, "Fin de la lista");
    }
}

/*******************************FIN APOYOS***************************/

static void do_list(User *u)
{
    char *pattern = strtok(NULL, " ");
    char *keyword;
    CregInfo *cr;
    int nchans, i;
    char buf[BUFSIZE];
    int is_servoper = is_services_oper(u);
    int16 matchflags = 0;

    if (!pattern) {
	syntax_error(s_CregServ, u, "LIST", CHAN_LIST_SYNTAX);
    } else {
	nchans = 0;
	
	while (is_servoper && (keyword = strtok(NULL, " "))) {
	    if (stricmp(keyword, "ENREGISTRO") == 0)
	        matchflags |= CR_PROCESO_REG;
	    if (stricmp(keyword, "REGISTRADO") == 0)
	        matchflags |= CR_REGISTRADO;
	    if (stricmp(keyword, "PENDIENTE") == 0)
	        matchflags |= CR_ACEPTADO;
	    if (stricmp(keyword, "DROPADO") == 0)
	        matchflags |= CR_DROPADO;
	    if (stricmp(keyword, "DENEGADO") == 0)
	        matchflags |= CR_RECHAZADO;
	    if (stricmp(keyword, "EXPIRADO") == 0)
	        matchflags |= CR_EXPIRADO;
	}                
	notice_lang(s_CregServ, u, CHAN_LIST_HEADER, pattern);
	for (i = 0; i < 256; i++) {
	    for (cr = creglists[i]; cr; cr = cr->next) {
		if ((matchflags != 0) && !(cr->estado & matchflags))
		    continue;
		        
		snprintf(buf, sizeof(buf), "%-20s  %s", cr->name,
						cr->desc ? cr->desc : "");
		if (stricmp(pattern, cr->name) == 0 ||
					match_wild_nocase(pattern, buf)) {
		    if (++nchans <= CSListMax) {
			if (cr->estado & CR_ACEPTADO) {
		            snprintf(buf, sizeof(buf), "%-20s [PENDIENTE] %s",
		                        cr->name, cr->desc);
		        }
                        if (cr->estado & CR_PROCESO_REG) {
                            snprintf(buf, sizeof(buf), "%-20s [P.APOYOS] %s",
                                        cr->name, cr->desc);
                        }		                       
			if (cr->estado & CR_REGISTRADO) {
                            snprintf(buf, sizeof(buf), "%-20s [REGISTRADO] %s",
                                        cr->name, cr->desc);
                        }		                       
			if (cr->estado & CR_RECHAZADO) {
                            snprintf(buf, sizeof(buf), "%-20s [RECHAZADO] %s",
                                        cr->name, cr->desc);
                        }		                       
			if (cr->estado & CR_DROPADO) {
                            snprintf(buf, sizeof(buf), "%-20s [DROPADO] %s",
                                        cr->name, cr->desc);
                        }		                       
			if (cr->estado & CR_EXPIRADO) {
                            snprintf(buf, sizeof(buf), "%-20s [EXPIRADO] %s",
                                        cr->name, cr->desc);
                        }		                       
			privmsg(s_CregServ, u->nick, "  %s", buf);
		    }
		}
	    }
	}
	notice_lang(s_CregServ, u, CHAN_LIST_END,
			nchans>CSListMax ? CSListMax : nchans, nchans);
    }
}


/************************INFORMACION DE CANALES*********************/
static void do_info(User *u)
{
    char *canal = strtok(NULL, " ");
    char *statu;
    CregInfo *cr;
    ChannelInfo *ci;
    ApoyosCreg *apoyos; 
    int i; 
    char buf[BUFSIZE];
    struct tm *tm;
    NickInfo *ni;

  
    if (!canal) {
        privmsg(s_CregServ, u->nick, "Sintaxis 12INFO <canal>");
        return;
    } else if (!(cr = cr_findcreg(canal))) {
        if (!(ci = cs_findchan(canal)))
          privmsg(s_CregServ, u->nick, "El canal 12%s no esta registrado", canal);
        else {
         if (ci->flags & CI_VERBOTEN)
          privmsg(s_CregServ, u->nick, "El canal 12%s no puede ser registrado ni utilizado", canal);
         else              
          privmsg(s_CregServ, u->nick, "El canal 12%s existe en 12CHaN pero no esta registrado con 12CReG", canal);
        }
    } else {
      cr=cr_findcreg(canal);

        if (cr->estado & CR_PROCESO_REG) {
               statu="PENDIENTE DE APOYOS";
        } else if (cr->estado & CR_REGISTRADO) {
         if (!(ci = cs_findchan(canal))) {
             statu="DESCONOCIDO";
         } else {
           if (ci->flags & CI_SUSPEND) {
               statu="SUSPENDIDO";
           } else {    
               statu="REGISTRADO";
           }
         }
        } else if (cr->estado & CR_ACEPTADO) {
               statu="PENDIENTE DE APROBACION";
        } else if (cr->estado & CR_RECHAZADO) {
               statu="DENEGADO";
        } else if (cr->estado & CR_DROPADO) {
               statu="DROPADO EN CHAN";
        } else if (cr->estado & CR_EXPIRADO) {
               statu="EXPIRO EN CHAN";
        } else {
               statu="DESCONOCIDO";
        }
        privmsg(s_CregServ, u->nick, "Información del canal 12%s", canal);

          if ((ni = findnick(cr->founder)))
	      privmsg(s_CregServ, u->nick, "Fundador: 12%s (12%s) - IP: (12%s)", cr->founder, cr->email, ni->last_usermask);
          else
	      privmsg(s_CregServ, u->nick, "Fundador: 12%s (12%s) - Nick no registrado", cr->founder, cr->email);
        
	tm = localtime(&cr->time_peticion);
        strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
        privmsg(s_CregServ, u->nick, "Fecha petición: 12%s", buf);



        privmsg(s_CregServ, u->nick, "Estado: 12%s", statu);
        if (cr->estado & CR_PROCESO_REG) {       
	tm = localtime(&cr->time_lastapoyo);
        strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
        privmsg(s_CregServ, u->nick, "Ultimo apoyo: 12%s", buf);
        }
	if (cr->motivo)        
             privmsg(s_CregServ, u->nick, "Motivo: 12%s", cr->motivo);
        if (cr->nickoper)        
             privmsg(s_CregServ, u->nick, "Ultimo cambio por: 12%s", cr->nickoper);
        if (cr->time_motivo) {
	   tm = localtime(&cr->time_motivo);
           strftime_lang(buf, sizeof(buf), u, STRFTIME_DATE_TIME_FORMAT, tm);
           privmsg(s_CregServ, u->nick, "Fecha cambio: 12%s", buf);
	}
        privmsg(s_CregServ, u->nick, "Descripción: 12%s", cr->desc);
        privmsg(s_CregServ, u->nick, "Apoyos:");
        for (apoyos = cr->apoyos, i = 0; i < cr->apoyoscount; apoyos++, i++) {
          if ((ni = findnick(apoyos->nickapoyo)))
              privmsg(s_CregServ, u->nick, "12%s (12%s) - IP: (12%s)", apoyos->nickapoyo, apoyos->emailapoyo, ni->last_usermask);
          else
              privmsg(s_CregServ, u->nick, "12%s (12%s) - Nick  no registrado", apoyos->nickapoyo, apoyos->emailapoyo);
        }
        privmsg(s_CregServ, u->nick, "Fin del INFO de CReG.");
    }
}

/***********************DENEGACION DE UN CANAL*******************/

static void do_deniega(User *u)
{
    char *chan = strtok(NULL, " ");
    char *razon = strtok(NULL, "");
    CregInfo *cr;

    if (!razon) {
        privmsg(s_CregServ, u->nick, "Sintaxis: 12DENIEGA <canal> <razón>");
    } else if (!(cr = cr_findcreg(chan))) {
        privmsg(s_CregServ, u->nick, "El canal 12%s no registrado en CReG", chan);
    } else if (!(cr->estado & CR_ACEPTADO)) {                                           
        privmsg(s_CregServ, u->nick, "El canal 12%s no esta pendiente de aprobacion", chan); 
    } else {
        cr->motivo = sstrdup(razon);
        cr->time_motivo = time(NULL);
        cr->nickoper = sstrdup(u->nick);
        cr->estado = 0;
        cr->estado |= CR_RECHAZADO;
        privmsg(s_CregServ, u->nick, "Al canal 12%s se le ha denegado el registro", chan);
        canalopers(s_CregServ, "12%s ha denegado el registro de 12%s", u->nick, chan);
    } 
}                                                                        

/******************ACEPTACION DE UN CANAL Y REG EN CHAN**********/
static void do_acepta(User *u)
{
    char *chan = strtok(NULL, " ");
    CregInfo *cr;
    NickInfo *ni;

    if (!chan) {
        privmsg(s_CregServ, u->nick, "Sintaxis: 12ACEPTA <canal>");
    } else if (!(cr = cr_findcreg(chan))) {
        privmsg(s_CregServ, u->nick, "El canal 12%s no registrado en CReG", chan);
    } else if (!(ni = findnick(cr->founder))) {
        privmsg(s_CregServ, u->nick, "El fundador del canal 12%s no tiene el nick registrado", chan);
    } else if (!(cr->estado & CR_ACEPTADO)) {                                           
        privmsg(s_CregServ, u->nick, "El canal 12%s no esta pendiente de aprobacion", chan); 
    } else if (!is_chanop(u->nick, chan)) {                                           
        privmsg(s_CregServ, u->nick, "Debes estar en el canal 12%s y tener OP para aceptarlo", chan);
    } else if (!registra_con_creg(u, ni, cr->name, cr->founderpass, cr->desc)) {
        privmsg(s_CregServ, u->nick, "El registro del canal 12%s en %s ha fallado", chan, s_ChanServ); 
    } else {
        cr=cr_findcreg(chan);
        cr->time_motivo = time(NULL);
        cr->nickoper = sstrdup(u->nick);
        cr->estado = 0;
        cr->estado |= CR_REGISTRADO;
        canalopers(s_CregServ, "12%s ha aceptado el canal 12%s", u->nick, chan);
        privmsg(s_CregServ, u->nick, "12%s ha sido aceptado en %s", chan, s_ChanServ); 
    }
}


static void do_drop(User *u)
{
    char *chan = strtok(NULL, " ");
    CregInfo *cr;
    ChannelInfo *cr2;

    if (!chan) {
        privmsg(s_CregServ, u->nick, "Sintaxis: 12DROP <canal>");
    } else if (!(cr = cr_findcreg(chan))) {
        privmsg(s_CregServ, u->nick, "El canal 12%s no registrado en CReG", chan);
    } else if ((cr2 = cs_findchan(chan))) {                                           
        privmsg(s_CregServ, u->nick, "El canal 12%s está registrado en CHaN y no puede ser dropado por CReG", chan); 
    } else {
        cr=cr_findcreg(chan);
        delcreg(cr);    
        privmsg(s_CregServ, u->nick, "El canal 12%s ha sido dropado de CReG", chan); 
        canalopers(s_CregServ, "12%s ha dropado el canal 12%s", u->nick, chan);
    }
}

static void do_fuerza(User *u)
{
    char *chan = strtok(NULL, " ");
    CregInfo *cr;
    NickInfo *ni;

    if (!chan) {
        privmsg(s_CregServ, u->nick, "Sintaxis: 12FUERZA <canal>");
    } else if (!(cr = cr_findcreg(chan))) {
        privmsg(s_CregServ, u->nick, "El canal 12%s no registrado en CReG", chan);
    } else if (!(ni = findnick(cr->founder))) {
        privmsg(s_CregServ, u->nick, "El fundador del canal 12%s no tiene el nick registrado", chan);
    } else if (!((cr->estado & CR_RECHAZADO) || (cr->estado & CR_PROCESO_REG))) {
        privmsg(s_CregServ, u->nick, "El canal 12%s no puede ser registrado de forma forzada", chan); 
    } else if (!is_chanop(u->nick, chan)) {              
        privmsg(s_CregServ, u->nick, "Debes estar en el canal 12%s y tener OP para aceptarlo", chan); 
    } else if (!registra_con_creg(u, ni, cr->name, cr->founderpass, cr->desc)) {
        privmsg(s_CregServ, u->nick, "El registro del canal 12%s en %s ha fallado", chan, s_ChanServ); 
    } else {
        cr=cr_findcreg(chan);
        cr->time_motivo = time(NULL);
        cr->nickoper = sstrdup(u->nick);
        cr->estado = 0;
        cr->estado |= CR_REGISTRADO;
        canalopers(s_CregServ, "12%s ha forzado la aceptación del canal 12%s", u->nick, chan);
        privmsg(s_CregServ, u->nick, "12%s ha sido registrado en %s", chan, s_ChanServ); 
    }
}

