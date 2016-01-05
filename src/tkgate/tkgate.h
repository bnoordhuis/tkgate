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
#ifndef __tkgate_h
#define __tkgate_h

#define TKGATE_EDITOR	1	/* This is the editor */
#define TOUCH_XGATE_ED	0	/* Consider TkGate.ed to be undoable */

#include "config.h"

#include <stdlib.h>
#include <sys/param.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <X11/Xlib.h> 
#include <X11/Xresource.h>
#include <X11/cursorfont.h>
#if HAVE_ICONV_H
#include <iconv.h>
#endif

#include "tcl.h"
#include "tk.h"
#include "fonts.h"
#include "zoom.h"
#include "tkgate_config.h"
#include "tkgate_misc.h"
#include "hash.h"
#include "list.h"
#include "misc.h"
#include "text.h"
#include "ycmalloc.h"
#include "html.h"
#include "expr.h"
#include "delay.h"
#include "icon.h"
#include "module.h"
#include "net.h"
#include "wires.h"
#include "elements.h"
#include "modsym.h"
#include "simulate.h"
#include "breakpoint.h"
#include "script.h"
#include "error.h"
#include "vparser.h"
#include "scope.h"
#include "gates.h"
#include "generic.h"
#include "functions.h"
#include "print.h"
#include "message.h"
#include "object.h"
#include "cpath.h"
#include "igenerate.h"
#include "verilog.h"
#include "editstate.h"
#include "circuit.h"
#include "primitives.h"

#if TCL_MAJOR_VERSION != 8
#error This program has not been tested with versions of tcl/tk other than 8.*
#endif

/*
 * Versions of tcl/tk from 8.4 change some uses of 'char*' to 'const char*'.  Tkgate
 * now assumes 'const char*' is always used so if we are using an earlier version of
 * tcl/tk we need to cast them back to 'char*' to prevent errors/warnings.
 */
#if TCL_MINOR_VERSION <= 3
typedef int New_Tcl_CmdProc(ClientData d,Tcl_Interp *tcl,int argc,const char *argv[]);
#define Tcl_CreateCommand(t,c,f,d,x) Tcl_CreateCommand(t,c,(Tcl_CmdProc*)(f),d,x)
#define Tk_ConfigureWidget(t, w, cs, argc, argv, gw, flags) Tk_ConfigureWidget(t, w, cs, argc, (char**)(argv), gw, flags)
#define Tk_CreateWindowFromPath(tcl, root, name, top) Tk_CreateWindowFromPath(tcl, root, (char*)(name), (char*)(top))
#define Tcl_SetVar(tcl, var , val, flags) Tcl_SetVar(tcl, (char*)(var) , (char*)(val), flags)
#define Tcl_Merge(argc, argv) Tcl_Merge(argc, (char**) (argv))
#define Tcl_SplitList(tcl, list, argcP, argvP) Tcl_SplitList(tcl, list, argcP, (char***) (argvP))
#else
typedef Tcl_CmdProc New_Tcl_CmdProc;
#endif

/*
 * Coordinate conversion macros.  The 'ctow' macros convert from circuit
 * to window coordinates while the 'wtoc' mactos convert from window
 * to circuit coordinates.
 */
#define ctow_x(x) ((x) + TkGate.circuit->org_x)
#define ctow_y(y) ((y) + TkGate.circuit->org_y)
#define wtoc_x(x) ((x) - TkGate.circuit->org_x)
#define wtoc_y(y) ((y) - TkGate.circuit->org_y)

/*
 * Modified flags to indicate what is changeing
 */
#define MF_MODULE	0x1		/* Changes that affect the module list or heirarchy */
#define MF_NET		0x2		/* Changes to the nets */
#define MF_INTERFACE	0x4		/* Changes to a module interface */
#define MF_GATE		0x8		/* Changes to gates or gate properties */
#define MF_NOMODULE	0xfffe		/* Set all modified flags except module change */ 
#define MF_ALL		0xffff		/* Set all modified flags */
#define MF_SYNCONLY	0x10000		/* Synchronize interface only */
#define MF_BEGINSPEC	0x20000		/* Begin modifications in a special module */
#define MF_ENDSPEC	0x40000		/* End modifications in a special module */
#define MF_FORCE	0x80000		/* Force non-special even if between BEGINSPEC/ENDSPEC  */

