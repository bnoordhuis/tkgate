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
#ifndef __modsym_h
#define __modsym_h

/*
 * GModSymPort - Symbol port definition. This is a helping class for
 * the GModSymbol class.
 */
typedef struct {
  char		*msp_name;		/* Name of port */
  int		msp_x,msp_y;		/* Position of port */
  int		msp_type;		/* Type of port (IN, OUT, INOUT) */
  int		msp_orient;		/* Base orientation */
  int		msp_size;		/* Number of bits */
  GPadLoc	msp_loc[NUMORIENTS];	/* Pad info for this port. */
} GSymPort;

/*
 * GMIconData - Data about an icon
 */
typedef struct {
  char		*data;			/* Data for the icon */
  int		x,y;			/* Offset for edit window (and port offset) */
  unsigned	w,h;			/* Size of the icon */
} GMIconData;

/*
 * GModSymbol - Data structure for module symbols
 */
struct GModSymbol_str {
  int	 	ms_gid;			/* MSS ID for icons */
  int		ms_refCount;		/* Reference count of this symbol */
  int		ms_mark;		/* Generic mark */ 
  GMIconData	ms_normal;		/* Data for normal icon */
  GMIconData	ms_select;		/* Data for select icon */
  int		ms_basex,ms_basey;	/* Base x/y point */
  int		ms_cx,ms_cy;		/* Center point */
  int		ms_ix[2][4],ms_iy[2][4]; /* Icon offset by orientation and bold/normal */
  int		ms_finalized;		/* Symbol has been finalized */
  int		ms_numPorts;		/* Number of ports */
  GSymPort	**ms_ports;		/* Array of ports */
};

/*
 * GSymPort methods
 */
GSymPort *new_GSymPort(const char *name,int x,int y,int t,int r,int s);
void delete_GSymPort(GSymPort*);

/*
 * GModSymbol methods
 */
GModSymbol *new_GModSymbol();						/* Create a new module symbol */
GModSymbol *copy_GModSymbol(GModSymbol *);				/* Create a copy of a module symbol */
void GModSymbol_attach(GModSymbol *ms);					/* Attach this symbol to something */
void GModSymbol_detach(GModSymbol *ms);					/* Detach this symbol from somthing */
void GModSymbol_setNormalIcon(GModSymbol *ms,const char*,int,int);
void GModSymbol_setSelectIcon(GModSymbol *ms,const char*,int,int);
const char *GModSymbol_getNormalIcon(GModSymbol *ms,int*,int*);
const char *GModSymbol_getSelectIcon(GModSymbol *ms,int*,int*);
void GModSymbol_addPort(GModSymbol *ms,GSymPort *p);
void GModSymbol_flushPorts(GModSymbol *ms);
void GModSymbol_draw(GModSymbol *ms,int x,int y,int r,int isBold);
int GModSymbol_numPorts(GModSymbol *ms);
void GModSymbol_getExtents(GModSymbol *ms,int r,int *minx,int *miny,int *maxx,int *maxy);
GSymPort *GModSymbol_getPort(GModSymbol *ms,int p);

HashElem *GModSymbol_first();
HashElem *GModSymbol_next(HashElem*);

#endif
