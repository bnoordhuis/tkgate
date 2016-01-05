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
#ifndef __html_h
#define __html_h

/*
 * HTML unit types
 */
#define HU_TEXT		0			/* Text string */
#define HU_RULE		1			/* Horizontal rule */
#define HU_BREAK	2			/* Line break */
#define HU_NEWLINE	3			/* New line */
#define HU_IMAGE	4			/* In-line image */

/*
 * Types of html elements
 */
#define HTML_TEXT	0			/* Block of text */
#define HTML_A		1			/* Hyperlink or tag */
#define HTML_B		2			/* Bold */
#define HTML_I		3			/* Italic */
#define HTML_H3		4			/* Heading 3 */
#define HTML_H2		5			/* Heading 2 */
#define HTML_H1		6			/* Heading 1 */
#define HTML_SMALL	7			/* Small text */
#define HTML_BIG	8			/* Big text */
#define HTML_TT		9			/* Typewriter text */
#define HTML_PRE	10			/* Preformated */	
#define HTML_BR		11			/* Break */
#define HTML_HR		12			/* Horizontal rule */
#define HTML_FONT	13			/* Font change */
#define HTML_CRLF	14			/* Newline */
#define HTML_CHAR	15			/* Special character */
#define HTML_P		16			/* Paragraph */

#define HTAG_MAX	5 			/* Maximum number of tags for an html element */

typedef unsigned int htmlcode_t;
typedef struct HtmlUnit_str HtmlUnit;
typedef struct HtmlContext_str HtmlContext;
typedef struct HtmlFont_str HtmlFont;
typedef struct Html_str Html;

/*
 * HtmlFont - Font description
 */
struct HtmlFont_str {
  fontfamily_t		family;			/* Current font family */
  fontprop_t		props;			/* Current font properties */
  fontsize_t		size;			/* Current font size index */
  fontsize_t		points;			/* Current font size in points*/
};

/*
 * HtmlContext - Style information
 *
 * This object contains style information about a text string including the
 * font and any associated hyperlinks or tags.  This object is used both
 * to represent the current state as well as to mark a piece of text.
 *
 */
struct HtmlContext_str {
  HtmlFont		hc_font;		/* Font */
  int			hc_pixel;		/* Pixel color */

  char			*hc_link;		/* Associated hyperlink */
  char			*hc_tag;		/* Associated tag */
  int			hc_preformat;		/* Are we in preformating mode */

  HtmlContext		*hc_next;		/* Next item in stack */

  /* 
   *     These are informational members computed automatically
   */
  Html			*hc_html;		/* Parent html object */
  XFontStruct		*hc_xFont;		/* The XFontStruct font definition */
  int			hc_is16bit;		/* Is this a 16-bit font (e.g., Japanese) */
  int			hc_spaceWidth;		/* Width of a space */
  int			hc_ascent;		/* Ascent of the current font */
  int			hc_descent;		/* Descent of the current font */
};

/*
 * HtmlUnit - Unit of formatted text.
 *
 * This object represents a block of text with the same html properties
 * associated with it.
 *
 */
struct HtmlUnit_str {
  int			hu_type;		/* Type of unit */
  char			*hu_text;		/* Text in the unit */
  int			hu_x,hu_y;		/* Position of unit (relative to block origin) */
  HtmlContext		*hu_context;		/* Context of the unit */
  int			hu_width;		/* Width of this unit */
  Tk_Image		hu_image;		/* Image if this is an image unit */ 

  HtmlUnit		*hu_next;
  HtmlUnit		*hu_prev;
};

/*
 * HtmlTagOpt - An option in an html tag
 */
typedef struct {
  char		*hto_label;
  char		*hto_value;
} HtmlTagOpt;


/*
 * HtmlTag - A parsed html tag
 */
typedef struct {
  char		*ht_name;			/* Name of this tag */
  int		ht_numOptions;			/* Number of options in tag */
  HtmlTagOpt	*ht_options;			/* Options */
} HtmlTag;

/*
 * Html - Block of html formatted text.
 */
struct Html_str {
  Locale	*h_locale;			/* Locale used to construct html */

  TargetDev_e	h_target;			/* Is this formatted for print or x11 display */

  int		h_reqWidth;			/* Requested width */
  int		h_width;			/* Actual width */
  int		h_height;			/* Actual height */
  int		h_isVisible;			/* Are there any visible characters */

  int		h_zoom;				/* Zoom level used for formatting */

  int		h_dataLen;			/* Length of data */
  char		*h_data;			/* Data for html formatting */

  HtmlContext	*h_context;			/* The context stack used in formatting */
  HtmlContext	*h_contextPool;			/* Dequeued but possibly in-use context objects */

  HtmlUnit	*h_head;			/* Pointer to first html unit */
  HtmlUnit	*h_tail;			/* Pointer to last html unit */
};

Html *new_Html(TargetDev_e target);		/* Create a new html object */
void delete_Html(Html*);			/* Delete an html object */
void Html_addLine(Html*,const char *line);	/* Add a line to html object */
void Html_format(Html*);			/* Format the html object */
void Html_draw(Html*,int x,int y);		/* Draw at specified position */
int Html_isHit(Html*,int x,int y);		/* Is a hit at (x,y) inside formatted object? */
const char *Html_getLink(Html*,int x,int y);	/* Get hyperlink referenced at (x,y) */

void Html_psPrint(Html *h,GPrint *P,int x,int y);

int HtmlFont_isEqual(const HtmlFont*,const HtmlFont*);	/* Return non-zero if fonts are equal */
HtmlFont *HtmlFont_init(HtmlFont*,fontfamily_t,fontprop_t,fontsize_t);
void HtmlFont_print(HtmlFont*,FILE*);

int Hyperlink_selectAt(int x,int y);		/* Select a hyperlink at (x,y) in response to mouse down */
int Hyperlink_confirmAt(int x,int y);		/* Confirm a hyperlink at (x,y) in response to mouse up */
void Hyperlink_cancel();			/* Cancel any selected hyperlink */
int Hyperlink_isPending();
const char *Hyperlink_getAt(int x,int y);	/* Return the hyperlink at the specified location */

#define HtmlContext_fontAscent(hc) (hc)->hc_ascent
#define HtmlContext_fontDescent(hc) (hc)->hc_descent


#endif
