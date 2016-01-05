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

#define NET_DEBUG 0

Net *new_Net_memory(const char *name,unsigned msb,unsigned lsb,
		    unsigned beginAddr,unsigned endAddr)
{
  Net *n = (Net*) malloc(sizeof(Net));
  int addrBits;

  n->n_name = strdup(name);
  n->n_msb = msb;
  n->n_lsb = lsb;
  n->n_nbits = iabs(msb-lsb) + 1;
  n->n_drivers = 0;
  n->n_type = NT_MEMORY;
  n->n_flags = 0;
  n->n_numDrivers = 0;
  n->n_numMonitors = 0;
  n->n_wfunc = Value_wire;
  List_init(&n->n_posedgeNotify);
  List_init(&n->n_negedgeNotify);

  Memory_init(&n->n_data.memory,beginAddr, endAddr, n->n_nbits,n);

  return n;
}

void delete_Net(Net *n)
{
  free(n->n_name);
  free(n);
}


/*****************************************************************************
 *
 * Create a new Net
 *
 * Parameter:
 *     name		Name of net
 *     ntype		Type code of net
 *     msb		MSB of net
 *     lsb		LSB of net
 *
 * 
 *
 *****************************************************************************/
Net *new_Net(const char *name,nettype_t ntype,unsigned msb,unsigned lsb)
{
  Net *n = (Net*) malloc(sizeof(Net));
  Value *value = Net_getValue(n);

  n->n_name = strdup(name);
  n->n_msb = msb;
  n->n_lsb = lsb;
  n->n_nbits = iabs(msb-lsb) + 1;
  n->n_drivers = 0;
  n->n_type = ntype;
  n->n_flags = 0;
  n->n_numMonitors = 0;
  n->n_numDrivers = 0;
  n->n_wfunc = Value_wire;
  List_init(&n->n_posedgeNotify);
  List_init(&n->n_negedgeNotify);
  Value_init(Net_getValue(n),n->n_nbits);

  if ((n->n_type & NT_P_AND))
    n->n_wfunc = Value_wand;
  else if ((n->n_type & NT_P_OR))
    n->n_wfunc = Value_wor;
  else if ((n->n_type & NT_P_PULL0))
    n->n_wfunc = Value_tri0;
  else if ((n->n_type & NT_P_PULL1))
    n->n_wfunc = Value_tri1;

  switch ((n->n_type & NT_P_REGTYPE_MASK)) {
  case NT_P_REAL :
    value->flags = SF_REAL;
    break;
  case NT_P_INTEGER :
  case NT_P_TIME :
    value->flags = SF_INT;
    break;
  }

  
  if (NT_GET_0STR(ntype) && !NT_GET_1STR(ntype)) {
    Value_zero(value);
    if (NT_GET_0STR(ntype) == NT_P_SUPPLY0) n->n_flags = NA_FIXED;
  } else if (NT_GET_1STR(ntype) && !NT_GET_0STR(ntype)) {
    Value_one(value);
    if (NT_GET_1STR(ntype) == NT_P_SUPPLY1) n->n_flags = NA_FIXED;
  } else if ((ntype & NT_P_WIRE)) {
    Value_float(value);
  } else {
    Value_unknown(value);
  }

  value->permFlags = SF_NETVAL | value->flags;

  return n;
}

/*****************************************************************************
 *
 * Return the local name of a net by trimming of leading '.' components
 *
 * Parameters
 *     n		Net 
 *
 * Returns:		Local name of net
 *
 *****************************************************************************/
const char *Net_getLocalName(Net *n)
{
  char *p = strrchr(n->n_name,'.');
  if (!p) return n->n_name;
  return p+1;
}

/*****************************************************************************
 *
 * Register a trigger as being a posedge listener of a net
 *
 * Parameters
 *     n		Net on which to look for posedge transitions
 *     t		Trigger to be registered.
 *
 *****************************************************************************/
