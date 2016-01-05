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

    Last edit by hansen on Mon Dec 22 06:31:44 2008
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "tkgate.h"

#define DEBUG_NET 0

static char *vtype_table[] = {
  "wire", "tri", "wand", "triand", "wor", "trior", "tri0", "tri1", "trireg"
};
static int vtype_table_length = sizeof(vtype_table)/sizeof(vtype_table[0]);

int ycIsKW(char*);

/*****************************************************************************
 *
 * Get a string for the "display type" of a net.
 *
 * Parameters:
 *      n		Net on which to lookup the dtype.
 *
 * Returns:		String representing the display type.
 *
 * Get the string for the net's "dtype".  The dtype is the type used to determine what
 * type of symbol should be displayed next to the net in the list of nets.  It
 * is not necessarily the actual verilog type.
 *
 *****************************************************************************/
const char *GNet_getDTypeStr(GNet *n)
{
  /*
   * Translate type code from HDL modules
   */
  switch (n->n_dtype) {
  case NT_INPUT : return "input";
  case NT_OUTPUT : return "output";
  case NT_INOUT : return "inout";
  case NT_REG : return "reg";
  case NT_WIRE : return "wire";
  }

  if (n->n_ionet) {
    switch (n->n_ionet->typeinfo->Code) {
      case GC_LOGICIN : return "input";
      case GC_LOGICOUT : return "output";
      case GC_LOGICTRI : return "inout";
    }
  }

  return "wire";
}

/*****************************************************************************
 *
 * Get a string for the "verilog type" of a net.
 *
 * Parameters:
 *      n		Net on which to lookup the dtype.
 *
 * Returns:		String representing the verilog type.
 *
 *****************************************************************************/
const char *GNet_getVType(GNet *n)
{
  int code = 0;

  if (n->n_ionet) {
    switch (n->n_ionet->typeinfo->Code) {
    case GC_SWITCH :
    case GC_DIP :
      return "switch";
    case GC_LOGICIN :
    case GC_LOGICOUT :
    case GC_LOGICTRI :
      return "port";
    }
    return n->n_ionet->typeinfo->vnames;
  }

  code = n->n_vtype;
  if (code > vtype_table_length)
    code = 0; 


  return vtype_table[code];
}

void GNet_setVType(GNet *n,const char *vtype)
{
  int i;

  ob_touch(n);

  for (i = 0;i < vtype_table_length;i++)
    if (strcmp(vtype_table[i],vtype) == 0) break;

  if (i >= vtype_table_length)
    i = 0;

  n->n_vtype = i;
  n->n_labelWidth = 0;
}


/*****************************************************************************
 *
 * Create a new net in a module with properties similar to another net
 *
 * Parameters:
 *      name		Net name hint
 *      rnet		Reference net on which to base the new net's properties
 *      M		Module in which to create net
 *
 *****************************************************************************/
GNet *new_GNet_compatable(const char *name,GNet *rnet,GModuleDef *M)
{
  GNet *net;
  static unsigned generation = 0;

  net = (GNet*) ob_malloc(sizeof(GNet),"GNet");

  if (M) {
    char buf[STRMAX];

    pickValidName(buf,name,"w",M->m_nets);
    net->n_signame = ob_strdup(buf);
    if (name) {
      net->n_show_name = 1;
    } else {
      net->n_show_name = 0;
    }
  } else {
    net->n_signame = 0;
    net->n_show_name = 0;
  }

  net->n_nbits = rnet ? rnet->n_nbits : 1;
  net->n_dtype = NT_NONE;
  net->n_vtype = NV_WIRE;
  net->n_refs = 0;
  net->n_mark = 0;
  net->n_ionet = 0;
  net->n_mod = M;
  net->n_driver = 0;
  net->n_next = 0;
  net->n_finalized = 0;
  net->n_labelWidth = 0;
  net->n_generation = generation++;
 
  SHash_insert(M->m_nets,net->n_signame,net);

  if (M && TkGate.circuit->es && M == TkGate.circuit->es->env && editstate_getInterfaceMode() == INTFMODE_NONE) {
    DoTcl("NetList::add %s -hidden %d -bits %d -type %s",net->n_signame,!net->n_show_name,net->n_nbits,GNet_getDTypeStr(net));
  }

#if DEBUG_NET
  printf("new_GNet() -> %x",net);
  if (net->n_signame)
    printf("  (%s)\n",net->n_signame);
  else
    printf("\n");
#endif

  return net;
}

