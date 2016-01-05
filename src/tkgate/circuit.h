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
#ifndef __circuit_h
#define __circuit_h

/*
 * Time unit codes
 */
#define UC_SEC		0
#define UC_MILLISEC	1
#define UC_MICROSEC	2
#define UC_NANOSEC	3
#define UC_PICOSEC	4
#define UC_FEMTOSEC	5

/*****************************************************************************
 *
 * Timing violation mode
 *
 *****************************************************************************/
#define TV_NONE		0
#define TV_AFTER	1
#define TV_ALL		2

/*****************************************************************************
 *
 * Representation of the current file.
 *
 *****************************************************************************/
struct CurrentFile_str {
  char	*baseName;		/* Base name of current circuit file */
  char	*dirName;		/* Name of directory for current file */
  char	*fullPathName;		/* Full path name of the current file */
};

struct GrabbedLabel_str {
  GNet		*net;		/* Net whose label has been grabbed */
  char		*label;		/* Text of label */
  int		ox,oy;		/* Offset from mouse poninter for text label */
  int		position;	/* Label positioning */
};

/*****************************************************************************
 *
 * Used to record probes that were used in simulation mode so they can be
 * reused when starting the simualtion again.  We keep track of the net so
 * that if we delete a net then we can remove probes associated with it.
 *
 *****************************************************************************/
typedef struct SavedProbe_str {
  char		*sp_name;	/* Full path name of probe */
  GNet		*sp_net;	/* Circuit net probe is based on */
} SavedProbe;

/*****************************************************************************
 *
 * Top-level circuit data
 *
 *****************************************************************************/
struct Circuit_str {
  CurrentFile	*currentFile;	/* Name of current file */
  char		*fileVersion;	/* Version of current file */
  char		*title;		/* Title of current circuit */

  Locale	*c_locale;	/* Locale of the file */
  char		*c_encoding;	/* Character encoding for save file. */
  Encoder	*c_saveEncoder;		/* Encoder used to save verilog data */
  Encoder	*c_loadEncoder;		/* Encoder used to load verilog data */
  Encoder	*c_displayEncoder;	/* Encoder used to used to display strings */
  Encoder	*c_psEncoder;		/* Encoder used to used to generate postscript */

  Timescale	c_timescale;	/* Circuit timescale */

  int		c_tvMode;	/* Timing violation mode (0=none, 1=after startup, 2=all) */
  double	c_startup;	/* Time required for initialization (suppress timing violations) */
  int		c_startupUnits;	/* Unit code for startup time */
  
  SimInterface	simulator;	/* top-level state for the simulator */
  NHash		*c_breakpoints;	/* Circuit breakpoints */ 
  NHash		*c_scripts;	/* Scripts active in simulator */
  List		*c_probes;	/* Saved list of nets with probes on them */

  GSearchContext *search;	/* The current search context */

  GModuleDef	*mid_mod;	/* Module interface descriptors */
  GModuleDef	*mid_altMod;	/* Module for alternate interface descriptions */
  GModuleDef	*mid_display;	/* Module for displaying a single interface */
  GModuleDef	*root_mod;	/* Top-level module */
  GSimModule	*root_ss;	/* Root simulation state */

  SHash		*moduleTable;	/* Table of modules */

  int		numInitScripts;	/* Number of simulation initialization scripts */
  char		**initScripts;	/* Simulation initialization scripts */

  char		*c_gatePrefix;	/* Prefix for tkgate primitives */

  int		discardChanges;	/* Are we discarding changes? */
  int		useExtBars;	/* Are we using extender bars? */
  int		showSwitchNets;	/* Show net names on switches and dips? */
  int		simAutoStart;	/* Auto-start of simulator */
  int		simClockMode;	/* 0=stop on all clocks, 1=stop on named clock */

  char		*simClockName;	/* Name of clock to stop on */

  int		no_set_modify;	/* Do not set 'modified' flag */
  unsigned	modified_flags;	/* Flags indicating what has changed and needs to be saved */

  int		c_isNewFile;	/* Is this a brand new never saved file? */

  /*****************************************************************************/
  /* These are fields that probably should be moved someplace else             */
  /*****************************************************************************/

  int		org_x,org_y;	/* Origin point for viewable */
  int		zoom_factor;	/* Zoom factor */
  EditState	*es;		/* Current editing context */
  GCutBuffer	*cut_buffer;	/* Buffer for cut/paste operations */
  GSelection	*mg_selection;	/* The current multi-gate selection */
  GCElement	*select;	/* Current single-gate selection */
  GCElement	*last;		/* Last selected gate */
  GWire		*wsel;		/* Selected wire */
  GWireNode	*wnsel;		/* Selected wire node */
  GNet		*nsel;		/* Selected net */
  GrabbedLabel	*labelsel;	/* Selected wire label */
  int rot;			/* Current rotation */
  int mode;			/* Current mode */
};

Circuit *new_Circuit();
void Circuit_setTitle(const char*);
void Circuit_setLocale(Circuit *c,Locale *);
void Circuit_setFileEncoding(Circuit *c,const char*);
void Circuit_setClockName(const char*);
void Circuit_setCurrentFile(const char*);
void Circuit_setCurrentFileVersion(const char*);
void Circuit_setScripts(int nScripts,const char **scripts);
void Circuit_setLibraries(int nLibraries,const char **libraries);
void Circuit_changeRoot(GModuleDef *M);
void Circuit_clear();
int Circuit_isSelection(Circuit *c);

void Circuit_loadLibrary(Circuit *c, const char *name);
void Circuit_unloadLibrary(Circuit *c, const char *name);
void Circuit_unloadAllLibraries(Circuit *c);

void Circuit_clearSavedProbes(Circuit *c);
void Circuit_addProbeName(Circuit *c,const char *name,GNet *net);
void Circuit_removeProbesOnNet(Circuit *c,GNet *net);
#define Circuit_getLoadFileEncoder(c) (c)->c_loadEncoder
#define Circuit_getSaveFileEncoder(c) (c)->c_saveEncoder
#define Circuit_getDisplayEncoder(c) (c)->c_displayEncoder
#define Circuit_getPSEncoder(c) (c)->c_psEncoder
void Circuit_invalidateHtml(Circuit *c);
void Circuit_initOptions(Circuit *c);

CurrentFile *new_CurrentFile();
int CurrentFile_set(CurrentFile *cf,const char *name);
#define CurrentFile_getBase(cf) (cf)->baseName
#define CurrentFile_getDir(cf) (cf)->dirName
#define CurrentFile_path(cf) (cf)->fullPathName

GrabbedLabel *new_GrabbedLabel();
void GrabbedLabel_draw(int x,int y);
void GrabbedLabel_unset();
void GrabbedLabel_set(GNet *net,int x,int y,int p);


void EditState_setMode(int mode);
void EditState_setRotation(int rot);
#define EditState_getMode() TkGate.circuit->mode
#define EditState_getRotation() TkGate.circuit->rot

void Timescale_save(Timescale *ts, FILE *f);
simtime_t Timescale_parse(int num,const char *units);
void Timescale_decode(unsigned long long n, int *num,char *units);
int Timescale_unitsToCode(const char *units);
const char *Timescale_codeToUnits(int code);

#endif
