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
#ifndef __simulate_h
#define __simulate_h
/*
    Simulator dependant	state.	Contains information on	the
    positions of switches and probes.  Also used to generate
    unique names for circuit elements.
*/


#define UNITS_S		0
#define UNITS_MS	1
#define UNITS_US	2
#define UNITS_NS	3
#define UNITS_PS	4
#define UNITS_FS	5
#define NUM_UNITS	6


/*****************************************************************************
 *
 * Simulator probe 
 *
 *****************************************************************************/
typedef struct {
  char 		*name;		/* Name of probe */
  GNet		*net;		/* Net of probe */
  int 		x,y;		/* Position */
  GSimModule	*ss;		/* Simmodule this belongs to */
} GSimProbe;

/*****************************************************************************
 *
 * Simulator switch 
 *
 *****************************************************************************/
typedef struct {
  char		*name;		/* Full name of wire */
  char		*gname;		/* Full name of gate */
  GCElement	*gate;		/* switch gate */
  unsigned	*state;		/* State of switch */
} GSimSwitch;

/*****************************************************************************
 *
 * Simulator LED (device that changes with simulation state) 
 *
 *****************************************************************************/
typedef struct {
  char		*name;		/* Full name of wire */
  char		*gname;		/* Full name of gate */
  GCElement	*gate;		/* led gate */
} GSimLed;

/*****************************************************************************
 *
 * Simulator Module - A simulator module is generated for each instance of a
 * module in the circuit hierarchy.
 *
 *****************************************************************************/
struct simmodule {
  GCElement	*inst;		/* Module instance in parent */
  GModuleDef	*mod;		/* Module this corresponds to */

  SHash		*probes;	/* Probes in this module instance */
  SHash		*switches;	/* Switches in this module instance */
  SHash		*leds;		/* Leds in this module instance */
  SHash		*children;	/* Child module instance */

  GSimModule	*parent;	/* Parent module instance */
};

/*****************************************************************************
 *
 * Simulator interface
 *
 *****************************************************************************/
typedef struct SimInterface_str {
  int		active;			/* Flag indicating running simulator */
  char		simFileName[STRMAX];	/* Temp file with circuit description */
  GSimModule	*sim_root;		/* Root simulator module */

  int		no_scope;		/* Non-zero if no scope should be created */ 

  int		area;			/* Area reported for circuit */
  int		staticPower;		/* Static power reported for circuit */

  int		si_tsmult;		/* Timescale multiplier */
  int		si_units;		/* Timescale units */
  simtime_t	si_precision;		/* Precision is smaller than units */
} SimInterface;

const char* SimInterface_unitsToStr(int);
int SimInterface_strToUnits(const char*);

void SimInterface_init(SimInterface*);
void SimInterface_begin(SimInterface*);
void SimInterface_end(SimInterface*);
void SimInterface_drawProbes(SimInterface*);
void SimInterface_hit(SimInterface*,int x,int y,int isDoubleClick);
void SimInterface_hitRelease(SimInterface*);
void SimInterface_addDelProbe(SimInterface *si,GSimModule *sM,const char *name,GWire *w,GWireNode *n,int x,int y);
void SimInterface_addDelHDLProbe(SimInterface *si,GSimModule *sM,const char *name,GNet *net);
int SimInterface_probeExists(SimInterface *si,GSimModule *sM,const char *name);
void SimInterface_send(SimInterface*,const char*,...);
int SimInterface_lookupGate(SimInterface*,const char*,GSimModule **M,GCElement **g,GSimSwitch **ss);
int SimInterface_lookupWire(SimInterface*,const char*,GSimModule **M,GWire **w,GNet **n);
int SimInterface_command(SimInterface *si,const char *C);
void SimInterface_changeCurrentModule(GSimModule *new_sm,GSimModule *old_sm);
void SimInterface_gateError(SimInterface *si,const char *gname,const char *msg);
void SimInterface_fileError(SimInterface *si,const char *msg);
void SimInterface_wireError(SimInterface *si,const char *wname,const char *msg);
void SimInterface_navigateToModule(EditState **es,const char *path);
void SimInterface_updateNetlistProbes(GSimModule *SM);
char *SimInterface_formatTime(SimInterface *si, char *buf,simtime_t t); 

void GSimModule_getNetPathName(GSimModule *M,GNet *n,char *buf);
void GSimModule_getFullPath(GSimModule *M,GCElement *g,char *buf);
char *GSimModule_getPathPrefix(GSimModule *M,char *buf);

GNet *sim_findNet(const char *name);
GCElement *sim_findGate(const char *name);
GModuleDef *sim_findContainingMod(const char *path);


int breakpoint_check(const char *s);

void getSimTempFile(char*);

void sendSimCmd(char *fmt,...);

#endif