/*****************************************************************************
 *
 * Create a new net in a module
 *
 * Parameters:
 *      name		Net name hint
 *      M		Module in which to create net
 *
 *****************************************************************************/
GNet *new_GNet(const char *name,GModuleDef *M)
{
  return new_GNet_compatable(name,0,M);
}

/*****************************************************************************
 *
 * Update net in the net list box
 *
 * Parameters:
 *      n		Target net for operation.
 *
 *****************************************************************************/
void net_update(GNet *n)
{
  if (n->n_mod && TkGate.circuit->es && n->n_mod == TkGate.circuit->es->env && editstate_getInterfaceMode() == INTFMODE_NONE)
    DoTcl("NetList::replace %s -hidden %d -bits %d -type %s",n->n_signame,!n->n_show_name,n->n_nbits,GNet_getDTypeStr(n));
}

void delete_GNet(GNet *net)
{
  ob_touch(net);
#if DEBUG_NET
  printf("GNet::Delete(%s) %x",net->n_signame ?: "",net);
#endif

  if (net->n_signame) {
    SHash_remove(net->n_mod->m_nets,net->n_signame);
    if (net->n_mod && TkGate.circuit->es && net->n_mod == TkGate.circuit->es->env && editstate_getInterfaceMode() == INTFMODE_NONE) {
      DoTcl("NetList::del %s",net->n_signame);
    }
    ob_free(net->n_signame);
#if DEBUG_NET
    printf(" [%d]",r);
#endif
  }
#if DEBUG_NET
  printf("\n");
#endif
  ob_free(net);
}

/*****************************************************************************
 *
 * Increment the reference count of a net.
 *
 * Parameters:
 *      net		Target net for operation.
 *
 *****************************************************************************/
void net_incref(GNet *net)
{
  ob_touch(net);
#if 0
  printf("net_incref(%s) [%d]\n",net->n_signame,net->n_refs+1);
#endif
  net->n_refs++;
}

/*
 * Decrement the reference count of a net.  If the reference
 * count goes to zero, the net is deleted.
 */
void net_decref(GNet *net)
{
  ob_touch(net);
#if 0
  printf("net_decref(%s) [%d]\n",net->n_signame,net->n_refs-1);
#endif
  if (--net->n_refs <= 0)
    delete_GNet(net);
}

/*
 * Select one of the nets 'a' or 'b'.  Typically used when joining two wires
 * to determine which net name will be retained and which will be deleted.
 */
GNet *net_pickOne(GNet *a,GNet *b,int decref_loser)
{
  GNet *r = 0;	/* Net to return */
  GNet *l = 0;	/* Losing net */

  if (!a) 
    r = b;
  else if (!b) 
    r = a;
  else if (a == b) {
    r = a;
    l = b;
  } else if (a->n_ionet) {
    r = a;
    l = b;
  } else if (b->n_ionet) {
    r = b;
    l = a;
  } else if (a->n_show_name) {
    r = a;
    l = b;
  } else if (b->n_show_name) {
    r = b;
    l = a;
  } else if (a->n_generation < b->n_generation) {
    r = a;
    l = b;
  } else {
    r = b;
    l = a;
  }

  if (l && decref_loser) net_decref(l);

  return r;
}

/*
   Check to see if it is OK to connect wires from the
   specified nets.  Return 1 if OK, return 0 and display
   a status bar message if not OK. 
*/
int net_connectOK(GNet *n1,GNet *n2,int isMidWire)
{
  if (n1 == n2) {
    message(0,msgLookup("err.badconsame"));	/* Connection refused because wires are part of the same net. */
    return 0;
  }
  if (n1->n_ionet && n2->n_ionet) {
    message(0,msgLookup("err.badconptsp"));	/* Connection refused because both wires are module ports or supply. */
    return 0;
  }

  if (!isMidWire && n1->n_nbits != n2->n_nbits) {
    message(0,msgLookup("err.badconbitw"));	/* Connection refused because bit widths do not match. */
    return 0;
  }

  return 1;
}

