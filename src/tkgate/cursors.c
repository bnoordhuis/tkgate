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
#include <stdlib.h>
#include <assert.h>
#include <X11/cursorfont.h>
#define BEAK_MODES
#include "tkgate.h"

#define DEBUG_CURSORS 0		/* 0,1 or 2 */


/*
 * IDS for groups of cursors
 */
#define DEFAULT_CURS	-1
#define SMALL_CURS	0
#define BIG_CURS	1
#define PORT_CURS	2
#define STREK_CURS	3
#define TREK_CURS	4
#define BAT_CURS	5

/*****************************************************************************
 *
 * Description of a cursor.
 *
 *****************************************************************************/
typedef struct {
  int			code;		/* code number for cursor */
  int			pmap;		/* pixmap cursor is on */
  iconDimensions	icon;		/* icon dimensions for cursor */
} CursorDef;

/*****************************************************************************
 *
 * Description of a pixmap with cursors in it.
 *
 *****************************************************************************/
typedef struct {
  int			pm_code;
  char			*pm_name;
  char			*pm_file;
} CursorPMDef;

/*****************************************************************************
 *
 * Table of cursor pixmap files
 *
 *****************************************************************************/
static CursorPMDef cursorPMs[] = {
  {SMALL_CURS,	"smallcurs",	"smallcurs.b"},
  {BIG_CURS,	"bigcurs",	"bigcurs.b"},
  {PORT_CURS,	"portcurs",	"portcurs.b"},
  {STREK_CURS,	"strekcurs",	"strek.b"},
  {TREK_CURS,	"trekcurs",	"trek.b"},
  {BAT_CURS,	"batcurs",	"bat.b"},
};
static int numPMs = sizeof(cursorPMs)/sizeof(cursorPMs[0]);

static CursorDef bigCursors[] = {
  {ARROWCURSOR,DEFAULT_CURS,	{0,0,0,0,0,0}},
  {OPENCUTTERS,BIG_CURS,	{0,0,22,26,3,3}},
  {CLOSEDCUTTERS,BIG_CURS,	{23,0,22,23,3,3}},
  {IRON,BIG_CURS,		{0,26,31,26,0,25}},
  {INVERTUPCURSOR,BIG_CURS,	{32,24,30,24,2,15}},
  {INVERTDNCURSOR,BIG_CURS,	{63,19,31,24,3,15}},
  {DELETEUPCURSOR,BIG_CURS,	{0,54,25,21,13,20}},
  {DELETEDNCURSOR,BIG_CURS,	{49,60,16,16,8,15}},
  {PENCIL,BIG_CURS,		{26,49,21,21,0,21}},
  {DOWNARROW,SMALL_CURS,	{0,51,16,16,8,16}},
  {UPARROW,SMALL_CURS,		{17,51,16,16,8,16}},
  {TRIARROW,SMALL_CURS,		{34,51,16,16,8,16}},
  {CHANGEDIRCURSOR,SMALL_CURS,	{51,51,16,16,8,16}},
  {REPCURSOR,BIG_CURS,		{50,47,24,12,10,6}},
  {SIZECURSOR,BIG_CURS,		{46,0,26,19,0,10}},
  {WRENCHCURSOR,SMALL_CURS,	{17,17,16,16,3,3}},
  {TEXTCURSOR,SMALL_CURS,	{68,0,16,16,8,8}},
  {ARROW0,SMALL_CURS,		{68,17,16,16,8,8}},
  {ARROW90,SMALL_CURS,		{68,51,16,16,8,8}},
  {ARROW180,SMALL_CURS,		{51,34,16,16,8,8}},
  {ARROW270,SMALL_CURS,		{68,34,16,16,8,8}},
  {BATCURSOR1,BAT_CURS,		{0,0,36,24,18,12}},
  {BATCURSOR2,BAT_CURS,		{37,0,25,24,13,12}},
  {BATCURSOR3,BAT_CURS,		{63,0,18,24,10,12}},
  {TREKDELETEUP,TREK_CURS,	{0,35,77,34,3,29}},
  {TREKDELETEDN,TREK_CURS,	{0,0,77,34,3,29}},
  {ADDPORTTOP,PORT_CURS,	{17,0,15,15,7,14}},
  {ADDPORTBOTTOM,PORT_CURS,	{0,0,15,15,7,0}},
  {ADDPORTLEFT,PORT_CURS,	{17,17,15,15,14,7}},
  {ADDPORTRIGHT,PORT_CURS,	{0,17,15,15,0,7}},
  {SCROLLCURSOR,SMALL_CURS, 	{0,68,16,16,8,8}},
  {-1}
};

