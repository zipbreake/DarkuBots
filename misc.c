/* Miscellaneous routines.
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

/* toupper/tolower:  Like the ANSI functions, but make sure we return an
 *                   int instead of a (signed) char.
 */

int toupper(char c)
{
    if (islower(c))
	return (unsigned char)c - ('a'-'A');
    else
	return (unsigned char)c;
}

#ifdef IRC_UNDERNET
/* toUpper , Compatiblidad Undernet
 * zoltan  1/11/2000
 */

int NTL_toupper_tab[] = {
#if (CHAR_MIN<0)
/* x80-x87 */ '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
/* x88-x8f */ '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
/* x90-x97 */ '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
/* x98-x9f */ '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
/* xa0-xa7 */ '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
/* xa8-xaf */ '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
/* xb0-xb7 */ '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
/* xb8-xbf */ '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
/* xc0-xc7 */ '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
/* xc8-xcf */ '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
/* xd0-xd7 */ '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xd7',
/* xd8-xdf */ '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xdf',
/* xe0-xe7 */ '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
/* xe8-xef */ '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
/* xf0-xf7 */ '\xf0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xf7',
/* xf8-xff */ '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xff'
                ,
#endif /* (CHAR_MIN<0) */
/* x00-x07 */ '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
/* x08-x0f */ '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
/* x10-x17 */ '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
/* x18-x1f */ '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
/* ' '-x27 */    ' ',    '!',    '"',    '#',    '$',    '%',    '&', '\x27',
/* '('-'/' */    '(',    ')',    '*',    '+',    ',',    '-',    '.',    '/',
/* '0'-'7' */    '0',    '1',    '2',    '3',    '4',    '5',    '6',    '7',
/* '8'-'?' */    '8',    '9',    ':',    ';',    '<',    '=',    '>',    '?',
/* '@'-'G' */    '@',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
/* 'H'-'O' */    'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',
/* 'P'-'W' */    'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',
/* 'X'-'_' */    'X',    'Y',    'Z',    '[', '\x5c',    ']',    '^',    '_',
/* '`'-'g' */    '`',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
/* 'h'-'o' */    'H',    'I',    'J',    'K',    'L',    'M',    'N',    'O',
/* 'p'-'w' */    'P',    'Q',    'R',    'S',    'T',    'U',    'V',    'W',
/* 'x'-x7f */    'X',    'Y',    'Z',    '[', '\x5c',    ']',    '^', '\x7f'
#if (!(CHAR_MIN<0))
                ,
/* x80-x87 */ '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
/* x88-x8f */ '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
/* x90-x97 */ '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
/* x98-x9f */ '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
/* xa0-xa7 */ '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
/* xa8-xaf */ '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
/* xb0-xb7 */ '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
/* xb8-xbf */ '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
/* xc0-xc7 */ '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
/* xc8-xcf */ '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
/* xd0-xd7 */ '\xd0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xd7',
/* xd8-xdf */ '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xdf',
/* xe0-xe7 */ '\xc0', '\xc1', '\xc2', '\xc3', '\xc4', '\xc5', '\xc6', '\xc7',
/* xe8-xef */ '\xc8', '\xc9', '\xca', '\xcb', '\xcc', '\xcd', '\xce', '\xcf',
/* xf0-xf7 */ '\xf0', '\xd1', '\xd2', '\xd3', '\xd4', '\xd5', '\xd6', '\xf7',
/* xf8-xff */ '\xd8', '\xd9', '\xda', '\xdb', '\xdc', '\xdd', '\xde', '\xff'
#endif /* (!(CHAR_MIN<0)) */
  };
#endif /* IRC_UNDERNET */
                                  
int tolower(char c)
{
    if (isupper(c))
	return (unsigned char)c + ('a'-'A');
    else
	return (unsigned char)c;
}

#ifdef IRC_UNDERNET
/* toLower , Compatiblidad Undernet
 * zoltan  1/11/2000
 */
 
