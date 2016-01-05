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
#ifndef __evqueue_h
#define __evqueue_h
/*****************************************************************************
 *
 * Event Queue Data Structures
 *
 * The event queue is the center of the simulation.  Everything to be executed
 * by the simulator passes through the event queue.
 *
 *****************************************************************************/

/*****************************************************************************
 * evtype_t - Event type code
 *****************************************************************************/
typedef enum {
  EV_UNKNOWN = 0,	/* An unknown event type */
  EV_THREAD = 1,	/* A thread becomes ready for execution */
  EV_NET = 2,		/* A net value change */
  EV_CONTROL = 3,	/* A control event occured */
  EV_DRIVER = 4,	/* A wire driver change event occured */
  EV_STROBE = 5,	/* A strobe display */
  EV_PROBE = 6,		/* A probe display */
  EV_MEM = 7,		/* A memory assignment */
} evtype_t;

/*****************************************************************************
 * evstatus_t - Event status
 *****************************************************************************/
typedef enum {
  ES_MACTIVE = 1,		/* Active event allocated by malloc() */
  ES_FACTIVE = 2,		/* Active event allocated from freelist */
  ES_FREELIST = 3,		/* Event is on free list */
} evstatus_t;

/*****************************************************************************
 *   Event queue flags
 *****************************************************************************/
typedef enum {
  EVF_RUN =		0x1,	/* Event queue is free running */
  EVF_POSCLOCK =	0x2,	/* Wait for a clock posedge transition */
  EVF_NEGCLOCK =	0x4,	/* Wait for a clock negedge transition */
  EVF_HASCLOCK =	0x8,	/* The circuit contains at least one clock */
  EVF_LIMIT =		0x10,	/* Limit simulation time until pause */
  EVF_NOCMD =		0x20,	/* Do not process commands */
  EVF_CLKWATCH =	0x40,	/* Check for clock signals */
} eqflag_t;

typedef enum {
  WC_NONE =		0,	/* No clock checking */
  WC_ANYPOS = 		1,	/* Any clock posedge */
  WC_ANYNEG = 		2,	/* Any clock negedge */
  WC_ACTIVEPOS =	3,	/* Active clock posedge */
  WC_ACTIVENEG =	4,	/* Active clock negedge */
} ckwatchcmd_t;

/*****************************************************************************
 *
 * Function type for event processing functions.
 *
 *****************************************************************************/
typedef void EventProcess_f(Event*,EvQueue*);

/*****************************************************************************
 *
 * Function type for event uninitialization functions.
 *
 *****************************************************************************/
typedef void EventUninit_f(Event*);

/*****************************************************************************
 *
 * EventVTable - virtual function table for events
 *
 *****************************************************************************/
typedef struct {
  evtype_t		evv_class;	/* Type of event */
  EventProcess_f	*evv_process;	/* Process an event */ 
  EventUninit_f		*evv_uninit;	/* Uninitialization of an event */
} EventVTable;

/*****************************************************************************
 *
 * EvBase - Basic event members shared by all event types.
 *
 *****************************************************************************/
typedef struct {
  EventVTable	*eb_vtable;	/* Virtual table for event */
  simtime_t	eb_time;	/* Time of event */
  Event		*eb_next;	/* Next event in bucket */
  evstatus_t	eb_status;	/* Event status */
} EvBase;

/*****************************************************************************
 *
 * EvStrobe - System task call delayed until strobe phase.
 *
 *****************************************************************************/
typedef struct {
  EvBase	es_base;	/* Base event members */
  systask_f	*es_task;	/* Task to execute */
  TaskContext	*es_taskContext;/* Task context */
} EvStrobe;

/*****************************************************************************
 *
 * EvProbe - Probe a net for changes.  These events are normally "persistant"
 * in that they remain in the finalQ until explicitly removed
 *
 *****************************************************************************/
typedef struct {
  EvBase	ep_base;	/* Base event members */
  char		*ep_who;	/* Who do we report to */
  Net		*ep_net;	/* Net to test. */
  char		*ep_name;	/* Net name to report with probe (which alias of ep_net) */
  Value		*ep_lastValue;	/* Last value of net */
} EvProbe;

