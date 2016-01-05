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
#include "tkgate.h"

static GCElement *Script_Make(EditState **es,GModuleDef *env,int GType,
			      int x,int y,int r,const char *Name,int noWires,
			      const char **options,int nOptions);
static int Script_EditProps(GCElement *g,int isLoadDialog);
static void Script_VerSave(FILE *f,GCElement *g);
static void Script_SetProp(GCElement *g,const char *prop,const void *value);
static GCElement *Script_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags);
static void Script_Delete(GCElement *g,GModuleDef *M,int drawp);


static iconDimensions script_iconDims[] = {
  {0,  0, 31, 31, 15, 15},
  {32, 0, 31, 31, 15, 15},
  {32, 32, 31, 31, 15, 15},
  {0,  32, 31, 31, 15, 15},
};
static int script_iconBoldOffset = 64;

static char *psScript[] = {
  "%",
  "% A Script",
  "/script {",
  "  [[0 0][0 0][0 0][0 0]] adjstartgate",
  "  -15.5 -15.5 moveto",
  "  -15.5 15.5 lineto",
  "  15.5 15.5 lineto",
  "  15.5 -15.5 lineto",
  "  closepath stroke",
  "  grestore",
  "} def",
  0
};

GGateInfo gate_script_info = {
  0,
  "SCRIPT",
  "script",0x0,
  "script",psScript,
  -1,0,

  {{"V",	{0,0},		{"gm.script",0,0},	"gat_make SCRIPT"},
   {0}},


  script_iconDims,

  0,{0},
  {{0,-19,CT},{0,-19,CT},{0,-19,CT},{0,-19,CT}},
  {1},

  {0},
  
  Script_Make,
  Nop_WriteCellDef,
  Generic_Init,
  Script_Delete,
  Generic_GetExtents,
  Generic_HitDistance,
  Generic_Draw,
  Generic_Move,
  Script_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Generic_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Generic_PSWrite,
  Script_EditProps,
  Script_VerSave,
  Script_SetProp
};

static GCElement *Script_Make(EditState **es,GModuleDef *env,int GType,
			      int x,int y,int r,const char *Name,int noWires,const char **options,int nOptions)
{
  GCElement *g;
  char buf[STRMAX];

  if (!Name) {
    Name = buf;
    sprintf(buf,"script1");
  }

  g = Generic_Make(es,env,GType,x,y,r,Name,noWires,options,nOptions);
  ob_touch(g);
  g->show_name = 1;

  return g;
}

static GCElement *Script_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng = Script_Make(&TkGate.circuit->es, M, g->typeinfo->Code, x, y, g->orient, g->ename, 0,0,0);
  TextLine *L;

  ng->u.comment.first = ng->u.comment.last = 0;
  for (L = g->u.comment.first;L;L = L->next) {
    Comment_addLine(ng,L->text);
  }
  

  return ng;
}

void Script_Delete(GCElement *g,GModuleDef *M,int drawp)
{
  if (M)     gate_remove(M,g);
  if (drawp) gate_draw(g,0);

  Comment_flushLines(g);
}


static void Script_SetProp(GCElement *g,const char *prop,const void *value)
{
  ob_touch(g);

  if (strcmp(prop,"/line") == 0) {
    Comment_addLine(g,value);
  }
}

static int Script_EditProps(GCElement *g,int isLoadDialog)
{
  char buf[STRMAX],pos[64];

  Generic_EditProps(g,isLoadDialog);

  if (isLoadDialog) {
    TextLine *L;
    int n = 0;

    for (L = g->u.comment.first;L;L = L->next) {
      sprintf(pos,"%d",n++);
      Tcl_SetVar2(TkGate.tcl,"edgat_commentLines",pos,L->text,TCL_GLOBAL_ONLY);
    }

    sprintf(buf,"%d",n);
    Tcl_SetVar(TkGate.tcl,"edgat_commentLen",buf,TCL_GLOBAL_ONLY);
  } else {
    const char *p;
    int n = 0;
    int i;
    Tcl_Interp *tcl = TkGate.tcl;

    Comment_flushLines(g);

    if ((p = Tcl_GetVar(tcl,"edgat_commentLen",TCL_GLOBAL_ONLY)))
      sscanf(p,"%d",&n);
    for (i = 0;i < n;i++) {
      sprintf(buf,"%d",i);
      p = Tcl_GetVar2(tcl,"edgat_commentLines",buf,TCL_GLOBAL_ONLY);
      if (p) Comment_addLine(g,p);
    }
   }
  return 0;
}

/*****************************************************************************
 *
 * Generate primitive cell definition for scripts.
 *
 * Parameters:
 *    f			File to write cell to.
 *    name		Name of cell to write.
 *
 *****************************************************************************/
static void Script_VerSave(FILE *f,GCElement *g)
{
  TextLine *L;
  void *encoder = Circuit_getSaveFileEncoder(TkGate.circuit);

  fprintf(f,"//: beginscript %s",g->ename);
  VerilogBasicGateComment(f,g,0);
  fprintf(f,"\n");

  for (L = g->u.comment.first;L;L = L->next) {
    char buf[STRMAX];
    recodeText(encoder,buf,STRMAX,L->text);
    fprintf(f,"%s\n",buf);
  }

  fprintf(f,"//: endscript\n");
}

void init_script()
{
  Pixmap P;

  P = Pixmap_registerFromFile("script","script.b");
  gateinfo_iconInit(&gate_script_info,P,script_iconDims,script_iconBoldOffset);
  RegisterGate(&gate_script_info);
}
