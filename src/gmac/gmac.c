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

    Last edit by hansen on Fri Jan  9 19:52:33 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include "hash.h"
#include "parser.h"
#include "grammar.h"
#include "gmac.h"

/****************************************************************************
 *
 * This is the Gate Microcode and Assembler Compiler (gmac).
 *
 * Code example:
 *

// Microcode memory bank declarations

microcode bank[31:0] iunit.m1;
microcode bank[63:32] iunit.m2;
microcode bank[95:64] iunit.m3;
map bank[7:0] iunit.map;

macrocode bank[7:0] iunit.m1;

// Microcode field declarations

field ~ldir[0], ~ldpc[1], cin[2], next[95:88];
field func[7:4]={add=0, sub=1, lshift=2, rshift=3, arshift=4,
  rroll=5, lroll=6, and=7, or=8, xor=9, not=10, neg=11, nand=12,
  nor=13, eq=14, nop=15 };
field mcpc[87:85]={next=0, jmp=1, jne=2, jlt=3, jgt=4, jeq=5, jc=6, jmap=7};
field next[31:24];

// Assembler declarations

registers R0=0, R1=1, R2=2, R3=3, R4=4, R5=5, R6=6, R7=7;

operands basic {
    %1,%2 = { +0[3:2]=0, +0[1:0]=0, +1[7:4]=%1, +1[3:0]=%2 };
    %1,#2 = { +0[3:2]=0, +0[1:0]=1, +1[7:4]=%1, +2=#2[7:0], +3=#2[15:8] };
    %1,%2,#3 = { +0[3:2]=0, +0[1:0]=1, +1[7:4]=%1, +1[3:0]=%2,
			+2=#3[7:0], +3=#3[15:8] };
    #1(%2),%3 = { +0[3:2]=0 };
    (%1),%2 = { +0[3:2]=0 };
};

op add {
  map 0x21 = add;       // Opcode to microcode mapping
  +0[7:4]=0x21;          // Top nibble of first word contians opcode
  operands basic;
};

begin microcode @ 0
  start:	ldpc  func=nop mcpc=next;
  		mcpc=jmap;
  nop:	mcpc=jne next=add ;
  add:	mcpc=jne next=nop;
end

begin macrocode @ 0
  mov	#123,R0;
  mov	#128,R1;
  mov	(R0),R3;
  add	R1,R3;
  mov	R3,(R0);
end

****************************************************************************/

int outputFormat = OF_VERILOG;

Code *ucode = 0;
Code *mcode = 0;
Code *map = 0;

/*
 * These undoable object functions are only used in the main tkgate code, but
 * since they are used in the common hash table code we provide stubs here for
 * the hash table code to use.
 */
void *ob_malloc(int s,char *x) { return malloc(s); }
void *ob_calloc(int n,int s,char *x) { return calloc(n,s); }
void ob_free(void *p) { free(p); }
char *ob_strdup(char *s) { return strdup(s); }
void ob_set_type(void *o,char *n) {}
void ob_touch(void *o) {}

static unsigned last_addr = 0;

void printindent(int n,FILE *f)
{
  while (n-- > 0) fprintf(f," ");
}


void Range_print(Range *R,FILE *f)
{
  if (R->msb == -1 && R->lsb == -1) return;

  if (R->msb == R->lsb)
    fprintf(f,"[%d]",R->msb);
  else
    fprintf(f,"[%d:%d]",R->msb,R->lsb);
}

Bank *new_Bank(char *name,int msb,int lsb,int start,int end)
{
  Bank *b = new(Bank);

  b->b_name = strdup(name);
  b->b_range.msb = msb;
  b->b_range.lsb = lsb;
  b->b_start = imin(start,end);
  b->b_end = imax(start,end);

  return b;
}

