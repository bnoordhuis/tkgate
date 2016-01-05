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

    Last edit by hansen on Wed Jan 28 08:41:34 2009
****************************************************************************/

/*****************************************************************************
 * Postscript Generator for TkGate.
 *
 * This file contains the basic functions for generating postscript files
 * for circuit descriptions.  Some of the most important data structures
 * use by the postscript generator are:
 *
 * GPrint	Contains the context for the generation of a single document.
 *		This includes where the document should be printed, the current
 *		document data, and the any options settings.
 *
 * GPrintOpt	Data structure with option values (in string format) used to create
 *		the GPrint object.  This data structure is normally filled in
 *  		based on the values in the print dialog box.
 *
 * GModLayout	This data structure represents a module with layout information.
 *		The layout information determines where on the page the module
 *		will be printed.
 *
 * GPage	This data structure represents a page.
 *
 *****************************************************************************/


#include "tkgate.h"
#include <sys/time.h>
#include <math.h>

/*
 * Font change codes
 */
#define FCC_BEGINKANJI	0201
#define FCC_ENDKANJI	0201

/*
 * Hierarchy graph constants
 */
#define HG_FONTSIZE	12	/* Font size for heirarchy graph */
#define HG_ROOTX	50	/* Root x position */
#define HG_ROOTY	475	/* Root y position */
#define HG_BOTTOMY	50	/* Lowest y position */
#define HG_INDENT	20	/* Amount to indent for children */
#define HG_LINE		25	/* Hieght of a line */
#define HG_LINEINDENT	10	/* Indent for heirarchy line */
#define HG_BOXHEIGHT	20	/* Height of the module name box */
#define HG_NEXTCOLUMN	225	/* Space between columns */
/*

Folio          8.27 x 13 in.
Statement      5.5 x 8.5
C0		917mm  x 1297mm
C5		162 x 229 mm
C10		28 x 40 mm

A		8.5 x 11
B		11 x 17
C		17 x 22
D		22 x 34
E		34 x 44

yotsugiri	10 x 12in
mutsugiri	8 x 10 in
yatsugiri	6.5 x 8.5 in

kiku 4		227 x 306 mm
kiku 5		151 x 227 mm
 */

/*****************************************************************************
 *
 * Postscript font name and metrics data from metrics.c
 *
 *****************************************************************************/
extern short fontmetrics_Courier;
extern short fontmetrics_Kanji;
extern short *psFontWidthTable[FF_MAX][FP_MAX];
extern char *psFontNameTable[FF_MAX][FP_MAX];

/*****************************************************************************
 *
 * Font to use for displaying hdl module.
 *
 *****************************************************************************/
HtmlFont hdl_font = {FF_COURIER,FP_ROMAN,0,10};


typedef struct hgnode HGNode;
struct hgnode {
  GModuleDef	*hg_module;		/* Module this node corresponds to */
  int		hg_maxDepth;		/* Maximum depth of tree */
  int		hg_expanded;		/* Non-zero if graph is expanded at this point */
  HGNode	*hg_nextSibling;	/* Next node in sibling list */
  HGNode	*hg_children;		/* Children of this node */
};


char *gateprolog[] = {
#include "psprolog.h"
  0
};

char *gateps_copyright[] = {
  "%",
  "% Copyright (C) 1987-2005 by Jeffery P. Hansen",
  "%    This program (the Postscript Prolog) is free software; you can redistribute",
  "%    it and/or modify it under the terms of the GNU General Public License",
  "%    as published by the Free Software Foundation; either version 2 of the",
  "%    License, or (at your option) any later version.",
  "%",
  "%    This program is distributed in the hope that it will be useful,",
  "%    but WITHOUT ANY WARRANTY; without even the implied warranty of",
  "%    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the",
  "%    GNU General Public License for more details.",
  "%",
  "%    You should have received a copy of the GNU General Public License",
  "%    along with this program; if not, write to the Free Software",
  "%    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.",
  "%",
  0
};

char *kanji_support_prolog[] = {
  "%",
  "% Replacement for regular show that switches between current font",
  "% and kanji font each time a special 'kanji swap' character is encountered",
  "%",
  "/kanji_show {",
  "  FCCToggleKanji search {",
  "    orig_show ",
  "    pop",
  "    in_kanji_text {",
  "      /in_kanji_text false def",
  "      swap_font setfont",
  "    } {",
  "      /in_kanji_text true def",
  "      /swap_font currentfont def",
  "      14 kfont",
  "    } ifelse",
  "    kanji_show",
  "  } {",
  "    orig_show",
  "  } ifelse",
  "} def",
  "",
  "%",
  "% v /s accum",
  "%",
  "/accum { dup cvx exec 3 -1 roll add def } def",
  "%",
  "% Replacement for regular stringwidth that switches between current font",
  "% and kanji font each time a special 'kanji swap' character is encountered",
  "%",
  "/kanji_stringwidth {",
  "  FCCToggleKanji search {",
  "    orig_stringwidth pop /accum_width accum",
  "    pop",
  "    in_kanji_text {",
  "      /in_kanji_text false def",
  "      swap_font setfont",
  "    } {",
  "      /in_kanji_text true def",
  "      /swap_font currentfont def",
  "      14 kfont",
  "    } ifelse",
  "    kanji_stringwidth",
  "  } {",
  "    orig_stringwidth pop /accum_width accum",
  "  } ifelse",
  "} def",
  "",
  "%",
  "% Override definition of show with kanji_show",
  "%",
  "/orig_show /show load def",
  "/show {",
  "  /in_kanji_text false def",
  "  /swap_font currentfont def",
  "  kanji_show",
  "} def",
  "",
  "%",
  "% Override definition of stringwidth with kanji_stringwidth",
  "%",
  "/orig_stringwidth /stringwidth load def",
  "/stringwidth {",
  "  /in_kanji_text false def",
  "  /swap_font currentfont def",
  "  /accum_width 0 def",
  "  kanji_stringwidth",
  "  accum_width 0",
  "} def",
  "",
  0
};

