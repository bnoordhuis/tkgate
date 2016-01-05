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
    Generic functions for gates.
*/
#ifndef __generic_h
#define __generic_h

const char *seekOption(const char*,const char**,int);

GCElement *Generic_Make(EditState **es,GModuleDef *env,int GType,
			int x,int y,int r,const char *Name,int noWires,const char**,int);
void Generic_Init(GCElement*);
void Generic_Delete(GCElement*,GModuleDef*,int);
void Generic_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd);
int Generic_HitDistance(GCElement*g,int,int);
void Generic_Draw(GCElement *g,int md);
void Generic_Move(GCElement*,int,int);
GCElement *Generic_Copy(GModuleDef*,GCElement*,int,int,unsigned);
void Generic_AddInput(EditState*,GCElement*);
void Generic_AddOutput(EditState*,GCElement*);
void Generic_RemovePort(EditState *es,GCElement *g,GWire *w);
void Generic_PSWrite(GPrint*,GModLayout*,GCElement*);
int Generic_EditProps(GCElement*,int isStore);
void Generic_VerSave(FILE*,GCElement*);
void Generic_Rotate(GCElement *g, int x,int y,int rdir);
GWireNode *Generic_wireSnap(GCElement *g,GWire *w,int *mod,int retry);

void Generic_editPropsULBasics(GCElement *g);
void Generic_editPropsDLBasics(GCElement *g);
void Generic_editPropsULDelay(GCElement *g);
void Generic_editPropsDLDelay(GCElement *g);
void Generic_editPropsULWires(GCElement *g);
void Generic_editPropsDLWires(GCElement *g);

void Generic_DrawGateLabel(GPrint *P,GCElement *g,const char *text);
void Generic_PSLabels(GPrint *P,GCElement *g);
void Generic_WriteCellDef(FILE *f,GCellSpec *gcs);

void Err_AddInput(EditState*,GCElement*);
void Err_AddOutput(EditState*,GCElement*);
void Err_AddInOut(EditState*,GCElement*);
void Err_ChangePin(EditState*,GCElement*);
void Err_RemovePort(EditState *es,GCElement *g,GWire *w);

GCElement *Nop_Copy(EditState *es,GCElement *g,int x,int y,unsigned flags);
void Nop_SimInitFunc(EditState*,GCElement*,const char*);
int Nop_SimHitFunc(EditState*,GCElement*);
int Nop_VerSave(FILE*,GCElement*);
void Nop_WriteCellDef(FILE *f,GCellSpec *gcs);

GCElement *InOut_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char**,int);
void InOut_Draw(GCElement*,int);
void InOut_VerSave(FILE *f,GCElement *g);
void InOut_PSWrite(GPrint *P,GModLayout *L,GCElement *g);

#endif

