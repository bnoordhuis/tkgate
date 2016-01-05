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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/*****************************************************************************
 *
 * Object memory functions possibly used here.
 *
 *****************************************************************************/
void *ob_malloc(int,char*);
void *ob_calloc(int,int,char*);
void ob_free(void*);
char *ob_strdup(const char*);
void ob_touch(void*);

typedef void *malloc_f(int,char*);
typedef void *calloc_f(int,int,char*);
typedef void free_f(void*);
typedef char *strdup_f(const char*);
typedef void touch_f(void*);

typedef struct {
  malloc_f *hv_malloc;
  calloc_f *hv_calloc;
  free_f   *hv_free;
  strdup_f *hv_strdup;
  touch_f  *hv_touch;
} HashVTable;

/*
 * Standard function replacements for object memory handling.
 */
static void *h_malloc(int size,char *name) { return malloc(size); }
static void *h_calloc(int n,int size,char *name) { return calloc(n,size);}
static void nop_touch(void *p) {}

#define HT_MALLOC(H,size,name)   (*((HashVTable*)H->vtable)->hv_malloc)(size,name)
#define HT_CALLOC(H,n,size,name) (*((HashVTable*)H->vtable)->hv_calloc)(n,size,name)
#define HT_FREE(H,p) 		 (*((HashVTable*)H->vtable)->hv_free)(p)
#define HT_STRDUP(H,s)		 (*((HashVTable*)H->vtable)->hv_strdup)(s)
#define HT_TOUCH(H,p)		 (*((HashVTable*)H->vtable)->hv_touch)(p)


static HashVTable obj_mmgr = {
  ob_malloc, ob_calloc, ob_free, ob_strdup, ob_touch
};

static HashVTable normal_mmgr = {
  h_malloc, h_calloc, free, strdup, nop_touch
};


/*
   Round N up to a power of 2.
*/
static unsigned roundup(unsigned N)
{
  unsigned P = N & (N ^ -N);

  if (P) {
    do {
      N = P;
      P = N & (N ^ -N);
    } while (P);
    N <<= 1;
  }
  return N;
} 

static unsigned int inthash(unsigned int key)
{
  key += 123456;
  key += (key << 12);
  key ^= (key >> 22);
  key += (key << 4);
  key ^= (key >> 9);
  key += (key << 10);
  key ^= (key >> 2);
  key += (key << 7);
  key ^= (key >> 12);

  return key;
}

unsigned computestrhash(const char *s)
{
  unsigned N = 0;
  unsigned H = 0;
  int i;

  for (i = 0;*s;s++, i++) {
    N <<= 8;
    N |= (unsigned char) *s;
    if ((i & 3) == 3)
      H = inthash(H + N);
  } 
  H = inthash(H + N);
  return H;
}


static HashElem *new_SHashElem(Hash *H,const char *key,unsigned hcode, void *val)
{	
  HashElem *E = (HashElem*) HT_MALLOC(H,sizeof(HashElem),"HashElem");

  E->key.s = HT_STRDUP(H,key);
  E->hashcode = hcode;
  E->value = val;
  E->next = 0;

  return E;
}

static HashElem *new_NHashElem(Hash *H,int key,unsigned hcode,void *val)
{	
  HashElem *E = (HashElem*) HT_MALLOC(H,sizeof(HashElem),"HashElem");

  E->key.d = key;
  E->hashcode = hcode;
  E->value = val;
  E->next = 0;

  return E;
}

void SHashElem_uninit(HashElem *E,Hash *H)
{
  HT_FREE(H,E->key.s);
}

Hash *new_Hash(int use_ob)
{
  Hash *H;

  if (use_ob) {
    H = (Hash*) ob_malloc(sizeof(Hash),"Hash");
    Hash_init(H,use_ob);
  } else {
    H = (Hash*) malloc(sizeof(Hash));
    Hash_init(H,use_ob);
  }

  return H;
}

void delete_Hash(Hash *H,HashElemDelFunc *hdel)
{
  Hash_uninit(H,hdel);
  HT_FREE(H,H);
}

