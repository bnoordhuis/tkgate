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

    Last edit by hansen on Tue Feb  3 10:24:32 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include "tkgate.h"

extern int sync_Xserver;
extern int did_interface_resize;


/*
 * This is a kludge to prevent a problem that can occur when a hyperlink leads
 * to a file that has a hyperlink in the same location such as the <PREVIOUS
 * and NEXT> links in the tutorial.  If the user starts click NEXT> too quickly,
 * it can count as a double click after loading the file.  To prevent this, we
 * clear the flag when reading a new circuit, then increment it each time we click
 * on the canvas.  The flag must be at least 2 to recognize a double click
 */
int click_count = 0;

void doScroll(int dx,int dy);

int gat_scope(ClientData d,Tcl_Interp *tcl,int argc,const char *argv[]);

/*****************************************************************************
 *
 * Top global variable with tkgate state.
 *
 *****************************************************************************/
TkGateParams TkGate;


static int did_doubleclick = 0;	/* Did we just do a double click */


static Tk_ConfigSpec configSpecs[] = {
  {TK_CONFIG_COLOR, "-background", "background", "Background",     		"white", Tk_Offset(TkgGateWin,bgColor), 0, 0},
  {TK_CONFIG_SYNONYM,"-bg", "background", 0, 0, 0, 0, 0},
  {TK_CONFIG_SYNONYM,"-fg", "foreground", 0, 0, 0, 0, 0},
  {TK_CONFIG_COLOR, "-foreground", "foreground", "Foreground",     		"black", Tk_Offset(TkgGateWin,fgColor), 0, 0},
  {TK_CONFIG_INT,    "-height", "height", "Height",		     		STR(TKG_GATEWIN_HEIGHT), Tk_Offset(TkgGateWin,height), 0, 0},
  {TK_CONFIG_INT,    "-width", "width", "Width",		     		STR(TKG_GATEWIN_WIDTH), Tk_Offset(TkgGateWin,width), 0, 0},
  {TK_CONFIG_STRING, "-xscrollcommand", "xscrollcommand",  "Xscrollcommand",    "", Tk_Offset(TkgGateWin,xscroll), 0, 0},
  {TK_CONFIG_STRING, "-yscrollcommand", "yscrollcommand",  "Yscrollcommand",	"", Tk_Offset(TkgGateWin,yscroll), 0, 0},
  {TK_CONFIG_STRING, "-takefocus", "takefocus",  "Takefocus", 			"", Tk_Offset(TkgGateWin,takefocus), 0, 0},
  {TK_CONFIG_END, 0, 0, 0, 0, 0, 0, 0}
};
int numConfigSpecs = sizeof(configSpecs)/sizeof(configSpecs[0]);

/*****************************************************************************
 *
 * Return the code for the current major mode
 *
 *****************************************************************************/
MajorMode tkgate_currentMode()
{
  return TkGate.ed->major_mode;
}

/*****************************************************************************
 *
 * Changes the major mode opening/closing any windows or connections necessary
 * to do so.
 *
 *****************************************************************************/
void tkgate_setMajorMode(MajorMode requestedMode)
{
  MajorMode currentMode;

  if (!TkGate.circuit || !TkGate.circuit->es) return;

  currentMode = tkgate_currentMode();

  /* 
   * No change in mode, but we might want to issue a message.
   */
  if (requestedMode == currentMode)

    /* Gen. error message if simulator is already running */
    if (currentMode == MM_SIMULATE) {
      message(0,msgLookup("err.sim.isrun"));
    return;
  }

  if (requestedMode == MM_SIMULATE || requestedMode == MM_ANALYZE) {
    GModuleDef *R;

    ob_begin_framef("-RCheck",FF_TRANSPARENT);
    R = GModuleDef_isRecursive(TkGate.circuit->es->env);
    ob_end_frame();

    if (R) {
      message(1,msgLookup("sim.recursive"),R->m_name);
      FlagRedraw(MF_ALL);
      return;
    }
  }

  /*
   * Transition to MM_EDIT before changing to new mode. 
   */
  switch (currentMode) {
  case MM_EDIT :
    break;
  case MM_SIMULATE :
    if (TkGate.circuit->simulator.active) {
      SimInterface_end(&TkGate.circuit->simulator);
      FlagRedraw();
    }
    break;
  case MM_ANALYZE :
    cpath_close();
    break;
  }

  /* Always reset edit mode when changing major modes */
  setEditMode(TkGate.circuit->es,MODE_MOVE);
#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->major_mode = requestedMode;

  switch (requestedMode) {
  case MM_EDIT :
    DoTcl("tkg_editLogo");
    ob_mode(OM_ENABLED);
    break;
  case MM_SIMULATE :
    ob_mode(OM_DISABLED);
    ob_clear();
    TkGate.circuit->simulator.no_scope = 0;
    SimInterface_begin(&TkGate.circuit->simulator);
    DoTcl("tkg_resetLogo");
    break;
  case MM_ANALYZE :
    ob_mode(OM_DISABLED);
    ob_clear();
    cpath_open();
    DoTcl("tkg_analysisLogo");
    break;
  }
  SetModified(MF_MODULE|MF_SYNCONLY);
}

/*****************************************************************************
 *
 * Show the name of the selected object on the status line.
 *
 *****************************************************************************/
void showSelectedName()
{
  if (TkGate.circuit->es && TkGate.circuit->es->isInterface) {
    if (TkGate.circuit->nsel) {
      GWire *w = TkGate.circuit->wsel;
      GCElement *g;

      if (w->name) 
	g = w->gate;
      else {
	w = wire_other(w);
	g = w->gate;
      }

      if (w && g && w->name && GCElement_isModule(g))
	message(0,msgLookup("msg.iselwire"),w->name,g->u.block.moduleName);
    } else if (TkGate.circuit->select) {
      if (!GCElement_isModule(TkGate.circuit->select)) {
	/* should be impossible */
	message(0,msgLookup("msg.iselgate"),
		TkGate.circuit->select->typeinfo->name,
		TkGate.circuit->select->ename);
      } else {
	message(0,msgLookup("msg.iselblock"),
		TkGate.circuit->select->u.block.moduleName);
      }
    }
  } else {
    if (TkGate.circuit->nsel) {
      message(0,msgLookup("msg.selwire"),GNet_getVType(TkGate.circuit->nsel),
	      TkGate.circuit->nsel->n_signame);				/* Selected wire named '%s'. */
    } else if (TkGate.circuit->select) {
      if (!GCElement_isModule(TkGate.circuit->select)) {
	message(0,msgLookup("msg.selgate"), 				/* Selected %s named '%s'. */
		TkGate.circuit->select->typeinfo->name,
		TkGate.circuit->select->ename);
      } else {
	message(0,msgLookup("msg.selblock"), 				/* "Selected %s block named '%s'." */
		TkGate.circuit->select->u.block.moduleName,
		TkGate.circuit->select->ename);
      }
    }
  }
}

/*****************************************************************************
 *
 * Return the pixel value for a named color.
 *
 *****************************************************************************/