PaperSize paperSizes[] = {
  {"Letter",	"lettertray",	INCH(8.5),	INCH(11)},
  {"Legal",	"legaltray",	INCH(8.5),	INCH(14)},
  {"Tabloid",	"11x17tray",	INCH(11),	INCH(17)},
  {"Executive",	"executivetray",INCH(7.5),	INCH(10)},
  {"A6",	"a6tray",	MM(105),	MM(148)},
  {"A5",	"a5tray",	MM(148),	MM(210)},
  {"A4",	"a4tray",	MM(210),	MM(297)},
  {"A3",	"a3tray",	MM(297),	MM(420)},
  {"A2",	"a2tray",	MM(420),	MM(594)},
  {"A1",	"a1tray",	MM(594),	MM(841)},
  {"B5",	"b5tray",	MM(176),	MM(250)},
  {"B4",	"b4tray",	MM(250),	MM(353)},
  {"B3",	"b3tray",	MM(353),	MM(500)},
  {"B2",	"b2tray",	MM(500),	MM(707)},
  {"SRA2",	"sra2tray",	MM(450),	MM(640)},
  {"RA2",	"ra2tray",	MM(430),	MM(610)},
  {"yatsugiri",	"yatsugiri",	INCH(6.5),	INCH(8.5)},
  {"mutsugiri",	"mutsugiri",	INCH(8),	INCH(10)},
  {"yotsugiri",	"yotsugiri",	INCH(10),	INCH(12)},
  {0}
};

 
const char *recipe_list[] = {
  /* Receipe 1 */
    "%	Pancakes - one serving\n"
    "%\n"
    "%  1 cup     Flour\n"
    "%  1 cup     Milk\n"
    "%  2         Eggs\n"
    "%  1   tbs   Melted Butter\n"
    "%  2   tsp   Sugar\n"
    "%  1/2 tsp   Salt\n"
    "%  1/2 tsp   Baking Soda\n"
    "%\n"
    "% Separate eggs placing whites aside in a clean bowl.  Combine flour and\n"
    "% milk in a separate bowl and mix until smooth.   Stir in melted butter,\n"
    "% sugar, salt and one of the egg yolks.  Now, using a clean mixer, whip\n"
    "% the egg whites until stiff.  Gently stir the egg whites into the batter.\n"
    "% This must be done by hand, or the pancakes will become flat.  Finally,\n"
    "% stir in the baking soda.\n",

  /* Receipe 2 */
    "% Okonomiyaki - one serving\n"
    "%\n"
    "%  3 tbs     Flour\n"
    "%  3 tbs     Water\n"
    "%  1         Egg\n"
    "%  1/8 head  Cabbage (shredded)\n"
    "%  1 tbs     Tororo-imo (Japanese potato) [optional]\n"
    "%  1 tsp     Benishoga (Pickled ginger)\n"
    "%  1 tbs     carrot (shredded)\n"
    "%  1 tbs     leeks (shredded)\n"
    "%  1 tbs     green pepper (shredded)\n"
    "%\n"
    "%  Fillings: beef, pork, vegtables\n"
    "%\n"
    "%  Toppings:\n"
    "%            Aonori (seaweed flakes)\n"
    "%            Katsuo-bushi (dried fish flakes)\n"
    "%            Japanese Tonkatsu Sauce\n"
    "%            Mayonaise\n"
    "%\n"
    "% Mix all of the basic ingedients together in a small bowl.  Place a small\n"
    "% quantity of filling on an oiled hot plate or griddle and fry for a few seconds.\n"
    "% Pour the okonomiyaki mixture from the bowl on top of the filling and smooth\n"
    "% out into a thick pancake-like shape.  Let cook for 5-7 min.  Turn over and cook\n"
    "% another 2-3 min.  Spread the tokatsu sauce and mayonaise on top of the okonomiyaki\n"
    "% then sprinkle with katuo-bushi and aonori.  Cut into 1/6ths and serve.\n",

  /* Receipe 3 */
  "% Tofu\n"
  "% \n"
  "% Ingredients (Japanese names in parentheses): \n"
  "% \n"
  "%      500g Soy Beans (Daizu) \n"
  "%      20g Bittern (Nigari) \n"
  "% \n"
  "% You will also need: \n"
  "% \n"
  "%      Cheesecloth \n"
  "%      Molds (milk cartons work fine) \n"
  "%      Press (rolled up newspaper wrapped in plastic wrap) \n"
  "%      Weights (plates, etc.) \n"
  "%      Cooking thermometer \n"
  "% \n"
  "% Soak the soy beans in water overnight (10-20 hours). Be sure to\n"
  "% use enough water so the beans can double in size. Prepare the molds\n"
  "% using one milk carton per mold (you can use cheese molds if you have\n"
  "% them). Cut off one side of the carton, and punch numerous holes in the\n"
  "% sides and bottom to allow liquid to escape. Reinforce the molds by\n"
  "% using a pair of longs sticks (one on each side) holding them together\n"
  "% with rubber bands on the ends. You should have one piece of\n"
  "% cheesecloth per mold which should be big enough to fit squarely into\n"
  "% the mold and cover the tofu inside.\n"
  "% \n"
  "% The next day, pour out the water, and pulverize the beans in a blender\n"
  "% using enough water to cover the beans (you will probably need to do\n"
  "% this in four to five batches). Pour the bean mixture into a very large\n"
  "% pot and add enough water to double the quantity of liquid. Bring the\n"
  "% liquid to a low boil and lower the temperature, skimming off any foam\n"
  "% that collects on the top. Boil for 20-30 min. stiring (and skimming)\n"
  "% constently. While it is boiling, disolve the bittern (the leftover\n"
  "% \"stuff\" that is formed when you extract salt from seawater) in a small\n"
  "% quantity of water. Place a piece of cheesecloth in a colander, and\n"
  "% pour the liquid through it into a large bowel. The liquid that passes\n"
  "% through is called \"soy milk\" and will become the tofu. The solid\n"
  "% material that remains in the cheesecloth is called \"okara\" and is\n"
  "% useful for various Japanese dishes (recipes will come later). For each\n"
  "% of the molds, fit the cheesecloth squarely into the corners. When the\n"
  "% temperature of the soy milk cools to 70C (158F) pour in the bittern\n"
  "% and stir genetly. When the tofu starts to separate from the water\n"
  "% (almost immediately), pour the mixture into the molds and cover with\n"
  "% cheesecloth. Next, place the plastic wrap covered newspapers on top of\n"
  "% the tofu (it should fit squarely) and place weights on top to press\n"
  "% out the liquid. After about 30 min., remove the tofu from the\n"
  "% molds. This should be done under water in a large pot or tub. Store\n"
  "% the tofu submerged in water.\n"
};
int recipe_count = sizeof(recipe_list)/sizeof(recipe_list[0]);


/*
  Protect an postscript special charcters with a backslash.  Also, if
  we have enabled Japanese support, insert codes to switch fonts when
  we find bytes with the high bit set.
 */
char *filterParen(char *buf,const char *s)
{
  if (!s) {
    *buf = 0;
    return buf;
  }

  if (TkGate.japaneseMode) {
    char *p;
    int in_kanji = 0;

    p = buf;
    while (*s) {
      char c;

      if ((*s & 0x80)) {
	if (!in_kanji)
	  p += sprintf(p,"\\%03o",FCC_BEGINKANJI);
	in_kanji = 1;
      } else {
	if (in_kanji)
	  p += sprintf(p,"\\%03o",FCC_ENDKANJI);
	in_kanji = 0;
      }

      c = (*s++ & 0x7f);

      if (strchr("()\\",c))
	*p++ = '\\';

      *p++ = c;
    }

    if (in_kanji)
      p += sprintf(p,"\\%03o",FCC_ENDKANJI);

    *p = 0;
    return buf;
  } else {
    char *p;

    p = buf;
    while (*s) {
      if (strchr("()\\",*s))
	*p++ = '\\';
      if (isprint(*s))
	*p++ = *s++;
      else
	p += sprintf(p,"\\%03o",((*s++)&0xff));
    }
    *p = 0;
    return buf;
  }
}

/*
  Return s with all characters in trim removed.
 */
char *trimChars(char *buf,char *s,char *trim)
{
  char *p;

  p = buf;
  while (*s) {
    if (!strchr(trim,*s))
      *p++ = *s++;
    else
      s++;
  }
  *p = 0;
  return buf;
}


/*****************************************************************************
 * 
 * Get the width of a postscript string in points.
 *
 * Parameters:
 *     F		Font to use for text width calculation
 *     s		Pointer to beginning of string
 *     len		Length of string
 *
 * Returns:		String width in points.
 * 
 *****************************************************************************/
int PSStringWidth(HtmlFont *F,const char *s,int len)
{
  int width = 0;
  int i;

  if (F->family == FF_COURIER) {
    width = fontmetrics_Courier*len*F->size;
  } else if (F->family == FF_KANJI) {
    width = fontmetrics_Kanji*len/2*F->size;
  } else {
    for (i = 0;i < len;i++) {
      unsigned char c = s[i];
      width += psFontWidthTable[F->family][F->props][c]*F->points;
    }
  }

  return width/1000;
}

void PSSetFont(GPrint *P,HtmlFont *font)
{
  char *encoding = "";

  if (font->family < FF_NOKANJI_MAX)
    encoding = "-Latin1";

  fprintf(P->p_f,"/%s%s findfont %d scalefont setfont\n",
	  psFontNameTable[font->family][font->props],encoding,font->points);
}

void PSDrawText(GPrint *P,HtmlFont *font,int x,int y,const char *text,int just)
{
  static HtmlFont curFont = {-1, -1, -1};
  char buf[STRMAX];
  char vjust = 'R';
  char hjust = 'L';

  if ((just & BetweenLeftAndRight)) hjust = 'C';
  if ((just & AtRight)) hjust = 'R';
  if ((just & AtLeft)) hjust = 'L';

  if ((just & BetweenTopAndBottom)) vjust = 'C';
  if ((just & AtBaseline)) vjust = 'L';
  if ((just & AtTop)) vjust = 'T';
  if ((just & AtBottom)) vjust = 'B';

  if (!HtmlFont_isEqual(&curFont, font)) {
    PSSetFont(P,font);
    curFont = *font;
  }

  fprintf(P->p_f,"(%s) /%c /%c %d %d pjshow\n",filterParen(buf,text),hjust,vjust,x,y);
}

void GPrintOpt_init(GPrintOpt *PO)
{
  PO->po_cmd = 0;
  PO->po_file = 0;
  PO->po_paper = 0;
  PO->po_orient = 0;
  PO->po_style = 0;
  PO->po_select = 0;
  PO->po_modlist = 0;
  PO->po_epsf = 0;
  PO->po_index = 0;
  PO->po_graph = 0;
  PO->po_4up = 0;
  PO->po_isDuplex = 0;
  PO->po_start = 0;
  PO->po_end = 0;
  PO->po_scaleLength = 0;
  PO->po_incLib = 0;
}

/*
 * Set up defaults for command line printing
 */
void GPrintOpt_clDefault(GPrintOpt *PO)
{
  PO->po_title = "";
  PO->po_cmd = 0;
  PO->po_file = 0;
  PO->po_paper = "Letter";
  PO->po_orient = "landscape";
  PO->po_style = "partition";
  PO->po_select = "all";
  PO->po_modlist = "";
  PO->po_epsf = "0";
  PO->po_index = "0";
  PO->po_graph = "0";
  PO->po_4up = "1";
  PO->po_isDuplex = "0";
  PO->po_start = 0;
  PO->po_end = 0;
  PO->po_scaleLength = 0;
  PO->po_incLib = "0";
}

