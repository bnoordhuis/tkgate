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

#define DEBUG_QUEUE 0
#define DEBUG_EVENT 0

extern int do_commandEcho;

static Event *event_freelist = 0;

void EvThread_process(EvThread *e,EvQueue *q);
void EvNet_process(EvNet *e,EvQueue *q);
void EvNet_uninit(EvNet *e);
void EvDriver_process(EvDriver *e,EvQueue *q);
void EvDriver_uninit(EvDriver *e);
void EvStrobe_process(EvStrobe *e,EvQueue *q);
void EvStrobe_uninit(EvStrobe *e);
void EvProbe_process(EvProbe *e,EvQueue *q);
void EvProbe_uninit(EvProbe *e);
void EvMem_process(EvMem *e,EvQueue *q);
void EvMem_uninit(EvMem *e);

static EventVTable evthread_vtable = {
  EV_THREAD,
  (EventProcess_f*) EvThread_process,
  (EventUninit_f*) 0,
};

static EventVTable evnet_vtable = {
  EV_NET,
  (EventProcess_f*) EvNet_process,
  (EventUninit_f*) EvNet_uninit,
};

static EventVTable evstrobe_vtable = {
  EV_STROBE,
  (EventProcess_f*) EvStrobe_process,
  (EventUninit_f*) EvStrobe_uninit,
};

static EventVTable evprobe_vtable = {
  EV_PROBE,
  (EventProcess_f*) EvProbe_process,
  (EventUninit_f*) EvProbe_uninit,
};

static EventVTable evdriver_vtable = {
  EV_DRIVER,
  (EventProcess_f*) EvDriver_process,
  (EventUninit_f*) EvDriver_uninit,
};

static EventVTable evmem_vtable = {
  EV_MEM,
  (EventProcess_f*) EvMem_process,
  (EventUninit_f*) EvMem_uninit,
};



/*****************************************************************************
 *
 * Insert an event E into the sorted list Q.  This insert is only used for "overflow"
 * with lead times exceeding the size of the time wheel.
 *
 *****************************************************************************/
static Event *EventList_insert(Event *Q,Event *E)
{
  if (!Q) {
    E->ev_base.eb_next = 0;
    return E;
  }

  if (E->ev_base.eb_time < Q->ev_base.eb_time) {
    E->ev_base.eb_next = Q;
    return E;
  }

  Q->ev_base.eb_next = EventList_insert(Q->ev_base.eb_next,E);

  return Q;
}

/*****************************************************************************
 *
 * Remove an event E from the list Q.
 *
 *****************************************************************************/
static Event *EventList_remove(Event *Q,Event *E)
{
  if (!Q) return 0;

  if (Q == E)
    return E->ev_base.eb_next;
  else {
    Q->ev_base.eb_next = EventList_remove(Q->ev_base.eb_next,E);
    return Q;
  }
}


void SQueue_init(SQueue *Q)
{
  Q->sq_head = Q->sq_tail = 0;
}

void SQueue_enqueue(SQueue *Q, Event *E)
{
  E->ev_base.eb_next = 0;

  if (Q->sq_tail) {
    Q->sq_tail->ev_base.eb_next = E;
    Q->sq_tail = E;
  } else
    Q->sq_tail = Q->sq_head = E;
}

void SQueue_remove(SQueue *Q, Event *E)
{
  if (Q->sq_head == E) {
    Q->sq_head = EventList_remove(Q->sq_head,E);
    if (!Q->sq_head)
      Q->sq_tail = 0;
    else if (Q->sq_tail == E) {
      E = Q->sq_head;
      while (E->ev_base.eb_next)
	E = E->ev_base.eb_next;;
      Q->sq_tail = E;
    }
  }
}


Event *SQueue_dequeue(SQueue *Q)
{
  Event *E = Q->sq_head;
  Q->sq_head = E->ev_base.eb_next;
  if (!Q->sq_head) Q->sq_tail = 0;
  return E;
}

/*****************************************************************************
 *
 * Create a thread activication event.
 *
 * Parameters:
 *     thread		Thread to be activated.
 *
 *****************************************************************************/
Event *new_EvThread(VGThread *thread)
{
  EvThread *tev = (EvThread *) new_Event();

  tev->et_base.eb_vtable = &evthread_vtable;
  tev->et_thread = thread;

  return (Event*) tev;
}

/*****************************************************************************
 *
 * Process a thread activation event
 *
 * Parameters:
 *     tev		Thread event to execute
 *     q		Event queue
 *
 *****************************************************************************/
void EvThread_process(EvThread *tev,EvQueue *q)
{
  VGThread *thread = tev->et_thread;

  thread->t_pending = 0;
  if (thread->t_isLive)
    VGThread_exec(thread);
  if (!thread->t_isLive)
    delete_VGThread(thread);
}

/*****************************************************************************
 *
 * Create a reg net assignment event.
 *
 * Parameters:
 *     n		Net to assign value to
 *     nlsb		LSB in n to begin assignment
 *     s		Value to assign
 *     smsb,slsb	Range in s from which to copy
 *     
 *
 *****************************************************************************/
