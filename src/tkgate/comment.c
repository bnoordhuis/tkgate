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

    Last edit by hansen on Sun Feb 22 18:12:36 2009
****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "tkgate.h"

#define ENGLISH_LINESPACE	12	/* Line spacing for English text */
#define KANJI_LINESPACE		15	/* Line spacing for lines with kanji characters */

static char *psComment[] = {
  "%",
  "% An COMMENT gate",
  "%",
  "/pscomment {",
  "  0 startgate",
  "  12 cfont",
  "  newpath 0 0 moveto",
  "  { {} forall exch neg 0 exch rmoveto gsave show grestore } forall",
  "  grestore",
  "} bind def",
  0
};

GCElement *Comment_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char**,int);
void Comment_Delete(GCElement *g,GModuleDef *env,int drawp);
void Comment_GetExtents(GCElement *g,TargetDev_e target,int *minx,int *miny,int *maxx,int *maxy,int *bd);
int Comment_HitDistance(GCElement *g,int x,int y);
void Comment_Draw(GCElement *g,int md);
void Comment_PSWrite(GPrint *P,GModLayout *L,GCElement *g);
void Comment_VerSave(FILE *f,GCElement *g);
int Comment_EditProps(GCElement *g,int isLoadDialog);
void Comment_SetProp(GCElement*,const char*,const void*);
GCElement *Comment_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned);
void Comment_Init(GCElement*g);
void Comment_Rotate(GCElement *g,int centX, int centY,int rdir);

GGateInfo gate_comment_info = {
  GC_COMMENT,
  "COMMENT",
  "comment",0x0,
  "pscomment",psComment,
  -1,-1,

  {{"C",	{0,0},			{"gm.comment",0,0},		"gat_make COMMENT"},
   {0}},
  0,

  0,{{0}},
  {{0,-12,CT},{12,0,LJ},{0,-12,CT},{12,0,LJ}},	/* Are these even used? */
  {1},

  {0},
  
  Comment_Make,
  Nop_WriteCellDef,
  Comment_Init,
  Comment_Delete,
  Comment_GetExtents,
  Comment_HitDistance,
  Comment_Draw,
  Generic_Move,
  Comment_Copy,
  Err_AddInput,
  Err_AddOutput,
  Err_AddInOut,
  Comment_Rotate,
  Err_RemovePort,
  Err_ChangePin,
  Nop_SimInitFunc,
  Nop_SimHitFunc,
  Comment_PSWrite,
  Comment_EditProps,
  Comment_VerSave,
  Comment_SetProp
};

#if 0
static int hasKanji(char *s)
{
  for (;*s;s++)
    if ((*s & 0x80)) return 1;

  return 0;
}
#endif

/*
 * Get the value of a built-in variable and put it in p.
 */
int Comment_getValue(char *p,const char *name)
{
  if (strcasecmp(name,"tkgate-version") == 0) {
    strcpy(p,TKGATE_FULL_VERSION);
  } else if (strcasecmp(name,"tkgate-homepage") == 0) {
    strcpy(p,TKGATE_HOMEPAGE);
  } else if (strcasecmp(name,"tkgate-mailcontact") == 0) {
    strcpy(p,TKGATE_MAILCONTACT);
  } else if (strcasecmp(name,"tkgate-copyright") == 0) {
    strcpy(p,TKGATE_COPYRIGHT);
  } else {
    *p = 0;
    return -1;
  }

  return 0;
}


/*
 * Expand any value strings in a comment line.
 *
 * Currently the only recognized tags are of the form:
 *
 * <value-of name="varname">
 *
 * where "varname" is the name of a variable.  The recognized variables are:
 *
 *     tkgate-version			TkGate version number.
 */
void Comment_expandLine(char *xline,const char *sline)
{
  const char *p;
  char *x = xline;
  char tag[STRMAX],name[STRMAX];

  for (p = sline;p && *p;) {
    int did_tag = 0;

    if (sscanf(p,"<%s",tag) == 1 && strcasecmp(tag,"value-of") == 0) {
      did_tag = 1;

      if (sscanf(p,"<%*s %[^=]=\"%[^\"]",tag,name) == 2) {
	if (strcasecmp(tag,"name") == 0) {
	  Comment_getValue(x,name);
	  x += strlen(x);
	}
      }

      p = strchr(p,'>');
      if (p) p++;
    }

    if (!did_tag) {
      *x++ = *p++;
    }
  }
  *x = 0;
}

void Comment_Init(GCElement*g)
{
  Generic_Init(g);
  ob_touch(g);
  g->u.comment.width = g->u.comment.height = 0;
  g->u.comment.first = g->u.comment.last = 0;
  g->u.comment.html = 0;
  g->u.comment.doLink = 0;
  g->u.comment.link = 0;
}

void Comment_flushLines(GCElement *g)
{
  TextLine *L,*N;

  ob_touch(g);

  for (L = g->u.comment.first;L;L = N) {
    N = L->next;
    ob_free(L->text);
    ob_free(L);
  }
  g->u.comment.first = g->u.comment.last = 0;

  if (g->u.comment.html)
    delete_Html(g->u.comment.html);
  g->u.comment.html = 0;
}

