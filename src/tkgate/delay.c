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
#include "tkgate.h"
#include "yybasic.h" 

void BeginDD();

#if !TKGATE_NOYYRESTART
void yyrestart(FILE*);
#endif

static int delay_def_is_init = 0;
static SHash *delay_def_table = 0;
static char **the_tech_list = 0;
static int tech_list_len = 0;

static GDelayDef *cur_dd = 0;

/*
  Add 'tech' to the list of defined technlogies.  If it is already defined,
  then do nothing.
 */
static void add_tech(const char *tech)
{
  if (!the_tech_list) {
    the_tech_list = (char**) malloc(16*sizeof(char*));
    the_tech_list[0] = strdup(tech);
    the_tech_list[1] = 0;
    tech_list_len = 1;
  } else {
    int i;
    for (i = 0;i < tech_list_len;i++)
      if (strcmp(tech,the_tech_list[i]) == 0) return;

    if ((tech_list_len+1) % 16) {
      the_tech_list = (char**)realloc(the_tech_list,(tech_list_len+17)*sizeof(char**));
    }

    the_tech_list[tech_list_len++] = strdup(tech);
    the_tech_list[tech_list_len] = 0;
  }
}

GDelayDef *new_GDelayDef(const char *tech,const char *prim)
{
  GDelayDef *dd = (GDelayDef *) malloc(sizeof(GDelayDef));

  dd->dd_tech = strdup(tech);
  dd->dd_prim = strdup(prim);
  dd->dd_area = 0;
  dd->dd_power = 0;
  dd->dd_area = 0;
  dd->dd_numDelays = 0;
  dd->dd_next = 0;

  return dd;
}

void delete_GDelayDef(GDelayDef *dd)
{
  int i;

  free(dd->dd_tech);
  free(dd->dd_prim);
  delete_Expr(dd->dd_area);
  delete_Expr(dd->dd_power);

  for (i = 0;i < dd->dd_numDelays;i++) {
    delete_Expr(dd->dd_delay[i]);
  }

  if (dd->dd_next) delete_GDelayDef(dd->dd_next);
}

void GDelayDef_flush()
{
  HashElem *E;

  if (!delay_def_is_init) return;

  for (E = Hash_first(delay_def_table);E;E = Hash_next(delay_def_table,E)) {
    GDelayDef *dd = (GDelayDef*) HashElem_obj(E);
    delete_GDelayDef(dd);
  }
  SHash_flush(delay_def_table);

  free(the_tech_list);
  tech_list_len = 0;
  the_tech_list = 0;
}

int GDelayDef_readFile(const char *file_name)
{
  extern const char *ycFileName;
  extern int ycLineNumber;
  FILE *f;

  if (!delay_def_is_init) {
    delay_def_is_init = 1;
    delay_def_table = new_SHash();
  } 

  if (!(f = fopen(file_name,"r"))) {
    return -1;
  }

  ycFileName = (char*)file_name;
  ycLineNumber = 1;
  yc_setup();
  BeginDD();
#if TKGATE_NOYYRESTART
  yyin = f;
#else
  yyrestart(f);
#endif
  yc_pushpool();
  yyparse();
  yc_poppool();
  fclose(f);

  return 0;
}

GDelayDef *GDelayDef_findList(const char *prim)
{
  return (GDelayDef*)SHash_find(delay_def_table,(char*)prim);
}

GDelayDef *GDelayDef_findTech(GDelayDef *dd,const char *tech)
{
 GDelayDef *l;

  /*
    Try to find matching technology first.
   */
  if (tech) {
    for (l = dd;l;l = l->dd_next)
      if (l->dd_tech && strcmp(l->dd_tech,tech) == 0)
	return l;
  }

  /*
    Look for default next
   */
  for (l = dd;l;l = l->dd_next)
    if (strcmp(l->dd_tech,"default") == 0)
      return l;

  return dd;			/* Return anything as a last resort */ 
}


GDelayDef *GDelayDef_find(const char *tech,const char *prim)
{
  GDelayDef *dd = GDelayDef_findList(prim);
 
  return GDelayDef_findTech(dd,tech);
}

