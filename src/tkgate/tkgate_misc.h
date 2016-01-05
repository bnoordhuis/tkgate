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
****************************************************************************/
/*
    Declarations for circuit representation structures.
*/
#ifndef __tkgate_misc_h
#define __tkgate_misc_h

typedef struct GCellSpec_str GCellSpec;
typedef struct EditState_str EditState;
typedef struct celemnt GCElement;
typedef struct net GNet;
typedef struct wire GWire;
typedef struct wirelist GWireList;
typedef struct wirenode GWireNode;
typedef struct gateinfo GGateInfo;
typedef struct simmodule GSimModule;
typedef struct modlayout GModLayout;
typedef struct GModSymbol_str GModSymbol;
typedef struct GError_str GError;
typedef struct GEScript_str GEScript;
typedef struct ErrorList_str ErrorList;
typedef struct TkgGatWin_str TkgGateWin;
typedef struct TkGateParams_str TkGateParams;
typedef struct EditData_str EditData;
typedef struct CurrentFile_str CurrentFile;
typedef struct Circuit_str Circuit;
typedef struct GPrint_str GPrint;
typedef struct SBreakPoint_str SBreakPoint;
typedef struct Script_str Script;
typedef struct GrabbedLabel_str GrabbedLabel;
typedef struct encoder_str Encoder;
typedef struct locale_str Locale;

/*
 * Set of special Tcl characters
 */
#define TCL_SPECIALCHARS "\"\\[]$"


/*
 * Zoom modes
 */
#define ZOOM_1		1		/* Normal zoom level */
#define ZOOM_MAX	4		/* Maximum magnification */


/*
   Gate editing modes
*/
#define MODE_CUT 	0		/* Cut wires */
#define MODE_MOVE	1		/* Move gates, wires, connect wires */ 
#define MODE_INVERT	2		/* Add/remove inverter on gate */
#define MODE_SCROLL	3		/* x/y scrolling with hand cursor */
#define MODE_NAME	5		/* Set the name of a gate or wire */
#define MODE_ADDINPUT	6		/* Add an input to a block */
#define MODE_ADDOUTPUT	7		/* Add an output to a block */
#define MODE_REPLICATE	8		/* Replicate a gate */
#define MODE_SETSIZE	9		/* Set the bit width of a wire */
#define MODE_ADDTRI	10		/* Add an inout to a block */
#define MODE_CHANGEDIR	11		/* Change direction of pin on a block */
#define MODE_DEBUG	12		/* Print debug info on gate or wire */
#define MODE_MAKESEL	13		/* Create a multi-gate selection */
#define MODE_MOVESEL	14		/* Operate on a multi-gate selection */
#define MODE_MOVENULL	15		/* A null mode to ignore events until a mouse release */

#define MODE_DELETE	-3		/* obsolete "delete gate" mode */

#define NOHIT		100000		/* Not a hit */

/* Wire directions */
#define ANY 0
#define TRI 1
#define IN 2
#define OUT 3

/* Directions */
#define ROT0 0
#define ROT90 1
#define ROT180 2
#define ROT270 3

#define DASH_LENGTH	6		/* Length of dash cycle (should be even) */


/*
  Replication options
 */
#define REP_NOWIRES	0x1		/* Do not replicate wires */

/*
 * Text justification options.
 */
#define BetweenLeftAndRight 0x1
#define AtRight 0x2
#define AtLeft 0x4
#define BetweenTopAndBottom 0x8
#define AtBaseline 0x10
#define AtTop 0x20
#define AtBottom 0x40
#define RJ AtBaseline|AtRight
#define CT AtBaseline|BetweenLeftAndRight
#define LJ AtBaseline|AtLeft

/* Languages for messages */
#define ENGLISH 0
#define JAPANESE 1

