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
#ifndef __module_h
#define __module_h

#define MSP_STEPSIZE	8

/*
 * Port position types
 */
#define PPT_FIXED	0		/* Fixed port positions */
#define PPT_SCALED	1		/* Scaled port positions */

#define MT_NETLIST	0		/* Module is a netlist */
#define MT_TEXTHDL	1		/* Module is textual HDL */

/*****************************************************************************
 *
 * Simulation time in epochs.
 *
 *****************************************************************************/
typedef unsigned long long simtime_t;

/*****************************************************************************
 *
 * Module timescale
 *
 *****************************************************************************/
typedef struct {
  simtime_t	ts_units;
  simtime_t	ts_precision;
} Timescale;


/*****************************************************************************
 *
 * GModuleDef - Data structure for specifying a functional block definition.
 *
 *****************************************************************************/
typedef struct GModuleDef_str {
  char		*m_name;		/* Module name */
  char		*m_fileName;		/* Name of file in which module was defined */
  char		*m_libName;		/* Name of library (if applicable) */
  unsigned	m_type;			/* Module type */
  int		m_lineno;		/* Base line number for HDL modules */

  /*
   * Module interface data
   */
  unsigned	m_pptype;		/* Port position type */
  GCElement	*m_interface;		/* Module interface */
  GCElement	*m_altInterface;	/* Alternate interface (symbol if m_interface is block and vice versa) */

  /*
   * Declared module parameter ports and their default values
   */
  SHash		*m_parmPorts;		/* Table of parameter ports and default values */

  /*
   * Module property flags
   */
  unsigned	m_used : 1;		/* Mark to see if used in circuit */
  unsigned	m_isLib : 1;		/* This is a library module */
  unsigned	m_isModload : 1;	/* Has this module been changed since loading */
  unsigned	m_isModsave : 1;	/* Has this module been changed since last save */
  unsigned	m_protData : 1;		/* Module data is read-only */
  unsigned	m_protIntf : 1;		/* Module interface is read-only  */
  unsigned	m_protEdit : 1;		/* Module interface is protected in edit mode. */
  unsigned	m_hidden : 1;		/* Internal data is "hidden" (i.e., can not be opened) */
  unsigned	m_isSpecial : 1;	/* This is a "special" module (i.e., cut buffer, interface list, etc.) */

  /*
   * These members are used for netlist modules.
   */
  GWireList	*m_wires;		/* Wires in module */
  SHash		*m_gates;		/* Table of gates in module */
  SHash		*m_nets;		/* Table of nets in module */

  /*
   * These members are used for text RTL modules.
   */
  char		*m_text;		/* Textual module description */
  int		m_textSize;		/* Size of buffer allocated for m_text */
  int		m_curLine;		/* Current line number in text buffer */
  int		m_curChar;		/* Current char number in text buffer */
  int		m_needScan;		/* Module needs to be rescanned */
} GModuleDef;

int block_getPortDir(GCElement *g,GWire *w);
int block_getPad(GCElement *g,GWire *w);
void block_scalewirelist(GWire *w,GCElement *g,int isY,int portsFixed);
void block_movewirelist(GWire *w,int x,int y);
void block_name(GCElement *g,EditState *es);
void block_namefunction(GCElement *g,EditState *es);
int block_in(int x,int y,GCElement *g);
int block_setPortName(GCElement *g,GWire *w,EditState *es);
void block_getwirexy(GWire *w,int d,int *x,int *y,int *p);
void block_free(GCElement *g);
void block_explode(GCElement *g);
int block_connect(GCElement *g,GWireNode *n,int Dir);
GWire *block_addwire(GWire *w,GWire *wl);
void block_changefunction(EditState *es,GCElement *g);
void block_changename(EditState *es,GCElement *g);
void block_deletewire(GWire *w);
void block_newport(EditState *es,int iodir);
void block_changedir(GWireNode *n,EditState *es);
void block_setdir(GWireNode *n,EditState *es,int);
int block_attach(GModuleDef *env,GCElement *g,GWire *w1,GWire *w2,
	     int x,int y,const char *name,int Dir);
