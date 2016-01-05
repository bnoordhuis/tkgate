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
/*
    Declarations for circuit representation structures.
*/
#ifndef __elements_h
#define __elements_h

/*****************************************************************************
 * Net types used to mark nets in HDL modules.
 *****************************************************************************/
#define NT_NONE		0
#define NT_INPUT	1
#define NT_OUTPUT	2
#define NT_INOUT	3
#define NT_WIRE		4
#define NT_REG		5

/*****************************************************************************
 * Net types used to mark nets in netlist modules.
 *****************************************************************************/
#define NV_WIRE		0
#define NV_TRI		1
#define NV_WAND		2
#define NV_TRIAND	3
#define NV_WOR		4
#define NV_TRIOR	5
#define NV_TRI0		6
#define NV_TRI1		7
#define NV_TRIREG	8

/*
 * GCellSpec - Decoded built-in gate cell 
 */
struct GCellSpec_str {
  char		gc_name[STRMAX];	/* Name of cell */
  GGateInfo    *gc_info;		/* Gate info pointer */
  char		gc_invSpec[STRMAX];	/* Inverter specification */
  int		gc_multiPad;		/* Multi-pad count */
  int		gc_numBits;		/* Bit width of cell */
#if 0
  int		gc_numDelays;		/* Number of delay values */
  int		gc_delays[MAXDELAYS];	/* Delay value for cell */
#endif
  int		gc_numParms;		/* Number of parameters */
  int		gc_parms[MAXPARMS];	/* Parameters values */
};

struct wirelist {
  GWire		*wl_wire;	/* Wire in the wirelist */
  GWireList	*wl_next;	/* Next in the wirelist */
};

typedef struct {
  int driver;			/* Which joint is driver */
  int gidx;			/* Index of joint (for reading circuits) */
} GGateJoint;

typedef struct {
  char *memfile;		/* File Containing memory definition */
} GGateMem;

typedef struct {
  char *moduleName;		/* Function definition of block */
  int gwidth,gheight;		/* Block dimensions */
  GModSymbol *symbol;		/* Symbol for this element */
  int numModParms;		/* Number of module parameters */
  char **modParms;		/* Module parameters (not set to default) */
} GGateBlock;

typedef struct {
  short omega,phi,duty;
} GGateClock;

typedef struct {
  unsigned perm_dipval;		/* Permanent dipval */
  unsigned dipval;		/* Current value */
  unsigned showNet;		/* Show net name with gate */
} GGateDip;

typedef struct {
  unsigned ltype;		/* Type of LED */
  unsigned value;		/* Current led value */
  unsigned valid;		/* Valid bits in value */
} GGateLed;

typedef struct {
  int spliceside;		/* Specifies which side of the wire the splice it on */
  int msb,lsb;			/* Range of bits selected by splice */
  int gidx;			/* Index of splice (for reading circuits) */
} GGateTap;

typedef struct {
  int portDir;			/* Specifies direction wires are layed out */
  int drivePort;		/* The port designated as the signal driver (0 or 1)*/
  int type;			/* Type code (AUTO, FIXED, TRAN) */
  int newDrivePort;		/* Used for AUTO updating */
} GGateConcat;

typedef struct {
  int extbar;			/* Use extender bar for multi-input basic gates */
} GGateBasic;

typedef struct {
  int		width,height;	/* Size of the comment block in pixels */
  int		reqWidth;	/* Requested width (html only) */ 
  TextLine	*first;		/* First line in comment */
  TextLine	*last;		/* Last line in comment */
  Html		*html;		/* Formatted html */

  /*
   * These are used for converting old-style comments.
   */
  int		doLink;		/* Old-style link flag */
  char		*link;		/* Old-style link text */
} GGateComment;

typedef struct {
  int		width,height;	/* Size of the frame */
  char		*text;		/* Text of the frame */
  Html		*html;		/* Formatted html */
} GGateFrame;

typedef struct {
  int		select_side;	/* Side that selector is on (0 means left in 0 orientation) */
  int		data_order;	/* Order of mux data lines (0 means increasing to right in 0 orientation) */
} GGateMux;

typedef struct {
  int		mirror;		/* Mirror image of ports */
} GGateFF;