MemInst *new_MemInst(int addr,int dmsb,int dlsb)
{
  MemInst *mi = new(MemInst);

  mi->mi_addr = addr;
  mi->mi_dRange.msb = dmsb;
  mi->mi_dRange.lsb = dlsb;
  mi->mi_sRange.msb = dmsb-dlsb;
  mi->mi_sRange.lsb = 0;
  mi->mi_valType = NT_VALUE;
  mi->mi_offset = 0;
  mi->mi_value.d = 0;
  mi->mi_next = 0;

  return mi;
}

void MemInst_setNumValue(MemInst *mi,int val)
{
  mi->mi_valType = NT_VALUE;
  mi->mi_offset = 0;
  mi->mi_value.d = val;
}

void MemInst_setSymValue(MemInst *mi,char *val,int offset,int msb,int lsb)
{
  mi->mi_valType = NT_SYMBOL;
  mi->mi_offset = offset;
  mi->mi_value.s = val;
  mi->mi_sRange.msb = msb;
  mi->mi_sRange.lsb = lsb;
}

int MemInst_makeReal(MemInst *mi,Code *c,Code *xc)
{
  int *n;
  char *gname = 0;

  if (mi->mi_valType == NT_VALUE) return 0;

  if ((gname = strchr(mi->mi_value.s,'.')))
    gname++;

  n = Code_findLabel(c,mi->mi_value.s);
  if (!n && gname) n = Code_findLabel(c,gname);
  if (!n && xc) n = Code_findLabel(xc,mi->mi_value.s);
  if (!n && xc && gname) n = Code_findLabel(xc,gname);

  if (!n) {
    yyerror("Undefined symbol '%s'.",mi->mi_value.s);
    return -1;
  }

  mi->mi_value.d = *n + mi->mi_offset;
  mi->mi_valType = NT_VALUE;

  return 0;
}

void MemInst_print(MemInst *mi,FILE *f)
{
  fprintf(f,"<@%x [%d:%d]",mi->mi_addr,mi->mi_dRange.msb,mi->mi_dRange.lsb);
  switch (mi->mi_valType) {
  case NT_VALUE :
    fprintf(f,"=0x%x",mi->mi_value.d);
    break;
  case NT_SYMBOL :
    fprintf(f,"=%s",mi->mi_value.s);
    break;
  }
  fprintf(f,">");
}

int MemInst_value(MemInst *mi)
{
  int n;

  if (mi->mi_sRange.msb < 0) return mi->mi_value.d;

  n = mi->mi_sRange.msb-mi->mi_sRange.lsb+1;
  return (mi->mi_value.d >> mi->mi_sRange.lsb) & NMASK(n);
}


MemProg *new_MemProg(int nbits,int base)
{
  MemProg *mb = new(MemProg);

  mb->mb_nbits = nbits;
  mb->mb_base = mb->mb_current = base;
  mb->mb_first = mb->mb_last = 0;

  return mb;
}

void MemProg_addInst(MemProg *mb,MemInst *mi)
{
  if (mb->mb_last) {
    mb->mb_last->mi_next = mi;
    mb->mb_last = mi;
  } else
    mb->mb_first = mb->mb_last = mi;
  mi->mi_next = 0;
}

UField *new_UField(char *name,int msb,int lsb,int isComp)
{
  UField *f = new(UField);
  
  f->uf_name = strdup(name);
  f->uf_msb = msb;
  f->uf_lsb = lsb;
  f->uf_isNegative = isComp;
  SHash_init(&f->uf_codes);

  return f;
}

Opr *new_Opr(int type,int rnum,int dnum)
{
  Opr *O = new(Opr);

  O->od_type = type;
  O->od_reg = rnum;
  O->od_val = dnum;

  return O;
}

void Opr_print(Opr *O,FILE *f)
{
  switch (O->od_type) {
  case AM_IMMEDIATE :
    fprintf(f,"#%d",O->od_val);
    break;
  case AM_IMMIND :
    fprintf(f,"(#%d)",O->od_val);
    break;
  case AM_DIRECT :
    fprintf(f,"%%%d",O->od_reg);
    break;
  case AM_INDIRECT :
    fprintf(f,"(%%%d)",O->od_reg);
    break;
  case AM_INDEXED :
    fprintf(f,"#%d(%%%d)",O->od_val,O->od_reg);
    break;
  default :
    fprintf(f,"?[#%d %% %d]",O->od_val,O->od_reg);
    break;
  }
}