int Tkg_GetColor(const char *name)
{
  XColor XC,eXC;

  XAllocNamedColor(TkGate.D,TkGate.CM,name,&XC,&eXC);
  return XC.pixel;
}

/*****************************************************************************
 *
 * Create a graphic context (GC) with the given transfer function, color
 * and font.
 *
 *****************************************************************************/
GC Tkg_createGC(int func,char *colorName,XFontStruct *font)
{
  unsigned mask;
  XGCValues values;
  int color, white;

  white = XWhitePixelOfScreen(TkGate.S);
  color  = Tkg_GetColor(colorName);

  mask = GCForeground | GCBackground | GCFunction | GCGraphicsExposures;

  if (func == GXxor) {
    mask |= GCPlaneMask;
    values.function = GXxor;
    values.foreground = color ^ white;
    values.background = 0;
    values.plane_mask = color ^ white;
  } else {
    values.function = func;
    values.foreground = color;
    values.background = white;
  }

  values.graphics_exposures = False;

  if (font) {
    values.font = font->fid;
    mask |= GCFont;
  }

  return XCreateGC(TkGate.D,TkGate.root,mask,&values);
}

/*****************************************************************************
 *
 * Set initial color for a GC
 *
 *****************************************************************************/
static void Tkg_setColor(GC gc,int func, const char *colorName)
{
  unsigned mask;
  XGCValues values;
  int color, white;

  if (!colorName) return;

  white = XWhitePixelOfScreen(TkGate.S);
  color  = Tkg_GetColor(colorName);

  mask = GCForeground | GCBackground;

  if (func == GXxor) {
    mask |= GCPlaneMask;
    values.foreground = color ^ white;
    values.background = 0;
    values.plane_mask = color ^ white;
  } else {
    values.foreground = color;
    values.background = white;
  }

  XChangeGC(TkGate.D,gc,mask,&values);
}

/*****************************************************************************
 *
 * Change the color of a GC
 *
 *****************************************************************************/
void Tkg_changeColor(GC gc,int func,int pixel)
{
  XGCValues values;
  unsigned mask;
  int white;

  white = XWhitePixelOfScreen(TkGate.S);
  mask = GCForeground | GCBackground;

  if (func == GXxor) {
    mask |= GCPlaneMask;
    values.foreground = pixel ^ white;
    values.background = 0;
    values.plane_mask = pixel ^ white;
  } else {
    values.foreground = pixel;
    values.background = white;
  }

  XChangeGC(TkGate.D,gc,mask,&values);
}

/*****************************************************************************
 *
 * Load Japanese fonts if we have not already done so.
 *
 *****************************************************************************/
void initJapanese()
{
  if (TkGate.D && !TkGate.ktextXF)
    TkGate.ktextXF = GetXFonts(FF_KANJI,FP_ROMAN,FS_SMALL);
}

/*****************************************************************************
 *
 * Set up all of the initial colors in the GCs used by tkgate.
 *
 *****************************************************************************/