GModLayout *new_GModLayout(GModuleDef *M)
{
  GModLayout *L = (GModLayout *) ob_malloc(sizeof(GModLayout),"GModLayout");
  assert(L);

  L->l_xbase = L->l_ybase = L->l_width = L->l_height = 0.0;
  L->l_isSmall = 0;
  L->l_xmin = L->l_xmax = L->l_ymin = L->l_ymax = 0;

  L->l_numRows = L->l_numCols = 1;

  L->l_mod = M;

  return L;
}

GModLayout *copyNew_GModLayout(GModLayout *L)
{
  GModLayout *cL = (GModLayout *) ob_malloc(sizeof(GModLayout),"GModLayout");
  assert(cL);

  *cL = *L;

  return cL;
}

void delete_GModLayout(GModLayout *L)
{
  ob_free(L);
}

GPage *new_GPage(int tp,int pn)
{
  GPage *P = (GPage *) ob_malloc(sizeof(GPage),"GPage");
  assert(P);

  P->pg_num = pn;
  P->pg_type = tp;
  P->pg_nmod = 0;

  return P;
}

void delete_GPage(GPage *P)
{
  ob_free(P);
}

/*****************************************************************************
 *
 * Get set of all modules used in the current circuit.
 *
 * Parameters:
 *     H	Return of set of modules used in the current circuit
 *     L	Array of layouts of used modules.
 *     M	Top-level module to search.
 *     incLib	Non-zero if top-level modules should be included.
 *
 *****************************************************************************/
static int GPrint_getUsedMods(NHash *H,GModLayout **L,GModuleDef *M,int incLib)
{
  int num = 0;
  HashElem *E;

  if (NHash_find(H,(int)M)) return 0;
  NHash_insert(H,(int)M,M);

  if (!M->m_isLib || incLib)
    L[num++] = new_GModLayout(M);

  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    if (GCElement_isModule(g)) {
      GModuleDef *CM = env_findModule(g->u.block.moduleName);
      if (CM) {
	num += GPrint_getUsedMods(H,L+num,CM,incLib);
      }
    }
  }

  return num;
}

/*****************************************************************************
 *
 * Create print context
 *
 * Parameters:
 *     PO		Printer options
 *
 * Returns:		New print context.
 *
 *****************************************************************************/
GPrint *new_GPrint(GPrintOpt *PO)
{
  GPrint *P = (GPrint *) ob_malloc(sizeof(GPrint),"GPrint");
  PaperSize *size = 0;
  int i;
  int incLib = 0;

  assert(P);

  P->p_opts = *PO;

  PHash_init_noob(&P->p_genSymbols);

  /*
   * Basic initialize so object is deletable.
   */
  P->p_isFile = 0;
  P->p_f = 0;
  P->p_mods = 0;

  P->p_isDuplex = (strcmp(PO->po_isDuplex,"1") == 0);
  P->p_isepsf   = (strcmp(PO->po_epsf,"1") == 0);
  P->p_style    = (PO->po_style && strcmp(PO->po_style,"fit") == 0) ? PS_FIT : PS_PARTITION;

  /*
   * Trace related parameters
   */
  P->p_trace.ti_start = P->p_trace.ti_end = 0;
  if (PO->po_start) sscanf(PO->po_start,"%llu",&P->p_trace.ti_start);
  if (PO->po_end) sscanf(PO->po_end,"%llu",&P->p_trace.ti_end);
  if (PO->po_scaleLength) sscanf(PO->po_scaleLength,"%llu",&P->p_trace.ti_scaleLength);


  if (PO->po_incLib) sscanf(PO->po_incLib,"%d",&incLib);

  /*
    If this is a scope trace then there must be a positive range.
   */
  if (PO->po_start) {
    if (P->p_trace.ti_start >= P->p_trace.ti_end) {
      message(1,msgLookup("err.badrange"));		/* Non-positive range for scope trace is not allowed. */
      delete_GPrint(P);
      return 0;
    }
  }

  /*
    Open printer or file.
   */
  if (PO->po_cmd) {
    if (!(P->p_f = popen(PO->po_cmd,"w"))) {
      message(1,msgLookup("err.badlprcmd"),PO->po_cmd);		/* Unable to exectute printer command '%s'. */
      delete_GPrint(P);
      return 0;
    }
    P->p_isFile = 0;
  } else {
    if (!(P->p_f = fopen(PO->po_file,"w"))) {
      message(1,msgLookup("err.badpsopen"),PO->po_file);	/* Unable to open file '%s' for postscript output. */
      delete_GPrint(P);
      return 0;
    }
    P->p_isFile = 1;
  }

  /*
    Find paper size and orientation
   */
  for (i = 0;paperSizes[i].ps_size;i++) {
    size = &paperSizes[i];
    if (strcmp(size->ps_size,PO->po_paper) == 0)
      break;
  }
  P->p_size = size;

  if (strcmp(PO->po_orient,"landscape") == 0 && !P->p_isepsf) {
    P->p_orient = 1;
    P->p_width = size->ps_height;
    P->p_height = size->ps_width;
  } else {
    P->p_orient = 0;
    P->p_width = size->ps_width;
    P->p_height = size->ps_height;
  }

  P->p_uWidth = P->p_width - PAGE_LMARGIN - PAGE_RMARGIN;
  P->p_uHeight = P->p_height- PAGE_TMARGIN - PAGE_BMARGIN - PAGE_LBLOCK;

  P->p_mods = 0;
  P->p_numMods = 0;

  if (P->p_isepsf) {
    /* EPSF implies that we use the current module */
    P->p_numMods = 1;
    P->p_mods = (GModLayout**) ob_malloc(sizeof(GModLayout*),"GModLayout*");
    assert(P->p_mods);
    P->p_mods[0] = new_GModLayout(TkGate.circuit->es->env);
  } else if (PO->po_select) {
    if (strcmp(PO->po_select,"all") == 0) {
      HashElem *E;
      P->p_mods = (GModLayout**) ob_malloc(Hash_numElems(TkGate.circuit->moduleTable)*sizeof(GModLayout*),"GModLayout*[]");
      assert(P->p_mods);

      for (E = Hash_first(TkGate.circuit->moduleTable);E;E = Hash_next(TkGate.circuit->moduleTable,E)) {
	GModuleDef *M = (GModuleDef *) HashElem_obj(E);
	if (!M->m_isLib || incLib) {
	  GModLayout *L = new_GModLayout(M);
	  P->p_mods[P->p_numMods++] = L;
	}
      }
    } else if (strcmp(PO->po_select,"use") == 0) {
      NHash *H = new_NHash();

      P->p_mods = (GModLayout**) ob_malloc(Hash_numElems(TkGate.circuit->moduleTable)*sizeof(GModLayout*),"GModLayout*[]");
      assert(P->p_mods);

      P->p_numMods = GPrint_getUsedMods(H,P->p_mods,TkGate.circuit->root_mod,incLib);
      delete_NHash(H);

    } else if (strcmp(PO->po_select,"sel") == 0) {
      char *T, *buf;

      P->p_numMods = 0;
      P->p_mods = (GModLayout**) ob_malloc(Hash_numElems(TkGate.circuit->moduleTable)*sizeof(GModLayout*),"GModLayout*");
      assert(P->p_mods);

      buf = strdup(PO->po_modlist);
      for (T = strtok(buf," ,");T;T = strtok(0," ,")) {
	char buf2[STRMAX];
	GModuleDef *M = env_findModule(trimChars(buf2,T,"()+"));
	if (M)
	  P->p_mods[P->p_numMods++] = new_GModLayout(M);
      }
      free(buf);

    } else { /* cur or unknown */
      P->p_numMods = 1;
      P->p_mods = (GModLayout**) ob_malloc(sizeof(GModLayout*),"GModLayout*");
      assert(P->p_mods);
      P->p_mods[0] = new_GModLayout(TkGate.circuit->es->env);
    }
  }

  P->p_numPages = P->p_numMods;

  return P;
}

/*****************************************************************************
 *
 * Delete a print context.
 *
 * Parameters:
 *     P		Print context to be deleted.
 *
 *****************************************************************************/
void delete_GPrint(GPrint *P)
{
  PHash_uninit(&P->p_genSymbols);

  if (P->p_f) {
    if (P->p_isFile)
      fclose(P->p_f);
    else
      pclose(P->p_f);
  }

  if (P->p_mods) {
    int i;
    for (i = 0;i < P->p_numMods;i++) {
      delete_GModLayout(P->p_mods[i]);
    }
    ob_free(P->p_mods);
  }
  ob_free(P);
}


