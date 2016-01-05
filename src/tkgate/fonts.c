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

    Last edit by hansen on Thu Feb 12 14:48:59 2009
****************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdarg.h>
#include "tkgate.h"

/*
 * Table of all the fonts we have loaded.  The indexes represent:
 *   [FF_MAX]			Font family
 *   [FP_MAX]			Point sizes
 *   [FS_MAX]			Font styles
 *   [ZOOM_MAX]			Zoom settings
 */
static XFontStruct *xfonts[FF_MAX][FP_MAX][FS_MAX][ZOOM_MAX+1] = {{{{0}}}};

/*
 * Names of the font families we support
 */
static char *font_family_names[FF_MAX] = {
  "courier",
  "helvetica",
  "times",
  "symbol",
  "kanji",
};

/*
 * Font sizes we support.  These correspond to the 7 font sizes levels in html.
 */
static int font_sizes[FS_MAX] = {8, 8, 10, 12, 14, 18, 24};
static int kanji_font_sizes[FS_MAX] = {9, 9, 11, 13, 15, 18, 25};

/*
 * Translate a font size code to a point size
 */
int getFontSize(int fs)
{
  return font_sizes[fs];
}

/*
 * Translate a kanji font size code to a point size
 */
int getKanjiFontSize(int fs)
{
  return kanji_font_sizes[fs];
}

/*
 * Translate a font family code to a font family name
 */
const char *getFontFamilyName(fontfamily_t ff)
{
  return font_family_names[ff];
}

/*
 * Get the x11 font name for the font with the requested properties and zoom-level
 */
void getFontName(char *fullName,fontfamily_t ff,fontprop_t fp,fontsize_t fs,int zoom)
{
  char *font_weight = (fp & FP_BOLD) ? "bold" : "medium";
  char *font_posture;

  if (ff == FF_TIMES) 
    font_posture = (fp & FP_ITALIC) ? "i" : "r";
  else
    font_posture = (fp & FP_ITALIC) ? "o" : "r";


  switch (ff) {
  case FF_KANJI :
    sprintf(fullName,
	    "-misc-fixed-medium-r-normal--*-%d-75-75-c-*-jisx0208.1983-0",
	    kanji_font_sizes[fs]*10*zoom);
    break;
  case FF_SYMBOL :
    sprintf(fullName,
	    "-adobe-symbol-medium-r-normal--%d-*-*-*-*-*-*",
 	    font_sizes[fs]*zoom);
    break;
  default :
    sprintf(fullName,
	    "-adobe-%s-%s-%s-normal--%d-*-*-*-*-*-%s",
	    font_family_names[ff],
	    font_weight,
	    font_posture,
 	    font_sizes[fs]*zoom,
	    TkGate.locale->l_encFont);
    break;
  }
}

XFontStruct *GetXFont(fontfamily_t ff,fontprop_t fp,fontsize_t fs,int zoom)
{
  char fontName[STRMAX];
  XFontStruct *xfs;

  getFontName(fontName,ff,fp,fs,zoom);

  xfs = XLoadQueryFont(TkGate.D,fontName);
  if (!xfs) xfs = XLoadQueryFont(TkGate.D,"fixed");
  if (!xfs) xfs = XLoadQueryFont(TkGate.D,"*");

  return xfs;
}

XFontStruct **GetXFonts(fontfamily_t ff,fontprop_t fp,fontsize_t fs)
{
  int z;

  for (z = 1;z <= ZOOM_MAX;z++) {
    xfonts[ff][fp][fs][z] = GetXFont(ff,fp,fs,z);
  }

  return xfonts[ff][fp][fs];
}

void UnloadAllFonts()
{
  fontfamily_t ff;
  fontprop_t fp;
  fontsize_t fs;
  int z;

  for (ff = 0;ff < FF_MAX; ff++) {
    for (fp = 0;fp < FP_MAX; fp++) {
      for (fs = 0;fs < FS_MAX; fs++) {
	for (z = 1;z <= ZOOM_MAX; z++) {
	  if (xfonts[ff][fp][fs][z]) {
	    /*	    Tk_FreeFont(tkfonts[ff][fp][fs][z]);*/
	  }
	}
      }
    }
  }
}