#define CE_ISO8859_1	"iso8859-1"
#define CE_ISO8859_2	"iso8859-2"
#define CE_UTF8		"utf-8"
#define CE_EUC_JP	"euc-jp"

/*
 * Search mode flags
 */
#define SF_NETS		0x1		/* Search net names */
#define SF_GATES	0x2		/* Search gate names */ 
#define SF_TEXT		0x4		/* Search comment text */
#define SF_IGNORECASE	0x100		/* Ignore case in comparison */

/*
 * Search qualifiers
 */
#define SQ_CONTAINS	0		/* Search for things containing target */
#define SQ_BEGINS	1		/* Search for things beginning with target */
#define SQ_ENDS		2		/* Search for things ending with target */
#define SQ_MATCHES	3		/* Search for things that exactly match */

/*
 * Interface editing sub-modes
 */
#define INTFMODE_NONE		0	/* We are not in interface mode */
#define INTFMODE_ALL		1	/* We are on the module list interface mode */
#define INTFMODE_SINGLE		2	/* We are in the single module interface mode */



/*
 * Describes an encoder object used to translate strings from one code to another.
 */
struct encoder_str {
  const char	*fromCode;
  const char	*toCode;
  int		isJapanese;
#if HAVE_ICONV_H
  iconv_t	ico;
#endif
};

/*
 * Describes a locale and all its encoding methods
 */
struct locale_str {
  const char	*l_code;		/* Locale code (e.g., en, ja, de, etc.) */
  const char	*l_name;		/* Name of the locale (e.g., English, Japanese, German) */
  const char	*l_messages;		/* Path of messages file */

  const char	*l_encFont;		/* Font family to use (e.g., iso8859-1) */
  const char	*l_encDisplay;		/* Encoding for display */
  const char	*l_encMessages;		/* Encoding of messages file */
  const char	*l_encVerilog;		/* Encoding of verilog save files */
  const char	*l_encPostscript;	/* Encoding for postscript files */
};


/*
 *  TkGate major modes
 */
typedef enum { MM_EDIT, 		/* Circuit editing mode */
	       MM_SIMULATE, 		/* Circuit simulation mode */
	       MM_ANALYZE		/* Critical path analysis mode */
}  MajorMode;

/*
 * Data structure used for building the "Make" menu.
 */
typedef struct  {
  char		*mname;		/* Actual name of the menu */
  int		count;		/* Number of entries on page */
  GKeyMenuEnt	**kme;		/* List of entries on page */
} BuildMenuRoot;

/*
 * Top-level data structure for "Make" menus
 */
typedef struct {
  SHash		*rmap;
  BuildMenuRoot	*roots[MAKEENTRYMAX];
  int numRoot;
}  MakeMenuData;

/*****************************************************************************
 *
 * This data structure is used to represent the main editing canvas.
 *
 *****************************************************************************/
struct TkgGatWin_str {
  Tk_Window	win;		/* Main TK window */
  Display	*d;		/* Pointer to X11 display structure */
  Tcl_Interp	*tcl;		/* The Tcl interpteter */
  int		width, height;	/* Size of main window */
  XColor	*bgColor;	/* Background border color of main window */
  XColor	*fgColor;	/* Foreground border color of main window */
  GC		gc;		/* Graphics context */
  TkGateParams	*parms;		/* TkGate main object */
  char		*xscroll;	/* Command for x scrolling */
  char		*yscroll;	/* Command for y scrolling */
  char		*takefocus;	/* Should we take focus */
};

struct EditData_str {
  MajorMode major_mode;		/* The current major mode */
  int saved_mode;		/* Saved minor edito mode for button 2 quick moving */

  int mark_vis;			/* Is the mark visible? */
  int mark_posted;		/* The mark has been posted (should be visible) */
  int mark_x, mark_y;		/* Position of mark */