/*
  Unselect all nets.
 */
void net_unselect(int drawp)
{
  if (TkGate.circuit->nsel) {
    GNet *n = TkGate.circuit->nsel;
    if (drawp) GNet_draw(n);
    TkGate.circuit->nsel = 0;
    if (drawp) GNet_draw(n);
  }
}

/*
  Make 'n' the selected net.
 */
void net_select(GNet *n,int drawp)
{
  if (TkGate.circuit->nsel == n) return;
  net_unselect(drawp);

  if (!n) return;

  DoTcl("NetList::shadowselection %s",n->n_signame);

  ob_touch(TkGate.circuit);

  if (drawp) GNet_draw(n);
  TkGate.circuit->nsel = n;
  if (drawp) GNet_draw(n);
}



/*
   Redraw an entire net.
*/
void GNet_draw(GNet *net)
{
  wire_drawnet(net->n_driver);
}

void net_setDType(GNet *net,int dtype)
{
  if (net->n_dtype == dtype) return;

  ob_touch(net);
  net->n_dtype = dtype;
  net->n_labelWidth = 0;

  if (net->n_mod && TkGate.circuit->es && net->n_mod == TkGate.circuit->es->env && editstate_getInterfaceMode() == INTFMODE_NONE) {
    DoTcl("NetList::replace %s -hidden %d -bits %d -type %s",net->n_signame,!net->n_show_name,net->n_nbits,GNet_getDTypeStr(net));
  }
}

void net_setSize(GNet *net,int nbits)
{
  if (net->n_nbits == nbits) return;

  ob_touch(net);
  net->n_nbits = nbits;

  if (net->n_mod && TkGate.circuit->es && net->n_mod == TkGate.circuit->es->env && editstate_getInterfaceMode() == INTFMODE_NONE) {
    DoTcl("NetList::replace %s -hidden %d -bits %d -type %s",net->n_signame,!net->n_show_name,net->n_nbits,GNet_getDTypeStr(net));
  }
}

void net_editProps(GNet *n,int x,int y)
{
  char *sn = n->n_show_name ? "" :"@";
  int ioCode = 0;
  int wx = ctow_x(x-50);
  int wy = ctow_y(y-50);
  const char *result;
  const char *vtype = GNet_getVType(n);

  if (n->n_ionet) {
    ioCode = n->n_ionet->typeinfo->Code;
    if (ioCode != GC_LOGICIN && ioCode != GC_LOGICOUT && ioCode != GC_LOGICTRI)
      ioCode = 0;	/* yappari yameta */
  }


  if (n->n_nbits == 1)
    DoTcl("tkg_editNet %d %d {%s%s} %d %s\n",wx,wy,n->n_signame,sn,ioCode,vtype);
  else
    DoTcl("tkg_editNet %d %d {%s[%d:0]%s} %d %s\n",wx,wy,n->n_signame,n->n_nbits-1,sn,ioCode,vtype);

  result = Tcl_GetStringResult(TkGate.tcl);

  if (*result != '0')
    SetModified(MF_NET);
}

/*
   Set the signal name of a wire to s.  
*/
void net_rename(GNet *net,const char *s,int showName)
{
  char *oldName = ob_strdup(net->n_signame);
  char buf[STRMAX];

  if (net->n_signame) {
    if (net->n_mod && TkGate.circuit->es && net->n_mod == TkGate.circuit->es->env && editstate_getInterfaceMode() == INTFMODE_NONE)
      DoTcl("NetList::del %s",net->n_signame);

    SHash_remove(net->n_mod->m_nets,net->n_signame);
    ob_free(net->n_signame);
  }
  ob_touch(net);
  net->n_signame = 0;
  net->n_labelWidth = 0;

  pickValidName(buf,s,"w",net->n_mod->m_nets);
  net->n_signame = ob_strdup(buf);

  if (showName)
    GNet_showName(net);
  else
    GNet_hideName(net);

  SHash_insert(net->n_mod->m_nets,net->n_signame,net);

  if (net->n_mod && TkGate.circuit->es && net->n_mod == TkGate.circuit->es->env && editstate_getInterfaceMode() == INTFMODE_NONE) {
    DoTcl("NetList::replace %s -hidden %d -bits %d -type %s",
	  net->n_signame,!net->n_show_name,net->n_nbits,GNet_getDTypeStr(net));
  }
  ob_free(oldName);
}

