/* Rutinas de envio de correos
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


#ifdef SMTP
int check_smtp(char buf[BUFSIZE]);
#endif


/**************** enviar_correo *******************************
Esta funcion envia un correo con el email configurado del source k sea
con el subject y body pertinentes al destino indicado.
*************************************************************/
int enviar_correo(const char * destino, const char *subject, const char *body)
{
#ifdef SMTP
	int sockmail;
	struct hostent *hp;
	struct sockaddr_in their_addr;
	char buf[1024];

	if ((hp=gethostbyname(ServerSMTP)) == NULL)
		return 0;
	if ((sockmail=socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 0;
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(PortSMTP);
	their_addr.sin_addr = *((struct in_addr *)hp->h_addr);
	bzero(&(their_addr.sin_zero),8);

	if (connect(sockmail, (struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == -1)
		return 0;
	sprintf(buf, "HELO localhost\n"
			"MAIL FROM: %s\n"
			"RCPT TO: %s\n"
			"DATA\n"
			"From: %s\n"
			"To: %s\n"
			"Subject: %s\n"
			"%s\n"
			".\n"
			"QUIT\n", SendFrom, destino, SendFrom, destino, subject, body);
	if (send(sockmail, buf, sizeof(buf),0) == -1)
		return 0;

	close(sockmail);
	return 1;
	
}    
#endif
#ifdef SENDMAIL
    FILE *p;
    char sendmail[PATH_MAX];
                                                                              
    snprintf(sendmail, sizeof(sendmail), SendMailPatch, destino);
//    snprintf(sendmail, sizeof(sendmail), "/usr/sbin/sendmail masakresoft@wanadoo.es");
                                                                                           
    if (!(p = popen(sendmail, "w"))) {
//        privmsg(s_NickServ, u->nick, "mail jodido");
        log("mail jodio");
        return 0;
    }

    fprintf(p, "From: %s\n", SendFrom);
    fprintf(p, "To: %s\n", destino);
    fprintf(p, "Subject: %s\n", subject);    
    fprintf(p, "%s\n", body);

    fprintf(p, "\n.\n");
         
    pclose(p);    
//    log("Parece que funciona!! - %s - %s - %s", destino, subject, body);
    return 1;    
}    
#endif



#ifdef SMTP
int check_smtp(char buf[BUFSIZE])
{
/** Esta funcion recibe la contestacion k recibimos del smtp y nos
devuelve si se ha producido un erro o no. Caso de ser asi, logea el
error. ***/

    char cod[5];
    int codigo;
        
  //log("SMTP = %s",buf);
    strncpy(cod,buf,4);
  //log("strncpy....");
    codigo = strlen(cod);
    cod[codigo] = '\0';
  //log("asignado cod..");
    if (cod) codigo = atoi(cod);
    else codigo = 500;
    
  //log("Codigo SMTP = %d",codigo);

    switch(codigo) {
        case 220:
            return 1;
            break;
                 
        case 221:
            return 1;
            break;    

        case 250:
            return 1;
            break;
                                
        case 354:
            return 1;
            break;            
            
        default:
            log("SMTP ERROR(%d) : %s",codigo,buf);
            return 0;
    }            
    return 0;
}
#endif

// #endif /* #ifdef A_TOMAR_POR_EL_CULO */
