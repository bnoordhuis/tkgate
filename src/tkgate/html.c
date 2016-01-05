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

    Last edit by hansen on Sun Feb 22 18:12:11 2009
****************************************************************************/
/*

This module implements a subset of html for use in tkgate comments.

The implemented elements are:
  <value-of name="name">			Special variable value
  <a href="url"></a>				Hyperlink
  <a name="tag">				Hyperlink tag
  <b></b>					Bold
  <i></i>					Italic
  <h3></h3>					Heading 3
  <h2></h2>					Heading 2
  <h1></h1>					Heading 1
  <small></small>				Small font
  <big></big>					Big font
  <tt></tt>					Fixed-width font
  <pre></pre>					Preformated
  <br>						Break
  <p>						Paragraph
  <font size=n color=color face=face></font>	Set font characteristics
  <hr>						Horizontal rule
  &??;						Special character

Special pre-processed tags:
  <tutorial-navigation>                         Generates html for navigation bar in tutorials

*/
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdarg.h>
#include "tkgate.h"

//
// Testing only
//
//#include <X11/Xatom.h>




/*
 * Step size for extending the array of html tag options.
 */
#define HTML_OSTEP 8

void expandSpecialDirs(char *file);

typedef void HtmlHandlerFunc(Html *h,HtmlTag *tag);

/*
 * Private class for specifying functions to handle html tags
 */
typedef struct {
  char			*tag;		/* Tag */
  HtmlHandlerFunc	*func;	        /* Handling function for tag */
} HtmlHandler;

typedef struct {
  char			*spec;		/* Special character specifier */
  char			*text;		/* Raw text for replacement */
  char			*pretag;	/* Html code to preceed text */	
  char			*posttag;	/* Html code to follow text */	
} HtmlSpecialSpec;

void Html_handle_basic(Html *h, HtmlTag *tag);
void Html_handle_valueOf(Html *h, HtmlTag *tag);
void Html_handle_a(Html *h, HtmlTag *tag);
void Html_handle_heading(Html *h, HtmlTag *tag);
void Html_handle_img(Html *h, HtmlTag *tag);

/*
 * Table of html tags that are recognized by tkgate
 */
HtmlHandler htmlHandlers[] = {
  {"a",		Html_handle_a },
  {"b",		Html_handle_basic },
  {"tt",	Html_handle_basic },
  {"i",		Html_handle_basic },
  {"big",	Html_handle_basic },
  {"small",	Html_handle_basic },
  {"font",	Html_handle_basic },
  {"h1",	Html_handle_heading },
  {"h2",	Html_handle_heading },
  {"h3",	Html_handle_heading },
  {"value-of",	Html_handle_valueOf },
  {"img",	Html_handle_img },
  {0, 0}
};

/*
 * Table of html special characters that are recognized by tkgate
 */