/*
 * Check 'name' to see if it is a valid name.  If 'H' is non-null, then the
 * name may not appear as a key in the hash table.  Returns 0 if the name
 * is valid.
 */
int checkValidName(const char *name,SHash *H)
{
  const char *p = name;

  if (!isalpha(*p) && *p != '_')
    return -1;

  for (p++;*p;p++) {
    if (!isalpha(*p) && !isdigit(*p) && *p != '_')
      return -1;
  }

  if (H && SHash_find(H,name))
    return -1;

  return 0;
}

/*****************************************************************************
 *
 * Pick name for an object based on suggested name.
 *
 * Parameters:
 *      buf		Return for generated name
 *      name		Suggested name
 *      base		Type-dependent base name for name generation
 *      H		Table of already registered names
 *
 * Pick a valid name for an object based on a possible suggested name 'name',
 * a base name 'base' for objects of a that type, and a hash table 'H' for objects
 * of that type.  One or more of 'name', 'base' and 'H' may be null.  The generated
 * name will be placed in 'buf'.  If 'name' is a valid name that does not appear in the
 * hash table 'H', 'name' will become the selected name.
 *
 *****************************************************************************/
void pickValidName(char *buf,const char *name,const char *base,SHash *H)
{
  char *p;
  int reason = 0;

  if (!base || !*base)
    base = "n";

  if (name) {
    if (buf != (char*)name)
      strcpy(buf,name);

    if (trimName(buf)) {
      reason = 1;
      if (!*buf) {
	if (base)
	  strcpy(buf,base);
	else
	  strcpy(buf,"name");
      }
    }

    if (!*buf)
      sprintf(buf,"%s0",base);

    if (H && SHash_find(H,buf))
      reason = 2;
    else if (ycIsKW(buf))
      reason = 4;
    else {
      if (reason)
	message(0,msgLookup("err.badnetname"));		/* "Illegal characters in identifier deleted." */
      return;
    }
  } else
    sprintf(buf,"%s0",base);

  p = buf+strlen(buf);

  if ((H && SHash_find(H,buf)) || ycIsKW(buf)) {
    char *p;
    int n;

    p = buf + strlen(buf);
    while (p > buf && isdigit((int)p[-1])) p--;

    n = 0;
    if (*p) sscanf(p,"%d",&n);

    do {
      sprintf(p,"%d",n++);
    } while ((H && SHash_find(H,buf)) || ycIsKW(buf));
  }

  switch (reason) {
  case 1 :
    message(0,msgLookup("err.netbcrename"),buf);	/* "Identifier renamed to '%s' because of illegal characters." */
    break;
  case 2 :
    message(0,msgLookup("err.netconfnet"),buf);
    break;
  case 3 :
    message(0,msgLookup("err.netconfgat"));
    break;
  case 4 :
    message(0,msgLookup("err.netconfkw"));
    break;
  }
}

