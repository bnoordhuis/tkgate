/****************************************************************************
    Copyright (C) 1987-2009 by Jeffery P. Hansen

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

    Last edit by hansen on Wed Mar 18 04:17:20 2009
****************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdarg.h>
#include "tkgate.h"
#if HAVE_ICONV_H
#include <iconv.h>
#endif

int guiActive = 0;

int MESSAGEBARSIZE = 50;

int MB_MESSAGEX = 25;
int MB_MESSAGEY = 40;

int MB_STATUSX = 25;
int MB_STATUSY = 20;

int KANJIFONT_WIDTH    	= 14;

/*
 * The editstate is an optional argument need for modes 2 and 3.
 *
 * Notes:
 *   TkGate.tcl		is non-zero if we are using a GUI
 *   guiActive		is non-zero if the GUI is being actively displayed
 *
 */
void message(int mode,const char *s,...)
{
  va_list ap;
  char buf[1024];
  extern int quietMode;
  int did_display = 0;		/* Could we display message in GUI? */

  va_start(ap, s);
  vsprintf(buf,s,ap);
  va_end(ap);


  if (TkGate.tcl && mode == 0) {
    /*
     * Legacy support for old-style mode values
     */
    Tcl_SetVar(TkGate.tcl,"tkg_statusMessage",buf,TCL_LEAVE_ERR_MSG|TCL_GLOBAL_ONLY);
    return;
  }

  if (TkGate.tcl && (mode & MC_STATUS)) {
    Tcl_SetVar(TkGate.tcl,"tkg_statusMessage",buf,TCL_LEAVE_ERR_MSG|TCL_GLOBAL_ONLY);
    did_display = 1;
  }


  if (guiActive && (mode & MC_ERRBOX)) {
    Tcl_VarEval(TkGate.tcl, "errmsg {",buf,"}",0);
    did_display = 1;
  }

  if (TkGate.tcl && (mode & MC_MSGLOG)) {
    const char *color = 0;

    if ((mode & MC_URGENT))
      color = "red";
    else if ((mode & MC_WARNING))
      color = "goldenrod";
    else
      color = "black";

    if ((mode & MC_SILENT))
      DoTclL("InfoPanel::log",buf,"-noshow","1","-color",color,NULL);
    else
      DoTclL("InfoPanel::log",buf,"-color",color,NULL);
    did_display = 1;
  }

  /*
   * If we were not able to display the message through the GUI and we
   * are not in quite mode, display it to the terminal.
   */
  if (!quietMode && !did_display) {
    fprintf(stderr,"tkgate: %s\n",buf);
  }

  /*
   * If we were not able to display the message in the GUI yet, defer it
   * for later display.
   */
  if (!did_display) {
  }
}

int fontheight(XFontStruct *F)
{
  return F->ascent+F->descent + 3;
}

/*
    Draw the string ala ZDrawString.  Includes temporary hacked support
    for strings with mixed latin and kanji characters.  Characters with
    the high bit set are assumed to be kanji.  This function is partially
    broken in that it assumes the font for the latin portion is TkGate.textF
    when in fact it may be whatever is set in the gc.  For now, since kanji
    is only allowed in comments and frames, this should not be a problem.
*/
void GKDrawString(Display *D,Window W,GC gc,int x,int y,const char *ps,int l)
{
  if (TkGate.japaneseMode) {
    char buf[STRMAX];
    char *s = buf;

    strncpy(buf,ps,STRMAX);
    while (l > 0) {
      int m = 0;
      int i;

      if ((s[0] & 0x80)) {		/* Kanji segment */
	for (m = 0;m < l && (s[m] & 0x80);m++);
	for (i = 0;i < m;i++) s[i] &= 0x7f;
	ZDrawString16(D,W,TkGate.kanjiGC,x,y,(XChar2b*)s,m/2);
	if (m != l) x += KANJIFONT_WIDTH*(m/2);
	for (i = 0;i < m;i++) s[i] |= 0x80;
      } else {				/* non-Kanji segment */
	for (m = 0;m < l && !(s[m] & 0x80);m++);
	ZDrawString(D,W,gc,x,y,s,m);
	if (m != l) x += XTextWidth(TkGate.textXF[TkGate.circuit->zoom_factor],s,m);
      }

      l -= m;
      s += m;
    }
  } else {
    /*
     * If not handling Japanese, don't do any special processing.
     */
    ZDrawString(D,W,gc,x,y,(char*)ps,l);
  }
}