/* Cursors */
#define TEXTCURSOR 0
#define OPENCUTTERS 1
#define CLOSEDCUTTERS 2
#define IRON 3
#define INVERTUPCURSOR 4
#define DELETEUPCURSOR 5
#define DELETEDNCURSOR 6
#define PAPERMOVECURSOR 7
#define PENCIL 9
#define DOWNARROW 10
#define UPARROW 11
#define REPCURSOR 12
#define SIZECURSOR 13
#define ARROWCURSOR 14
#define BATCURSOR 15
#define BATCURSOR1 16
#define BATCURSOR2 17
#define BATCURSOR3 18
#define ARROW0 19
#define ARROW90 20
#define ARROW180 21
#define ARROW270 22
#define MENUCURSOR 23
#define TRIARROW 24
#define CHANGEDIRCURSOR 25
#define WRENCHCURSOR 26
#define TREKDELETEUP 27
#define TREKDELETEDN 28
#define INVERTDNCURSOR 29
#define ADDPORTTOP 30
#define ADDPORTBOTTOM 31
#define ADDPORTLEFT 32
#define ADDPORTRIGHT 33
#define SCROLLCURSOR 34
#define MAXCURSORS 35

/*
 * Special cursors
 */
#define CANCELMOUSEOVER		-1	/* Cancel any special mouseover cursors */
#define HYPERLINKCURSOR 	-2	/* Use the hyperlink mouseover cursor */ 

#define SCROLL_LIMIT 100

#define TKG_GATEWIN_HEIGHT	500
#define TKG_GATEWIN_WIDTH	500

/*
  Codes for logErrorMessage
 */
#define ERL_ERROR		0,__FILE__,__LINE__
#define ERL_WARN		1,__FILE__,__LINE__
#define ERL_FATAL		2,__FILE__,__LINE__

/*
   Macros defining variable names from tcl scripts.
*/
#define FONT_PATH		"font_path"
#define TEXT_FONT		"body_font"
#define BOLDTEXT_FONT		"boldBody_font"
#define SMALLTEXT_FONT		"smallBody_font"
#define SMALLBOLDTEXT_FONT	"boldSmallBody_font"
#define SHAPES_FONT		"cursor_font"

/* Orientations */
#define NORMAL 0
#define UP 0
#define RIGHT  1
#define DOWN 2
#define LEFT 3

/* Coder types */
#define ENCODER 0
#define DECODER 1
#define PRIENCODER 2

/* Gate draw types */
#define GD_NORMAL	0	/* Normal drawing (gate with wires) */
#define GD_NOINWIRE	1	/* Suppress input wire drawing */
#define GD_NOWIRE	2	/* Suppress wire drawing */
#define GD_PADONLY	3	/* Draw only wire pad names (block type only) */ 

#define DEFAULT_PORT_NAME "*port_name*"
 
#define bits(var,size) unsigned int var : size
#define abs(x) ((x) < 0 ? (-(x)) : (x))
#define midpointp(m,v1,v2) (abs((m)-(v1)) + abs((m)-(v2)) == abs((v1)-(v2)))

/* The distance formula */
#define distance(x1,y1,x2,y2) ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))
#define wdist(w,x,y)	distance((w)->nodes->x,(w)->nodes->y,x,y)

#define sqr(x) ((x)*(x))

#define nmask(n) ((n) < 32 ? ((1 << (n)) -1) : -1)


/* Test a node to see if it's mobile */
#define anchoredp(n) ((n)->end && (n)->end->gate)
#define ujanchored(n) (n->end ? (n->end->gate ? n->end->gate->gclass != JOINT : TRUE): FALSE)
#define boatanchored(n) (n->boatanchored)
/* Type cast connection description */
#define connect(g,t) ((struct t *) g->connections)	/* Obsolete, use c_<>() */
#define dce_SetClipRectangle(x,y,w,h) wm_SetClipRectangle((x) + TkGate.org_x,(y) + TkGate.org_y,w,h)

#define vertical(n,m) (((n)->x == (m)->x) && ((n)->y != (m)->y))
#define horizontal(n,m) (((n)->x != (m)->x) && ((n)->y == (m)->y))
#define zerolength(n,m) (((n)->x == (m)->x) && ((n)->y == (m)->y))

/*
 * Print message if debugging is active.
 */
#define debugprint(m) {if (debugmode) printf(m);}

/*
 * Target device for formatting
 */
typedef enum  {
  TD_X11,
  TD_PRINT
} TargetDev_e;

#endif
