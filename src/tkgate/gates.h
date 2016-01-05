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
    Declarations for Gate operations.
*/
#ifndef __gates_h
#define __gates_h

/*
 * Codes for i/o direction arrow types
 */
#define IODT_PORT	0	/* Plain arrows on block ports */
#define IODT_PLAIN	1	/* Plain arrows on concat */
#define IODT_BOXED	2	/* Boxed arrows on concat */

/*
 *   Code numbers for gate types which require special handling.
 */
#define GC_BLOCK 	0x1001
#define GC_JOINT	0x1002
#define GC_SWITCH	0x1003
#define GC_DIP		0x1004
#define GC_TAP		0x1005
#define GC_LOGICIN	0x1006
#define GC_LOGICOUT	0x1007
#define GC_LOGICTRI	0x1008
#define GC_REGISTER	0x1009
#define GC_COMMENT	0x100a
#define GC_LED		0x100b
#define GC_FLIPFLOP	0x100c
#define GC_SYMBLOCK	0x100d
#define GC_CLOCK	0x100e
#define GC_CONCAT	0x100f
#define GC_ROM		0x1010
#define GC_RAM		0x1011

/*
   Gate pad numbers for some of the gates.
*/
#define BLOCK_TIN 0
#define BLOCK_LIN 1
#define BLOCK_BIN 2
#define BLOCK_RIN 3
#define BLOCK_TOUT 4
#define BLOCK_LOUT 5
#define BLOCK_BOUT 6
#define BLOCK_ROUT 7
#define BLOCK_TTRI 8
#define BLOCK_LTRI 9
#define BLOCK_BTRI 10
#define BLOCK_RTRI 11

#define REGISTER_OUT 0
#define REGISTER_IN 1
#define REGISTER_EN 2
#define REGISTER_CLR 3
#define REGISTER_CK 4

#define FLIPFLOP_Q 	0
#define FLIPFLOP_NQ	1
#define FLIPFLOP_IN	2
#define FLIPFLOP_EN	3
#define FLIPFLOP_CLR	4
#define FLIPFLOP_CK	5

#define LOGICIN_OUT 0
#define LOGICOUT_IN 0

#define TRI_OUT 0

#define DIP_OUT 0

#define TAP_TAP 0
#define TAP_IN 1
#define TAP_OUT 2

#define CONCAT_IN 0
#define CONCAT_OUT 1



void gate_add(GModuleDef*,GCElement *);
void gate_remove(GModuleDef*,GCElement *);
GCElement *gate_new(int x,int y,int rot,int type);
int gate_delete(GCElement *g,GModuleDef *env,int drawp);
GCElement *gate_hit(GModuleDef*,int,int);
void gate_displayall(GModuleDef *mdef);
void gate_draw(GCElement *g,int md);
void gate_drawgatename(GCElement *g,const char*);
void gate_namegate(GCElement *g,EditState *es);
void gate_move(GCElement *g,int x,int y);
void gate_mark(GCElement *g,int select);
void gate_getbbx(GCElement *g,TargetDev_e,int *minx,int *miny,int *maxx,int *maxy);
void gate_doReplication(EditState *es);
void gate_repgate(EditState *es,GCElement *g,int x,int y);
void gate_setinvertwirelist(GWire *nw,GWire *w);
void gate_hashrepline(EditState *es,int x,int y);
GCElement *gate_lookup(const char *name,GModuleDef *mdef);
void gate_moveTo(GCElement *g,int x,int y);
void gate_setName(GCElement *g,const char *name,GModuleDef *mdef);
void gate_cutOffWires(GCElement *g,GModuleDef *M,int drawp);
void gate_transmute(GCElement *g,GGateInfo *ngi);
void gate_moveObject(GCElement *g,int dx,int dy);
void gate_drawWires(GCElement *g,int md);
void gate_unattachwirelist(GWire *w,GModuleDef *env,int drawp);

void block_setWireEnd(GCElement *g,GWire *w,int pad);
GWire *block_hitPort(GCElement *g,int x,int y);
int Block_HitComponent(GCElement *g,int x,int y);
int Block_HitEdge(GCElement *g,int x,int y);

void block_updateInterface(GCElement *g,GModuleDef *m);

int GGateInfo_getPadNum(GGateInfo *gi,const char *pname);

void gateinfo_iconInit(GGateInfo *gi,Pixmap P,iconDimensions *id,int boldOffset);
void gateinfo_1iconInit(GGateInfo *gi,Pixmap P,iconDimensions *id,int boldOffset);
void gateinfo_altIconInit(GGateInfo *gi,Pixmap P,iconDimensions *id,int boldOffset);

void GGateInfo_encodeTypeName(char *buf,GCElement *g);
int GGateInfo_decodeTypeName(const char *name,GCellSpec *gcs);

void RegisterGate(GGateInfo *gi);

void GetPinIOMark(GWire *w,int d,int iod,int atype,int *x,int *y,Icon **arrow);
void DrawPinIOMark(GWire *w,int d,int iod,int atype);


/*
 * Extra memory required for allocating a GCElement object with n ports.
 */
#define requiredPortMemory(n) (imax((n)-DEFAULT_PADMAX,0)*sizeof(GWire*))


#endif