static void GPrint_pageSetup(GPrint *P)
{
#if 0
  const char *tray = P->p_size->ps_tray;
#endif
  const char *dup_mode = P->p_isDuplex ? "true" : "false";

  /*
   * Don't generate page setup for EPSF files.
   */
  if (P->p_isepsf) return;

  fprintf(P->p_f,"%%%%BeginSetup\n");
  fprintf(P->p_f,"%%%%IncludeResource: font Hevetica\n");
  fprintf(P->p_f,"%%%%IncludeResource: font Hevetica-Oblique\n");
  fprintf(P->p_f,"%%%%IncludeResource: font Hevetica-Bold\n");
  fprintf(P->p_f,"%%%%IncludeResource: font Hevetica-BoldOblique\n");
  fprintf(P->p_f,"%%%%IncludeResource: font Symbol\n");

  /*
   * This is causing problems for non-Letter size paper.
   */
#if 0
  fprintf(P->p_f,"%%%%BeginFeature: *PageSize %s\n",tray);
  fprintf(P->p_f,"statusdict /%s known {\n",tray);
  fprintf(P->p_f,"  statusdict begin\n");
  fprintf(P->p_f,"    %s\n",tray);
  fprintf(P->p_f,"  end\n");
  fprintf(P->p_f,"} if\n");
  fprintf(P->p_f,"%%%%EndFeature\n");
#endif

  fprintf(P->p_f,"%%%%BeginFeature: *DuplexMode %s\n",dup_mode);
  fprintf(P->p_f,"statusdict /setduplexmode known {\n");
  fprintf(P->p_f,"  statusdict begin\n");
  fprintf(P->p_f,"    %s setduplexmode\n",dup_mode);
  fprintf(P->p_f,"  end\n");
  fprintf(P->p_f,"} if\n");
  fprintf(P->p_f,"%%%%EndFeature\n");

  fprintf(P->p_f,"%%%%EndSetup\n");
}

void GPrint_createFonts(GPrint *P,Encoder *encoding)
{
  const char *encodingName = "-Latin1";
  int i,j;

  for (i = 0;i < FF_NOKANJI_MAX;i++)
    for (j = 0;j < FP_MAX;j++)
      fprintf(P->p_f,"/%s-Latin1 /%s findfont defLatin1\n",
	      psFontNameTable[i][j],psFontNameTable[i][j]);

  /*
   * Generate font commands for backward compatability.
   */
  fprintf(P->p_f,"/cfont { /%s%s findfont exch scalefont setfont } bind def\n",
	  psFontNameTable[FF_COURIER][FP_ROMAN],encodingName);
  fprintf(P->p_f,"/rfont { /%s%s findfont exch scalefont setfont } bind def\n",
	  psFontNameTable[FF_HELVETICA][FP_ROMAN],encodingName);
  fprintf(P->p_f,"/bfont { /%s%s findfont exch scalefont setfont } bind def\n",
	  psFontNameTable[FF_HELVETICA][FP_BOLD],encodingName);
  fprintf(P->p_f,"/ifont { /%s%s findfont exch scalefont setfont } bind def\n",
	  psFontNameTable[FF_HELVETICA][FP_ITALIC],encodingName);
  fprintf(P->p_f,"/bifont { /%s%s findfont exch scalefont setfont } bind def\n",
	  psFontNameTable[FF_HELVETICA][(FP_BOLD|FP_ITALIC)],encodingName);
  fprintf(P->p_f,"/syfont { /%s findfont exch scalefont setfont } bind def\n",
	  psFontNameTable[FF_SYMBOL][FP_ROMAN]);

  /*
   * Generate kanji font if needed
   */
  if (TkGate.japaneseMode) {
    fprintf(P->p_f,"/kfont { /%s findfont exch scalefont setfont } bind def\n",
	    psFontNameTable[FF_KANJI][FP_ROMAN]);
  }

}

/*****************************************************************************
 *
 * Generate the preamble of a postscript file.
 *
 * Parameters:
 *     P		Print context to use.
 *     do_gates		Non-zero if preamble code for drawing gates should be generated.
 *
 *****************************************************************************/
void GPrint_outputPreamble(GPrint *P,int do_gates)
{
  static char *month[] = {"January","February","March","April","May","June","July",
			  "August","September","October","November","December"};
  struct passwd *pw;
  struct tm *tm;
  time_t clock[1];
  int i,j;
  HashElem *E;
  int n;
  char buf[STRMAX];
  int recipe_pos = random() % 15;

  pw = getpwuid(getuid());

  time(clock);
  tm = localtime(clock);

  fprintf(P->p_f,"%%!PS-Adobe-3.0%s\n",P->p_isepsf ? " EPSF-1.2" : "");
  /*  fprintf(P->p_f,"%%%%Title: %s\n",P->p_opts.po_title);*/
  fprintf(P->p_f,"%%%%Title: %s\n",TkGate.circuit->title);


  fprintf(P->p_f,"%%%%DocumentNeededResources: font");
  for (i = 0;i < FF_MAX;i++)
    for (j = 0;j < FP_MAX;j++) {
      if (TkGate.japaneseMode || i != FF_KANJI)
	fprintf(P->p_f," %s",psFontNameTable[i][j]);
    }
  fprintf(P->p_f,"\n");

  fprintf(P->p_f,"%%%%Creator: %s and TKGate %s\n",filterParen(buf,pw->pw_gecos),VERSIONS[0].vd_name);
  fprintf(P->p_f,"%%%%CreationDate: %d %s %d\n",tm->tm_mday,month[tm->tm_mon],1900+tm->tm_year);
  if (!P->p_isepsf) {
    fprintf(P->p_f,"%%%%DocumentPaperSizes: %s\n",P->p_opts.po_paper);
    fprintf(P->p_f,"%%%%Pages: %d\n",P->p_numPages);
    fprintf(P->p_f,"%%%%Orientation: %s\n",P->p_orient ? "Landscape" : "Portrait");
  } else {
    if (P->p_mods && P->p_mods[0]) {
      GModLayout *ML = P->p_mods[0];
      fprintf(P->p_f,"%%%%BoundingBox: %d %d %d %d\n",
	      EPSF_MINX,EPSF_MINY,
	      EPSF_MINX+(ML->l_xmax-ML->l_xmin),EPSF_MINY+(ML->l_ymax-ML->l_ymin));
    }
  }

  fprintf(P->p_f,"%%%%EndComments\n");

  for (i = 0;gateps_copyright[i];i++)
    fprintf(P->p_f,"%s\n",gateps_copyright[i]);
  
  if (TkGate.japaneseMode) {
    for (i = 0;kanji_support_prolog[i];i++)
      fprintf(P->p_f,"%s\n",kanji_support_prolog[i]);
  }

  for (i = 0;gateprolog[i];i++)
    fprintf(P->p_f,"%s\n",gateprolog[i]);

  if (do_gates) {
    n = 0;
    for (E = Hash_first(GateIdxHash);E;E = Hash_next(GateIdxHash,E)) {
      GGateInfo *gi = (GGateInfo*) HashElem_obj(E);

      if (++n == recipe_pos) {
	int r_idx;
	const char *r;

	fprintf(P->p_f,"%% ...and now for something completely different...\n");
	fprintf(P->p_f,"%%\n");

	r_idx = random() % recipe_count;
	r = recipe_list[r_idx];
	fprintf(P->p_f,"%s",r);
      }

      if (!gi->psdef) continue;
      for (i = 0;gi->psdef[i];i++)
	fprintf(P->p_f,"%s\n",gi->psdef[i]);
    }
  }

  fprintf(P->p_f,"\n");
  fprintf(P->p_f,"/Author (%s) def\n",filterParen(buf,pw->pw_gecos));
  fprintf(P->p_f,"/Site (%s) def\n",filterParen(buf,TkGate.siteName));
  fprintf(P->p_f,"/Date (%s %d, %d) def\n",month[tm->tm_mon],tm->tm_mday,1900+tm->tm_year);
  fprintf(P->p_f,"/CircuitName (%s) def\n",filterParen(buf,TkGate.circuit->title));
  fprintf(P->p_f,"/isepsf %s def\n",P->p_isepsf ? "true" : "false");
  fprintf(P->p_f,"/NumPages ( of %d) def\n",P->p_numPages);
  fprintf(P->p_f,"/GateVersion (TKGate %s) def\n",VERSIONS[0].vd_name);
  fprintf(P->p_f,"/IsLandscape %s def\n",P->p_orient ? "true" : "false");
  fprintf(P->p_f,"/PageWidth %0.2f def\n",P->p_width);
  fprintf(P->p_f,"/PageHeight %0.2f def\n",P->p_height);
  fprintf(P->p_f,"/LMargin %d def\n",PAGE_LMARGIN);
  fprintf(P->p_f,"/RMargin %d def\n",PAGE_LMARGIN);
  fprintf(P->p_f,"/TMargin %d def\n",PAGE_LMARGIN);
  fprintf(P->p_f,"/BMargin %d def\n",PAGE_LMARGIN);
  fprintf(P->p_f,"/LBlock %d def\n",PAGE_LBLOCK);

  if (TkGate.japaneseMode) {
    fprintf(P->p_f,"/FCCToggleKanji (\\%03o) def\n",FCC_BEGINKANJI);
  }

  /*
   * If using Latin1 fonts, generate definitions for those fonts.
   */
  GPrint_createFonts(P, Circuit_getPSEncoder(TkGate.circuit));

  fprintf(P->p_f,"%%%%EndProlog\n");
  GPrint_pageSetup(P);
  fprintf(P->p_f,"\n");
}