int GWire_pickProbePosition(GWire *w,int *x,int *y)
{
  int orig_x = *x;
  int orig_y = *y;
  int best_d = 0x7ffffff;
  int best_x, best_y;
  GWireNode *n;
  

  w = wire_driver(w);
  for (n = w->nodes;n && n->out;n = n->out) {
    int x1 = n->x;
    int y1 = n->y;
    int x2 = n->out->x;
    int y2 = n->out->y;
    int d = 0x7fffffff;
    int test_x, test_y;

    if (x1 == x2) {
      if (iabs(orig_y-y1) <= iabs(y1-y2) && iabs(orig_y-y2) <= iabs(y1-y2)) {
	d = iabs(orig_x-x1)*iabs(orig_x-x1);
	test_x = x1;
	test_y = orig_y;
      }
    } else {
      if (iabs(orig_x-x1) <= iabs(x1-x2) && iabs(orig_x-x2) <= iabs(x1-x2)) {
	d = iabs(orig_y-y1)*iabs(orig_y-y1);
	test_x = orig_x;
	test_y = y1;
      }
    }
    if (d == 0x7fffffff) {
      int d1 = iabs(orig_x-x1)*iabs(orig_x-x1) + iabs(orig_y-y1)*iabs(orig_y-y1);
      int d2 = iabs(orig_x-x2)*iabs(orig_x-x2) + iabs(orig_y-y2)*iabs(orig_y-y2);
      if (d1 < d2) {
	d = d1;
	test_x = x1;
	test_y = y1;
      } else {
	d = d2;
	test_x = x2;
	test_y = y2;
      }
    }

    if (d < best_d) {
      best_d = d;
      best_x = test_x;
      best_y = test_y;
    }
  }

  *x = best_x;
  *y = best_y;

  return best_d;
}

int GNet_getDisplayLabel(GNet *net,char label[],int len,int mode)
{
  *label = 0;

  if (net->n_ionet && mode == DLM_GET_VISIBLE) {
    switch (net->n_ionet->typeinfo->Code) {
    case GC_LOGICIN :
    case GC_LOGICOUT :
    case GC_LOGICTRI :
      return 0;
    }
  }

  if ((net->n_show_name && net->n_signame) || mode != DLM_GET_VISIBLE) {
    if (net->n_vtype != NV_WIRE)
      sprintf(label,"%s(%s)",net->n_signame,GNet_getVType(net));
    else
      sprintf(label,"%s",net->n_signame);
  } else if (net->n_vtype != NV_WIRE)
    sprintf(label,"(%s)",GNet_getVType(net));

  return strlen(label);
}

/*
   Get the position (x,y) and justification p for a signal name
   on wire w.
*/
void SignalNamePos(GWireNode *n1,GWireNode *n2,int *x,int *y,int *p,int *q)
{
  *x = n1->x + (n2->x - n1->x)/3;
  *y = n1->y + (n2->y - n1->y)/3;

  if (n1->x == n2->x) {
    *x += 5;
    if (p) *p = AtLeft|BetweenTopAndBottom;
  } else {
    *x -= 5;
    if (p) *p = BetweenLeftAndRight|AtBottom;
  }


  if (q) *q = (n1->x == n2->x) ? (n1->y <= n2->y) : 2 + (n1->x <= n2->x);
}

int GNet_getWires(GNet *n,GWire **wlist,unsigned which)
{
  return GWire_getNetWires(n->n_driver,wlist,which);
}

/*****************************************************************************
 *
 * Make sure the name on a net is hidden
 *
 * Parameter:
 *     net			Net on which to make all labels hidden
 *
 *****************************************************************************/
void GNet_hideName(GNet *net)
{
  ob_touch(net);
  net->n_show_name = 0;
  GNet_checkNameVisibility(net,0);
  net_update(net);
}

/*****************************************************************************
 *
 * Make sure the name on a net is visible
 *
 * Parameter:
 *     net			Net on which to make the label visible
 *
 *****************************************************************************/
void GNet_showName(GNet *net)
{
  ob_touch(net);
  net->n_show_name = 1;
  GNet_checkNameVisibility(net,1);
  net_update(net);
}

