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

    Last edit by hansen on Thu Apr 13 17:59:12 2000
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <pwd.h>
#include "hash.h"
#include "parser.h"
#include "grammar.h"
#include "gmac.h"

extern Code *mcode;
extern Code *ucode;
extern Code *map;

int ycLineNumber;
char ycFileName[1024];
int yyErrorCount = 0;

#ifndef yyrestart
void yyrestart(FILE*);
#endif

/*
  Current objects (used by parser)
 */
struct {
  MemProg	*mb;			/* The current memory program */
  UField	*field;			/* The most recently defined field */ 
  SHash		*eset;			/* The current enum set */
  OpDef		*opdef;			/* The current operator */
  OprDef	*oprdef;		/* The current operand */
  OprGroup	*ogdef;			/* The current operand group */
  int		dtype;			/* Current data type for .short, .byte, etc.*/
  int		noprs;			/* Number of operands */
  int		opr_type[MAXOPRS];	/* Operand types */ 
  int		opr_regs[MAXOPRS];	/* Operand registers */ 
  Number	*opr_nums[MAXOPRS];	/* Operand numbers */ 
  char		*proc_name;		/* Current procedure name */
} cur;

int yywrap()
{
  return 1;
}

int yyerror(char *s,...)
{
  va_list ap;

  va_start(ap,s);

  fprintf(stderr,"<\"%s\",%d> ",ycFileName,ycLineNumber);
  vfprintf(stderr,s,ap);
  fprintf(stderr,"\n");

  va_end(ap);

  yyErrorCount++;

  return 0;
}


void ycBank(int w,struct range *br,char *name,struct range *ar)
{
  Bank *B;

  if (br->msb < br->lsb) {
    yyerror("Msb can not be less than lsb in bank declaration.");
    return;
  }

  if ((br->msb-br->lsb+1) > 32) {
    yyerror("Banks can not be more than 32 bits.");
    return;
  }

  B = new_Bank(name,br->msb,br->lsb,ar ? ar->msb : -1,ar ? ar->lsb : -1);

  switch (w) {
  case MICROCODE :
    Code_addBank(ucode,B);
    break;
  case MACROCODE :
    Code_addBank(mcode,B);
    break;
  case MAP :
    Code_addBank(map,B);
    break;
  }
}

void ycField(char *name,struct range *r,int iscomp)
{
  if (iscomp && r->msb != r->lsb) {
    yyerror("Compliment operator not allowed on multi-bit fields.");
    return;
  }

  cur.field = new_UField(name,r->msb,r->lsb,iscomp);
  Code_addField(ucode,cur.field);
  cur.eset = &cur.field->uf_codes;
}

void ycEnum(char *name,int value)
{
  int *n = new(int);
  *n = value;

  SHash_insert(cur.eset,name,n);
}

void ycBeginUCode(int addr)
{
  cur.mb = new_MemProg(Code_nbits(ucode),addr);

  Code_addMemProg(ucode,cur.mb);
}

void ycEndUCode()
{
}

void ycULabel(char *name)
{
  if (Code_findLabel(ucode,name)) {
    yyerror("Redefinition of target '%s'.",name);
    return;
  }
  Code_addLabel(ucode,name,MemProg_current(cur.mb));
}

void ycUNext()
{
  MemProg_incAddr(cur.mb);
}

void ycUSpec(struct number *n1,struct number *n2)
{
  UField *f = 0;
  int dmsb,dlsb;
  MemInst *mi;

  switch (n1->ntype) {
  case NT_VALUE:
    dmsb = dlsb = n1->v.d;
    break;
  case NT_SYMBOL :
    f = Code_findField(ucode,n1->v.s);
    if (!f) {
      yyerror("Undefined field '%s' in micro-instruction.",n1->v.s);
      return;
    }
    dmsb = UField_msb(f);
    dlsb = UField_lsb(f);
    break;
  default:
    yyerror("Unexpected relative symbol in micro-instruction.");
    return;
  }

  if (dmsb < dlsb) {
    int x = dmsb;
    dmsb = dlsb;
    dlsb = x;
  }


  mi = new_MemInst(MemProg_current(cur.mb),dmsb,dlsb);
  MemProg_addInst(cur.mb,mi);

  if (n2) {
    switch (n2->ntype) {
    case NT_VALUE:
      MemInst_setNumValue(mi,n2->v.d);
      break;
    case NT_SYMBOL :
      {
	int *n = 0;
	if (f)
	  n = UField_findEnum(f,n2->v.s);
	if (!n)
	  n = Code_findLabel(ucode,n2->v.s);
	if (n) {
	  MemInst_setNumValue(mi,*n);
	} else {
	  MemInst_setSymValue(mi,n2->v.s,0,dmsb-dlsb,0);
	}
      }
      break;
    default:
      yyerror("Unexpected relative symbol in micro-instruction.");
      return;
    }
  } else
    MemInst_setNumValue(mi,1);
}