void Hash_init(Hash *H,int use_ob)
{
  if (use_ob) ob_touch(H);
  if (use_ob)
    H->vtable = &obj_mmgr;
  else
    H->vtable = &normal_mmgr;

  H->size = roundup(INITIAL_HASHSIZE);
  H->mask = H->size-1; 
  H->num = 0;
  H->loop_ok = 0;
  H->elems = (HashElem**) HT_CALLOC(H,H->size,sizeof(HashElem*),"HashElem[]");
  if (use_ob) ob_touch(H->elems);
}

void Hash_uninit(Hash *H,HashElemDelFunc *hdel)
{
  int i;

  HT_TOUCH(H,H);
  HT_TOUCH(H,H->elems);

  for (i = 0;i < H->size;i++) {
    HashElem *E,*N;
    for (E = H->elems[i];E;E = N) {
      N = E->next;
      if (hdel)
	(*hdel)(E,H);
      HT_FREE(H,E);
    }
  }
  HT_FREE(H,H->elems);
}

HashElem *Hash_first(Hash *H)
{
  int i;

  H->loop_ok = 1;		/* This debugging flag does not need an ob_touch */
  for (i = 0;i < H->size;i++)
    if (H->elems[i])
      return H->elems[i];
  return 0;
}

HashElem *Hash_next(Hash *H,HashElem *E)
{
  int A;

  if (!H->loop_ok) {
    printf("echo Illegal sequential hash table access.\n");
    fflush(stdout);
  }


  if (E->next) return E->next;

  for (A = (E->hashcode & H->mask) + 1;A < H->size;A++)
    if (H->elems[A])
      return H->elems[A];

  return 0;
}

/*
   Double the size of a hash table
*/
void Hash_grow(Hash *H)
{
  Hash_resize(H,(H->size << 1));
}

/*
 * Request a specific size for a hash table (rounded up to power of two)
 */
void Hash_resize(Hash *H,int reqSize)
{
  int S = H->size;
  HashElem **nea;
  int i;

  reqSize = roundup(reqSize);

  HT_TOUCH(H,H);
  HT_TOUCH(H,H->elems);

  H->size = reqSize;
  H->mask = H->size - 1;
  nea = (HashElem**) HT_CALLOC(H,H->size,sizeof(HashElem*),"HashElem[]");

  for (i = 0;i < S;i++) {
    HashElem *E;
    while ((E = H->elems[i])) {
      unsigned A = E->hashcode & H->mask;
      H->elems[i] = E->next;
      HT_TOUCH(H,E);
      E->next = nea[A];
      nea[A] = E;
    }
  }
  HT_FREE(H,H->elems);
  H->elems = nea;
}

void Hash_removeElem(Hash *H,int A,HashElem *E)
{
  HashElem *P;

  HT_TOUCH(H,H);

  H->num--;

  H->loop_ok = 0;

  if (H->elems[A] == E) {
    HT_TOUCH(H,H->elems);
    H->elems[A] = E->next;
    return;
  }

  for (P = H->elems[A];P->next != E;P = P->next);

  HT_TOUCH(H,P);
  P->next = E->next;
}

void *SHash_find(Hash *H,const char *key)
{
  unsigned HC = computestrhash(key);
  unsigned A = HC & H->mask;
  HashElem *E;

  for (E = H->elems[A];E;E = E->next)
    if (E->hashcode == HC && strcmp(E->key.s,key) == 0)
      return E->value;
  return 0;
}

int SHash_insert(Hash *H,const char *key,void* val)
{
  unsigned HC = computestrhash(key);
  unsigned A = HC & H->mask;
  HashElem *E;

  HT_TOUCH(H,H);

  H->loop_ok = 0;
  for (E = H->elems[A];E;E = E->next)
    if (E->hashcode == HC && strcmp(E->key.s,key) == 0)
      return -1;

  HT_TOUCH(H,H->elems);
  E = new_SHashElem(H,key,HC,val);
  E->next = H->elems[A];
  H->elems[A] = E;
  H->num++;
  if (H->size*HASH_MAXLOAD < H->num)
    Hash_grow(H);

  return 0;
}