void Net_posedgeListen(Net*n,Trigger*t)
{
  List_addToTail(&n->n_posedgeNotify,t);
}

/*****************************************************************************
 *
 * Register a trigger as being a negedge listener of a net
 *
 * Parameters
 *     n		Net on which to look for negedge transitions
 *     t		Trigger to be registered.
 *
 *****************************************************************************/
void Net_negedgeListen(Net*n,Trigger*t)
{
  List_addToTail(&n->n_negedgeNotify,t);
}

/*****************************************************************************
 *
 * This function is called every time there is a posedge transition on n 
 *
 * Parameters
 *     n		Net on which a posedge transition was observed.
 *
 *****************************************************************************/
void Net_posedgeNotify(Net *n)
{
  ListElem *le;

  if ((n->n_flags & NA_CLOCK))
    EvQueue_clockNotify(Circuit_getQueue(&vgsim.vg_circuit), n, TT_POSEDGE);

  for (le = List_first(&n->n_posedgeNotify);le;le = List_next(&n->n_posedgeNotify,le)) {
    Trigger_fire((Trigger*)ListElem_obj(le));
  }
}

/*****************************************************************************
 *
 * This function is called every time there is a negedge transition on n 
 *
 * Parameters
 *     n		Net on which a negedge transition was observed.
 *
 *****************************************************************************/
void Net_negedgeNotify(Net *n)
{
  ListElem *le;

  if ((n->n_flags & NA_CLOCK))
    EvQueue_clockNotify(Circuit_getQueue(&vgsim.vg_circuit), n, TT_NEGEDGE);

  for (le = List_first(&n->n_negedgeNotify);le;le = List_next(&n->n_negedgeNotify,le)) {
    Trigger_fire((Trigger*)ListElem_obj(le));
  }
}

/*****************************************************************************
 *
 * Chage the value of a net and notify anything that is triggering on this net.
 *
 * Parameters:
 *      n		Net to be set. 
 *      s		New value for net (or NULL if this is an "event" node).
 *
 * Determines the transition edge type based on the current and new values for
 * the net and sets the new value.  If the new value is the same as the current
 * value, nothing is done.  Otherwise, the EDGE, POSEDGE or NEGEDGE listeners
 * as appropriate are notified.
 *
 *****************************************************************************/
void Net_set(Net*n,Value*s)
{
  EvQueue *Q;
  Value *temp_s = 0;

  if (!s) {
    Net_posedgeNotify(n);
    return;
  }

  if ((n->n_type & NT_P_REGTYPE_MASK) == NT_P_REAL) {
    real_t dst, src;
    int src_ok,dst_ok;

    src_ok = Value_toReal(s,&src);
    dst_ok = Value_toReal(Net_getValue(n), &dst);

    if (src_ok == dst_ok && src == dst)
      return;	/* Value not changed */

    if (n->n_numMonitors > 0) {
      Q = Circuit_getQueue(&vgsim.vg_circuit);
      EvQueue_monitoredChangeNotify(Q);
    }

    if (src_ok)
      Value_convertNaN(Net_getValue(n));
    else
      Value_convertR(Net_getValue(n), src);

    Net_posedgeNotify(n);
    return;
  }

  if ((n->n_flags & NA_FIXED)) return;

  if ((s->flags & SF_REAL)) {
    real_t v_f;
    unsigned v_u;
    if (Value_toReal(s,&v_f) != 0)
      v_f = 0;
    v_u = (unsigned) v_f;
    temp_s = new_Value(Value_nbits(s));
    s = temp_s;
    Value_convertI(s, v_u);
  }

  switch (Value_transitionType(Net_getValue(n), s)) {
  case TT_NONE :
    break;
  case TT_EDGE :
  case TT_POSEDGE :
    if (n->n_numMonitors > 0) {
      Q = Circuit_getQueue(&vgsim.vg_circuit);
      EvQueue_monitoredChangeNotify(Q);
    }
    Value_copy(Net_getValue(n), s);
    Net_posedgeNotify(n);
    break;
  case TT_NEGEDGE :
    if (n->n_numMonitors > 0) {
      Q = Circuit_getQueue(&vgsim.vg_circuit);
      EvQueue_monitoredChangeNotify(Q);
    }
    Value_copy(Net_getValue(n), s);
    Net_negedgeNotify(n);
    break;
  }

  switch ((n->n_type & NT_P_REGTYPE_MASK)) {
  case  NT_P_INTEGER :
  case  NT_P_TIME :
    Net_getValue(n)->flags = SF_INT;    
    break;
  default :
    Net_getValue(n)->flags = 0;    
    break;
  }

  if (temp_s)
    delete_Value(temp_s);
  
}

