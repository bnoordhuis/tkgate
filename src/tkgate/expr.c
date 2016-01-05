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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "yybasic.h"
#include "vgrammar.h"
#include "hash.h"
#include "ycmalloc.h"

struct opsym {
  int	opCode;
  char	*opSym;
};

int expr_error;
const char *expr_errsym = 0;

static int Expr_eval_aux(SHash *H,Expr *e,int *rval,EValueLookup *f,void *d);

struct opsym opsymTable[] = {
  {ADD,"+"},
  {SUB,"-"},
  {MUL,"*"},
  {POW,"**"},
  {DIV,"/"},
  {OR,"||"},
  {AND,"&&"},
  {GT,">"},
  {GE,">="},
  {LT,"<"},
  {LE,"<="},
  {EQ,"=="},
  {NE,"!="},
  {NOT,"!"},
};

static const char *findSymbol(int code)
{
  int n = sizeof(opsymTable)/sizeof(opsymTable[0]);
  int i;

  for (i = 0;i < n;i++)
    if (code == opsymTable[i].opCode)
      return opsymTable[i].opSym;
  return "?";
}

static int *intdup(int n)
{
  int *p = (int*)malloc(sizeof(int));
  *p = n;
  return p;
}

static Expr *new_Expr()
{
  Expr *E = (Expr*) malloc(sizeof(Expr));

  memset(E,0,sizeof(*E));

  return E;
}

void delete_Expr(Expr *E)
{
  if (!E) return;
  if (E->lit) free(E->lit);
  if (E->l) delete_Expr(E->l);
  if (E->r) delete_Expr(E->r);
  if (E->x) delete_Expr(E->x);
  free(E);
}


Expr *Expr_lit(const char *l)
{
  Expr *E = new_Expr();

  E->op = LITERAL;
  E->lit = strdup(l);

  return E;
}

Expr *Expr_num(int n)
{
  Expr *E = new_Expr();

  E->op = NUMBER;
  E->value = n;

  return E;
}

Expr *Expr_op(int op,Expr *l,Expr *r)
{
  Expr *E = new_Expr();

  E->op = op;
  E->l = l;
  E->r = r;

  return E;
}

Expr *Expr_op3(int op,Expr *l,Expr *r,Expr *x)
{
  Expr *E = new_Expr();

  E->op = op;
  E->l = l;
  E->r = r;
  E->x = x;

  return E;
}

Expr *Expr_case(int op,int tag)
{
  Expr *E = new_Expr();

  E->op = op;
  E->value = tag;

  return E;
}
Expr *Expr_func(const char *f,Expr *l,Expr *r)
{
  Expr *E = new_Expr();

  E->op = FUNC;
  E->lit = strdup(f);
  E->l = l;
  E->r = r;

  return E;
}

static int apply_op(int op,int a,int b,int *err)
{
  *err = 0;

  switch (op) {
  case ADD :
    return a+b;
  case SUB :
    return a-b;
  case POW :
    {
      int p = 1;
      if (b > 32) *err = EE_OVERFLOW;
      while (b-- > 0) p *= a; 
      return p;
    }
  case MUL :
    return a*b;
  case DIV :
    if (b == 0) *err = EE_DIV0;
    return a/b;
  case OR :
    return a||b;
  case AND :
    return a&&b;
  case GT :
    return a>b;
  case GE :
    return a>=b;
  case LT :
    return a<b;
  case LE :
    return a<=b;
  case EQ :
    return a==b;
  case NE :
    return a!=b;
  case NOT :
    return !b;
  case UNEG :
    return -b;
  }
  return 0;
}

static int Expr_eval_default(SHash *H,Expr *e,int *rval,EValueLookup *f,void *d)
{
  int rr;

  if (!e) return EE_NOCASE;


  if (e->op == DEFAULT) return 0;
  if (e->op != NEXT) {
    return EE_NOCASE;
  }

  rr = Expr_eval_default(H,e->l,rval,f,d);
  if (rr == EE_NOCASE)
    rr = Expr_eval_default(H,e->r,rval,f,d);
  else if (!rr)
    rr = Expr_eval_aux(H,e->r,rval,f,d);

  return rr;
}

static int Expr_eval_switch(int s,SHash *H,Expr *e,int *rval,EValueLookup *f,void *d)
{
  int rr;

  if (!e) return EE_NOCASE;


  if (e->op == CASE && e->value == s) return 0;
  if (e->op != NEXT) {
    return EE_NOCASE;
  }

  rr = Expr_eval_switch(s,H,e->l,rval,f,d);
  if (rr == EE_NOCASE)
    rr = Expr_eval_switch(s,H,e->r,rval,f,d);
  else if (!rr)
    rr = Expr_eval_aux(H,e->r,rval,f,d);

  return rr;
}