void GNet_checkNameVisibility(GNet *net,int action)
{
  if (net->n_show_name) {
    GWire *wlist[GNet_numWires(net)];
    GWireNode *n;
    int count = GNet_getWires(net,wlist,GW_DRIVER);
    int i;

#if 0
    printf("checkNameVis %s nw=%d count=%d\n",net->n_signame,GNet_numWires(net),count);
#endif

    /*
     * Look for a labeled wire, if there is one, then return and do nothing further.
     */
    for (i = 0;i < count;i++) {
#if 0
      printf("  pad %d\n",i);
#endif
      for (n = wlist[i]->nodes;n;n = n->out) {
#if 0
	printf("    wire %p islab=%d\n",n,n->isLabeled);
#endif
	if (n->isLabeled) return;
      } 
    }

    /*
     * None of the existing wires were labeled, so pick one and label it.
     */
    for (n = net->n_driver->nodes;n && n->out;n = n->out)
      if (!n->showSize)
	break;

    if (action) {
      ob_touch(n);
      //printf("%p: make labeled (1)\n",n);
      n->isLabeled = 1;
      n->offset = 50;
    } else {
      ob_touch(net);
      net->n_show_name = 0;
    }
  }
}


int GNet_getLabelWidth(GNet *n)
{
  if (!n->n_labelWidth) {
    char label[STRMAX];

    if (GNet_getDisplayLabel(n, label, STRMAX, DLM_GET_ALWAYS) > 0) {
      n->n_labelWidth = GKTextWidth(TkGate.stextbXF[TkGate.circuit->zoom_factor],label,strlen(label));
    } else
      n->n_labelWidth = 42;
  }

  return n->n_labelWidth;
}

/*****************************************************************************
 *
 * Square of distance from a point to a line segment.
 *
 *****************************************************************************/
int segment_distance(int x,int y,int x1,int y1,int x2,int y2)
{
  if (x1 == x2) {	/* Veritical segment */
    if (y1 > y2) { int t = y1;y1=y2;y2=t;}

    if (y < y1) {
      return (y-y1)*(y-y1) + (x-x1)*(x-x1);
    } else if (y > y2) {
      return (y-y2)*(y-y2) + (x-x2)*(x-x2);
    } else {
      return (x-x1)*(x-x1);
    }

  } else {		/* Horizontal segment */
    if (x1 > x2) { int t = x1;x1=x2;x2=t;}

    if (x < x1) {
      return (y-y1)*(y-y1) + (x-x1)*(x-x1);
    } else if (x > x2) {
      return (y-y2)*(y-y2) + (x-x2)*(x-x2);
    } else {
      return (y-y1)*(y-y1);
    }
  }
}

void GNet_labelClosest(GNet *net,int x,int y)
{
  GWire *wlist[GNet_numWires(net)];
  GWireNode *n,*best_n = 0;
  int best_dist = 0;
  int count = GNet_getWires(net,wlist,GW_DRIVER);
  int offset;
  int side;
  int i;


  for (i = 0;i < count;i++) {
    for (n = wlist[i]->nodes;n && n->out;n = n->out) {
      int d = segment_distance(x,y,n->x,n->y,n->out->x,n->out->y);

      if (!best_n || d < best_dist) {
	best_dist = d;
	best_n = n;
      }
    }
  }

  if (best_dist > 50*50) {
    GNet_checkNameVisibility(net,0);    
    net_update(net);
    return;
  }


  /*
   * We found the node we wish to label...
   */
  if (best_n) {
    int x1 = best_n->x;
    int y1 = best_n->y;
    int x2 = best_n->out->x;
    int y2 = best_n->out->y;
    GrabbedLabel *gl = TkGate.circuit->labelsel;

    GNet_draw(net);
    ob_touch(best_n);
    best_n->isLabeled = 1;

    if (y1 != y2) {
      /* Vertical */
      int ay = y + gl->oy;

      if ((gl->position & AtTop))
	ay += 12;

      offset = 100*(ay-y1)/(y2-y1);
      side = (x < x1);
    } else if (x1 != x2) {
      /* Horizontal */
      int ax = x + gl->ox;

      if ((gl->position & AtRight))
	ax -= GNet_getLabelWidth(net);

      offset = 100*(ax-x1)/(x2-x1);
      side = (y > y1);
    } else {
      offset = 50;
      side = 0;
    }

    if (offset < 1) offset = 1;
    if (offset > 99) offset = 99;
    best_n->offset = offset;
    best_n->labelSide = side;
    GNet_draw(net);
  }
}

