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
#include "thyme.h"

/*****************************************************************************
 *
 * Comparison function for elements in list of nets
 *
 * Parameters:
 *     vA,vB		Pointer to the pointers to the elements.
 *
 * Returns:		<0, 0 or >0 depending on comparison results.
 *
 * This function is used to ensure that lists of nets in triggers are in a
 * canonical order.  We don't really care what that order is as long as it
 * is the same for any list of nets.  For this reason we just sort on the
 * addresses of the Nets in memory. 
 *
 *
 *****************************************************************************/
static int addrCmp(const void *vA,const void *vB)
{
  void *A = *(void**)vA;
  void *B = *(void**)vB;

  if (A < B)
    return -1;
  else if (A > B)
    return 1;
  else
    return 0;
}

/*****************************************************************************
 *
 * Create a new trigger object.
 *
 * Parameters:
 *     posedges		Sorted list of Net* which invoke this trigger on a posedge
 *     negedges		Sorted list of Net* which invoke this trigger on a negedge
 *     next		Next trigger in hash bucket
 *
 * Returns:		Newly created Trigger object.
 *
 * Single bit signals that do not have a qualifier will be placed in both
 * the posedges and negedges lists.  Multi-bit signals will be placed in
 * only the posedges list and be triggered on any change.
 *
 *****************************************************************************/
Trigger *new_Trigger(List *posedges,List *negedges,Trigger *next)
{
  Trigger *t = (Trigger*) malloc(sizeof(Trigger));

  t->t_posedges = posedges ? copy_List(posedges) : new_List();
  t->t_negedges = negedges ? copy_List(negedges) : new_List();
  t->t_events = 0;
  t->t_next = 0;

  return t;
}

/*****************************************************************************
 *
 * Get the hash code for a pair of lists.
 *
 * Parameters:
 *     posedges		Sorted list of posedge Net* 
 *     negedges		Sorted list of negedge Net* 
 *
 * Returns:		Hash code representing lists.
 *
 * The value returned is a hash value representing the pair of lists.  For any
 * identical pairs of lists, the same hash code will always be returned.  For
 * unmatching pairs of lists, the value returned should "usually" be different. 
 *
 *****************************************************************************/
unsigned Trigger_getHashCode(List *posedges,List *negedges)
{
  ListElem *le;
  unsigned HC = 0;

  for (le = List_first(posedges);le;le = List_next(posedges,le)) {
    unsigned X = (unsigned) ListElem_obj(le);
    HC = ((HC+X) << 8) + X*X + (HC>>8);
  }
  for (le = List_first(negedges);le;le = List_next(negedges,le)) {
    unsigned X = (unsigned) ListElem_obj(le);
    HC = ((HC+X) << 8) + X*X + (HC>>8);
  }

  return HC;
}

/*****************************************************************************
 *
 * Sort both lists and get the hash code using Trigger_getHashCode
 *
 * Parameters:
 *     posedges		Unsorted list of posedge Net* 
 *     negedges		Unsorted list of negedge Net* 
 *
 * Returns:		Hash code representing lists.
 *
 *****************************************************************************/
unsigned Trigger_sortAndGetHashCode(List *posedges,List *negedges)
{
  List_sort(posedges,addrCmp);
  List_sort(negedges,addrCmp);
  return Trigger_getHashCode(posedges,negedges);
}


/*****************************************************************************
 *
 * Test a trigger to see if it exactly matches the posesges and negedges list.
 * it is assumed that the lists are sorted.
 *
 * Parameters:
 *     t		Trigger to compare
 *     posedges		Sorted list of posedge sensitive nets
 *     negedges		Sorted list of negedge sensitive nets
 *
 *****************************************************************************/
