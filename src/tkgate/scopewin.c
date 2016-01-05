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

    Last edit by hansen on Sat Jan  3 22:18:22 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tkgate.h"

#define SST_NONE		0		/* Nothing was selected */
#define SST_TIMESHIFT		1		/* Selected scope for time shifting */
#define SST_TRACE		2		/* Selecting an individual trace */

int scope_active = 0;				/* Flag indicating that the scope is active */

static int scope_selectType = SST_NONE;		/* Type of scope selection */
static int scope_clickValue = 0;		/* x-position of mouse press if time shifting traces */
static GTrace *scope_traceSelect = 0;		/* The selected trace for moving traces up/down */

extern GScope *Scope;

#if !TKGATE_BROKENCONFIGWIDGET
static Tk_ConfigSpec configSpecs[] = {
  {TK_CONFIG_COLOR, "-background", "background", "Background",
   "white", Tk_Offset(GScope,bgColor), 0, 0},
  {TK_CONFIG_SYNONYM, "-bg", "background", 0, 0, 0, 0, 0},
  {TK_CONFIG_SYNONYM, "-fg", "foreground", 0, 0, 0, 0, 0},
  {TK_CONFIG_COLOR, "-foreground", "foreground", "Foreground",
     "black", Tk_Offset(GScope,fgColor), 0, 0},
  {TK_CONFIG_INT, "-height", "height", "Height",
     STR(TKG_GATEWIN_HEIGHT), Tk_Offset(GScope,Height), 0, 0},
  {TK_CONFIG_INT, "-width", "width", "Width",
     STR(TKG_GATEWIN_WIDTH), Tk_Offset(GScope,Width), 0, 0},
  {TK_CONFIG_STRING, "-xscrollcommand", "xscrollcommand",  "Xscrollcommand", 
     "", Tk_Offset(GScope,xscroll), 0, 0},
  {TK_CONFIG_STRING, "-yscrollcommand", "yscrollcommand",  "Yscrollcommand", 
     "", Tk_Offset(GScope,yscroll), 0, 0},
  {TK_CONFIG_END, 0, 0, 0, 0, 0, 0, 0}
};
#endif

static int scopeWinConfigure(Tcl_Interp *tcl, GScope *sw, int argc, const char *argv[], int flags)
{
#if TKGATE_BROKENCONFIGWIDGET
  sw->Width = 500;
  sw->Height = 400;
  sw->xscroll = ".scope.main.horz set";
  sw->yscroll = ".scope.main.vert set";

  Tk_SetWindowBackground(sw->win, XWhitePixelOfScreen(TkGate.S));
#else
  if (Tk_ConfigureWidget(tcl, sw->win, configSpecs, argc, argv, (char*) sw, flags) != TCL_OK)
    return TCL_ERROR;
  Tk_SetWindowBackground(sw->win, sw->bgColor->pixel);
#endif

  Tk_GeometryRequest(sw->win,sw->Width,sw->Height);

  if (sw->gc == None) {
    XGCValues gcv;

    gcv.function = GXcopy;
    gcv.graphics_exposures = False;
    sw->gc = Tk_GetGC(sw->win,GCFunction|GCGraphicsExposures, &gcv);
  }

  return TCL_OK;
}

static void scopeWinEvent(ClientData data, XEvent *E)
{
  if (E->type == DestroyNotify) {
    int old_mode = ob_get_mode();

    scope_active = 0;
    ob_mode(OM_DISABLED);
    ob_clear();
    delete_GScope(Scope);
    ob_mode(old_mode);
    Scope = 0;
  }
  ReqScopeRedisplay();
}

static void scopeYViewCommand(GScope *S,const char *command,const char *arg)
{
  int visTraces = (S->Height-ScopeBOTTOMHEIGHT)/ScopeTRACEHEIGHT;
  int newStart = S->Start;
  double f;
  int n;

  if (strcmp(command,"moveto") == 0) {
    if (S->NumTraces) {
      sscanf(arg,"%lf",&f);
      newStart = S->NumTraces*(f+0.5/(double)S->NumTraces);
    }
  } else if (strcmp(command,"scroll") == 0) {
    sscanf(arg,"%d",&n);
    if (n < 0)
      newStart--;
    else if (n > 0)
      newStart++;
  }
  
  if (newStart >= S->NumTraces-visTraces)
    newStart = S->NumTraces-visTraces;
  if (newStart < 0)
    newStart = 0;
  
  if (newStart != S->Start) {
    S->Start = newStart;
    ReqScopeRedisplay();
  }
}

static void scopeXViewCommand(GScope *S,const char *command,const char *arg)
{
  int newLeftTime = S->s_leftTime;
  double f;
  int n;

  if (strcmp(command,"moveto") == 0) {
    int NI = S->s_interval ? (S->s_time/S->s_interval) : 0;
    if (NI) {
      sscanf(arg,"%lf",&f);
      n = NI*(f+0.5/(double)NI);
      if (n < 0) n = 0;
      if (n > NI)
	n = NI-1;
      newLeftTime = n*S->s_interval;
    }
  } else if (strcmp(command,"scroll") == 0) {
    sscanf(arg,"%d",&n);
    if (n < 0)
      newLeftTime -= S->s_interval;
    else if (n > 0)
      newLeftTime += S->s_interval;
  }
  if (newLeftTime > S->s_time)
    newLeftTime = (S->s_time/S->s_interval)*S->s_interval;
  if (newLeftTime < 0)
    newLeftTime = 0;
  
  if (newLeftTime != S->s_leftTime) {
    S->s_leftTime = newLeftTime;
    ReqScopeRedisplay();
  }
}

