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

    Last edit by hansen on Mon Feb  2 16:54:46 2009
****************************************************************************/
#include "thyme.h"

static char lastExprError[STRMAX] = "";

/*****************************************************************************
 * Precedence level at which to omit spaces arround the operator when
 * printing.
 *****************************************************************************/
#define TIGHT_LEVEL 11

/*
 * Operator table
 */
extern OpDesc opTable[];
extern int opTable_size;

/*****************************************************************************
 *
 * Get the last error message resulting from an expression manipulation.
 *
 * Parameters:
 *     buf		Buffer in which to store message.
 *
 *****************************************************************************/
void Expr_getLastError(char *buf)
{
  strcpy(buf, lastExprError);
}

/*****************************************************************************
 *
 * Find the operator descriptor for the specified expression code
 *
 * Parameters:
 *     code		Code to look up
 *
 * Returns:		Operator descritor if found, null if not found.
 *
 *****************************************************************************/
static OpDesc *OpDesc_find(exprcode_t code)
{
  int i;

  for (i = 0;i < opTable_size;i++) {
    if (opTable[i].od_type == code)
      return &opTable[i];
  }
  return 0;
}

/*****************************************************************************
 *
 * Find the operator descriptor from a function pointer
 *
 * Parameters:
 *     func		Code to look up
 *
 * Returns:		Operator descritor if found, null if not found.
 *
 *****************************************************************************/
OpDesc *OpDesc_findFunc(valueop_f *opfunc)
{
  int i;

  for (i = 0;i < opTable_size;i++) {
    if (opTable[i].od_opfunc == opfunc || opTable[i].od_w_opfunc == opfunc)
      return &opTable[i];
  }
  return 0;
}

/*****************************************************************************
 *
 * Create a literal expression
 *
 * Parameters:
 *     name		String with literal name.
 *
 * Returns:		Expression object representing an identifier.
 *
 *****************************************************************************/
Expr *new_Expr_lit(const char *name)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = E_LITERAL;
  e->e.literal.name = strdup(name);
  if (strchr(name,'.'))
    e->e.literal.ishlit = 1;
  else
    e->e.literal.ishlit = 0;

  return e;
}

/*****************************************************************************
 *
 * Create a task expression
 *
 * Parameters:
 *     name		Name of task to execute
 *     args		Arguments of task
 *
 * Returns:		Expression object representing a task.
 *
 *****************************************************************************/
Expr *new_Expr_task(const char *name,List *args)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = E_TASK;
  e->e.task.name = strdup(name);
  e->e.task.argc = args ? List_numElems(args) : 0;
  e->e.task.argv = 0;

  if (args && List_numElems(args) > 0) {
    ListElem *le;
    int i = 0;

    e->e.task.argv = (Expr**) malloc(List_numElems(args)*sizeof(Expr*));
    for (le = List_first(args);le;le = List_next(args,le)) {
      e->e.task.argv[i++] = (Expr*) ListElem_obj(le);
    }
  }

  return e;
}


/*****************************************************************************
 *
 * Create a single operand operator expression
 *
 * Parameters:
 *     op		Operator code
 *     x		Expression being operated on.
 *
 * Returns:		Single operand operator expression.
 *
 *****************************************************************************/
Expr *new_Expr_op1(exprcode_t op,Expr *x)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = op;
  e->e.opr[0] = 0;
  e->e.opr[1] = x;
  e->e.opr[2] = 0;

  return e;
}

/*****************************************************************************
 *
 * Create a two operand operator expression
 *
 * Parameters:
 *     op		Operator code
 *     l		Left-hand operand
 *     r		Right-hand operand
 *
 * Returns:		Two operand operator expression.
 *
 *****************************************************************************/
Expr *new_Expr_op(exprcode_t op,Expr *l,Expr *r)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = op;
  e->e.opr[0] = l;
  e->e.opr[1] = r;
  e->e.opr[2] = 0;

  return e;
}

/*****************************************************************************
 *
 * Create a three operand operator expression (e.g. "a ? b : c")
 *
 * Parameters:
 *     op		Operator code
 *     l		Left-hand operand
 *     m		Middle operand
 *     r		Right-hand operand
 *
 * Returns:		Three operand operator expression.
 *
 *****************************************************************************/
Expr *new_Expr_op3(exprcode_t op,Expr *l,Expr *m,Expr *r)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = op;
  e->e.opr[0] = l;
  e->e.opr[1] = m;
  e->e.opr[2] = r;

  return e;
}

/***************************************************************************** 
 *
 * Create a repeating concat operator expression
 *
 * Parameters:
 *     b		Number of replications (must evaluate to constant)
 *     x		Expression to be replicated
 *
 * Returns:		Concatenated expression.
 *
 *****************************************************************************/
Expr *new_Expr_repcat(Expr *b,Expr *x)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = E_REPCAT;
  e->e.opr[0] = b;
  e->e.opr[1] = x;
  e->e.opr[2] = 0;

  return e;
}

/***************************************************************************** 
 *
 * Create an expression for an unsized integer
 *
 * Parameters:
 *     n		Integer to create expression from.
 *
 * Returns:		Integer value expression.
 *
 *****************************************************************************/
Expr *new_Expr_num(int n)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = E_NUMBER;
  e->e.snum = new_Value(SSWORDSIZE);
  Value_convertI(e->e.snum,n);

  return e;
}

/***************************************************************************** 
 *
 * Create an expression for a float
 *
 * Parameters:
 *     n		Integer to create expression from.
 *
 * Returns:		Integer value expression.
 *
 *****************************************************************************/
Expr *new_Expr_realnum(real_t n)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = E_REAL;
  e->e.snum = new_Value(SSWORDSIZE);
  Value_convertR(e->e.snum,n);

  return e;
}

/***************************************************************************** 
 *
 * Create an expression for a sized value
 *
 * Parameters:
 *     spec		Specification of sized value (e.g., "8'h3f")
 *
 * Returns:		Sized value expression.
 *
 *****************************************************************************/
Expr *new_Expr_hex(const char *spec)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = E_HEX;
  e->e.snum = new_Value(0);
  if (Value_convert(e->e.snum,spec) != 0)
    errorFile(Place_getCurrent(),ERR_BADVALUE,spec);

  return e;
}

/***************************************************************************** 
 *
 * Create an expression from a string.
 *
 * Parameters:
 *     s		String to convert to expression
 *
 * Returns:		Expression representing string
 *
 *****************************************************************************/
Expr *new_Expr_str(const char *s)
{
  Expr *e = (Expr*) malloc(sizeof(Expr));

  e->e_type = E_HEX;
  e->e.snum = new_Value(0);
  Value_convertStr(e->e.snum,s);

  return e;
}

/*****************************************************************************
 *
 * Free memory associated with an expression and all children.
 *
 * Parameters:
 *     e		Expression to be freed.
 *
 *****************************************************************************/
void delete_Expr(Expr *e)
{
  int i;

  switch (e->e_type) {
  case E_LITERAL :
    free(Expr_getLitName(e));
    break;
  case E_HEX :
    delete_Value(e->e.snum);
    break;
  case E_NUMBER :
    break;
  default :			/* Assumed to be an operator */
    for (i = 0;i < 3;i++)
      if (e->e.opr[i])
	delete_Expr(e->e.opr[i]);
    break;
  }

  free(e);
}


/*****************************************************************************
 *
 * Print internal part of a concatenation expression
 *
 * Parameters:
 *     f		File on which to print
 *     e		Expression to be printed
 *
 *****************************************************************************/
static void Expr_printCat(Expr *e,FILE *f)
{
  if (e->e_type != E_CONCAT) {
    Expr_print(e,f);
    return;
  }

  Expr_printCat(e->e.opr[0],f);
  fprintf(f,",");
  Expr_printCat(e->e.opr[1],f);
}

/*****************************************************************************
 *
 * Generate string of a concatenation expression
 *
 * Parameters:
 *     p		String pointer to write to
 *     e		Expression to be printed
 *
 *****************************************************************************/