/*
   Get the width of a string.  This may be extended in the future to handle
   mixed english/japanese fonts.
*/
int GKTextWidth(XFontStruct *F,const char *ps,int l)
{
  if (TkGate.japaneseMode) {
    char buf[STRMAX];
    char *s = buf;
    int w = 0;


    strncpy(buf,ps,STRMAX);

    while (l > 0) {
      int m = 0;
      int i;

      if ((s[0] & 0x80)) {		/* Kanji segment */
	for (m = 0;m < l && (s[m] & 0x80);m++);
	for (i = 0;i < m;i++) s[i] &= 0x7f;
      
	w += KANJIFONT_WIDTH*(m/2);
	for (i = 0;i < m;i++) s[i] |= 0x80;
      } else {				/* non-Kanji segment */
	for (m = 0;m < l && !(s[m] & 0x80);m++);
	w += XTextWidth(F,s,m);
      }

      l -= m;
      s += m;
    }

    return w;
  } else {
    return XTextWidth(F,ps,l);
  }
}

int PosDrawString(Window W,XFontStruct *F,GC gc,int x,int y,const char *S,int p){
  int x_w,y_w;

  if (!F) F = TkGate.textXF[1];

  if (*S == '_') {
    S++;
    x_w = GKTextWidth(F,S,strlen(S));
    y_w = fontheight(F);

    if (p & BetweenLeftAndRight) {
      ZDrawLine(TkGate.D,W,gc,x - x_w/2,y - 2*y_w/3,
		x + x_w/2,y - 2*y_w/3);
    } else if (p & AtRight) {
      ZDrawLine(TkGate.D,W,gc,x - x_w,y - 2*y_w/3,
		x,y - 2*y_w/3);
    } else if (p & AtLeft) {
      ZDrawLine(TkGate.D,W,gc,x,y - 2*y_w/3,
		x + x_w,y - 2*y_w/3);
    }
  } else
    x_w = GKTextWidth(F,S,strlen(S));
  
  if (p & BetweenLeftAndRight)
    x -= x_w/2;
  else if (p & AtRight)
    x -= x_w;

  if (p & BetweenTopAndBottom)
    y += (F->ascent - F->descent)/2;
  else if (p & AtTop)
    y += F->ascent;
  else if (p & AtBottom)
    y -= F->descent;


  GKDrawString(TkGate.D,W,gc,x,y,S,strlen(S));
  
  return x_w + x;
}

int dce_DrawString(GC gc,int x,int y,int p,const char *s)
{
  return PosDrawString(TkGate.W,0,gc,ctow_x(x),ctow_y(y),s,p);
}

int RelPosDrawString(Window W,XFontStruct *F,GC gc,int x,int y,const char *S,int p)
{
  int ex = PosDrawString(W,F,gc,ctow_x(x),ctow_y(y),S,p);
  return wtoc_x(ex);
}

/*
 * Lookup a message from the appropriate locality file.
 */
char *msgLookup(const char *tag)
{
  char *s = 0;
  int i;

  for (i = 0;i < 10;i++) {
    s = (char*) SHash_find(message_table,tag);

    if (!s) {
      /*
       * Message not found, generate placeholder message
       */
      char buf[1024];
      sprintf(buf,"<no-msg: %s>",tag);
      s = ob_strdup(buf);
      SHash_insert(message_table,tag,s);
      break;
    } else if (*s == '`') {
      /*
       * Redirection message.  Assume message body points to another message.
       */
      tag = s+1;
    } else
      break;
  }

  if (i == 10) {
    /*
     * Too many levels of indirection
     */
    char buf[1024];
    sprintf(buf,"<recursive: %s>",tag);
    s = ob_strdup(buf);
    SHash_insert(message_table,tag,s);
  }

  return s;
}

/*
 * Character encoding translation code
 */
#if HAVE_ICONV_H
static SHash *iconv_translators = 0;

