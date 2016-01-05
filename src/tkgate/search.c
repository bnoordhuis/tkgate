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

    Last edit by hansen on Wed Jan 14 14:20:33 2009
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include "tkgate.h"

/*****************************************************************************
 *
 * Create a "GSearchContext" object used for keeping information about search
 * operations.
 *
 *****************************************************************************/
GSearchContext *new_GSearchContext()
{
  GSearchContext *S = (GSearchContext *) ob_malloc(sizeof(GSearchContext),"GSearchContext");

  S->mode = 0;
  S->target = 0;
  S->m_elem = 0;
  S->g_elem = 0;
  S->n_elem = 0;

  return S;
}

/*****************************************************************************
 *
 * Set the position as a result of a search.
 *
 *****************************************************************************/
void GSearchContext_setPosition(GModuleDef *M,GCElement *g,GNet *n)
{
  int x,y;

  x = y = 0;

  if (TkGate.circuit->es->env != M) {
    ob_touch(TkGate.circuit);
    editstate_navigateToModule(&TkGate.circuit->es,M);
    editstate_setCurrent(TkGate.circuit->es);
    ClearErrorMark();
  }

  if (g) {
    int minx,miny,maxx,maxy;

    gate_getbbx(g,TD_X11,&minx,&miny,&maxx,&maxy);
    x = (minx+maxx)/2;
    y = (miny+maxy)/2;

    TkGate_clearSelection();
    sel_clear(TkGate.circuit->es,1);
    sel_appendGate(TkGate.circuit->es,g,1);
    sel_finish(TkGate.circuit->es);

    message(0,msgLookup("msg.foundgate"),g->ename);		/* "Found gate named '%s'" */
  } else if (n) {
    GWireNode *wn1,*wn2;

    wn1 = n->n_driver->nodes;
    wn2 = wn1->out ? wn1->out : wn1->in;

    x = (wn1->x + wn2->x)/2;
    y = (wn1->y + wn2->y)/2;

    TkGate_clearSelection();
    sel_clear(TkGate.circuit->es,1);

    message(0,msgLookup("msg.foundwire"),n->n_signame);		/* "Found wire named '%s'" */
  }


  TkGate_setOrigin(TkGate.width/2 - x,TkGate.height/2 - y);
  ob_touch(TkGate.circuit->es);
  editstate_saveOrig(TkGate.circuit->es);

  SetErrorPosition(x,y);
  FlagRedraw();
}

/*****************************************************************************
 *
 * Clear the context from a seach operation
 *
 *****************************************************************************/
void GSearchContext_clear(GSearchContext *S)
{
  ob_touch(S);
  S->mode = 0;
  if (S->target) ob_free(S->target);
  S->target = 0;
  S->m_elem = 0;
  S->g_elem = 0;
  S->n_elem = 0;
}

static int GSearchContext_strcmp(GSearchContext *S,const char *s1,const char *s2)
{
  if ((S->mode & SF_IGNORECASE))
    return strcasecmp(s1,s2);
  else
    return strcmp(s1,s2);
}

static int GSearchContext_strncmp(GSearchContext *S,const char *s1,const char *s2,int n)
{
  if ((S->mode & SF_IGNORECASE))
    return strncasecmp(s1,s2,n);
  else
    return strncmp(s1,s2,n);
}

static char *GSearchContext_strstr(GSearchContext *S,const char *s1,const char *s2)
{
  if ((S->mode & SF_IGNORECASE))
    return strcasestr(s1,s2);
  else
    return strstr(s1,s2);
}


/*****************************************************************************
 *
 * Find a string with the specified mode values.
 *
 * Parameters:
 *    S			The search context to use
 *    target		The string to look for
 *    mode		Search mode specifier
 *    qualifier		Search qualifier (contains, begins, ends)
 *
 *****************************************************************************/
void GSearchContext_find(GSearchContext *S,const char *target,int mode,int qualifier)
{
  GModuleDef *M = 0;

  /*
   * If no target string, clear the search context and return.
   */
  if (!*target) {
    GSearchContext_clear(S);
    return;
  }

  /*
   * If any search parameters have changed, clear the current context
   * to begin a new search.
   */
  if (S->mode != mode || !S->target || GSearchContext_strcmp(S,S->target,target) != 0)
    GSearchContext_clear(S);

  ob_touch(S);
  if (S->target) ob_free(S->target);
  S->target = ob_strdup(target);
  S->mode = mode;

  if (!S->m_elem) {
    S->m_elem = Hash_first(TkGate.circuit->moduleTable);
    if (S->m_elem) {
      M = (GModuleDef *) HashElem_obj(S->m_elem);
      if ((mode & SF_GATES)) S->g_elem = Hash_first(M->m_gates);
      if ((mode & SF_NETS)) S->n_elem = Hash_first(M->m_gates);
    }
  } else
    M = (GModuleDef *) HashElem_obj(S->m_elem);

  while (S->m_elem) {
    if (S->g_elem) {
      GCElement *g = (GCElement*) HashElem_obj(S->g_elem);
      S->g_elem = Hash_next(M->m_gates,S->g_elem);

      if (g->typeinfo->Code == GC_JOINT) continue;

      if (GSearchContext_strstr(S,g->ename,S->target)) {
	GSearchContext_setPosition(M,g,0);
	return;
      }
    } else if (S->n_elem) {
      GNet *n = (GNet*) HashElem_obj(S->n_elem);
      S->n_elem = Hash_next(M->m_nets,S->n_elem);

      if (GSearchContext_strstr(S,n->n_signame,S->target)) {
	GSearchContext_setPosition(M,0,n);
	return;
      }
    } else {
      S->m_elem = Hash_next(TkGate.circuit->moduleTable,S->m_elem);
      if (S->m_elem) {
	M = (GModuleDef *) HashElem_obj(S->m_elem);
	if ((mode & SF_GATES)) S->g_elem = Hash_first(M->m_gates);
	if ((mode & SF_NETS)) S->n_elem = Hash_first(M->m_nets);
      }
    }
  }
  message(1,msgLookup("msg.searchagn"),S->target);	/* Target string '%s' not found.  Hit 'find' to restart search again. */
}