HtmlSpecialSpec htmlSpecialSpecs[] = {
  {"&nbsp;",	" ",	0,	0},
  {"&gt;",	">",	0,	0},
  {"&lt;",	"<",	0,	0},
  {"&amp;",	"&",	0,	0},


  {"&Alpha;",	"A",	"<font face=symbol>",	"</font>"},
  {"&Beta;",	"B",	"<font face=symbol>",	"</font>"},
  {"&Gamma;",	"G",	"<font face=symbol>",	"</font>"},
  {"&Delta;",	"D",	"<font face=symbol>",	"</font>"},
  {"&Epsilon;",	"E",	"<font face=symbol>",	"</font>"},
  {"&Zeta;",	"Z",	"<font face=symbol>",	"</font>"},
  {"&Eta;",	"H",	"<font face=symbol>",	"</font>"}, 	
  {"&Theta;",	"Q",	"<font face=symbol>",	"</font>"}, 
  {"&Iota;",	"I",	"<font face=symbol>",	"</font>"}, 	
  {"&Kappa;",	"K",	"<font face=symbol>",	"</font>"}, 
  {"&Lambda;",	"L",	"<font face=symbol>",	"</font>"},
  {"&Mu;",	"M",	"<font face=symbol>",	"</font>"}, 	
  {"&Nu;",	"N",	"<font face=symbol>",	"</font>"}, 	
  {"&Xi;",	"X",	"<font face=symbol>",	"</font>"}, 	
  {"&Omicron;",	"O",	"<font face=symbol>",	"</font>"},
  {"&Pi;",	"P",	"<font face=symbol>",	"</font>"}, 	
  {"&Rho;",	"R",	"<font face=symbol>",	"</font>"}, 	
  {"&Sigma;",	"S",	"<font face=symbol>",	"</font>"}, 
  {"&Tau;",	"T",	"<font face=symbol>",	"</font>"}, 	
  {"&Upsilon;",	"U",	"<font face=symbol>",	"</font>"},
  {"&Phi;",	"F",	"<font face=symbol>",	"</font>"}, 	
  {"&Chi;",	"C",	"<font face=symbol>",	"</font>"}, 	
  {"&Psi;",	"Y",	"<font face=symbol>",	"</font>"}, 	
  {"&Omega;",	"W",	"<font face=symbol>",	"</font>"}, 
  {"&alpha;",	"a",	"<font face=symbol>",	"</font>"}, 
  {"&beta;",	"b",	"<font face=symbol>",	"</font>"}, 	
  {"&gamma;",	"g",	"<font face=symbol>",	"</font>"}, 
  {"&delta;",	"d",	"<font face=symbol>",	"</font>"}, 
  {"&epsilon;",	"e",	"<font face=symbol>",	"</font>"},
  {"&zeta;",	"z",	"<font face=symbol>",	"</font>"}, 	
  {"&eta;",	"h",	"<font face=symbol>",	"</font>"}, 	
  {"&theta;",	"q",	"<font face=symbol>",	"</font>"}, 
  {"&iota;",	"i",	"<font face=symbol>",	"</font>"}, 	
  {"&kappa;",	"k",	"<font face=symbol>",	"</font>"}, 
  {"&lambda;",	"l",	"<font face=symbol>",	"</font>"},
  {"&mu;",	"m",	"<font face=symbol>",	"</font>"}, 
  {"&nu;",	"n",	"<font face=symbol>",	"</font>"}, 
  {"&xi;",	"x",	"<font face=symbol>",	"</font>"}, 
  {"&omicron;",	"o",	"<font face=symbol>",	"</font>"}, 
  {"&pi;",	"p",	"<font face=symbol>",	"</font>"}, 	
  {"&rho;",	"r",	"<font face=symbol>",	"</font>"}, 	
  {"&sigmaf;",	"V",	"<font face=symbol>",	"</font>"},
  {"&sigma;",	"s",	"<font face=symbol>",	"</font>"},
  {"&tau;",	"t",	"<font face=symbol>",	"</font>"}, 	
  {"&upsilon;",	"u",	"<font face=symbol>",	"</font>"},
  {"&phi;",	"f",	"<font face=symbol>",	"</font>"}, 	
  {"&chi;",	"c",	"<font face=symbol>",	"</font>"}, 	
  {"&psi;",	"y",	"<font face=symbol>",	"</font>"}, 	
  {"&omega;",	"w",	"<font face=symbol>",	"</font>"},
  {"&thetasym;","J",	"<font face=symbol>",	"</font>"},
  {"&upsih;",	"\241",	"<font face=symbol>",	"</font>"}, 
  {"&piv;",	"v",	"<font face=symbol>",	"</font>"}, 

  {"&bull;",	"\267",	"<font face=symbol>",	"</font>"},
  {"&hellip;",	"\274",	"<font face=symbol>",	"</font>"},
  {"&prime;",	"\242",	"<font face=symbol>",	"</font>"},
  {"&Prime;",	"\262",	"<font face=symbol>",	"</font>"},
  {"&oline;",	"\140",	"<font face=symbol>",	"</font>"},
  {"&frasl;",	"\244",	"<font face=symbol>",	"</font>"},
  {"&weierp;",	"\303",	"<font face=symbol>",	"</font>"},
  {"&image;",	"\301",	"<font face=symbol>",	"</font>"},
  {"&real;",	"\302",	"<font face=symbol>",	"</font>"},
  {"&trade;",	"\324",	"<font face=symbol>",	"</font>"},
  {"&alefsym;",	"\300",	"<font face=symbol>",	"</font>"},
  {"&larr;",	"\254",	"<font face=symbol>",	"</font>"},
  {"&uarr;",	"\255",	"<font face=symbol>",	"</font>"},
  {"&rarr;",	"\256",	"<font face=symbol>",	"</font>"},
  {"&darr;",	"\257",	"<font face=symbol>",	"</font>"},
  {"&harr;",	"\253",	"<font face=symbol>",	"</font>"},
  {"&crarr;",	"\277",	"<font face=symbol>",	"</font>"},
  {"&lArr;",	"\334",	"<font face=symbol>",	"</font>"},
  {"&uArr;",	"\335",	"<font face=symbol>",	"</font>"},
  {"&rArr;",	"\336",	"<font face=symbol>",	"</font>"},
  {"&dArr;",	"\337",	"<font face=symbol>",	"</font>"},
  {"&hArr;",	"\333",	"<font face=symbol>",	"</font>"},
  {"&forall;",	"\042",	"<font face=symbol>",	"</font>"},
  {"&part;",	"\266",	"<font face=symbol>",	"</font>"},
  {"&exist;",	"\044",	"<font face=symbol>",	"</font>"},
  {"&empty;",	"\306",	"<font face=symbol>",	"</font>"},
  {"&nabla;",	"\321",	"<font face=symbol>",	"</font>"},
  {"&isin;",	"\316",	"<font face=symbol>",	"</font>"},
  {"&notin;",	"\317",	"<font face=symbol>",	"</font>"},
  {"&ni;",	"\267",	"<font face=symbol>",	"</font>"},	/* NOT CORRECT */
  {"&prod;",	"\325",	"<font face=symbol>",	"</font>"},
  {"&sum;",	"\345",	"<font face=symbol>",	"</font>"},
  {"&minus;",	"-",	"<font face=symbol>",	"</font>"},
  {"&lowast;",	"*",	"<font face=symbol>",	"</font>"},
  {"&radic;",	"\326",	"<font face=symbol>",	"</font>"},
  {"&prop;",	"\265",	"<font face=symbol>",	"</font>"},
  {"&infin;",	"\245",	"<font face=symbol>",	"</font>"},
  {"&ang;",	"\320",	"<font face=symbol>",	"</font>"},
  {"&and;",	"\331",	"<font face=symbol>",	"</font>"},
  {"&or;",	"\332",	"<font face=symbol>",	"</font>"},
  {"&cap;",	"\307",	"<font face=symbol>",	"</font>"},
  {"&cup;",	"\310",	"<font face=symbol>",	"</font>"},
  {"&int;",	"\362",	"<font face=symbol>",	"</font>"},
  {"&there4;",	"\134",	"<font face=symbol>",	"</font>"},
  {"&sim;",	"\176",	"<font face=symbol>",	"</font>"},
  {"&cong;",	"\100",	"<font face=symbol>",	"</font>"},
  {"&asymp;",	"\273",	"<font face=symbol>",	"</font>"},
  {"&ne;",	"\271",	"<font face=symbol>",	"</font>"},
  {"&equiv;",	"\272",	"<font face=symbol>",	"</font>"},
  {"&le;",	"\243",	"<font face=symbol>",	"</font>"},
  {"&ge;",	"\263",	"<font face=symbol>",	"</font>"},
  {"&sub;",	"\314",	"<font face=symbol>",	"</font>"},
  {"&sup;",	"\311",	"<font face=symbol>",	"</font>"},
  {"&nsub;",	"\313",	"<font face=symbol>",	"</font>"},
  {"&sube;",	"\315",	"<font face=symbol>",	"</font>"},
  {"&supe;",	"\312",	"<font face=symbol>",	"</font>"},
  {"&oplus;",	"\305",	"<font face=symbol>",	"</font>"},
  {"&otimes;",	"\304",	"<font face=symbol>",	"</font>"},
  {"&perp;",	"\267",	"<font face=symbol>",	"</font>"},	/* NOT CORRECT */
  {"&sdot;",	"\327",	"<font face=symbol>",	"</font>"},
  {"&lceil;",	"\351",	"<font face=symbol>",	"</font>"},
  {"&rceil;",	"\371",	"<font face=symbol>",	"</font>"},
  {"&lfloor;",	"\353",	"<font face=symbol>",	"</font>"},
  {"&rfloor;",	"\373",	"<font face=symbol>",	"</font>"},
  {"&lang;",	"\341",	"<font face=symbol>",	"</font>"},
  {"&rang;",	"\361",	"<font face=symbol>",	"</font>"},
  {"&loz;",	"\340",	"<font face=symbol>",	"</font>"},
  {"&spades;",	"\252",	"<font face=symbol>",	"</font>"},
  {"&clubs;",	"\247",	"<font face=symbol>",	"</font>"},
  {"&hearts;",	"\251",	"<font face=symbol>",	"</font>"},
  {"&diams;",	"\250",	"<font face=symbol>",	"</font>"},

  {0,0,0,0}
};


