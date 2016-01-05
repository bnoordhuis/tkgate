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
#ifndef __cpath_h
#define __cpath_h

void cpath_clear();
void cpath_flush();
GWire *cpath_findGatePort(const char *Gname,const char *Pname,int n,GModuleDef *M);
GWire *cpath_findPort(const char *name,GModuleDef *M);
int cpath_highlightPath(GWire *w1,GWire *w2);
GWire *cpath_guessPort(GWire *w,GModuleDef *M,int isOut);
void cpath_close();
int cpath_command(const char *C);
void cpath_open();
void cpath_show(int n,const char *pelems[]);
void cpath_reshow();
void cpath_draw(int x1,int y1,int x2,int y2);
void cpath_flash();
void cpath_initNetDelayTable();
void cpath_flushNetDelayTable();
void cpath_registerNetAlias(const char *alias_name,const char *canon_name);
void cpath_registerNetDelay(const char *name,int fd,int bd);
int  cpath_getNetDelay(const char *name,int *fd,int *bd);
void cpath_mouseDown(EditState *es);
void cpath_mouseUp(EditState *es);

#endif