int Trigger_match(Trigger *t,List *posedges,List *negedges)
{
  ListElem *le1, *le2;

  if (List_numElems(t->t_posedges) != List_numElems(posedges)) return 0;
  if (List_numElems(t->t_negedges) != List_numElems(negedges)) return 0;

  le1 = List_first(t->t_posedges);
  le2 = List_first(posedges);
  while (le1 && le2) {
    if (ListElem_obj(le1) != ListElem_obj(le2))
      return 0;
    le1 = List_next(t->t_posedges,le1);
    le2 = List_next(posedges,le2);
  }

  le1 = List_first(t->t_negedges);
  le2 = List_first(negedges);
  while (le1 && le2) {
    if (ListElem_obj(le1) != ListElem_obj(le2))
      return 0;
    le1 = List_next(t->t_negedges,le1);
    le2 = List_next(negedges,le2);
  }

  return 1;
}


/*****************************************************************************
 *
 * Find/create a the specified trigger.
 *
 * Parameters:
 *     tlist		Pointer to head of list of triggers with matching hashcodes
 *     posedges		Sorted list of posedge sensitive Net*
 *     negedges		Sorted list of negedge sensitive Net*
 *
 * Looks through the list *tlist of triggers for a trigger with the same
 * posedges/negedges as those passed.  If the trigger is found it is returned
 * and if it is not found a new trigger is created and it is inserted into
 * *tlist.  Assuming Trigger_getHashCode() returns good values, the *tlist
 * should almost always have either 0 or 1 elements, and when it has 1 element
 * the lookup should almost always succeed. 
 *
 *****************************************************************************/
Trigger *Trigger_getTrigger(Trigger **tlist,List *posedges,List *negedges)
{
  Trigger *t;
  ListElem *le;

  for (t = *tlist;t;t = t->t_next)
    if (Trigger_match(t,posedges,negedges))
      return t;

  *tlist = t = new_Trigger(posedges,negedges,*tlist);

  for (le = List_first(posedges);le;le = List_next(posedges,le)) {
    Net_posedgeListen((Net*) ListElem_obj(le),t);
  }

  for (le = List_first(negedges);le;le = List_next(negedges,le)) {
    Net_negedgeListen((Net*) ListElem_obj(le),t);
  }

  return t;
}

/*****************************************************************************
 *
 * Print representation of a trigger.
 *
 * Parameters:
 *     t		Trigger to print.
 *     f		File on which to print representation.
 *
 *****************************************************************************/
void Trigger_print(Trigger *t,FILE *f)
{
  ListElem *le;

  fprintf(f,"@id<%p> (",t);
  for (le = List_first(t->t_posedges);le;le = List_next(t->t_posedges,le)) {
    Net *n = (Net*)ListElem_obj(le);

    if (Net_nbits(n) > 1)
      fprintf(f," %s",Net_getName(n));
    else
      fprintf(f," posedge %s",Net_getName(n));
  }
  for (le = List_first(t->t_negedges);le;le = List_next(t->t_negedges,le)) {
    Net *n = (Net*)ListElem_obj(le);

    fprintf(f," negedge %s",Net_getName(n));
  }
  fprintf(f,")");
}

/*****************************************************************************
 *
 * Fire a trigger and move all its queued events to the global time queue.
 *
 * Parameters:
 *     t		Trigger to fire.
 *
 * When this function is called, any events that are stored in the trigger
 * are moved to the main simulator time queue at the current time.
 *
 *****************************************************************************/
void Trigger_fire(Trigger *t)
{
  EvQueue *Q = Circuit_getQueue(&vgsim.vg_circuit);

  while (t->t_events) {
    Event *e = t->t_events; 
    t->t_events = e->ev_base.eb_next;

    EvQueue_enqueueAfter(Q,e,0);
  }
}

/*****************************************************************************
 *
 * Queue an event to be invoked when the trigger is fired.
 *
 * Parameters:
 *     t		Trigger in which to place event
 *     e		Event to be invoked when trigger fires.
 *
 *****************************************************************************/
void Trigger_enqueue(Trigger *t, Event *e)
{
  e->ev_base.eb_next = t->t_events;
  t->t_events = e;
}