#define DATA_STEP_SIZE			512

static HtmlContext default_context = {
  {FF_HELVETICA, FP_ROMAN, FS_NORMAL},
  0,
  0,
  0,
  1,
  0
};


int istruevalue(const char *s)
{
  int n;

  if (sscanf(s,"%d",&n) == 1) {
    return (n != 0);
  }

  if (strcasecmp(s,"t") == 0) return 1;
  if (strcasecmp(s,"y") == 0) return 1;
  if (strcasecmp(s,"true") == 0) return 1;
  if (strcasecmp(s,"yes") == 0) return 1;

  return 0;
}

static Encoder *Html_getEncoder(Html *h)
{
  if (h->h_target == TD_X11) 
    return Circuit_getDisplayEncoder(TkGate.circuit);
  else
    return Circuit_getPSEncoder(TkGate.circuit);
}

int HtmlFont_isEqual(const HtmlFont *a,const HtmlFont *b)
{
  if (a->family != b->family) return 0;
  if (a->props != b->props) return 0;
  if (a->size != b->size) return 0;
  if (a->points != b->points) return 0;
  return 1;
}

void HtmlFont_updatePoints(HtmlFont *font)
{
  if (font->family == FF_KANJI)
    font->points = getKanjiFontSize(font->size);
  else
    font->points = getFontSize(font->size);
}


HtmlFont *HtmlFont_init(HtmlFont *font,fontfamily_t family,fontprop_t props,fontsize_t points)
{
  int i;

  ob_touch(font);

  font->family = family;
  font->props = props;
  font->points = points;

  if (family != FF_KANJI) {
    for (i = 0;i < FS_MAX;i++)
      if (getFontSize(i) >= points) break;
  } else {
    for (i = 0;i < FS_MAX;i++)
      if (getKanjiFontSize(i) >= points) break;
  }
  if (i >= FS_MAX) i = FS_MAX - 1;
  font->size = i;

  return font;
}


HtmlTag *new_HtmlTag()
{
  HtmlTag *ht = (HtmlTag*) ob_malloc(sizeof(HtmlTag),"HtmlTag");

  ht->ht_name = 0;
  ht->ht_numOptions = 0;
  ht->ht_options = 0;

  return ht;
}

void delete_HtmlTag(HtmlTag *ht)
{
  if (ht->ht_name) ob_free(ht->ht_name);
  if (ht->ht_options) {
    int i;
    
    for (i = 0;i < ht->ht_numOptions;i++) {
      ob_free(ht->ht_options[i].hto_label);
     ob_free(ht->ht_options[i].hto_value);
    }

    ob_free(ht->ht_options);
  }
  ob_free(ht);
}


/*****************************************************************************
 * Break down an options string into tags and values.  Return the
 * number of tags parsed, or -1 if there was an error.
 *****************************************************************************/
HtmlTag *Html_parseTag(const char *tag)
{
  char name[STRMAX];
  char options[STRMAX];
  char ptag[STRMAX],pvalue[STRMAX];
  char *p;
  char c;
  HtmlTag *ht = 0;

  if (sscanf(tag,"< %[^ \t\n>] %[^>] %c",name,options,&c) == 3 && c == '>') {
    /* Tag with options */
  } else if (sscanf(tag,"< %[^ \t\n>] %c",name,&c) == 2 && c == '>') {
    /* Tag with no options */
    *options = 0;
  } else {
    /* Ignore badly formatted tag */
    return 0;
  }

  ht = new_HtmlTag();
  ht->ht_name = ob_strdup(name);

  p = options;
  while (*p && isspace(*p)) p++;
  while (*p) {

    if (sscanf(p,"%[^=] = \"%[^\"]",ptag,pvalue) == 2) {
      /* ok - advance past option */
      while (*p && *p != '=') p++;
      while (*p && *p != '"') p++;
      if (*p) p++;
      while (*p && *p != '"') p++;
      if (*p) p++;
    } else if (sscanf(p,"%[^=] = %s",ptag,pvalue) == 2) {
      /* ok - advance past option */
      while (*p && *p != '=') p++;
      if (*p) p++;
      while (*p && isspace(*p)) p++;
      while (*p && !isspace(*p)) p++;
    } else {
      while (*p && isspace(*p)) p++;		/* Ignore unparsable tag */
      while (*p && !isspace(*p)) p++;
      continue;
    }

    if ((ht->ht_numOptions % HTML_OSTEP) == 0) {
      if (ht->ht_numOptions == 0)
	ht->ht_options = (HtmlTagOpt*) ob_malloc(sizeof(HtmlTagOpt)*HTML_OSTEP,"HtmlTagOpt");
      else
	ht->ht_options = (HtmlTagOpt*) ob_realloc(ht->ht_options,sizeof(HtmlTagOpt)*(ht->ht_numOptions + HTML_OSTEP));
    }

    ht->ht_options[ht->ht_numOptions].hto_label = ob_strdup(ptag);
    ht->ht_options[ht->ht_numOptions].hto_value = ob_strdup(pvalue);
    ht->ht_numOptions++;

    while (*p && isspace(*p)) p++;
  }

  return ht;
}

static int  HtmlContext_stringWidth(HtmlContext *hc,const char *text,int len)
{
  switch (hc->hc_html->h_target) {
  case TD_X11 :
    if (hc->hc_is16bit)
      return XTextWidth16(hc->hc_xFont,(XChar2b*)text,len/2);
    else
      return XTextWidth(hc->hc_xFont,text,len);
  case TD_PRINT :
    return PSStringWidth(&hc->hc_font,text,len);
  }
  return 0;
}

void HtmlFont_print(HtmlFont *hf,FILE *f)
{
  char name[STRMAX];
  getFontName(name,hf->family,hf->props,hf->size,1);
  fprintf(f,"%s",name);
}


