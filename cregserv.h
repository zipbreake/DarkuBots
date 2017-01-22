/* Declarations for CregServ.
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
          
typedef struct {
    char *nickapoyo;
    char *emailapoyo;
    time_t time_apoyo;
} ApoyosCreg;
            

typedef struct creginfo_ CregInfo;

struct creginfo_ {
    CregInfo *next, *prev;
    char name[CHANMAX];          /* Nombre del canal */
    char *founder;               /* Nick del founder */
            
    char founderpass[PASSMAX];   /* Password de founder */
    char *desc;                  /* Descripcion del canal */
    char *email;                 /* Email del founder */
    time_t time_peticion;        /* Hora de la peticion */
              
    char *nickoper;              /* Nick del OPER */
    time_t time_motivo;          /* Hora del cambio de estado del canal */
    char *motivo;                /* Motivo de la suspension, etc.. */
                                        
    char passapoyo[PASSMAX];     /* Contraseña de apoyo */
    time_t time_lastapoyo;       /* Hora del ultimo apoyo realizado */
                                                
    int32 estado;   		/* Estado del canal */

    int16 apoyoscount;           /* Contador del numero de apoyos */
    ApoyosCreg *apoyos;          /* Lista de los apoyos realizados */
};
                
                
#define CR_PROCESO_REG  0x00000001
#define CR_EXPIRADO     0x00000002
#define CR_RECHAZADO    0x00000004
#define CR_DROPADO      0x00000008
#define CR_ACEPTADO     0x00000010
#define CR_REGISTRADO   0x00000020

