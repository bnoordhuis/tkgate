/****************************************************************************
    Copyright (C) 1987-2005 by Jeffery P. Hansen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
****************************************************************************/
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "misc.h"

/*
 * Protected tolower() function in case it only works for upper case letters
 * on this system.
 */
#define p_tolower(c) (isupper(c) ? tolower(c) : (c))


const char *article(const char *m)
{
  if (strchr("aeiouxAEIOUX",*m))
    return "an ";
  else
    return "a ";
}

unsigned htoi(const char *s)
{
  unsigned v;

  if (sscanf(s,"%x",&v) != 1) v = 0;

  return v;
}

char *strend(char *s)
{
    return s + strlen(s);
}

/*
    return the cpu type.
*/
char *GetSysType()
{
  static char *sys_name = 0;

  if (!sys_name) {
    char buf[1024];
    FILE *p;

    if ((p = popen("uname","r"))) {
      char *x;
      fgets(buf,1024,p);
      pclose(p);
      if ((x = strrchr(buf,'\n'))) *x = 0;
    } else
      strcpy(buf,"unknown");
    sys_name = strdup(buf);
  }

  return sys_name;
}

char *skipfield(char *s)
{
    while (*s == ' ' || *s == '\t' || *s == '\n') s++;
    while (*s && *s != ' ' && *s != '\t' && *s != '\n') s++;

    return s;
}

char *quoteChars(char *buf,const char *str,const char *qlist)
{
  char *p = buf;

  for (;*str;str++) {
    if (strchr(qlist,*str))
      *p++ = '\\';
    *p++ = *str;
  }
  *p = 0;

  return buf;
}

/*****************************************************************************
 *
 * Make a string "Tcl Safe"
 *
 *****************************************************************************/
char *tclizeString(char *buf,const char *str)
{
  int needQuotes = 0;
  int hasQuotes = 0;
  char *p = buf;

  if (*str == '"' && str[strlen(str)-1] == '"')  {
    str++;
    *p++ = '"';
    hasQuotes = 1;
  }

  needQuotes = strchr(str,' ') || strchr(str,'\t');

  if (needQuotes)
    *p++ = '"';

  for (;*str;str++) {
    if (*str == '\\')
      *p++ = '\\';

    if (*str == '"' && (!hasQuotes || str[1]))
      *p++ = '\\';

    if (*str == '\n') {
      *p++ = '\\';
      *p++ = 'n';
    } else if (*str == '\r') {
      *p++ = '\\';
      *p++ = 'r';
    } else if (isprint(*str)) {
      *p++ = *str;
    } else {
      *p++ = ' ';;
    }
  }

  if (needQuotes)
    *p++ = '"';
  *p = 0;

  return buf;
}


/*****************************************************************************
 *
 * Print an n-level indentation (two spaces per level)
 *
 * Parameters:
 *     n		Number of levels of indentation
 *     f		File to print indentation on.
 *
 *****************************************************************************/
void printIndent(int n,FILE *f)
{
  int i;

  for (i = 0;i < n;i++)
    fprintf(f,"  ");
}

/*****************************************************************************
 *
 * Returns the log 2 of n rounded up.
 *
 * Parameters:
 *     n		Number to which to apply log2 operations
 *
 * Returns:		Log2 of n rounded up.
 *
 * Examples: 
 *	log2(1) = 1
 *	log2(2) = 1
 *	log2(3) = 2
 *	log2(4) = 2
 *	log2(5) = 3
 *
 * The number of select lines on an n way multiplexor is then log2(n).
 *
 *****************************************************************************/
int required_bits(int n)
{
  int i,b;

  for (i = b = 0;n; i++) {
    b += (n & 1);
    n >>= 1;
  }

  return i - 1 + (b > 1);
}

void strreplace(char *s,const char *old_str,const char *new_str,int gbl)
{
  char *b = s;
  int ol = strlen(old_str);
  int nl = strlen(new_str);
  int sl = strlen(s);

  for (;*s;s++) {
    if ( (sl-(s-b)) < ol) break;		/* Not enough chars left */

    if (strncmp(s,old_str,ol) == 0) {
      memmove(s+nl,s+ol,(sl-(s-b)));
      memmove(s,new_str,nl);
      s += nl;
      sl += nl-ol;
      if (!gbl) return;
    }
  }
  b[sl] = 0;
}