static char *Expr_getstrCat(Expr *e,char *p)
{
  if (e->e_type != E_CONCAT) {
    p = Expr_getstr(e,p);
    return p;
  }

  p = Expr_getstrCat(e->e.opr[0],p);
  p += sprintf(p,",");
  p = Expr_getstrCat(e->e.opr[1],p);

  return p;
}

/*****************************************************************************
 *
 * Generate a string with an expression 
 *
 * Parameters:
 *     p		String to write to.
 *     e		Expression to be printed
 *
 *****************************************************************************/
char *Expr_getstr(Expr*e,char *p)
{
  OpDesc *od = 0;

  if (!e) {
    p+=sprintf(p,"<<null>>");
    return p;
  }

  /*
   * Check for and print any non-operator or special operator (contatenation) expressions.
   */
  switch (e->e_type) {
  case E_AT :
    p+=sprintf(p,"@(");
    if (e->e.opr[1])
      p = Expr_getstr(e->e.opr[1],p);
    else
      p += sprintf(p,"*");
    p += sprintf(p,")");
    break;
  case E_LITERAL :
    p += sprintf(p,"%s",Expr_getLitName(e));
    return p;
  case E_NUMBER :
  case E_HEX :
    Value_getstr(e->e.snum,p);
    p += strlen(p);
    return p;
  case E_REPCAT :
    p += sprintf(p,"{");
    p = Expr_getstr(e->e.opr[0],p);
    if (e->e.opr[1]->e_type == E_CONCAT)
      p = Expr_getstr(e->e.opr[1],p);		/* Omit braces if repeating a concat */
    else {
      p += sprintf(p,"{");
      p = Expr_getstr(e->e.opr[1],p);
      p += sprintf(p,"}");
    }
    p += sprintf(p,"}");
    return p;
  case E_CONCAT :
    p += sprintf(p,"{");
    p = Expr_getstrCat(e,p);
    p += sprintf(p,"}");
    return p;
  case E_RANGE :
  case E_VECTORP :
  case E_VECTORN :
    p = Expr_getstr(e->e.opr[0],p);
    p += sprintf(p,"[");
    p = Expr_getstr(e->e.opr[1],p);
    if (e->e.opr[2]) {
      if (e->e_type == E_RANGE)
	p += sprintf(p,":");
      else if (e->e_type == E_VECTORP)
	p += sprintf(p,"+:");
      else
	p += sprintf(p,"-:");

      p = Expr_getstr(e->e.opr[2],p);
    }
    p += sprintf(p,"]");
    return p;
  default:
    break;
  }

  od = OpDesc_find(e->e_type);
  if (!od) {
    p += sprintf(p,"[?]");			/* unknown expression type */
    return p;
  }

  if (od->od_nopr == 3) {
    return p;
  }
  
  if (e->e.opr[0]) {
    OpDesc *opr_od = OpDesc_find(e->e.opr[0]->e_type);

    if (opr_od && opr_od->od_plev > od->od_plev) p += sprintf(p,"(");
    p = Expr_getstr(e->e.opr[0],p);
    if (opr_od && opr_od->od_plev > od->od_plev) p += sprintf(p,")");
  }

  if (e->e.opr[1]) {
    OpDesc *opr_od = OpDesc_find(e->e.opr[1]->e_type);

    if (od->od_plev >= TIGHT_LEVEL)
      p += sprintf(p,"%s",od->od_text);
    else
      p += sprintf(p," %s ",od->od_text);

    if (opr_od && opr_od->od_plev > od->od_plev) p += sprintf(p,"(");
    p = Expr_getstr(e->e.opr[1],p);
    if (opr_od && opr_od->od_plev > od->od_plev) p += sprintf(p,")");
  }

  if (e->e.opr[2]) {
    OpDesc *opr_od = OpDesc_find(e->e.opr[2]->e_type);

    if (od->od_plev >= TIGHT_LEVEL)
      p += sprintf(p,"%s",strend(od->od_text)+1);
    else
      p += sprintf(p," %s ",strend(od->od_text)+1);

    if (opr_od && opr_od->od_plev > od->od_plev) p += sprintf(p,"(");
    p = Expr_getstr(e->e.opr[2],p);
    if (opr_od && opr_od->od_plev > od->od_plev) p += sprintf(p,")");
  }

  return p;
}

/*****************************************************************************
 *
 * Print an expression
 *
 * Parameters:
 *     f		File on which to print
 *     e		Expression to be printed
 *
 *****************************************************************************/
void Expr_print(Expr*e,FILE *f)
{
  OpDesc *od = 0;

  if (!e) {
    fprintf(f,"<<null>>");
    return;
  }

  /*
   * Check for and print any non-operator or special operator (contatenation) expressions.
   */
  switch (e->e_type) {
  case E_AT :
    fprintf(f,"@(");
    if (e->e.opr[1])
      Expr_print(e->e.opr[1],f);
    else
      fprintf(f,"*");
    fprintf(f,")");
    break;
  case E_LITERAL :
    fprintf(f,"%s",Expr_getLitName(e));
    return;
  case E_NUMBER :
  case E_HEX :
    Value_print(e->e.snum,f);
    return;
  case E_REPCAT :
    fprintf(f,"{");
    Expr_print(e->e.opr[0],f);
    if (e->e.opr[1]->e_type == E_CONCAT)
      Expr_print(e->e.opr[1],f);		/* Omit braces if repeating a concat */
    else {
      fprintf(f,"{");
      Expr_print(e->e.opr[1],f);
      fprintf(f,"}");
    }
    fprintf(f,"}");
    return;
  case E_CONCAT :
    fprintf(f,"{");
    Expr_printCat(e,f);
    fprintf(f,"}");
    return;
  case E_RANGE :
  case E_VECTORP :
  case E_VECTORN :
    Expr_print(e->e.opr[0],f);
    fprintf(f,"[");
    Expr_print(e->e.opr[1],f);
    if (e->e.opr[2]) {
      if (e->e_type == E_RANGE)
	fprintf(f,":");
      else if (e->e_type == E_VECTORP)
	fprintf(f,"+:");
      else
	fprintf(f,"-:");

      Expr_print(e->e.opr[2],f);
    }
    fprintf(f,"]");
    return;
  case E_DELAY :
    fprintf(f,"#");
    if (e->e.opr[DT_MIN]) {
      Expr_print(e->e.opr[DT_MIN],f);
      fprintf(f,":");
      Expr_print(e->e.opr[DT_TYP],f);
      fprintf(f,":");
      Expr_print(e->e.opr[DT_MAX],f);
    } else
      Expr_print(e->e.opr[DT_TYP],f);
    return;
  default:
    break;
  }

  od = OpDesc_find(e->e_type);
  if (!od) {
    fprintf(f,"[?]");			/* unknown expression type */
    return;
  }

  if (od->od_nopr == 3) {
    return;
  }
  
  if (e->e.opr[0]) {
    OpDesc *opr_od = OpDesc_find(e->e.opr[0]->e_type);

    if (opr_od && opr_od->od_plev > od->od_plev) fprintf(f,"(");
    Expr_print(e->e.opr[0],f);
    if (opr_od && opr_od->od_plev > od->od_plev) fprintf(f,")");
  }

  if (e->e.opr[1]) {
    OpDesc *opr_od = OpDesc_find(e->e.opr[1]->e_type);

    if (od->od_plev >= TIGHT_LEVEL)
      fprintf(f,"%s",od->od_text);
    else
      fprintf(f," %s ",od->od_text);

    if (opr_od && opr_od->od_plev > od->od_plev) fprintf(f,"(");
    Expr_print(e->e.opr[1],f);
    if (opr_od && opr_od->od_plev > od->od_plev) fprintf(f,")");
  }

  if (e->e.opr[2]) {
    OpDesc *opr_od = OpDesc_find(e->e.opr[2]->e_type);

    if (od->od_plev >= TIGHT_LEVEL)
      fprintf(f,"%s",strend(od->od_text)+1);
    else
      fprintf(f," %s ",strend(od->od_text)+1);

    if (opr_od && opr_od->od_plev > od->od_plev) fprintf(f,"(");
    Expr_print(e->e.opr[2],f);
    if (opr_od && opr_od->od_plev > od->od_plev) fprintf(f,")");
  }
}

