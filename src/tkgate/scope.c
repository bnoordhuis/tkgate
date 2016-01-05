/****************************************************************************
    Copyright (C) 1987-2009 by Jeffery P. Hansen

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

    Last edit by hansen on Sat Apr 11 15:53:44 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tkgate.h"

#define SKIPFIELD(C) { \
    for (;*C && isspace(*C);C++); \
    for (;*C && !isspace(*C);C++); \
    }

int RangeVals[MAXRANGEPOS] = {
  10,20,50,
  100,200,500,
  1000,2000,5000,
  10000,20000,50000,
  100000,200000,500000,
  1000000,2000000,5000000,
};

GScope *Scope = NULL;

static GTrace *scope_highlightedTrace = 0;

void idleGateWin(ClientData);

/*****************************************************************************
 *
 * Convert an x screen coordinate to a simulation time value
 *
 * Parameters:
 *      S		Scope object
 *	x		x screen coordinate in scope window
 *
 * Returns:		Simulation time
 *
 *****************************************************************************/
simtime_t GScope_x2t(GScope *S,int x)
{
  int pmin = ScopeLEFTMARGIN;
  int pmax = S->Width-ScopeRIGHTMARGIN;
  double f = ((double)x-pmin)/(double)(pmax-pmin);
  return (simtime_t)(S->s_leftTime + S->s_range*f);
}

/*****************************************************************************
 *
 * Convert a simulation time to an x screen coordinate
 *
 * Parameters:
 *      S		Scope object
 *	t		Simulation time
 *
 * Returns:		x screen coordinate in scope window
 *
 *****************************************************************************/
int GScope_t2x(GScope *S,simtime_t t)
{
  int pmin = ScopeLEFTMARGIN;
  int pmax = S->Width-ScopeRIGHTMARGIN;
  double f = ((double)t - S->s_leftTime)/(double)S->s_range;

  return (int) (f*(pmax-pmin)+pmin);
}

/*****************************************************************************
 *
 * Request a complete redraw of the scope window
 *
 *****************************************************************************/
void ReqScopeRedisplay()
{
  TkGate.idle_ev.scope_redraw = 1;

  if (!TkGate.idle_ev.pending) {
    TkGate.idle_ev.pending = 1;
    Tk_DoWhenIdle(idleGateWin,0);
  }
}

/*****************************************************************************
 *
 * Request a redraw of only the traces and the left origin
 *
 *****************************************************************************/
void ReqScopeTraceRedisplay()
{
  TkGate.idle_ev.trace_redraw = 1;

  if (!TkGate.idle_ev.pending) {
    TkGate.idle_ev.pending = 1;
    Tk_DoWhenIdle(idleGateWin,0);
  }
}

/*****************************************************************************
 *
 * Return non-zero if value V is a transition
 *
 * Parameters:
 *      s		Scope object
 *      v		Trace value object 
 *      n		Number of bits of trace
 *
 *****************************************************************************/
int isTransition(GScope *s,GValue *v,int n)
{
  if (!v || !v->v_next) return 0;

  if (n == 1) {
    if (v->v_next->v_time <= s->s_leftTime) return 0;
    if (v->v_next->v_time > s->s_leftTime + s->s_range) return 0;
    if (v->v_code == VC_UNRECORDED) return 0;
    if (v->v_next->v_code == VC_UNRECORDED) return 0;
    if (v->v_code == v->v_next->v_code) return 0;
  } else {
    if (v->v_next->v_time < s->s_leftTime) return 0;
    if (v->v_next->v_time > s->s_leftTime + s->s_range) return 0;
  }
  
  return 1;
}

/*****************************************************************************
 *
 * Adjust scope so that T->Current is the first change before LTIme
 *
 * Parameters:
 *      T		Trace to be adjusted
 *      LTime		Time at left side of scope window
 *
 *****************************************************************************/
void GTrace_adjust(GTrace *T,simtime_t LTime)
{
  while (T->t_current->v_next && T->t_current->v_next->v_time <= LTime) {
    T->t_current = T->t_current->v_next;
  }
  while (T->t_current->v_prev && T->t_current->v_time > LTime) {
    T->t_current = T->t_current->v_prev;
  }
}

/*****************************************************************************
 *
 * Get screen x position for a value change
 *
 * Parameters:
 *	S		Scope object
 *	V		Value object
 *	LTime		Time at left side of scope window
 *
 *****************************************************************************/
int GScope_getXPos(GScope *S,GValue *V,simtime_t LTime)
{
  simtime_t T;

  if ((!V) || (V->v_time > S->s_leftTime + S->s_range)) {
    if (S->s_time - S->s_leftTime > S->s_range)
      T = S->s_leftTime + S->s_range;
    else
      T = S->s_time;
  } else if (V->v_time < LTime)
    T = LTime;
  else
    T = V->v_time;

  if (T < S->s_leftTime)  T = S->s_leftTime;

  return ScopeLEFTMARGIN + (T - S->s_leftTime)*S->s_scale;
}

/*****************************************************************************
 *
 * Get GC for color to use with value
 *
 * Parameters:
 *	V		Value object
 *	nbits		Number of bits in trace
 *
 *****************************************************************************/