OpInst *new_OpInst(int otype,int base,int dmsb,int dlsb,int val,int smsb,int slsb,int offset)
{
  OpInst *oi = new(OpInst);

  oi->oi_type = otype;
  oi->oi_base = base;
  oi->oi_dRange.msb = dmsb;
  oi->oi_dRange.lsb = dlsb;
  oi->oi_sRange.msb = smsb;
  oi->oi_sRange.lsb = slsb;
  oi->oi_value = val;

  if (oi->oi_sRange.msb < 0) {
    oi->oi_sRange.msb = oi->oi_dRange.msb-oi->oi_dRange.lsb;
    oi->oi_sRange.lsb = 0;
  }

  return oi;
}

void OpInst_print(OpInst *oi,FILE *f)
{
  fprintf(f,"+%d",oi->oi_base);
  Range_print(&oi->oi_dRange,f);
  fprintf(f,"=");

  switch (oi->oi_type) {
  case OI_DATA :
    fprintf(f,"%d",oi->oi_value);
    break;
  case OI_REGOP :
    fprintf(f,"%%%d",oi->oi_value);
    break;
  case OI_NUMOP :
    fprintf(f,"#%d",oi->oi_value);
    break;
  case OI_RELNUMOP :
    fprintf(f,"#%d-@%d",oi->oi_value,oi->oi_offset);
    break;
  default :
    fprintf(f,"?");
    break;
  }
  Range_print(&oi->oi_sRange,f);
}

MemInst *OpInst_genMemInst(OpInst *oi,MemProg *mp,int *r,Number **n)
{
  MemInst *mi;
  unsigned mask;
  int b;

  if ((!r || !n) && oi->oi_type != OI_DATA)
    return 0;


  if (oi->oi_dRange.msb >= 0) {
    mi = new_MemInst(MemProg_current(mp)+oi->oi_base,oi->oi_dRange.msb,oi->oi_dRange.lsb);
    b = oi->oi_dRange.msb-oi->oi_dRange.lsb+1;
  }  else {
    mi = new_MemInst(MemProg_current(mp)+oi->oi_base,mp->mb_nbits-1,0);
    b = mp->mb_nbits;
  }

  if (b < 32)
    mask = (1 << b)-1;
  else
    mask = 0xffffffff;

  switch (oi->oi_type) {
  case OI_DATA :
    MemInst_setNumValue(mi,mask&(oi->oi_value >> oi->oi_sRange.lsb));
    break;
  case OI_REGOP :
    MemInst_setNumValue(mi,mask&(r[oi->oi_value] >> oi->oi_sRange.lsb));
    break;
  case OI_NUMOP :
    if (n[oi->oi_value]->ntype == NT_VALUE)
      MemInst_setNumValue(mi,mask&(n[oi->oi_value]->v.d  >> oi->oi_sRange.lsb));
    else
      MemInst_setSymValue(mi,n[oi->oi_value]->v.s,0,oi->oi_sRange.msb,oi->oi_sRange.lsb);
    break;
  case OI_RELNUMOP :
    if (n[oi->oi_value]->ntype == NT_VALUE)
      MemInst_setNumValue(mi,mask&((n[oi->oi_value]->v.d-MemProg_current(mp))  >> oi->oi_sRange.lsb));
    else
      MemInst_setSymValue(mi,n[oi->oi_value]->v.s,-MemProg_current(mp),oi->oi_sRange.msb,oi->oi_sRange.lsb);
    break;
  }
  return mi;
}

OprDef *new_OprDef(int nbits)
{
  OprDef *D = new(OprDef);

  D->od_numOprs = 0;
  D->od_oprs = (Opr**)malloc(sizeof(Opr*));

  D->od_numInsts = 0;
  D->od_insts = (OpInst**)malloc(sizeof(OpInst*));

  D->od_next = 0;

  return D;
}