int NTL_tolower_tab[] = {
#if (CHAR_MIN<0)
/* x80-x87 */ '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
/* x88-x8f */ '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
/* x90-x97 */ '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
/* x98-x9f */ '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
/* xa0-xa7 */ '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
/* xa8-xaf */ '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
/* xb0-xb7 */ '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
/* xb8-xbf */ '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
/* xc0-xc7 */ '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
/* xc8-xcf */ '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
/* xd0-xd7 */ '\xd0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xd7',
/* xd8-xdf */ '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xdf',
/* xe0-xe7 */ '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
/* xe8-xef */ '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
/* xf0-xf7 */ '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xf7',
/* xf8-xff */ '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xff'
                ,
#endif /* (CHAR_MIN<0) */
/* x00-x07 */ '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
/* x08-x0f */ '\x08', '\x09', '\x0a', '\x0b', '\x0c', '\x0d', '\x0e', '\x0f',
/* x10-x17 */ '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
/* x18-x1f */ '\x18', '\x19', '\x1a', '\x1b', '\x1c', '\x1d', '\x1e', '\x1f',
/* ' '-x27 */    ' ',    '!',    '"',    '#',    '$',    '%',    '&', '\x27',
/* '('-'/' */    '(',    ')',    '*',    '+',    ',',    '-',    '.',    '/',
/* '0'-'7' */    '0',    '1',    '2',    '3',    '4',    '5',    '6',    '7',
/* '8'-'?' */    '8',    '9',    ':',    ';',    '<',    '=',    '>',    '?',
/* '@'-'G' */    '@',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
/* 'H'-'O' */    'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
/* 'P'-'W' */    'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
/* 'X'-'_' */    'x',    'y',    'z',    '{',    '|',    '}',    '~',    '_',
/* '`'-'g' */    '`',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
/* 'h'-'o' */    'h',    'i',    'j',    'k',    'l',    'm',    'n',    'o',
/* 'p'-'w' */    'p',    'q',    'r',    's',    't',    'u',    'v',    'w',
/* 'x'-x7f */    'x',    'y',    'z',    '{',    '|',    '}',    '~', '\x7f'
#if (!(CHAR_MIN<0))
                ,
/* x80-x87 */ '\x80', '\x81', '\x82', '\x83', '\x84', '\x85', '\x86', '\x87',
/* x88-x8f */ '\x88', '\x89', '\x8a', '\x8b', '\x8c', '\x8d', '\x8e', '\x8f',
/* x90-x97 */ '\x90', '\x91', '\x92', '\x93', '\x94', '\x95', '\x96', '\x97',
/* x98-x9f */ '\x98', '\x99', '\x9a', '\x9b', '\x9c', '\x9d', '\x9e', '\x9f',
/* xa0-xa7 */ '\xa0', '\xa1', '\xa2', '\xa3', '\xa4', '\xa5', '\xa6', '\xa7',
/* xa8-xaf */ '\xa8', '\xa9', '\xaa', '\xab', '\xac', '\xad', '\xae', '\xaf',
/* xb0-xb7 */ '\xb0', '\xb1', '\xb2', '\xb3', '\xb4', '\xb5', '\xb6', '\xb7',
/* xb8-xbf */ '\xb8', '\xb9', '\xba', '\xbb', '\xbc', '\xbd', '\xbe', '\xbf',
/* xc0-xc7 */ '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
/* xc8-xcf */ '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
/* xd0-xd7 */ '\xd0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xd7',
/* xd8-xdf */ '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xdf',
/* xe0-xe7 */ '\xe0', '\xe1', '\xe2', '\xe3', '\xe4', '\xe5', '\xe6', '\xe7',
/* xe8-xef */ '\xe8', '\xe9', '\xea', '\xeb', '\xec', '\xed', '\xee', '\xef',
/* xf0-xf7 */ '\xf0', '\xf1', '\xf2', '\xf3', '\xf4', '\xf5', '\xf6', '\xf7',
/* xf8-xff */ '\xf8', '\xf9', '\xfa', '\xfb', '\xfc', '\xfd', '\xfe', '\xff'
#endif /* (!(CHAR_MIN<0)) */
  };
#endif /* IRC_UNDERNET */                                    
/*************************************************************************/

/* strscpy:  Copy at most len-1 characters from a string to a buffer, and
 *           add a null terminator after the last character copied.
 */

char *strscpy(char *d, const char *s, size_t len)
{
    char *d_orig = d;

    if (!len)
	return d;
    while (--len && (*d++ = *s++))
	;
    *d = 0;
    return d_orig;
}

/*************************************************************************/

/* stristr:  Search case-insensitively for string s2 within string s1,
 *           returning the first occurrence of s2 or NULL if s2 was not
 *           found.
 */

char *stristr(char *s1, char *s2)
{
    register char *s = s1, *d = s2;

    while (*s1) {
	if (tolower(*s1) == tolower(*d)) {
	    s1++;
	    d++;
	    if (*d == 0)
		return s;
	} else {
	    s = ++s1;
	    d = s2;
	}
    }
    return NULL;
}