Event *new_EvNet(Net *n,int nlsb,Value *s,int smsb,int slsb)
{
  EvNet *en = (EvNet *) new_Event();
  int nbits = smsb-slsb+1;

  en->en_base.eb_vtable = &evnet_vtable;
  en->en_net = n;
  en->en_lsb = nlsb;
  en->en_state = new_Value(nbits);

  if (Value_nbits(s) == nbits)
    Value_copy(en->en_state,s);
  else
    Value_copyRange(en->en_state,0,s,smsb,slsb);

  return (Event*) en;
}

/*****************************************************************************
 *
 * Process a net assignment event
 *
 * Parameters:
 *     en		Net assignment event to execute
 *     q		Event queue
 *
 *****************************************************************************/
void EvNet_process(EvNet *en,EvQueue *q)
{
  int s_nbits = Value_nbits(en->en_state);

  if (Net_nbits(en->en_net) == s_nbits)
    Net_set(en->en_net, en->en_state);
  else
    Net_setRange(en->en_net, en->en_lsb, en->en_state, s_nbits-1, 0);
}

/*****************************************************************************
 *
 * Uninitialize a net assignment event
 *
 * Parameters:
 *     en		Net assignment event to uninitialize
 *
 *****************************************************************************/
void EvNet_uninit(EvNet *en)
{
  delete_Value(en->en_state);
}

/*****************************************************************************
 *
 * Create a strobe event.
 *
 * Parameters:
 *     thread		Thread to use as task context
 *     task		System task to execute
 *     argc		LSB in n to begin assignment
 *     argv		Value to assign
 *     
 *
 *****************************************************************************/
Event *new_EvStrobe(systask_f *task,TaskContext *tContext)
{
  EvStrobe *e = (EvStrobe *) new_Event();

  e->es_base.eb_vtable = &evstrobe_vtable;
  e->es_task = task;
  e->es_taskContext = tContext;

  return (Event*)e;
}

/*****************************************************************************
 *
 * Process a strobed system task call
 *
 * Parameters:
 *     en		Net assignment event to execute
 *     q		Event queue
 *
 *****************************************************************************/
void EvStrobe_process(EvStrobe *e,EvQueue *q)
{
  TaskContext *tc = e->es_taskContext;

  if (tc) {
    tc->tc_action = TA_STROBE;
    VGThread_goto(tc->tc_thread,tc->tc_codeBlock,0);
    VGThread_exec(tc->tc_thread);
  }
}

/*****************************************************************************
 *
 * Uninitialize a strobe event
 *
 * Parameters:
 *     e		Strobe event to uninitialize
 *
 *****************************************************************************/
void EvStrobe_uninit(EvStrobe *e)
{
  TaskContext *tc = e->es_taskContext;

  tc->tc_action = TA_REMOVE;
  (*e->es_task)(0,0,0,0,tc);
}


/*****************************************************************************
 *
 * Create a probe event.
 *
 * Parameters:
 *     net		Net to probe
 *     who		Who to report to
 *     name		Name of net
 *     
 *
 *****************************************************************************/
Event *new_EvProbe(Net *n,const char *who,const char *name)
{
  EvProbe *e = (EvProbe *) new_Event();

  e->ep_base.eb_vtable = &evprobe_vtable;
  e->ep_who = who ? strdup(who) : 0;
  e->ep_net = n;
  e->ep_name = name ? strdup(name) : 0;
  e->ep_lastValue = new_Value(Net_nbits(n));
  Value_copy(e->ep_lastValue, Net_getValue(n));

  return (Event*) e;
}

/*****************************************************************************
 *
 * Process a probe event
 *
 * Parameters:
 *     en		Net assignment event to execute
 *     q		Event queue
 *
 *****************************************************************************/
void EvProbe_process(EvProbe *e,EvQueue *q)
{
  Net *n = e->ep_net;

  if (!Value_isEqual(e->ep_lastValue, Net_getValue(n))) {
    Value_copy(e->ep_lastValue, Net_getValue(n));
    Net_reportValue(n, e->ep_who, e->ep_name, q->eq_circuit);
  }
}

/*****************************************************************************
 *
 * Uninitialize a probe event
 *
 * Parameters:
 *     e		Strobe event to uninitialize
 *
 *****************************************************************************/
void EvProbe_uninit(EvProbe *e)
{
  if (e->ep_name) {
    free(e->ep_name);
    e->ep_name = 0;
  }
}

/*****************************************************************************
 *
 * Create a memory assignment event.
 *
 * Parameters:
 *     n		Memory to assign value to
 *     addr		Address for assignment
 *     nlsb		LSB in n to begin assignment
 *     s		Value to assign
 *     smsb,slsb	Range in s from which to copy
 *     
 *
 *****************************************************************************/
Event *new_EvMem(Net *n,Value *addr,int nlsb,Value *s,int smsb,int slsb)
{
  Memory *m  = &n->n_data.memory;
  EvMem *em = (EvMem *) new_Event();
  int nbits = smsb-slsb+1;

  em->em_base.eb_vtable = &evmem_vtable;
  em->em_mem = n;
  em->em_lsb = nlsb;
  em->em_state = new_Value(nbits);

  if (Value_toInt(addr,&em->em_addr) < 0) {
    if ((Memory_getFlags(m) & MF_INITIALIZED))
      errorRun(ERR_MEMADDR,Net_getName(n));
  }

  if (Value_nbits(s) == nbits)
    Value_copy(em->em_state,s);
  else
    Value_copyRange(em->em_state,0,s,smsb,slsb);

  return (Event*) em;
}