void OprDef_addOpInst(OprDef *D,OpInst *I)
{
  D->od_insts = (OpInst**)realloc(D->od_insts,(D->od_numInsts+1)*sizeof(OpInst*));
  D->od_insts[D->od_numInsts++] = I;
}

void OprDef_addOpr(OprDef *D,int type,int rnum,int dnum)
{
  D->od_oprs = (Opr**)realloc(D->od_oprs,(D->od_numOprs+1)*sizeof(Opr*));
  D->od_oprs[D->od_numOprs++] = new_Opr(type,rnum,dnum);
}

void OprDef_print(OprDef *D,FILE *f)
{
  int i;

  if (D->od_numOprs == 0)
    fprintf(f,"-");
  else {
    for (i = 0;i < D->od_numOprs;i++) {
      if (i) fprintf(f,", ");
      Opr_print(D->od_oprs[i],f);
    }
  }
  fprintf(f," = {");
  for (i = 0;i < D->od_numInsts;i++) {
    if (i) fprintf(f,", ");
    OpInst_print(D->od_insts[i],f);
  }

  fprintf(f,"}\n");
}

int OprDef_isMatch(OprDef *od,int *opr,int nopr)
{
  int i;

  if (nopr != od->od_numOprs)
    return 0;

  for (i = 0;i < nopr;i++)
    if (od->od_oprs[i]->od_type != opr[i])
      return 0;

  return 1;
}

int OprDef_check(OprDef *od)
{
  int a[MAXOPRS*2];
  int i;

  for (i = 0;i < MAXOPRS*2;i++)
    a[i] = -1;

  for (i = 0;i < od->od_numOprs;i++) {
    Opr *p = od->od_oprs[i];
    switch (p->od_type) {
    case AM_IMMEDIATE :
    case AM_IMMIND :
      if (p->od_val < 0 || p->od_val >= MAXOPRS*2) return GE_IDRANGE;
      if (a[p->od_val] >= 0) return GE_REDECID;
      a[p->od_val] = 1;
      break;
    case AM_DIRECT :
    case AM_INDIRECT :
      if (p->od_reg < 0 || p->od_reg >= MAXOPRS*2) return GE_IDRANGE;
      if (a[p->od_reg] >= 0) return GE_REDECID;
      a[p->od_reg] = 2;
      break;
    case AM_INDEXED :
      if (p->od_reg < 0 || p->od_reg >= MAXOPRS*2) return GE_IDRANGE;
      if (p->od_val < 0 || p->od_val >= MAXOPRS*2) return GE_IDRANGE;
      if (a[p->od_reg] >= 0) return GE_REDECID;
      if (a[p->od_val] >= 0) return GE_REDECID;
      a[p->od_val] = 1;
      a[p->od_reg] = 2;
      break;
    }
  }

  for (i = 0;i < od->od_numInsts;i++) {
    OpInst *oi = od->od_insts[i];
    switch (oi->oi_type) {
    case OI_REGOP :
      if (oi->oi_value < 0 || oi->oi_value >= MAXOPRS*2) return GE_IDRANGE;
      if (a[oi->oi_value] == 1) return GE_BADTYPE;
      if (a[oi->oi_value] != 2) return GE_UNDEFID;
      break;
    case OI_NUMOP :
    case OI_RELNUMOP :
      if (oi->oi_value < 0 || oi->oi_value >= MAXOPRS*2) return GE_IDRANGE;
      if (a[oi->oi_value] == 2) return GE_BADTYPE;
      if (a[oi->oi_value] != 1) return GE_UNDEFID;
      break;
      break;
    }
  }

  return GE_OK;
}


OprGroup *new_OprGroup(char *name,int nbits)
{ 
  OprGroup *G = new(OprGroup);

  G->og_name = strdup(name);
  G->og_nbits = nbits;
  G->og_numOdefs = 0;
  G->og_odefs = (OprDef**)malloc(sizeof(OprDef*));

  return G;
}

