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
#ifndef __icon_h
#define __icon_h

/*
   Metrics for a gate icon.  (x,y) is the position within a pixmap at
   which the icon is located, (w,h) is the size of the icon, and
   (ox, oy) is the position of the icon origin within the icon.
*/
typedef struct {
  short x,y,w,h,ox,oy;
} iconDimensions;


typedef struct {
  Pixmap	pm;			/* Pixmap icon is stored on (for each zoom value) */
  int		x,y;			/* Position of icon on pixmap */
  int		width,height;		/* Size of icon on pixmap */
  int		ox,oy;			/* Origin position (relative to (x,y)) */
} Icon;

typedef struct {
  Pixmap	p[ZOOM_MAX+1];		/* The pixmaps in the set (by zoom factor) */
  int 		width,height;		/* Base size of pixmap */
} PixmapZoomSet;


Pixmap Pixmap_register(char *name,unsigned char bits[],int w,int h);
Pixmap Pixmap_registerFromFile(char *name,char *file);
Pixmap Pixmap_registerFromFileWithParms(char *name,char *file,int*,int*,int*,int*);
Pixmap Pixmap_find(char *name);
Pixmap Pixmap_zoom(Pixmap,int);
void Pixmap_initZoomSet(Pixmap p,int w,int h);
void Pixmap_deleteZoomSet(Pixmap p);

Icon *new_Icon(Pixmap P,int x,int y,int w,int h,int ox,int oy);
Icon *new_IconID(Pixmap P,iconDimensions *id,int yoffset);
void Icon_draw(Display *D,Window W,GC gc,int x,int y,Icon *ico);

#endif