/*****************************************************************************
 *
 * Return a range
 *
 * Parameters:
 *     msb		Most significant bit expression.
 *     lsb		Least significant bit expression.
 *
 *****************************************************************************/
VRange *new_VRange(rangestyle_t rs,Expr *left,Expr *right)
{
  VRange *r = (VRange*) malloc(sizeof(VRange));

  r->vr_style = rs;
  r->vr_left = left;
  r->vr_right = right;

  return r;
}

void delete_VRange(VRange *r, int recursive)
{
  if (recursive) {
    if (r->vr_left) delete_Expr(r->vr_left);
    if (r->vr_right) delete_Expr(r->vr_right);
  }
  free(r);
}


/*****************************************************************************
 *
 * Print a range
 *
 * Parameters:
 *     r		Range to print
 *     f		File on which to print
 *
 *****************************************************************************/
void VRange_print(VRange *r,FILE *f)
{
  if (!r) return;

  if (r->vr_style == RS_AUTO) {
    fprintf(f,"[*]");
    return;
  }


  fprintf(f,"[");
  Expr_print(r->vr_left, f);
  if (r->vr_style != RS_SINGLE) {
    if (r->vr_style == RS_BASEUP)
      fprintf(f,"+:");
    else if (r->vr_style == RS_BASEDN)
      fprintf(f,"-:");
    else
      fprintf(f,":");

    Expr_print(r->vr_right, f);
  }
  fprintf(f,"]");
}

/*****************************************************************************
 *
 * Print a range that is directly specified with numbers.
 *
 * Parameters:
 *     r		Range to print
 *     f		File on which to print
 *
 *****************************************************************************/
int VRange_getDirect(VRange *r,char *buf)
{
  unsigned size;

  if (VRange_getSize(r, 0, &size) < 0) {
    sprintf(buf,"0:0");
    return -1;
  }

  sprintf(buf,"%u:0",size-1);
  return 0;
}


/*****************************************************************************
 *
 * Get the LSB expression of a range
 *
 * Parameters:
 *     r		Range to evaluate
 *
 * Returns:		Expression for LSB
 *
 *****************************************************************************/
Expr *VRange_getLsb(VRange *r)
{
  switch (r->vr_style) {
  case RS_SINGLE :
  case RS_BASEUP :
  case RS_BASEDN :
    return r->vr_left;
  case RS_MAXMIN :
    return r->vr_right;
  case RS_AUTO :
    return 0;
  }
  return 0;
}

/*****************************************************************************
 *
 * Evaluate constant expressions and return the MSB of a range.
 *
 * Parameters:
 *     r		Range to evaluate
 *     scope		Scope for variable lookup
 *     *msb		Returns msb here.
 *
 *****************************************************************************/
int VRange_parmEvalMsb(VRange *r,Scope *scope,unsigned *msb)
{
  switch (r->vr_style) {
  case RS_SINGLE :
  case RS_MAXMIN :
  case RS_BASEDN :
    return Expr_parmEvalI(r->vr_left,scope,msb,0);
  case RS_BASEUP :
    {
      unsigned lsb,width;

      if (Expr_parmEvalI(r->vr_left,scope,&lsb,0) < 0) return -1;
      if (Expr_parmEvalI(r->vr_right,scope,&width,0) < 0) return -1;
      *msb = lsb + width -1;
      return 0;
    }
  case RS_AUTO :
    *msb =  Expr_getCollapsedBitSize(r->vr_left,scope)-1;
    return 0;
  }
  return -1;
}

/*****************************************************************************
 *
 * Evaluate constant expressions and return the LSB of a range.
 *
 * Parameters:
 *     r		Range to evaluate
 *     scope		Scope for variable lookup
 *     *lsb		Returns lsb here.
 *
 *****************************************************************************/
int VRange_parmEvalLsb(VRange *r,Scope *scope,unsigned *lsb)
{
  switch (r->vr_style) {
  case RS_SINGLE :
  case RS_BASEUP :
    return Expr_parmEvalI(r->vr_left,scope,lsb,0);
  case RS_MAXMIN :
    return Expr_parmEvalI(r->vr_right,scope,lsb,0);
  case RS_BASEDN :
    {
      unsigned msb,width;

      if (Expr_parmEvalI(r->vr_left,scope,&msb,0) < 0) return -1;
      if (Expr_parmEvalI(r->vr_right,scope,&width,0) < 0) return -1;
      *lsb = msb + 1 - width;
      return 0;
    }
  case RS_AUTO :
    *lsb = 0;
    return 0;
  }
  return 0;
}


int VRange_getSize(VRange *r,Scope *scope,unsigned *width)
{
  if (!r) {
    *width = 1;
    return 0;
  }

  switch (r->vr_style) {
  case RS_SINGLE :
    *width = 1;
    break;
  case RS_BASEUP :
  case RS_BASEDN :
    if (Expr_parmEvalI(r->vr_right,scope,width,0) < 0)
      return -1;
    break;
  case RS_MAXMIN :
    {
      unsigned msb,lsb;

      if (Expr_parmEvalI(r->vr_left,scope,&msb,0) < 0) return -1;
      if (Expr_parmEvalI(r->vr_right,scope,&lsb,0) < 0) return -1;
      *width = msb - lsb + 1;
    }
    break;
  case RS_AUTO :
    *width =  Expr_getCollapsedBitSize(r->vr_left,scope);
    return 0;
  }

  return 0;
}


/*****************************************************************************
 *
 * Create a NameExpr object
 *
 * Parameters:
 *     name		Name to associate with exression (or null)
 *     e		Expression to name 
 *
 *****************************************************************************/
NameExpr *new_NameExpr(const char *name,Expr *e)
{
  NameExpr *ne = (NameExpr*) malloc(sizeof(NameExpr));

  ne->ne_name = name ? strdup(name) : 0;
  ne->ne_expr = e;

  return ne;
}

/*****************************************************************************
 *
 * Evaluate a parameter expression when expecting an integer value.
 *
 * Parameters:
 *     e		Expression to evaluate
 *     scope		Scope for variable lookup
 *     n		Evaluated value
 *
 * Returns:		Non-zero on error
 *
 * This function calls Expr_parmEval and tries to convert it to an integer.
 * It returns non-zero if either of these steps fail.
 *
 *****************************************************************************/
int Expr_parmEvalI(Expr *e,Scope *scope,unsigned *n,parmevflags_t flags)
{
  Value *r = Expr_parmEval(e,scope,flags);
  int rcode = 0;
 
  if (!r) return -1;
  if (Value_toInt(r, n) < 0) {
    rcode = -1;
  }

  delete_Value(r);
  return rcode;
}


/*****************************************************************************
 *
 * Evaluate a parameter expression.
 *
 * Parameters:
 *     e		Expression to evaluate
 *     scope		Scope for variable lookup
 *
 * Returns:		Value value of expression (or null on error)
 *
 * This function is used to evaluate expressions that are expected to evaluate
 * to a constant.  The constant can be either a sized value or an unsized value.
 * These expressions are generally found in places such as delay values or
 * bit ranges and can contain identifiers for constant values.
 *
 *
 *****************************************************************************/
