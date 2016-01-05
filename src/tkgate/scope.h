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
#ifndef __scope_h
#define __scope_h

/* Value codes */
#define VC_UNRECORDED	-1
#define VC_ZERO		0
#define VC_ONE		1
#define VC_UNKNOWN	2
#define VC_FLOAT	3
#define VC_HIGH		4
#define VC_LOW		5
#define VC_CONTENTION	6
#define VC_NUMCASES	7

#define ScopeTRACEHEIGHT 20
#define ScopeBOTTOMHEIGHT 45
#define ScopeLEFTMARGIN 110
#define ScopeRIGHTMARGIN 20

#define ScopeLOW	(20*ScopeTRACEHEIGHT/100)
#define ScopeHIGH	(80*ScopeTRACEHEIGHT/100)
#define ScopeLOWMED	(40*ScopeTRACEHEIGHT/100)
#define ScopeHIGHMED	(60*ScopeTRACEHEIGHT/100)
#define ScopeMEDIUM	(50*ScopeTRACEHEIGHT/100)
#define ScopeTEXTPOS	(30*ScopeTRACEHEIGHT/100)

#define MAXMARKS 10
#define MINMARKS 5

#define MAXRANGEPOS 18
#define DEFAULTRANGEPOS 5

#define MAXACTIVE 100		/* Maximum number of active traces */
#define MAXMARK 2		/* Maximum number of marks */

typedef struct value GValue;
struct value {
  simtime_t	v_time;		/* Time which value becomes effective */
  short		v_code;		/* value code (see VC_ macros) */
  char 		*v_hexValue;	/* Value string for multi-bit values */
  char 		*v_dpyHexValue;	/* Currently displayed hex value */
  GValue 	*v_next;	/* Next change in value */
  GValue 	*v_prev;	/* Previous change in value */
};

typedef struct trace {
  char 		*t_name;	/* Name of signal */
  char 		*t_printName;	/* Print Name of signal */
  char 		*t_visName;	/* Visible part of PrintName */
  char 		*t_dVisName;	/* Deletable ref to VisName */
  int 		t_nBits;	/* Number of bits of signal */
  GValue 	*t_first;	/* First value entry */
  GValue 	*t_last;	/* Latest change in value */
  GValue 	*t_current;	/* Current value entry visible on left side of scope */
} GTrace;

typedef struct scope {
  Tk_Window	win;			/* The TK Window for the scope */
  Display	*d;			/* The display */
  Tcl_Interp	*tcl;			/* The tcl interpreter */
  GC		gc;			/* The GC for this window */
  int		x,y;			/* Position of window */
  int		Width,Height;		/* Size of window */

  XColor	*bgColor;
  XColor	*fgColor;
  char		*xscroll;
  char		*yscroll;

  int		NumTraces;		/* Total number of traces */
  int		Start;			/* First currently displayed trace */
  GTrace	*Traces[MAXACTIVE];	/* Active traces */

  int		hash_width;		/* Width of the '#' character */

  int		enable_xhair;		/* Enable crosshair */
  int		show_xhair;		/* Show crosshair */
  int		xhair_x;		/* Crosshair x position */
  int		show_mark;		/* Show selection */
  int		mark_count;		/* Number of active marks */
  int		mark_x[MAXMARK];	/* Mark x positions */
  int		mark_val[MAXMARK];	/* Mark values */

  int		s_baseRangePos;		/* Base range position */
  int		s_rangePos;		/* Range position */
  simtime_t	s_precision;		/* Precision */

  simtime_t	s_oldTime;		/* Old time */
  simtime_t	s_time;			/* Current time */
  simtime_t	s_leftTime;		/* Time at left margin */
  simtime_t	s_range;		/* Number of epochs displayed */
  simtime_t	s_interval;		/* Tickmark Interval */
  double	s_scale;		/* Scale for points */
} GScope;

GTrace *new_GTrace(const char *name,const char *printName,int nbits, simtime_t curTime);
void delete_GTrace(GTrace*);
void trace_observe(GTrace *T,simtime_t time,int vcode,const char *hexValue);

GValue *new_Value(simtime_t CurTime,int Code,const char *Value,GValue *Prev);


void Scope_stepTo(simtime_t t);
void Scope_setValue(const char *name,const char *val);
void Scope_rename(const char *oldName,const char *newName);

GScope *new_GScope(simtime_t precision);
void delete_GScope(GScope *);
void GScope_addTrace(GScope*,const char *Name,const char *PrintName,int Bits,int doSort);
GTrace *GScope_findTrace(GScope *S,const char *Name);
void GScope_fullUpdate(GScope*);
void GScope_deleteTrace(GScope*,const char *name);
void GScope_showCrossLine(GScope *S);
void GScope_hideCrossLine(GScope *S);
void GScope_drawCrossLine(GScope *S,int x);

void GScope_setMark(GScope *S,int x,int isDn,unsigned state);	/* Set initial mark point */

void GScope_clearSelection(GScope *S);		/* Clear the trace selection */
void GScope_showSelection(GScope *S);		/* Make trace selection visible */
void GScope_hideSelection(GScope *S);		/* Make trace selection invisible */
void GScope_setShowXHairState(int);
void GScope_moveSelection(GScope *S);		/* Move selection with mouse */
GTrace *GScope_hitTrace(GScope *S,int y);	/* Look for trace at specified y coordinate */
void GScope_moveTrace(GScope *S,GTrace *t,int y); /* Move trace so that y is in trace */
void GScope_setTraceHighlight(GScope *S,GTrace *t);
void GScope_deleteSelectedTrace(GScope *S);	/* Delete the trace that is selected in the scope window */

simtime_t GScope_x2t(GScope*,int x);		/* Convert an x screen position to a time */
int GScope_t2x(GScope*,simtime_t x);		/* Convert a time to an x screen position */

void GScope_pickInterval(simtime_t *R,simtime_t *I,simtime_t precision);

void GScope_saveProbes(GScope *S);
void GScope_postFullName(GTrace *t);

#endif