void Comment_addLine(GCElement *g,const char *text)
{
  TextLine *L = (TextLine*) ob_malloc(sizeof(TextLine),"TextLine"); 

  ob_touch(L);
  L->text = ob_strdup(text);
  L->next = 0;

  ob_touch(g);
  if (g->u.comment.last) {
    ob_touch(g->u.comment.last);
    g->u.comment.last->next = L;
    g->u.comment.last = L;
  } else
    g->u.comment.first = g->u.comment.last = L;
}

void Comment_prependLine(GCElement *g,const char *text)
{
  TextLine *L = (TextLine*) ob_malloc(sizeof(TextLine),"TextLine"); 

  L->text = ob_strdup(text);
  L->next = g->u.comment.first;

  ob_touch(g);
  g->u.comment.first = L;
  if (!g->u.comment.last) {
    g->u.comment.last = L;
  }
}

GCElement *Comment_Make(EditState **es,GModuleDef *env,int GType,
		      int x,int y,int r,const char *Name,int noWire,const char **options,int nOptions)
{
  GCElement *g;
  const char *nodialog = 0;

  if (!(g = Generic_Make(es,env,GType,x,y,r,Name,noWire,options,nOptions)))
    return NULL;

  ob_touch(g);
  g->u.comment.first = g->u.comment.last = 0;
  g->u.comment.html = 0;

  nodialog  = seekOption("-nodialog",options,nOptions);

  if (es && (!nodialog || *nodialog != '1')) {
    GGateInfo *gi = g->typeinfo;
    int ok;
    const char *temp;

    (*gi->EditProps)(g,1);
    DoTcl("EditGate::post");
    if ((temp = Tcl_GetVar(TkGate.tcl,"edgat_ok",TCL_GLOBAL_ONLY)) && sscanf(temp,"%d",&ok) == 1 && ok) {
      if ((*gi->EditProps)(g,0) != 0)
	return 0;
    }    else {
      gate_delete(g,(*es)->env,1);
      return 0;
    }

  }

  return g;
}

void Comment_Delete(GCElement *g,GModuleDef *M,int drawp)
{
  if (M)     gate_remove(M,g);
  if (drawp) gate_draw(g,0);

  Comment_flushLines(g);
}

static void Comment_flushHtml(GCElement *g)
{
  if (g->u.comment.html) delete_Html(g->u.comment.html);
  g->u.comment.html = 0;
}

/*****************************************************************************
 *
 * Build the html for a comment gate.
 *
 *****************************************************************************/
static int Comment_buildHtml(GCElement *g)
{
  TextLine *L;
  Html *h;
  int is_empty = 0;

  /* 
     Already built and with the correct zoom factor
  */
  if (g->u.comment.html
      && g->u.comment.html->h_zoom == TkGate.circuit->zoom_factor
      && g->u.comment.html->h_locale == TkGate.circuit->c_locale)
    return 0;

  ob_begin_framef("-BuildHtml",FF_TRANSPARENT);
  ob_touch(g);

  if (g->u.comment.html) {
    delete_Html(g->u.comment.html);
    g->u.comment.html = 0;
  }

  h = new_Html(TD_X11);
  for (L = g->u.comment.first;L;L = L->next) {
    Html_addLine(h,L->text);
  }
  Html_format(h);

  if (!h->h_isVisible) {
    delete_Html(h);
    h = new_Html(TD_X11);
    Html_addLine(h,msgLookup("comment.empty"));
    Html_format(h);
    is_empty = 1;
  }

  g->u.comment.html = h;
  g->u.comment.width = h->h_width;
  g->u.comment.height = h->h_height;

  ob_end_frame();

  return is_empty;
}

void Comment_GetExtents(GCElement *g,TargetDev_e target,
			int *minx,int *miny,int *maxx,int *maxy,int *bd)
{
  switch (target) {
  case TD_X11 :
    Comment_buildHtml(g);

    break;
  case TD_PRINT :
    {
      Html *h = new_Html(TD_PRINT);
      g->u.comment.width = h->h_width;
      g->u.comment.height = h->h_height;
      delete_Html(h);
    }
    break;
  }

  *minx = g->xpos;
  *miny = g->ypos;
  *maxx = g->xpos + g->u.comment.width;
  *maxy = g->ypos + g->u.comment.height;

  if (bd) *bd = 20;
}