/*****************************************************************************
 *
 * Generate the postscript trailer
 *
 * Parameters:
 *     P		Print context to use.
 *
 *****************************************************************************/
void GPrint_outputTrailer(GPrint *P)
{
  fprintf(P->p_f,"%%%%Tailer\n");
}

/*****************************************************************************
 *
 * Print the currently selected critical path of a module
 *
 * Parameters:
 *     P		Print context to use.
 *     L		Module layout to be printed.
 *
 *****************************************************************************/
static void GPrint_printModuleCPath(GPrint *P,GModLayout *L)
{
  GModuleDef *M = L->l_mod;
  GWireList *wl;

  for (wl = M->m_wires;wl;wl = wl->wl_next) {
    GWire *w = wl->wl_wire;
    GWireNode *n = 0;

    /*
     * Draw critical path if set
     */
    if (w->cpath) {
      if (w->gate && w->gate->typeinfo->Code == GC_JOINT) {
	int x = w->gate->xpos;
	int y = w->gate->ypos;

	fprintf(P->p_f,"%d %d joint_gray\n",x,y);
      }
      if (w->nodes->out) {
	fprintf(P->p_f,"gsave\n");
	fprintf(P->p_f,"8 setlinewidth\n");
	fprintf(P->p_f,".5 setgray\n");
	fprintf(P->p_f,"newpath\n");
      
	for (n = w->nodes;n;n = n->out) {
	  int x,y;

	  x = n->x;
	  y = n->y;

	  if (n == w->nodes) {
	    fprintf(P->p_f,"%d %d moveto\n",x,y);
	  } else {
	    fprintf(P->p_f,"%d %d lineto\n",x,y);
	  }
	}
	fprintf(P->p_f,"stroke\n");
	fprintf(P->p_f,"grestore\n");
      }      
    }
  }
}

void GPrint_printLabelsAndSizes(GPrint *P,GModLayout *L,GWire *w)
{
  GWireNode *n;
  int x,y,p;
  char label[STRMAX];
  
  GNet_getDisplayLabel(w->net, label, STRMAX, DLM_GET_ALWAYS);

  for (n = w->nodes;n && n->out;n = n->out) {
    if (n->showSize && w->net->n_nbits > 1) {
      x = (n->x + n->out->x)/2;
      y = (n->y + n->out->y)/2;
      fprintf(P->p_f,"%d %d sizesymbol\n",x,y);
      if (n->x == n->out->x)
	fprintf(P->p_f,"(%d) %d %d %d wirelabel\n",w->net->n_nbits,BetweenTopAndBottom|AtLeft,x+8,y);
      else
	fprintf(P->p_f,"(%d) %d %d %d wirelabel\n",w->net->n_nbits,BetweenLeftAndRight|AtBottom,x,y-3);
    }
    if (n->isLabeled) {
      GWireNode_getLabelPos(n,w->net,&x,&y,&p);
      
      fprintf(P->p_f,"(%s) %d %d %d wirelabel\n",label,p,x,y);

    }
  }
}

/*****************************************************************************
 *
 * Display all of the wires in a module.
 *
 * Parameters:
 *     P		Print context to use.
 *     L		Module for which wires are to be displayed.
 *
 *****************************************************************************/
static void GPrint_printModuleWires(GPrint *P,GModLayout *L)
{
  GModuleDef *M = L->l_mod;
  GWireList *wl;
  int last_was_multi = 0;

  for (wl = M->m_wires;wl;wl = wl->wl_next) {
    GWire *w = wl->wl_wire;
    GWireNode *n = 0;
    int is_multi;

    if (!w->nodes->out) continue;	/* Print in one direction only */

    GPrint_printLabelsAndSizes(P,L,w);

    /*
     * Do the actual drawing of the wire.
     */
    is_multi = (w->net->n_nbits != 1);
    if (is_multi != last_was_multi) {
      if (is_multi)
	fprintf(P->p_f,"2 setlinewidth\n");
      else
	fprintf(P->p_f,"1 setlinewidth\n");
    }
    last_was_multi = is_multi;
    fprintf(P->p_f,"newpath\n");
    for (n = w->nodes;n;n = n->out) {
      int x,y;

      x = n->x;
      y = n->y;
      if (n->end && n->end->invert) {
	int x2,y2,xi,yi;

	xi = x;
	yi = y;

	if (n->out) {
	  x2 = n->out->x;
	  y2 = n->out->y;
	} else {
	  x2 = n->in->x;
	  y2 = n->in->y;
	}

	if (y == y2) {
	  if (x2 > x) {
	    xi += 2;
	    x += 4;
	  } else {
	    xi -= 2;
	    x -= 4;
	  }
	} else {
	  if (y2 > y) {
	    yi += 2;
	    y += 4;
	  } else {
	    yi -= 2;
	    y -= 4;
	  }
	}

	fprintf(P->p_f,"%d %d inverter\n",xi,yi);
      }

      if (n == w->nodes) {
	fprintf(P->p_f,"%d %d moveto\n",x,y);
      } else {
	fprintf(P->p_f,"%d %d lineto\n",x,y);
      }
    }
    fprintf(P->p_f,"stroke\n");
  }
}

/*****************************************************************************
 *
 * Print a single HDL module
 *
 * Parameters:
 *      P		Printer state variable
 *      L		Module with layout information
 *
 *
 *****************************************************************************/
static void GPrint_printHdlModule(GPrint *P,GModLayout *L,double xoff,double yoff)
{
  int linesPerPage = (P->p_uHeight-2*PAGE_MODMARGIN)/(int)(1.2*hdl_font.points);
  int lineCount = 0;
  int startLine = 0;
  GModuleDef *M = L->l_mod;
  Encoder *encoder = Circuit_getPSEncoder(TkGate.circuit);
  char *c;
  int x,y;

  //  fprintf(P->p_f,"%f %f translate\n",xoff,yoff);

#if 0
  fprintf(P->p_f,"gsave\n");
  fprintf(P->p_f,"  1 0 0 setrgbcolor\n");
  fprintf(P->p_f,"  newpath\n");
  fprintf(P->p_f,"  %d %d moveto\n",0,0);
  fprintf(P->p_f,"  %d %d lineto\n",L->l_width,0);
  fprintf(P->p_f,"  %d %d lineto\n",L->l_width,L->l_height);
  fprintf(P->p_f,"  %d %d lineto\n",0,L->l_height);
  fprintf(P->p_f,"  closepath stroke\n");
  fprintf(P->p_f,"grestore\n");
#endif

  fprintf(P->p_f,"%d cfont\n",hdl_font.points);
  x = 0;
  y = L->l_height-0;

  startLine = linesPerPage*L->l_r;

  for (c = M->m_text;*c;) {
    char buf[STRMAX];		/* Buffer for parenthesis filtering */
    char line[STRMAX];		/* Raw line of text */
    char dline[STRMAX];		/* decode raw line of text */
    int l;
    char *e;

    for (e = c;*e && *e != '\n';e++);

    l = (e-c);
    if (l > STRMAX-1) l = STRMAX-1;
    strncpy(line,c,e-c);
    line[l] = 0;

    if (lineCount >= startLine) {
      y -= 1.2*hdl_font.points;
      fprintf(P->p_f,"%d %d moveto\n",x,y);
      recodeText(encoder,dline,STRMAX,line);
      fprintf(P->p_f,"(%s) show\n",filterParen(buf,dline));
    }
    lineCount++;

    if (lineCount >= (startLine + linesPerPage)) break;

    if (!*e) break;
    c = e+1;
  }
}

/*****************************************************************************
 *
 * Print a single netlist module
 *
 * Parameters:
 *      P		Printer state variable
 *      L		Module with layout information
 *
 *
 *****************************************************************************/