/*****************************************************************************
 *
 * Search some text for a match
 *
 * Parameters:
 *    S			The search context to use
 *    target		Target string to look for.
 *    text		String to search for target
 *    modName		Name of module to report on hit
 *    typeName		Type of object to report on hit
 *    objName		Name of object to report on hit
 *    qualifier		Search qualifier
 *    result		Result variable to use
 *    *count		Count of hits
 *    lineNo		Line number if this is a comment (-1 otherwise)
 *
 *****************************************************************************/
static int Search_hitTest(GSearchContext *S,const char *target,const char *text,const char *modName,const char *typeName,
			  const char *objName,int qualifier, const char *result, int *count,int lineNo)
{
  char buf[STRMAX],scount[STRMAX];
  int isHit = 0;
  int target_len;
  int text_len;
  const char *p = 0;

  switch (qualifier) {
    case SQ_CONTAINS :
      if ((p = GSearchContext_strstr(S,text,target)))
	isHit = 1;
      break;
    case SQ_BEGINS :
      target_len = strlen(target);
      if (GSearchContext_strncmp(S,target,text,target_len) == 0) {
	isHit = 1;
	p = text;
      }
      break;
    case SQ_ENDS :
      target_len = strlen(target);
      text_len = strlen(text);
      if (target_len <= text_len && GSearchContext_strncmp(S,target,text+text_len-target_len,target_len) == 0) {
	isHit = 1;
	p = text+text_len-target_len;
      }
      break;
    case SQ_MATCHES :
      if (GSearchContext_strcmp(S,target,text) == 0) {
	isHit = 1;
	p = text;
      }
      break;
  }

  /*
   * Report the search hit
   */
  if (isHit) {
    if (lineNo > 0)
      sprintf(buf,"%s %s %s@%d.%d",modName,typeName,objName,lineNo,(p-text));
    else
      sprintf(buf,"%s %s %s",modName,typeName,objName);
    sprintf(scount,"%d",(*count)++);
    Tcl_SetVar2(TkGate.tcl,(char*)result,scount,buf,TCL_GLOBAL_ONLY);
  }

  return isHit;
}

/*****************************************************************************
 *
 * Get list of hits for a specified search target
 *
 * Parameters:
 *    S			The search context to use
 *    target		The string to look for
 *    mode		Search mode specifier
 *    qualifier		Search qualifier (contains, begins, ends)
 *
 *****************************************************************************/
void GSearchContext_list(GSearchContext *S,const char *target,int mode,int qualifier,const char *result)
{
  HashElem *ME,*E;
  int count = 0;
  char scount[STRMAX];

  S->mode = mode;

  /*
   * If no target string, return
   */
  if (!*target) {
    return;
  }

  for (ME = Hash_first(TkGate.circuit->moduleTable);ME; ME = Hash_next(TkGate.circuit->moduleTable,ME)) {
    GModuleDef *M = (GModuleDef *) HashElem_obj(ME);

    if ((mode & SF_GATES) || (mode & SF_TEXT)) {
      for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
	GCElement *g = (GCElement*) HashElem_obj(E);

	if (g->typeinfo->Code == GC_JOINT) continue;

	if ((mode & SF_GATES)) {
	  if (Search_hitTest(S,target, g->ename, M->m_name, "gate", g->ename, qualifier, result, &count,-1))
	    continue;
	}

	if ((mode & SF_TEXT) && g->typeinfo->Code == GC_COMMENT) {
	  TextLine *CL;
	  int lineNo = 1;

	  for (CL = g->u.comment.first;CL;CL = CL->next) {
	    if (Search_hitTest(S,target, CL->text, M->m_name, "comment", g->ename, qualifier, result, &count,lineNo++))
	      break;
	  }
	}
      }
    }

    if ((mode & SF_NETS)) {
      for (E = Hash_first(M->m_nets);E;E = Hash_next(M->m_nets,E)) {
	GNet *n = (GNet*) HashElem_obj(E);

	Search_hitTest(S, target, n->n_signame, M->m_name, "net", n->n_signame, qualifier, result, &count,-1);
      }
    }
  }

  sprintf(scount,"%d",count);
  Tcl_SetVar2(TkGate.tcl,(char*)result,"count",scount,TCL_GLOBAL_ONLY);
}

/*****************************************************************************
 *
 * Search for an object by name and open it in an edit window.
 *
 * Parameters:
 *    spec		Specification of object
 *
 *****************************************************************************/
void GSearchContext_goto(const char *spec)
{
  char modName[STRMAX];
  char typeName[STRMAX];
  char objName[STRMAX];

  if (sscanf(spec,"%s %s %[^ \t@]",modName,typeName,objName) == 3) {
    GModuleDef *M = env_findModule(modName);
    GNet *n = 0;
    GCElement *g = 0;

    if (M) {
      if (strcmp(typeName,"net") == 0)
	n = GModuleDef_findNet(M,objName);
      else
	g = GModuleDef_findGate(M,objName);
    }

    if (g || n) {
      GSearchContext_setPosition(M,g,n);
    } else {
      message(1,msgLookup("err.badfind"),objName);
    }
  }
}