/*****************************************************************************
 *
 * EvNet - Event causing a net value to change
 *
 *****************************************************************************/
typedef struct {
  EvBase	en_base;	/* Base event members */
  Net		*en_net;	/* Net which changed */
  int		en_lsb;		/* LSB in net to write to (MSB implied from en_state width) */
  Value		*en_state;	/* New value for net */
} EvNet;

/*****************************************************************************
 *
 * EvDriver - Event causing a wire driver value to change
 *
 *****************************************************************************/
typedef struct {
  EvBase	ed_base;	/* Base event members */
  Net		*ed_net;	/* Net which changed */
  int		ed_id;		/* ID of driver that changed */
  Value		*ed_state;	/* New value for net */
} EvDriver;

/*****************************************************************************
 *
 * EvControl - A control event
 *
 *****************************************************************************/
typedef struct {
  EvBase	ec_base;	/* Base event members */
  unsigned	ec_type;	/* Control code */
  void		*ec_data;	/* Control data */
} EvControl;

/*****************************************************************************
 *
 * EvThread - A thread activation event
 *
 *****************************************************************************/
typedef struct {
  EvBase	et_base;	/* Base event members */
  VGThread	*et_thread;	/* Thread affected by event */
} EvThread;

/*****************************************************************************
 *
 * EvMem - Event causing a memory value to change
 *
 *****************************************************************************/
typedef struct {
  EvBase	em_base;	/* Base event members */
  Net		*em_mem;	/* Net for memory that is changing */
  unsigned	em_addr;	/* Address in memory to change */
  int		em_lsb;		/* LSB in net to write to (MSB implied from en_state width) */
  Value		*em_state;	/* New value for memory */
} EvMem;

/*****************************************************************************
 *
 * Event - Simulator event
 *
 *****************************************************************************/
union Event_union {
  EventVTable	*ev_vtable;
  EvBase	ev_base;
  EvNet		ev_net;
  EvStrobe	ev_strobe;
  EvDriver	ev_driver;
  EvThread	ev_thread;
  EvControl	ev_control;
  EvProbe	ev_probe;
  EvMem		ev_mem;
};

/*****************************************************************************
 *
 * SQueue - Simple event queue.
 *
 *****************************************************************************/
typedef struct {
  Event		*sq_head;
  Event		*sq_tail;
} SQueue;

/*****************************************************************************
 *
 * WClock - Clock watch description
 *
 *****************************************************************************/
typedef struct {
  ckwatchcmd_t	wc_cmd;				/* Clock watch command */
  Net		*wc_net;			/* Net of clock */
  int		wc_numEdge;			/* Number of edges to watch */
  deltatime_t	wc_overstep;			/* Overstep amount */
} WClock;

/*****************************************************************************
 *
 * EvQueue - The event queue.
 *
 *****************************************************************************/
struct EvQueue_str {
  Circuit	*eq_circuit;			/* Pointer to top level cicuit */
  eqflag_t	eq_flags;			/* Event queue flags */

  simtime_t	eq_finalTime;			/* Time of last final processing */
  simtime_t	eq_curTime;			/* Current time step */
  simtime_t	eq_limitTime;			/* Time to stop simulator if limit enabled */

  unsigned	eq_numPending;			/* Number of pending events */
  Event		*eq_wheelHead[THYMEWHEEL_SIZE];	/* Event queues for each step (for dequeue) */
  Event		*eq_wheelTail[THYMEWHEEL_SIZE];	/* Event queues for each step (for enqueue) */

  int		eq_monitorOn;			/* Non-zero if monitor is enabled */

  SQueue	eq_assignQ;			/* Queue for current time non-blocking assignment */
  SQueue	eq_strobeQ;			/* Queue for current strobe events */
  SQueue	eq_inactiveQ;			/* Inactive events queue (used for #0 statements) */
  SHash		eq_finalQ;			/* Persistent events occuring at end of each step */
  Event		*eq_monitor;			/* Event for monitor execution */
  Event		*eq_overflowQ;			/* Overflow events ouside regular time window */
  Event		*eq_realQ;			/* Events with real-time time stamps */