OprDef *OprGroup_addOprDef(OprGroup *G)
{
  OprDef *D = new_OprDef(G->og_nbits);

  G->og_odefs = (OprDef**)realloc(G->og_odefs,(G->og_numOdefs+1)*sizeof(OprDef*));
  G->og_odefs[G->og_numOdefs++] = D;

  return D;
}

void OprGroup_print(OprGroup *og,int indent,FILE *f)
{
  int i;

  printindent(indent,f); fprintf(f,"operands %s {\n",og->og_name);
  for (i = 0;i < og->og_numOdefs;i++) {
    printindent(indent+2,f);
    OprDef_print(og->og_odefs[i],f);
  }
  printindent(indent,f); fprintf(f,"}\n");
}


OprDef *OprGroup_findOpr(OprGroup *og,int *opr,int nopr)
{
  int i;

  for (i = 0;i < og->og_numOdefs;i++) {
    if (OprDef_isMatch(og->og_odefs[i],opr,nopr))
      return og->og_odefs[i];
  }
  return 0;
}

OpDef *new_OpDef(char *name,int nbits)
{
  OpDef *od = new(OpDef);

  od->od_name = strdup(name);
  od->od_numGroups = 0;
  od->od_groups = (OprGroup**)malloc(sizeof(OprGroup*));
  od->od_groups[0] = new_OprGroup("*local*",nbits);
  od->od_numInsts = 0;
  od->od_insts = (OpInst**)malloc(sizeof(OpInst*));

  return od;
}

void OpDef_addOpInst(OpDef *od,OpInst *oi)
{
  od->od_insts = (OpInst**)realloc(od->od_insts,(od->od_numInsts+1)*sizeof(OpInst*));
  od->od_insts[od->od_numInsts++] = oi;
}

void OpDef_addGroup(OpDef *od,OprGroup *og)
{
  od->od_groups = (OprGroup**)realloc(od->od_groups,(od->od_numGroups+1)*sizeof(OprGroup*));
  od->od_groups[od->od_numGroups++] = og;
}

void OpDef_print(OpDef *od,FILE *f)
{
  int i;

  fprintf(f,"op %s {\n",od->od_name);
  for (i = 0;i < od->od_numInsts;i++) {
    fprintf(f,"   ");
    OpInst_print(od->od_insts[i],f);
    fprintf(f,"\n");
  }
  for (i = 0;i < od->od_numGroups;i++) {
    OprGroup_print(od->od_groups[i],3,f);
  }
  fprintf(f,"}\n");
}

OprDef *OpDef_findOpr(OpDef *od,int *opr,int nopr)
{
  OprDef *d;
  int i;

  for (i = 0;i < od->od_numGroups;i++) {
    if ((d = OprGroup_findOpr(od->od_groups[i],opr,nopr)))
      return d;
  }
  return 0;
}



Code *new_Code(int type)
{
  Code *c = new(Code);

  c->c_type = type;
  SHash_init(&c->c_ops);
  SHash_init(&c->c_oprs);
  SHash_init(&c->c_labels);

  c->c_nbits = 0;
  c->c_numBlocks = 0;
  c->c_numBanks = 0;

  return c;
}

void Code_addBank(Code *c,Bank *b)
{
  c->c_nbits = imax(c->c_nbits,b->b_range.msb+1);
  c->c_banks[c->c_numBanks++] = b;
}

void Code_addMemProg(Code *c,MemProg *mb)
{
  c->c_blocks[c->c_numBlocks++] = mb;
}

void Code_addField(Code *c,UField *f)
{
  SHash_insert(&c->c_ops,UField_name(f),f);
}

void Code_addLabel(Code *c,char *lab,int addr)
{
  int *n = new(int);
  *n = addr;
  SHash_insert(&c->c_labels,lab,n);
}

