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
#include "list.h"

List *new_List()
{
  List *L = (List*) malloc(sizeof(List));

  List_init(L);

  return L;
}

void delete_List(List *L)
{
  List_uninit(L);
  free(L);
}


List *copy_List(List *L)
{
  List *CL = (List*) malloc(sizeof(List));
  ListElem *E;

  List_init(CL);
  for (E = List_first(L);E;E = List_next(L,E)) {
    List_addToTail(CL, ListElem_obj(E));
  }

  return CL;
}

void List_init(List *L)
{
  L->num = 0;
  L->first = L->last = 0;
}

void List_flush(List *L)
{
  ListElem *E,*N;

  for (E = List_first(L);E;E = N) {
    N = List_next(L,E);
    free(E);
  }
  List_init(L);
}

void List_addToHead(List *L,void *p)
{
  ListElem *E = (ListElem*)malloc(sizeof(ListElem));

  if (L->first) L->first->prev = E;
  E->obj = p;
  E->next = L->first;
  E->prev = 0;
  L->first = E;
  if (!L->last) L->last = E;
  L->num++;
}

void List_addToTail(List *L,void *p)
{
  ListElem *E = (ListElem*)malloc(sizeof(ListElem));

  if (L->last) L->last->next = E;
  E->obj = p;
  E->prev = L->last;
  E->next = 0;
  L->last = E;
  if (!L->first) L->first = E;
  L->num++;
}

void *List_popHead(List *L)
{
  ListElem *E = L->first;
  void *p;

  if (!E) return 0;
  p = E->obj;
  L->first = E->next;
  if (L->first)
    L->first->prev = 0;
  else
    L->last = 0;
  free(E);
  L->num--;

  return p;
}

void *List_popTail(List *L)
{
  ListElem *E = L->last;
  void *p;

  if (!E) return 0;
  p = E->obj;
  L->last = E->prev;
  if (L->last)
    L->last->next = 0;
  else
    L->first = 0;
  free(E);
  L->num--;

  return p;
}

void *List_remove(List *L,ListElem *E)
{
  if (E->prev)
    E->prev->next = E->next;
  else {
    L->first = E->next;
    L->first->prev = 0;
  }

  if (E->next)
    E->next->prev = E->prev;
  else {
    L->last = E->prev;
    L->last->next = 0;
  }
  L->num--;
  free(E);
  return L;
}

void List_append(List *L,List *A)
{
  ListElem *E;

  for (E = List_first(A);E;E = List_next(A,E)) {
    List_addToTail(L,ListElem_obj(E));
  }
}
	 

/*
  get the nth element in list
 */
void *List_nth(List *L,int n)
{
  ListElem *E;

  for (E = List_first(L);E && n-- > 0;E = List_next(L,E));

  return E ? ListElem_obj(E) : 0;
}

void *List_find(List *L,const void *obj,elemcmp_f *cmpfunc)
{
  ListElem *le;

  for (le = List_first(L);le;le = List_next(L,le)) {
    void *list_obj = ListElem_obj(le);

    if ((*cmpfunc)(&list_obj,&obj) == 0)
      return list_obj;
  }

  return 0;
}

void List_sort(List *L,elemcmp_f *cmpfunc)
{
  if (List_numElems(L) > 1) {
    void **a = (void**) malloc(sizeof(void*)*List_numElems(L));
    ListElem *le;
    int i;

    i = 0;
    for (le = List_first(L);le;le = List_next(L,le))
      a[i++] = ListElem_obj(le);

    qsort(a,List_numElems(L),sizeof(void*),cmpfunc);

    i = 0;
    for (le = List_first(L);le;le = List_next(L,le))
      le->obj = a[i++];

    free(a);
  }
}