void ycMap(struct number *val,struct number *addr)
{
  MemProg *mb;
  MemInst *mi;

  if (Code_numMBlocks(map) == 0) {
    mb = new_MemProg(Code_nbits(ucode),0);
    Code_addMemProg(map,mb);
  } else
    mb = Code_getMBlock(map,0);

  if (addr->ntype  != NT_VALUE) {
    yyerror("Symbolic value not allowed for map address.");
    return;
  }

  mi = new_MemInst(addr->v.d,31,0);
  MemProg_addInst(mb,mi);

  if (val->ntype == NT_VALUE)
    MemInst_setNumValue(mi,val->v.d);
  else
    MemInst_setSymValue(mi,val->v.s,0,31,0);
}

void ycBeginOp(char *name)
{
  if (SHash_find(&mcode->c_ops,name)) {
    yyerror("Redefinition of operator '%s'.",name);
    return;
  }

  cur.opdef = new_OpDef(name,Code_nbits(mcode));
  SHash_insert(&mcode->c_ops,name,cur.opdef);
}

void ycEndOp()
{
  /*  OpDef_print(cur.opdef,stdout);*/
  cur.opdef = 0;
}

void ycAddOprGroup(char *name)
{
  OprGroup *og = (OprGroup *) SHash_find(&mcode->c_oprs,name);
  if (!name) {
    yyerror("Undefined operand group '%s'\n",name);
    return;
  }
  OpDef_addGroup(cur.opdef,og);
}

void ycBeginOprGroup(char *name)
{
  if (name) {
    if (SHash_find(&mcode->c_oprs,name)) {
      yyerror("Redefinition of operator '%s'.",name);
      return;
    }
  }

  cur.ogdef = new_OprGroup(name ? name : "*local*",Code_nbits(mcode));
  cur.oprdef = 0;

  if (name && !cur.opdef)
    SHash_insert(&mcode->c_oprs,name,cur.ogdef);

  if (cur.opdef)
    OpDef_addGroup(cur.opdef,cur.ogdef);
}

void ycEndOprGroup()
{
  cur.ogdef = 0;
  cur.oprdef = 0;
}

void ycBeginOperand()
{
  cur.oprdef = OprGroup_addOprDef(cur.ogdef);
}

void ycEndOperand()
{
  int r = OprDef_check(cur.oprdef);

  switch (r) {
  case GE_IDRANGE :
    yyerror("Id number(s) in operand declaration out of range.");
    break;
  case GE_REDECID :
    yyerror("Id numbers in operand declaration must be unique.");
    break;
  case GE_UNDEFID :
    yyerror("Undefined Id number used in operand declaration.");
    break;
  case GE_BADTYPE :
    yyerror("Type mismatch in operand declaration.");
    break;
  case GE_OK :
    break;
  default :
    yyerror("Bogonic internal code %d in ycEndOperand.",r);
    break;
  }

  cur.oprdef = 0;
}

void ycOLHS(int type,int n1,int n2)
{
  OprDef_addOpr(cur.oprdef,type,n1,n2);
}

void ycORHS(int type,int bnum,struct range *br,int vnum,struct range *vr,int offset)
{
  OpInst *oi;

  oi = new_OpInst(type,bnum,br->msb,br->lsb,vnum,vr ? vr->msb : -1,vr ? vr->lsb : -1,offset);

  if (cur.oprdef)
    OprDef_addOpInst(cur.oprdef,oi);
  else
    OpDef_addOpInst(cur.opdef,oi);
}

