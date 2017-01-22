/* Protocolo Undernet P10 para los Services.
 *
 * Services is copyright (c) 1996-1999 Andy Church.
 *     E-mail: <achurch@dragonfire.net>
 * This program is free but copyrighted software; see the file COPYING for
 * details.
 *
 * Modulo Undernet P10 hecho por zoltan <zolty@ctv.es>
 * 23-09-2000
 *
 * El codigo ha sido tomado del ircu de Undernet 
 * Web para pillar ircus de undernet <http://coder-com.undernet.org>
 *
 * DarkuBots es una adaptación de Javier Fernández Viña, ZipBreake.
 * E-Mail: javier@jfv.es || Web: http://jfv.es/
 *
 */



#ifdef IRC_UNDERNET_P10

#include "services.h"


char convert2y[NUMNICKBASE] = {
  'A','B','C','D','E','F','G','H','I','J','K','L','M',
  'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
  'a','b','c','d','e','f','g','h','i','j','k','l','m',
  'n','o','p','q','r','s','t','u','v','w','x','y','z',
  '0','1','2','3','4','5','6','7','8','9',
  '[',']'
};
  
unsigned char convert2n[NUMNICKMAXCHAR + 1] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,52,53,54,55,56,57,58,59,60,61, 0, 0,
  0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
 15,16,17,18,19,20,21,22,23,24,25,62, 0,63, 0, 0, 0,26,27,28,
 29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
 49,50,51
};

unsigned int base64toint(const char *str)
{
  register unsigned int i;
  i = convert2n[(unsigned char)str[5]];
  i += convert2n[(unsigned char)str[4]] << 6;
  i += convert2n[(unsigned char)str[3]] << 12;
  i += convert2n[(unsigned char)str[2]] << 18;
  i += convert2n[(unsigned char)str[1]] << 24;
  i += convert2n[(unsigned char)str[0]] << 30;
  return i;
}

const char *inttobase64(unsigned int i)
{
  static char base64buf[7];
  base64buf[0] = convert2y[(i >> 30) & 0x3f];
  base64buf[1] = convert2y[(i >> 24) & 0x3f];
  base64buf[2] = convert2y[(i >> 18) & 0x3f];
  base64buf[3] = convert2y[(i >> 12) & 0x3f];
  base64buf[4] = convert2y[(i >> 6) & 0x3f];
  base64buf[5] = convert2y[i & 0x3f];
  base64buf[6] = 0; /* (static is initialized 0) */
  return base64buf;
}


#endif /* IRC_UNDERNET_P10 */