int trimName(char *buf)
{
  char *p;
  char *q;
  int did_trim = 0;

  if (isdigit((int)*buf)) {
    memmove(buf+1,buf,strlen(buf)+1);
    *buf = 'D';
    did_trim = 1;
  }

  p = q = buf;

  for (;*q;q++) {
    if (isalpha((int)*q) || isdigit((int)*q) || *q == '_')
      *p++ = *q;
    else if (*q == ' ') { 
      if (p == buf)
	*p++ = 'S';
      else
	*p++ = '_';
    } else
      did_trim = 1;
  }
  *p = 0;

  return did_trim;
}

/*****************************************************************************
 *
 * Return a verilog format specification for a bit range.  
 * 
 * Parameters:
 *    n			Number of bits.
 *
 * Returns:		A verilog syntax bit range specifier, or empty string
 *			if n is less than or equal to 1.
 *
 *****************************************************************************/
const char *bitrangeSpec(int n)
{
  static char buf[32];

  if (n > 1)
    sprintf(buf," [%d:0]",n-1);
  else
    *buf = 0;

  return buf;
}

/*****************************************************************************
 *
 * Simple token handling for blocks of verilog text
 *
 * Parameters:
 *     **text		Text to be parsed (or 0 if getting the next
 *     **token_begin	Pointer to first character in token found 
 *     **token_end	Pointer to one after last character in token found 
 *
 * Returns:		Zero if no tokens where available
 *
 * The primary purpose of this function is to get the positions of the beginning
 * and end of modules in a block of verilog text.  This is complicated by the
 * fact that we do not want to be confused when the word "module" appears in a
 * comment or in a string.  To this purpose, this function will only return
 * "word" tokens.  That is tokens that are a string of alphanumeric characters
 * or the "_" character.  Comments and other tokens such as strings will be 
 * ignored.
 *
 *****************************************************************************/
int getNextToken(char **text,char **token_begin,char **token_end)
{
  int token_type = ST_END;
  char *p;

  p = *text;

  for (;;) {

    /* Skip any whitespace */
    for (;*p;p++) {
      if (!isspace(*p)) break;
    }

    if (!*p) return ST_END;		/* Nothing left to parse */

    *token_begin = p;
    if (*p == '\\') {
      /*
       * Handle any backslash literal
       */
      p++;
      while (*p && !isspace(*p)) p++;
      *token_end = p;
      token_type = ST_LITERAL;
      break;
    } else if (*p == '"') {
      /*
       * Handle strings.
       */
      p++;
      for (;*p && *p != '"';p++) {
	if (*p == '\\' && p[1]) p++;	/* Skip quoted characters */
      }
      if (*p) p++;
      *token_end = p;
      token_type = ST_STRING;
      break;
    } else if (isdigit(*p) || (*p == '.' && isdigit(p[1]))) {
      while (isdigit(*p)) p++;
      if (*p == '.') p++;
      while (isdigit(*p)) p++;
      if (*p == 'e' || *p == 'E') {
	p++;
	if (*p == '-' || *p == '+') p++;
	while (isdigit(*p)) p++;
      }
      *token_end = p;
      token_type = ST_NUMBER;
    } else if (isalpha(*p) || *p == '_') {
      *token_begin = p++;
      while (isalnum(*p) || *p == '_') p++;
      *token_end = p;
      token_type = ST_LITERAL;
      break;
    } else if (strncmp(p,"//",2) == 0) {
      while (*p && *p != '\n') p++;
    } else if (strncmp(p,"/*",2) == 0) {
      while (*p && strncmp(p,"*/",2)) p++;
      if (*p) p += 2;
    } else {
      p++;	/* Any other character */
      *token_end = p;
      token_type = ST_SYMBOL;
    }

  }

  *text = *token_end;
  return token_type;
}

