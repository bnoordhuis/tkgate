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
 * Create a new channel with the specified name
 *
 * Parameters:
 *     name		Name for new channel
 *
 * Returns:		Newly created channel
 *
 *****************************************************************************/
Channel *new_Channel(const char *name)
{
  Channel *c = (Channel*) malloc(sizeof(Channel));

  c->c_name = strdup(name);
  List_init(&c->c_queue);
  List_init(&c->c_wake);
  c->c_isWatched = 0; 
  c->c_format = 0; 

  return c;
}

/*****************************************************************************
 *
 * Report a value from a channel to the watcher
 *
 *****************************************************************************/
static void Channel_reportWatched(Channel *c,Value *v)
{
  EvQueue *Q = vgsim.vg_circuit.c_evQueue;
  char buf[STRMAX];

  if (!c->c_format)
    c->c_format = strdup("%h");
  Value_format(v,c->c_format,buf);
  vgio_printf("tell $queue %s %s @ %llu\n",c->c_name,buf,EvQueue_getCurTime(Q));
}

/*****************************************************************************
 *
 * Make the specified thread wait for data on a channel
 *
 * Parameters:
 *      c		Channel to wait for
 *      thread		Thread to suspend until data is ready.
 *
 *****************************************************************************/
void Channel_wait(Channel *c, VGThread *thread)
{
  List_addToTail(&c->c_wake,new_EvThread(thread));
  VGThread_suspend(thread);
}

/*****************************************************************************
 *
 * Return the length of the data queue for a channel.
 *
 * Parameters:
 *      c		Channel object
 *
 *****************************************************************************/
int Channel_queueLen(Channel *c)
{
  return List_numElems(&c->c_queue);
}

/*****************************************************************************
 *
 * Set a watch on a channel
 *
 * Parameters:
 *      c		Channel object
 *      isWatched	Flag to indicate if a channel should be watched.
 *      format		Format in which to report values 
 *
 *****************************************************************************/
int Channel_setWatch(Channel *c, int isWatched, const char *format)
{
  c->c_isWatched = isWatched;
  if (c->c_format) free(c->c_format);
  c->c_format = format ? strdup(format) : strdup("%h");
  if (isWatched) {
    while (List_numElems(&c->c_queue) > 0) {
      Value *v = (Value*) List_popHead(&c->c_queue);
      Channel_reportWatched(c,v);
    }
  }
  return 0;
}

int Channel_read(Channel *c, Value *data)
{
  Value *v;

  if (List_numElems(&c->c_queue) == 0)
    return -1;

  v = (Value*) List_popHead(&c->c_queue);

  Value_copy(data, v);
  delete_Value(v);

  return 0;
}

int Channel_write(Channel *c, Value *data)
{
  Value *v;

  if (c->c_isWatched) {
    Channel_reportWatched(c,data);
    return 0;
  }

  v = new_Value(Value_nbits(data));

  Value_copy(v, data);
  List_addToTail(&c->c_queue, v);

#if 0
  {
    char buf[STRMAX];

    Value_getstr(data,buf);
    vgio_echo("Channel_write(%s, %s)\n",c->c_name,buf);
  }
#endif

  while (List_numElems(&c->c_wake) > 0) {
    Event *e = (Event*) List_popHead(&c->c_wake);
    EvQueue_enqueueAfter(Circuit_getQueue(&vgsim.vg_circuit), e, 0);
  }

  return 0;
}