void GDelayDef_begin(const char *tech,const char *prim)
{
  GDelayDef *old_dd = (GDelayDef*)SHash_find(delay_def_table,(char*)prim);
  GDelayDef *new_dd,*l;

  add_tech(tech);

  for (l = old_dd;l;l = l->dd_next) {
    if (strcmp(l->dd_tech,tech) == 0)
      break;
  }
  if (l) {
    int i;

    delete_Expr(l->dd_area);
    delete_Expr(l->dd_power);

    l->dd_area = 0;
    l->dd_power = 0;
    for (i = 0;i < l->dd_numDelays;i++) {
      delete_Expr(l->dd_delay[i]);
      l->dd_names[i] = 0;
      l->dd_delay[i] = 0;
    }

    l->dd_numDelays = 0;
    new_dd = l;
  } else {
    new_dd = new_GDelayDef(tech,prim);
    new_dd->dd_next = old_dd; 
    if (old_dd)
      SHash_remove(delay_def_table,(char*)prim);
    SHash_insert(delay_def_table,(char*)prim,new_dd);
  }

  cur_dd = new_dd;
}

void GDelayDef_end()
{
  cur_dd = 0;
}

void PrimSet(const char *code,const char *parm,Expr *e)
{
  if (strcmp(code,"delay") == 0) {
    int i;

    i = cur_dd->dd_numDelays++;
    cur_dd->dd_names[i] = strdup(parm);
    cur_dd->dd_delay[i] = e;
  } else if (strcmp(code,"area") == 0) {
    cur_dd->dd_area = e;
  } else if (strcmp(code,"power") == 0) {
    cur_dd->dd_power = e;
  }
}

char **GDelayDef_getTechList()
{
  return the_tech_list;
}

/*****************************************************************************
 *
 * Gate-delay lookup function based on a gate instantce
 *
 * Parameters:
 *     func		Function that is being invoked
 *     name		Name of pad	
 *     data		Data for lookup (gate)
 *     rval		Returned value of function
 *
 * Returns:		Non-zero on error.
 *
 *****************************************************************************/
static int gate_properties_lookup(const char *func,const char *name,void *data,int *rval)
{
  GCElement *g = (GCElement*) data;
  int N = GCElement_numPads(g);
  int b = 0, n = 0, c = 0;
  int i;

  if (!func) return EE_BADFUNC;
  if (!name) return EE_NOTDEF;

  for (i = 0;i < N;i++) {
    if (strcmp(GCElement_getPadName(g,i),name) == 0) {
      GWire *w;
      for (w = g->wires[i];w;w = w->next) {
	GNet *net = w->net;
	if (net->n_nbits > b) b = net->n_nbits;
	if (w->invert) c++;
	n++;
      }
    }
  }

  if (n == 0) {
    expr_errsym = name;
    return EE_NOTDEF;
  }

  if (strcmp(func,"bits") == 0) {
    *rval = b;
    return 0;
  } else if (strcmp(func,"inv") == 0) {
    *rval = c;
    return 0;
  } else if (strcmp(func,"num") == 0) {
    *rval = n;
    return 0;
  } else {
    expr_errsym = func;
    return EE_BADFUNC;
  }

  return 0;
}

int GDelayDef_getDelays(GDelayDef *dd, GCElement *g, int *delays)
{
  GGateInfo *gi = g->typeinfo;
  int i, j, n, rr;

  for (i = 0;gi->delayNames[i];i++) {
    delays[i] = 1;
    for (j = 0;j < gi->num_delays;j++)
      if (*dd->dd_names[j] == '*' || strcmp(dd->dd_names[j],gi->delayNames[i]) == 0) {
	rr = Expr_eval(dd->dd_delay[j],&n,gate_properties_lookup,g);
	if (rr != EE_OK) return -1;
	delays[i] = n;
	break;
      }

    if (j == gi->num_delays) {
      message(MC_MSGLOG|MC_URGENT,"Failed to find delay <%s> value for %s<%s>.",gi->delayNames[i],g->ename,gi->name);
    }

  }

  return gi->num_delays;
}