static void HtmlContext_activateFont(HtmlContext *hc)
{
  ob_touch(hc);


  switch (hc->hc_html->h_target) {
  case TD_X11 :
#if 0
    printf("%p: activate-x ",hc);HtmlFont_print(&hc->hc_font,stdout);printf("\n");
#endif

    hc->hc_xFont  = GetXFont(hc->hc_font.family, hc->hc_font.props, hc->hc_font.size, TkGate.circuit->zoom_factor);
    hc->hc_is16bit = (hc->hc_font.family == FF_KANJI);

    if (hc->hc_is16bit)
      hc->hc_spaceWidth = XTextWidth16(hc->hc_xFont, (XChar2b*)"  ", 1);
    else
      hc->hc_spaceWidth = XTextWidth(hc->hc_xFont, " ", 1);

    hc->hc_ascent = hc->hc_xFont->ascent;
    hc->hc_descent = hc->hc_xFont->descent;
    break;
  case TD_PRINT :
#if 0
    printf("activate-ps ");HtmlFont_print(&hc->hc_font,stdout);printf("\n");
#endif
    hc->hc_xFont = 0;
    hc->hc_is16bit = (hc->hc_font.family == FF_KANJI);

    if (hc->hc_is16bit)
      hc->hc_spaceWidth = PSStringWidth(&hc->hc_font,"  ",2);
    else
      hc->hc_spaceWidth = PSStringWidth(&hc->hc_font," ",1);

    hc->hc_descent = (int)(hc->hc_font.points*0.2);
    hc->hc_ascent = (int)(hc->hc_font.points*1.2) - hc->hc_descent;
    break;
  }
}

static HtmlContext *new_HtmlContext(HtmlContext *base,Html *html)
{
  HtmlContext *hc = (HtmlContext*) ob_malloc(sizeof(HtmlContext),"HtmlContext");

  //  printf("%p: new_HtmlContext()\n",hc);

  if (base)
    *hc = *base;
  else {
    *hc = default_context;
    hc->hc_pixel = TkGate.comment_pixel;
  }

  hc->hc_html = html;

  if (hc->hc_link) hc->hc_link = ob_strdup(hc->hc_link);
  if (hc->hc_tag)  hc->hc_tag = ob_strdup(hc->hc_tag);
  hc->hc_next = 0;
  
  HtmlFont_updatePoints(&hc->hc_font);
  HtmlContext_activateFont(hc);

  return hc;
}

static void delete_HtmlContext(HtmlContext *hc)
{
  if (hc->hc_link) ob_free(hc->hc_link);
  if (hc->hc_tag) ob_free(hc->hc_tag);
}

static HtmlUnit *new_HtmlUnit(const char *text,int len,HtmlContext *hc)
{
  HtmlUnit *hu = ob_malloc(sizeof(HtmlUnit),"HtmlUnit");

  hu->hu_type = HU_TEXT;
  hu->hu_text = ob_malloc(len+1,"char*");
  strncpy(hu->hu_text,text,len);
  hu->hu_text[len] =0;
  hu->hu_x = hu->hu_y = 0;
  hu->hu_context = hc;
  hu->hu_image = 0;

  return hu;
}

static HtmlUnit *new_HtmlUnit_T(int htype,HtmlContext *hc)
{
  HtmlUnit *hu = ob_malloc(sizeof(HtmlUnit),"HtmlUnit");

  hu->hu_type = htype;
  hu->hu_text =0;
  hu->hu_x = hu->hu_y = 0;
  hu->hu_context = hc;
  hu->hu_image = 0;

  return hu;
}

static void delete_HtmlUnit(HtmlUnit *hu)
{
  if (hu->hu_image)
    Tk_FreeImage(hu->hu_image);

  if (hu->hu_text)
    ob_free(hu->hu_text);
  ob_free(hu);
}

Html *new_Html(TargetDev_e target)
{
  Html *h = (Html*) ob_malloc(sizeof(Html),"Html");

  h->h_reqWidth = 100;
  h->h_width = h->h_reqWidth;
  h->h_target = target;
  h->h_height = 0;
  h->h_dataLen = 0;
  h->h_data = 0;
  h->h_head = 0;
  h->h_tail = 0;
  h->h_context = new_HtmlContext(0,h);
  h->h_contextPool = 0;
  h->h_zoom = TkGate.circuit->zoom_factor;
  h->h_locale = TkGate.circuit->c_locale;

  return h;
}

void delete_Html(Html *h)
{
  HtmlUnit *u,*next_u;
  HtmlContext *hc,*next_hc;

  if (h->h_data) ob_free(h->h_data);

  for (u = h->h_head;u;u = next_u) {
    next_u = u->hu_next;
    delete_HtmlUnit(u);
  }

  for (hc = h->h_context;hc;hc = next_hc) {
    next_hc = hc->hc_next;
    delete_HtmlContext(hc);
  }
  for (hc = h->h_contextPool;hc;hc = next_hc) {
    next_hc = hc->hc_next;
    delete_HtmlContext(hc);
  }


  ob_free(h);
}

const char *Html_makeTutorialNavigationLine_bymodule(char *line)
{
  char *p = line;
  GModuleDef *M = TkGate.circuit->es->env;
  int cur_pnum,max_pnum,i;
  int spaces = 45;

  for (max_pnum = 0;;) {
    char name[STRMAX];

    sprintf(name,"PAGE%d",max_pnum+1);
    if (!env_findModule(name)) break;
    max_pnum++;
  }

  if (max_pnum > 1) {
    if (sscanf(M->m_name,"PAGE%d",&cur_pnum) != 1 || cur_pnum < 1 || cur_pnum > max_pnum)
      return "[no-controls]";
  
    if (cur_pnum == 1) {
      int i;

      p += sprintf(p,"<font color=gray>&lt;%s</font>   ",msgLookup("tutorial.prev"));
    } else {
      p += sprintf(p,"<a href=\"#/PAGE%d\">&lt;%s</a>   ",cur_pnum-1,msgLookup("tutorial.prev"));
    }

    for (i = 1;i <= max_pnum;i++) {
      if (i == cur_pnum)
	p += sprintf(p," <b size=5>%d</b>",i);
      else
	p += sprintf(p," <a href=\"#/PAGE%d\">%d</b>",i,i);

      if (i < 10)
	spaces -= 2;
      else
	spaces -= 3;
    }

    if (cur_pnum == max_pnum)
      p += sprintf(p,"    <font color=gray>%s></font>",msgLookup("tutorial.next"));
    else
      p += sprintf(p,"    <a href=\"#/PAGE%d\">%s></a>",cur_pnum+1,msgLookup("tutorial.next"));

    for (i = 0;i < spaces;i++)
      p += sprintf(p," ");

    p += sprintf(p,"   ");
  }

  p += sprintf(p,"<a href=\"index.v\">&lang;%s&rang;</a>     <a href=\"%s#/PAGE%d\">&lang;%s&rang;</a>",
	       msgLookup("tutorial.chapter"),
	       CurrentFile_path(TkGate.circuit->currentFile),cur_pnum,
	       msgLookup("tutorial.reload"));

  return line;
}