int SHash_replace(Hash *H,const char *key,void* val)
{
  unsigned HC = computestrhash(key);
  unsigned A = HC & H->mask;
  HashElem *E;

  HT_TOUCH(H,H);
  HT_TOUCH(H,H->elems);

  H->loop_ok = 0;
  for (E = H->elems[A];E;E = E->next)
    if (E->hashcode == HC && strcmp(E->key.s,key) == 0) {
      HT_TOUCH(H,E);
      E->value = val;
      return 1;
    }

  E = new_SHashElem(H,key,HC,val);
  E->next = H->elems[A];
  H->elems[A] = E;
  H->num++;
  if (H->size*HASH_MAXLOAD < H->num)
    Hash_grow(H);

  return 0;
}

int SHash_remove(Hash *H,const char *key)
{
  unsigned HC = computestrhash(key);
  unsigned A = HC & H->mask;
  HashElem *E;

  HT_TOUCH(H,H);

  H->loop_ok = 0;
  for (E = H->elems[A];E;E = E->next)
    if (E->hashcode == HC && strcmp(E->key.s,key) == 0) {
      Hash_removeElem(H,A,E);
      HT_FREE(H,E->key.s);
      HT_FREE(H,E);
      return 0;
    }

  return -1;
}

void Hash_flush(Hash *H,HashElemDelFunc *hdel)
{
  int i;

  HT_TOUCH(H,H);
  HT_TOUCH(H,H->elems);

  H->loop_ok = 0;

  for (i = 0;i < H->size;i++) {
    HashElem *E = H->elems[i];
    HashElem *N;
    for (;E;E = N) {
      N = E->next;
      if (hdel) (*hdel)(E,H);
      HT_FREE(H,E);
    }
    H->elems[i] = 0;
  }
  H->num = 0;
}



void *NHash_find(Hash *H,int key)
{
  unsigned HC = inthash(key);
  unsigned A = HC & H->mask;
  HashElem *E;

  for (E = H->elems[A];E;E = E->next)
    if (E->key.d == key)
      return E->value;
  return 0;
}

int NHash_insert(Hash *H,int key,void* val)
{
  unsigned HC = inthash(key);
  unsigned A = HC & H->mask;
  HashElem *E;

  HT_TOUCH(H,H);
  HT_TOUCH(H,H->elems);

  H->loop_ok = 0;

  for (E = H->elems[A];E;E = E->next)
    if (E->key.d == key)
      return -1;

  E = new_NHashElem(H,key,HC,val);
  E->next = H->elems[A];
  H->elems[A] = E;
  H->num++;
  if (H->size*HASH_MAXLOAD < H->num)
    Hash_grow(H);

  return 0;
}

int NHash_replace(Hash *H,int key,void* val)
{
  unsigned HC = inthash(key);
  unsigned A = HC & H->mask;
  HashElem *E;

  HT_TOUCH(H,H);
  HT_TOUCH(H,H->elems);

  H->loop_ok = 0;

  for (E = H->elems[A];E;E = E->next)
    if (E->key.d == key) {
      E->value = val;
      return 0;
    }

  E = new_NHashElem(H,key,HC,val);
  E->next = H->elems[A];
  H->elems[A] = E;
  H->num++;
  if (H->size*HASH_MAXLOAD < H->num)
    Hash_grow(H);

  return 0;
}

int NHash_remove(Hash *H,int key)
{
  unsigned HC = inthash(key);
  unsigned A = HC & H->mask;
  HashElem *E;

  HT_TOUCH(H,H);
  H->loop_ok = 0;

  for (E = H->elems[A];E;E = E->next)
    if (E->key.d == key) {
      Hash_removeElem(H,A,E);
      HT_FREE(H,E);
      return 0;
    }

  return -1;
}