/*****************************************************************************
 *
 * Change the value of a subrange of net and notify anything that is triggering on this net.
 *
 * Parameters:
 *      n		Net to be set. 
 *      s		New value for net
 *
 * Determines the transition edge type based on the current and new values for
 * the net and sets the new value.  If the new value is the same as the current
 * value, nothing is done.  Otherwise, the EDGE, POSEDGE or NEGEDGE listeners
 * as appropriate are notified.
 *
 *****************************************************************************/
void Net_setRange(Net*n,int nlsb,Value*s,int smsb,int slsb)
{
  EvQueue *Q;

  if ((n->n_type & NT_P_REGTYPE_MASK) == NT_P_REAL) {
    Value_convertNaN(Net_getValue(n));
    return;
  }

  if ((n->n_flags & NA_FIXED)) return;

  switch (Value_copyRange(Net_getValue(n), nlsb, s, smsb, slsb)) {
  case TT_EDGE :
  case TT_POSEDGE :
    if (n->n_numMonitors > 0) {
      Q = Circuit_getQueue(&vgsim.vg_circuit);
      EvQueue_monitoredChangeNotify(Q);
    }
    Net_posedgeNotify(n);
    break;
  case TT_NEGEDGE :
    if (n->n_numMonitors > 0) {
      Q = Circuit_getQueue(&vgsim.vg_circuit);
      EvQueue_monitoredChangeNotify(Q);
    }
    Net_negedgeNotify(n);
    break;
  case TT_NONE :
    break;
  }
}

/*****************************************************************************
 *
 * Set a net to the unknown state and notify triggers.
 *
 * Parameters:
 *      n		Net that is transitioning to the unknown state.
 *
 *****************************************************************************/
void Net_makeUnknown(Net *n)
{
  Value_unknown(Net_getValue(n));
  Net_posedgeNotify(n);
}


/*****************************************************************************
 *
 * Print a human-readable string for a net
 *
 * Parameters:
 *      n		Net to be printed
 *      f		File on which to print
 *
 *****************************************************************************/
void Net_print(Net *n,FILE *f)
{

  if (n->n_type != MEMORY) {
    char buf[STRMAX];

    NT_getStr(n->n_type,buf);
    fprintf(f,"%s [%d:%d] %s",buf,n->n_msb,n->n_lsb,n->n_name);
  } else
    fprintf(f,"reg [%d:%d] %s[]",n->n_msb,n->n_lsb,n->n_name);
}

/*****************************************************************************
 *
 * Create a new driver on a net and return the Value object for it.
 *
 * Parameters:
 *     n		Net on which to create driver
 *
 * Returns:		Driver ID of newly created driver.
 *
 *****************************************************************************/
int Net_addDriver(Net*n)
{
  int id = n->n_numDrivers++;

  if (id == 0) 
    n->n_drivers = (Value**) malloc(sizeof(Value*)*n->n_numDrivers);
  else
    n->n_drivers = (Value**) realloc(n->n_drivers, sizeof(Value*)*n->n_numDrivers);

  n->n_drivers[id] = new_Value(Net_nbits(n));
  Value_float(n->n_drivers[id]);

  return id;
}