GC GValue_getColor(GValue *V,int nbits)
{
  switch (V->v_code) {
  case VC_UNRECORDED :
  case VC_UNKNOWN :
  case VC_CONTENTION :
  case VC_LOW :
  case VC_HIGH :
  default :
    return TkGate.scopeUnknownGC;
  case VC_FLOAT :
    return TkGate.scopeFloatGC;
  case VC_ONE :
    if (nbits == 1) return TkGate.scopeOneGC;
    break;
  case VC_ZERO :  
    if (nbits == 1) return TkGate.scopeZeroGC;
    break;
  }

  if (V->v_hexValue) {
    if (strchr(V->v_hexValue,'x'))
      return TkGate.scopeUnknownGC;
    else if (*V->v_hexValue == '0')
      return TkGate.scopeZeroGC;
  }
  return TkGate.scopeOneGC;
}

void GTrace_drawTransValue(GTrace *T,GValue *V,GScope *S,int y,
			   simtime_t LTime,int isFullUpdate)
{
  int x,nextX,width;
  char *new_dpy = 0;
  GC gc;

  if (!V) return;

  x = GScope_getXPos(S,V,LTime);

  if (V->v_next)
    nextX = GScope_getXPos(S,V->v_next,LTime);
  else
    nextX = -1;

  if (!V->v_hexValue)
    new_dpy = "";
  else {
    width = GKTextWidth(TkGate.stextXF[1],V->v_hexValue,strlen(V->v_hexValue));

    if (nextX < 0 || width <= nextX-x-3)
      new_dpy = V->v_hexValue;
    else if (S->hash_width <= nextX-x-3)
      new_dpy = "#";
    else
      new_dpy = "";
  }

  if (isFullUpdate && V->v_dpyHexValue) {
    ob_free(V->v_dpyHexValue);
    V->v_dpyHexValue = 0;
  }

  if (V->v_dpyHexValue && (strcmp(V->v_dpyHexValue,new_dpy) != 0)) {
    char *s = V->v_dpyHexValue;
    gc = TkGate.scopeClearGC;

    XDrawString(TkGate.D,TkGate.ScopeW,gc,x+2,y-ScopeLOW-3,s,strlen(s));
    ob_free(V->v_dpyHexValue);
    V->v_dpyHexValue = 0;
  }

  if (*new_dpy) {
    gc = GValue_getColor(V,T->t_nBits);
    XDrawString(TkGate.D,TkGate.ScopeW,gc,x+2,y-ScopeLOW-3,new_dpy,strlen(new_dpy));
  }
  V->v_dpyHexValue = ob_strdup(new_dpy);
}

/*
  Returns a code indicating the proper vertical lines to be drawn
  on a logic transition.  The codes are:

     0		No vertical line
     1		Bottom half only
     2		Top half only
     3		Full vertical line
 */
unsigned transition_type(int from,int to)
{
  /*
    Rows are the 'from' values and columns are the 'to' values.
   */
  static unsigned char trans[VC_NUMCASES][VC_NUMCASES] = {
      /* 0 	1	x	z	H	L	C */
       {0,	3,	3,	1,	3,	1,	3},	/* 0 */
       {3,	0,	3,	2,	2,	3,	3},	/* 1 */
       {3,	3,	0,	3,	3,	3,	3},	/* x */
       {1,	2,	3,	0,	2,	1,	3},	/* z */
       {3,	2,	3,	2,	0,	3,	3},	/* H */
       {1,	1,	3,	1,	3,	0,	3},	/* L */
       {3,	3,	3,	3,	3,	3,	0},	/* C */
  };

  return trans[from][to];
}

void GTrace_updateTransition(GTrace *T,GValue *V,GScope *S,int y,int x1,int x2,simtime_t LTime,int isFullUpdate)
{
  if (isTransition(S,V,T->t_nBits)) {
    GC gc;

    gc = GValue_getColor(V->v_next,T->t_nBits);

    if (T->t_nBits > 1) {

      GTrace_drawTransValue(T,V,S,y,LTime,isFullUpdate);
      GTrace_drawTransValue(T,V->v_next,S,y,LTime,isFullUpdate);
      XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		x2,y-ScopeLOW,x2,y-ScopeHIGH+1);
    } else {
      unsigned tt;

      if (!V->v_next) return;

      tt = transition_type(V->v_code,V->v_next->v_code);
      switch (tt) {
      case 1 :	/* Low half */
	XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		  x2,y-ScopeMEDIUM,
		  x2,y-ScopeLOW);
	break;
      case 2 :	/* High half */
	XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		  x2,y-ScopeMEDIUM,
		  x2,y-ScopeHIGH);
	break;
      case 3 :	/* Full */
	XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		  x2,y-ScopeLOW,
		  x2,y-ScopeHIGH);
	break;
      }
    }
  }
}