/*****************************************************************************
 *
 * Process a net assignment event
 *
 * Parameters:
 *     en		Net assignment event to execute
 *     q		Event queue
 *
 *****************************************************************************/
void EvMem_process(EvMem *em,EvQueue *q)
{
  Memory *m  = &em->em_mem->n_data.memory;
  int s_nbits = Value_nbits(em->em_state);

#if 0
  printf("EvMem_process: addr=%x data=",em->em_addr);
  Value_print(em->em_state,stdout);
  printf(" lsb=%d",em->em_lsb);
  printf("\n");
#endif

  Memory_setFlags(m, MF_INITIALIZED);
  if (Net_nbits(em->em_mem) == s_nbits) {
    Net_memSet(em->em_mem, em->em_addr, em->em_state);
  } else
    Net_memSetRange(em->em_mem, em->em_addr, em->em_lsb, em->em_state, s_nbits-1, 0);
}


/*****************************************************************************
 *
 * Uninitialize a net assignment event
 *
 * Parameters:
 *     en		Net assignment event to uninitialize
 *
 *****************************************************************************/
void EvMem_uninit(EvMem *em)
{
  delete_Value(em->em_state);
}


/*****************************************************************************
 *
 * Create a wire net assignment event.
 *
 * Parameters:
 *     n		Net to assign value to
 *     id		Driver id of n to assign
 *     nlsb		LSB in n to begin assignment
 *     s		Value to assign
 *     smsb,slsb	Range in s from which to copy
 *     
 *
 *****************************************************************************/
Event *new_EvDriver(Net *n,int id,int nlsb,Value *s,int smsb,int slsb)
{
  EvDriver *ed = (EvDriver *) new_Event();
  int nbits = Net_nbits(n);

  ed->ed_base.eb_vtable = &evdriver_vtable;
  ed->ed_net = n;
  ed->ed_id = id;
  ed->ed_state = new_Value(nbits);

  if (Value_nbits(s) == nbits)
    Value_copy(ed->ed_state, s);
  else {
    Value_float(ed->ed_state);
    Value_copyRange(ed->ed_state,nlsb,s,smsb,slsb);
  }

  return (Event*) ed;
}

/*****************************************************************************
 *
 * Process a wire net driver assignment event
 *
 * Parameters:
 *     en		Net assignment event to execute
 *     q		Event queue
 *
 *****************************************************************************/
void EvDriver_process(EvDriver *ed,EvQueue *q)
{
  Value *driver = Net_getDriver(ed->ed_net, ed->ed_id);
  Value_copy(driver, ed->ed_state);

  Net_driverChangeNotify(ed->ed_net, ed->ed_id);
}

/*****************************************************************************
 *
 * Uninitialize a wire net assignment event
 *
 * Parameters:
 *     en		Wire net assignment event to uninitialize
 *
 *****************************************************************************/
void EvDriver_uninit(EvDriver *ed)
{
  delete_Value(ed->ed_state);
}


/*****************************************************************************
 *
 * Allocate a new Event using the free list if not empty
 *
 * Returns:		Uninitialized Event object.
 *
 *****************************************************************************/
Event *new_Event()
{
  Event *E;

  if (event_freelist) {
    E = event_freelist;
    event_freelist = E->ev_base.eb_next;
    E->ev_base.eb_status = ES_MACTIVE;
  } else {
    E = (Event*) malloc(sizeof(Event));
    E->ev_base.eb_status = ES_FACTIVE;
  }
  E->ev_base.eb_vtable = 0;
  E->ev_base.eb_time = 0;
  E->ev_base.eb_next = 0;

  return E;
}

/*****************************************************************************
 *
 * Return an Event to the freelist
 *
 * Parameters:
 *     E		Event to be "deleted".
 *
 *****************************************************************************/
void delete_Event(Event *e)
{
  if (e->ev_base.eb_vtable->evv_uninit)
    (*e->ev_base.eb_vtable->evv_uninit)(e);

  e->ev_base.eb_vtable = 0;
  e->ev_base.eb_status = ES_FREELIST;

  e->ev_base.eb_next = event_freelist;
  event_freelist = e;
}

/*****************************************************************************
 *
 * Insert an event (E) into a list of events (PQ) in priority order.
 *
 *****************************************************************************/
Event *Event_priorityInsert(Event *PQ,Event *E)
{
  if (!PQ) {
    E->ev_base.eb_next = 0;
    return E;
  }

  if (PQ->ev_base.eb_time > E->ev_base.eb_time) {
    E->ev_base.eb_next = PQ;
    return E;
  }

  PQ->ev_base.eb_next = Event_priorityInsert(PQ->ev_base.eb_next,E);
  return PQ;
}

/*****************************************************************************
 *
 * Create an event queue.
 *
 * Parameters:
 *     C		Circuit controlled by event queue
 *
 *****************************************************************************/
EvQueue *new_EvQueue(Circuit *C)
{
  EvQueue *Q = (EvQueue*) malloc(sizeof(EvQueue));
  EvQueue_init(Q,C);
  return Q;
}