const char *Html_makeTutorialNavigationLine_byfile(char *line)
{
  char *p = line;
  int cur_pnum,max_pnum,i;
  int spaces = 45;
  const char *curDirName = CurrentFile_getDir(TkGate.circuit->currentFile);
  const char *curFileName = CurrentFile_getBase(TkGate.circuit->currentFile);
  char baseName[STRMAX], extension[STRMAX], path[STRMAX];

  if (sscanf(curFileName,"%[^0123456789]%d%s",baseName,&cur_pnum,extension) != 3 || strcmp(extension,".v") != 0)
    return "[no-controls - invalid file name]";

#if 1
  sprintf(path,"%s/%s",curDirName,baseName);
#endif

  for (max_pnum = 0;;) {
    char name[STRMAX];
    struct stat sb;

    sprintf(name,"%s%d.v",path,max_pnum+1);

    if (stat(name, &sb) != 0) break;

    max_pnum++;
  }

  if (max_pnum > 1) {
    if (cur_pnum == 1) {
      p += sprintf(p,"<font color=gray>&lt;%s</font>   ",msgLookup("tutorial.prev"));
    }  else
      p += sprintf(p,"<a href=\"%s%d.v\">&lt;%s</a>   ",baseName,cur_pnum-1,msgLookup("tutorial.prev"));

    for (i = 1;i <= max_pnum;i++) {
      if (i == cur_pnum)
	p += sprintf(p," <b size=5>%d</b>",i);
      else
	p += sprintf(p," <a href=\"%s%d.v\">%d</b>",baseName,i,i);

      if (i < 10)
	spaces -= 2;
      else
	spaces -= 3;
    }

    if (cur_pnum == max_pnum)
      p += sprintf(p,"    <font color=gray>%s></font>",msgLookup("tutorial.next"));
   else
      p += sprintf(p,"    <a href=\"%s%d.v\">%s></a>",baseName,cur_pnum+1,msgLookup("tutorial.next"));

    for (i = 0;i < spaces;i++)
      p += sprintf(p," ");

    p += sprintf(p,"   ");
  }

  p += sprintf(p,"<a href=\"index.v\">&lang;%s&rang;</a>     <a href=\"%s%d.v\">&lang;%s&rang;</a>",
	       msgLookup("tutorial.chapter"),
	       baseName,cur_pnum,
	       msgLookup("tutorial.reload"));

  return line;
}

/*****************************************************************************
 *
 * Generate the HTML resulting from the <tutorial-navigation> tag.
 *
 *****************************************************************************/
const char *Html_makeTutorialNavigationLine(Html *h, HtmlTag *navtag)
{
  Encoder *encoder = Html_getEncoder(h);
  static char line[STRMAX];
  char buf[STRMAX];
  int byfile = 0;
  int i;

  //  *buf = 0;

  for (i = 0;i < navtag->ht_numOptions;i++) {
    if (strcasecmp(navtag->ht_options[i].hto_label, "byfile") == 0)
      byfile = istruevalue(navtag->ht_options[i].hto_value);
  }

  if (byfile)
    Html_makeTutorialNavigationLine_byfile(buf);
  else
    Html_makeTutorialNavigationLine_bymodule(buf);

  recodeText(encoder, line, STRMAX, buf);

  return line;
}

/******************************************************************************
 *
 * Add a line of data to the html object
 *
 *****************************************************************************/
void Html_addLine(Html *h,const char *line)
{
  char buf[STRMAX];
  /* Current # of allocated bytes */
  int curLen = ((h->h_dataLen+DATA_STEP_SIZE-1)/DATA_STEP_SIZE)*DATA_STEP_SIZE;
  int l;
  Encoder *encoder = Html_getEncoder(h);

  ob_touch(h);

  recodeText(encoder, buf, STRMAX, line);
  line = buf;

  //
  // This is a kludge to automatically create the tutorial navigation buttons
  //
  if (strncasecmp(line,"<tutorial-navigation",20) == 0) {
    HtmlTag *navtag = Html_parseTag(line);

    line = Html_makeTutorialNavigationLine(h,navtag);
  }

  l = strlen(line);


  /*
   * Check to see if we need to increase length of data buffer.  Be sure to 
   * make space for the newline and the null character.
   */
  if (h->h_dataLen + l + 2 > curLen) {
    int newLen = ((h->h_dataLen + l + 2 + DATA_STEP_SIZE-1)/DATA_STEP_SIZE)*DATA_STEP_SIZE;

    if (h->h_data) {
        h->h_data = ob_realloc(h->h_data,newLen);
     } else
       h->h_data = ob_malloc(newLen,"char*");
  }

  /*
   * Append the data plus a newline.
   */
  strcpy(h->h_data + h->h_dataLen,line);
  h->h_dataLen += l;
  h->h_data[h->h_dataLen++] = '\n';
  h->h_data[h->h_dataLen] = 0;
}

/******************************************************************************
 *
 * Delete all html units from an html object.
 *
 *****************************************************************************/
static void Html_flushUnits(Html *h)
{
  HtmlUnit *hu,*next_hu;

  ob_touch(h);

  for (hu = h->h_head;hu;hu = next_hu) {
    next_hu = hu->hu_next;
    delete_HtmlUnit(hu);
  }
  h->h_head = h->h_tail = 0;
}

static void Html_addUnit(Html *h,HtmlUnit *hu)
{
  ob_touch(h);

  if (h->h_tail) {
    h->h_tail->hu_next = hu;
    hu->hu_prev = h->h_tail;
    h->h_tail = hu;
    hu->hu_next = 0;
  } else {
    h->h_head = h->h_tail = hu;
    hu->hu_prev = 0;
    hu->hu_next = 0;
  }
}

static void Html_pushContext(Html *h,HtmlContext *hc)
{
  ob_touch(hc);
  ob_touch(h);

  hc->hc_next = h->h_context;
  h->h_context = hc;
}

static void Html_popContext(Html *h)
{
  HtmlContext *hc = h->h_context;

  if (!hc->hc_next) return;		/* Don't pop last context */

  ob_touch(hc);
  ob_touch(h);

  h->h_context = h->h_context->hc_next;  

  hc->hc_next = h->h_contextPool;
  h->h_contextPool = hc;
}

