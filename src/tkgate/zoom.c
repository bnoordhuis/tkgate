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
#include "tkgate.h"

#define Z TkGate.circuit->zoom_factor

void ZDrawLine(Display *D,Drawable d,GC gc,int x1,int y1,int x2,int y2)
{
  XDrawLine(D,d,gc,Z*x1,Z*y1,Z*x2,Z*y2);
}

void ZDrawLines(Display *D,Drawable d,GC gc,XPoint *points,int npoints,int mode)
{
  if (Z == 1)
    XDrawLines(D,d,gc,points,npoints,mode);
  else {
    int i;
    for (i = 0;i < npoints;i++) {
      points[i].x *= Z;
      points[i].y *= Z;
    }
    XDrawLines(D,d,gc,points,npoints,mode);
    for (i = 0;i < npoints;i++) {
      points[i].x /= Z;
      points[i].y /= Z;
    }
  }
}

void ZDrawRectangle(Display *D, Drawable d,GC gc,int x,int y,unsigned width,unsigned height)
{
  XDrawRectangle(D,d,gc,Z*x,Z*y,Z*width,Z*height);
}

void ZFillRectangle(Display *D, Drawable d,GC gc,int x,int y,unsigned width,unsigned height)
{
  XFillRectangle(D,d,gc,Z*x,Z*y,Z*width,Z*height);
}

void ZDrawString(Display *D,Drawable d,GC gc,int x,int y,char *string,int length)
{
  XDrawString(D,d,gc,Z*x,Z*y,string,length);
}

void ZDrawString16(Display *D,Drawable d,GC gc,int x,int y,XChar2b *string,int length)
{
  XDrawString16(D,d,gc,Z*x,Z*y,string,length);
}

void ZCopyArea(Display *D,Drawable src,Drawable dest,GC gc,int src_x,int src_y,
	       unsigned width,unsigned height,int dest_x,int dest_y)
{
  src = Pixmap_zoom(src,Z); 
  XCopyArea(D,src,dest,gc,Z*src_x,Z*src_y,Z*width,Z*height,Z*dest_x,Z*dest_y);

}

void ZCopyPlane(Display *D,Drawable src,Drawable dest,GC gc,int src_x,int src_y,
		unsigned width,unsigned height,int dest_x,int dest_y,unsigned long plane)
{
  src = Pixmap_zoom(src,Z); 
  XCopyPlane(D,src,dest,gc,Z*src_x,Z*src_y,Z*width,Z*height,Z*dest_x,Z*dest_y,plane);
}


void unZoomMetrics(Tk_FontMetrics *metrics)
{
  metrics->linespace /= Z;
  metrics->ascent /= Z;
  metrics->descent /= Z;
}