void GTrace_updateValue(GTrace *T,GValue *V,int y,int x1,int x2)
{
  GC gc = GValue_getColor(V,T->t_nBits);

  switch (V->v_code) {
  case VC_UNRECORDED :
    break;
  case VC_UNKNOWN :
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeLOW,
	      x2,y-ScopeLOW);
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeHIGH,
	      x2,y-ScopeHIGH);
    break;
  case VC_FLOAT :
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeMEDIUM,
	      x2,y-ScopeMEDIUM);
    break;
  case VC_LOW :
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeLOW,
	      x2,y-ScopeLOW);
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeMEDIUM,
	      x2,y-ScopeMEDIUM);
    break;
  case VC_HIGH :
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeHIGH,
	      x2,y-ScopeHIGH);
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeMEDIUM,
	      x2,y-ScopeMEDIUM);
    break;
  case VC_CONTENTION :
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeLOW,
	      x2,y-ScopeLOW);
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeHIGH,
	      x2,y-ScopeHIGH);
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeLOWMED,
	      x2,y-ScopeLOWMED);
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeHIGHMED,
	      x2,y-ScopeHIGHMED);
    XDrawLine(TkGate.D,TkGate.ScopeW,gc,
	      x1,y-ScopeMEDIUM,
	      x2,y-ScopeMEDIUM);
    break;
  case VC_ONE :
    if (T->t_nBits == 1) {
      XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		x1,y-ScopeHIGH,
		x2,y-ScopeHIGH);
    } else {
      XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		x1,y-ScopeLOW,
		x2,y-ScopeLOW);
      XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		x1,y-ScopeHIGH,
		x2,y-ScopeHIGH);
    }
    break;
  case VC_ZERO :  
    if (T->t_nBits == 1) {
      XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		x1,y-ScopeLOW,
		x2,y-ScopeLOW);
    } else {
      XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		x1,y-ScopeLOW,
		x2,y-ScopeLOW);
      XDrawLine(TkGate.D,TkGate.ScopeW,gc,
		x1,y-ScopeHIGH,
		x2,y-ScopeHIGH);
    }
    break;
  }
}


void GTrace_update(GTrace *T,GScope *S,int y,simtime_t LTime,int isFullUpdate)
{
  /*    int lasttime,value,vcode,*/
  int x1,x2;
  GValue *V;

  GTrace_adjust(T,LTime);

  for (V = T->t_current;V;V = V->v_next) {
    x1 = GScope_getXPos(S,V,LTime);
    x2 = GScope_getXPos(S,V->v_next,LTime);

    GTrace_updateValue(T,V,y,x1,x2);
    GTrace_updateTransition(T,V,S,y,x1,x2,LTime,isFullUpdate);
    if (V->v_next && V->v_next->v_time > S->s_leftTime + S->s_range) break;
  }
}

int GScope_getTraceIndex(GScope *S,GTrace *t)
{
  int i;

  for (i = 0;i < S->NumTraces;i++) {
    if (S->Traces[i] == t) return i;
  }

  return -1;
}

void GScope_updateTransition(GScope *S,simtime_t OldTime,int isFullUpdate)
{
  int i;
  int Y = ScopeTRACEHEIGHT;
  for (i = S->Start;i < S->NumTraces;i++) {
    int x1,x2;
    GTrace *T = S->Traces[i];

    if (T->t_last->v_time == OldTime) {
      GTrace_adjust(T,OldTime);
      x1 = GScope_getXPos(S,T->t_last,OldTime);
      x2 = GScope_getXPos(S,0,OldTime);
      GTrace_updateTransition(T,T->t_last->v_prev,S,Y,x1,x2,0,isFullUpdate);
    }

    Y += ScopeTRACEHEIGHT;
    if (Y > S->Height-ScopeBOTTOMHEIGHT) break;
  }
}

GTrace *GScope_hitTrace(GScope *S,int y)
{
  int i;
  int trace_y = ScopeTRACEHEIGHT;

  for (i = S->Start;i < S->NumTraces;i++) {
    GTrace *T = S->Traces[i];

    if (y < trace_y)
      return T;
      
    trace_y += ScopeTRACEHEIGHT;
    if (trace_y > S->Height-ScopeBOTTOMHEIGHT) break;
  }

  return 0;
}

void GScope_moveTrace(GScope *S,GTrace *t,int y)
{
  int cpos = GScope_getTraceIndex(S,t);
  int ipos = y/ScopeTRACEHEIGHT;				/* Insert position */
  int imax = (S->Height-ScopeBOTTOMHEIGHT)/ScopeTRACEHEIGHT;	/* Last trace position */
  int i;

  if (ipos < 0) return;
  if (ipos > imax) return;

  /*
   * Get target index for insertion
   */
  ipos += S->Start; 
  if (ipos >= S->NumTraces) ipos = S->NumTraces-1;

  if (ipos == cpos) return;					/* No change */
  

  if (cpos < ipos) {
    for (i = cpos;i < ipos;i++)
      S->Traces[i] = S->Traces[i+1];
  } else {
    for (i = cpos;i > ipos;i--)
      S->Traces[i] = S->Traces[i-1];
  }

  S->Traces[ipos] = t;

  ReqScopeRedisplay();
}

void GScope_deleteSelectedTrace(GScope *S)
{
  int i;
  const char *name; 

  if (!scope_highlightedTrace) return;

  /*
   * Make sure highlighted trace is still a real trace
   */
  for (i = 0;i < S->NumTraces;i++) {
    if (S->Traces[i] == scope_highlightedTrace)
      break;
  }
  if (i == S->NumTraces) return;

  name = scope_highlightedTrace->t_name;
  SimInterface_delProbe(&TkGate.circuit->simulator,name);
  sendSimCmd("$unprobe %s",name);

  scope_highlightedTrace = 0;
  ReqScopeRedisplay();
}


void GScope_clearMark(GScope *S)
{
  S->mark_val[0] = S->mark_val[1] = 0;
}

void GScope_setTraceHighlight(GScope *S,GTrace *t)
{
  scope_highlightedTrace = t;
  if (t)
    GScope_clearMark(S);		/* hide the time selection */
  ReqScopeRedisplay();
}