static CursorDef smallCursors[] = {
  {ARROWCURSOR,DEFAULT_CURS,	{0,0,0,0,0,0}},
  {OPENCUTTERS,SMALL_CURS,	{0,0,16,16,1,1}},
  {CLOSEDCUTTERS,SMALL_CURS,	{17,0,16,16,1,1}},
  {IRON,SMALL_CURS,		{34,0,16,16,0,16}},
  {INVERTUPCURSOR,SMALL_CURS,	{51,0,16,16,2,6}},
  {INVERTDNCURSOR,SMALL_CURS,	{51,0,16,16,2,6}},
  {DELETEUPCURSOR,SMALL_CURS,	{34,17,16,16,8,16}},
  {DELETEDNCURSOR,SMALL_CURS,	{51,17,16,16,5,11}},
  {PENCIL,SMALL_CURS,		{0,34,16,16,0,16}},
  {DOWNARROW,SMALL_CURS,	{0,51,16,16,8,16}},
  {UPARROW,SMALL_CURS,		{17,51,16,16,8,16}},
  {TRIARROW,SMALL_CURS,		{34,51,16,16,8,16}},
  {CHANGEDIRCURSOR,SMALL_CURS,	{51,51,16,16,8,16}},
  {REPCURSOR,SMALL_CURS,	{17,34,16,9,8,5}},
  {SIZECURSOR,SMALL_CURS,	{34,34,16,9,0,4}},
  {WRENCHCURSOR,SMALL_CURS,	{17,17,16,16,3,3}},
  {TEXTCURSOR,SMALL_CURS,	{68,0,16,16,8,8}},
  {ARROW0,SMALL_CURS,		{68,17,16,16,8,8}},
  {ARROW90,SMALL_CURS,		{68,51,16,16,8,8}},
  {ARROW180,SMALL_CURS,		{51,34,16,16,8,8}},
  {ARROW270,SMALL_CURS,		{68,34,16,16,8,8}},
  {BATCURSOR1,BAT_CURS,		{0,24,16,16,0,0}},
  {BATCURSOR2,BAT_CURS,		{17,24,16,16,0,0}},
  {BATCURSOR3,BAT_CURS,		{34,24,16,16,0,0}},
  {TREKDELETEUP,STREK_CURS,	{0,0,16,16,0,7}},
  {TREKDELETEDN,STREK_CURS,	{17,0,16,16,0,7}},
  {ADDPORTTOP,PORT_CURS,	{17,0,15,15,7,14}},
  {ADDPORTBOTTOM,PORT_CURS,	{0,0,15,15,7,0}},
  {ADDPORTLEFT,PORT_CURS,	{17,17,15,15,14,7}},
  {ADDPORTRIGHT,PORT_CURS,	{0,17,15,15,0,7}},
  {SCROLLCURSOR,SMALL_CURS, 	{0,68,16,16,8,8}},
  {-1}
};

static Cursor LastCursor;

NHash *GateCursors = 0;

int mouseoverState = CANCELMOUSEOVER;


void Cursor_register(int id,Cursor C)
{
  Cursor *pC = (Cursor*)ob_malloc(sizeof(Cursor),"Cursor");
  *pC = C;
  NHash_insert(GateCursors,id,pC);
}

Cursor Cursor_find(int id)
{
  Cursor *pC = (Cursor*)NHash_find(GateCursors,id);
  if (!pC) return None;
  return *pC;
}