/*****************************************************************************
 *
 *  Notify net that driver with id 'id' has changed.
 *
 *  Parameters:
 *       n		Net on which a driver change has occured
 *       id		ID of driver thange changed (currently unused)
 *
 *****************************************************************************/
void Net_driverChangeNotify(Net *n,int id)
{
  Value *s;
  int i;

#if NET_DEBUG
  printf("   Net_driverChange %s<%d> was ",n->n_name,n->n_numDrivers);
  Value_print(Net_getValue(n),stdout);
  printf(" ");
#endif

  switch (n->n_numDrivers) {
  case 0 :
    s = new_Value(Net_nbits(n));
    Value_float(s);
    if ((n->n_type & NT_P_TRIREG))
      Value_trireg(s,s,&n->n_data.value);
    Net_set(n,s);
    delete_Value(s);
    break;
  case 1 :
    s = n->n_drivers[0];
    if ((n->n_type & NT_P_TRIREG))
      Value_trireg(s,s,&n->n_data.value);
    Net_set(n,s);
    /* no delete needed */
    break;
  case 2:
    s = new_Value(Net_nbits(n));
    (*n->n_wfunc)(s,n->n_drivers[0],n->n_drivers[1]);
    if ((n->n_type & NT_P_TRIREG))
      Value_trireg(s,s,&n->n_data.value);
    Net_set(n,s);
    delete_Value(s);
    break;
  default :
    s = new_Value(Net_nbits(n));
    (*n->n_wfunc)(s,n->n_drivers[0],n->n_drivers[1]);
    for (i = 2;i < n->n_numDrivers;i++)
      (*n->n_wfunc)(s,s,n->n_drivers[i]);
    if ((n->n_type & NT_P_TRIREG))
      Value_trireg(s,s,&n->n_data.value);
    Net_set(n,s);
    delete_Value(s);
    break;
  }
}
#if 0
#if NET_DEBUG|1
    printf("   Net_driverChange %s<%d> (%s) was ",n->n_name,n->n_numDrivers,
	   (n->n_type & NT_P_TRIREG) ? "trireg" : "non-trireg");
    Value_print(Net_getValue(n),stdout);
    printf("\n");
#endif

  if (n->n_numDrivers == 1) {
    Value *s = n->n_drivers[0];

    Net_set(n,s);
  } else {
    Value *s = new_Value(Net_nbits(n));
    int i;

    Value_float(s);

    for (i = 0;i < n->n_numDrivers;i++)
      (*n->n_wfunc)(s,s,n->n_drivers[i]);

    /*
     * If this is a trireg net, merge driver result with the current net value.
     */
    if ((n->n_type & NT_P_TRIREG))
      Value_trireg(s,s,&n->n_data.value);

    Net_set(n,s);
    delete_Value(s);
  }
#endif

void Net_reportValue(Net *n,const char *who,const char *name,Circuit *c)
{
  EvQueue *Q = c->c_evQueue;
  char buf[STRMAX];

  if (!name) name = n->n_name;


  Value_getvstr(Net_getValue(n),buf);
  if (who)
    vgio_printf("tell %s %s %s @ %llu\n",who,name,buf,EvQueue_getCurTime(Q));
  else
    vgio_printf("valueof %s %s @ %llu\n",name,buf,EvQueue_getCurTime(Q));
}

void Net_memSet(Net *n, unsigned addr, Value *data)
{
  Memory_put(&n->n_data.memory, addr, data);
  Memory_accessNotify(&n->n_data.memory, addr, 1);
  Net_posedgeNotify(n);
}

void Net_memSetRange(Net *n, unsigned addr,int nLsb,Value *data,int vMsb,int vLsb)
{
  Memory_putRange(&n->n_data.memory, addr, nLsb, data, vMsb, vLsb);
  Memory_accessNotify(&n->n_data.memory, addr, 1);
  Net_posedgeNotify(n);
}

SNetMap *new_SNetMap(Net *net,int netLsb,int snLsb,int width)
{
  abort();
  return 0;
}