void GScope_update(GScope *S,simtime_t OldTime)
{
  int i;

  if ((OldTime >= S->s_leftTime + S->s_range) && (S->NumTraces > 0)) {
    return;
  }

  if (S->s_time >= S->s_leftTime + S->s_range) {
    S->s_leftTime = ((S->s_time - S->s_range/3)/S->s_interval)*S->s_interval;
    if (S->s_leftTime < 0) S->s_leftTime = 0;

    ReqScopeRedisplay();
  } else if (S->Start != -1) {
    int Y = ScopeTRACEHEIGHT;
    for (i = S->Start;i < S->NumTraces;i++) {
      GTrace_update(S->Traces[i],S,Y,OldTime,0);
      Y += ScopeTRACEHEIGHT;
      if (Y > S->Height-ScopeBOTTOMHEIGHT) break;
    }
  }
}

void GTrace_draw(GTrace *T,GScope *S,int y,int doName)
{
  int M;
  GC gc;

  if (T == scope_highlightedTrace) {
    XFillRectangle(TkGate.D,TkGate.ScopeW,TkGate.scopeSelectGC,0,y-ScopeHIGH-1,
	       S->Width,ScopeHIGH-ScopeLOW+2);
  } else if (!doName) {
    XFillRectangle(TkGate.D,TkGate.ScopeW,TkGate.scopeClearGC,ScopeLEFTMARGIN,y-ScopeHIGH-1,
	       S->Width-ScopeLEFTMARGIN,ScopeHIGH-ScopeLOW+2);
  }


  if (T->t_current && !T->t_current->v_next)
    GTrace_drawTransValue(T,T->t_current,S,y,S->s_leftTime,1);

  if (doName) {

    XSetLineAttributes(TkGate.D,TkGate.scopeGridGC,2,LineSolid,CapButt,JoinBevel);
    XDrawLine(TkGate.D,TkGate.ScopeW,TkGate.scopeGridGC,ScopeLEFTMARGIN,y,
	    S->Width-ScopeRIGHTMARGIN,y);

    for (M = S->s_leftTime;M <= S->s_leftTime + S->s_range;M += S->s_interval) {
      int P;

      P = ScopeLEFTMARGIN + (M-S->s_leftTime)*S->s_scale;
      XDrawLine(TkGate.D,TkGate.ScopeW,TkGate.scopeGridGC,P,y-2,P,y+2);
    }
    XSetLineAttributes(TkGate.D,TkGate.scopeGridGC,1,LineSolid,CapButt,JoinBevel);

    if (!T->t_visName) {
      int w;
      char *p;

      T->t_visName = T->t_dVisName = ob_strdup(T->t_printName);

      if ((p = strchr(T->t_visName,'.')))
	T->t_visName = p+1;      

      while ((w = GKTextWidth(TkGate.textXF[1],T->t_visName,strlen(T->t_visName))) > ScopeLEFTMARGIN-10) {
	if ((p = strchr(T->t_visName+1,'.')))
	  T->t_visName = p;
	else
	  T->t_visName++;
      }
    }


    XDrawLine(TkGate.D,TkGate.ScopeW,TkGate.scopeGridGC,10,y,
	      ScopeLEFTMARGIN-10,y);


    if (T->t_nBits > 1)
      gc = TkGate.busGC;
    else
      gc = TkGate.wireGC;

    XSetFont(TkGate.D,gc,TkGate.textXF[1]->fid);
#if 0
    PosDrawString(TkGate.ScopeW,NULL,gc,10,y-ScopeTEXTPOS,
		  T->t_visName,AtLeft);
#endif
    XDrawString(TkGate.D,TkGate.ScopeW,gc,10,y-ScopeTEXTPOS,
		T->t_visName,strlen(T->t_visName));
    XSetFont(TkGate.D,gc,TkGate.stextXF[1]->fid);
  }

  GTrace_update(T,S,y,S->s_leftTime,1);
}

void GScope_showCrossLine(GScope *S)
{
  if (Scope->show_xhair) return;
  Scope->show_xhair = 1;

  if (Scope->xhair_x >= ScopeLEFTMARGIN && Scope->xhair_x <= Scope->Width - ScopeRIGHTMARGIN) {
    if (Scope->enable_xhair)
      XDrawLine(TkGate.D,TkGate.ScopeW,TkGate.scopeXHairGC,Scope->xhair_x,0,
		Scope->xhair_x,Scope->Height-ScopeBOTTOMHEIGHT);
  }
}

void GScope_hideCrossLine(GScope *S)
{
  if (!Scope->show_xhair) return;
  if (Scope->NumTraces <= 0) return;

  if (Scope->xhair_x >= ScopeLEFTMARGIN && Scope->xhair_x <= Scope->Width - ScopeRIGHTMARGIN) {
    if (Scope->enable_xhair)
      XDrawLine(TkGate.D,TkGate.ScopeW,TkGate.scopeXHairGC,Scope->xhair_x,0,
		Scope->xhair_x,Scope->Height-ScopeBOTTOMHEIGHT);
  }

  Scope->show_xhair = 0;
}

void GScope_drawCrossLine(GScope *S,int x)
{
  if (Scope->NumTraces <= 0) return;
  if (!Scope->show_xhair) return;

  if (Scope->xhair_x >= ScopeLEFTMARGIN && Scope->xhair_x <= Scope->Width - ScopeRIGHTMARGIN) {
    if (Scope->enable_xhair)
    XDrawLine(TkGate.D,TkGate.ScopeW,TkGate.scopeXHairGC,Scope->xhair_x,0,
	      Scope->xhair_x,Scope->Height-ScopeBOTTOMHEIGHT);
  }

  Scope->xhair_x = x;

  if (Scope->xhair_x >= ScopeLEFTMARGIN && Scope->xhair_x <= Scope->Width - ScopeRIGHTMARGIN) {
    if (Scope->enable_xhair)
      XDrawLine(TkGate.D,TkGate.ScopeW,TkGate.scopeXHairGC,Scope->xhair_x,0,
		Scope->xhair_x,Scope->Height-ScopeBOTTOMHEIGHT);
  }
}