static void GPrint_printNetlistModule(GPrint *P,GModLayout *L,double scale,double xoff,double yoff)
{
  GModuleDef *M = L->l_mod;
  HashElem *E;

  fprintf(P->p_f,"0 %d translate\n",L->l_height);
  if (scale != 1.0)
    fprintf(P->p_f,"%f %f scale\n",scale,scale);
  fprintf(P->p_f,"1 -1 scale\n");
  fprintf(P->p_f,"%f %f translate\n",xoff,yoff);

  GPrint_printModuleCPath(P,L);
  GPrint_printModuleWires(P,L);


  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    (*g->typeinfo->PSWrite)(P,L,g);
  }
}


/*****************************************************************************
 *
 * Print a single module
 *
 * Parameters:
 *      P		Printer state variable
 *      L		Module with layout information
 *
 *
 *****************************************************************************/
static void GPrint_printModule(GPrint *P,GModLayout *L,double scale,double xoff,double yoff)
{
  GModuleDef *M = L->l_mod;


  fprintf(P->p_f,"gsave\n");

  if (GModuleDef_getType(M) == MT_TEXTHDL) {
    GPrint_printHdlModule(P,L,xoff,yoff);
  } else {
    GPrint_printNetlistModule(P,L,scale,xoff,yoff);
  }

  fprintf(P->p_f,"grestore\n");
}


/*****************************************************************************
 *
 * Print a layout of a module
 *
 * Parameters:
 *     P
 *     L
 * 
 * When pages are partitioned, we include a little overlap on the edges to
 * compensate for any gates that might have been cut in two.
 * 
 *         V
 * +-------+-+-------+
 * |       | |       |
 * |       | |       |
 * |       | |       |
 * +-------+-+-------+
 * |       | |       |
 * +-------+-+-------+<
 * |       | |       |
 * |       | |       |
 * |       | |       |
 * +-------+-+-------+
 *
 *****************************************************************************/
static void GPrint_printLayout(GPrint *P,GModLayout *L)
{
  double xoff,yoff,scale;

  scale = 1.0;

  if (L->l_numRows <= 1 && L->l_numCols <= 1) {
    /*
     * The circuit is not paritioned.  Center it in the layout area by computing
     * the necessary offset.  If it is bigger than the layout area scale it
     * to make it fit.
     */
    int cwidth = (L->l_xmax-L->l_xmin);
    int cheight = (L->l_ymax-L->l_ymin);
    if (cwidth > L->l_width-PAGE_MODMARGIN*2 || cheight > L->l_height-PAGE_MODMARGIN*2) {
      double xscale = (double)(L->l_width-PAGE_MODMARGIN*2)/(double)cwidth;
      double yscale = (double)(L->l_height-PAGE_MODMARGIN*2)/(double)cheight;
      if (xscale < yscale)
	scale = xscale;
      else
	scale = yscale;
    }
    xoff = ((double)L->l_width/scale - (L->l_xmax+L->l_xmin))/2.0;
    yoff = ((double)L->l_height/scale - (L->l_ymax+L->l_ymin))/2.0;
  } else {
    /*
     * This is a piece of a partitioned circuit.
     */

    int xmin = L->l_xmin + L->l_c*L->l_width;
    int ymin = L->l_ymin + L->l_r*L->l_height;
    int xmax = xmin + L->l_width;
    int ymax = ymin + L->l_height;

    xoff = ((double)L->l_width/scale - (xmax+xmin))/2.0;
    yoff = ((double)L->l_height/scale - (ymax+ymin))/2.0;
  }

#if 0
  fprintf(P->p_f,"newpath\n");
  fprintf(P->p_f,"%d %d 3 0 360 arc closepath fill\n",PAGE_MODMARGIN,PAGE_MODMARGIN);
  fprintf(P->p_f,"%d %d 3 0 360 arc closepath fill\n",L->l_width-PAGE_MODMARGIN,PAGE_MODMARGIN);
  fprintf(P->p_f,"%d %d 3 0 360 arc closepath fill\n",L->l_width-PAGE_MODMARGIN,L->l_height-PAGE_MODMARGIN);
  fprintf(P->p_f,"%d %d 3 0 360 arc closepath fill\n",PAGE_MODMARGIN,L->l_height-PAGE_MODMARGIN);

  fprintf(P->p_f,"%d %d moveto\n",PAGE_MODMARGIN,PAGE_MODMARGIN);
  fprintf(P->p_f,"%d %d lineto stroke\n",L->l_width-PAGE_MODMARGIN,L->l_height-PAGE_MODMARGIN);
  fprintf(P->p_f,"%d %d moveto\n",L->l_width-PAGE_MODMARGIN,PAGE_MODMARGIN);
  fprintf(P->p_f,"%d %d lineto stroke\n",PAGE_MODMARGIN,L->l_height-PAGE_MODMARGIN);
#endif

  GPrint_printModule(P,L,scale,xoff,yoff);
}

static void GPrint_printModPage(GPrint *P,GPage *PG)
{
  char buf[STRMAX];
  GModLayout *L = PG->pg_mods[0];

  if (L->l_numCols > 1 || L->l_numRows > 1) {
    fprintf(P->p_f,"%d %d %d %d (%d of %d) (%s) BP_P\n",
	    L->l_r,L->l_c,L->l_numRows,L->l_numCols,PG->pg_num,P->p_numPages,
	    filterParen(buf,L->l_mod->m_name));
    fprintf(P->p_f,"() %d %d %d %d BM\n",
	    L->l_xbase,L->l_ybase,L->l_width,L->l_height);
  } else {
    fprintf(P->p_f,"(%d of %d) (%s) BP\n",PG->pg_num,P->p_numPages,
	    filterParen(buf,L->l_mod->m_name));

    fprintf(P->p_f,"() %d %d %d %d BM\n",
	    L->l_xbase,L->l_ybase,L->l_width,L->l_height);
  }
  GPrint_printLayout(P,L);
  fprintf(P->p_f,"EM\n");
  fprintf(P->p_f,"EP\n");
}

static void GPrint_printMod4Page(GPrint *P,GPage *PG)
{
  char buf[STRMAX],*p;
  int i;

  /*
   * Set up list of module names.
   */
  *buf = 0;
  p = buf;
  for (i = 0;i < PG->pg_nmod;i++) {
    GModLayout *L = PG->pg_mods[i];
    if (i) {
      strcat(p,", ");
      p+=2;
    }
    filterParen(p,L->l_mod->m_name);
    p += strlen(p);
  }

  fprintf(P->p_f,"(%d of %d) (%s) BP\n",PG->pg_num,P->p_numPages,buf);
  for (i = 0;i < PG->pg_nmod;i++) {
    GModLayout *L = PG->pg_mods[i];
    fprintf(P->p_f,"(%s) %d %d %d %d BM_C\n",filterParen(buf,L->l_mod->m_name),
	    L->l_xbase,L->l_ybase,L->l_width,L->l_height);
    GPrint_printLayout(P,L);
    fprintf(P->p_f,"EM\n");
  }
  fprintf(P->p_f,"EP\n");
}

static void GPrint_printIndexPage(GPrint *P,GPage *PG)
{
  int x,y;
  char *mod_list[MODMAX];	/* Module names */
  int start_page[MODMAX];	/* Start page of module */
  int end_page[MODMAX];		/* End page of module */
  int num_modules = 0;
  int i,j;

  for (i = 0;i < P->p_numPages;i++) {
    GPage *G = P->p_pages[i];
    if (G->pg_type != PT_MODULE && G->pg_type != PT_MOD4) continue;
    for (j = 0;j < G->pg_nmod;j++) {
      GModLayout *L = G->pg_mods[j];
      GModuleDef *M = L->l_mod;
      if (L->l_r == 0 && L->l_c == 0) {		/* It is assumed (0,0) is printed first */
	mod_list[num_modules] = M->m_name;
	start_page[num_modules] = G->pg_num;
	num_modules++;
      }
      end_page[num_modules-1] = G->pg_num;
    }
  }


  fprintf(P->p_f,"(%d of %d) (<index>) BP\n",PG->pg_num,P->p_numPages);


  /*
    Set position so that next entry will cause a new column to be generated
    at the desired place.
   */
  x = PIDX_LMARGIN-PIDX_COLSEP;
  y = 0;

  for (i = 0;i < num_modules;i++) {
    char buf[STRMAX];

    y -= PIDX_LINESEP;
    if (y < 0) {
      x += PIDX_COLSEP;
      y = P->p_uHeight - PIDX_TMARGIN;
      fprintf(P->p_f,"%d bfont\n",PIDX_FONT);
      fprintf(P->p_f,"%d %d moveto (Modules) show\n",x,y);
      fprintf(P->p_f,"%d %d moveto (Page) show\n",x+PIDX_PGCOLSEP,y);
      fprintf(P->p_f,"%d %d moveto %d %d lineto stroke\n",x,y-PIDX_HDRLINEPOS,x+PIDX_LINELEN,y-PIDX_HDRLINEPOS);
      fprintf(P->p_f,"%d rfont\n",PIDX_FONT);
      y -= PIDX_HDRSEP;
    }

    fprintf(P->p_f,"%d %d moveto (%s) show\n",x,y,filterParen(buf,mod_list[i]));
    if (start_page[i] == end_page[i])
      fprintf(P->p_f,"%d %d moveto (%d) show\n",x+PIDX_PGCOLSEP,y,start_page[i]);
    else
      fprintf(P->p_f,"%d %d moveto (%d-%d) show\n",x+PIDX_PGCOLSEP,y,start_page[i],end_page[i]);
    
  }

  fprintf(P->p_f,"EP\n");
}