int Code_realizeMems(Code *c,Code *xc)
{
  int i;
  int code = 0;

  for (i = 0;i < c->c_numBlocks;i++) {
    MemProg *mb = c->c_blocks[i];
    MemInst *mi;

    for (mi = MemProg_firstMI(mb);mi;mi = MemInst_next(mi))
      if (MemInst_makeReal(mi,c,xc) < 0)
	code = -1;
  }

  return code;
}

static void gen_reset()
{
  last_addr = 0;
}

/*
 * Generate a word of output
 */
static void gen_word(FILE *f,unsigned addr,unsigned data,int *num_on_line)
{
  int do_addr = 0;

  if (*num_on_line == WORDSPERLINE) {
    fprintf(f,"\n");
    *num_on_line = 0;
  }

  switch (outputFormat) {
  case OF_OLDGATE :
    if (*num_on_line == 0)
      fprintf(f,"%x/",addr);
    fprintf(f," %x",data);
    break;
  case OF_VERILOG:
    //
    // No current address, or a skip in addresses
    //
    if ((last_addr+1) != addr)
      do_addr = 1;

    //
    // Address begins a block of 256
    //
    if ((addr & 0xff) == 0)
      do_addr = 1;

    if (do_addr) {
      fprintf(f,"\n@%x\n",addr);
      (*num_on_line) = 0;
    }
    if (*num_on_line !=  0)
      fprintf(f," ");
    fprintf(f,"%x",data);
    last_addr = addr;
    break;
  }
  (*num_on_line)++;
}

void Code_generate(Code *c,FILE *f)
{
  unsigned data;
  int i,j;

  data = 0;

  for (i = 0;i < c->c_numBanks;i++) {
    Bank *b = c->c_banks[i];
    MemInst *mi;

    if (c->c_numBlocks == 0) continue;

    if (outputFormat == OF_VERILOG)
      fprintf(f,"\n@memory %s\n",Bank_name(b));
    else
      fprintf(f,"\nmemory %s\n",Bank_name(b));

    gen_reset();

    for (j = 0;j < c->c_numBlocks;j++) {
      MemProg *mb = c->c_blocks[j];
      int num_on_line = 0;
      int cur_addr = -1;

      if (!Bank_noAddrRange(b) && !isOverlap(Bank_start(b),Bank_end(b),
					     MemProg_start(mb),MemProg_end(mb)))
	continue;

      for (mi = MemProg_firstMI(mb);mi;mi = MemInst_next(mi)) {
	int shift;
	HashElem *E;

	if (!Bank_noAddrRange(b)) {
	  if (mi->mi_addr < Bank_start(b)) { continue; }
	  if (mi->mi_addr > Bank_end(b)) { break; }
	}

	if (mi->mi_addr != cur_addr) {
	  if (cur_addr >= 0) {
	    gen_word(f,cur_addr,data,&num_on_line);
	    if (mi->mi_addr != cur_addr+1) {
	      fprintf(f,"\n");
	      num_on_line = 0;
	    }
	  }
	  
	  data = 0;
	  cur_addr = mi->mi_addr;

	  if (c->c_type == MICROCODE) {
	    for (E = Hash_first(&c->c_ops);E;E = Hash_next(&c->c_ops,E)) {
	      UField *F = (UField *) HashElem_obj(E);
	      int x;

	      if (!F->uf_isNegative) continue;
	      x = F->uf_lsb - Bank_lsb(b);
	      if (x >= 0  && x < 32) {
		data |= 1 << x;
	      }
	    }
	  }
	}

	if (isOverlap(mi->mi_dRange.lsb,mi->mi_dRange.msb,Bank_lsb(b),Bank_msb(b))) {
	  shift = mi->mi_dRange.lsb-Bank_lsb(b);
	  if  (shift > 0) 
	    data ^= MemInst_value(mi) << shift;
	  else
	    data ^= MemInst_value(mi) >> -shift;
	}
      }
      if (cur_addr >= 0)
	gen_word(f,cur_addr,data,&num_on_line);
      fprintf(f,"\n");
    }
  }
}