Value *Expr_parmEval(Expr *e,Scope *scope,parmevflags_t flags)
{
  Value *r;

  if (!e) return 0;

  r = new_Value(0);

  switch (e->e_type) {
  case E_NOT :
  case E_UINV :
  case E_UNEG :
  case E_UAND :
  case E_UOR :
  case E_UXOR :
  case E_UNAND :
  case E_UNOR :
  case E_UNXOR :
  case E_MUL :
  case E_DIV :
  case E_MOD :
  case E_ADD :
  case E_SUB :
  case E_RSHIFT :
  case E_LSHIFT :
  case E_ARSHIFT :
  case E_ALSHIFT :
  case E_GT :
  case E_LT :
  case E_GE :
  case E_LE :
  case E_EQ :
  case E_NE :
  case E_EQZ :
  case E_NEZ :
  case E_BAND :
  case E_BNAND :
  case E_BXOR :
  case E_BNXOR :
  case E_BOR :
  case E_BNOR :
  case E_AND :
  case E_OR :
  case E_QUEST :
  case E_CONCAT :
  case E_REPCAT :
    {
      Value *t[3];
      int i;
      OpDesc *od = OpDesc_find(e->e_type);
      int sumsize = 0;
      int maxsize = 1;

      if (!od) {
	errorFile(Place_getCurrent(),ERR_IE_NOOP);
	goto abortEval;
      }

      for (i = 0;i < 3;i++) {
	if (e->e.opr[i]) {
	  t[i] = Expr_parmEval(e->e.opr[i],scope,flags);
	  if (!t[i]) goto abortEval;
	  if (Value_nbits(t[i]) > maxsize) maxsize = Value_nbits(t[i]);
	  sumsize += Value_nbits(t[i]);
	} else
	  t[i] = 0;
      }

      if ((od->od_nopr == 1 && !t[1])
	  || (od->od_nopr == 2 && (!t[0] || !t[1]))
	  || (od->od_nopr == 3 && (!t[0] || !t[1] || !t[2]))
	  ) {
	return 0;
      }

      switch (od->od_outSize) {
      case OS_MAX : Value_resize(r,maxsize); break;
      case OS_SUM : Value_resize(r,sumsize); break;
      default :	    Value_resize(r,1); break;
      }

      if ((*od->od_opfunc)(r,t[0],t[1],t[2]) != 0) {
	errorFile(Place_getCurrent(),ERR_BADOP,od->od_text);
	goto abortEval;
      }

      for (i = 0;i < 3;i++) {
      	if (t[i]) delete_Value(t[i]);
      }
    }
    break;
  case E_DELAY :
  case E_AT :			/* Illegal parameter expressions */
  case E_COND :
  case E_POSEDGE :
  case E_NEGEDGE :
  case E_EVENTOR :
  case E_TASK :
  case E_VECTORP :
  case E_VECTORN :
  case E_RANGE :
    errorFile(Place_getCurrent(),ERR_BADCONSTOP);
    goto abortEval;
    break;
  case E_LITERAL :		/* Look up as parameter */
    {
      Value *v;

      if (!scope)
	goto abortEval;

      /*
       * If SPECPARM flag is set, try looking up this literal as a specify parameter.
       */
      if ((flags & PEF_SPECPARM)) {
	ModuleInst *mi = Scope_getModuleInst(scope);
	ModuleDecl *m = ModuleInst_getModDecl(mi);
	Specify *spec = ModuleDecl_getSpecify(m);
	int n;

	if (Specify_getSpecParm(spec,Expr_getLitName(e),mi, &n) >= 0) {
	  v =  new_Value(SSWORDSIZE);
	  Value_convertI(v,n);
	  return v;
	}
      }

      v = Scope_findParm(scope,Expr_getLitName(e));
      if (v) {
	Value_reinit(r,Value_nbits(v));
	Value_copy(r,v);
      } else {
	errorFile(Place_getCurrent(),ERR_NOTPARM,Expr_getLitName(e));
	goto abortEval;
      }
    }
    break;
  case E_NUMBER :
  case E_HEX :
  case E_REAL :
    Value_reinit(r,Value_nbits(e->e.snum));
    Value_copy(r,e->e.snum);
    break;
  }

  return r;

 abortEval:
  delete_Value(r);
  return 0;
}

/*****************************************************************************
 *
 * Find the bit-size of a vector expression.
 *
 * Parameters:
 *     e		Expression to analyze.
 *     scope		Scope for variable lookup
 *
 * Returns:		Bit-size of expression.
 *
 * This function takes an expression like a[3] or m[3][5] and determines its
 * bit size.  Note that the meaning of something like a[3] depends on whether
 * a is a simple reg/net or a memory.
 *
 *****************************************************************************/
static int Expr_vectorGetSize(Expr *e, Scope *scope)
{
  unsigned size;
  Net *n;
  VRange *addr, *bits;

  if (Expr_decodeVector(e,scope, &n, &addr, &bits) < 0) {
    return 0;
  }

  if (bits) {
    if (VRange_getSize(bits, scope, &size) < 0)
      size = 0;
  } else
    size = Net_nbits(n);
    

  if (addr) delete_VRange(addr, 0);
  if (bits) delete_VRange(bits, 0);
 
  return size;
}

static Value *Expr_vectorGenerate(Expr *e, int nbits, Scope *scope, CodeBlock *cb)
{
  unsigned width;
  Value *src_value, *ret_value, *nLsb;
  VRange *addr, *bits;
  Net *n;

  if (Expr_decodeVector(e, scope, &n, &addr, &bits) < 0) {
    return 0;
  }

  if (addr) {
    Value *nAddr;

    src_value = new_Value(Net_nbits(n));
    nAddr = Expr_generate(VRange_getLsb(addr),SSWORDSIZE,scope,cb);
    BCMemFetch_init(CodeBlock_nextEmpty(cb),n,nAddr, src_value);
  } else
    src_value = Net_getValue(n);


  if (bits) {
    if (VRange_getSize(bits,scope,&width) < 0)
      return 0;

    nLsb = Expr_generate(VRange_getLsb(bits),SSWORDSIZE,scope,cb);
  } else {
    nLsb = 0;
    width = Net_nbits(n);
  }

  ret_value = new_Value(imax(nbits,width));
  Value_zero(ret_value);

#if 0
  printf("BCCopyRange_generate: %s width=%d bits=(%p) ",Net_getName(n),width,bits);
  VRange_print(bits,stdout);
  printf("\n");
#endif

  BCCopyRange_init(CodeBlock_nextEmpty(cb),ret_value,0,src_value,nLsb,width);

  if (bits) delete_VRange(bits, 0);
  if (addr) delete_VRange(addr, 0);

  return ret_value;
}


/*****************************************************************************
 *
 * Get the required bit size of an expression.
 *
 * Parameters:
 *     e		Expression to check.
 *     scope		Scope for variable lookup
 *
 *****************************************************************************/
int Expr_getBitSize(Expr *e,Scope *scope)
{
  if (!e) return 0;

  switch (e->e_type) {
  case E_NOT :
  case E_UINV :
  case E_UNEG :
  case E_UAND :
  case E_UOR :
  case E_UXOR :
  case E_UNAND :
  case E_UNOR :
  case E_UNXOR :
  case E_MUL :
  case E_DIV :
  case E_MOD :
  case E_ADD :
  case E_SUB :
  case E_RSHIFT :
  case E_LSHIFT :
  case E_ARSHIFT :
  case E_ALSHIFT :
  case E_GT :
  case E_LT :
  case E_GE :
  case E_LE :
  case E_EQ :
  case E_NE :
  case E_EQZ :
  case E_NEZ :
  case E_BAND :
  case E_BNAND :
  case E_BXOR :
  case E_BNXOR :
  case E_BOR :
  case E_BNOR :
  case E_AND :
  case E_OR :
  case E_QUEST :
    {
      int i;
      int size = 0;
      OpDesc *od = OpDesc_find(e->e_type);

      if (!od) {
	errorFile(Place_getCurrent(),ERR_IE_NOOP);
	return 0;
      }

      for (i = 0;i < 3;i++) {
	int s;

	if (!e->e.opr[i]) continue;
	s = Expr_getBitSize(e->e.opr[i], scope);
	if (s == 0) return 0;			/* There was a problem with a sub-expression */
	if (s > size) size = s;
      }
      return size;
    }
  case E_CONCAT :
    {
      int i;
      int size = 0;
      OpDesc *od = OpDesc_find(e->e_type);

      if (!od) {
	errorFile(Place_getCurrent(),ERR_IE_NOOP);
	return 0;
      }

      for (i = 0;i < 3;i++) {
	int s;

	if (!e->e.opr[i]) continue;
	s = Expr_getBitSize(e->e.opr[i], scope);
	if (s == 0) return 0;			/* There was a problem with a sub-expression */
	size += s;
      }
      return size;
    }
    break;
  case E_REPCAT :
    {
      int reps,size;

      if (Expr_parmEvalI(e->e.opr[0], scope,&reps,0) < 0) return 0;
      size = Expr_getBitSize(e->e.opr[1],scope);
      return reps*size;
    }
    break;
  case E_AT :					/* Not allowed in regular expressions */
  case E_COND :
  case E_DELAY :
  case E_POSEDGE :
  case E_NEGEDGE :
  case E_EVENTOR :
    errorFile(Place_getCurrent(),ERR_BADXOP);
    return 0;
    break;
  case E_RANGE :
  case E_VECTORP :
  case E_VECTORN :
    return Expr_vectorGetSize(e, scope);
  case E_LITERAL :
    {
      Net *n =  Scope_findNet(scope, Expr_getLitName(e),0);
      if (n) {
	return Value_nbits(Net_getValue(n));
      } else {
	errorFile(Place_getCurrent(),ERR_NOTDEF,Expr_getLitName(e));
	return 0;
      }
    }
    break;
  case E_TASK :
    {
      UserTask *ut = ModuleInst_findTask(Scope_getModuleInst(scope),Expr_getLitName(e));
      if (ut) {
	return UserTask_nbits(ut);
      }
    }
  case E_NUMBER :
    return SSWORDSIZE;
  case E_HEX :
    return Value_nbits(e->e.snum);
  case E_REAL :
    return SSREALSIZE;
  }

  return 0;
}