/*****************************************************************************
 *
 * Event queue initialization.
 *
 * Parameters:
 *     Q		Queue to be initialized
 *     C		Circuit controlled by event queue
 *
 *****************************************************************************/
void EvQueue_init(EvQueue *Q,Circuit *C)
{
  int i;

  Q->eq_circuit = C;
  Q->eq_finalTime = ~0;
  Q->eq_curTime = 0;
  Q->eq_numPending = 0;
  Q->eq_flags = 0;
  SQueue_init(&Q->eq_assignQ);
  SQueue_init(&Q->eq_strobeQ);
  SQueue_init(&Q->eq_inactiveQ);
  SHash_init(&Q->eq_finalQ);
  SHash_resize(&Q->eq_finalQ,4); /* Make initial hash table small for run-time efficiency */
  Q->eq_overflowQ = 0;
  Q->eq_realQ = 0;
  Q->eq_monitor = 0;
  Q->eq_monitorOn = 1;
  Q->eq_monitoredChange = 0;

  for (i = 0;i < THYMEWHEEL_SIZE;i++)
    Q->eq_wheelHead[i] = Q->eq_wheelTail[i] = 0;
}

void Event_print(Event *e)
{
  switch (Event_getType(e)) {
  case EV_UNKNOWN : printf("[UNKNOWN]");break;
  case EV_THREAD  : printf("[THREAD]");break;
  case EV_NET     : printf("[NET]");break;
  case EV_CONTROL : printf("[CONTROL]");break;
  case EV_DRIVER  : printf("[DRIVER]");break;
  case EV_STROBE  : printf("[STROBE]");break;
  case EV_PROBE   : printf("[PROBE]");break;
  }
}

void EvQueue_print(EvQueue *Q)
{
  unsigned s = Q->eq_curTime & THYMEWHEEL_MASK;
  unsigned i;
  Event *e;

  printf("Pending: %d S:%d\n",Q->eq_numPending,s);
  for (i = 0;i < THYMEWHEEL_SIZE;i++) {
    if (i != s && !Q->eq_wheelHead[i]) continue;
    if (i == s)
      printf(">>%3d:",i);
    else
      printf("  %3d:",i);
    for (e = Q->eq_wheelHead[i];e;e = e->ev_base.eb_next) {
      printf(" ");
      Event_print(e);
    }
    printf("\n");
  }

  if (Q->eq_assignQ.sq_head) {
    printf("  A:");
    for (e = Q->eq_assignQ.sq_head;e;e = e->ev_base.eb_next) {
      printf(" ");
      Event_print(e);
    }
  }

  if (Q->eq_strobeQ.sq_head) {
    printf("  S:");
    for (e = Q->eq_strobeQ.sq_head;e;e = e->ev_base.eb_next) {
      printf(" ");
      Event_print(e);
    }
    printf("\n");
  }

  if (Q->eq_inactiveQ.sq_head) {
    printf("  I:");
    for (e = Q->eq_inactiveQ.sq_head;e;e = e->ev_base.eb_next) {
      printf(" ");
      Event_print(e);
    }
    printf("\n");
  }

  if (Q->eq_overflowQ) {
    printf("  OV:");
    for (e = Q->eq_overflowQ;e;e = e->ev_base.eb_next) {
      printf(" ");
      Event_print(e);
    }
    printf("\n");
  }

  if (Q->eq_monitor) {
    printf("  OV:");
    for (e = Q->eq_monitor;e;e = e->ev_base.eb_next) {
      printf(" ");
      Event_print(e);
    }
    printf("\n");
  }

  if (Q->eq_realQ) {
    printf("  R:");
    for (e = Q->eq_realQ;e;e = e->ev_base.eb_next) {
      printf(" ");
      Event_print(e);
    }
    printf("\n");
  }
}

void EvQueue_go(EvQueue *Q)
{
  (Q)->eq_flags |= EVF_RUN;

  if (vgsim.vg_interactive)
    vgio_printf("go @ %llu\n",Q->eq_curTime);
}


/*****************************************************************************
 *
 * Schedule an event in the final event queue.
 *
 * Parameter:
 *     Q		Queue in which to insert event
 *     key		Name of event
 *     e		Event to be placed in queue
 *
 * The final event queue is used for persistent events that are issued at the
 * end of an epoch and are not automatically removed from the queue.  It is
 * similar to the monitor queue except there can be multiple events in the
 * queue.
 * 
 *****************************************************************************/
void EvQueue_enqueueFinal(EvQueue *Q, const char *key, Event *e)
{
  SHash_insert(&Q->eq_finalQ,key,e);
}

/*****************************************************************************
 *
 * Remove a named event from the final event queue.
 *
 * Parameter:
 *     Q		Queue in which to insert event
 *     key		Name of the event to be remove
 * 
 *****************************************************************************/
void EvQueue_removeFinal(EvQueue *Q, const char *key)
{
  Event *e = EvQueue_findFinal(Q,key);

  if (e) {
    SHash_remove(&Q->eq_finalQ,key);
    delete_Event(e);
  }
}