  WClock	eq_watchClock;			/* Clock watch description */

  int		eq_monitoredChange;		/* Monitored variables have changed in this epoch */
};

/*****************************************************************************
 * EvQueue member functions
 *****************************************************************************/
EvQueue *new_EvQueue(Circuit *C);
void EvQueue_init(EvQueue *Q,Circuit *C);

void EvQueue_print(EvQueue *Q);

void EvQueue_enqueue(EvQueue *Q,Event *E);
void EvQueue_enqueueAt(EvQueue *Q,Event *E,simtime_t t);
void EvQueue_enqueueAfter(EvQueue *Q,Event *E,deltatime_t t);
void EvQueue_enqueueInactive(EvQueue *Q, Event*);
void EvQueue_enqueueStrobe(EvQueue *Q, Event*);
void EvQueue_enqueueMonitor(EvQueue *Q, Event*);
void EvQueue_timedEnqueue(EvQueue *Q, Event *E, simtime_t t);
void EvQueue_enqueueAtHead(EvQueue *Q, Event *E);

void EvQueue_enqueueFinal(EvQueue *Q, const char *key, Event*);
#define EvQueue_findFinal(Q,key) ((Event*) SHash_find(&(Q)->eq_finalQ, key))
void EvQueue_removeFinal(EvQueue *Q, const char *key);

Event *EvQueue_dequeue(EvQueue*);
void EvQueue_remove(EvQueue *Q,Event *E);

void EvQueue_update(EvQueue *Q);
void EvQueue_mainEventLoop(EvQueue *Q);
void EvQueue_interactiveMainEventLoop(EvQueue *Q);
int EvQueue_check(EvQueue *Q);
void EvQueue_stopAfter(EvQueue*,deltatime_t dt);
void EvQueue_clockStop(EvQueue *Q,Net *n,int numEdge,transtype_t tt,deltatime_t dt);
void EvQueue_clockNotify(EvQueue *Q,Net *n, transtype_t tt);
#define EvQueue_pending(Q)	(Q)->eq_numPending
#define EvQueue_getCurTime(Q)	(Q)->eq_curTime
#define EvQueue_isRunning(Q)	((Q)->eq_flags & EVF_RUN)
#define EvQueue_stop(Q)		((Q)->eq_flags &= ~(EVF_RUN|EVF_LIMIT))
#define EvQueue_monitoredChangeNotify(Q) ((Q)->eq_monitoredChange = 1)
void EvQueue_go(EvQueue *Q);


/*****************************************************************************
 * Event member functions
 *****************************************************************************/
Event *new_Event();
void delete_Event(Event*);
Event *new_EvThread(VGThread *t);
Event *new_EvNet(Net*,int nlsb,Value*,int smsb,int slsb);
Event *new_EvStrobe(systask_f *task,TaskContext *tContext);
Event *new_EvProbe(Net *n,const char *who,const char *name);
Event *new_EvDriver(Net *n,int id,int nlsb,Value *s,int smsb,int slsb);
Event *new_EvMem(Net*,Value *addr,int nlsb,Value*,int smsb,int slsb);
Event *Event_priorityInsert(Event *PQ,Event *E);
#define Event_process(e,q) (*(e)->ev_vtable->evv_process)(e,q)
#define Event_getType(e) (e)->ev_vtable->evv_class
#define Event_isThreadUser(e,t) (Event_getType(e) == EV_THREAD && ((EvThread*)(e))->et_thread == (t))
void Event_print(Event*e);

Event *new_EvThread(VGThread *thread);

void SQueue_init(SQueue *Q);
void SQueue_enqueue(SQueue *Q, Event *E);
void SQueue_remove(SQueue *Q, Event *E);
Event *SQueue_dequeue(SQueue *Q);

#endif