/*****************************************************************************
 *
 * Handle a <img> element.
 *
 *****************************************************************************/
void Html_handle_img(Html *h, HtmlTag *tag)
{
  HtmlContext *hc = new_HtmlContext(h->h_context,h);
  HtmlUnit *hu = new_HtmlUnit_T(HU_IMAGE,hc);
  int width = 16, height = 16;
  const char *gifFile = "blk_copy.gif";
  int i;

  hc->hc_pixel = -1;

  ob_touch(hu);

  for (i = 0;i < tag->ht_numOptions;i++) {
    if (strcasecmp(tag->ht_options[i].hto_label, "src") == 0) {
      char buf[STRMAX];

      strcpy(buf,tag->ht_options[i].hto_value);
      expandSpecialDirs(buf);
      gifFile = ob_strdup(buf);
    } else if (strcasecmp(tag->ht_options[i].hto_label, "bgcolor") == 0) {
      ob_touch(hc);
      hc->hc_pixel = Tkg_GetColor(tag->ht_options[i].hto_value);
    }
  }

  Html_pushContext(h,hc);
  Html_popContext(h);
  Html_addUnit(h,hu);

  ob_touch(hu);
  ob_touch(hc);

  DoTcl("gifI %s",gifFile);
  hu->hu_image = Tk_GetImage(TkGate.tcl, Tk_MainWindow(TkGate.tcl), TkGate.tcl->result, 0, 0);
  if (hu->hu_image)
    Tk_SizeOfImage(hu->hu_image, &width, &height);

  hu->hu_text = 0;
  hu->hu_width = width;

  hc->hc_descent = h->h_context->hc_descent;
  hc->hc_ascent = height - hc->hc_descent;
}

/*****************************************************************************
 *
 * Handle a <value-of> element.
 *
 *****************************************************************************/
void Html_handle_valueOf(Html *h, HtmlTag *tag)
{
  extern char *release_date;
  char *name = 0;
  const char *text = 0;
  int i;

  for (i = 0;i < tag->ht_numOptions;i++)
    if (strcasecmp(tag->ht_options[i].hto_label,"name") == 0)
      name = tag->ht_options[i].hto_value;

  if (!name) return;

  if (strcasecmp(name,"tkgate-version") == 0) {
    text = TKGATE_FULL_VERSION;
  } else if (strcasecmp(name,"tkgate-homepage") == 0) {
    text = TKGATE_HOMEPAGE;
  } else if (strcasecmp(name,"tkgate-mailcontact") == 0) {
    text = TKGATE_MAILCONTACT;
  } else if (strcasecmp(name,"tkgate-copyright") == 0) {
    text = TKGATE_COPYRIGHT;
  } else if (strcasecmp(name,"tkgate-release-date") == 0) {
    text = release_date;
  } else if (strcasecmp(name,"pancake-recipe") == 0) {
    extern const char *recipe_list[];
    //text = recipe_list[0];
  }

  if (text)
    Html_addUnit(h,new_HtmlUnit(text,strlen(text),h->h_context));
}

/*****************************************************************************
 *
 * Handle font modifiers in a tag
 *
 *****************************************************************************/
void HtmlContext_handle_modifiers(HtmlContext *hc,HtmlTag *tag)
{
  int i;

  for (i = 0;i < tag->ht_numOptions;i++) {
    const char *label = tag->ht_options[i].hto_label;
    const char *value = tag->ht_options[i].hto_value;

    if (strcasecmp(label,"face") == 0) {
      fontfamily_t j;

      ob_touch(hc);
      
      for (j = 0;j < FF_MAX;j++)
	if (strcasecmp(getFontFamilyName(j),value) == 0)
	  hc->hc_font.family = j;
    } else if (strcasecmp(label,"size") == 0) {
      /*
       * Scan font size.  We decrement the scanned size to convert from the
       * html specification to our internal size code.
       */
      if (sscanf(value,"%d",&hc->hc_font.size) == 1) hc->hc_font.size--;
      HtmlFont_updatePoints(&hc->hc_font);
    } else if (strcasecmp(label,"color") == 0) {
      ob_touch(hc);
      if (hc->hc_html->h_target == TD_X11)
	hc->hc_pixel = Tkg_GetColor(value);
      else
	hc->hc_pixel = 0;
    }
  }
}


/*****************************************************************************
 *
 * Handler for basic open/close html tags which modify the context.
 *
 *****************************************************************************/
void Html_handle_basic(Html *h, HtmlTag *tag)
{
  HtmlContext *hc = new_HtmlContext(h->h_context,h);

  ob_touch(hc);
  if (strcasecmp(tag->ht_name,"b") == 0) {
    hc->hc_font.props |= FP_BOLD;
    HtmlContext_handle_modifiers(hc,tag);
  } else if (strcasecmp(tag->ht_name,"i") == 0) {
    hc->hc_font.props |= FP_ITALIC;
    HtmlContext_handle_modifiers(hc,tag);
  } else if (strcasecmp(tag->ht_name,"tt") == 0) {
    hc->hc_font.family = FF_COURIER;
    HtmlContext_handle_modifiers(hc,tag);
  } else if (strcasecmp(tag->ht_name,"big") == 0) {
    if (hc->hc_font.size + 1  < FS_MAX)
      hc->hc_font.size++;
    HtmlFont_updatePoints(&hc->hc_font);
    HtmlContext_handle_modifiers(hc,tag);
  } else if (strcasecmp(tag->ht_name,"small") == 0) {
    if (hc->hc_font.size - 1  >= 0)
      hc->hc_font.size--;
    HtmlFont_updatePoints(&hc->hc_font);
    HtmlContext_handle_modifiers(hc,tag);
  } else if (strcasecmp(tag->ht_name,"font") == 0) {
    HtmlContext_handle_modifiers(hc,tag);
  }

  HtmlContext_activateFont(hc);
  Html_pushContext(h,hc);
}

/*****************************************************************************
 *
 * Handler for heading tags
 *
 *****************************************************************************/