/*
 * This is the data structure for a circuit element (GCElement defined above).  All
 * instance of gates, modules, etc. are defined by this data structure.  This data
 * structure is a sort of a poor man's C++ class with virtual functions.  The "typeinfo"
 * member points to a structure that represents the derived class (AND, OR, MUX, etc.).
 * It containes some type specific data as well as a bunch of function pointers that
 * can be thought of as virtual methods.  There is also a "union" which contains gate
 * type specific data.  For example u.clock contains data that is specific to clock gates.
 */
struct celemnt {
  GGateInfo *typeinfo;		/* Type information of gate */
  char *ename;			/* Element Name */
  int enumb;			/* Element number */
  int xpos,ypos,orient;		/* Position of element */
  char *tech;			/* Technology of gate */
  int *delays;			/* Delay values for the gate */

  unsigned selected:1;		/* Gate selected */
  unsigned top:1;		/* Top edge selected */
  unsigned bottom:1;		/* Bottom edge selected */
  unsigned right:1;		/* Right edge selected */
  unsigned left:1;		/* Left edge selected */
  unsigned mark:1;		/* Generic mark for gates */
  unsigned show_name:1;		/* Names should be displayed */
  unsigned anchored:1;		/* Gate position is fixed */
  unsigned cpath_cut:1;		/* Gate is a critical path cut-point */
  unsigned cust_delay:1;	/* Use custom delay values */
  unsigned isModuleInterface:1;	/* This intance is a module interface */

  union {
    GGateClock		clock;
    GGateDip		sw;
    GGateLed		led;
    GGateTap		tap;
    GGateJoint		joint;
    GGateMem		mem;
    GGateBlock		block;
    GGateConcat		cat;
    GGateBasic		basic;
    GGateFrame		frame;
    GGateComment	comment;
    GGateMux		mux;
    GGateFF		ff;
  } u;

  /* 
   * Vector of wire pads (allocated in gate_new).  This array can be extended
   * for some symbol modules by doing a realloc.
   */
  GWire *wires[DEFAULT_PADMAX];
};

/*
 * Location information for a gate "pad"
 */
typedef struct locate {
  int x1,y1;			/* Starting position of pad */
  int x2,y2;			/* Ending position of pad */
  int dir;			/* Direction of pad */
} GPadLoc;

/*
   Used for gateinfo on wires.
*/
typedef struct padinfo {
  char *Name;			/* Pin name for wires on this pad */
  int iotype;			/* I/O type (IN, OUT, TRI) */
  int Size;			/* Default bit-size */
  int Num;			/* Initial number of pins */
  GPadLoc *Loc;			/* Location info for pins */
  unsigned CanAdd : 1;		/* Can add to pin list */
} GPadInfo;

/*
  Used to specify the position of a gate name label.
 */
typedef struct {
  int x,y;			/* Position of label */
  int just;			/* Justification mode for text */
} GLabelPos;

typedef struct gKeyMenuEnt {
  char *key_seq;		/* Key sequence */
  struct {
    char *name;			/* Root menu entry to use */
    int ul;			/* Underline position */
  } root;
  struct {
    char *name;			/* Entry to use */
    int ul;			/* Underline position */
    char *gtag;			/* Grouping tag */
    int order;			/* Ordering within group */
  } entry;
  char *command;		/* Command to execute */
  short accel_label_only;	/* Use key_seq only as menu accelerator label */
} GKeyMenuEnt;

/*
 * GGateInfo - Gate type definition
 *
 * This data structure is used to describe a gate type.
 *
 */
struct gateinfo {
  int		Code;					/* Code number for gate */
  char		*name;					/* Name of this gate type */
  char		*vnames;				/* Verilog names */
  unsigned	vmask;					/* Verilog name mutating ports */
  char		*psprint;				/* Postscript display function for gate */
  char		**psdef;				/* Postscript definition for gate */

  int		gi_multiPad;				/* Pad used to generate number of ports of gate */
  int		gi_bitPad;				/* Pad used to generate bit size of gate */

  GKeyMenuEnt cmds[MAXKBINDS];				/* Menu and keyboard shortcuts */

  iconDimensions *dim;					/* Size of gate */

  int NumPads;						/* Number of pin types */
  GPadInfo Pad[DEFAULT_PADMAX];				/* Pad type descriptions */

  GLabelPos lpos[NUMORIENTS];				/* Positions of gate name labels */