void GScope_drawSelection(GScope *S)
{
  int Y,i;
  int x1,x2;
  int pmin = ScopeLEFTMARGIN;
  int pmax = S->Width-ScopeRIGHTMARGIN;

  x1 = GScope_t2x(S,S->mark_val[0]);
  x2 = GScope_t2x(S,S->mark_val[1]);

  if (x2 < x1) {
    int t = x1; x1 = x2; x2 = t;
  }
#if 0
  printf("Scope_drawSelection[%d..%d]\n",x1,x2);
#endif

  if (x1 < pmin) x1 = pmin;
  if (x1 > pmax) x1 = pmax;
  if (x2 < pmin) x2 = pmin;
  if (x2 > pmax) x2 = pmax;
  if (x1 == x2) return;

  if (S->mark_count == 0) return;

  Y = ScopeTRACEHEIGHT;
  for (i = S->Start;i < S->NumTraces;i++) {
    XFillRectangle(TkGate.D,TkGate.ScopeW,TkGate.scopeSelectGC,
		   x1,Y-ScopeHIGH-1,
		   x2-x1,ScopeHIGH-ScopeLOW+2);

    Y += ScopeTRACEHEIGHT;
    if (Y > S->Height-ScopeBOTTOMHEIGHT) break;
  }
}

void GScope_showSelection(GScope *S)
{
#if 0
  printf("GScope_showSelection() show_mark=%d mark_count=%d\n",S->show_mark,S->mark_count);
#endif

  if (S->show_mark) return;
  S->show_mark = 1;

  GScope_drawSelection(S);
}

void GScope_hideSelection(GScope *S)
{
#if 0
  printf("GScope_hideSelection() show_mark=%d\n",S->show_mark);
#endif

  if (!S->show_mark) return;
  S->show_mark = 0;

  GScope_drawSelection(S);
}

/*
 * Update selected region during a selection drag on the scope.
 */
void GScope_moveSelection(GScope *S)
{
  int Y,i;
  int x1,x2;
  int pmin = ScopeLEFTMARGIN;
  int pmax = S->Width-ScopeRIGHTMARGIN;

  x1 = GScope_t2x(S,S->mark_val[1]);
  if (S->mark_count == 1) {
    S->mark_val[1] = GScope_x2t(S,S->xhair_x);
  }
  x2 = GScope_t2x(S,S->mark_val[1]);

  if (x2 < x1) {
    int t = x1; x1 = x2; x2 = t;
  }

  if (x1 < pmin) x1 = pmin;
  if (x1 > pmax) x1 = pmax;
  if (x2 < pmin) x2 = pmin;
  if (x2 > pmax) x2 = pmax;
  if (x1 == x2) return;

  if (S->mark_count == 0) return;

  Y = ScopeTRACEHEIGHT;
  for (i = S->Start;i < S->NumTraces;i++) {
    XFillRectangle(TkGate.D,TkGate.ScopeW,TkGate.scopeSelectGC,
		   x1,Y-ScopeHIGH-1,
		   x2-x1,ScopeHIGH-ScopeLOW+2);

    Y += ScopeTRACEHEIGHT;
    if (Y > S->Height-ScopeBOTTOMHEIGHT) break;
  }
}

void GScope_setMark(GScope *S,int x,int isDn,unsigned state)
{
  int val;
  int is_shift = ((state & ShiftMask) != 0);
  int pmin = ScopeLEFTMARGIN;
  int pmax = S->Width-ScopeRIGHTMARGIN;
  x = imin(x,pmax);
  x = imax(x,pmin);
  val = GScope_x2t(S,x);

  switch (S->mark_count) {
  case 2 :
    if (!isDn) return;			/* Ignore up clicks when no selection */
    if (is_shift) {
      int d0 = iabs(S->mark_val[0] - val);
      int d1 = iabs(S->mark_val[1] - val);
      if (d0 < d1)
	S->mark_val[0] = val;
      else
	S->mark_val[1] = val;
      break;
    } else
      S->mark_count = 0;
    /* pass through */
  case 0 :
    if (!isDn) return;			/* Ignore up clicks when no selection */

    S->mark_x[0] = x;
    S->mark_x[1] = x;
    S->mark_val[0] = val;
    S->mark_val[1] = val;
    S->mark_count++;
    break;
  case 1 :
    if (!isDn && is_shift) return;	/* Ignore up clicks with shift */

    S->mark_x[1] = x;
    S->mark_val[1] = val;
    S->mark_count++;

    if (S->mark_val[0] == S->mark_val[1])
      S->mark_count = 0;

    break;
  }
}

/*****************************************************************************
 *
 * Set flag indicating if cross-hair is visible and update display on screen
 *
 * Parameters:
 *      n	New cross-hair state (1=visible, 0=invisible)
 *
 *****************************************************************************/
void GScope_setShowXHairState(int n)
{
  extern int scope_active;
  if (!scope_active) return;

  if (Scope->enable_xhair == n) return;

  GScope_hideCrossLine(Scope);
  Scope->enable_xhair = n;
  Scope->xhair_x = 0;
  GScope_showCrossLine(Scope);
}

