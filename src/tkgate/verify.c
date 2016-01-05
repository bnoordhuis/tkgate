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

    Last edit by hansen on Sun Dec 21 22:27:18 2008
****************************************************************************/
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <string.h>
#include "tkgate.h"

static int did_message = 0;
static int err_count = 0; 
static NHash *v_wires = 0;

void verify_error(GModuleDef *m,char *msg,...)
{
  extern int quietMode;

  va_list ap;

  if (!quietMode) {
    printf("\n[%s] ",m->m_name);
    va_start(ap,msg);
    vprintf(msg,ap);
    va_end(ap);
    fflush(stdout);

    if (!did_message && TkGate.tcl) {
      message(1,"A verify error has ocurred (further verify errors will be reported only to the console).");
      did_message = 1;
    }
  }

  err_count++;
}

void verify_net(GNet *n,GModuleDef *m)
{
  GWireList *wl;
  GWire **wset;
  int i;
  int num_wires = 0;

  if (n->n_mod != m)
    verify_error(m,"module %s contains net %s claiming to be from module %s",
	   m->m_name,n->n_signame,n->n_mod->m_name);


  for (wl = m->m_wires;wl;wl = wl->wl_next) {
    GWire *w = wl->wl_wire;
    if (!w) continue;
    if (w->net == n) num_wires++;
  }

  wset = (GWire**) ob_malloc(num_wires*sizeof(GWire*),"GWire*[]");
  for (i = 0;i < num_wires;i++) wset[i] = 0;

  for (wl = m->m_wires;wl;wl = wl->wl_next) {
    GWire *w = wl->wl_wire;
    if (!w) continue;
    if (w->net != n) continue;
      
    if (w->nidx < 0 || w->nidx >= num_wires) {
      verify_error(m,"index %d of wire 0x%x on net %s is out of range [0..%d].",
		   w->nidx,w,n->n_signame,num_wires-1);
      continue;
    } 

    if (wset[w->nidx]) {
      verify_error(m,"index %d on net %s is used by multiple wires 0x%x and 0x%x.",
		   w->nidx,n->n_signame,wset[w->nidx],w);
      continue;
    }
    wset[w->nidx] = w;
  }

  for (i = 0;i < num_wires;i++) {
    if (!wset[i])
      verify_error(m,"index %d on net %s is not used.",i,n->n_signame);
  }

  ob_free(wset);
}

void verify_wire(GWire *w,GModuleDef *m)
{
  if (w->gate) {
    int p,n;
    GCElement *g = w->gate;

    if (posongate(w,g,&p,&n) != 0) {
      verify_error(m,"wire 0x%x(%s) falsly claims to be attached to gate %s(%s).",
		   w,w->net->n_signame,g->ename,g->typeinfo->name);
    }
  }
}

void verify_gate(GCElement *g,GModuleDef *m)
{
  int N = GCElement_numPads(g);
  int i;
  NHash *H = new_NHash();

  for (i = 0;i < N;i++) {
    GWire *w;

    for (w = g->wires[i];w;w = w->next) {
      if (NHash_find(H,(unsigned)w)) {
	verify_error(m,"wire 0x%x(%s) attached to gate %s(%s) multiple times.",
		     w,w->net->n_signame,g->ename,g->typeinfo->name);
	continue;
      }
      NHash_insert(H,(unsigned)w,w);
      if (w->gate != g) {
	if (w->gate)
	  verify_error(m,"wire 0x%x(%s) attached to gate %s(%s) is really attached to %s(%s).",
		       w,w->net->n_signame,g->ename,g->typeinfo->name,w->gate->ename,w->gate->typeinfo->name);
	else
	  verify_error(m,"wire 0x%x(%s) attached to gate %s(%s) is really unattached.",
		       w,w->net->n_signame,g->ename,g->typeinfo->name);
      }
    }
  }

  switch (g->typeinfo->Code) {
  case GC_TAP :
    if (g->wires[TAP_IN]->net != g->wires[TAP_OUT]->net)
      verify_error(m,"tap 0x%x(%s) has inconsistant attachments.",
		   g,g->ename,g->wires[TAP_IN]->net->n_signame,g->wires[TAP_OUT]->net->n_signame);
    break;
  }

  delete_NHash(H);
}

void verify_module(GModuleDef *M)
{
  HashElem *E;
  GWireList *wl;

  v_wires = new_NHash();

  for (E = Hash_first(M->m_gates);E;E = Hash_next(M->m_gates,E)) {
    GCElement *g = (GCElement*) HashElem_obj(E);
    verify_gate(g,M);
  }
  for (E = Hash_first(M->m_nets);E;E = Hash_next(M->m_nets,E)) {
    GNet *n = (GNet*) HashElem_obj(E);
    verify_net(n,M);
  }

  for (wl = M->m_wires;wl;wl = wl->wl_next) {
    verify_wire(wl->wl_wire,M);
  }

  delete_NHash(v_wires);
}


int verify_circuit()
{
  HashElem *E;
  extern int quietMode;

  err_count = 0; 

  if (!quietMode) {
    printf("verifying...");
    fflush(stdout);
  }

  for (E = Hash_first(TkGate.circuit->moduleTable);E;E = Hash_next(TkGate.circuit->moduleTable,E)) {
    GModuleDef *M = (GModuleDef*) HashElem_obj(E);
    verify_module(M);
  }

  if (!quietMode) {
    if (err_count > 0) printf("\n");
    printf("[%d inconsistencie(s) found]\n",err_count);
  }

  return err_count;
}