void init_cursors()
{
  XColor FGC,BGC,FGCreal,BGCreal;
  int MaxW,MaxH;
  Cursor default_cursor = XCreateFontCursor(TkGate.D,XC_top_left_arrow);
  CursorDef *CSet;
  int i;

  GateCursors = new_NHash();

  for (i = 0;i < numPMs;i++) {
    CursorPMDef *pmd = &cursorPMs[i];
    assert(pmd->pm_code == i);
#if 0
    Pixmap_register(pmd->pm_name,pmd->bits,pmd->width,pmd->height);
#endif
    Pixmap_registerFromFile(pmd->pm_name,pmd->pm_file);
  }

  XQueryBestCursor(TkGate.D,TkGate.root,32,32,&MaxW,&MaxH);
#if DEBUG_CURSORS
    printf("[Max cursor size is %dx%d]\n",MaxW,MaxH);
#endif
  if (MaxW >= 32 || MaxH >= 32) {
#if DEBUG_CURSORS
    printf("using bigCursors\n");
#endif
    CSet = bigCursors;
  } else {
    if (MaxW < 16 && MaxH < 16) {
      printf("[Warning: hardware cursors limited to "
	     "%dx%d (need at least 16x16).]\n",MaxW,MaxH);
    }
#if DEBUG_CURSORS
    printf("using smallCursors\n");
#endif
    CSet = smallCursors;
  }

  XAllocNamedColor(TkGate.D,TkGate.CM,"black",&FGCreal,&FGC);
  XAllocNamedColor(TkGate.D,TkGate.CM,"white",&BGCreal,&BGC);

  for (;CSet->code >= 0;CSet++) {
    Cursor C;

    if (CSet->pmap == DEFAULT_CURS || CSet->icon.w <= 0) {
      C = default_cursor;
#if DEBUG_CURSORS
      printf("curor %d: default\n",CSet->code);
#endif
    } else {
      Pixmap iconPM = Pixmap_find(cursorPMs[CSet->pmap].pm_name);
      Pixmap srcPM = XCreatePixmap(TkGate.D,TkGate.root,CSet->icon.w,CSet->icon.h,1);
      XCopyArea(TkGate.D,iconPM,srcPM,TkGate.bitGC,
		CSet->icon.x,CSet->icon.y,
		CSet->icon.w,CSet->icon.h,0,0);

      C = XCreatePixmapCursor(TkGate.D,srcPM,srcPM,&FGCreal,&BGCreal,
			      CSet->icon.ox,CSet->icon.oy);
      XFreePixmap(TkGate.D,srcPM);
#if DEBUG_CURSORS
      printf("curor %d: @(%d, %d) %dx%d = %x\n",CSet->code,
	     CSet->icon.x,CSet->icon.y,CSet->icon.w,CSet->icon.h,C);
#endif
    }
    Cursor_register(CSet->code,C);
  }
#if DEBUG_CURSORS
  printf("cursors done.\n");
#endif
}

void wm_SetCursor(int id)
{
  Cursor C = Cursor_find(id);

#if DEBUG_CURSORS > 1
  printf("wm_SetCursor(%d) = %x\n",id,C);
#endif
  mouseoverState = CANCELMOUSEOVER;
  XDefineCursor(TkGate.D,TkGate.W,C);
  LastCursor = C;
  XFlush(TkGate.D);
}

void PushCursor(int id)
{
  Cursor C = Cursor_find(id);

#if DEBUG_CURSORS > 1
  printf("PushCursor(%d) = %x\n",id,C);
#endif

  mouseoverState = CANCELMOUSEOVER;
  XDefineCursor(TkGate.D,TkGate.W,C);
  LastCursor = C;
  XFlush(TkGate.D);
}

void PopCursor()
{
#if DEBUG_CURSORS > 1
  printf("PopCursor() = %x\n",LastCursor);
#endif

  XDefineCursor(TkGate.D,TkGate.W,LastCursor);
  XFlush(TkGate.D);
}

void MouseoverCursor(int request)
{
  static Cursor link_cursor = 0;

  if (mouseoverState == request) return;
  mouseoverState = request;

  switch (request) {
  case CANCELMOUSEOVER :
    PopCursor();
    break;
  case HYPERLINKCURSOR :
    if (link_cursor == 0)
      link_cursor = XCreateFontCursor(TkGate.D, XC_hand2);

    XDefineCursor(TkGate.D,TkGate.W, link_cursor);
    XFlush(TkGate.D);
    break;
  default :
    {
      Cursor C = Cursor_find(request);
      XDefineCursor(TkGate.D,TkGate.W,C);
      XFlush(TkGate.D);
    }
    break;
  }
}