int block_edgehit(GCElement *g,int x,int y);
GCElement *block_replicate(GCElement *g,GModuleDef *M,int x,int y);
void block_canonicalizewire(GCElement *g,GWire *w);
int block_insidep(const char *n,EditState *es);
char *block_reportname(GCElement *g);
int block_isDataProtected(GCElement *g);
int block_isIntfProtected(GCElement *g);
int block_isFixedPort(GCElement *g);
void block_resetAllWireEnds(GCElement *g);
void block_cutoffwire(GWire *w,EditState *es);

void modint_flush();
void modint_reset(EditState *es,GCElement *g);
int modint_setInterface(GModuleDef *M,GCElement *g);
void modint_arrange(EditState *es);
void modint_update(EditState *es);
void modint_edit(EditState **es,GModuleDef *M);
GCElement *modint_findDisplay(const char*);
GCElement *modint_find(const char *name);
GCElement *modint_findInAlt(const char *name);
void modint_deleteInterface(GModuleDef *);
void modint_renameInterface(GModuleDef *);
void modint_syncDisplay(GModuleDef*);
void modint_makeSymbolInterface(GModuleDef *M,GModSymbol *ms);
void modint_openSelected(const char *modName);
void modint_close();
void modint_center();

GModuleDef *new_GModuleDef(const char *name);
void delete_GModuleDef(GModuleDef *M);

GNet *GModuleDef_findNet(GModuleDef *M,const char *name);
GCElement *GModuleDef_findGate(GModuleDef *M,const char *name);
void GModuleDef_getBBX(GModuleDef *M,TargetDev_e target, int *minX,int *maxX,int *minY,int *maxY);
void GModuleDef_copyInto(GModuleDef *D,GModuleDef *S,int dx,int dy,int selOnly,int selDst);
void GModuleDef_copy(GModuleDef *D,GModuleDef *S);
void GModuleDef_setLibrary(GModuleDef *M,const char *name);
void GModuleDef_setFileName(GModuleDef *M,const char *name);
void GModuleDef_claim(GModuleDef *M);
int GModuleDef_isDataProtected(GModuleDef *M);
int GModuleDef_isIntfProtected(GModuleDef *M);
void GModuleDef_flush(GModuleDef *M);
#define GModuleDef_getName(M) ((M)->m_name)
#define GModuleDef_getType(M) ((M)->m_type)
#define GModuleDef_isLibrary(M) ((M)->m_isLib)
#define GModuleDef_isTop(M) ((M) == TkGate.circuit->root_mod || (M) == TkGate.circuit->mid_mod)
int GModuleDef_hasSubModules(GModuleDef *M);
GModuleDef *GModuleDef_isRecursive(GModuleDef *M);
void GModuleDef_getPropsToTcl(GModuleDef *M, Tcl_Interp *tcl,char *aname);
void GModuleDef_setPropsFromTcl(GModuleDef *M, Tcl_Interp *tcl,char *aname);
int GModuleDef_scanHDLModule(GModuleDef *M);
void GModuleDef_saveText(GModuleDef *M,const char *text);
void GModuleDef_allocText(GModuleDef *M,int n);
int GModuleDef_numHdlLines(GModuleDef *M);
void GModuleDef_listNets(GModuleDef *mdef);
void GModuleDef_listPorts(GModuleDef *mdef);
GNet *GModuleDef_labelHit(GModuleDef *m,int x,int y);
int GModuleDef_grabLabel(GModuleDef *m,int x,int y,GrabbedLabel *gl);

GCElement *SymBlock_convert(GCElement *g,GModSymbol *ms,GModuleDef *M,int doWires);
void SymBlock_remakePorts(GCElement *g,GModuleDef *M);

void Block_EditParmProps(GCElement *g, int isLoadDialog);
void Block_VerSaveModParms(FILE *f,GCElement *g);
void Block_setParmPort(GCElement *g,const char *parmName,const char *newValue);
void Block_unsetParmPort(GCElement *g,const char *parmName);
const char *Block_getParmPort(GCElement *g,const char *name);

#endif