static void GScope_setClickTime(GScope *S,int clickValue,double f)
{
  if (clickValue >= Scope->s_range*f)
    S->s_leftTime = ((clickValue - S->s_range*f)/S->s_interval)*S->s_interval;
  else
    S->s_leftTime = 0;
}

static void scopeZoomCommand(GScope *S,const char *type,const char *arg)
{
  int d,r;

  if (strcmp(type,"set") == 0) {
    if (sscanf(arg,"%d",&d) != 1)
      return;

    r = S->s_baseRangePos+d;
  } else {
    r = S->s_rangePos;
    if (sscanf(arg,"%d",&d) == 1)
      r += d;
  }



  if (r < 0) r = 0;
  if (r >= MAXRANGEPOS) r = MAXRANGEPOS-1;

  if (r != S->s_rangePos) {
    extern int RangeVals[];
    int oldMiddle;
    double f;

    if (S->s_time < RangeVals[r]) {
      oldMiddle = 0;
      f = 0.0;
    } else if (S->s_time <= S->s_leftTime + S->s_range) {
      oldMiddle = S->s_time;
      f = ((double)S->s_time-S->s_leftTime)/(double)S->s_range;
    } else {
      oldMiddle = S->s_leftTime + S->s_range/2;
      f = 0.5;
    }

    S->s_rangePos = r;
    S->s_range = RangeVals[S->s_rangePos];
    GScope_pickInterval(&S->s_range,&S->s_interval,S->s_precision);

    GScope_setClickTime(Scope,oldMiddle,f);

    ReqScopeRedisplay();
  }
}

static int scopeWinCommand(ClientData data,Tcl_Interp *tcl,int argc,const char *argv[])
{
  if (strcmp(argv[1],"yview") == 0) {
    scopeYViewCommand(Scope,argv[2],argv[3]);
  } else if (strcmp(argv[1],"xview") == 0) {
    scopeXViewCommand(Scope,argv[2],argv[3]);
  } else if (strcmp(argv[1],"zoom") == 0) {
    scopeZoomCommand(Scope,"inc",argv[2]);
  } else if (strcmp(argv[1],"setzoom") == 0) {
    scopeZoomCommand(Scope,"set",argv[2]);
  }

  return TCL_OK;
}

/*
 * Mouse was pressed on the name part of a logic trace so we should prepare to slide
 * it up and down the list of traces.
 */
static void ScopeWin_traceSelect(int x,int y,int px,int py,int n,int state)
{
  scope_traceSelect = GScope_hitTrace(Scope,y);
  if (scope_traceSelect)
    GScope_setTraceHighlight(Scope,scope_traceSelect);

  /*
   * Right mouse button
   */
  if (n == 3) {
    if (scope_traceSelect)
      DoTcl("PopupMenu::scopePost %d %d",px,py);
  }
}

static void ScopeWin_moveTrace(int x,int y,int n,int state)
{
  if ((state & Button1MotionMask)) {
    if (!scope_traceSelect) return;
    GScope_moveTrace(Scope,scope_traceSelect,y);
    GScope_postFullName(0);
  } else {
    GTrace *t = GScope_hitTrace(Scope,y);
    GScope_postFullName(t);
  }
}

static void ScopeWin_releaseTrace(int x,int y,int n,int state)
{
  if (scope_traceSelect)
    GScope_moveTrace(Scope,scope_traceSelect,y);
  scope_traceSelect = 0;
}

/*
 * Mouse was pressed on the logic trace part of the traces so we should prepare to slide
 * the time line. 
 */
static void ScopeWin_timeSelect(int x,int y,int n,int state)
{
  int pmin = ScopeLEFTMARGIN;
  int pmax = Scope->Width-ScopeRIGHTMARGIN;
  extern int RangeVals[];
  double f;

  GScope_setTraceHighlight(Scope,0);

#if 0
  if (x < pmin || x > pmax) {
    scope_clickValue = -1;
    return;
  }
#endif

  f = ((double)x-pmin)/(double)(pmax-pmin);
  scope_clickValue = Scope->s_leftTime + Scope->s_range*f;

#if 1
  if (scope_clickValue > Scope->s_time)
    scope_clickValue = Scope->s_time;
#endif

  if ((state & ControlMask)) {
    int r = 0;

    if (n == 1)
      r = Scope->s_rangePos-1;
    else if (n == 3)
      r = Scope->s_rangePos+1;
    
    if (r < 0) r = 0;
    if (r >= MAXRANGEPOS) r = MAXRANGEPOS-1;

    Scope->s_rangePos = r;
    Scope->s_range = RangeVals[Scope->s_rangePos];
    GScope_pickInterval(&Scope->s_range,&Scope->s_interval,Scope->s_precision);

    GScope_setClickTime(Scope,scope_clickValue,f);
  } else {
    if (n == 1) {
      GScope_setClickTime(Scope,scope_clickValue,f);
    } else if (n == 3) {
      GScope_setMark(Scope,x,1,state);
    }
  }

  ReqScopeRedisplay();
}

