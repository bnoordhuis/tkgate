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
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "config.h"
#include "yybasic.h"
#include "vgrammar.h"
#include "vparser.h"
#include "ycmalloc.h"

#define STRMAX 8192

extern struct lex_keywordentry lex_verilog_words[];
extern int lex_verilog_num;
extern struct lex_keywordentry lex_anotate_words[];
extern int lex_anotate_num;
extern struct lex_keywordentry lex_delay_words[];
extern int lex_delay_num;

static int default_module_type = HDLMODULE;
static int current_module_type = HDLMODULE;

typedef void *Encoder;

extern Encoder *parserEncoder;

/*****************************************************************************
 *
 * List of all Verilog reserved words.  These are used by the ycIsKW() function
 * to indicate these names should not be used as identifiers in user circuits. 
 *
 *****************************************************************************/
struct lex_keywordentry lex_reserved_words[] = {
  {"always",1},
  {"and",1},
  {"assign",1},
  {"attribute",1},
  {"begin",1},
  {"buf",1},
  {"bufif0",1},
  {"bufif1",1},
  {"case",1},
  {"casex",1},
  {"casez",1},
  {"cmos",1},
  {"deassign",1},
  {"default",1},
  {"defparam",1},
  {"disable",1},
  {"edge",1},
  {"else",1},
  {"end",1},
  {"endattribute",1},
  {"endcase",1},
  {"endfunction",1},
  {"endmodule",1},
  {"endprimitive",1},
  {"endspecify",1},
  {"endtable",1},
  {"endtask",1},
  {"event",1},
  {"for",1},
  {"force",1},
  {"forever",1},
  {"fork",1},
  {"function",1},
  {"highz0",1},
  {"highz1",1},
  {"if",1},
  {"ifnone",1},
  {"initial",1},
  {"inout",1},
  {"input",1},
  {"integer",1},
  {"join",1},
  {"large",1},
  {"medium",1},
  {"module",1},
  {"macromodule",1},
  {"nand",1},
  {"negedge",1},
  {"nmos",1},
  {"nor",1},
  {"not",1},
  {"notif0",1},
  {"notif1",1},
  {"or",1},
  {"output",1},
  {"parameter",1},
  {"pmos",1},
  {"posedge",1},
  {"primitive",1},
  {"pull0",1},
  {"pull1",1},
  {"pulldown",1},
  {"pullup",1},
  {"rcmos",1},
  {"real",1},
  {"realtime",1},
  {"reg",1},
  {"release",1},
  {"repeat",1},
  {"rnmos",1},
  {"rpmos",1},
  {"rtran",1},
  {"rtranif0",1},
  {"rtranif1",1},
  {"scalared",1},
  {"signed",1},
  {"small",1},
  {"specify",1},
  {"specparam",1},
  {"strength",1},
  {"strong0",1},
  {"strong1",1},
  {"supply0",1},
  {"supply1",1},
  {"table",1},
  {"task",1},
  {"time",1},
  {"tran",1},
  {"tranif0",1},
  {"tranif1",1},
  {"tri",1},
  {"tri0",1},
  {"tri1",1},
  {"triand",1},
  {"trior",1},
  {"trireg",1},
  {"unsigned",1},
  {"vectored",1},
  {"wait",1},
  {"wand",1},
  {"weak0",1},
  {"weak1",1},
  {"while",1},
  {"wire",1},
  {"wor",1},
  {"xnor",1},
  {"xor",1},
};

int lex_reserved_num = sizeof(lex_reserved_words)/sizeof(lex_reserved_words[0]);

int nativeVerilog = 0;

int ycCheckHdlLine(char *s)
{
  char *p;

  /*
   * Trim trailing newline.
   */
  p = strrchr(s,'\n');
  if (p) *p = 0;

  if (strstr(s,"//: /builtinEnd") != 0) {
    ycSetModuleType(XMODULE);
    return END;
  }

  if (strstr(s,"//: endscript") != 0) {
    return END;
  }

  /*
   * If this line has a '//: /netlistEnd' or '//: /hdlEnd' on it, treat this as
   * the end of a new-style text segment.  If the line has 'endmodule' on it,
   * treat it as the end of a native verilog module.
   */
  if (nativeVerilog) {
    if (strstr(s,"endmodule") != 0)
      return ENDMODULE;
  } else {
    if (strstr(s,"//: /netlistEnd") != 0 || strstr(s,"//: /hdlEnd") != 0) {
      ycSetModuleType(XMODULE);
      return END;
    }
  }



  yylval.S = yc_strdup(s);
  return HDLLINE;
}


int ycKeyCmp(const char *S1,const char *S2)
{
  for (;*S1;S1++,S2++) {
    int C1 = islower(*S1) ? toupper(*S1) : *S1;
    int C2 = islower(*S2) ? toupper(*S2) : *S2;
    if (C1 != C2) return (C1 < C2) ? -1 : 1;
  }
  return *S2 ? -1 : 0;
}

int ycLookup(char *Tok,struct lex_keywordentry *low,int len)
{
  struct lex_keywordentry *high = low+len-1;
  struct lex_keywordentry *K;

  while (low <= high) {
    K = low + (high-low)/2;
    switch (ycKeyCmp(K->Keyword,Tok)) {
    case  0 :
      return K->Value;
    case -1 :
      low = K + 1;
      break;
    case  1 :
      high = K - 1;
      break;
    }
  }

  return -1;
}

/*****************************************************************************
 *
 *
 *
 *****************************************************************************/
int ycIsKW(char *Tok)
{
  if (ycLookup(Tok,lex_anotate_words,lex_anotate_num) >= 0) return 1;
  if (ycLookup(Tok,lex_verilog_words,lex_verilog_num) >= 0) return 1;
  if (ycLookup(Tok,lex_reserved_words,lex_reserved_num) >= 0) return 1;
  return 0;
}

/*
    Parse a "literal-like" token.  If it is in a keyword table, return the
    token for that keyword, otherwise treat it as a literal.
*/
int ycLiteral(char *Tok,struct lex_keywordentry *low,int len)
{
  int t;

  if (!Tok) Tok = "?";
  yylval.S = "???";

  t = ycLookup(Tok,low,len);
  yylval.S = yc_strdup(Tok);

  /*
   * Special case for module keywords
   */
  switch (t) {
  case XMODULE :
    t = current_module_type;
    break;
  case NETLISTBEGIN :
    ycSetModuleType(NETMODULE);
    break;
  case HDLBEGIN :
    ycSetModuleType(HDLMODULE);
    break;
  case END :
    ycSetModuleType(XMODULE);
    break;
  }

  return t >= 0 ? t : LITERAL;
}

int ycString(char *s)
{
  char buf[STRMAX];
  char buf2[STRMAX];
  char *p;

  /*
   * Copy string and remove quotes.
   */
  strcpy(buf,s+1);
  buf[strlen(buf)-1] = 0;

  /*
   * Remove any '\' characters that may have been used to quote a quote character.
   */
  for (p = buf;*p;p++)
    if (*p == '\\' && p[1]) {
      memmove(p,p+1,strlen(p+1)+1);
      p++;
    }

  /*
   * Translate the string from the file coding system.
   */
  recodeText(parserEncoder, buf2, STRMAX, buf);

  /*
   * Set resulting string
   */
  yylval.S = yc_strdup(buf2);

  return STRING;
}

int yywrap()
{
  return 1;
}

void ycSetDefaultModuleType(int modType)
{
  default_module_type = modType;
  current_module_type = modType;
}

void ycSetModuleType(int modType)
{
  if (modType == XMODULE)
    current_module_type = default_module_type;
  else
    current_module_type = modType;
}