Encoder *getEncoder(const char *toCode,const char *fromCode)
{
  char key[STRMAX];
  Encoder *encoder = 0;

  /*
   * If either encoding is null, return null encoder.
   */
  if (strcmp(toCode,"null") == 0) return 0;
  if (strcmp(fromCode,"null") == 0) return 0;

  /*
   * Allocate a new translator table
   */
  if (!iconv_translators)
    iconv_translators = new_SHash_noob();
  
  /*
   * Construct key and see if we have a translator.
   */
  sprintf(key,"%s %s",toCode,fromCode);
  encoder = (Encoder*) SHash_find(iconv_translators, key);
  if (encoder)
    return encoder;

  /*
   * Try to create the translator, return 0 (and an error) if we could not.
   */
  encoder = (Encoder*) malloc(sizeof(Encoder));
  encoder->fromCode = strdup(fromCode);
  encoder->toCode = strdup(toCode);
  encoder->ico = iconv_open(toCode,fromCode);
  encoder->isJapanese = (strcmp(encoder->toCode,CE_EUC_JP) == 0);

  SHash_insert(iconv_translators, key, encoder);
  if (encoder->ico == ((iconv_t)-1)) {
    logError(ERL_ERROR,"Unable to create encoding <%s> to <%s>.",fromCode,toCode);
    return 0;
  }
#if LOCALE_DEBUG
  printf("created encoder <%s> to <%s>\n",fromCode,toCode);
#endif

  return encoder;
}

size_t recodeText(Encoder *encoder, char *toString,int len, const char *fromString)
{
  size_t inSize,outSize,result;
  const char *inPtr;
  char *outPtr;

  /*
   * There was no encoder.  Just copy fromString to toString.
   */
  if (!encoder || encoder->ico == ((iconv_t)-1)) {
    strncpy(toString,fromString,len);
    toString[len-1] = 0;
    return 0;
  }

  /*  printf("recoding <%s> to <%s>: %s\n",encoder->fromCode,encoder->toCode,fromString);*/

  inPtr = fromString;
  inSize = strlen(fromString);
  outPtr = toString;
  outSize = len;

  result = iconv(encoder->ico,&inPtr,&inSize,&outPtr,&outSize);
  if (result < 0) {
    strncpy(toString,fromString,len);
    toString[len-1] = 0;
    return -1;
  }

  *outPtr = 0;

  return result;
}

char *recodeTextP(Encoder *encoder, const char *fromString)
{
  char *outString;
  size_t inSize,outSize,result;
  const char *inPtr;
  char *outPtr;
  char *outBuf;
  size_t bufSize;

  /*
   * There was no encoder.  Just copy fromString to toString.
   */
  if (!encoder || encoder->ico == ((iconv_t)-1)) {
    return strdup(fromString);
  }

  /*  printf("recoding <%s> to <%s>: %s\n",encoder->fromCode,encoder->toCode,fromString);*/

  inPtr = fromString;
  inSize = strlen(fromString);
  outSize = inSize+1;
  bufSize = outSize;
  outBuf = (char*)malloc(outSize);
  outPtr = outBuf;

  while (inSize > 0) {
    result = iconv(encoder->ico,&inPtr,&inSize,&outPtr,&outSize);
    if (result < 0) {
      /* Coding error - returning intranslated string */
      return strdup(fromString);
    }
    if (inSize > 0) {
      char *nb = (char*) realloc(outBuf,bufSize+2*inSize+1);
      outPtr = nb + (outPtr-outBuf);
      outBuf = nb;
    }
  }

  *outPtr = 0;

  return outBuf;
}

 
int isJapaneseDisplay(Encoder *encoder)
{
  return encoder && encoder->isJapanese;
}

#else

/*
 * We could not create the encoder, so we will to null-translations.
 */

Encoder *getEncoder(const char *toCode,const char *fromCode)
{
  return 0;
}

size_t recodeText(Encoder *encoder, char *toString,int len, const char *fromString)
{
  strncpy(toString,fromString,len);
  toString[len-1] = 0;
  return 0;
}

int isJapaneseDisplay(Encoder *e)
{
  return 0;
}

#endif