  int rx,ry;			/* Point of current mouse activity (raw coordinates) */

  int tx,ty;			/* Point of current mouse activity (circuit coordinates) */
  int lx,ly;			/* Point of last mouse activity (circuit coordinates) */
  int sx,sy;			/* Point of saved mouse activity (circuit coordinates) */

  int scr_x,scr_y;		/* Scrolling origin point */


  /*
   * Used for hand scroll
   */
  struct {
    int orgSave_x,orgSave_y;	/* Saved origin point */
    int setSave_x,setSave_y;	/* Saved button press point */
  } handScroll;

  int min_x,max_x;		/* Min and max x-coordinates of current module */ 
  int min_y,max_y;		/* Min and max y-coordinates of current module */ 
};

/*****************************************************************************
 *
 * State information for idle events.  Idle events are events that are executed
 * when the interface is "idle".  This allows the redraw rate to be throttled
 * based on available processing power as opposed to redraws after each mouse
 * movement.
 *
 *****************************************************************************/
typedef struct {			
  int pending;			/*   idle event pending */
  int redraw;			/*   redraw requested */
  int scroll_area;		/*   scroll change in viewable area requested */
  int scroll_new_x;		/*   x-coordinate for new origin */
  int scroll_new_y;		/*   y-coordinate for new origin */
  int scope_redraw;		/*   redraw scope window */
  int trace_redraw;		/*   redraw a trace in scope window */
} IdleEv;

/*****************************************************************************
 *
 * State information for popup menus.
 *
 *****************************************************************************/
typedef struct {			
  int		isSet;		/*    Non-zero if we are doing a popup */
  GCElement	*g;		/*    selected gate */
  GWire		*w;		/*    selected wire */
  GWireNode	*n;		/*    selected wire node */
  GNet		*net;		/*    Selected net */
  int		x,y;		/*    position of pointer */
  int		wx,wy;		/*    untranslated position of pointer */
} PopState;

/*****************************************************************************
 *
 * This data structure represents the global state of the tkgate application.
 * There is only one object of this type defined as the global variable "TkGate".
 *
 *****************************************************************************/
struct TkGateParams_str {
  Tcl_Interp	*tcl;		/* The TCL intepreter */
  Display	*D;		/* X Display */
  Screen	*S;		/* Default screen of display */
  Window	W;		/* Editing window */
  Window	ScopeW;		/* Scope Window */
  Window	root;		/* Root window */
  Colormap	CM;		/* The color map */
  XrmDatabase	rdb;		/* The resource database */
  int		bitorder;	/* Order of bits in image data */
  int           byteorder;      /* Order of bytes in image data */

  TkgGateWin	*gw;		/* Tcl/Tk view of main window. */

  char *homedir;		/* Directory for all tkgate files */

  XFontStruct	**textXF;	/* Normal text */
  XFontStruct	**textbXF;	/* Bold text */
  XFontStruct	**stextXF;	/* Small text */
  XFontStruct	**stextbXF;	/* Small bold text */
  XFontStruct	**ktextXF;	/* Kanji font */ 

  GC instGC;			/* GC for built-in instances (variable font) */
  GC moduleGC;			/* GC for modules (variable font)  */
  GC modportGC;			/* GC for module ports  (variable font) */
  GC frameGC;			/* Dashed line GC for frames  (variable font) */
  GC commentGC;			/* GC for comments (variable font) */
  GC imageGC;			/* GC for images */
  GC hyperlinkGC;		/* GC for hyperlink comments (variable font) */
  GC wireGC;			/* GC for wire drawing */
  GC busGC;			/* GC for multi-bit wire drawing */
  GC selWireGC;			/* GC for selected wire drawing */
  GC selBusGC;			/* GC for selected multi-bit wire drawing */
  GC toolGC;			/* GC for TkGate tools, etc. */
  GC cpathGC;			/* GC for critical path */
  GC kanjiGC;			/* GC for kanji (comments only) */ 

  GC copyGC;			/* Basic GC for copying areas, etc. */

