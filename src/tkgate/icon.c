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

    Last edit by hansen on Tue Jan 20 14:47:22 2009
****************************************************************************/
#include <stdlib.h>
#include "tkgate.h"
#include <X11/Xutil.h>

#define DEBUG_ICON 0

static SHash *icon_pixmaps = 0;			/* Table of pixmaps used for gate icons */
static NHash *zoomed_pixmaps = 0;		/* Table of zoomed versions of icon_pixmaps */

void init_iconTables()
{
  icon_pixmaps = new_SHash_noob();
  zoomed_pixmaps = new_NHash_noob();
}

void Pixmap_initZoomSet(Pixmap p,int w,int h)
{
  int i;
  PixmapZoomSet *zset;

  zset = (PixmapZoomSet*)malloc(sizeof(PixmapZoomSet));
  for (i = 0;i < ZOOM_MAX+1;i++)
    zset->p[i] = 0;
  zset->p[1] = p;
  zset->width = w;
  zset->height = h;
  NHash_insert(zoomed_pixmaps,p,zset);
}

void Pixmap_deleteZoomSet(Pixmap p)
{
  PixmapZoomSet *zset;
  int i;

  zset = (PixmapZoomSet*)NHash_find(zoomed_pixmaps,p);
  if (!zset) return;

  NHash_remove(zoomed_pixmaps,p);
  
  for (i = 0;i < ZOOM_MAX+1;i++)
    if (zset->p[i]) {
      printf("XFreePixmap(%x)\n",(unsigned)zset->p[i]);
      XFreePixmap(TkGate.D,zset->p[i]);
    }
  free(zset);
}


Pixmap Pixmap_register(char *name,unsigned char bits[],int w,int h)
{
  Pixmap *P;

  if (!TkGate.tcl) return None;

  if (SHash_find(icon_pixmaps,name))
    return None;

  P = (Pixmap*) malloc(sizeof(Pixmap));
  *P = XCreatePixmapFromBitmapData(TkGate.D,TkGate.root,bits,w,h,1,0,1);
  SHash_insert(icon_pixmaps,name,P);
  Pixmap_initZoomSet(*P,w,h);

  return *P;
}

Pixmap Pixmap_registerFromFileWithParms(char *name,char *file,int *pw,int *ph,int *px,int *py)
{
  static const char *base_dir = 0;
  Pixmap *P;
  int w,h,x,y;
  char buf[STRMAX];

  if (!TkGate.tcl) return None;


  if (!base_dir) {
    base_dir = Tcl_GetVar(TkGate.tcl,"bd",TCL_GLOBAL_ONLY);
    if (!base_dir) base_dir = "images";
  }

  sprintf(buf,"%s/%s",base_dir,file);

  if (SHash_find(icon_pixmaps,name))
    return None;

  P = (Pixmap*) malloc(sizeof(Pixmap));

  if (XReadBitmapFile(TkGate.D,TkGate.root,buf,&w,&h,P,&x,&y) != BitmapSuccess) {
    fprintf(stderr,"Failed to find required bitmap '%s'\n",buf);
    exit(1);
    return None;
  }

  SHash_insert(icon_pixmaps,name,P);
  Pixmap_initZoomSet(*P,w,h);

  if (pw) *pw = w;
  if (ph) *ph = h;
  if (px) *px = x;
  if (py) *py = y;

  return *P;
}

Pixmap Pixmap_registerFromFile(char *name,char *file)
{
  return Pixmap_registerFromFileWithParms(name,file,0,0,0,0);
}


Pixmap Pixmap_find(char *name)
{
  Pixmap *P = (Pixmap*) SHash_find(icon_pixmaps,name);

  if (!P) return None;

  return *P;
}

/*
 * Create a copy of pixmap p magnified by scale factor zp
 */
Pixmap Pixmap_createZoomPixmap(Pixmap p,int w,int h,int zp)
{
  XImage *SI = XGetImage(TkGate.D,p,0,0,w,h,0x1,XYPixmap);
  Pixmap DP = XCreatePixmap(TkGate.D,TkGate.root,zp*w,zp*h,1);
  XImage *DI = XGetImage(TkGate.D,DP,0,0,zp*w,zp*h,0x1,XYPixmap);
  int x,y;

  for (x = 0;x < w;x++) {
    for (y = 0;y < h;y++) {
      int v = XGetPixel(SI,x,y);
      int a,b;

      for (a = 0;a < zp;a++)
	for (b = 0;b < zp;b++)
	  XPutPixel(DI,x*zp+a,y*zp+b,v);
    }
  }

  XPutImage(TkGate.D,DP,TkGate.bitGC,DI,0,0,0,0,zp*w,zp*h);


  XDestroyImage(DI);
  XDestroyImage(SI);

  return DP;
}


/*
 * Find and/or create zoomed pixmap
 */
Pixmap Pixmap_zoom(Pixmap p,int z)
{
  PixmapZoomSet *zset;
  if (z == 1) return p;

  zset = (PixmapZoomSet*)NHash_find(zoomed_pixmaps,p);
  if (!zset) return None;
  if (!zset->p[z]) {
    zset->p[z] = Pixmap_createZoomPixmap(p,zset->width,zset->height,z);
  }

  return zset->p[z];
}

Icon *new_Icon(Pixmap P,int x,int y,int w,int h,int ox,int oy)
{
  Icon *I = (Icon*) malloc(sizeof(Icon));

  I->pm = P;
  I->x = x;
  I->y = y;
  I->width = w;
  I->height = h;
  I->ox = ox;
  I->oy = oy;

#if DEBUG_ICON
  printf("create icon [%p p=(%d,%d) %dx%d o=(%d,%d) ]\n",
	 I->pm,I->x,I->y,I->width,I->height,I->ox,I->oy);
#endif
  return I;
}

Icon *new_IconID(Pixmap P,iconDimensions *id,int yoffset)
{
  return new_Icon(P,id->x,id->y+yoffset,id->w,id->h,id->ox,id->oy);
}

void Icon_draw(Display *D,Window W,GC gc,int x,int y,Icon *I)
{
#if DEBUG_ICON
  printf("draw icon [%p p=(%d,%d) %dx%d o=(%d,%d) ] @(%d, %d) \n",
	 I->pm,I->x,I->y,I->width,I->height,I->ox,I->oy,x,y);
#endif
  ZCopyPlane(D,I->pm,W,gc,I->x,I->y,I->width,I->height,x-I->ox,y-I->oy,0x1);
}

