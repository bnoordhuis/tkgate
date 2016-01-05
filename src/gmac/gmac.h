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

    Last edit by hansen on Fri Jan  9 19:20:55 2009
****************************************************************************/
#ifndef __gmac_h
#define __gmac_h
#include "config.h"

#define MAXBLOCKS		128	/* Maximum number of code blocks */
#define MAXBANKS		128	/* Maximum number of banks */
#define WORDSPERLINE		8	/* Number of words to output per line */

#define GE_OK			0	/* No error */
#define GE_IDRANGE		1	/* Negative or too large id number */
#define GE_REDECID		2	/* Redefined id number */
#define GE_UNDEFID		3	/* Undefined id number */
#define GE_BADTYPE		4	/* Reg used as value or vice versa */

/*
 * Output formats
 */
#define OF_OLDGATE		0	/* Old TkGate 1.8 format */
#define OF_VERILOG		1	/* Standard verilog format */

#define NMASK(n)	(((n) == 32) ? 0xffffffff : ((1 << (n))-1))

#define new(t)		(t*)malloc(sizeof(t))

/*
 * A bank corresponds to a single RAM or ROM device in a tkgate circuit file.
 * The msb and lsb are bit range from micro-word or macro-word which map to the
 * device, and the address range specifies the portion of address space which
 * will get mapped at address 0 on the specific device.
 */
typedef struct {
  char		*b_name;		/* Name of bank */
  Range		b_range;		/* Range of bits */
  int		b_start;		/* Starting address */
  int		b_end;			/* Ending address */
} Bank;

/*
 * A memory instruction is an operation which modifies a memory location.
 * Memory instructions are used as an intermediate form for generated code
 * as well as to represent macro operations.
 */
typedef struct meminst {
  int		mi_addr;		/* Address of memory instruction */
  Range		mi_sRange;		/* Bit range of source */
  Range		mi_dRange;		/* Bit range of destination */
  int		mi_valType;		/* Type of value to store */
  int		mi_offset;		/* Offset for value (if needed) */
  union {
    int	d;				/* Numeric value */
    char *s;				/* Symbolic value */
  } mi_value;
  struct meminst *mi_next;		/* Next instruction */
} MemInst;

/*
 * A memory program (MemProg) is a sequence of memory instructions.
 */
typedef struct {
  int		mb_nbits;		/* Number of bits in a word */
  unsigned	mb_base;		/* Base address of block */
  unsigned	mb_current;		/* Current address in block */
  MemInst	*mb_first;		/* First memory instruction */
  MemInst	*mb_last;		/* Last memory instruction */
} MemProg;

/*
 * Descriptor for a single operand.  od_type is an AM_* value.
 *
 * %1
 * #2(%3)
 */
typedef struct {
  int		od_type;		/* Type of operand */
  int		od_reg;			/* Id of register */
  int		od_val;			/* Id of value */
} Opr;

/*
 * Describes an operation used to build a macro instruction.
 *
 * Examples:
 *   +1[3:0]=1		type=OI_DATA
 *   +0[3:0]=%2		type=OI_REGOP
 *   +3[3:0]=#1[3:0]	type=OI_NUMOP
 *   +3[3:0]=#1-@+2	type=OI_RELNUMOP
 * 
 */
typedef struct {
  int		oi_type;	/* Type of operation */
  int		oi_base;	/* Base word */
  Range		oi_dRange;	/* Bit range on destionation */
  Range		oi_sRange;	/* Bit range on source */
  int		oi_value;	/* Value or index */
  int		oi_offset;	/* Offset for relative operands */
} OpInst;

/*
 * An OprDef represents a single set of operands and its memory program.
 *
 * %1,%2 = { +1[7:4]=%1, +0[3:0]=%2 } 
 */
typedef struct opr_def OprDef;
struct opr_def {
  int		od_numOprs;		/* Number of operands */
  Opr		**od_oprs;		/* Array of operands */

  int		od_numInsts;		/* Number of op instructions */
  OpInst	**od_insts;		/* Array of op instructions */

  OprDef	*od_next;		/* Next operand */
};

/*
 * An OprGroup is an ordered list of operand sets (OprDef).
 *
 * operands foo {
 *    %1,%2 = { +0[3:0]=0, +1[7:4]=%1, +1[3:0]=%2 };
 *    %1,#2 = { +0[3:0]=5, +1[7:4]=%1, +2[7:0]=#2 };
 * }
 */
typedef struct {
  char		*og_name;		/* Name of group */
  int		og_nbits;		/* Bit width of macro memory */
  int		og_numOdefs;		/* Number of operand defintions */
  OprDef	**og_odefs;		/* Array of operand definitions */
} OprGroup;

/*
 * Definition of macro operation
 */
