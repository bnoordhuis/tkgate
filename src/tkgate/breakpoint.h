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
#ifndef __breakpoint_h
#define __breakpoint_h

/* Breakpoint states */
#define BPS_INVALID	-1	/* The breakpoint expression was invalid */
#define BPS_GO	 	0	/* Breakpoint has not been activated */
#define BPS_STOP	1	/* Simulation has stopped at this breakpoint */
#define BPS_IGNORE	2	/* This breakpoint is being ignored */
#define BPS_STANDBY	3	/* Breakpoint is in the standby state */

/*****************************************************************************
 *
 * Simulator breakpoint 
 *
 *****************************************************************************/
struct SBreakPoint_str {
  char		*bp_condition;	/* Condition of the breakpoint */
  int		bp_id;		/* Identifier of the breakpoint */
  int		bp_state;	/* State of the breakpoint */
};

void BrkPtTable_delete(NHash *bpm,int idx);
void BrkPtTable_enable(NHash *bpm,int idx);
void BrkPtTable_disable(NHash *bpm,int idx);
int BrkPtTable_insert(NHash *bpm,int idx,const char *condition);
void BrkPtTable_sendAll(NHash *bpm);
void BrkPtTable_activate(NHash *bpm,int id,const char *value);
void BrkPtTable_clearStop(NHash *bpm);
void BrkPtTable_error(NHash *bpm,int id);
void BrkPtTable_flush(NHash *bpm);
void BrkPtTable_loadInterface(NHash *bpm);
void delete_SBreakPoint(SBreakPoint *bp);
SBreakPoint *new_SBreakPoint(int idx,int state, const char *condition);

#endif