/*****************************************************************************
 *
 * Draw the base time value for a scope (i.e., the time on the left edge)
 *
 * Parameters:
 *    S		Scope object
 *    y		y-coordinate at which to draw value
 *
 *****************************************************************************/
static void GScope_drawBaseTime(GScope *S,simtime_t base,int y)
{
  SimInterface *si = &TkGate.circuit->simulator;
  double base_f = (double)base*si->si_tsmult / (double)S->s_precision;
  int units = si->si_units;
  char buf[STRMAX];
  int P = S->s_precision;

  while (base_f > 1000 && units > 0) {
    base_f /= 1000.0;
    units--;
    P *= 1000;
  }
  P /= si->si_tsmult;

  switch (P) {
  case 0:
  case 1:
    sprintf(buf,"%0.0f%s",base_f,SimInterface_unitsToStr(units));
    break;
  case 10:
    sprintf(buf,"%0.1f%s",base_f,SimInterface_unitsToStr(units));
    break;
  case 100:
    sprintf(buf,"%0.2f%s",base_f,SimInterface_unitsToStr(units));
    break;
  default :
    sprintf(buf,"%0.3f%s",base_f,SimInterface_unitsToStr(units));
    break;
  }

  XDrawString(TkGate.D,TkGate.ScopeW,TkGate.scopeGridGC,
	      10,y,buf,strlen(buf));
}

/*****************************************************************************
 *
 * Choose appropriate parameters for converting epoch values to printable
 * tick labeled
 *
 *****************************************************************************/
void GScope_pickTimeLabeling(SimInterface *si,simtime_t range,simtime_t interval,
			     char **format,double *tickScale,int *units)
{
  double range_f, interval_f;

  *units = si->si_units;
  *tickScale = (double)si->si_tsmult/(double)si->si_precision;
  range_f = *tickScale * (double)range;
  *units = si->si_units;
  *format = "%lf";

  while (range_f < 1 && *units < NUM_UNITS) {
    *tickScale *= 1000;
    range_f *= 1000;
    (*units)++;
  }
  while (range_f > 1000 && *units > 0) {
    *tickScale /= 1000;
    range_f /= 1000;
    (*units)--;
  }

  interval_f = interval * *tickScale;

  if (interval_f < 1)
    *format = "+%0.1lf";
  else
    *format = "+%0.0lf";
}

/*****************************************************************************
 *
 * Draw the scope window scale
 *
 * Parameters:
 *      S 		Scope object
 *      doTicks		Non-zero if scope ticks should also be updated
 *
 * If doTicks is zero, only the base time will be updated.  If doTicks is
 * non-zero, it is assumed that the screen has been cleared.
 *
 *****************************************************************************/
static void GScope_drawScale(GScope *S,int doTicks)
{
  double W = S->Width - ScopeLEFTMARGIN - ScopeRIGHTMARGIN;
  double UnitSize = W / (double) S->s_range;
  int y = S->Height - ScopeBOTTOMHEIGHT + 20;
  simtime_t base = S->s_leftTime;
  SimInterface *si = &TkGate.circuit->simulator;

  S->s_scale = UnitSize; 

  if (!doTicks) {
    XFillRectangle(TkGate.D,TkGate.ScopeW,TkGate.scopeClearGC,
		   0,S->Height-ScopeBOTTOMHEIGHT,
		   ScopeLEFTMARGIN-10,ScopeBOTTOMHEIGHT);
  }

  /*
   * Draw the base value
   */
  GScope_drawBaseTime(S,base,y);

  

  /*
   * Draw the tick values. 
   */
  if (doTicks) {
    char buf[STRMAX];
    simtime_t T;
    char *format;
    double tickScale;
    int units;
    const char *units_s;
    int text_w;

    GScope_pickTimeLabeling(si, S->s_range, S->s_interval, &format, &tickScale, &units);

    for (T = base;
	 T < base + S->s_range + S->s_interval-1;
	 T += S->s_interval) {
      double tick = (T-base)*tickScale;

      sprintf(buf,format,tick);
	
      text_w = GKTextWidth(TkGate.textXF[1],buf,strlen(buf))/2;
      XDrawString(TkGate.D,TkGate.ScopeW,TkGate.scopeGridGC,
		  ScopeLEFTMARGIN+(int)((T-S->s_leftTime)*UnitSize-text_w),
		  y,buf,strlen(buf));
    }

    
    units_s = SimInterface_unitsToStr(units);
    text_w = GKTextWidth(TkGate.textXF[1],units_s,strlen(units_s));
    XDrawString(TkGate.D,TkGate.ScopeW,TkGate.scopeGridGC,
		S->Width - ScopeRIGHTMARGIN - text_w,
		y+15,units_s,strlen(units_s));

  }
}

/*****************************************************************************
 *
 * Do a complete update of the scope window.
 *
 * Parameters:
 *      S		Scope object
 *
 *****************************************************************************/