  struct {
    unsigned CanRot:1;					/* Do rotations exist */
    unsigned NoInvert:1;				/* Can't put inverters on it */
    unsigned IsIOnet:1;					/* Is this an I/O net (port, ground, or vdd) */
    unsigned WireProps:1;				/* Is gate display affected by wire property changes? */
    unsigned special_move : 1;				/* Special movement method */
    unsigned single_icon : 1;				/* set if this is a single icon gate  */
  } Flags;

  char		*delayNames[MAXDELAYS];			/* Name of delay parameters */
  
  /* Function for gate creation */
  GCElement *(*MakeFunction)(EditState**,GModuleDef*,
			     int,int,int,int,const char*,int,const char**,int);
  void (*WriteCellDef)(FILE *,GCellSpec*);		/* Write a cell definition */
  void (*InitGate)(GCElement*);				/* Initialize new gate */
  void (*DeleteGate)(GCElement*,GModuleDef*,int);	/* Delete gate function */
  void (*GetExtents)(GCElement *g
		     ,TargetDev_e target
		     ,int *minx,int *miny
		     ,int *maxx,int *maxy,int *bd);	/* Get bounding box of gate */
  int (*HitDistance)(GCElement*,int,int);		/* Distance to gate from (x,y) */
  void (*DrawGate)(GCElement*,int);			/* Draw function */
  void (*MoveGate)(GCElement*,int,int);			/* Move function */
  GCElement *(*CopyGate)(GModuleDef*,GCElement*
			      ,int,int,unsigned);	/* Copy a gate */
  void (*AddInput)(EditState*,GCElement*);		/* Add Input to a gate */
  void (*AddOutput)(EditState*,GCElement*);		/* Add Output to a gate */
  void (*AddTri)(EditState*,GCElement*);		/* Add Tri-State pin to a gate */
  void (*Rotate)(GCElement*,int,int,int);		/* Rotate a gate and fix wires */
  void (*RemovePort)(EditState*,GCElement*,GWire*);	/* Remove port from a gate */
  void (*ChangePin)(EditState*,GCElement*);		/* Change direction of pin */
  void (*SimInitFunc)(EditState*,GCElement*,const char*); /* For simulator initialization */
  int (*SimHitFunc)(EditState*,GCElement*);		/* Handles sim-mode mouse hit on gate */
  void (*PSWrite)(GPrint*,GModLayout*,GCElement*);	/* Write PostScript for gate */
  int (*EditProps)(GCElement*,int); 			/* Edit gate properties (new style) */
  void (*VerSave)(FILE*,GCElement*); 			/* Save in verilog format */
  void (*SetProp)(GCElement*,const char*,const void*);	/* Set property */
  void (*VersionDelta)(GCElement*,Version*);		/* Update from old version */
  GWireNode *(*WireSnap)(GCElement*,GWire*,int *,int);	/* Snap wires attached to a gate */

  /*********** members after here are initilized at run-time ***********/
  Icon		*icon[2*NUMORIENTS];			/* Icons for each orientation */
  GDelayDef	*delay_defs;				/* Delay definitions for this primitive */
  int		num_delays;				/* Number of elements in delayNames[] */
  Icon		*altIcon[2*NUMORIENTS];			/* Icons for each orientation */
};

int GCElement_numPads(GCElement *g);
GPadLoc *GCElement_getPadLoc(GCElement *g,int p,int r);
int GCElement_getPadDir(GCElement *g,int p);
const char *GCElement_getPadName(GCElement *g,int p);
#define GCElement_getPadNum(g,p) (g)->typeinfo->Pad[(p)].Num
int GCElement_getPadCanAdd(GCElement *g,int p);
#define GCElement_getType(g) (g)->typeinfo->Code
#define GCElement_isModule(g) ((g)->typeinfo->Code == GC_BLOCK || (g)->typeinfo->Code == GC_SYMBLOCK)
void GCElement_setSymbol(GCElement *g, GModSymbol *s);
#define GCElement_getSymbol(g) ((g)->u.block.symbol)
GWire *GCElement_getPort(GCElement *g, const char *portName);

GGateInfo *GGateInfo_lookup(const char*);
GGateInfo *GGateInfo_vlookup(const char*);
int GGateInfo_variantNum(GGateInfo *gi,const char *func);
GGateInfo *GGateInfo_codeLookup();

/*
 * Public comment methods.
 */
const char *Comment_getHyperlink(GCElement *g,int x,int y);

#endif