void setGCcolors()
{
  const char *bgcolor = Tcl_GetVar(TkGate.tcl,"tkg_backgroundColor",TCL_GLOBAL_ONLY);
  int bgcolor_pixel = Tkg_GetColor(bgcolor);

  Tk_SetWindowBackground(TkGate.gw->win, bgcolor_pixel);

  Tkg_setColor(TkGate.instGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_instColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.moduleGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_moduleColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.modportGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_modulePortColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.frameGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_frameColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.commentGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_commentColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.imageGC,		GXcopy, Tcl_GetVar(TkGate.tcl,"tkg_commentColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.hyperlinkGC,	GXxor, Tcl_GetVar(TkGate.tcl,"tkg_hyperlinkColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.wireGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_wireColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.busGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_busColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.selWireGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_wireColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.selBusGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_busColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.toolGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_toolColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.cpathGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_cpathColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.kanjiGC,		GXxor, Tcl_GetVar(TkGate.tcl,"tkg_commentColor",TCL_GLOBAL_ONLY));

  /*  Tkg_setColor(TkGate.scopeSelectGC,	GXxor,  Tcl_GetVar(TkGate.tcl,"tkg_gridColor",TCL_GLOBAL_ONLY));*/
  Tkg_setColor(TkGate.scopeXHairGC,	GXxor,  Tcl_GetVar(TkGate.tcl,"tkg_gridColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.scopeGridGC,	GXcopy, Tcl_GetVar(TkGate.tcl,"tkg_gridColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.scopeOneGC,	GXcopy, Tcl_GetVar(TkGate.tcl,"tkg_oneColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.scopeZeroGC,	GXcopy, Tcl_GetVar(TkGate.tcl,"tkg_zeroColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.scopeFloatGC,	GXcopy, Tcl_GetVar(TkGate.tcl,"tkg_floatColor",TCL_GLOBAL_ONLY));
  Tkg_setColor(TkGate.scopeUnknownGC,	GXcopy, Tcl_GetVar(TkGate.tcl,"tkg_unknownColor",TCL_GLOBAL_ONLY));

  TkGate.inst_pixel       = Tkg_GetColor(Tcl_GetVar(TkGate.tcl,"tkg_instColor",TCL_GLOBAL_ONLY));
  TkGate.ledoff_pixel     = Tkg_GetColor(Tcl_GetVar(TkGate.tcl,"tkg_offLedColor",TCL_GLOBAL_ONLY));
  TkGate.ledon_pixel      = Tkg_GetColor(Tcl_GetVar(TkGate.tcl,"tkg_onLedColor",TCL_GLOBAL_ONLY));
  TkGate.ledunknown_pixel = Tkg_GetColor(Tcl_GetVar(TkGate.tcl,"tkg_zLedColor",TCL_GLOBAL_ONLY));

  TkGate.comment_pixel = Tkg_GetColor(Tcl_GetVar(TkGate.tcl,"tkg_commentColor",TCL_GLOBAL_ONLY));
  TkGate.hyperlink_pixel = Tkg_GetColor(Tcl_GetVar(TkGate.tcl,"tkg_hyperlinkColor",TCL_GLOBAL_ONLY));
}

/*****************************************************************************
 *
 *  Allocate GCs for main window
 *
 *****************************************************************************/
void initGCs()
{
  unsigned mask;
  XGCValues values;
  Pixmap bitPM;

  /*
     Do font set up
   */
  TkGate.textXF  = GetXFonts(FF_COURIER,FP_ROMAN,FS_NORMAL);
  TkGate.textbXF = GetXFonts(FF_COURIER,FP_BOLD,FS_NORMAL);
  TkGate.stextXF = GetXFonts(FF_COURIER,FP_ROMAN,FS_SMALL);
  TkGate.stextbXF = GetXFonts(FF_COURIER,FP_BOLD,FS_SMALL);
  if (TkGate.japaneseMode)
    TkGate.ktextXF = GetXFonts(FF_KANJI,FP_ROMAN,FS_SMALL);
  else
    TkGate.ktextXF = 0;


  TkGate.inst_pixel = Tkg_GetColor("blue");
  TkGate.ledoff_pixel = Tkg_GetColor("firebrick4");
  TkGate.ledon_pixel = Tkg_GetColor("red");
  TkGate.ledunknown_pixel = Tkg_GetColor("yellow");

  TkGate.instGC       = Tkg_createGC(GXxor,"blue",TkGate.textXF[1]);
  TkGate.moduleGC     = Tkg_createGC(GXxor,"magenta4",TkGate.textXF[1]);
  TkGate.modportGC    = Tkg_createGC(GXxor,"cyan4",TkGate.textXF[1]);
  TkGate.frameGC      = Tkg_createGC(GXxor,"tan4",TkGate.textXF[1]);
  TkGate.commentGC    = Tkg_createGC(GXxor,"tan4",TkGate.textXF[1]);
  TkGate.imageGC      = Tkg_createGC(GXcopy,"tan4",TkGate.textXF[1]);
  TkGate.hyperlinkGC  = Tkg_createGC(GXxor,"tan4",TkGate.textXF[1]);
  TkGate.wireGC       = Tkg_createGC(GXxor,"green4",TkGate.stextXF[1]);
  TkGate.busGC        = Tkg_createGC(GXxor,"red",TkGate.stextXF[1]);
  TkGate.selWireGC    = Tkg_createGC(GXxor,"green4",TkGate.stextbXF[1]);
  TkGate.selBusGC     = Tkg_createGC(GXxor,"red",TkGate.stextbXF[1]);
  TkGate.toolGC       = Tkg_createGC(GXxor,"black",TkGate.textXF[1]);
  TkGate.cpathGC      = Tkg_createGC(GXxor,"red",TkGate.textXF[1]);
  TkGate.kanjiGC      = Tkg_createGC(GXxor,"tan4",TkGate.ktextXF ? TkGate.ktextXF[1] : 0);

  TkGate.copyGC       = Tkg_createGC(GXcopy,"black",TkGate.textXF[1]);
  XSetGraphicsExposures(TkGate.D,TkGate.copyGC,True);

  TkGate.scopeXHairGC   = Tkg_createGC(GXxor,"black",TkGate.textXF[1]);
  TkGate.scopeSelectGC  = Tkg_createGC(GXxor,"grey90",TkGate.textXF[1]);
  TkGate.scopeGridGC    = Tkg_createGC(GXcopy,"black",TkGate.textXF[1]);
  TkGate.scopeOneGC     = Tkg_createGC(GXcopy,"green4",TkGate.stextXF[1]);
  TkGate.scopeZeroGC    = Tkg_createGC(GXcopy,"magenta",TkGate.stextXF[1]);
  TkGate.scopeFloatGC   = Tkg_createGC(GXcopy,"blue",TkGate.stextXF[1]);
  TkGate.scopeUnknownGC = Tkg_createGC(GXcopy,"red",TkGate.stextXF[1]);
  TkGate.scopeClearGC   = Tkg_createGC(GXcopy,"white",TkGate.stextXF[1]);


  XSetLineAttributes(TkGate.D,TkGate.busGC,2,LineSolid,CapButt,JoinMiter);
  XSetLineAttributes(TkGate.D,TkGate.selBusGC,3,LineSolid,CapButt,JoinMiter);
  XSetLineAttributes(TkGate.D,TkGate.selWireGC,2,LineSolid,CapButt,JoinMiter);

  mask = GCLineStyle | GCDashList;
  values.line_style = LineOnOffDash;
  values.dashes = DASH_LENGTH/2;
  XChangeGC(TkGate.D,TkGate.frameGC, mask, &values);

  mask = GCForeground | GCBackground | GCFunction | GCGraphicsExposures; 
  values.foreground = 1;
  values.background = 1;
  values.function = GXcopy;
  values.graphics_exposures = False;

  bitPM = XCreatePixmap(TkGate.D,TkGate.root,8,8,1);
  TkGate.bitGC = XCreateGC(TkGate.D,bitPM,mask,&values);
  XFreePixmap(TkGate.D,bitPM);
}


/*****************************************************************************
 *
 * The TkGateParms object is the top-level data structure used in the
 * original xgate program.  Since there is too much depending on it,
 * just initialize it appropriately for now.
 *
 *****************************************************************************/
static void initGateParms(TkgGateWin *gw,TkGateParams *P)
{
  Tk_MakeWindowExist(gw->win);

  P->W = Tk_WindowId(gw->win);

  P->circuit->es = new_EditState();
  P->circuit->es->env = env_defineModule("main",1);
  editstate_setCurrent(P->circuit->es);
  TkGate.circuit->root_mod = P->circuit->es->env;
  TkGate.circuit->no_set_modify = 0;
  TkGate_setOrigin(0,0);
  ClearModified();
}

/*****************************************************************************
 *
 * gateCleanUp - This function is called when exiting tkgate to cleanup any
 * data structures that need to be explicitly released before we exit.
 *
 *****************************************************************************/
void gateCleanUp()
{
  UnloadAllFonts();			/* Free all font structures */
}

/*****************************************************************************
 *
 * Rescan all HDL modules
 *
 * This function is called as an idle task while in edit mode.  It looks for
 * any HDL modules that have been modified since the last scan and does a scan
 * on them by running the simulator in scan mode.
 *
 *****************************************************************************/
void RescanHDLModules()
{
  HashElem *e;
  SHash *modules = TkGate.circuit->moduleTable;
  int anyscanned = 0;

  for (e = Hash_first(modules);e;e = Hash_next(modules,e)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(e);
    if (GModuleDef_getType(M) == MT_TEXTHDL && M->m_needScan) {
      if (!anyscanned)
	ob_begin_framef("ScanMod",FF_TRANSPARENT|FF_BACKGROUND);
      GModuleDef_scanHDLModule(M);
      anyscanned = 1;
    }
  }

  if (anyscanned) {
    SetModified(MF_MODULE|MF_SYNCONLY);
    ob_end_frame();
  }
} 


/*****************************************************************************
 *
 * This function is called when the tk event queue becomes idle.
 *
 *****************************************************************************/
void idleGateWin(ClientData data)
{
  extern int want_exit;
  MajorMode cm = tkgate_currentMode();

  if (want_exit) {
    switch (cm) {
    case MM_SIMULATE :
      if (TkGate.circuit->simulator.active)
	SimInterface_end(&TkGate.circuit->simulator);
      break;
    case MM_ANALYZE :
      cpath_close();
      break;
    case MM_EDIT :
      break;
    }
    DoTcl("File::closeApplication");
    want_exit = 0;
  }

#if TOUCH_XGATE_ED
  ob_begin_framef("-Idle",FF_TRANSPARENT);
#endif

  if (TkGate.idle_ev.scroll_area) {
    ob_begin_framef("Scroll",FF_STICKY);
    doScroll(wtoc_x(TkGate.idle_ev.scroll_new_x),
	     wtoc_y(TkGate.idle_ev.scroll_new_y));
    ob_end_frame();

    TkGate.idle_ev.scroll_new_x = TkGate.circuit->org_x;
    TkGate.idle_ev.scroll_new_y = TkGate.circuit->org_y;
    TkGate.idle_ev.scroll_area = 0;
    TkGate.idle_ev.redraw = 0;
  } else {
    if (TkGate.idle_ev.redraw) {
      editstate_fullUpdate(TkGate.circuit->es);
      TkGate.idle_ev.redraw = 0;
    }
  }

  if (TkGate.idle_ev.scope_redraw || TkGate.idle_ev.trace_redraw) {
    extern GScope *Scope;
    if (Scope)
      GScope_fullUpdate(Scope);
    TkGate.idle_ev.scope_redraw = 0;
    TkGate.idle_ev.trace_redraw = 0;
  }

#if TOUCH_XGATE_ED
  ob_end_frame();
#endif

  /*
   * Synchronize main tabs with actual edit state.
   */
  switch (cm) {
  case MM_SIMULATE :
    DoTcl("syncMainTabs Simulate");
    break;
  case MM_ANALYZE :
    DoTcl("syncMainTabs CriticalPath");
    break;
  case MM_EDIT :
    {
      EditState *es = TkGate.circuit->es;
      if (es && es->isInterface)
	DoTcl("syncMainTabs EditInterfaces");
      else
	DoTcl("syncMainTabs Edit");
    }
    RescanHDLModules();
    break;
  }

  SynchronizeInterface();

  TkGate.idle_ev.pending = 0;
}

/*****************************************************************************
 *
 * This function is called to request that a redraw be performed the
 * next time the tk event queue becomes empty.
 *
 *****************************************************************************/
void FlagRedraw()
{
  TkGate.idle_ev.redraw = 1;
  TkGate.idle_ev.scroll_area = 0;

  if (!TkGate.idle_ev.pending) {
    TkGate.idle_ev.pending = 1;
    Tk_DoWhenIdle(idleGateWin,0);
  }
}

/*****************************************************************************
 *
 * This function is called to request that scrolling be done the next time
 * the tk event queue becomes empty.  Calls to this function accumulate
 * x and y change, and when the action is performed the combined scrolling
 * action is performed.
 *
 *****************************************************************************/
void FlagScrolling()
{
  if (!TkGate.idle_ev.redraw) {
    TkGate.idle_ev.redraw = 1;
    TkGate.idle_ev.scroll_area = 1;
  }

  if (!TkGate.idle_ev.pending) {
    TkGate.idle_ev.pending = 1;
    Tk_DoWhenIdle(idleGateWin,0);
  }
}

/*****************************************************************************
 *
 * Handle an X event for the main tkgate window (primarily Exposure events)
 * that hasn't already been handled elsewhere.
 *
 *****************************************************************************/
static void gateWinEvent(ClientData data, XEvent *E)
{
#if TOUCH_XGATE_ED
  ob_begin_framef("-WEvent",FF_TRANSPARENT);
#endif

  switch (E->type) {
  case DestroyNotify :
    gateCleanUp();
    break;
  case GraphicsExpose :
    if (TkGate.regionUpdate) {
      XGraphicsExposeEvent *xE = &E->xgraphicsexpose;
      editstate_regionUpdate(TkGate.circuit->es,xE->x,xE->y,xE->x+xE->width,xE->y+xE->height);
    } else {
      FlagRedraw();
    }
    break;
  case Expose :
    if (TkGate.regionUpdate) {
      XExposeEvent *xE = &E->xexpose;
      editstate_regionUpdate(TkGate.circuit->es,xE->x,xE->y,xE->x+xE->width,xE->y+xE->height);
    } else {
      FlagRedraw();
    }
    break;
  case NoExpose :
    break;
  default :
    FlagRedraw();
  }
#if TOUCH_XGATE_ED
  ob_end_frame();
#endif
}

/*****************************************************************************
 *
 * Update the positions of the scroll bars to reflect the new window visiblity.
 *
 *****************************************************************************/
void scrollbar_update()
{
  int width = TkGate.width/TkGate.circuit->zoom_factor;
  int height = TkGate.height/TkGate.circuit->zoom_factor;
  int x0 = -TkGate.circuit->org_x;
  int y0 = -TkGate.circuit->org_y;
  int min_x = TkGate.ed->min_x;
  int min_y = TkGate.ed->min_y;
  int max_x = TkGate.ed->max_x;
  int max_y = TkGate.ed->max_y;
  double range_x = 1.0/(double)imax(10,max_x-min_x);
  double range_y = 1.0/(double)imax(10,max_y-min_y);
  double xb = 0.0;
  double xe = 1.0;
  double yb = 0.0;
  double ye = 1.0;

  xb = (x0          - min_x)*range_x;
  xe = (x0 + width  - min_x)*range_x;
  yb = (y0          - min_y)*range_y;
  ye = (y0 + height - min_y)*range_y;

  if (xb < 0) xb = 0;
  if (xe > 1) xe = 1;
  if (yb < 0) yb = 0;
  if (ye > 1) ye = 1;

  if (min_x >= max_x) {
    xb = yb = 0;
    xe = ye = 1;
  }

  DoTcl("%s %lf %lf",TkGate.gw->xscroll,xb,xe);
  DoTcl("%s %lf %lf",TkGate.gw->yscroll,yb,ye);
#if 0
  printf("Y-Scroll: [%d..%d] (%d..%d) --> %s %lf %lf\n",min_y,max_y,y0,y0+TkGate.height,TkGate.gw->yscroll,yb,ye);
  printf("X-Scroll: [%d..%d] (%d..%d) --> %s %lf %lf\n",min_x,max_x,x0,x0+TkGate.width ,TkGate.gw->xscroll,xb,xe);
#endif
}

/*****************************************************************************
 *
 * Get the bounding box and update the scroll bars to reflect it.
 *
 *****************************************************************************/
void scrollbar_bbx_update()
{
#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  GModuleDef_getBBX(TkGate.circuit->es->env, TD_X11,
		    &TkGate.ed->min_x,&TkGate.ed->max_x,
		    &TkGate.ed->min_y,&TkGate.ed->max_y);
  scrollbar_update();
}


/*****************************************************************************
 *
 * Parse a scroll distance specification.
 *
 *****************************************************************************/
static int parseScrollDistance(int argc,const char *argv[],int org,int min_c,int max_c,int range_c,int *new_org_c)
{
  int len = max_c - min_c;
  int old_new_org_c = *new_org_c;				/* Old value of coordinate update value */
  int x;

  if (argc == 1) {						/* n */
    sscanf(argv[0],"%d",&x);
  } else if (argc == 2 && strcmp(argv[0],"press") == 0) {	/* press n */
    int b;
    sscanf(argv[1],"%d",&b);

    scrollbar_update();

    TkGate.ed->scr_x = TkGate.circuit->org_x;
    TkGate.ed->scr_y = TkGate.circuit->org_y;
  } else if (argc == 2 && strcmp(argv[0],"moveto") == 0) {	/* moveto n */
    double dx;
    sscanf(argv[1],"%lf",&dx);

    *new_org_c = -(min_c + (int) (dx*len)) - imin(0,-org-min_c);


  } else if (argc == 3 && strcmp(argv[0],"scroll") == 0		/* scroll n pages */
	     && strcmp(argv[2],"pages") == 0) {
    double dx;
    sscanf(argv[1],"%lf",&dx);

    *new_org_c -= (int) (dx*len);
  } else if (argc == 3 && strcmp(argv[0],"scroll") == 0		/* scroll n units */
	     && strcmp(argv[2],"units") == 0) {
    double dx;
    sscanf(argv[1],"%lf",&dx);

    *new_org_c -= (int) (dx*50);
  } else if (argc == 3 && strcmp(argv[0],"scroll") == 0		/* scroll n pixels */
	     && strcmp(argv[2],"pixels") == 0) {
    double dx;
    sscanf(argv[1],"%lf",&dx);

    *new_org_c -= (int) (dx);
  }

  if (-*new_org_c > max_c + SCROLL_LIMIT)
    *new_org_c = -(max_c + SCROLL_LIMIT);
  if (-*new_org_c < min_c - range_c - SCROLL_LIMIT)
    *new_org_c = -(min_c - range_c - SCROLL_LIMIT);


  return old_new_org_c != *new_org_c;
}

/*****************************************************************************
 *
 * Parse an X distance scroll specification
 *
 *****************************************************************************/
static int parseXScrollDistance(int argc,const char *argv[])
{
  int x0 = TkGate.ed->scr_x;
  int min_x = TkGate.ed->min_x;
  int max_x = imax(TkGate.ed->max_x,TkGate.ed->min_x+TkGate.width/TkGate.circuit->zoom_factor);

  return parseScrollDistance(argc,argv,x0,min_x,max_x,TkGate.width/TkGate.circuit->zoom_factor,
			     &TkGate.idle_ev.scroll_new_x);
}

/*****************************************************************************
 *
 * Parse an Y distance scroll specification
 *
 *****************************************************************************/
static int parseYScrollDistance(int argc,const char *argv[])
{
  int y0 = TkGate.ed->scr_y;
  int min_y = TkGate.ed->min_y;
  int max_y = imax(TkGate.ed->max_y,TkGate.ed->min_y+TkGate.height/TkGate.circuit->zoom_factor);

  return parseScrollDistance(argc,argv,y0,min_y,max_y,TkGate.height/TkGate.circuit->zoom_factor,&TkGate.idle_ev.scroll_new_y);
}

/*
 * Do a scrolling update in the circuit window.
 */
void doScroll(int dx,int dy)
{
  /*
   * If circuit is empty, return to default position.
   */
  if (TkGate.ed->min_x >= TkGate.ed->max_x) {
    TkGate_setOrigin(0,0);
    return;
  }

  if (dx != 0) {
    int x1,y1,w,h,x2,y2;
    int delta = -dx*TkGate.circuit->zoom_factor;

    h = TkGate.height;
    w = TkGate.width;
    y1 = y2 = 0;
    if (delta > 0) {
      x1 = delta;
      x2 = 0;
    } else {
      x1 = 0;
      x2 = -delta;
    }

    XCopyArea(TkGate.D,TkGate.W,TkGate.W,TkGate.copyGC,x1,y1,w,h,x2,y2);

    TkGate_setOrigin(TkGate.circuit->org_x + dx,TkGate.circuit->org_y);
    if (delta > 0) {
      editstate_regionUpdate(TkGate.circuit->es,w-delta-1,0,w+1,h+1);
    } else {
      editstate_regionUpdate(TkGate.circuit->es,0,0,-delta+1,h+1);
    }
  }
  if (dy != 0) {
    int x1,y1,w,h,x2,y2;
    int delta = -dy*TkGate.circuit->zoom_factor;

    h = TkGate.height;
    w = TkGate.width;
    x1 = x2 = 0;
    if (delta > 0) {
      y1 = delta;
      y2 = 0;
    } else {
      y1 = 0;
      y2 = -delta;
    }

    XCopyArea(TkGate.D,TkGate.W,TkGate.W,TkGate.copyGC,x1,y1,w,h,x2,y2);

    TkGate_setOrigin(TkGate.circuit->org_x,TkGate.circuit->org_y + dy);
    if (delta > 0) {
      editstate_regionUpdate(TkGate.circuit->es,0,h-delta-1,w+1,h+1);
    } else {
      editstate_regionUpdate(TkGate.circuit->es,0,0,w+1,-delta+1);
    }
  }
}

/*****************************************************************************
 *
 * Respond to a tcl "window" command on the main edit window.  Normally this
 * is a scroll bar change notice.
 *
 *****************************************************************************/
static int gateWinCommand(ClientData data, Tcl_Interp *tcl, int argc, const char *argv[])
{
  TkgGateWin *gw = (TkgGateWin*) data;


  if (strcmp(argv[1],"xview") == 0) {
    TkGate.idle_ev.scroll_new_x = TkGate.circuit->org_x;
    TkGate.idle_ev.scroll_new_y = TkGate.circuit->org_y;
    if (parseXScrollDistance(argc-2,argv+2)) {
      if (TkGate.smoothScroll) {
	FlagScrolling();
      } else {
	ob_begin_framef("Scroll",FF_TRANSPARENT);
	TkGate_setOrigin(TkGate.idle_ev.scroll_new_x,TkGate.circuit->org_y);
	ob_end_frame();
	FlagRedraw();
      }
    }
  } else if (strcmp(argv[1],"yview") == 0) {
    TkGate.idle_ev.scroll_new_x = TkGate.circuit->org_x;
    TkGate.idle_ev.scroll_new_y = TkGate.circuit->org_y;
    if (parseYScrollDistance(argc-2,argv+2)) {
      if (TkGate.smoothScroll) {
	FlagScrolling();
      } else {
	ob_begin_framef("Scroll",FF_TRANSPARENT);
	TkGate_setOrigin(TkGate.circuit->org_x, TkGate.idle_ev.scroll_new_y);
	ob_end_frame();
	FlagRedraw();
      }
    }
  } else if (argc == 3 && strcmp(argv[1],"cget") == 0) {
    int i;

    for (i = 0;i < numConfigSpecs;i++) {
      Tk_ConfigSpec *C = &configSpecs[i];
      if (C->type == TK_CONFIG_STRING && strcmp(C->argvName,argv[2]) == 0) {
	char *value = *(char**)(((char*)gw) + C->offset);
	strcpy(tcl->result,value);
	break;
      }
    }
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Called if our call to Tk_ConfigureWidget failed.  It seems to be broken
 * in some tcl/tk versions.
 *
 *****************************************************************************/
void configWarning(int p)
{
  printf("\n- FATAL ERROR -\n\n");
  printf("If you get this message, it probably means that Tk_ConfigureWidget is\n");
  printf("broken on your machine.  Try recompiling with TKGATE_BROKENCONFIGWIDGET\n");
  printf("set to 1 in the top-level file config.h.\n");

  exit(1);
}

/*****************************************************************************
 *
 * Callback function that is called when the size of the main window is changed
 * by the user, normally be resizing with the mouse.
 *
 *****************************************************************************/
static int tkg_configMain(ClientData data, Tcl_Interp *tcl, int argc, const char **argv)
{
  TkgGateWin *gw = (TkgGateWin *) data;

#if TKGATE_BROKENCONFIGWIDGET
  char *wstr = Tcl_GetVar(TkGate.tcl,"tkg_initialWidth",TCL_GLOBAL_ONLY);
  char *hstr = Tcl_GetVar(TkGate.tcl,"tkg_initialHeight",TCL_GLOBAL_ONLY);
  if (wstr) sscanf(wstr,"%d",&gw->width);
  if (hstr) sscanf(hstr,"%d",&gw->height);
  gw->xscroll = ".horz set";
  gw->yscroll = ".vert set";
  gw->takefocus = 0;

  Tk_SetWindowBackground(gw->win, XWhitePixelOfScreen(TkGate.S));
#else
  signal(SIGBUS,configWarning);
  signal(SIGSEGV,configWarning);

  if (Tk_ConfigureWidget(tcl, gw->win, configSpecs, argc, argv, (char*) gw, 0) != TCL_OK)
    return TCL_ERROR;

  Tk_SetWindowBackground(gw->win, gw->bgColor->pixel);

  signal(SIGBUS,SIG_DFL);
  signal(SIGSEGV,SIG_DFL);
#endif

  Tk_GeometryRequest(gw->win,gw->width,gw->height);

  if (gw->gc == None) {
    XGCValues gcv;

    gcv.function = GXxor;
    gcv.graphics_exposures = False;
    gw->gc = Tk_GetGC(gw->win,GCFunction|GCGraphicsExposures, &gcv);

  }

  did_interface_resize = 1;

  return TCL_OK;
}

/*****************************************************************************
 *
 * Called when tkgate window is externally destroyed.
 *
 *****************************************************************************/
static void gateWinDestroy(ClientData data)
{
}

/*****************************************************************************
 * Process the command for creating the main tkgate circuit editing window.
 * 
 * Usage: gatewin .w
 *
 *****************************************************************************/
static int tkg_gateWin(ClientData data, Tcl_Interp *tcl, int argc, const char **argv)
{
  Tk_Window root = (Tk_Window) data;
  TkgGateWin *gw;
  Tk_Window w;

  if (argc < 2) {
    Tcl_AppendResult(tcl, "wrong # args: should be \"",argv[0], " pathName ?options?\"", 0);
    return TCL_ERROR;
  }

  w = Tk_CreateWindowFromPath(tcl, root, argv[1], 0);
  if (!w) return TCL_ERROR;
  Tk_SetClass(w, "GateWin");

  gw = (TkgGateWin*) ob_malloc(sizeof(TkgGateWin),"TkgGateWin");
  gw->win = w;
  gw->d = Tk_Display(w);
  gw->tcl = tcl;
  gw->width = TKG_GATEWIN_WIDTH;
  gw->height = TKG_GATEWIN_HEIGHT;
  gw->gc = None;
  gw->bgColor = gw->fgColor = 0;
  gw->parms = &TkGate;
  gw->xscroll = 0;
  gw->yscroll = 0;
  gw->takefocus = 0;
  TkGate.gw = gw;

  initGateParms(gw,&TkGate);

  Tk_CreateEventHandler(w, ExposureMask|StructureNotifyMask,
			gateWinEvent, gw);
  Tcl_CreateCommand(tcl, Tk_PathName(w), gateWinCommand, gw, gateWinDestroy);
  if (tkg_configMain(gw, tcl, argc-2, argv+2) != TCL_OK) {
    Tk_DestroyWindow(w);
    return TCL_ERROR;
  }

  setGCcolors();

  tcl->result = Tk_PathName(w);

  return TCL_OK;
}

/*****************************************************************************
 *
 * Do a tcl/tk script command.  Arguments act like printf and are used to 
 * construct the command before execution.
 *
 *****************************************************************************/
int DoTcl(const char *str,...)
{
  char buf[1024];
  int r;
  va_list ap;

  /*
   * Ignore tcl commands invoked before starting up the editor.
   */
  if (!TkGate.tcl) return TCL_OK;

  va_start(ap,str);

  vsprintf(buf,str,ap);
  r = Tcl_Eval(TkGate.tcl,buf);
  va_end(ap);

  if (r != TCL_OK) {
    printf("DoTCL Error: %s\n",Tcl_GetStringResult(TkGate.tcl));
    //    printf("DoTCL Error: %s\n",TkGate.tcl->result);
    printf("   while executing: %s\n",buf);
  }

  return r;
}

/*****************************************************************************
 *
 * Do a tcl/tk script command constructed from string objects.
 *
 *****************************************************************************/
int DoTclL(const char *cmd,...)
{
  Tcl_Obj *objv[50];
  int n;
  int r;
  va_list ap;

  objv[0] = Tcl_NewStringObj(cmd,strlen(cmd));

  va_start(ap,cmd);
  for (n = 1;n < 50;n++) {
    char *s = va_arg(ap,char*);
    if (!s) break;
    objv[n] = Tcl_NewStringObj(s,strlen(s));
  }
  va_end(ap);

  r = Tcl_EvalObjv(TkGate.tcl, n, objv, TCL_EVAL_DIRECT|TCL_EVAL_GLOBAL);
  /* Are the objects freed here? */

  if (r != TCL_OK) {
    printf("tkgate: DoTclL Error - %s\n",TkGate.tcl->result);
    printf("   while executing: %s\n",cmd);
  }


  return r;
}

/*****************************************************************************
 *
 * Do a tcl/tk script command constructed from string objects.
 *
 *****************************************************************************/
int DoTclV(const char *cmd,int nargs,const char **args)
{
  Tcl_Obj *objv[50];
  int n;
  int r;

  objv[0] = Tcl_NewStringObj(cmd,strlen(cmd));

  for (n = 1;n < 50;n++) {
    if (nargs-- <= 0) break;
    objv[n] = Tcl_NewStringObj(args[n-1],strlen(args[n-1]));
  }

  r = Tcl_EvalObjv(TkGate.tcl, n, objv, TCL_EVAL_DIRECT|TCL_EVAL_GLOBAL);
  /* Are the objects freed here? */

  if (r != TCL_OK) {
    printf("DoTclV Error: %s\n",TkGate.tcl->result);
    printf("   while executing: %s\n",cmd);
  }


  return r;
}



/*****************************************************************************
 *
 * Handler function for button down events in the tkgate edit window.
 *
 *****************************************************************************/
static int tkg_buttonPress(ClientData data, Tcl_Interp *tcl, int argc, const char *argv[])
{
  EditState *es = TkGate.circuit->es;
  int x,y,state,button;

  click_count++;
  
  DoTcl("tkg_cancel");
  DoTcl("tkg_undoSelections gate");

  sscanf(argv[2],"%d",&x);
  sscanf(argv[3],"%d",&y);
  sscanf(argv[4],"%d",&state);
  sscanf(argv[5],"%d",&button);

  if (button == 2) {
    TkGate.ed->saved_mode = EditState_getMode();
    DoTcl("ToolBar::selectTool 3");
    button = 1;
  }

  TkGate.ed->rx = x;
  TkGate.ed->ry = y;

  x /= TkGate.circuit->zoom_factor;
  y /= TkGate.circuit->zoom_factor;

  TkGate.state = state;
  TkGate.button = button;

#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->lx = TkGate.ed->tx;
  TkGate.ed->ly = TkGate.ed->ty;
  TkGate.ed->tx = wtoc_x(x);
  TkGate.ed->ty = wtoc_y(y);

  /*
   * Check to see if we are selecting a hyperlink.
   */
  if (!(state & (ShiftMask|ControlMask))
      && (EditState_getMode() == MODE_MOVE)
      && Hyperlink_selectAt(TkGate.ed->tx,TkGate.ed->ty)) {
    return TCL_OK;
  }

  /*
   * Special handling for scrolling mode
   */
  if (EditState_getMode() == MODE_SCROLL) {
    HandScroll_set(es);
    return TCL_OK;
  }

  switch (tkgate_currentMode()) {
  case MM_SIMULATE :
    SimInterface_hit(&TkGate.circuit->simulator,TkGate.ed->tx,TkGate.ed->ty,0);
    break;
  case MM_ANALYZE :
    cpath_mouseDown(es);
    break;
  case MM_EDIT :
    ClearErrorMark();
    mark_unpost();
    EditState_selectobject(es);
    mark_post();
    break;
  }


  showSelectedName();

  return TCL_OK;
}

/*****************************************************************************
 *
 * Handler function for double clicks in the tkgate edit window.
 *
 *****************************************************************************/
static int tkg_buttonDoublePress(ClientData data, Tcl_Interp *tcl, int argc, const char *argv[])
{
  if (click_count < 1) return TCL_OK;

  switch (tkgate_currentMode()) {
  case MM_SIMULATE :
    SimInterface_hit(&TkGate.circuit->simulator,TkGate.ed->tx,TkGate.ed->ty,1);
    break;
  case MM_ANALYZE :
    break;
  case MM_EDIT :
    if (EditState_getMode() == MODE_MOVE) {
      did_doubleclick = 1;
    }
    break;
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Generate a checkpoint file name from the specified base filename.
 *
 *****************************************************************************/
int getCheckpointFilename(char *checkPointFile,const char *fileName,size_t size)
{
  const char *p,*l;
  char *q;
  int n;

  if (!fileName) {
      strcpy(checkPointFile,"#checkpoint#");
      return 0;
  }

  if (strlen(fileName)+2 >= size)
    return -1;

  /*
   * Start with a copy of the file name. 
   */
  strcpy(checkPointFile,fileName);

  /*
   * Find the base name of the file.
   */
  p = strrchr(fileName,'/');
  if (p) {
    p++;
    q = checkPointFile+(p-fileName);
  } else {
    p = fileName;
    q = checkPointFile;
  }

  /*
   * The filename part of the path was empty (this condition
   * is probably not possible).
   */
  if (!*p) return -1;

  /*
   * We are editing a checkpoint file, do not create further checkpoints.
   */
  if (*p == '#' && p[strlen(p)-1] == '#')
    return -1;


  /*
   * Create the checkpoint file name.
   */
  sprintf(q,"#%s#",p);

  return 0;
}

/*****************************************************************************
 *
 * Test if we want to write the checkpoint file and if so, write it.
 *
 *****************************************************************************/
static void doCheckpointTest()
{
  extern int wantCheckpoint,checkpointenabled;
  const char *currentFile = CurrentFile_path(TkGate.circuit->currentFile);

  if (wantCheckpoint && checkpointenabled && TkGate.changedp
      && !TkGate.circuit->discardChanges && currentFile) {
    char buf[STRMAX];

    /* Generate checkpoint file name */
    if (getCheckpointFilename(buf,currentFile,STRMAX) == 0) {
      message(0,msgLookup("msg.wroteckpt"),buf);  /* Checkpointed to %s... */
      VerilogWriteModules(buf,VSO_NOHDLCHECK);	  /* Write the checkpoint file */
    }
  }

  wantCheckpoint = 0;
}

static void checkMouseoverAction(int x,int y)
{
  EditState *es = TkGate.circuit->es;
  int tx = wtoc_x(x);
  int ty = wtoc_y(y);
  const char *link = 0;
  static const char *visible_link = 0;

  if (EditState_getMode() == MODE_MOVE)
    link = Hyperlink_getAt(tx,ty);

  if (link) {
    MouseoverCursor(HYPERLINKCURSOR);
  } else if (tkgate_currentMode() == MM_EDIT && EditState_getMode() == MODE_MOVE) {
    GCElement *g = gate_hit(es->env,tx,ty);
    GWireNode *n = wire_hit(tx,ty,es->env->m_wires);

    int pside = PSIDE_UNKNOWN;

    if (!n && g && GCElement_getType(g) == GC_BLOCK) {
      pside = Block_HitEdge(g,tx,ty);
    }

    switch (pside) {
    case PSIDE_LEFT :
      MouseoverCursor(ADDPORTLEFT);
      break;
    case PSIDE_RIGHT :
      MouseoverCursor(ADDPORTRIGHT);
      break;
    case PSIDE_BOTTOM :
      MouseoverCursor(ADDPORTBOTTOM);
      break;
    case PSIDE_TOP :
      MouseoverCursor(ADDPORTTOP);
      break;
    default :
      MouseoverCursor(CANCELMOUSEOVER);
      break;
    }
  } else
    MouseoverCursor(CANCELMOUSEOVER);


  /*
   * Update the status line if mouse is over a link
   */
  if (link != visible_link) {
    if (link)
      message(0,link);
    else
      message(0,"");

    visible_link = link;
  }
}


/*****************************************************************************
 *
 * Handler function for button release events in the tkgate edit window.
 *
 *****************************************************************************/
static int tkg_buttonRelease(ClientData data, Tcl_Interp *tcl, int argc, const char *argv[])
{
  EditState *es = TkGate.circuit->es;
  int x,y,state;
  int ret = TCL_OK;

  DoTcl("tkg_hideTempMessage");

  sscanf(argv[2],"%d",&x);
  sscanf(argv[3],"%d",&y);
  sscanf(argv[4],"%d",&state);

  TkGate.ed->rx = x;
  TkGate.ed->ry = y;

  x /= TkGate.circuit->zoom_factor;
  y /= TkGate.circuit->zoom_factor;

  TkGate.state = state;
  TkGate.button = -1;
#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->lx = TkGate.ed->tx;
  TkGate.ed->ly = TkGate.ed->ty;
  TkGate.ed->tx = wtoc_x(x);
  TkGate.ed->ty = wtoc_y(y);

  if (TkGate.circuit->select || TkGate.circuit->nsel)
    message(0,"");

  if (EditState_getMode() == MODE_SCROLL) {
    HandScroll_drop(es);
    wm_SetCursor(SCROLLCURSOR);

    if (TkGate.ed->saved_mode >= 0) {
      DoTcl("ToolBar::selectTool %d",TkGate.ed->saved_mode);
      TkGate.ed->saved_mode = -1;
    }
    return TCL_OK;
  }

  switch (tkgate_currentMode()) {
  case MM_SIMULATE :
    SimInterface_hitRelease(&TkGate.circuit->simulator);
    ret = TCL_OK;
    break;
  case MM_ANALYZE :
    cpath_mouseUp(es);
    ret = TCL_OK;
    break;
  case MM_EDIT :
    mark_unpost();

    EditState_dropobject(es);
    ob_touch(TkGate.circuit);
    TkGate.circuit->wnsel = NULL;
    TkGate.circuit->wsel = NULL;

    if (es->isInterface && did_interface_resize) {
      modint_arrange(es);
      FlagRedraw();
      did_interface_resize = 0;
    }

    if (did_doubleclick && EditState_getMode() == MODE_MOVE) {
      GCElement *g;
      GWire *w;
      EditState_selectobject(es);
      g = TkGate.circuit->select;
      w = TkGate.circuit->wsel;
      EditState_dropobject(es);
      mark_unpost();

      if (g) {
	ob_suggest_name("EditProps");
	DoTcl("gat_editProps");
      } else if (w) {
	if (!es->isInterface) {
	  ob_suggest_name("EditProps");
	  net_editProps(w->net,TkGate.ed->tx,TkGate.ed->ty);
	}
      }
    }

    did_doubleclick = 0;

    doCheckpointTest();

    scrollbar_bbx_update();

    ret = TCL_OK;
    break;
  }

  if (debugContinuousVerify)
    verify_circuit();

  SynchronizeInterface();

  checkMouseoverAction(x,y);

  Hyperlink_confirmAt(TkGate.ed->tx,TkGate.ed->ty);

  return ret;
}


/*****************************************************************************
 *
 * Handler function for button motion events in the tkgate edit window.
 *
 *****************************************************************************/
static int tkg_buttonMotion(ClientData data, Tcl_Interp *tcl, int argc, const char *argv[])
{
  EditState *es = TkGate.circuit->es;
  int x,y,state;

  if (Hyperlink_isPending()) return TCL_OK;

  sscanf(argv[2],"%d",&x);
  sscanf(argv[3],"%d",&y);
  sscanf(argv[4],"%d",&state);

  TkGate.ed->rx = x;
  TkGate.ed->ry = y;

  x /= TkGate.circuit->zoom_factor;
  y /= TkGate.circuit->zoom_factor;

  /*
   * Mouseover cursor handling if no buttons are pressed
   */
  if (!(state & (Button1Mask|Button2Mask|Button3Mask))) {
    checkMouseoverAction(x,y);
    return TCL_OK;
  }

  /*
   * Only track button 1 or 2 motin
   */
  if (!(state & (Button1Mask|Button2Mask)))
    return TCL_OK;


  /*
   * Special handling for scrolling
   */
  if (EditState_getMode() == MODE_SCROLL) {
    HandScroll_move(es);
    return TCL_OK;
  }

  switch (tkgate_currentMode()) {
  case MM_SIMULATE :
    break;
  case MM_ANALYZE :
    break;
  case MM_EDIT :
    TkGate.state = state;
#if TOUCH_XGATE_ED
    ob_touch(TkGate.ed);
#endif
    TkGate.ed->lx = TkGate.ed->tx;
    TkGate.ed->ly = TkGate.ed->ty;
    TkGate.ed->tx = wtoc_x(x);
    TkGate.ed->ty = wtoc_y(y);

    EditState_moveobject(es);

    break;
  }

  return TCL_OK;
}

/*****************************************************************************
 *
 * Clear the selection.
 *
 *****************************************************************************/
void TkGate_clearSelection()
{
#if TOUCH_XGATE_ED
  ob_touch(TkGate.ed);
#endif
  TkGate.ed->tx = TkGate.ed->ty = TkGate.ed->lx = TkGate.ed->ly = TkGate.ed->sx = TkGate.ed->sy = 0;
  TkGate.circuit->select = 0;
  TkGate.circuit->last = 0;
  TkGate.circuit->wsel = 0;
  TkGate.circuit->wnsel = 0;
  TkGate.circuit->nsel = 0;
}

/*****************************************************************************
 *
 * Set up widget for the main tkgate window.
 *
 *****************************************************************************/
void init_mainWindow(Tcl_Interp *tcl)
{
  Tk_Window root = Tk_MainWindow(tcl);
  Tk_MakeWindowExist(root);

  TkGate.circuit = new_Circuit();
  TkGate.ed = (EditData*) ob_malloc(sizeof(EditData),"EditData");
  TkGate.ed->mark_vis = 0;
  TkGate.ed->mark_posted = 0;
  TkGate.ed->major_mode = MM_EDIT;
  TkGate.ed->saved_mode = -1;
  TkGate.errl = new_ErrorList();
  TkGate.tcl = tcl;
  TkGate.D = Tk_Display(root);
  TkGate.root = Tk_WindowId(root);
  TkGate.S = XDefaultScreenOfDisplay(TkGate.D);
  TkGate.CM = DefaultColormapOfScreen(TkGate.S);
  TkGate.ScopeW = None;
  TkGate.bitorder = XBitmapBitOrder(TkGate.D);
  TkGate.byteorder = XImageByteOrder(TkGate.D);
  TkGate.rdb = XrmGetDatabase(TkGate.D);
  TkGate.ErrorMarkTimeout = 0;

  TkGate_clearSelection();

  SimInterface_init(&TkGate.circuit->simulator);

  if (sync_Xserver) {
    printf("[synchonized X11 connection]\n");
    XSynchronize(TkGate.D,True);
  }
  
  initGCs();
  
  Tcl_CreateCommand(tcl,"gat_scope"
		    ,gat_scope
		    ,(ClientData)root
		    ,0);

  Tcl_CreateCommand(tcl,"tkg_gatewin"
		    ,tkg_gateWin
		    ,(ClientData)root
		    ,0);

  Tcl_CreateCommand(tcl,"tkg_configMain"
		    ,tkg_configMain
		    ,(ClientData)root
		    ,0);

  Tcl_CreateCommand(tcl,"tkg_buttonPress"
		    ,tkg_buttonPress
		    ,(ClientData)root
		    ,0);

  Tcl_CreateCommand(tcl,"tkg_buttonDoublePress"
		    ,tkg_buttonDoublePress
		    ,(ClientData)root
		    ,0);

  Tcl_CreateCommand(tcl,"tkg_buttonRelease"
		    ,tkg_buttonRelease
		    ,(ClientData)root
		    ,0);

  Tcl_CreateCommand(tcl,"tkg_buttonMotion"
		    ,tkg_buttonMotion
		    ,(ClientData)root
		    ,0);
}