static int Expr_eval_aux(SHash *H,Expr *e,int *rval,EValueLookup *f,void *d)
{
  if (!e) {
    *rval = 0;
    return 0;
  }

  switch (e->op) {
  case LITERAL :
    {
      int *h = 0;

      if (H)
	h = SHash_find(H,e->lit);

      if (!h) {
	expr_errsym = e->lit;
	return EE_NOTDEF;
      }

      *rval = *h;
      return 0;
    }
    break;
  case FUNC :
    {
      int nl = 0,nr = 0,nx = 0,rr;

      if (strcmp(e->lit,"bits") == 0
	  || strcmp(e->lit,"inv") == 0
	  || strcmp(e->lit,"num") == 0) {
	if (e->l->op != LITERAL) return  EE_NOTLIT;
	if (!f) {
	  expr_errsym = e->l->lit;
	  return EE_NOTDEF;
	}
	return (*f)(e->lit,e->l->lit,d,rval);
      }

      if (e->l && (rr = Expr_eval_aux(H,e->l,&nl,f,d))) return rr;
      if (e->r && (rr = Expr_eval_aux(H,e->r,&nr,f,d) < 0)) return rr;
      if (e->x && (rr = Expr_eval_aux(H,e->x,&nx,f,d) < 0)) return rr;

      if (strcmp(e->lit,"log") == 0) {
	int n = 0;
	int a = 0;
	while (nl) {
	  if ((nl != 1) && (nl & 1)) a = 1;
	  nl >>= 1;
	  n++;
	}
	n += a;
	*rval = n;
	return 0;
      } else {
	expr_errsym = e->lit;
	return EE_BADFUNC;
      }
    }
    break;
  case NUMBER :
    *rval = e->value;
    return 0;
    break;
  case ADD :
  case SUB :
  case MUL :
  case POW :
  case DIV :
  case OR :
  case AND :
  case GT :
  case GE :
  case LT :
  case LE :
  case EQ :
  case NE :
    {
      int nl,nr,rr,err;

      if (!e->l || !e->r) return EE_INTERNAL;
      if ((rr = Expr_eval_aux(H,e->l,&nl,f,d))) return rr;
      if ((rr = Expr_eval_aux(H,e->r,&nr,f,d))) return rr;
      
      *rval = apply_op(e->op,nl,nr,&err);
      return err;
    }
    break;
  case NOT :
  case UNEG :
    {
      int nr,rr,err;

      if (!e->r) return EE_INTERNAL;
      if ((rr = Expr_eval_aux(H,e->r,&nr,f,d))) return rr;

      *rval = apply_op(e->op,0,nr,&err);
      return err;
    }
    break;
  case NEXT :
    {
      int rr;

      if (e->l && (rr = Expr_eval_aux(H,e->l,rval,f,d))) return rr;
      if (e->r && (rr = Expr_eval_aux(H,e->r,rval,f,d))) return rr;
      return 0;
    }
    break;
  case ASGN :
    {
      int rr;
      int *h;

      if (!H) return EE_INTERNAL;
      if (!e->l || e->l->op != LITERAL) return EE_INTERNAL;
      if ((rr = Expr_eval_aux(H,e->r,rval,f,d))) return rr;

      if ((h = SHash_find(H,e->l->lit))) {
	SHash_remove(H,e->l->lit);
	free(h);
      }
      SHash_insert(H,e->l->lit,intdup(*rval));
      return 0;
    }
    break;
  case IF :
    {
      int rr,c;
 
      if ((rr = Expr_eval_aux(H,e->l,&c,f,d))) return rr;

      if (c) {
	if ((rr = Expr_eval_aux(H,e->r,rval,f,d))) return rr;
      } else {
	if ((rr = Expr_eval_aux(H,e->x,rval,f,d))) return rr;
      }

      return 0;
    }
    break;
  case BREAK :
    *rval = 0;
    return EE_BREAK;
  case RETURN :
    {
      int rr;

      if ((rr = Expr_eval_aux(H,e->l,rval,f,d))) return rr;
      return EE_RETURN;
    }
    break;
  case DEFAULT :
  case CASE :
    return 0;
  case SWITCH :
    {
      int rr,s;

      if ((rr = Expr_eval_aux(H,e->l,&s,f,d))) return rr;

      rr = Expr_eval_switch(s,H,e->r,rval,f,d);
      if (rr == EE_OK) return 0;
      if (rr == EE_BREAK) return 0;
      if (rr == EE_RETURN) return rr;
      if (rr != EE_NOCASE) return rr;

      rr = Expr_eval_default(H,e->r,rval,f,d);
      if (rr == EE_BREAK) return 0;
      if (rr == EE_NOCASE) return 0;
      return rr;
    }
  }
  return EE_INTERNAL;
}