char *getLineStart(char *p,char *buf_start)
{
  if (p > buf_start) p--;

  while (p > buf_start && *p != '\n' ) p--;
  if (*p == '\n') p++;

  return p;
}

char *getLineEnd(char *p)
{
  while (*p && *p != '\n') p++;
  return p;
}

/*****************************************************************************
 *
 * Test a string to see if it is a valid verilog constant.
 *
 *****************************************************************************/
int isVerilogConstant(const char *s)
{
  int did_digits = 0;

  while (isspace(*s)) s++;

  if (strlen(s) == 0) return 0;

  if (*s == '-') s++;

  while (isdigit(*s)) {
    did_digits = 1;
    s++;
  }

  /*
   * Space or string end after digits.  This must be a plain decimal constant.
   */
  if (did_digits && (!*s || isspace(*s))) {
    while (isspace(*s)) s++;
    return (!*s);
  }

  /*
   * Character after any digits must be a '
   */
  if (*s != '\'') return 0;
  s++;

  did_digits = 0;
  switch (*s++) {
  case 'd' :
    while (isdigit(*s) || strchr("xXzZ",*s) != 0) { s++; did_digits = 1; }
    break;
  case 'h' :
    while (isxdigit(*s) || strchr("xXzZ",*s) != 0) { s++; did_digits = 1; }
    break;
  case 'o' :
    while ((*s >= '0' && *s <= '7') || strchr("xXzZ",*s) != 0) { s++; did_digits = 1; }
    break;
  case 'b' :
    while (strchr("01xzXZ",*s)) { s++; did_digits = 1; }
    break;
  default :
    return 0;
  }

  if (!did_digits) return 0;

  while (isspace(*s)) s++;
  return (!*s);
}

#if !HAVE_STRCASECMP
int strcasecmp(const char *s1,const char *s2)
{
  for (;p_tolower(*s1) == p_tolower(*s2);s1++, s2++) {
    if (!*s1 || !*s2) break;
  }

  return p_tolower(*s1) - p_tolower(*s2);
}
#endif

#if !HAVE_STRNCASECMP
int strncasecmp(const char *s1,const char *s2,size_t n)
{
  for (;n > 0 && p_tolower(*s1) == p_tolower(*s2);s1++, s2++, n--) {
    if (!*s1 || !*s2) break;
  }
  if (n == 0) return 0; 

  return p_tolower(*s1) - p_tolower(*s2);
}
#endif

#if !HAVE_STRCASESTR
const char *strcasestr(const char *big,const char *little)
{
  const char *p,*q,*l;

  for (p = big;*p;p++) {
    for (q = p, l = little;*l && p_tolower(*q) == p_tolower(*l);q++, l++);
    if (!*l) return p;
    if (!*q) return 0;
  }

  return 0;
}
#endif

#if !HAVE_STRSPN
size_t strspn(const char *s,const char *charset)
{
  size_t n = 0;

  for (;*s;s++, n++) {
    const char *p;
    for (p = charset;*p;p++)
      if (*s == *p) break;
    if (!*p) break;
  }

  return n;
}
#endif

#if !HAVE_STRCSPN
size_t strcspn(const char *s,const char *charset)
{
  size_t n = 0;

  for (;*s;s++, n++) {
    const char *p;
    for (p = charset;*p;p++)
      if (*s == *p) break;
    if (*p) break;
  }

  return n;
}
#endif

#if !HAVE_STRDUP
char *strdup(const char *s)
{
  int l = strlen(s);
  char *d = (char*)malloc(l+1);

  strcpy(d,s);
  
  return d;
}
#endif


int rotateX(int x,int y,int r)
{
  r = (r+4)&0x3;
  switch (r) {
  case 0 : return x;
  case 1 : return y;
  case 2 : return -x;
  case 3 : return -y;
  }
  return 0;
}

int rotateY(int x,int y,int r)
{
  r = (r+4)&0x3;
  switch (r) {
  case 0 : return y;
  case 1 : return -x;
  case 2 : return -y;
  case 3 : return x;
  }
  return 0;
}