/*****************************************************************************
 *
 * Schedule an event in the inactive event queue.
 *
 * Parameter:
 *     Q		Queue in which to insert event
 *     E		Event to be placed in queue
 *
 * This queue is used for zero delay '#0' statements.  These events are issued
 * after all events in the active event queue, but before assignments and
 * strobe events.
 * 
 *****************************************************************************/
void EvQueue_enqueueInactive(EvQueue *Q, Event *e)
{
  Q->eq_numPending++;
  SQueue_enqueue(&Q->eq_inactiveQ,e);
}

/*****************************************************************************
 *
 * Schedule an event in the strobe event queue.
 *
 * Parameter:
 *     Q		Queue in which to insert event
 *     e		Event to be placed in queue
 *
 * The strobe queue is primarily used to implement the $strobe task.  Events
 * in this queue are issued last in an epoch.
 * 
 *****************************************************************************/
void EvQueue_enqueueStrobe(EvQueue *Q, Event *e)
{
  Q->eq_numPending++;
  SQueue_enqueue(&Q->eq_strobeQ,e);
}

/*****************************************************************************
 *
 * Schedule an event in the monitor event queue.
 *
 * Parameter:
 *     Q		Queue in which to insert event
 *     E		Event to be placed in queue
 *
 * Only a single event can be in the monitor event queue and it is issued
 * at the end of every epoch.  Queueing an new event when there is alreay
 * an evenit in the queue causes the old event to be deleted.  This "queue"
 * is primarily used to implement the $montor() task.
 * 
 *****************************************************************************/
void EvQueue_enqueueMonitor(EvQueue *Q, Event *e)
{
  if (Q->eq_monitor) {
    delete_Event(Q->eq_monitor);
  }
  Q->eq_monitor = e;
}

/*****************************************************************************
 *
 * Schedule an event in the active event queue.
 *
 * Parameter:
 *     Q		Queue in which to insert event
 *     E		Event to be placed in queue
 * 
 *****************************************************************************/
void EvQueue_enqueue(EvQueue *Q,Event *E)
{
  unsigned s = E->ev_base.eb_time & THYMEWHEEL_MASK;

  Q->eq_numPending++;
  E->ev_base.eb_next = 0;

  if (E->ev_base.eb_time - Q->eq_curTime >= THYMEWHEEL_SIZE) {
    /*
     * Insert event into overflow queue
     */
    Q->eq_overflowQ = EventList_insert(Q->eq_overflowQ,E);
    return;
  } else if (Q->eq_wheelTail[s]) {
    /*
     * Insert event on tail of slot queue
     */
    Q->eq_wheelTail[s]->ev_base.eb_next = E;
    Q->eq_wheelTail[s] = E;
  } else {
    /*
     * Insert event into empty slot queue 
     */
    Q->eq_wheelTail[s] = Q->eq_wheelHead[s] = E;
  }

#if 0
  printf("%5llu: enqueue T=%x  QL=%d ",Q->eq_curTime,Event_getType(E),Q->eq_numPending);
  EvQueue_print(Q);printf("\n");
#endif
}

void EvQueue_enqueueAtHead(EvQueue *Q, Event *E)
{
  unsigned s = Q->eq_curTime & THYMEWHEEL_MASK;

  E->ev_base.eb_time = Q->eq_curTime;
  E->ev_base.eb_next = 0;

  if (!Q->eq_wheelHead[s])
    Q->eq_wheelTail[s] = E;

  E->ev_base.eb_next = Q->eq_wheelHead[s]; 
  Q->eq_wheelHead[s] = E;
  Q->eq_numPending++;
}


void EvQueue_enqueueAt(EvQueue *Q,Event *E,simtime_t t)
{
  E->ev_base.eb_time = t;
  EvQueue_enqueue(Q,E);
}

void EvQueue_enqueueAfter(EvQueue *Q,Event *E,deltatime_t t)
{
  E->ev_base.eb_time = Q->eq_curTime + t;
  EvQueue_enqueue(Q,E);
}


/*****************************************************************************
 *
 * Advance simulation time and do any end of epoch processing
 *
 *****************************************************************************/
void EvQueue_doFinal(EvQueue *Q)
{
  Event *e;
  HashElem *he;

  if (Q->eq_finalTime == Q->eq_curTime) return;
  Q->eq_finalTime = Q->eq_curTime;

  if (Q->eq_monitor && Q->eq_monitorOn) {
    e = Q->eq_monitor;
    Event_process(e, Q);
  }

  for (he = Hash_first(&Q->eq_finalQ);he;he = Hash_next(&Q->eq_finalQ,he)) {
    e = (Event*) HashElem_obj(he);
    Event_process(e, Q);
  }
}

static int EvQueue_primaryPending(EvQueue *Q)
{
  unsigned s = Q->eq_curTime & THYMEWHEEL_MASK;

  if (Q->eq_wheelHead[s]) return 1;
  if (Q->eq_inactiveQ.sq_head) return 1;

  return 0;
}


/*****************************************************************************
 *
 * Dequeue the next event from the current time step in the event queue.
 *
 * Parameters:
 *     Q		Queue from which to dequeue event
 *
 * Returns:		The soonest event from the queue.
 *
 *****************************************************************************/