/*****************************************************************************
 *
 * Get the required bit size of an expression except that operators that
 * reduce the bit size to a single bit return a bit size of 1.
 *
 * Parameters:
 *     e		Expression to check.
 *     scope		Scope for varaible lookup
 *
 *****************************************************************************/
int Expr_getCollapsedBitSize(Expr *e,Scope *scope)
{
  if (!e) return 0;

  switch (e->e_type) {
  case E_NOT :
  case E_GT :
  case E_LT :
  case E_GE :
  case E_LE :
  case E_EQ :
  case E_NE :
  case E_EQZ :
  case E_NEZ :
  case E_AND :
  case E_OR :
    return 1;
  case E_UINV :
  case E_UNEG :
  case E_UAND :
  case E_UOR :
  case E_UXOR :
  case E_UNAND :
  case E_UNOR :
  case E_UNXOR :
  case E_MUL :
  case E_DIV :
  case E_MOD :
  case E_ADD :
  case E_SUB :
  case E_RSHIFT :
  case E_LSHIFT :
  case E_ARSHIFT :
  case E_ALSHIFT :
  case E_BAND :
  case E_BNAND :
  case E_BXOR :
  case E_BNXOR :
  case E_BOR :
  case E_BNOR :
  case E_QUEST :
    {
      int i;
      int size = 0;
      OpDesc *od = OpDesc_find(e->e_type);

      if (!od) {
	errorFile(Place_getCurrent(),ERR_IE_NOOP);
	return 0;
      }

      for (i = 0;i < 3;i++) {
	int s;

	if (!e->e.opr[i]) continue;
	s = Expr_getCollapsedBitSize(e->e.opr[i],scope);
	if (s == 0) return 0;			/* There was a problem with a sub-expression */
	if (s > size) size = s;
      }
      return size;
    }
  case E_CONCAT :
    {
      int i;
      int size = 0;
      OpDesc *od = OpDesc_find(e->e_type);

      if (!od) {
	errorFile(Place_getCurrent(),ERR_IE_NOOP);
	return 0;
      }

      for (i = 0;i < 3;i++) {
	int s;

	if (!e->e.opr[i]) continue;
	s = Expr_getCollapsedBitSize(e->e.opr[i],scope);
	if (s == 0) return 0;			/* There was a problem with a sub-expression */
	size += s;
      }
      return size;
    }
    break;
  case E_REPCAT :
    {
      int reps,size;

      if (Expr_parmEvalI(e->e.opr[0],scope,&reps,0) < 0) return 0;
      size = Expr_getCollapsedBitSize(e->e.opr[1],scope);
      return reps*size;
    }
    break;
  case E_AT :					/* Not allowed in regular expressions */
  case E_COND :
  case E_DELAY :
  case E_POSEDGE :
  case E_NEGEDGE :
  case E_EVENTOR :
    errorFile(Place_getCurrent(),ERR_BADXOP);
    return 0;
    break;
  case E_RANGE :
  case E_VECTORP :
  case E_VECTORN :
    return Expr_vectorGetSize(e, scope);
  case E_LITERAL :
    {
      Net *n =  Scope_findNet(scope,Expr_getLitName(e),0);
      if (n) {
	return Value_nbits(Net_getValue(n));
      } else {
	errorFile(Place_getCurrent(),ERR_NOTDEF,Expr_getLitName(e));
	return 0;
      }
    }
    break;
  case E_TASK :
    {
      UserTask *ut = ModuleInst_findTask(Scope_getModuleInst(scope),Expr_getLitName(e));
      if (ut) {
	return UserTask_nbits(ut);
      }
    }
  case E_NUMBER :
  case E_REAL :
    return SSREALSIZE;
  case E_HEX :
    return Value_nbits(e->e.snum);
  }

  return 0;
}

/*****************************************************************************
 *
 * Generate byte code to evaluate an expression in the natural bit-size of e.
 *
 * Parameters:
 *     e		Expression for which to generate byte code
 *     scope		Scope to use for variables
 *     cb		Code block in which to write bytecode
 *
 * Returns:		Value with value of expression
 *
 *****************************************************************************/
Value *Expr_generateS(Expr *e,Scope *scope,CodeBlock *cb)
{
  int nbits = Expr_getBitSize(e, scope);
  if (nbits <= 0) return 0;
  return Expr_generate(e, nbits, scope, cb);
}


/*****************************************************************************
 *
 * Generate byte code to evaluate an expression
 *
 * Parameters:
 *     e		Expression for which to generate byte code
 *     nbits		Bitsize of expression
 *     Scope		Scope to use for variables
 *     cb		Code block in which to write bytecode
 *
 * Returns:		Value with value of expression
 *
 *****************************************************************************/