/*
 * Mouse motion that should slide the timeline.
 */
static void ScopeWin_moveTime(int x,int y,int n,int state)
{
  int pmin = ScopeLEFTMARGIN;
  int pmax = Scope->Width-ScopeRIGHTMARGIN;
  double f;

  GScope_drawCrossLine(Scope,x);

  /*  printf("scopeButtonMotion(%d, %d)\n",x,y);*/
  if (Scope->mark_count == 1) {
    GScope_moveSelection(Scope);
  }

  if (!(state & Button1MotionMask)) return;

  if (x < pmin || x > pmax || scope_clickValue < 0) return;
  f = ((double)x-pmin)/(double)(pmax-pmin);

  GScope_setClickTime(Scope,scope_clickValue,f);

  ReqScopeTraceRedisplay();
}

static void ScopeWin_releaseTime(int x,int y,int n,int state)
{
  ScopeWin_moveTime(x,y,n,state);
  if (n == 1)
    ReqScopeRedisplay();
  else if (n == 3)
    GScope_setMark(Scope,x,0,state);
}


void scopeButtonPress(int x,int y,int px,int py,int n,int state)
{
  GScope_postFullName(0);

  if (x < ScopeLEFTMARGIN) {
    scope_selectType = SST_TRACE;
    ScopeWin_traceSelect(x,y,px,py,n,state);
  } else if (x < (Scope->Width-ScopeRIGHTMARGIN)) {
    scope_selectType = SST_TIMESHIFT;
    ScopeWin_timeSelect(x,y,n,state);
  }  else {
    scope_selectType = SST_NONE;
    scope_clickValue = -1;
  }
}

void scopeButtonMotion(int x,int y,int px,int py,int n,int state)
{
  switch (scope_selectType) {
  case SST_TRACE :
    ScopeWin_moveTrace(x,y,n,state);
    GScope_postFullName(0);
    break;
  case SST_TIMESHIFT :
    ScopeWin_moveTime(x,y,n,state);
    GScope_postFullName(0);
    break;
  default:
    if (x < ScopeLEFTMARGIN)
      ScopeWin_moveTrace(x,y,n,state);
    else {
      ScopeWin_moveTime(x,y,n,state);
      GScope_postFullName(0);
    }
  }
}

void scopeButtonRelease(int x,int y,int px,int py,int n,int state)
{
  switch (scope_selectType) {
  case SST_TRACE :
    ScopeWin_releaseTrace(x,y,n,state);
    break;
  case SST_TIMESHIFT :
    ScopeWin_releaseTime(x,y,n,state);
    break;
  default:
    if (x < ScopeLEFTMARGIN)
      ScopeWin_releaseTrace(x,y,n,state);
    else
      ScopeWin_releaseTime(x,y,n,state);
  }

  scope_selectType = SST_NONE;
}



void scopeWinDestroy(ClientData data)
{
}

/*****************************************************************************
 *
 *   Create a scope window
 *
 *****************************************************************************/
int gat_scope(ClientData data, Tcl_Interp *tcl, int argc, const char *argv[])
{
  Tk_Window root = (Tk_Window) data;
  GScope *sw;
  Tk_Window w;

  if (argc < 2) {
    Tcl_AppendResult(tcl, "wrong # args: should be \"",argv[0], " pathName ?options?\"", 0);
    return TCL_ERROR;
  }

  w = Tk_CreateWindowFromPath(tcl, root, argv[1], 0);
  if (!w) {
    printf("gat_scope had window error\n");
    return TCL_ERROR;
  }
  Tk_MakeWindowExist(w);
  Tk_SetClass(w, "ScopeWin");

  sw = new_GScope(TkGate.circuit->simulator.si_precision);
  sw->win = w;
  sw->d = Tk_Display(w);
  sw->tcl = tcl;
  sw->x = sw->y = 0;
  sw->Width = TKG_GATEWIN_WIDTH;
  sw->Height = TKG_GATEWIN_HEIGHT;
  sw->gc = None;
  sw->bgColor = 0;
  sw->fgColor = 0;
  sw->xscroll = 0;
  sw->yscroll = 0;

  Scope = sw;
  TkGate.ScopeW = Tk_WindowId(sw->win);

  Tk_CreateEventHandler(w, ExposureMask|StructureNotifyMask, scopeWinEvent, sw);
  Tcl_CreateCommand(tcl, Tk_PathName(w), scopeWinCommand, sw, scopeWinDestroy);
  if (scopeWinConfigure(tcl, sw, argc-2, argv+2,0) != TCL_OK) {
    Tk_DestroyWindow(w);
    return TCL_ERROR;
  }

  tcl->result = Tk_PathName(w);

  scope_active = 1;

  return TCL_OK;
}