/*************************************************************************/

/* strupper, strlower:  Convert a string to upper or lower case.
 */

char *strupper(char *s)
{
    char *t = s;
    while (*t) {
	*t = toupper(*t);
	t++;
    }
    return s;
}

char *strlower(char *s)
{
    char *t = s;
    while (*t) {
	*t = tolower(*t);
	t++;
    }
    return s;
}

#ifdef IRC_UNDERNET
/* Para compatiblidad Undernet
 * zoltan 1/11/2000
 */
int strCasecmp(const char *a, const char *b)
{
    register const char *ra = a;
    register const char *rb = b;
    while (toLower(*ra) == toLower(*rb))
        if (!*ra++)
            return 0;
        else
            rb++;
        return (*ra - *rb);

}
#endif
/*************************************************************************/

/*
 * strtoken.c
 *
 * Walk through a string of tokens, using a set of separators.
 */

char *strToken(char **save, char *str, char *fs)
{
  char *pos = *save;            /* keep last position across calls */
  register char *tmp;
    
  if (str)
      pos = str;                  /* new string scan */
      
  while (pos && *pos && strchr(fs, *pos) != NULL)
      pos++;                      /* skip leading separators */
       
  if (!pos || !*pos)
      return (pos = *save = NULL);        /* string contains only sep's */

  tmp = pos;                    /* now, keep position of the token */
  
  while (*pos && strchr(fs, *pos) == NULL)
      pos++;                      /* skip content of the token */
          
  if (*pos)
      *pos++ = '\0';              /* remove first sep after the token */
  else
      pos = NULL;                 /* end of string */
       
  *save = pos;
  return (tmp);
}                  

/*
 * * NOT encouraged to use!
  */
  
char *strTok(char *str, char *fs)
{
  
   static char *pos;
     
   return strtoken(&pos, str, fs);
        
}
        
                                   
/*************************************************************************/

/* strnrepl:  Replace occurrences of `old' with `new' in string `s'.  Stop
 *            replacing if a replacement would cause the string to exceed
 *            `size' bytes (including the null terminator).  Return the
 *            string.
 */

char *strnrepl(char *s, int32 size, const char *old, const char *new)
{
    char *ptr = s;
    int32 left = strlen(s);
    int32 avail = size - (left+1);
    int32 oldlen = strlen(old);
    int32 newlen = strlen(new);
    int32 diff = newlen - oldlen;

    while (left >= oldlen) {
	if (strncmp(ptr, old, oldlen) != 0) {
	    left--;
	    ptr++;
	    continue;
	}
	if (diff > avail)
	    break;
	if (diff != 0)
	    memmove(ptr+oldlen+diff, ptr+oldlen, left+1);
	strncpy(ptr, new, newlen);
	ptr += newlen;
	left -= oldlen;
    }
    return s;
}

/*************************************************************************/
/*************************************************************************/

/* merge_args:  Take an argument count and argument vector and merge them
 *              into a single string in which each argument is separated by
 *              a space.
 */

char *merge_args(int argc, char **argv)
{
    int i;
    static char s[4096];
    char *t;

    t = s;
    for (i = 0; i < argc; i++)
	t += snprintf(t, sizeof(s)-(t-s), "%s%s", *argv++, (i<argc-1) ? " " : "");
    return s;
}

/*************************************************************************/
/*************************************************************************/

/* match_wild:  Attempt to match a string to a pattern which might contain
 *              '*' or '?' wildcards.  Return 1 if the string matches the
 *              pattern, 0 if not.
 */

static int do_match_wild(const char *pattern, const char *str, int docase)
{
    char c;
    const char *s;

    /* This WILL eventually terminate: either by *pattern == 0, or by a
     * trailing '*'. */

    for (;;) {
	switch (c = *pattern++) {
	  case 0:
	    if (!*str)
		return 1;
	    return 0;
	  case '?':
	    if (!*str)
		return 0;
	    str++;
	    break;
	  case '*':
	    if (!*pattern)
		return 1;	/* trailing '*' matches everything else */
	    s = str;
	    while (*s) {
		if ((docase ? (*s==*pattern) : (tolower(*s)==tolower(*pattern)))
					&& do_match_wild(pattern, s, docase))
		    return 1;
		s++;
	    }
	    break;
	  default:
	    if (docase ? (*str++ != c) : (tolower(*str++) != tolower(c)))
		return 0;
	    break;
	} /* switch */
    }
}