Event *EvQueue_dequeue(EvQueue *Q)
{
  unsigned s = Q->eq_curTime & THYMEWHEEL_MASK;
  Event *E;

  for (;;) {

    /*
     * If there are no events in the primary queue for the current time, look
     * in the assign queue and the strobe queue.  Only if both of these are
     * empty to we advance simulation time.
     */
    if (!Q->eq_wheelHead[s]) {
      if (Q->eq_inactiveQ.sq_head) {
	E = SQueue_dequeue(&Q->eq_inactiveQ);
	break;
      }
      if (Q->eq_assignQ.sq_head) {
	E = SQueue_dequeue(&Q->eq_assignQ);
	break;
      }
      if (Q->eq_strobeQ.sq_head) {
	E = SQueue_dequeue(&Q->eq_strobeQ);
	break;
      }

      return 0; 		/* No more events in this time step */
    } else {
      evtype_t etype;

      E = Q->eq_wheelHead[s];
      Q->eq_wheelHead[s] = E->ev_base.eb_next;
      if (!Q->eq_wheelHead[s]) Q->eq_wheelTail[s] = 0;

      /*
       * If the event is a non-blocking assignment, move it to the assignQ and
       * skip to the next event.
       */
      etype = Event_getType(E);
      if (etype == EV_NET) {
	SQueue_enqueue(&Q->eq_assignQ,E);
      } else
	break;
    }
  }

  E->ev_base.eb_next = 0;
  Q->eq_numPending--;

#if 0
  printf("%5llu: dequeue T=%x  QL=%d ",Q->eq_curTime,Event_getType(E),Q->eq_numPending);
  EvQueue_print(Q);printf("\n");
#endif

  return E;
}

/*****************************************************************************
 *
 * Remove a specific event from the queue.
 *
 * Parameters
 *     Q		Queue from which to remove event
 *     E		Event to remove
 *
 *****************************************************************************/
void EvQueue_remove(EvQueue *Q,Event *E)
{
  if (E->ev_base.eb_time - Q->eq_curTime < THYMEWHEEL_SIZE) {
    /*
     * Event is in main timewheel
     */
    unsigned s = E->ev_base.eb_time % THYMEWHEEL_SIZE;
    Event *P,*D;

    P = 0;
    for (D = Q->eq_wheelHead[s];D;D = D->ev_base.eb_next) {
      if (D == E) break;
      P = D;
    }

    if (P) {
      P->ev_base.eb_next = E->ev_base.eb_next;
      if (!P->ev_base.eb_next)
	Q->eq_wheelTail[s] = P;
    } else {
      Q->eq_wheelHead[s] = E->ev_base.eb_next;
      if (!Q->eq_wheelHead[s])
	Q->eq_wheelTail[s] = 0;
    }
  } else {
    /*
     * Event is in overflow list
     */
    
    Q->eq_overflowQ = EventList_remove(Q->eq_overflowQ, E);
  }
}

/*****************************************************************************
 *
 * Check the time wheel for inconsistencies.
 *
 * Parameters:
 *     Q		Queue to be checked.
 *
 *****************************************************************************/
int EvQueue_check(EvQueue *Q)
{
  unsigned s;
  int count = 0;
  Event *e;

  for (s = 0;s < THYMEWHEEL_SIZE;s++) {
    for (e = Q->eq_wheelHead[s];e;e = e->ev_base.eb_next) {
      if (!(e->ev_base.eb_status == ES_MACTIVE || e->ev_base.eb_status == ES_FACTIVE))
	return -1;
      count++;
    }
  }

  for (e = Q->eq_inactiveQ.sq_head;e;e = e->ev_base.eb_next) count++;
  for (e = Q->eq_strobeQ.sq_head;e;e = e->ev_base.eb_next) count++;


  if (count != Q->eq_numPending) {
    printf("count=%d != Q->eq_numPending=%d\n",count,Q->eq_numPending);
    return -1;
  }

  return 0;
}


/*****************************************************************************
 *
 * Promote events in the overflow queue to the main active queue.
 *
 * Parameters:
 *     Q		Queue to be checked.
 *
 * This function is called after advancing time to check for events in the
 * overflow queue that may now possibly be moved to the main active queue.
 * The overflow queue is used for events that are scheduled in the distant
 * future beyond THYMEWHEEL_SIZE epochs.  Normally we expect this queue
 * to be mostly unusued except for a something like a '#100000 $finish' statement.
 *
 *****************************************************************************/
void EvQueue_promoteOverflow(EvQueue *Q)
{
  while (Q->eq_overflowQ && (Q->eq_overflowQ->ev_base.eb_time - Q->eq_curTime < THYMEWHEEL_SIZE)) {
    Event *overflow_event = Q->eq_overflowQ;

    Q->eq_numPending--;
    Q->eq_overflowQ = Q->eq_overflowQ->ev_base.eb_next;
    EvQueue_enqueue(Q,overflow_event);
  }
}

/*****************************************************************************
 *
 * Do all active and inactive events in the current step
 *
 * Parameters:
 *     Q		Queue to be processed.
 *
 *****************************************************************************/
void EvQueue_update(EvQueue *Q)
{
  Event *e;

  while (EvQueue_primaryPending(Q)) {
    e = EvQueue_dequeue(Q);
    if (e) {
      Event_process(e,Q);
      delete_Event(e);
    }
  }
}


