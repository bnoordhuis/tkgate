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

    Last edit by hansen on Sat Apr 23 22:53:00 2005
****************************************************************************/
#ifndef __igenerate_h
#define __igenerate_h

/*
 * Code for the sides of a module instance.  These should match the
 * w->orient code for wires attached to that side.  
 */
#define PSIDE_UNKNOWN	-1
#define PSIDE_RIGHT	0
#define PSIDE_TOP	1
#define PSIDE_LEFT	2
#define PSIDE_BOTTOM	3
#define PSIDE_MAX	4

/*****************************************************************************
 *
 * Temporary descriptor for interface generator port description.
 *
 * Objects of this type are generally temporary and thus are created/destroyed
 * with malloc/free instead of ob_malloc/ob_free.
 *
 *****************************************************************************/
typedef struct {
  char		*igp_name;		/* Name of port */
  int		igp_iotype;		/* I/O type of port */
  int		igp_side;		/* Side of block for port */
  int		igp_bits;		/* Number of bits */
  int		igp_pos;		/* Port position */
  int		igp_offset;		/* Offset of this pin as numerator and denominator */

  /* These members are used only for symbol blocks */
  int		igp_keep;		/* Keep the position of the port */
  int		igp_x,igp_y;		/* Position of port */
} IGPort;

IGPort **igen_getInterfacePorts(GModuleDef *M,int *N);
void igen_freePortList(IGPort **portList,int n);
int igen_generate_initial(GModuleDef *M);

int igen_command(const char *op,const char *mod_name,int argc,const char *argv[]);

#endif