int match_wild(const char *pattern, const char *str)
{
    return do_match_wild(pattern, str, 1);
}

int match_wild_nocase(const char *pattern, const char *str)
{
    return do_match_wild(pattern, str, 0);
}

/*************************************************************************/
/*************************************************************************/

/* Process a string containing a number/range list in the form
 * "n1[-n2][,n3[-n4]]...", calling a caller-specified routine for each
 * number in the list.  If the callback returns -1, stop immediately.
 * Returns the sum of all nonnegative return values from the callback.
 * If `count' is non-NULL, it will be set to the total number of times the
 * callback was called.
 *
 * The callback should be of type range_callback_t, which is defined as:
 *	int (*range_callback_t)(User *u, int num, va_list args)
 */

int process_numlist(const char *numstr, int *count_ret,
		range_callback_t callback, User *u, ...)
{

    int n1, n2, min, max, i;
    int retval = 0;
    int numcount = 0;
    va_list args;
    static char numflag[65537];
                    
    memset(numflag, 0, sizeof(numflag));
    min = 65536;
    max = 0;
    va_start(args, u);
    
    /*
     * This algorithm ignores invalid characters, ignores a dash
     * when it precedes a comma, and ignores everything from the
     * end of a valid number or range to the next comma or null.
     */
    while (*numstr) {
	n1 = n2 = strtol(numstr, (char **)&numstr, 10);
	numstr += strcspn(numstr, "0123456789,-");
	if (*numstr == '-') {
	    numstr++;
	    numstr += strcspn(numstr, "0123456789,");
	    if (isdigit(*numstr)) {
		n2 = strtol(numstr, (char **)&numstr, 10);
		numstr += strcspn(numstr, "0123456789,-");
	    }
	}
        if (n1 < 0)
            n1 = 0;
        if (n2 > 65536)
            n2 = 65536;
        if (n1 < min)
            min = n1;
        if (n2 > max)
            max = n2;
        while (n1 <= n2) {
            numflag[n1] = 1;
            n1++;
        }                                                                                	
        numstr += strcspn(numstr, ",");
        if (*numstr)
            numstr++;
    }
    
    /* Now call the callback routine for each index. */
    numcount = 0;
    for (i = min; i <= max; i++) {
        int res;
        if (!numflag[i])
            continue;
        numcount++;
        res = callback(u, i, args);
        if (debug)
            log("debug: process_numlist: tried to do %d; result = %d", i, res);
        if (res < 0)
            break;
        retval += res;
    }
    
    va_end(args);
    if (count_ret)
        *count_ret = numcount;
    return retval;
}                                                                                

/*************************************************************************/

/* dotime:  Return the number of seconds corresponding to the given time
 *          string.  If the given string does not represent a valid time,
 *          return -1.
 *
 *          A time string is either a plain integer (representing a number
 *          of seconds), or an integer followed by one of these characters:
 *          "s" (seconds), "m" (minutes), "h" (hours), or "d" (days).
 */

int dotime(const char *s)
{
    int amount;

    amount = strtol(s, (char **)&s, 10);
    if (*s) {
	switch (*s) {
	    case 's': return amount;
	    case 'm': return amount*60;
	    case 'h': return amount*3600;
	    case 'd': return amount*86400;
	    default : return -1;
	}
    } else {
	return amount;
    }
}

/*************************************************************************/

#define DOMAIN_MAXLEN   255
#define DOMPART_MAXLEN  63

int valid_domain(const char *str)
{
    const char *s;
    int i;
    static const char valid_domain_chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-.";

    if (!*str)
        return 0;
    if (str[strspn(str,valid_domain_chars)] != 0)
        return 0;
    s = str;
    while (s-str < DOMAIN_MAXLEN && *s) {
        if (*s == '-' || *s == '.')
            return 0;
        i = strcspn(s, ".");
        if (i > DOMPART_MAXLEN)
            return 0;
        s += i;
        if (*s)
            s++;
    }
    if (s-str > DOMAIN_MAXLEN || *s)
        return 0;
    if (s[-1] == '.')
        return 0;
    return 1;
}

int valid_ipvirtual(const char *str)
{
    const char *s;
    int i;
    static const char valid_domain_chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-.";

    if (!*str)
        return 0;
    if (str[strspn(str,valid_domain_chars)] != 0)
        return 0;

    return 1;
}