void ycMLabel(char *name)
{
  char *ename = 0;
  char buf[1024];

  if (cur.proc_name) {

    sprintf(buf,"%s.%s",cur.proc_name,name);
    ename = name;
    name = buf;
  } else
    ename = name;

  if (Code_findLabel(mcode,name)) {
    yyerror("Redefinition of target '%s'.",ename);
    return;
  }
  Code_addLabel(mcode,name,MemProg_current(cur.mb));
}

void ycMSymbol(char *name,int value)
{
  char *ename = 0;
  char buf[1024];

  if (cur.proc_name) {

    sprintf(buf,"%s.%s",cur.proc_name,name);
    ename = name;
    name = buf;
  } else
    ename = name;

  if (Code_findLabel(mcode,name)) {
    yyerror("Redefinition of target '%s'.",ename);
    return;
  }
  Code_addLabel(mcode,name,value);
}

void ycBeginMCode(int addr)
{
  cur.mb = new_MemProg(Code_nbits(mcode),addr);
  Code_addMemProg(mcode,cur.mb);

  cur.opdef = 0;
  cur.proc_name = 0;
}

void ycEndMCode()
{
}

void ycMOp(char *name)
{
  cur.opdef = (OpDef*) SHash_find(&mcode->c_ops,name);
  if (!cur.opdef) {
    yyerror("Undefined operator '%s'",name);
    return;
  }
  cur.noprs = 0;
}

void ycMOperand(int am,struct number *n,int r)
{
  if (cur.noprs == MAXOPRS) {
    yyerror("Too many operands for instruction.");
    return;
  }

  if (cur.proc_name && n && n->ntype == NT_SYMBOL) {
    char buf[1024];

    sprintf(buf,"%s.%s",cur.proc_name,n->v.s);
    n->v.s = strdup(buf);
  }

  cur.opr_type[cur.noprs] = am;
  cur.opr_nums[cur.noprs] = n;
  cur.opr_regs[cur.noprs] = r;

  cur.noprs++;
}

void ycMNext()
{
  if (cur.opdef) {
    OprDef *od = OpDef_findOpr(cur.opdef,cur.opr_type,cur.noprs);
    if (!od) {
      yyerror("Illegal operands for '%s' instruction.",cur.opdef->od_name);
    } else {
      int i;
      int r[MAXOPRS*2];
      Number *n[MAXOPRS*2];
      MemInst *mi;
      int maxAddr = 0;

      for (i = 0;i < od->od_numOprs;i++) {
	Opr *p = od->od_oprs[i];
	switch (p->od_type) {
	case AM_IMMEDIATE :
	case AM_IMMIND :
	  n[p->od_val] = cur.opr_nums[i];
	  break;
	case AM_DIRECT :
	case AM_INDIRECT :
	  r[p->od_reg] = cur.opr_regs[i];
	  break;
	case AM_INDEXED :
	  n[p->od_val] = cur.opr_nums[i];
	  r[p->od_reg] = cur.opr_regs[i];
	  break;
	}
      }

      for (i = 0;i < cur.opdef->od_numInsts;i++) {
	if ((mi = OpInst_genMemInst(cur.opdef->od_insts[i],cur.mb,0,0))) {
	  if (mi->mi_addr > maxAddr) maxAddr = mi->mi_addr;
	  MemProg_addInst(cur.mb,mi);
	} else
	  yyerror("Illegal memory instruction in definition of '%s'.",cur.opdef->od_name);
      }

      for (i = 0;i < od->od_numInsts;i++) {
	if ((mi = OpInst_genMemInst(od->od_insts[i],cur.mb,r,n))) {
	  if (mi->mi_addr > maxAddr) maxAddr = mi->mi_addr;
	  MemProg_addInst(cur.mb,mi);
	}
      }
      while (MemProg_current(cur.mb) <= maxAddr)
	MemProg_incAddr(cur.mb);
    }
    cur.opdef = 0;
  }
}

void ycBss(int n)
{
  cur.mb->mb_current += n;
}

void ycData(int dt)
{
  cur.dtype = dt;
}