/*****************************************************************************
 *
 * Main event loop in batch mode
 *
 * Parameters:
 *     Q		Queue to be processed.
 *
 * This function does the primary event processing for the simulator.  Once all
 * of the initialization is complete, this function is called and does not return
 * until the end of the simulation.  This function simply tests for pending events
 * and advances the simulator to the next epoch until the event queue is empty.
 * An empty event queue signifies the end of the simulation.
 *
 *****************************************************************************/
void EvQueue_mainEventLoop(EvQueue *Q)
{
  EvQueue_go(Q);


  while (EvQueue_isRunning(Q)) {
    Event *e = EvQueue_dequeue(Q);
    if (((Q->eq_flags & EVF_LIMIT) && Q->eq_curTime >= Q->eq_limitTime)) break;
    if (e) {
      Event_process(e,Q);
      delete_Event(e);
    } else {
      if (Q->eq_monitoredChange)
	EvQueue_doFinal(Q);

      Q->eq_monitoredChange = 0;
      if (EvQueue_pending(Q) > 0) {
	Q->eq_curTime++;
      } else {
	if (Q->eq_realQ) {
	  /* NOTE: There is a possible race condition if the timer event occurs after */
	  /* the test of do_input_check.  The minor consequenses will be that a time check */
	  /* could be delayed by one polling period. */
	  if (!do_input_check)
	    pause();
	}else
	  break;
      }
    }

    if (do_input_check) {
      if (Q->eq_realQ) {
	struct timeval tv;
	simtime_t now; 

	gettimeofday(&tv,0);
	now = tv_to_ms(&tv);

	while (Q->eq_realQ && now > Q->eq_realQ->ev_base.eb_time) {
	  Event *e = Q->eq_realQ;
	  Q->eq_realQ = e->ev_base.eb_next;
	  Event_process(e,Q);
	  delete_Event(e);
	}
      }
      do_input_check = 0;
    }

  }
}


/*****************************************************************************
 *
 * Main event loop in interactive mode
 *
 * Parameters:
 *     Q		Queue to be processed.
 *
 * This function does the primary event processing for the simulator.  Once all
 * of the initialization is complete, this function is called and does not return
 * until the end of the simulation.  This function simply tests for pending events
 * and advances the simulator to the next epoch until the event queue is empty.
 * An empty event queue signifies the end of the simulation.
 *
 *****************************************************************************/
void EvQueue_interactiveMainEventLoop(EvQueue *Q)
{
  char buf[STRMAX];
  Event *e;
  int simulator_running;

  /*
   * Enable asynchronous input.
   */
  input_setup();


  /*
   * Set flag to see if the simulator is executing.   Note the EvQueue_isRunning just
   * checks the status of the EV_RUN flag, but the simulator_running flag can be set
   * to zero due to a lack of events in the event queue.
   */
  simulator_running = EvQueue_isRunning(Q);

  /*
   * Main event loop.  This loop will only terminate when the standard input is
   * closed.
   */
  for (;;) {
    if (!(Q->eq_flags & EVF_RUN))
      simulator_running = 0;
    if (simulator_running) {
      /*****************************************************************************
       *Queue is running
       *****************************************************************************/

      /*
       * Asynchronously check for and process input.  We also process any real-time
       * events here.
       */
      if (do_input_check && !(Q->eq_flags & EVF_NOCMD)) {
	if (input_ready(0)) {
	  if (!get_line(buf,STRMAX)) return;
	  Circuit_exec(Q->eq_circuit,buf);
	} else {
	  if (Q->eq_realQ) {
	    struct timeval tv;
	    simtime_t now; 

	    gettimeofday(&tv,0);
	    now = tv_to_ms(&tv);

	    while (Q->eq_realQ && now > Q->eq_realQ->ev_base.eb_time) {
	      Event *e = Q->eq_realQ;
	      Q->eq_realQ = e->ev_base.eb_next;
	      Event_process(e,Q);
	      delete_Event(e);
	    }
	  }

	  do_input_check = 0;
	}
	continue;
      }

      /*
       * Get the next event in this epoch.  Process it if we found one otherwise
       * advance time to the next epoch. 
       */
      e = EvQueue_dequeue(Q);
      if (e) {
	Event_process(e,Q);
	delete_Event(e);
      } else {
	/*
	 * Advance simulator time and check overflow queue to see if we can move
	 * overflow events to the main queue. 
	 */
	if (Q->eq_monitoredChange)
	  EvQueue_doFinal(Q);
	Q->eq_curTime++;
	Q->eq_monitoredChange = 0;
	EvQueue_promoteOverflow(Q);

	/*
	 * If the event queue is empty or we have requested a stop at this time,
	 * stop execution of the queue and go into input mode.
	 */
	if (((Q->eq_flags & EVF_LIMIT) && Q->eq_curTime >= Q->eq_limitTime)) {
	  EvQueue_stop(Q);
	  simulator_running = 0;
	} else if (EvQueue_pending(Q) == 0) {
	  /* NOTE: There is a possible race condition if the timer event occurs after */
	  /* the test of do_input_check.  The minor consequenses will be that a time check */
	  /* could be delayed by one polling period. */
	  if (Q->eq_realQ) {
	    if (!do_input_check)
	      pause();
	  } else
	    simulator_running = 0;
	}

      }
    } else {
      /*****************************************************************************
       *Queue is halted
       *****************************************************************************/
      Q->eq_flags &= ~EVF_LIMIT;

      if (EvQueue_isRunning(Q) && EvQueue_pending(Q) == 0)
	vgio_printf("readystop @ %llu (qempty - %d - %d)\n",Q->eq_curTime,EvQueue_isRunning(Q), EvQueue_pending(Q));
      else
	vgio_printf("stop @ %llu (stoped - %d - %d)\n",Q->eq_curTime,EvQueue_isRunning(Q), EvQueue_pending(Q));


      input_ready(1);
      if (!get_line(buf,STRMAX)) return;
      Circuit_exec(Q->eq_circuit,buf);
      simulator_running = EvQueue_isRunning(Q);
    }
  }
}