void Html_handle_heading(Html *h, HtmlTag *tag)
{
  HtmlContext *hc = new_HtmlContext(h->h_context,h);

  Html_addUnit(h,new_HtmlUnit_T(HU_BREAK,h->h_context));

  ob_touch(hc);

  hc->hc_font.props |= FP_BOLD;

  if (strcasecmp(tag->ht_name,"h1") == 0) {
    hc->hc_font.size = FS_XHUGE;
  } else if (strcasecmp(tag->ht_name,"h2") == 0) {
    hc->hc_font.size = FS_HUGE;
  } else if (strcasecmp(tag->ht_name,"h3") == 0) {
    hc->hc_font.size = FS_LARGE;
  }

  HtmlFont_updatePoints(&hc->hc_font);

  HtmlContext_handle_modifiers(hc,tag);

  HtmlContext_activateFont(hc);
  Html_pushContext(h,hc);
}

void Html_handle_a(Html *h, HtmlTag *ht)
{
  HtmlContext *hc = new_HtmlContext(h->h_context,h);
  int i;

  ob_touch(hc);
  hc->hc_pixel = TkGate.hyperlink_pixel;
  HtmlContext_activateFont(hc);

  for (i = 0;i < ht->ht_numOptions;i++) {
    if (strcasecmp(ht->ht_options[i].hto_label, "href") == 0) {
      hc->hc_link = ob_strdup(ht->ht_options[i].hto_value);
    } else if (strcasecmp(ht->ht_options[i].hto_label, "name") == 0) {
      hc->hc_tag = ob_strdup(ht->ht_options[i].hto_value);
    }
  }


  Html_pushContext(h,hc);
}

/*****************************************************************************
 *
 * Process the html tag that starts at 'tag' and is 'len' characters long.
 *
 *****************************************************************************/
static void Html_processTag(Html *h, const char *tag,int len)
{
  HtmlTag *ht;
  int i;

  ht = Html_parseTag(tag);
  if (!ht) return;


  if (*ht->ht_name == '/') {
    Html_popContext(h);
    return;
  }

  for (i = 0;htmlHandlers[i].tag;i++)
    if (strcasecmp(ht->ht_name,htmlHandlers[i].tag) == 0)
      (*htmlHandlers[i].func)(h,ht);
}

/*****************************************************************************
 *
 * Process character entity references.
 *
 *****************************************************************************/
static void Html_processSpecialChar(Html *h,char *spec,int len)
{
  char *text = 0;
  char *pretag = 0;
  char *posttag = 0;
  int i;

  for (i = 0;htmlSpecialSpecs[i].spec;i++) {
    if (strncmp(spec,htmlSpecialSpecs[i].spec,len) == 0) {
      text = htmlSpecialSpecs[i].text;
      pretag = htmlSpecialSpecs[i].pretag;
      posttag = htmlSpecialSpecs[i].posttag;
    }
  }

  if (text) {
    if (pretag) Html_processTag(h,pretag,strlen(pretag));
    Html_addUnit(h,new_HtmlUnit(text,strlen(text),h->h_context));
    if (posttag) Html_processTag(h,posttag,strlen(posttag));
  } else
    Html_addUnit(h,new_HtmlUnit(spec,len,h->h_context));
}


/******************************************************************************
 *
 * Parition an html object into pieces.  Each piece is one of:
 *   * An html tag
 *   * An html special character sequence
 *   * A new line
 *   * A text string
 *
 *****************************************************************************/
void Html_partition(Html *h,char *data)
{
  char *p,*q;
  Encoder *encoder = Html_getEncoder(h);
  int isJapanese = isJapaneseDisplay(encoder);

  Html_flushUnits(h);

  if (!data) return;				/* No data */

  p = q = data;
  while (*p) {
    if (*p == '<') {
      /******************************************************************
       *
       * This is an html tag.  Include everything up to end of tag.
       *
       ******************************************************************/
      q = strchr(p,'>');
      if (!q) q = p + strlen(p);

      Html_processTag(h,p,q-p+1);
      if (!*q) break;
      p = q+1;
    } else if (*p == '&') {
      /******************************************************************
       *
       * This is special character.  Parse until ';' and convert to the
       * specified character.
       *
       ******************************************************************/
      q = strchr(p,';');
      if (!q) q = p + strlen(p);

      Html_processSpecialChar(h,p,q-p+1);
      if (!*q) break;
      p = q+1;
    } else if (*p == '\n') {
      /******************************************************************
       *
       * This is newline.  Insert a newline unit if we are in preformat
       * mode, otherwise just insert a space.
       *
       ******************************************************************/
      if (h->h_context->hc_preformat) {
	HtmlUnit *hu = new_HtmlUnit_T(HU_NEWLINE,h->h_context);
	Html_addUnit(h,hu);
      } else
	Html_addUnit(h,new_HtmlUnit(" ",1,h->h_context));
      p++;
    } else if ((*p & 0x80) && isJapanese) {
      /******************************************************************
       *
       * This is Kanji text.  Scan until we find a non-8-bit character.
       *
       ******************************************************************/
      HtmlContext *hc;
      HtmlUnit *hu;
      int i;

      for (q = p;*q;q++)
	if (!(*q & 0x80))
	  break;


      hc = new_HtmlContext(h->h_context,h);
      hc->hc_font.family = FF_KANJI;
      HtmlFont_updatePoints(&hc->hc_font);
      HtmlContext_activateFont(hc);
      Html_pushContext(h,hc);
      Html_popContext(h);
      hu = new_HtmlUnit(p,q-p,hc);
      Html_addUnit(h,hu);
      for (i = 0;hu->hu_text[i];i++) hu->hu_text[i] &= 0x7f;

      if (!*q) break;
      p = q;
    } else {
      /******************************************************************
       *
       * This is a regular non-kanji text string
       *
       ******************************************************************/
      HtmlUnit *hu;

      for (q = p;*q;q++)
	if (strchr("<&\n",*q) != 0 || ((*q & 0x80) && isJapanese))
	  break;
      hu = new_HtmlUnit(p,q-p,h->h_context);
      Html_addUnit(h,hu);

      if (!*q) break;
      p = q;
    }
  }
}

/******************************************************************************
 *
 * Format an html object.
 *
 * Parameters:
 *     h		Html object to be formatted.
 *
 *****************************************************************************/