void GScope_fullUpdate(GScope *S)
{
  extern int scope_active;
  Window R;
  int i,X,Y,W,H,BW,DP;
  double vs,ve,hs,he;


  if (!scope_active) return;

  if (TkGate.idle_ev.scope_redraw) {
    XClearWindow(TkGate.D,TkGate.ScopeW);
    XGetGeometry(TkGate.D,TkGate.ScopeW,&R,&X,&Y,&W,&H,&BW,&DP);
    S->Width = W;
    S->Height = H;

    S->show_xhair = 0;
    S->show_mark = 0;
  } else {
    W = S->Width;
    H = S->Height;

    GScope_hideCrossLine(S);
    GScope_hideSelection(S);
  }


  if (S->NumTraces > 0) {
    GScope_drawScale(S,TkGate.idle_ev.scope_redraw);

    Y = ScopeTRACEHEIGHT;
    for (i = S->Start;i < S->NumTraces;i++) {
      GTrace_draw(Scope->Traces[i],S,Y,TkGate.idle_ev.scope_redraw);
      Y += ScopeTRACEHEIGHT;
      if (Y > H-ScopeBOTTOMHEIGHT) break;
    }

    vs = (double)S->Start/(double)S->NumTraces;
    ve = (double)(i+1)/(double)S->NumTraces;

    if (S->s_time > 0) {
      hs = (double)S->s_leftTime/(double)S->s_time;
      he = (double)(S->s_leftTime+S->s_range)/(double)S->s_time;
      if (he > 1.0) he = 1.0;
    } else {
      hs = 0.0;
      he = 1.0;
    }
  } else {
    static char *msg;
    char buf[STRMAX];

    msg = msgLookup("scope.emptymsg");
    recodeText(Circuit_getDisplayEncoder(TkGate.circuit), buf, STRMAX, msg);

    vs = 0.0;
    ve = 1.0;
    hs = 0.0;
    he = 1.0;
    XDrawString(TkGate.D,TkGate.ScopeW,TkGate.scopeGridGC,
		20,H/2,buf,strlen(buf));
  }

  GScope_showCrossLine(S);
  GScope_showSelection(S);

  DoTcl(".scope.main.vert set %f %f",vs,ve);
  DoTcl(".scope.main.horz set %f %f",hs,he);
}

void Scope_stepTo(simtime_t t)
{
  int OldTime = Scope->s_time;
  Scope->s_time = t;
  GScope_update(Scope,OldTime);
}

void Scope_setOneBitValue(GTrace *T,simtime_t Time,int c)
{
  switch (c) {
  case '1' :
    trace_observe(T,Time,VC_ONE,0);
    break;
  case '0' :
    trace_observe(T,Time,VC_ZERO,0);
    break;
  case 'x' :
  case 'X' :
    trace_observe(T,Time,VC_UNKNOWN,0);
    break;
  case 'z' :
  case 'Z' :
    trace_observe(T,Time,VC_FLOAT,0);
    break;
  case 'l' :
  case 'L' :
    trace_observe(T,Time,VC_LOW,0);
    break;
  case 'h' :
  case 'H' :
    trace_observe(T,Time,VC_HIGH,0);
    break;
  }
}

void Scope_setValue(const char *name,const char *val)
{
  int nbits;
  char c,*p,*q;
  GTrace *T = GScope_findTrace(Scope,name);

  if (!T) return;		/* Not an observed value */

  if (sscanf(val,"%d'%c",&nbits,&c) != 2) {
    trace_observe(T,Scope->s_time,VC_FLOAT,0);
    return;
  }
  p = strchr(val,'\'') + 2;

  /*
   * Handle single bit values here.
   */
  if (nbits == 1) {
    Scope_setOneBitValue(T,Scope->s_time,*p);
    GScope_updateTransition(Scope,Scope->s_time,0);
    return;
  }

  /*
   * If multi-bit signal and all bits are floating, treat as
   * a single-bit floating signal.
   */
  for (q = p;*q;q++)
    if (*q != 'z') break;
  if (!*q) {
    Scope_setOneBitValue(T,Scope->s_time,'z');
    GScope_updateTransition(Scope,Scope->s_time,0);
    return;
  }

  trace_observe(T,Scope->s_time,VC_ZERO,p);

  if (Scope->s_time <= Scope->s_leftTime + Scope->s_range)
    GScope_updateTransition(Scope,Scope->s_time,0);
}

/*
  Report observations on repName to traceName as well.
*/
void Scope_rename(const char *oldName,const char *newName)
{
  GTrace *T = GScope_findTrace(Scope,oldName);
  if (T) {
    ob_free(T->t_name);
    T->t_name = ob_strdup(newName);
  }
}

/*
  Observe the value of a trace at a particular time.  The value
  may be specified with a vcode (for 1-bit traces), or a value string
  (for n-bit traces).  If the value of the trace changes the change
  will be recorded.
 */
void trace_observe(GTrace *T,simtime_t time,int vcode,const char *value)
{
  if (T->t_last->v_time == time) {
    char *old_str = T->t_last->v_hexValue;
    const char *new_str = value;

    if (new_str)
      while (*new_str == '0' && new_str[1]) new_str++;

    if (new_str) {
      T->t_last->v_hexValue = ob_strdup(new_str);
    } else
      T->t_last->v_hexValue = 0;

    if (old_str) ob_free(old_str);

    T->t_last->v_code = vcode;
  } else if (T->t_last->v_time < time) {
    if (T->t_last->v_code != vcode || 
	(value && !T->t_last->v_hexValue) ||
	(value && T->t_last->v_hexValue && strcmp(value,T->t_last->v_hexValue) != 0)) {
      T->t_last->v_next = new_Value(time,vcode,value,T->t_last);
      T->t_last = T->t_last->v_next;
    }
  } else {
    logError(ERL_WARN,"Time travel not allowed.");
    return;
  }
}