  GC scopeGridGC;		/* Scope GC for grid lines, etc. */
  GC scopeOneGC;		/* Scope GC for non-zero signal values */
  GC scopeZeroGC;		/* Scope GC for 0 signal values */
  GC scopeFloatGC;		/* Scope GC for z,l,h signal values */
  GC scopeUnknownGC;		/* Scope GC for x signal values */
  GC scopeClearGC;		/* Scope GC for clearing signal values */
  GC scopeXHairGC;		/* Scope GC for crosshair. */
  GC scopeSelectGC;		/* Scope GC for scope selection. */

  GC bitGC;			/* special 1-bit GC */

  int inst_pixel;		/* Pixel value for instances */
  int ledoff_pixel;		/* Pixel value for off leds */
  int ledon_pixel;		/* Pixel value for on leds */
  int ledunknown_pixel;		/* Pixel value for led with unknown input */
  int comment_pixel;		/* Pixel value for comments */
  int hyperlink_pixel;		/* Pixel value for hyperlinks */

  IdleEv idle_ev;		/* Idle event flags */
  PopState popstate;		/* Popup menu state */

  SHash *libraries;		/* Libraries loaded into tkgate */

  int cpath_flashState;		/* State for flashing cpath */
  int cpath_len;		/* Length of current critical path */
  char **cpath_pelems;		/* The current critcal path */

  int width,height;		/* Size of edit window */
  unsigned state;		/* Mouse button state mask */
  unsigned button;		/* Mouse button that was pressed */

  EditData	*ed;		/* Basic editor data */
  Circuit	*circuit;	/* Top-level circuit data */
  ErrorList	*errl;		/* Error list manager data */

  int ErrorMarkTimeout;		/* Status flag for drawing the error "X" marker. */

  unsigned	sync_flags;	/* Flags indicating what tcl elements need synchronizing  */

  /*
   * Encoding and font handling
   */
  Locale	*locale;		/* Locale that we are running under */
  SHash		*localeTable;		/* Locales that are available */
  SHash		*localeNameTable;	/* Locales by full name */
  int		japaneseMode;		/* Are we in Japanese mode? */

  char *errorLogFile;
  char *siteName;
  char *defaultTech;
  char *browserCommand;
  char *emailCommand;
  int regionUpdate;
  int smoothScroll;
  int changedp;
  int startrekp;
  int baderp;
  int batp;
  int batc;
  int flashCPath;
  int saveTraces;
  int sortTraces;
  int extendBars;
  int showSwitchNets;
};

typedef struct{
  char	 		*command;
  New_Tcl_CmdProc	*func;
} Tkg_Command;

int Tkg_GetColor(const char *name);



void configureMainWindow(Tcl_Interp *);
void message(int,const char*,...);

int checkValidName(const char *name,SHash *H);
void pickValidName(char *buf,const char *name,const char *base,SHash *H);

void logError(int code,const char *fname,int lnum,const char *s,...);

void GCellSpec_writeBeginModule(FILE *f,GCellSpec *gcs);
void GCellSpec_writeEndModule(FILE *f,GCellSpec *gcs);


unsigned transition_type(int from,int to);

Encoder *getEncoder(const char *toCode,const char *fromCode);
size_t recodeText(Encoder *encoder, char *toString,int len, const char *fromString);
char *recodeTextP(Encoder *encoder, const char *fromString);
int isJapaneseDisplay(Encoder*);

void icon_init();
void SetUpCursors();
void MouseoverCursor(int cursorType);
void MakeHashTables();


int DoTcl(const char*,...);
int DoTclL(const char*,...);
int DoTclV(const char*,int,const char**);
void InitTclProcs(Tcl_Interp *tcl);


void mark_draw();
void mark_flush();
void mark_hide();
void mark_redraw();
void mark_post();
void mark_unpost();

void net_editProps(GNet *n,int x,int y);

/*
 * Convert value to a string.
 */
#define STR(X) #X

/*****************************************************************************
 * Hold-overs from ancient wm window manager code.
 *****************************************************************************/
void wm_GetDimensions(int *X,int *Y);
void wm_SetCursor(int id);

