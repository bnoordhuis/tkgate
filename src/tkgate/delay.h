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
#ifndef __delay_h
#define __delay_h

typedef struct GDelayDef_str GDelayDef;
struct GDelayDef_str {
  char		*dd_tech;		/* Technology */
  char		*dd_prim;		/* Primitive name */
  Expr		*dd_area;		/* Area expression */
  Expr		*dd_power;		/* Power expression */
  int		dd_numDelays;		/* Number of delay specifications */
  char		*dd_names[MAXDELAYS];	/* Names of delay parameters */
  Expr		*dd_delay[MAXDELAYS];	/* Array of delay expressions */
  GDelayDef	*dd_next;		/* Next in list of delay definitions */
};

int GDelayDef_readFile(const char *file_name);			/* Read a delay definition file */
GDelayDef *GDelayDef_find(const char *tech,const char *prim);	/* Find a delay definition */
GDelayDef *GDelayDef_findList(const char *prim);		/* Find list of delay definitions for a primitive */
GDelayDef *GDelayDef_findTech(GDelayDef *,const char *tech);	/* Find delay definition for a technology from a list */

int GDelayDef_getDelays(GDelayDef *dd, GCElement *g, int *delays);

void GDelayDef_begin(const char *tech,const char *prim);
void GDelayDef_end();

void GDelayDef_flush();

char **GDelayDef_getTechList(); 				/* Get list of defined technologies */

#endif