int Comment_HitDistance(GCElement *g,int x,int y)
{
  ob_touch(g);

  g->left = 0;
  g->right = 0;
  g->top = 0;
  g->bottom = 0;

  if ((x > g->xpos - 10) &&
      (x < g->xpos + g->u.frame.width + 10) &&
      (y > g->ypos - 10) &&
      (y < g->ypos + g->u.frame.height + 10)) {
    int rel_x = (x-g->xpos)*TkGate.circuit->zoom_factor;
    int rel_y = (y-g->ypos)*TkGate.circuit->zoom_factor;

    if (g->selected) {
      if (x < g->xpos + 10) {
	g->left = 1;
      }
      if (x > g->xpos + g->u.frame.width - 10) {
	g->right = 1;
      }
      if (y < g->ypos + 10) {
	g->top = 1;
      }
      if (y > g->ypos + g->u.frame.height - 10) {
	g->bottom = 1;
      }
      if (g->left || g->right || g->top || g->bottom) {
	return 0;
      }
    }

    Comment_buildHtml(g);
    if (!Html_isHit(g->u.comment.html,rel_x,rel_y)) {
      return NOHIT;
    }

    return GATERANGE-1;
  }

  return NOHIT;
}

GCElement *Comment_Copy(GModuleDef *M,GCElement *g,int x,int y,unsigned flags)
{
  GCElement *ng;
  TextLine *L;

  ng = Generic_Copy(M,g,x,y,flags);
  ob_touch(ng);
  ng->u.comment.first = ng->u.comment.last = 0;
  for (L = g->u.comment.first;L;L = L->next) {
    Comment_addLine(ng,L->text);
  }
  
  ng->u.comment.reqWidth = g->u.comment.reqWidth;

  Comment_buildHtml(ng);

  return ng;
}

void Comment_Draw(GCElement *g,int md)
{
  GC gc = TkGate.commentGC;
  int x,y; 

  Comment_buildHtml(g);

  x = g->xpos;
  y = g->ypos;

  Html_draw(g->u.comment.html,x,y);

  if (g->selected) {
    ZDrawRectangle(TkGate.D,TkGate.W,gc,
		   ctow_x(g->xpos),ctow_y(g->ypos),g->u.comment.width,g->u.comment.height);
  }
}

void Comment_PSWrite(GPrint *P,GModLayout *L,GCElement *g)
{
  TextLine *l;
  Html *h;

  h = new_Html(TD_PRINT);
  for (l = g->u.comment.first;l;l = l->next) {
    Html_addLine(h,l->text);
  }
  Html_format(h);
  Html_psPrint(h,P,g->xpos,g->ypos);
  delete_Html(h);
}

void Comment_VerSave(FILE *f,GCElement *g)
{
  char buf[STRMAX],buf2[STRMAX];
  TextLine *L;
  void *encoder = Circuit_getSaveFileEncoder(TkGate.circuit);

  fprintf(f,"  //: comment %s",g->ename);

  VerilogBasicGateComment(f,g,0);
  fprintf(f,"\n");

  for (L = g->u.comment.first;L;L = L->next) {
    recodeText(encoder,buf,STRMAX,L->text);
    fprintf(f,"  //: /line:\"%s\"\n",quoteChars(buf2,buf,"\"\\"));
  }
  fprintf(f,"  //: /end\n");
}

int Comment_EditProps(GCElement *g,int isLoadDialog)
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
    if (Comment_buildHtml(g)) {
      gate_delete(g,TkGate.circuit->es->env,1);
      return -1;		// Indicate we deleted the gate
    }
   }
  return 0;
}

void Comment_SetProp(GCElement *g,const char *prop,const void *value)
{
  char *s = (char*)value;

  ob_touch(g);

  if (strcmp(prop,"/line") == 0) {
    Comment_flushHtml(g);

    /*
     * If /link and /dolink have been specified, this is an old-style link
     * specification and we must convert it to the new style.
     */
    if (g->u.comment.doLink && g->u.comment.link) {
      char buf[STRMAX];

      sprintf(buf,"<a href=\"%s\">%s</a>",g->u.comment.link,s);
      Comment_addLine(g,buf);
    } else
      Comment_addLine(g,s);
  }
  if (strcmp(prop,"/dolink") == 0) {
    g->u.comment.doLink = *(int*)value;
  }
  if (strcmp(prop,"/link") == 0) {
    if (g->u.comment.link)
      ob_free(g->u.comment.link);
    g->u.comment.link = ob_strdup(s);
  }
}


void Comment_Rotate(GCElement *g,int centX, int centY,int rdir)
{
  int x = g->xpos + g->u.comment.width/2;
  int y = g->ypos + g->u.comment.height/2;
  int nx,ny;

  ob_touch(g);
  nx = rotateX(x - centX,y - centY, rdir) + centX;
  ny = rotateY(x - centX,y - centY, rdir) + centY;

  g->xpos = nx - g->u.comment.width/2;
  g->ypos = ny - g->u.comment.height/2;
}

/*
 * Test to see if (x,y) touches a hyperlink in the comment.  If so then return
 * the link.
 */
const char *Comment_getHyperlink(GCElement *g,int x,int y)
{
  int rel_x = (x-g->xpos)*TkGate.circuit->zoom_factor;
  int rel_y = (y-g->ypos)*TkGate.circuit->zoom_factor;

  Comment_buildHtml(g);

  return Html_getLink(g->u.comment.html,rel_x,rel_y);
}

void init_comment()
{
  RegisterGate(&gate_comment_info);
}

