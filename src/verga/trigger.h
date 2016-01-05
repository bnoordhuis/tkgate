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
#ifndef __trigger_h
#define __trigger_h

/*****************************************************************************
 *
 * Trigger - event triggering class
 *
 *****************************************************************************/
struct Trigger_str {
  List/*Net*/		*t_posedges;	/* posedge triggers */
  List/*Net*/		*t_negedges;	/* negedge triggers */
  Event			*t_events;	/* Events waiting for this trigger */
  Trigger		*t_next;	/* Next in hash bucket */
};

Trigger *new_Trigger(List *posedges,List *negedges,Trigger *next);
unsigned Trigger_getHashCode(List *posedges,List *negedges);
unsigned Trigger_sortAndGetHashCode(List *posedges,List *negedges);
Trigger *Trigger_getTrigger(Trigger **tlist,List *posedges,List *negedges);
void Trigger_print(Trigger*,FILE *f);
void Trigger_fire(Trigger*);
void Trigger_enqueue(Trigger *t, Event *e);
#endif