int Expr_eval(Expr *e,int *rval,EValueLookup *f,void *d)
{
  if (e && e->op == NEXT) {
    int ec;
    SHash *H = new_SHash_noob();

    ec = Expr_eval_aux(H,e,rval,f,d);
    delete_SHash(H);

    if (ec == EE_RETURN) ec = EE_OK; 
    return ec;
  } else
    return Expr_eval_aux(0,e,rval,f,d);


}

/*****************************************************************************
 *
 * Print an expression to a string a la sprintf
 *
 * Parameters:
 *
 *     s		Buffer to which to print expression
 *     n		Size of the string buffer.
 *     e		Expression to print
 *
 * Returns:		Number of characters printed on success, -1 on failure.
 *
 * Note, only basic expression are supported by this function.
 *
 *****************************************************************************/
int Expr_sprint(char *s,int n,Expr *e)
{
  int l;
  char *start = s;

  if (!e) return -1;

  switch (e->op) {
  case LITERAL :
    if (strlen(e->lit) >= n)
      return -1;

    strcpy(s,e->lit);
    return strlen(s);
  case FUNC :
    if (strlen(e->lit)+strlen(e->l->lit)+2 >= n)
      return -1;

    return sprintf(s,"%s(%s)",e->lit,e->l->lit);
  case NUMBER :
    {
      char buf[1024];
      sprintf(buf,"%d",e->value);
      
      if (strlen(buf) >= n)
	return -1;

      strcpy(s,buf);
      return strlen(s);
    }
    break;
  case ADD :
  case SUB :
  case MUL :
  case POW :
  case DIV :
  case OR :
  case AND :
  case GT :
  case GE :
  case LT :
  case LE :
  case EQ :
  case NE :
    if (n < 4) return -1;
    s += sprintf(s,"(");n--;
    l = Expr_sprint(s,n,e->l);
    if (l < 0) return -1;
    s += l; n -= l;
    if (n < 4) return -1;
    s += sprintf("%s",findSymbol(e->op));n -= strlen(s);
    l = Expr_sprint(s,n,e->r);
    if (l < 0) return -1;
    s += l; n -= l;
    if (n < 2) return -1;
    s += sprintf(s,")");

    return strlen(start);
  case NOT :
    if (n < 2) return -1;
    *s++ = '!'; n--; 
    l = Expr_sprint(s,n,e->l); 
    if (l < 0) return -1;
    return strlen(start);
  case UNEG :
    if (n < 2) return -1;
    *s++ = '-'; n--; 
    l = Expr_sprint(s,n,e->l); 
    if (l < 0) return -1;
    return strlen(start);
  }
  return -1;
}

int Expr_print(Expr *e)
{
  if (!e) {
    printf("; ");
    return 0;
  }

  switch (e->op) {
  case LITERAL :
    printf("%s",e->lit);
    break;
  case FUNC :
    printf("%s(%s)",e->lit,e->l->lit);
    break;
  case NUMBER :
    printf("%d",e->value);
    break;
  case ADD :
  case SUB :
  case MUL :
  case POW :
  case DIV :
  case OR :
  case AND :
  case GT :
  case GE :
  case LT :
  case LE :
  case EQ :
  case NE :
    printf("("); Expr_print(e->l);  printf("%s",findSymbol(e->op));  Expr_print(e->r);  printf(")");
    break;
  case NOT :
    printf("!");
    Expr_print(e->l); 
    break;
  case UNEG :
    printf("-");
    Expr_print(e->l); 
    break;
  case NEXT :
    printf("{");
    if (e->l) {
      Expr_print(e->l); 
    }
    if (e->r) {
      Expr_print(e->r); 
    }
    printf("}");
    break;
  case ASGN :
    printf("%s = ",e->l->lit);
    Expr_print(e->r); 
    printf("; ");
    break;
  case IF :
    printf("if (");
    Expr_print(e->l); 
    printf(")");
    Expr_print(e->r); 
    if (e->x) {
      printf(" else ");
      Expr_print(e->x); 
    }
    break;
  case BREAK :
    printf(" break;");
    break;
  case RETURN :
    printf(" return;");
    break;
  case DEFAULT :
    printf(" default :");
    break;
  case CASE :
    printf(" case %d :",e->value);
    break;
  case SWITCH :
    printf("switch (");
    Expr_print(e->l); 
    printf(")");
    Expr_print(e->r); 
  }
  return 0;
}