/*
 * Manage changes within tkgate
 */
void SetModified(unsigned flags);		/* Flags are from the MF_ group */
void ClearModified();
void UpdateModifiedIndicator();
void FlagRedraw();
void SynchronizeInterface();


void SetBatCursor();
void ClearErrorMark();
void DrawErrorPositionMark();
void ReqScopeRedisplay();

void TkGate_clearSelection();
void TkGate_setOrigin(int x,int y);

int required_bits(int n);
void badermessage(const char *m);
int posongate(GWire *w,GCElement *g,int *p,int *n);
int findwirepos(GWire *w,GWire *l);
int GetPadPosition(GCElement *g,const char *name);
void panicSave(int);
FILE *openInPath(const char*);
int badaddr(void *S);

void Tkg_changeColor(GC gc,int func,int pixel);

int fontheight(XFontStruct *F);

void line(int x1,int y1,int x2,int y2);
void box(int x,int y,int w,int h);
void mk_gate(int x,int y,GGateInfo *gi,int rot,int selected);

char *filterParen(char *buf,const char *s);

int dce_DrawString(GC gc,int x,int y,int p,const char *s);
int RelPosDrawString(Window W,XFontStruct *F,GC gc,int x,int y,const char *S,int p);
int PosDrawString(Window W,XFontStruct *F,GC gc,int x,int y,const char *S,int p);
void GKDrawString(Display *D,Window W,GC gc,int x,int y,const char *s,int l);
int GKTextWidth(XFontStruct *F,const char *S,int l);
void DrawTextCursor(Window W,int x,int y);

void cpath_show(int n,const char *netNames[]);
void cpath_initNetDelayTable();
void cpath_flushNetDelayTable();
void cpath_registerNetDelay(const char *,int,int);
void cpath_registerNetAlias(const char *,const char*);
int  cpath_getNetDelay(const char *,int*,int*);
void cpath_showNetDelay(GNet *n);

void scrollbar_update();
void scrollbar_bbx_update();

char *msgLookup(const char*);

void dohyperlink(const char*);

void makeMakeMenu(const char *m);

MajorMode tkgate_currentMode();
void tkgate_setMajorMode(MajorMode mm);

void init_gates();

int verify_circuit();

void setEditCursor(int mode);

void TkGate_init(Tcl_Interp*);
void TkGate_initDelay();
void SetErrorPosition(int,int);

void gateCleanUp();

void initJapanese();
void setGCcolors();
void makeMakeShortcuts();

double traceLinesPerPage(const char *orient,const char *paper);
int tracePageCountEst(const char *orient,const char *paper,int scale,int start,int end);

void scopeButtonPress(int x,int y,int px,int py,int n,int state);
void scopeButtonMotion(int x,int y,int px,int py,int n,int state);
void scopeButtonRelease(int x,int y,int px,int py,int n,int state);

void hdl_load(GModuleDef *M);
int hdl_checkSave(const char *name);
int hdl_save(const char *name);
int hdl_close(void);
void hdl_saveCursor(const char*,int,int);
int hdl_getCursor(int*,int*);
int hdl_replaceName(GModuleDef *M,const char *new_name);

void undoSignals();

void Concat_updateAutos(GModuleDef *M,int doDraw);

void getLocaleSet();
void localization_Setup(Tcl_Interp *tcl);

extern TkGateParams TkGate;

extern int KANJIFONT_WIDTH;

extern Icon *INVERTER;
extern Icon *Mark;
extern Icon *SIZEHASH;
extern Icon *REPCOUNTMARK;

extern int ycLineNumber;
extern const char *ycFileName;

extern SHash *message_table;

extern char tkgate_homedir[STRMAX];
extern char *tkgate_exampledir;
extern char *tkgate_tutorialdir;
extern MakeMenuData *makeMenuData;

extern VersionDef VERSIONS[];
extern int numVersions;

extern int debugContinuousVerify;
extern int debugSimInterface;

extern NHash *GateIdxHash;

extern int debugmode;

extern int hdl_isactive;

#endif