static HGNode *new_HGNode(GModuleDef *M)
{
  HGNode *G = (HGNode*) ob_malloc(sizeof(HGNode),"HGNode");

  G->hg_module = M;
  G->hg_maxDepth = 1;
  G->hg_expanded = 1;
  G->hg_nextSibling = 0;
  G->hg_children = 0;

  return G;
}

static void HGNode_addChild(HGNode *G,HGNode *CG)
{
  if (CG->hg_maxDepth+1 > G->hg_maxDepth)
    G->hg_maxDepth = CG->hg_maxDepth+1;
  CG->hg_nextSibling = G->hg_children;
  G->hg_children = CG;
}

static HGNode *build_HGNodeGraph(GModuleDef *M,NHash *H)
{
  HGNode *G;
  HashElem *E;
  NHash *LH = new_NHash();

  G = new_HGNode(M);

  if (NHash_find(H,(unsigned)M)) {
    G->hg_expanded = 0;
  } else {
    G->hg_expanded = 1;
    NHash_insert(H,(unsigned)M,M);
    for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
      GCElement *g = (GCElement*) HashElem_obj(E);
      if (GCElement_isModule(g)) {
	GModuleDef *CM = env_findModule(g->u.block.moduleName);
	HGNode *CG;

	if (CM && !NHash_find(LH,(unsigned)CM)) {
	  NHash_insert(LH,(unsigned)CM,CM);
	  CG = build_HGNodeGraph(CM,H);
	  HGNode_addChild(G,CG);
	}
      }
    }
  }

  delete_NHash(LH);

  return G;
}

static void unbuild_HGNodeGraph(HGNode *G)
{
  if (!G) return;
  unbuild_HGNodeGraph(G->hg_nextSibling);
  unbuild_HGNodeGraph(G->hg_children);
  ob_free(G);
}

static void HGNodeGraph_print(HGNode *G,int level)
{
  int i;

  if (!G) return;

  for (i = 0;i < level;i++) printf(" ");
  printf("%s\n",G->hg_module->m_name);
  HGNodeGraph_print(G->hg_children,level+1);
  HGNodeGraph_print(G->hg_nextSibling,level);
}

static int HGNode_translatePoint(int *x,int *y)
{
  int n = 0;

  while (*y < HG_BOTTOMY) {
    *y += HG_ROOTY-HG_BOTTOMY;
    *x += HG_NEXTCOLUMN;
    n++;
  }

  return n;
}

static void HGNode_draw(HGNode *G,GPrint *P,int x,int y)
{
  char buf[STRMAX];

  HGNode_translatePoint(&x,&y);
  fprintf(P->p_f,"(%s) %s %d %d hgnode\n",filterParen(buf,G->hg_module->m_name),
	  (G->hg_expanded ? "false" : "true"),x,y);
}

static void HGNode_drawLine(GPrint *P,int x1,int y1,int x2,int y2)
{
  int n1 = HGNode_translatePoint(&x1,&y1);
  int n2 = HGNode_translatePoint(&x2,&y2);

  if (n1 == n2)
    fprintf(P->p_f,"%d %d %d %d line\n",x1,y1,x2,y2);
  else if (n1 < n2) {
    fprintf(P->p_f,"%d %d %d %d line\n",x1,y1,x1,HG_BOTTOMY-HG_BOXHEIGHT/2);
    fprintf(P->p_f,"%d %d %d %d line\n",x2,HG_ROOTY,x2,y2);
  } else {
    fprintf(P->p_f,"%d %d %d %d line\n",x1,HG_ROOTY,x1,y2);
    fprintf(P->p_f,"%d %d %d %d line\n",x2,y1,x2,HG_BOTTOMY-HG_BOXHEIGHT/2);
  }
}

static int HGNodeGraph_draw(HGNode *G,GPrint *P,int x,int y)
{
  int nc,ns;

  if (!G) return 0;

  HGNode_draw(G,P,x,y);

  if (G->hg_children) {
    HGNode_drawLine(P,x+HG_LINEINDENT,y-HG_BOXHEIGHT/2,x+HG_LINEINDENT,y-HG_LINE);
    HGNode_drawLine(P,x+HG_LINEINDENT,y-HG_LINE,x+HG_INDENT,y-HG_LINE);
    nc = HGNodeGraph_draw(G->hg_children,P,x+HG_INDENT,y-HG_LINE);
  } else
    nc = 0;

  if (G->hg_nextSibling) {
    HGNode_drawLine(P,x-HG_INDENT+HG_LINEINDENT,y,x-HG_INDENT+HG_LINEINDENT,y-HG_LINE*(nc+1));
    HGNode_drawLine(P,x-HG_INDENT+HG_LINEINDENT,y-HG_LINE*(nc+1),x,y-HG_LINE*(nc+1));
    ns = HGNodeGraph_draw(G->hg_nextSibling,P,x,y-HG_LINE*(nc+1));
  } else
    ns = 0;

  return nc + ns + 1;
}

static void GPrint_printGraphPage(GPrint *P,GPage *PG)
{
  NHash *H = new_NHash();
  HGNode *G = 0;

  G = build_HGNodeGraph(TkGate.circuit->root_mod,H);

  fprintf(P->p_f,"(%d of %d) (<hierarchy>) BP\n",PG->pg_num,P->p_numPages);
  fprintf(P->p_f,"%d bfont\n",HG_FONTSIZE);
  HGNodeGraph_draw(G,P,HG_ROOTX,HG_ROOTY);
  fprintf(P->p_f,"EP\n");

#if 0
  for (E = Hash_first(H);E;E = Hash_next(H,E)) {
    NHashElem_key(E);
    HashElem_obj(E);
  }
#endif

  unbuild_HGNodeGraph(G);
  delete_NHash(H);
}

void GPrint_outputPages(GPrint *P)
{
  int i;

  for (i = 0;i < P->p_numPages;i++) {
    GPage *PG = P->p_pages[i];

    fprintf(P->p_f,"\n%%%%Page: %d %d\n",i+1,i+1);

    switch (PG->pg_type) {
    case PT_MODULE :
      GPrint_printModPage(P,PG);
      break;
    case PT_MOD4 :
      GPrint_printMod4Page(P,PG);
      break;
    case PT_INDEX :
      GPrint_printIndexPage(P,PG);
      break;
    case PT_GRAPH :
      GPrint_printGraphPage(P,PG);
      break;
    case PT_TRACE :
      GPrint_printTracePage(P,PG);
      break;
    }
  }
}

/*
 * Do special case for EPSF setup
 */
void GPrint_setupEPSFPage(GPrint *P)
{
  GModLayout *L = P->p_mods[0];
  int cwidth,cheight;
  GPage *PG;

  GModuleDef_getBBX(L->l_mod, TD_PRINT, &L->l_xmin,&L->l_xmax,&L->l_ymin,&L->l_ymax);
  cwidth  = L->l_xmax-L->l_xmin + 2*PAGE_MODMARGIN;
  cheight = L->l_ymax-L->l_ymin + 2*PAGE_MODMARGIN;

  L->l_isSmall = 0;
  L->l_numRows = 1;
  L->l_numCols = 1;


  P->p_pages = (GPage**) ob_malloc(sizeof(GPage*),"GPage*[]");
  assert(P->p_pages);

  
  L->l_xbase = EPSF_MINX;
  L->l_ybase = EPSF_MINY;
  L->l_width = L->l_xmax-L->l_xmin;
  L->l_height = L->l_ymax-L->l_ymin;

  PG = new_GPage(PT_MODULE,1);
  PG->pg_nmod = 1;
  PG->pg_mods[0] = L;
  P->p_pages[0] = PG;
  L->l_r = 0;
  L->l_c = 0;

  P->p_numPages = 1;
}

/*
 * Sort the modules in "hierarchy" order.
 */