GScope *new_GScope(simtime_t precision)
{
  GScope *S;

  S = (GScope *) ob_malloc(sizeof(GScope),"GScope");
  S->Width = S->Height = 0;
  S->Start = 0;
  S->NumTraces = 0;
  S->s_time = S->s_leftTime = 0;
  S->s_baseRangePos = DEFAULTRANGEPOS;
  S->s_rangePos = S->s_baseRangePos;
  S->s_precision = precision;
  S->s_range = RangeVals[S->s_rangePos];
  GScope_pickInterval(&S->s_range,&S->s_interval,S->s_precision);
  S->s_scale = 0.0;

  S->hash_width = GKTextWidth(TkGate.stextXF[1],"#",1);

  S->enable_xhair = 1;
  S->show_xhair = 1;
  S->show_mark = 0;
  S->xhair_x = 0;
  S->mark_count = 0;

  return S;
}

void delete_GScope(GScope *S)
{
  int i;

  for (i = 0;i < S->NumTraces;i++) {
    GTrace *T = S->Traces[i];
    delete_GTrace(T);
  }
  S->NumTraces = 0;
}

void GScope_deleteTrace(GScope *S,const char *name)
{
  int i;

  for (i = 0;i < S->NumTraces;i++) {
    GTrace *T = S->Traces[i];
    if (strcmp(T->t_printName,name) == 0) {
      delete_GTrace(T);
      break;
    }
  }

  S->NumTraces--;
  for (;i < S->NumTraces;i++)
    S->Traces[i] = S->Traces[i+1];

  if (S->NumTraces == 0)
    DoTcl("Scope::makeHidden");



  if (S->NumTraces == 0)
    S->Start = 0; 
  else if (S->Start >= S->NumTraces)
    S->Start = S->NumTraces-1;

  ReqScopeRedisplay();
}

void GScope_addTrace(GScope *S,const char *Name,const char *PrintName,int Bits,int doSort)
{
  GTrace *T;
  int i,j;

  if (!(T = GScope_findTrace(S,Name))) {
    T = new_GTrace(Name,PrintName,Bits,S->s_time);
  } else {
    if (T->t_printName) ob_free(T->t_printName);
    T->t_printName = (char*) ob_strdup(PrintName);
    return;
  }

  if (doSort) {
    for (i = 0;i < S->NumTraces;i++) {
      GTrace *sT = S->Traces[i];
      if (strcasecmp(T->t_printName,sT->t_printName) <= 0)
	break;
    }
  } else
    i = S->NumTraces;

  for (j = S->NumTraces;j > i;j--)
    S->Traces[j] = S->Traces[j-1];
  S->NumTraces++;
  S->Traces[i] = T;

  if (S->NumTraces == 1)
    DoTcl("Scope::makeVisible");

  ReqScopeRedisplay();
}

GTrace *new_GTrace(const char *name,const char *printName,int nBits,simtime_t curTime)
{
  GTrace *T;

  T = (GTrace*) ob_malloc(sizeof(GTrace),"GTrace");
  T->t_name = (char*) ob_strdup(name);
  T->t_printName = (char*) ob_strdup(printName);
  T->t_visName = 0;
  T->t_dVisName = 0;
  T->t_nBits = nBits;

  T->t_first = T->t_last = T->t_current = new_Value(0,VC_UNRECORDED,0,NULL);
  trace_observe(T,curTime,VC_UNRECORDED,0);

  return T;
}

void delete_GTrace(GTrace *T)
{
  GValue *V;

  ob_free(T->t_name);
  ob_free(T->t_printName);
  if (T->t_dVisName) ob_free(T->t_dVisName);

  for (V = T->t_first;V;V = V->v_next) {
    if (V->v_dpyHexValue) ob_free(V->v_dpyHexValue);
    if (V->v_hexValue) ob_free(V->v_hexValue);
  }

  ob_free(T);
}

GTrace *GScope_findTrace(GScope *S,const char *Name)
{
  int i;

  for (i = 0;i < S->NumTraces;i++) {
    GTrace *T = S->Traces[i];
    if (!strcmp(T->t_name,Name))
      return T;
  }

  return NULL;
}


GValue *new_Value(simtime_t CurTime,int Code,const char *value,GValue *Prev)
{
  GValue *V;

  V = (GValue *) ob_malloc(sizeof(GValue),"GValue");
  V->v_time = CurTime;
  V->v_code = Code;
  if (value) {
    while (*value == '0' && value[1]) value++;
    V->v_hexValue = ob_strdup(value);
  } else
    V->v_hexValue = 0;
  V->v_dpyHexValue = 0;
  V->v_prev = Prev;
  V->v_next = NULL;

  return V;
}

void GScope_pickInterval(simtime_t *R,simtime_t *I,simtime_t precision)
{
  int p,k,d;

  k = *R/10;
  d = 1;
  for (p = 0;k > 10;p++) {
    d *= 10;
    k /= 10;
  }
  *I = k * d;

  *R *= precision;
  *I *= precision;
}

void GScope_saveProbes(GScope *S)
{
  Circuit *c = TkGate.circuit;
  int i;

  if (!S) return;

  Circuit_clearSavedProbes(c);

  for (i = 0;i < S->NumTraces;i++) {
    GTrace *T = S->Traces[i];
    Circuit_addProbeName(c,T->t_name,0);
  }
}

void GScope_removeProbesOnNet(Circuit *c,GNet *net)
{
  /*STUB*/
}

void GScope_postFullName(GTrace *t)
{
  if (t) {
    DoTcl("Scope::showTraceName %s",t->t_name);
  } else
    DoTcl("Scope::hideTraceName");
}

/*
ScopeExposePred(D,E,arg)
Display *D;
XEvent *E;
char *arg;
{
    return E->type == Expose && E->xany.window == TkGate.ScopeW;
}
*/