void writeOutput(FILE *f)
{
  if (Code_numMBlocks(ucode)) {
    Code_realizeMems(ucode,0);
    Code_realizeMems(map,ucode);
    Code_generate(ucode,f);
    Code_generate(map,f);
  }
  if (Code_numMBlocks(mcode)) {
    Code_realizeMems(mcode,0);
    Code_generate(mcode,f);
  }
}

int q_strcmp(const void *va,const void *vb)
{
  char *a = *(char**)va;
  char *b = *(char**)vb;

  return strcmp(a,b);
}

char **get_sorted_symbol_list(SHash *H)
{
  HashElem *E;
  char **sym_list;
  int n = Hash_numElems(H);
  int i;

  sym_list = (char**) malloc(sizeof(char*)*(n+1));
  i = 0;
  for (E = Hash_first(H);E;E = Hash_next(H,E)) {
    sym_list[i++] = SHashElem_key(E);
  }
  if (n > 1)
    qsort(sym_list,n,sizeof(char*),q_strcmp);

  return sym_list;
}

void writeMap(FILE *f)
{
  int i;

  if (ucode) {
    int n = Hash_numElems(&ucode->c_labels);
    char **sym_list = get_sorted_symbol_list(&ucode->c_labels);

    fprintf(f,"Microcode Symbols (total=%d):\n",n);
    fprintf(f,"  ---------------------------------------\n");
    for (i = 0;i < n;i++) {
      char *label = sym_list[i];
      unsigned addr = *(unsigned*)SHash_find(&ucode->c_labels,label);
      fprintf(f,"  %-30s %08x\n",label,addr);
    }
    fprintf(f,"\n\n");
  }
  if (mcode) {
    int n = Hash_numElems(&mcode->c_labels);
    char **sym_list = get_sorted_symbol_list(&mcode->c_labels);

    fprintf(f,"Macrocode Symbols (total=%d):\n",Hash_numElems(&mcode->c_labels));
    fprintf(f,"  Global         Local\n");
    fprintf(f,"  ---------------------------------------\n");
    for (i = 0;i < n;i++) {
      char *label = sym_list[i];
      unsigned addr = *(unsigned*)SHash_find(&mcode->c_labels,label);
      char *p = strchr(label,'.');
      if (p) 
	fprintf(f,"                 %-15s %08x\n",p+1,addr);
      else
	fprintf(f,"  %-30s %08x\n",label,addr);
    }
    fprintf(f,"\n\n");
  }
}


void usage()
{
  fprintf(stderr,"Usage: gmac [-o outfile][-m mapfile][infiles...]\n");
  exit(1);
}

int main(int argc,char *argv[])
{
  extern char *optarg;
  extern int optind;
#if OPTRESET
  extern int optreset;
#endif
  char *outf = 0;
  char *mapFile = 0;
  int c;

  mcode = new_Code(MACROCODE);
  ucode = new_Code(MICROCODE);
  map = new_Code(MAP);

  while (argc > 0) {
    while ((c = getopt(argc,argv,"go:m:")) != EOF) {
      switch (c) {
      case 'g' :
	outputFormat = OF_OLDGATE;
	break;
      case 'o' :
	if (outf) usage();
	outf = optarg;
	break;
      case 'm' :
	if (mapFile) usage();
	mapFile = optarg;
	break;
      default :
	usage();
	break;
      }
    }
    argc -= optind;
    argv += optind;
#if OPTRESET
    optreset = 1;
#endif
    optind = 0;
    if (argc > 0) {
      ParseFile(argv[0]);
      argc--;
      argv++;
    }
  }

  if (outf) {
    FILE *f;

    if (!(f = fopen(outf,"w"))) {
      fprintf(stderr,"Failed to open output file '%s'.",outf);
    }
    writeOutput(f);
    fclose(f);
  } else {
    writeOutput(stdout);
  }

  if (mapFile) {
    FILE *f;

    if (!(f = fopen(mapFile,"w"))) {
      fprintf(stderr,"Failed to open map file '%s' for output.",mapFile);
    }
    writeMap(f);
    fclose(f);
  }


  return 0;
}