void GPrint_hsortPages(GPrint *P)
{
  NHash *H = new_NHash();	/* Hash table for building hierarchy graph */
  NHash *Mhash = new_NHash();	/* Hash table for modules to be printed */
  HGNode *G = 0;
  List Q;		/* NOTE: lists do not have undo management.  We must clean up everything here. */
  GModLayout *L;
  int N = 0;
  int i;

  List_init(&Q);

  for (i = 0;i < P->p_numMods;i++) {
    L = P->p_mods[i];
    NHash_insert(Mhash,(unsigned)L->l_mod,L);
  }


  //  P->p_numMods;
  G = build_HGNodeGraph(TkGate.circuit->root_mod,H);
  List_addToHead(&Q,G);
  while (List_numElems(&Q) > 0) {
    GModuleDef *M;

    G = (HGNode*) List_popHead(&Q);
    M = G->hg_module;

    for (G = G->hg_children;G;G = G->hg_nextSibling) {
      List_addToTail(&Q,G);
    }

    if ((L = (GModLayout*) NHash_find(Mhash,(unsigned)M))) {
      if (N < P->p_numMods)
	P->p_mods[N++] = L;
      NHash_remove(Mhash,(unsigned)M);
    }
  }

  /*
   * If there were fewer sorted modules, then in the original list, this means
   * that we have unused modules.  In this case, we determine which modules are
   * unused and add them to the end of the list.
   */
  if (N != P->p_numMods) {
    HashElem *E;

    for (E = Hash_first(Mhash);E;E = Hash_next(Mhash,E)) {
      L = (GModLayout*) HashElem_obj(E);
      if (N < P->p_numMods)
	P->p_mods[N++] = L;
    }
  }

  List_uninit(&Q);
  unbuild_HGNodeGraph(G);
  delete_NHash(H);
  delete_NHash(Mhash);
}

/*****************************************************************************
 *
 * Test a module to see if it is a "small" page.
 *
 *
 *
 *****************************************************************************/
static int GPrint_isSmallModule(GPrint *P,GModuleDef *M,int cwidth,int cheight)
{
  if (GModuleDef_getType(M) == MT_TEXTHDL)
    return 0;

  return cwidth < P->p_uWidth/2		/* This module is less than half the page width */
    && cheight < P->p_uHeight/2		/* and less than half the page height */
    && !GModuleDef_isTop(M);		/* and it is not the root module... */
}

/*****************************************************************************
 *
 * Figure out what pages are going to be generated and what will be on them.
 *
 *****************************************************************************/
void GPrint_setupPages(GPrint *P)
{
  int i,idx;
  int numSmall = 0;
  int numLarge = 0;

  /*
   * If this is an EPSF page, set things up a little different.
   */
  if (P->p_isepsf) {
    GPrint_setupEPSFPage(P);
    return;
  }

  GPrint_hsortPages(P);

  /*
    Compute page sizes and decide which ones are small
   */
  for (i = 0;i < P->p_numMods;i++) {
    GModLayout *L = P->p_mods[i];
    int cwidth,cheight;

    GModuleDef_getBBX(L->l_mod, TD_PRINT, &L->l_xmin,&L->l_xmax,&L->l_ymin,&L->l_ymax);
    cwidth  = L->l_xmax-L->l_xmin + 2*PAGE_MODMARGIN;
    cheight = L->l_ymax-L->l_ymin + 2*PAGE_MODMARGIN;

    /* If 4-up mode is enabled and module is small, mark it as a "small" module. */
    if (*P->p_opts.po_4up == '1' && GPrint_isSmallModule(P,L->l_mod,cwidth,cheight)) {
      L->l_isSmall = 1;
      numSmall++;
    }

    /*
     * If module is not a "small module" check to see how many pages we need for it.
     */
    if (!L->l_isSmall) {
      if (GModuleDef_getType(L->l_mod) == MT_NETLIST) {
	/* Netlist Module */
	if (P->p_style == PS_FIT || (cwidth < P->p_uWidth && cheight < P->p_uHeight)) {
	  numLarge++;
	  L->l_numRows = 1;
	  L->l_numCols = 1;
	} else {
	  int j;
	  /*
	    Recompute size without margins added in
	  */
	  cwidth  = L->l_xmax-L->l_xmin;
	  cheight = L->l_ymax-L->l_ymin;

	  for (j = 1;cwidth+j*(2*PAGE_MODMARGIN)+(j-1)*PAGE_OVERLAP > j*P->p_uWidth;j++);
	  L->l_numCols = j;

	  for (j = 1;cheight+j*(2*PAGE_MODMARGIN)+(j-1)*PAGE_OVERLAP > j*P->p_uHeight;j++);
	  L->l_numRows = j;

	  numLarge += L->l_numRows*L->l_numCols;
	}
      } else {
	/* HDL Module */
	int linesPerPage = (P->p_uHeight-2*PAGE_MODMARGIN)/(int)(1.2*hdl_font.points);
	int numLines = GModuleDef_numHdlLines(L->l_mod);
	int numPages = (numLines+linesPerPage-1)/linesPerPage;
	
	L->l_numCols = 1;
	L->l_numRows = numPages;
	numLarge += numPages;
      }
    }
  }

  if (*P->p_opts.po_index == '1') numLarge++;
  if (*P->p_opts.po_graph == '1') numLarge++;

  P->p_numPages = numLarge + (numSmall+MODPGMAX-1)/MODPGMAX;
  P->p_pages = (GPage**) ob_malloc(P->p_numPages*sizeof(GPage*),"GPage*[]");
  assert(P->p_pages);

  idx = 0;
  if (*P->p_opts.po_index == '1') {
    P->p_pages[idx] = new_GPage(PT_INDEX,idx+1);
    idx++;
  }
  if (*P->p_opts.po_graph == '1') {
    P->p_pages[idx] = new_GPage(PT_GRAPH,idx+1);
    idx++;
  }

  /*
    Setup pages for large modules
    */
  if (numLarge) {
    for (i = 0;i < P->p_numMods;i++) {
      GModLayout *L = P->p_mods[i];
      if (L->l_isSmall) continue;

      L->l_xbase = PAGE_MODMARGIN;
      L->l_ybase = PAGE_MODMARGIN;
      L->l_width = P->p_uWidth - 2*PAGE_MODMARGIN;
      L->l_height = P->p_uHeight - 2*PAGE_MODMARGIN;

      if (L->l_numRows <= 1 && L->l_numCols <= 1) {
	GPage *PG = new_GPage(PT_MODULE,idx+1);
	PG->pg_nmod = 1;
	PG->pg_mods[0] = L;
	P->p_pages[idx++] = PG;
	L->l_r = 0;
	L->l_c = 0;
      } else {
	int r,c;
	for (r = 0;r < L->l_numRows;r++)
	  for (c = 0;c < L->l_numCols;c++) {
	    GPage *PG = new_GPage(PT_MODULE,idx+1);
	    GModLayout *PL = copyNew_GModLayout(L);
	    PL->l_r = r;
	    PL->l_c = c;
	    PG->pg_nmod = 1;
	    PG->pg_mods[0] = PL;
	    P->p_pages[idx++] = PG;
	  }
      }
    }
  }

  if (numSmall) {
    GPage *PG = 0;

    for (i = 0;i < P->p_numMods;i++) {
      GModLayout *L = P->p_mods[i];
      if (!L->l_isSmall) continue;

      if (!PG) {
	PG = new_GPage(PT_MOD4,idx+1);
	P->p_pages[idx++] = PG;
      }

      L->l_xbase = (PG->pg_nmod & 0x1) ? (P->p_uWidth/2) : 0;
      L->l_ybase = (PG->pg_nmod & 0x2) ? 0 : (P->p_uHeight/2);
      L->l_width = P->p_uWidth/2;
      L->l_height = P->p_uHeight/2;

      L->l_xbase += PAGE_MODMARGIN;
      L->l_ybase += PAGE_MODMARGIN;
      L->l_width -= 3*PAGE_MODMARGIN/2;
      L->l_height -= 3*PAGE_MODMARGIN/2;

      L->l_r = 0;
      L->l_c = 0;

      if (!(PG->pg_nmod & 0x2)) {
	L->l_ybase -= PAGE_MODMARGIN/2;
      }

      PG->pg_mods[PG->pg_nmod++] = L;
      if (PG->pg_nmod == MODPGMAX) PG = 0;
    }
  }

  P->p_numPages = idx;
}

/*
 * Print a circuit diagram document
 */
void GPrintOpt_print(GPrintOpt *PO)
{
  GPrint *P = new_GPrint(PO);

  if (!P) return;

  GPrint_setupPages(P);
  GPrint_outputPreamble(P,1);
  GPrint_outputPages(P);
  GPrint_outputTrailer(P);

  delete_GPrint(P);
}
