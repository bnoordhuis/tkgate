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
#ifndef __list_h
#define __list_h

typedef struct listelem ListElem;
typedef struct list List;

typedef int elemcmp_f(const void*,const void*);

struct listelem {
  ListElem	*next,*prev;
  void		*obj;
};

struct list {
  int		num;
  ListElem	*first,*last;
};

List *new_List();
void delete_List(List*);
List *copy_List(List*);

void List_init(List*);
void List_flush(List*);

void List_sort(List*L,elemcmp_f*);

void List_append(List*L,List*A);

void List_addToHead(List*,void*);
void List_addToTail(List*,void*);
void *List_popHead(List*);
void *List_popTail(List*);
void *List_remove(List*,ListElem*);
void *List_nth(List*,int);

#define List_uninit(L)	List_flush(L)
#define List_first(L)	(L)->first
#define List_last(L)	(L)->last
#define List_numElems(L) (L)->num
#define List_next(L,E)	(E)->next
#define List_prev(L,E)	(E)->prev

#define ListElem_obj(E)	(E)->obj

void *List_find(List *L,const void *obj,elemcmp_f*);

#endif