/*****************************************************************************
 *
 * Schedule a simulator stop at some time in the future.
 *
 * Parameters:
 *     Q		Queue to be stopped 
 *     dt		Relative time to stop execution.
 *
 * This function will schedule a $stop of the simulation 'dt' epochs in the
 * future (dt can be 0).  The simulation will run until all events in the
 * scheduled epoch have been processed, the simulator will advance one time
 * unit to the beginning of the next epoch and the simulator will stop before
 * any events in the new epoch are processed.  If there is already a stop
 * scheduled, the sooner of the existing and requested stop time will be used.
 *
 *****************************************************************************/
void EvQueue_stopAfter(EvQueue *Q,deltatime_t dt)
{
  simtime_t stopTime = Q->eq_curTime + dt;

  if (!(Q->eq_flags & EVF_LIMIT) || stopTime < Q->eq_limitTime) {
    Q->eq_flags |= EVF_LIMIT;
    Q->eq_limitTime = stopTime;
  }
}

/*****************************************************************************
 *
 * Schedule a clock triggered stop at some time in the future.
 *
 * Parameters:
 *     Q		Queue to be stopped 
 *     n		Net to watch for clock transition
 *     tt		Type of transition to trigger on.
 *     dt		Time after clock event to schedule $stop
 *
 *****************************************************************************/
void EvQueue_clockStop(EvQueue *Q,Net *n,int numEdge,transtype_t tt,deltatime_t dt)
{
  if (tt == TT_NEGEDGE && n)
    Q->eq_watchClock.wc_cmd = WC_ACTIVENEG;
  else if (tt == TT_POSEDGE && n)
    Q->eq_watchClock.wc_cmd = WC_ACTIVEPOS;
  else if (tt == TT_NEGEDGE && !n)
    Q->eq_watchClock.wc_cmd = WC_ANYNEG;
  else if (tt == TT_POSEDGE && !n)
    Q->eq_watchClock.wc_cmd = WC_ANYPOS;
  else
    Q->eq_watchClock.wc_cmd = WC_NONE;

  Q->eq_watchClock.wc_net = n;
  Q->eq_watchClock.wc_numEdge = numEdge;
  Q->eq_watchClock.wc_overstep = dt;

  Q->eq_flags |= EVF_CLKWATCH;
}

/*****************************************************************************
 *
 * Receive notification that a clock signal has changed value.
 *
 * Parameters:
 *     Q		Event queue 
 *     n		Net that has changed
 *     tt		Type of transition
 *
 *****************************************************************************/
void EvQueue_clockNotify(EvQueue *Q,Net *n, transtype_t tt)
{
  int sawTrigger = 0;

  if (!(Q->eq_flags & EVF_CLKWATCH)) return;

  switch (Q->eq_watchClock.wc_cmd) {
  case WC_ACTIVENEG :
    if (Q->eq_watchClock.wc_net == n && tt == TT_NEGEDGE)
      sawTrigger = 1;
    break;
  case WC_ACTIVEPOS :
    if (Q->eq_watchClock.wc_net == n && tt == TT_POSEDGE)
      sawTrigger = 1;
    break;
  case WC_ANYNEG :
    if (tt == TT_NEGEDGE)
      sawTrigger = 1;
    break;
  case WC_ANYPOS :
    if (tt == TT_POSEDGE)
      sawTrigger = 1;
    break;
  case WC_NONE :
  default :
    break;
  }

  /*
   * We saw the triggering event.  If we have seen the required number of edge events,
   * convert this watch to a step stop.
   */
  if (sawTrigger) {
    if (--Q->eq_watchClock.wc_numEdge <= 0) {
      Q->eq_flags &= ~EVF_CLKWATCH;
      EvQueue_stopAfter(Q, Q->eq_watchClock.wc_overstep);
    }
  }
}

/*****************************************************************************
 *
 * Enqueue an event after a specific amount of real elapsed time in milliseconds.
 *
 * Parameters:
 *     Q		Event queue 
 *     E		Event to execute at timer expiration
 *     t		Absolute time of event.
 *
 *****************************************************************************/
void EvQueue_timedEnqueue(EvQueue *Q, Event *E, simtime_t t)
{
  /*
   * Enable the timer event if not already enabled
   */
  input_setup();

  E->ev_base.eb_time = t;
  Q->eq_realQ = Event_priorityInsert(Q->eq_realQ, E);
}