Value *Expr_generate(Expr *e,int nbits,Scope *scope,CodeBlock *cb)
{
  if (!e) return 0;

  switch (e->e_type) {
  case E_NOT :
  case E_UINV :
  case E_UNEG :
  case E_UAND :
  case E_UOR :
  case E_UXOR :
  case E_UNAND :
  case E_UNOR :
  case E_UNXOR :
  case E_MUL :
  case E_DIV :
  case E_MOD :
  case E_ADD :
  case E_SUB :
  case E_RSHIFT :
  case E_LSHIFT :
  case E_ARSHIFT :
  case E_ALSHIFT :
  case E_GT :
  case E_LT :
  case E_GE :
  case E_LE :
  case E_EQ :
  case E_NE :
  case E_EQZ :
  case E_NEZ :
  case E_BAND :
  case E_BNAND :
  case E_BXOR :
  case E_BNXOR :
  case E_BOR :
  case E_BNOR :
  case E_AND :
  case E_OR :
  case E_QUEST :
    {
      int i;
      OpDesc *od = OpDesc_find(e->e_type);
      Value *temp_s[3];
      Value *lhs;
      int has_real = 0;

      if (!od) return 0;

      for (i = 0;i < 3;i++) {
	if (e->e.opr[i]) {
	  temp_s[i] = Expr_generate(e->e.opr[i],nbits,scope,cb);
	  if (Value_isReal(temp_s[i])) {
	    has_real = 1;
	  }
	} else
	  temp_s[i] = 0;
      }

      /*
       * Generate byte code for this operations.  If the operands are less than or
       * equal to the native word size on this machine, use the more efficient
       * version optimized for single word operands if it is available.
       */
      lhs = new_Value(nbits);
      if (has_real && od->od_f_opfunc) {
	BCOpr_init(CodeBlock_nextEmpty(cb),od->od_f_opfunc,lhs,temp_s[0],temp_s[1],temp_s[2]);
	if (od->od_outSize == OS_MAX) {
	  lhs->flags |= SF_REAL;
	}
      } else if (nbits <= SSWORDSIZE && od->od_w_opfunc)
	BCOpr_init(CodeBlock_nextEmpty(cb),od->od_w_opfunc,lhs,temp_s[0],temp_s[1],temp_s[2]);
      else
	BCOpr_init(CodeBlock_nextEmpty(cb),od->od_opfunc,lhs,temp_s[0],temp_s[1],temp_s[2]);

      return lhs;
    }
    break;
  case E_CONCAT :
    {
      int i;
      OpDesc *od = OpDesc_find(e->e_type);
      Value *temp_s[3];
      Value *lhs;

      if (!od) return 0;

      for (i = 0;i < 3;i++) {
	if (e->e.opr[i]) {
	  temp_s[i] = Expr_generateS(e->e.opr[i],scope,cb);
	}else
	  temp_s[i] = 0;
      }
      lhs = new_Value(nbits);
      BCOpr_init(CodeBlock_nextEmpty(cb),od->od_opfunc,lhs,temp_s[0],temp_s[1],temp_s[2]);

      return lhs;
    }
    break;
  case E_REPCAT :
    {
      OpDesc *od = OpDesc_find(e->e_type);
      Value *temp_s[2];
      Value *lhs;

      if (!od) return 0;

      temp_s[0] = Expr_generateS(e->e.opr[0],scope,cb);
      temp_s[1] = Expr_generateS(e->e.opr[1],scope,cb);

      lhs = new_Value(nbits);
      BCOpr_init(CodeBlock_nextEmpty(cb),od->od_opfunc,lhs,temp_s[0],temp_s[1],0);

      return lhs;
    }
    break;
  case E_AT :					/* Not allowed in regular expressions */
  case E_COND :
  case E_DELAY :
  case E_POSEDGE :
  case E_NEGEDGE :
  case E_EVENTOR :
    errorFile(Place_getCurrent(),ERR_BADXOP);
    return 0;
  case E_LITERAL :				/* Look up as parameter */
    {
      Net *n =  Scope_findNet(scope,Expr_getLitName(e),0);
      Value *lhs;

      if (!n) return 0;

      if ((Net_getType(n) & NT_P_MEMORY)) {
	errorFile(Place_getCurrent(),ERR_BADARRAYUSG,Expr_getLitName(e));
	return 0;
      }


      if (Value_nbits(Net_getValue(n)) == nbits)
	return Net_getValue(n);
      else {
	lhs = new_Value(nbits);
	BCCopy_init(CodeBlock_nextEmpty(cb),lhs,Net_getValue(n));
	return lhs;
      }
    }
    break;
  case E_RANGE :
  case E_VECTORP :
  case E_VECTORN :
    return Expr_vectorGenerate(e, nbits, scope, cb);
  case E_TASK :
    {
      UserTask *ut = ModuleInst_findTask(Scope_getModuleInst(scope),Expr_getLitName(e));
      SysTaskDescript *func = SysTask_findEnt(e->e.task.name);
      void **sargs = 0;
      Value *lhs;
      int i;
      int offset = ut ? 1 : 0;

      if (e->e.task.argc+offset > 0) {
	sargs = (void**) malloc((e->e.task.argc+offset)*sizeof(void*));
	for (i = 0;i < e->e.task.argc;i++)
	  sargs[i+offset] = Expr_generateS(e->e.task.argv[i],scope,cb);
      }

      if (ut) {
	if (UserTask_getType(ut) != UTT_FUNCTION) {
	  errorFile(Place_getCurrent(),ERR_TASKASFUNC,Expr_getLitName(e));
	  return 0;
	}
	sargs[0] = lhs = new_Value(UserTask_nbits(ut));
	if (UserTask_isAuto(ut))
	  UserTask_generateInlineCall(ut,sargs,cb);
	else
	  UserTask_generateCall(ut,sargs,cb);
      } else if (func) {

	if (e->e.task.argc < func->st_minArgs || e->e.task.argc > func->st_maxArgs) {
	  errorFile(Place_getCurrent(),ERR_TASKARGS,e->e.task.name);
	  return 0;
	}

	lhs = new_Value(nbits);
	BCTask_init(CodeBlock_nextEmpty(cb),func->st_func,0,lhs,e->e.task.argc,sargs);
      } else {
	errorFile(Place_getCurrent(),ERR_NOTASK,e->e.task.name);
	lhs = 0;
      }
      return lhs;
    }
    break;
  case E_NUMBER :
  case E_HEX :
    {
      if (Value_nbits(e->e.snum) == nbits)
	return e->e.snum;
      else {						/* Resize constant if not of requested size */ 
	Value *lhs;

	lhs = new_Value(nbits);
	Value_zero(lhs);
	Value_copyRange(lhs,0,e->e.snum,Value_nbits(e->e.snum)-1,0);
	return lhs;
      }
    }
    break;
  case E_REAL :
    {
      if (Value_nbits(e->e.snum) == nbits)
	return e->e.snum;
      else {						/* Resize constant if not of requested size */ 
	Value *lhs;

	lhs = new_Value(nbits);
	Value_zero(lhs);
	Value_copyRange(lhs,0,e->e.snum,Value_nbits(e->e.snum)-1,0);
	lhs->flags &= ~SF_REAL;
	return lhs;
      }
    }
    break;
  }

  return 0;
}

/*****************************************************************************
 *
 * Get the set of nets that are read by an expression
 *
 *****************************************************************************/
void Expr_getReaders(Expr*e, Scope *scope, PHash *H)
{
  if (!e) return;

  switch (e->e_type) {
  case E_NOT :
  case E_UINV :
  case E_UNEG :
  case E_UAND :
  case E_UOR :
  case E_UXOR :
  case E_UNAND :
  case E_UNOR :
  case E_UNXOR :
  case E_MUL :
  case E_DIV :
  case E_MOD :
  case E_ADD :
  case E_SUB :
  case E_RSHIFT :
  case E_LSHIFT :
  case E_ARSHIFT :
  case E_ALSHIFT :
  case E_GT :
  case E_LT :
  case E_GE :
  case E_LE :
  case E_EQ :
  case E_NE :
  case E_EQZ :
  case E_NEZ :
  case E_BAND :
  case E_BNAND :
  case E_BXOR :
  case E_BNXOR :
  case E_BOR :
  case E_BNOR :
  case E_AND :
  case E_OR :
  case E_QUEST :
  case E_CONCAT :
  case E_REPCAT :
    {
      int i;
      OpDesc *od = OpDesc_find(e->e_type);

      if (!od) break;

      for (i = 0;i < 3;i++) {
	if (e->e.opr[i])
	  Expr_getReaders(e->e.opr[i], scope, H);
      }
    }
    break;
  case E_AT :					/* Not allowed in regular expressions */
  case E_COND :
  case E_DELAY :
  case E_POSEDGE :
  case E_NEGEDGE :
  case E_EVENTOR :
    break;
  case E_VECTORP :
  case E_VECTORN :
  case E_RANGE :
    Expr_getReaders(e->e.opr[0], scope, H);
    Expr_getReaders(e->e.opr[1], scope, H);
    if (e->e.opr[2])
      Expr_getReaders(e->e.opr[2], scope, H);
    break;
  case E_LITERAL :				/* Look up as parameter */
    if (scope) {
      Net *n;

      n =  Scope_findNet(scope,Expr_getLitName(e),0);

      if (!n) break;

      PHash_insert(H, n, n);
    } else {
      PHash_insert(H, e, e);
    }
    break;
  case E_TASK :
    {
      if (e->e.task.argc > 0) {
	int i;

	for (i = 0;i < e->e.task.argc;i++)
	  Expr_getReaders(e->e.task.argv[i], scope, H);
      }
    }
    break;
  case E_NUMBER :
  case E_HEX :
  case E_REAL :
    break;
  }
}

/*****************************************************************************
 *
 * Get the set of literals that are read by an expression 
 *
 *****************************************************************************/
