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
#ifndef __vparser_h
#define __vparser_h
#include "expr.h"

#define TYPE_INT	0
#define TYPE_STR	1

void VerSetRootMod();
void VerEndGate();
void VerSetProperty(const char *prop,int n);
void VerSetStrProperty(const char *prop,const char *value);
void VerCheckVersion(const char*);
void VerCircuitProp(const char*,const void*,int);
void VerModuleProp(const char*,const void*,int);
void VerAddScript(const char*);
void VerAddVLib(const char*);
void VerNewModule(int,const char*,int);
void VerModParm(const char*);
void VerEndModule();
void VerNewNet(const char*,int,int);
void VerGate(const char *gtype,const char *instName);
void VerCell(const char *ctype,const char *instName);
void VerAttach(const char*,const char*,int);
void VerRevPad(int);
void VerMakeNode(int,int,int,int,int);
void VerEndDecls();
void VerMakeWire(int);
void VerSetPos(int,int);
void VerSetSize(int,int);
void VerSetRot(int);
void VerSetShowName(int);
void VerSetWireDecorationPos(int,int,int);
void VerPlaceWire(int);
void VerJointNet(const char*);
void VerBlockPort(const char *pname,int pdir,int widx);
void VerBeginBD();
void VerEndBD();
void VerBDPort(const char*,int,const char*,int,int,int);
void VerTranDup();
void VerTranRange(int,int);
void VerIconData(const char *data);
void VerSetIcon(const char *name,int N,int x,int y);
void VerSymPort(const char *ptype,const char *name,int nbits,int x,int y,int orient);
void VerSymbol(unsigned id);
void VerSetBlockSymbol(unsigned id);
void VerAddHdlLine(const char *text);
void VerRenameCurrent(const char *name);
void VerModHashParm(const char*,Expr*);
void VerCallMParmFlush();
void VerCallMParmAdd(const char *name,const char *value);
void VerAddScriptLine(const char *line);

void VerBeginDelayDef(const char *tech, const char *prim);
void VerEndDelayDef();
void PrimSet(const char*,const char*,Expr*);

int ycIsKW(char *Tok);

#endif
