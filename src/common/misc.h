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
#ifndef __misc_h
#define __misc_h

/*
 * Functions that we may need to provide ourself if they are not available
 * under this operating system.
 */
#if !HAVE_STRCASECMP
int strcasecmp(const char *s1,const char *s2);
#endif
#if !HAVE_STRNCASECMP
int strncasecmp(const char *s1,const char *s2,size_t n);
#endif
#if !HAVE_STRCASESTR
const char *strcasestr(const char *big,const char *little);
#endif
#if !HAVE_STRSPN
size_t strspn(const char *s,const char *charset);
#endif
#if !HAVE_STRCSPN
size_t strspn(const char *s,const char *charset);
#endif
#if !HAVE_STRDUP
char *strdup(const char *s);
#endif

/*
 * Version Flag codes.
 */
#define VF_OLD		0x1		/* This is an old, possibly incompatable version */
#define VF_NOBACK	0x2		/* Save file may be unreadable in earlier versions */
#define VF_UNKNOWN	0x4		/* Unknown version number (future version?) */

/*
 * Token types used by getNextToken
 */
#define ST_END		0		/* No token was found (end of buffer) */
#define ST_LITERAL	1		/* A literal token such as "foo" and "_bar" */
#define ST_NUMBER	2		/* A number token such as "1", "0.2", "5e-10" */
#define ST_STRING	3		/* A quoted string token */
#define ST_SYMBOL	4		/* A symbol token such as "+", "@", etc. */		

#define imin(a,b) ((a) < (b) ? (a) : (b))
#define imax(a,b) ((a) > (b) ? (a) : (b))
#define iabs(n) (((n) < 0) ? -(n) : (n))

#define binstr(b) ((b) ? "1" : "0")
#define strbin(b) ((b) && *(b) == '1')

#define isodigit(c) ((c) >= '0' && (c) <= '7')

typedef struct {
  char		*vd_name;		/* Name for version number */
  unsigned	vd_flags;		/* flags for this version */
} VersionDef;

typedef struct {
  char str[32];		/* String version of version number */
  int major;		/* Major version number */
  int minor;		/* Minor version number */
  int revision;		/* Revision */
  int alpha;		/* Alpha level */
  int beta;		/* Beta level */
} Version;

typedef struct {
  int	length;
  char	**names;
} NetAliasList;

char *strend(char *s);
char *GetSysType();
char *skipfield(char *s);
char *quoteChars(char *buf,const char *str,const char *qlist);
char *tclizeString(char *buf,const char *str);
void strreplace(char*s,const char *old_str,const char *new_str,int gbl);
int getNextToken(char **text,char **token_begin,char **token_end);
char *getLineStart(char *p,char *buf_start);
char *getLineEnd(char *p);
void printIndent(int n,FILE *f);
int isVerilogConstant(const char *s);

int required_bits(int n);

int ParseVersion(const char *version,Version *V);
int VersionCmp(Version *V1,Version *V2);

int rotateX(int x,int y,int d);
int rotateY(int x,int y,int d);

const char *verIotypeStr(int code);
const char *iotypeStr(int code);
int strIOType(const char *code);

unsigned htoi(const char *s);
const char *article(const char *m);
int trimName(char *buf);
const char *bitrangeSpec(int n);

#define tv_to_ms(tv) ( ((unsigned long long)(tv)->tv_sec)*1000 + (tv)->tv_usec/1000)

#endif