void Expr_getStaticReaders(Expr*e, SHash *H)
{
  if (!e) return;

  switch (e->e_type) {
  case E_NOT :
  case E_UINV :
  case E_UNEG :
  case E_UAND :
  case E_UOR :
  case E_UXOR :
  case E_UNAND :
  case E_UNOR :
  case E_UNXOR :
  case E_MUL :
  case E_DIV :
  case E_MOD :
  case E_ADD :
  case E_SUB :
  case E_RSHIFT :
  case E_LSHIFT :
  case E_ARSHIFT :
  case E_ALSHIFT :
  case E_GT :
  case E_LT :
  case E_GE :
  case E_LE :
  case E_EQ :
  case E_NE :
  case E_EQZ :
  case E_NEZ :
  case E_BAND :
  case E_BNAND :
  case E_BXOR :
  case E_BNXOR :
  case E_BOR :
  case E_BNOR :
  case E_AND :
  case E_OR :
  case E_QUEST :
  case E_CONCAT :
  case E_REPCAT :
    {
      int i;
      OpDesc *od = OpDesc_find(e->e_type);

      if (!od) break;

      for (i = 0;i < 3;i++) {
	if (e->e.opr[i])
	  Expr_getStaticReaders(e->e.opr[i], H);
      }
    }
    break;
  case E_AT :					/* Not allowed in regular expressions */
  case E_COND :
  case E_DELAY :
  case E_POSEDGE :
  case E_NEGEDGE :
  case E_EVENTOR :
    break;
  case E_VECTORP :
  case E_VECTORN :
  case E_RANGE :
    Expr_getStaticReaders(e->e.opr[0], H);
    Expr_getStaticReaders(e->e.opr[1], H);
    if (e->e.opr[2])
      Expr_getStaticReaders(e->e.opr[2], H);
    break;
  case E_LITERAL :				/* Look up as parameter */
    if (e->e.literal.ishlit) {
      errorFile(Place_getCurrent(),ERR_BADHIER,e->e.literal.name);
      break;
    }
    SHash_insert(H, e->e.literal.name, e);
    break;
  case E_TASK :
    {
      if (e->e.task.argc > 0) {
	int i;

	for (i = 0;i < e->e.task.argc;i++)
	  Expr_getStaticReaders(e->e.task.argv[i], H);
      }
    }
    break;
  case E_NUMBER :
  case E_HEX :
  case E_REAL :
    break;
  }
}

/*****************************************************************************
 *
 * Expand a concat expression into a list of expressions LSB first
 *
 * Parameters:
 *     e		Concat expression to expand
 *     scope		Scope for variable lookup
 *     clist		List of expressions
 *
 * Note: It is assumed that clist is initialized to empty before calling
 * this function.
 *
 *****************************************************************************/
void Expr_expandConcat(Expr *e,Scope *scope,List *clist)
{
  if (Expr_type(e) == E_CONCAT) {
    Expr_expandConcat(e->e.opr[1], scope, clist);
    Expr_expandConcat(e->e.opr[0], scope, clist);
  } else if (Expr_type(e) == E_REPCAT) {
    unsigned n;
    unsigned i;

    if (!scope) return;		/* repcats are ignored if no module instance given */

    if (Expr_parmEvalI(e->e.opr[0], scope,&n,0) < 0)
      return;

    for (i = 0;i < n;i++) {
      Expr_expandConcat(e->e.opr[1],scope,clist);
    }
  } else {
    List_addToTail(clist,e);
  }
}


/*****************************************************************************
 *
 * Convert a trigger expression into a list of triggers.
 *
 * Parameters:
 *     trigger		Trigger expression
 *     triggerList	Output list of triggers.
 *
 *****************************************************************************/
void Expr_makeTriggerList(Expr *trigger,List *triggerList)
{
  if (!trigger) return;

  if (trigger->e_type == E_EVENTOR) {
    Expr_makeTriggerList(trigger->e.opr[0],triggerList);
    Expr_makeTriggerList(trigger->e.opr[1],triggerList);
    return;
  }

  List_addToTail(triggerList,trigger);
}

/*****************************************************************************
 *
 * Get the default trigger from a hash table containing all the readers.
 *
 *****************************************************************************/
Trigger *Expr_getDefaultTriggerFromSet(PHash *P,Circuit *c)
{
  List posedges;
  List negedges;
  HashElem *he;
  Trigger *t;

  List_init(&posedges);
  List_init(&negedges);

  for (he = Hash_first(P);he;he = Hash_next(P, he)) {
    Net *n = (Net*) HashElem_obj(he);
    List_addToTail(&posedges, n);
    if (Net_nbits(n) == 1)
      List_addToTail(&negedges, n);
  }

  t = Circuit_getTrigger(c, &posedges, &negedges);

  List_uninit(&posedges);
  List_uninit(&negedges);

  return t;
}

/*****************************************************************************
 *
 * Get the default trigger for an expression (one that fires for any input)
 *
 * Parameters:
 *     e		Expression to examine
 *     scope		Scope to use for variable lookup
 *
 * Returns:		Trigger for the specified readerlist.
 *
 *****************************************************************************/
Trigger *Expr_getDefaultTrigger(Expr *e,Scope *scope)
{
  PHash P;
  Trigger *t;

  if (!e) {
    errorFile(Place_getCurrent(),ERR_NONEXPCTL);
    return 0;
  }

  PHash_init(&P);
  Expr_getReaders(e, scope, &P);
  t = Expr_getDefaultTriggerFromSet(&P, &vgsim.vg_circuit);
  PHash_uninit(&P);

  return t;
}

/*****************************************************************************
 *
 * Convert a trigger expression into a trigger object.
 *
 * Parameters:
 *     trigger		Trigger expression.
 *     scope		Scope to use for variable lookup
 *     stat		Statment blocking condition modifies (used for "@(*)")
 *
 * Returns:		Trigger object or null on error.
 *
 *****************************************************************************/
Trigger *Expr_getTrigger(Expr *trigger,Scope *scope, StatDecl *stat)
{
  List triggerList;
  List posedges;
  List negedges;
  ListElem *le;
  Trigger *t = 0;

  /*
   * If "trigger" is null, then this is a @(*) trigger and we need to get the
   * event list from the modified statement.
   */
  if (!trigger) {
    PHash P;
    HashElem *he;

    if (!stat) {
      errorFile(Place_getCurrent(),ERR_NONSTATCTL);
      return 0;
    }

    PHash_init(&P);
    List_init(&posedges);
    List_init(&negedges);

    StatDecl_getReaders(stat, scope, &P);

    for (he = Hash_first(&P);he;he = Hash_next(&P, he)) {
      Net *n = (Net*) HashElem_obj(he);
      List_addToTail(&posedges, n);
      if (Net_nbits(n) == 1)
	List_addToTail(&negedges, n);
    }

    t = Circuit_getTrigger(&vgsim.vg_circuit,&posedges,&negedges);

    PHash_uninit(&P);
    List_uninit(&posedges);
    List_uninit(&negedges);
    return t;
  }

  /*
   * Normal event trigger case starts here.
   */
  List_init(&triggerList);
  List_init(&posedges);
  List_init(&negedges);

  Expr_makeTriggerList(trigger,&triggerList);

  for (le = List_first(&triggerList);le;le = List_next(&triggerList,le)) {
    Expr *e = (Expr*) ListElem_obj(le);
    Net *n = 0;
    const char *name = 0;
    transtype_t transition = TT_NONE;

    if (Expr_type(e) == E_LITERAL) {
      transition = TT_EDGE;
      name = Expr_getLitName(e);
    } else if (Expr_type(e) == E_POSEDGE && Expr_type(e->e.opr[1]) == E_LITERAL) {
      transition = TT_POSEDGE;
      name = Expr_getLitName(e->e.opr[1]);
    } else if (Expr_type(e) == E_NEGEDGE && Expr_type(e->e.opr[1]) == E_LITERAL) {
      transition = TT_NEGEDGE;
      name = Expr_getLitName(e->e.opr[1]);
    } else {
      errorFile(Place_getCurrent(),ERR_BADEVENT);
      goto abortGen;
    }

    n = Scope_findNet(scope,name,0);
    if (!n) {
      errorFile(Place_getCurrent(),ERR_BADEVENTNET,name);
      goto abortGen;
    }

    if (Net_nbits(n) == 1) {
      if (transition == TT_POSEDGE || transition == TT_EDGE )
	List_addToTail(&posedges,n);
      if (transition == TT_NEGEDGE || transition == TT_EDGE )
	List_addToTail(&negedges,n);
    } else {
      /*
       * We only use the 'posedge' list for multi-bit wires and use that list
       * for all transitions.
       */
      if (transition != TT_EDGE) {
	errorFile(Place_getCurrent(),ERR_BADEDGEEVENT,name);
	goto abortGen;
      }
      List_addToTail(&posedges,n);
    }
  }

  t = Circuit_getTrigger(&vgsim.vg_circuit,&posedges,&negedges);

 abortGen:
  List_uninit(&triggerList);
  List_uninit(&posedges);
  List_uninit(&negedges);

  return t;
}