void ycDataNum(int v)
{
  MemInst *mi;

  switch (cur.dtype) {
  case DBYTE :
    mi = new_MemInst(MemProg_current(cur.mb),7,0);
    MemInst_setNumValue(mi,v);
    MemProg_addInst(cur.mb,mi);
    MemProg_incAddr(cur.mb);
    break;
  case DSHORT :
    if ((MemProg_current(cur.mb) & 0x1)) {
      mi = new_MemInst(MemProg_current(cur.mb),7,0);
      MemInst_setNumValue(mi,0);
      MemProg_addInst(cur.mb,mi);
      MemProg_incAddr(cur.mb);
    }

    mi = new_MemInst(MemProg_current(cur.mb),7,0);
    MemInst_setNumValue(mi,(v&0xff));
    MemProg_addInst(cur.mb,mi);
    MemProg_incAddr(cur.mb);

    mi = new_MemInst(MemProg_current(cur.mb),7,0);
    MemInst_setNumValue(mi,((v>>8)&0xff));
    MemProg_addInst(cur.mb,mi);
    MemProg_incAddr(cur.mb);
    break;
  case DLONG :
    while ((MemProg_current(cur.mb) & 0x3)) {
      mi = new_MemInst(MemProg_current(cur.mb),7,0);
      MemInst_setNumValue(mi,0);
      MemProg_addInst(cur.mb,mi);
      MemProg_incAddr(cur.mb);
    }

    mi = new_MemInst(MemProg_current(cur.mb),7,0);
    MemInst_setNumValue(mi,(v&0xff));
    MemProg_addInst(cur.mb,mi);
    MemProg_incAddr(cur.mb);

    mi = new_MemInst(MemProg_current(cur.mb),7,0);
    MemInst_setNumValue(mi,((v>>8)&0xff));
    MemProg_addInst(cur.mb,mi);
    MemProg_incAddr(cur.mb);

    mi = new_MemInst(MemProg_current(cur.mb),7,0);
    MemInst_setNumValue(mi,((v>>16)&0xff));
    MemProg_addInst(cur.mb,mi);
    MemProg_incAddr(cur.mb);

    mi = new_MemInst(MemProg_current(cur.mb),7,0);
    MemInst_setNumValue(mi,((v>>24)&0xff));
    MemProg_addInst(cur.mb,mi);
    MemProg_incAddr(cur.mb);
    break;
  default :
    yyerror("Bogonic internal code %d in ycDataNum",cur.dtype);
    break;
  }

}

void ycDataLit(char *v)
{
  yyerror("Unsupported use of literal in data value.");
}

void ycDataStr(char *v)
{
  for (;*v;v++) {
    ycDataNum(*v);
  }
}

void ycBeginProc(char *pname)
{
  if (cur.proc_name) 
    yyerror("Nested procedures not allowed.  Currently in '%s'",cur.proc_name);

  ycMLabel(pname);
  cur.proc_name = pname;
}

void ycEndProc()
{
  cur.proc_name = 0;
}

void ycRegRecord(char *name,int value)
{
  extern struct lex_keywordentry lex_regs[];
  extern int num_lex_regs;
  int i,j;

  for (i = 0;i < num_lex_regs;i++) {
    if (ycKeyCmp(lex_regs[i].Keyword,name) >= 0) break;
  }

  for (j = num_lex_regs;j > i;j--)
    lex_regs[j] = lex_regs[j-1];
  num_lex_regs++;

  lex_regs[i].Keyword = strdup(name);
  lex_regs[i].Value = value;  
}

void ParseFile(char *fileName)
{
  FILE *f;

  if (!(f = fopen(fileName,"r"))) {
    fprintf(stderr,"Failed to open input file '%s'\n",fileName);
    exit(1);
  }

  strcpy(ycFileName,fileName);
  ycLineNumber = 1;

  yyrestart(f);
  BeginBA();
  yyparse();
  fclose(f);

  if (yyErrorCount)
    exit(1);
}

void Number_print(Number *N,FILE *f)
{
  switch (N->ntype) {
  case NT_VALUE :
    printf("<%d>",N->v.d);
    break;
  case NT_SYMBOL :
    printf("<%s>",N->v.s);
    break;
  case NT_RELSYMBOL :
    printf("<%s+%d>",N->v.s,N->offset);
    break;
  }
}

Number *Number_copy(Number *sn)
{
  Number *dn = new(Number);

  *dn = *sn;

  return dn;
}