void Html_format(Html *h)
{
  HtmlUnit *hu,*hu2;
  HtmlUnit *linestart_hu;
  int max_ascent = 0;
  int max_descent = 0;
  int max_width = 0;
  int x = 0, y = 0;					/* Current text position */
  char *p;


  ob_touch(h);
  Html_partition(h,h->h_data);

  h->h_isVisible = 0;

  linestart_hu = h->h_head;				/* This is the text unit for the current line */
  for (hu = h->h_head;hu;hu = hu->hu_next) {
    HtmlContext *hc = hu->hu_context;			/* Get context of this unit */

    /*
     * Update line height metrics
     */
    if (HtmlContext_fontAscent(hc) > max_ascent)
      max_ascent = HtmlContext_fontAscent(hc);
    if (HtmlContext_fontDescent(hc) > max_descent)
      max_descent = HtmlContext_fontDescent(hc);

    switch (hu->hu_type) {
    case HU_BREAK :
      if (x == 0) break;		/* Do a newline only if we are not already on a newline */
      /* fall through */
    case HU_NEWLINE :
      y += max_ascent;
      for (hu2 = linestart_hu;hu2 != hu;hu2 = hu2->hu_next) {
	ob_touch(hu2);
	hu2->hu_y = y;
      }
      y += max_descent;
      x = 0;

      /*
       * Reset metrics and advance linestart_hu to start of next line.
       */
      max_ascent = 0;
      max_descent = 0;
      linestart_hu = hu->hu_next;
      break;
    case HU_TEXT :
      ob_touch(hu);

      hu->hu_x = x;

      hu->hu_width = HtmlContext_stringWidth(hc,hu->hu_text,strlen(hu->hu_text));

      x += hu->hu_width;
      if (x > max_width) max_width = x;

      if (!h->h_isVisible) {
	for (p = hu->hu_text;*p;p++)
	  if (!isspace(*p)) {
	    h->h_isVisible = 1;
	  }
      }
      break;
    case HU_IMAGE :
      hu->hu_x = x;
      x += hu->hu_width;
      if (x > max_width) max_width = x;
      h->h_isVisible = 1;
      break;
    }
  }

  h->h_width = max_width;
  h->h_height = y + max_descent;
}

void Html_psPrint(Html *h,GPrint *P,int x,int y)
{
  HtmlUnit *hu;
  HtmlContext *last_hc = 0;
  Encoder *encoder = Circuit_getPSEncoder(TkGate.circuit);
  char text[STRMAX];

  for (hu = h->h_head;hu;hu = hu->hu_next) {
    HtmlContext *hc = hu->hu_context;			/* Get context of this unit */

    /*
     * Update properties only if there was a change.
     */
    if (hc != last_hc) {
      // Tkg_changeColor(gc, GXxor, hc->hc_pixel);
      last_hc = hc;
    }

    switch (hu->hu_type) {
    case HU_TEXT :
      PSDrawText(P,&hc->hc_font,hu->hu_x + x,hu->hu_y + y,hu->hu_text,AtBaseline|AtLeft);
      break;
    case HU_IMAGE :
      break;
    default :
      break;
    }
  }
}

/*****************************************************************************
 *
 * Draw a block of html text.
 *
 *****************************************************************************/
void Html_draw(Html *h,int x,int y)
{
  GC gc = TkGate.commentGC;
  GC igc = TkGate.imageGC;
  HtmlUnit *hu;
  HtmlContext *last_hc = 0;

  x = ctow_x(x)*TkGate.circuit->zoom_factor;
  y = ctow_y(y)*TkGate.circuit->zoom_factor;

  for (hu = h->h_head;hu;hu = hu->hu_next) {
    HtmlContext *hc = hu->hu_context;			/* Get context of this unit */

    switch (hu->hu_type) {
    case HU_TEXT :
      /*
       * Update properties only if there was a change.
       */
      if (hc != last_hc) {
	XSetFont(TkGate.D,gc,hc->hc_xFont->fid);

	if (hc->hc_pixel >= 0)
	  Tkg_changeColor(gc, GXxor, hc->hc_pixel);
	last_hc = hc;
      }

      if (hc->hc_font.family == FF_KANJI) {
	XDrawString16(TkGate.D,TkGate.W,gc,hu->hu_x + x,hu->hu_y + y,
		      (XChar2b*)hu->hu_text,strlen(hu->hu_text)/2);

      } else {
	XDrawString(TkGate.D,TkGate.W,gc,hu->hu_x + x,hu->hu_y + y,
		    hu->hu_text,strlen(hu->hu_text));
      }
      
      break;
    case HU_IMAGE :
      {
	int width = hu->hu_width;
	int height = HtmlContext_fontAscent(hc) + HtmlContext_fontDescent(hc);
	int base_x = hu->hu_x + x;
	int base_y = hu->hu_y + y - HtmlContext_fontAscent(hc);

	if (hc->hc_pixel >= 0) {
	  XSetForeground(TkGate.D,igc,hc->hc_pixel);
	  ZFillRectangle(TkGate.D,TkGate.W,igc, base_x,base_y, width, height);
	} else
	  XSetForeground(TkGate.D,igc,XWhitePixelOfScreen(TkGate.S));
	Tk_RedrawImage(hu->hu_image, 0, 0, width, height, TkGate.W, base_x, base_y);
      }
      break;
    default :
      break;
    }
  }
}

int Html_isHit(Html *h,int x,int y)
{
  HtmlUnit *hu;

  for (hu = h->h_head;hu;hu = hu->hu_next) {
    HtmlContext *hc = hu->hu_context;			/* Get context of this unit */

    if (x >= hu->hu_x && x <= (hu->hu_x + hu->hu_width)
	&& y <= (hu->hu_y + HtmlContext_fontDescent(hc)) && y >= (hu->hu_y - HtmlContext_fontAscent(hc))) {

      return 1;
    }
  }

  return 0;
}

const char *Html_getLink(Html *h,int x,int y)
{
  HtmlUnit *hu;

  for (hu = h->h_head;hu;hu = hu->hu_next) {
    HtmlContext *hc = hu->hu_context;			/* Get context of this unit */

    if (!hc->hc_link) continue;

    if (hu->hu_type == HU_IMAGE) {
      int width = hu->hu_width;
      int height = HtmlContext_fontAscent(hc) + HtmlContext_fontDescent(hc);
      int base_x = hu->hu_x;
      int base_y = hu->hu_y - HtmlContext_fontAscent(hc);

      if (x >= base_x && x <= (base_x + width)
	  && y >= base_y && y <= (base_y + height)) {

	return hc->hc_link;
      }
    } else {
      if (x >= hu->hu_x && x <= (hu->hu_x + hu->hu_width)
	  && y <= (hu->hu_y + HtmlContext_fontDescent(hc)) && y >= (hu->hu_y - HtmlContext_fontAscent(hc))) {

	return hc->hc_link;
      }
    }
  }

  return 0;
}