/*****************************************************************************
 *
 * Convert a delay value expression into an unsigned
 *
 * Parameters:
 *     delayExpr	Delay value expression.
 *     scope		Scope to use for variable lookup.
 *     ts		Timescale of delay
 *     *delay		Output delay value
 *
 * Returns:		Non-zero on error.
 *
 *****************************************************************************/
int Expr_getDelay(Expr *delayExpr,Scope *scope,Timescale *ts, deltatime_t *delay)
{
  Value *delay_value;
  Expr *e;

  e = delayExpr->e.opr[vgsim.vg_delayType];
  if (!e)
    e = delayExpr->e.opr[DT_TYP];

  delay_value = Expr_parmEval(e, scope,0);

  if (!delay_value) return -1;

  if (Value_isReal(delay_value)) {
    real_t r;

    Value_toReal(delay_value,&r);
    r *= ts->ts_units/ts->ts_precision;
    *delay = (deltatime_t) r;
    *delay *= ts->ts_precision/vgsim.vg_timescale.ts_precision;
  } else {
    Value_toTime(delay_value,delay);
    *delay *= ts->ts_units/vgsim.vg_timescale.ts_precision;
  }

  delete_Value(delay_value);
  return 0;
}

/*****************************************************************************
 *
 * Generate bytecode for a delay expression.
 *
 * Parameters:
 *     delay		Delay value expression
 *     scope		Scope to use for variable lookup
 *     cb		CodeBlock to use
 *
 *****************************************************************************/
int Expr_generateDelay(Expr *delay, Scope *scope, CodeBlock *cb)
{
  Timescale *ts = ModuleInst_getTimescale(CodeBlock_getModuleInst(cb));
  deltatime_t idelay;

  if (Expr_getDelay(delay,scope,ts,&idelay) != 0)
    return -1;

  BCDelay_init(CodeBlock_nextEmpty(cb),idelay);

  return 0;
}

/*****************************************************************************
 *
 * Generate bytecode for a trigger expression.
 *
 * Parameters:
 *     triggerExpr 	Trigger expression
 *     scope 		Scope to use for variable lookup.
 *     cb 		CodeBlock to use.
 *     stat		Statment blocking condition modifies (used for "@(*)")
 *
 *****************************************************************************/
int Expr_generateTrigger(Expr *triggerExpr, Scope *scope, CodeBlock *cb, StatDecl *stat)
{
  Trigger *t = 0;

  t = Expr_getTrigger(triggerExpr, scope, stat);
  if (!t) return -1;

  BCTrigger_init(CodeBlock_nextEmpty(cb),t);

  return 0;
}

/*****************************************************************************
 *
 * Generate a thread suspension with wakeup either by delay or trigger
 *
 * Parameter:
 *     econd		Expression for either a delay or trigger
 *     Scope		Scope to use for variable lookup
 *     cb		CodeBlock in which to generate bytecode.
 *     stat		Statment blocking condition modifies (used for "@(*)")
 *
 *****************************************************************************/
int Expr_generateBCond(Expr *bcond, Scope *scope, CodeBlock *cb, StatDecl *stat)
{
  if (Expr_type(bcond) == E_DELAY)
    return Expr_generateDelay(bcond, scope, cb);
  else if (Expr_type(bcond) == E_AT)
    return Expr_generateTrigger(bcond->e.opr[1], scope, cb,stat);
  else
    return -1;
}

/*****************************************************************************
 *
 * Decode an expression for a literal, memory access and/or bit select.
 *
 * Parameters:
 *     e		Expression with [] operator
 *     scope		Scope to use for variable lookup
 *     *n		Net that is being accessed.
 *     *addr		Address range
 *     *bits		Bit range
 *
 *****************************************************************************/
int Expr_decodeVector(Expr *e,Scope *scope,Net **n,VRange **addr,VRange **bits)
{
  char *name = 0;

  *addr = 0;
  if (bits) *bits = 0;

  switch (Expr_type(e)) {
  case E_LITERAL :
    name = Expr_getLitName(e);
    *n =  Scope_findNet(scope, name,0);

    if (*n && (Net_getType(*n) & NT_P_MEMORY)) {
      errorFile(Place_getCurrent(),ERR_BADARRAYLHS,Expr_getLitName(e));
    }

    break;
  case E_VECTORP :
  case E_VECTORN :
  case E_RANGE :
    if (Expr_type(e->e.opr[0]) != E_LITERAL) {
      /*
       * We have multiple [].  Assume the next one in is an address, and
       * this is a bit select.
       */
      if (Expr_decodeVector(e->e.opr[0],scope,n,addr,0) < 0)
	return -1;
    } else {
      /*
       * Single level of [].  Now lets see if it is a bit select or a memory reference.
       */
      name = Expr_getLitName(e->e.opr[0]);
      *n =  Scope_findNet(scope, name,0);
      if (!*n) break;		/* Failed to find net */

      /*
       * If this is a memory, create a VRange for the address lookup.
       */
      if ((Net_getType(*n) & NT_P_MEMORY)) {
	if (e->e.opr[2]) {
	  errorFile(Place_getCurrent(),ERR_BADARRAYRNG,name);
	  return -1;
	}

	*addr = new_VRange(RS_SINGLE,e->e.opr[1],0);
	break;
      }
    }
    if (bits) {
      /*
       * We are either the second level bit select off of a memory, or a first level
       * bit select off of a net if we get here.
       */
      switch (Expr_type(e)) {
      case E_VECTORP :
	if (!e->e.opr[2])
	  *bits = new_VRange(RS_SINGLE,e->e.opr[1],0);
	else
	  *bits = new_VRange(RS_BASEUP,e->e.opr[1],e->e.opr[2]);
	break;
      case E_VECTORN :
	*bits = new_VRange(RS_BASEDN,e->e.opr[1],e->e.opr[2]);
	break;
      case E_RANGE :
	*bits = new_VRange(RS_MAXMIN,e->e.opr[1],e->e.opr[2]);
	break;
      default :
	*bits = 0;
	break;
      }
    }
    break;
  default :
    {
      char buf[STRMAX];
      sprintf(buf,"%d",Expr_type(e));
      errorFile(Place_getCurrent(),ERR_IE_BADEXP,buf);
    }
    return -1;
  }


  /*
   * A null net means we encountered an error looking up the net name.  Generate
   * an error and return -1.
   */
  if (!*n) {
    if (name) {
      errorFile(Place_getCurrent(),ERR_NOTDEF,name);
      return -1;
    } else {
      errorFile(Place_getCurrent(),ERR_IE_BADVAR,name);
      return -1;
    }
  }

  return 0;
}

int Expr_lhsGenerate(Expr *e,Scope *scope, CodeBlock *cb,Net **n,Value **nLsb,unsigned *size,Value **nAddr)
{
  VRange *addr, *bits;

  if (Expr_decodeVector(e,scope, n, &addr, &bits) < 0) {
    return -1;
  }

  if (addr) {
    if (!nAddr) {
      return -1;
    }
    *nAddr = Expr_generate(VRange_getLsb(addr),SSWORDSIZE,scope,cb);
  } else {
    if (nAddr) *nAddr = 0;
  }


  if (bits) {
    if (VRange_getSize(bits, scope,size) < 0)
      return -1;
  } else
    *size = Net_nbits(*n);

  if (bits) {
    *nLsb = Expr_generate(VRange_getLsb(bits),SSWORDSIZE,scope,cb);
  } else
    *nLsb = 0;

  if (bits) delete_VRange(bits, 0);
  if (addr) delete_VRange(addr, 0);

  return 0;
}