typedef struct {
  char		*od_name;		/* Name of operator */
  int		od_numGroups;		/* Number of operand groups */
  OprGroup	**od_groups;		/* Array of operand group.  [0] is local group */

  int		od_numInsts;		/* Number of top-level operator instuctions */
  OpInst	**od_insts;		/* Top-level operator instuctions */
} OpDef;

typedef struct {
  char		*uf_name;		/* Name of field */
  int		uf_isNegative;		/* Is this a negative field? */
  int		uf_msb;			/* High bit of field */
  int		uf_lsb;			/* Low bit of field */
  SHash		uf_codes;		/* Symbolic values of field */
} UField;

typedef struct {
  int		c_type;			/* MACROCODE, MICROCODE, or MAP */
  int		c_numBlocks;		/* Number of blocks */
  MemProg	*c_blocks[MAXBLOCKS];	/* Array of code blocks */

  int		c_numBanks;		/* Number of banks */
  Bank		*c_banks[MAXBANKS];	/* Array of banks */

  int		c_nbits;		/* Number of bits in micro-instruction */
  SHash		c_labels;		/* Branch targets */

  SHash		c_ops;			/* Fields in the microcode, ops in macrocode */
  SHash		c_oprs;			/* Operands for macrocode */
} Code;

#define Bank_name(b)		(b)->b_name
#define Bank_msb(b)		(b)->b_range.msb
#define Bank_lsb(b)		(b)->b_range.lsb
#define Bank_start(b)		(b)->b_start
#define Bank_end(b)		(b)->b_end
#define Bank_noAddrRange(b)	((b)->b_start < 0)

#define MemProg_current(mb)	(mb)->mb_current
#define MemProg_incAddr(mb)	(mb)->mb_current++
#define MemProg_start(mb)	(mb)->mb_base
#define MemProg_end(mb)	(mb)->mb_current
#define MemProg_firstMI(mb)	(mb)->mb_first

#define MemInst_next(mi)	(mi)->mi_next

#define UField_name(f)		(f)->uf_name
#define UField_msb(f)		(f)->uf_msb
#define UField_lsb(f)		(f)->uf_lsb
#define UField_findEnum(f,n)	((int*)SHash_find(&(f)->uf_codes,n))

#define Code_nbits(c)		(c)->c_nbits
#define Code_findField(c,n)	((UField*)SHash_find(&(c)->c_ops,n))
#define Code_findLabel(c,n)	((int*)SHash_find(&(c)->c_labels,n))
#define Code_getMBlock(c,n)	(c)->c_blocks[(n)]
#define Code_numMBlocks(c)	(c)->c_numBlocks

#define imin(a,b)	((a) < (b) ? (a) : (b))
#define imax(a,b)	((a) > (b) ? (a) : (b))

#define isOverlap(s1,e1,s2,e2)	(s2 <= e1 && e2 >= s1)

MemInst *new_MemInst(int addr,int dmsb,int dlsb);
void MemInst_setNumValue(MemInst*,int val);
void MemInst_setSymValue(MemInst*,char *val,int offset,int msb,int lsb);
void MemInst_print(MemInst *mi,FILE *f);
int MemInst_value(MemInst *mi);

MemProg *new_MemProg(int,int);
void MemProg_addInst(MemProg*,MemInst*);

Bank *new_Bank(char *name,int msb,int lsb,int start,int end);

UField *new_UField(char *name,int msb,int lsb,int isComp);

OpInst *new_OpInst(int otype,int base,int dmsb,int dlsb,int val,int smsb,int slsb,int offset);
MemInst *OpInst_genMemInst(OpInst*,MemProg*,int*,Number**);

OprDef *new_OprDef(int);
void OprDef_addOpr(OprDef*,int,int,int);
void OprDef_addOpInst(OprDef*,OpInst*);
void OprDef_print(OprDef*,FILE*);
int OprDef_isMatch(OprDef*,int*,int);
int OprDef_check(OprDef*);

OprGroup *new_OprGroup(char*,int);
OprDef *OprGroup_addOprDef(OprGroup *);
OprDef *OprGroup_findOpr(OprGroup*,int*,int);

OpDef *new_OpDef(char*,int);
void OpDef_addOpInst(OpDef*,OpInst*);
void OpDef_addGroup(OpDef*,OprGroup*);
void OpDef_print(OpDef*,FILE*);
OprDef *OpDef_findOpr(OpDef*,int*,int);

Code *new_Code(int);
void Code_addBank(Code *c,Bank *B);
void Code_addField(Code *c,UField *f);
void Code_addMemProg(Code *c,MemProg *mb);
void Code_addLabel(Code *c,char *lab,int addr);
void Code_generate(Code*,FILE*);

void ParseFile(char *fileName);

#endif

