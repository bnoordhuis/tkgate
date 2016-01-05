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
#ifndef __net_h
#define __net_h

#define GW_DRIVER	0x1	/* Driver wires only */
#define GW_DRIVEE	0x2	/* Drivee wires only */
#define GW_ALL		0x3	/* All wires */


#define DLM_GET_VISIBLE	0	/* Get name only when visible */
#define DLM_GET_ALWAYS	1	/* Get name always */



/*
   A GNet is a collection of wires and wire segments all connected by
   joints and outsplices.
*/
struct net {
  char		*n_signame;	/* Signal name of this net */

  int		n_labelWidth;	/* Width of the label text */

  int		n_nbits;	/* Bit width of this net */
  short 	n_dtype;	/* Used only in HDL modules to track net type */
  short		n_vtype;	/* Used only in netlist modules to track net type */
  int		n_refs;		/* Reference count */
  GModuleDef	*n_mod;		/* Module this net is in */
  GCElement	*n_ionet;	/* I/O gate if this is a port */
  unsigned	n_mark : 1;	/* Mark for nets */
  unsigned	n_show_name : 1;/* Is net name visible? */
  unsigned	n_finalized : 1;/* Is net finalized? */

  unsigned	n_generation;	/* Generation number of net (used in net merging) */	

  short		n_wnum;		/* Counter for counting wires */
  short		n_gnum;		/* Counter for counting joints, etc. */
  GWire		*n_driver;	/* "driver" (for display purposes) wire of net */
  GNet		*n_next;	/* Next net in freelist */
};

GNet *new_GNet(const char*,GModuleDef*);
GNet *new_GNet_compatable(const char*,GNet*,GModuleDef*);
void delete_GNet(GNet*);
GNet *net_pickOne(GNet*,GNet*,int);
void net_decref(GNet*);
void net_incref(GNet*);
void net_update(GNet *n);
int net_connectOK(GNet *n1,GNet *n2,int isMidWire);
void net_setSize(GNet *net,int nbits);
void net_setDType(GNet *net,int dtype);
void net_rename(GNet *net,const char *s,int showName);
void net_select(GNet *n,int drawp);
void net_unselect(int drawp);

const char *GNet_getDTypeStr(GNet *n);
void GNet_labelClosest(GNet *net,int x,int y);
void GNet_draw(GNet*);
void GNet_hideName(GNet*);
void GNet_showName(GNet*);
void GNet_checkNameVisibility(GNet*,int);
void GNet_setVType(GNet *n,const char *vtype);
const char *GNet_getVType(GNet *n);
int  GNet_getDisplayLabel(GNet *net,char label[],int len,int mode);
int GNet_getWires(GNet *n,GWire **wlist,unsigned which);
int GNet_getLabelWidth(GNet *net);
#define GNet_getShowName(n) (n)->n_show_name
#define GNet_numWires(n) (n)->n_wnum
#define GNet_getNBits(n) (n)->n_nbits

#endif
