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
#include "tkgate.h"


/*****************************************************************************
 *
 * Helping function to clear all breakpoints and reclaim allocated memory.
 * Used in conjunction with Hash_flush().
 *
 *****************************************************************************/
static void breakpoint_flush(HashElem *e,Hash *h)
{
  SBreakPoint *b = (SBreakPoint*) HashElem_obj(e);
  delete_SBreakPoint(b);
}

/*****************************************************************************
 *
 * Helping function used with qsort() to sort breakpoints by their ID value.
 *
 *****************************************************************************/
static int breakpoint_compare(const void *vA,const void *vB)
{
  const SBreakPoint *A = *(SBreakPoint**) vA;
  const SBreakPoint *B = *(SBreakPoint**) vB;

  return A->bp_id - B->bp_id;
}

/*****************************************************************************
 *
 * Generates a breakpoint file and loads it into the simulator.
 *
 * Parameters:
 *     bp			Breakpoint to be set.
 *
 *
 * Creates a script file to implement the specified breakpoint and loads the
 * breakpoint as if it were a script file.  A breakpoint can be any expression
 * in terms of nets in the user circuit and can optionally be prepended by
 * the 'negedge' or 'posedge' keyword.  It can also optionally have a '#' followed
 * by a number at the end.  The [*] in the net declaration/assignment means that
 * the bit width of the declared net will match the bitwidth of the right-hand
 * side expression.  The is a tkgate extension to verilog intended only for use
 * with tkgate breakpoints.
 *
 * Some examples of the scripts generated from various breakpoint commands.
 *
 * Command: $break 1 (W + E)
 *
 *    wire [*] BP$1 = (W + E);
 *    always @(BP$1) $tkg$break(1,BP$1);
 *
 * Command: $break 2 posedge (W > E)
 *
 *    wire [*] BP$2 = W > E;
 *    always @(posedge BP$2) #10 $tkg$break(2,BP$2);
 *
 * Command: $break 3 (W > E) #10
 *
 *    wire [*] BP$3 = W > E;
 *    always @(BP$3) #10 $tkg$break(3,BP$3);
 *
 * Command: $break 4 #10
 *
 *    always #10 $tkg$break(4,1'b1);
 *
 *****************************************************************************/
void SBreakPoint_enable(SBreakPoint *bp)
{
  char fileName[STRMAX];
  char expression[STRMAX];
  const char *p;
  const char *edge;
  char *q;
  int delay = -1; 
  int isDelayOnly;
  FILE *f;
  char x;


  p = bp->bp_condition;

  while (*p && isspace(*p)) p++;

  if (strncmp(p,"posedge",7) == 0 && !isalnum(p[7])) {
    edge = "posedge";
    p += 7;
  } else if (strncmp(p,"negedge",7) == 0 && !isalnum(p[7])) {
    edge = "negedge";
    p += 7;
  } else
    edge = "";

  strcpy(expression,p);
  q = strchr(expression,'#');
  if (q) {
    /* Make sure there is no tailing junk by trying to scan an additional character */
    if (sscanf(q,"#%d %c",&delay,&x) == 1) {
      *q = 0;
    } else if (sscanf(q,"#(%d) %c",&delay,&x) == 1) {
      *q = 0;
    } else
      delay = -1;
  }

  isDelayOnly = (sscanf(expression,"%c",&x) == 0);

  /*
   * Empty breakpoint
   */
  if (isDelayOnly && delay < 0.0) {
    BrkPtTable_error(TkGate.circuit->c_breakpoints,bp->bp_id);
    return;
  }


  getSimTempFile(fileName);
  f = fopen(fileName,"w");
  if (!f) {
    message(MC_MSGLOG|MC_URGENT,"Failed to create temorary file '%s' for breakpoint.",fileName);
    BrkPtTable_error(TkGate.circuit->c_breakpoints,bp->bp_id);
    return;
  }

  if (isDelayOnly) {
    fprintf(f,"always #%d $tkg$reportbreak(%d, 1'b1);\n",delay,bp->bp_id);
  } else {
    fprintf(f,"wire [*] \\BP$%d = %s;\n", bp->bp_id, expression);
    if (delay > 0)
      fprintf(f,"always #0 @(%s \\BP$%d ) #%d $tkg$reportbreak(%d, \\BP$%d );\n",
	      edge, bp->bp_id, delay, bp->bp_id, bp->bp_id);
    else
      fprintf(f,"always #0 @(%s \\BP$%d ) $tkg$reportbreak(%d, \\BP$%d );\n",
	      edge, bp->bp_id, bp->bp_id, bp->bp_id);
  }

  fclose(f);

  sendSimCmd("$script break:%d %s -delete -silent -breakpt", bp->bp_id, fileName);
}

/*****************************************************************************
 *
 * Create a new breapoint object
 *
 *****************************************************************************/
SBreakPoint *new_SBreakPoint(int idx,int state, const char *condition)
{
  SBreakPoint *bp = (SBreakPoint *) ob_malloc(sizeof(SBreakPoint*) , "SBreakPoint");

  bp->bp_id = idx;
  bp->bp_state = state;
  bp->bp_condition = ob_strdup(condition);

  return bp;
}

/*****************************************************************************
 *
 * Delete a breapoint object and reclaim its memory.
 *
 *****************************************************************************/
void delete_SBreakPoint(SBreakPoint *bp)
{
  ob_free(bp->bp_condition);
  ob_free(bp);
}


/*****************************************************************************
 *
 * Insert breakpoint with the specified id
 *
 *****************************************************************************/
int BrkPtTable_insert(NHash *bpm,int id,const char *condition)
{
  SBreakPoint *bp;

  while (NHash_find(bpm,id))
    id++;

  bp = new_SBreakPoint(id,BPS_GO,condition);
  NHash_insert(bpm,id,bp);

  if (TkGate.circuit->simulator.active)
    SBreakPoint_enable(bp);

  return id;
}

/*****************************************************************************
 *
 *  Delete breakpoint at an index (not id number)
 *
 *****************************************************************************/
void BrkPtTable_delete(NHash *bpm,int id)
{
  SBreakPoint *bp = NHash_find(bpm,id);
  if (!bp) return;

  NHash_remove(bpm,id);

  if (TkGate.circuit->simulator.active)
    sendSimCmd("$delscript break:%d",id);
}

void BrkPtTable_enable(NHash *bpm,int id)
{
  SBreakPoint *bp = NHash_find(bpm,id);
  if (!bp) return;

  if (TkGate.circuit->simulator.active) {
    if (bp->bp_state == BPS_INVALID || bp->bp_state == BPS_IGNORE) {
      ob_touch(bp);
      bp->bp_state = BPS_GO;
      SBreakPoint_enable(bp);
    }
    DoTcl("Breakpoint::setState %d %d",bp->bp_id,bp->bp_state);
  } else {
    ob_touch(bp);
    bp->bp_state = BPS_STANDBY;
    DoTcl("Breakpoint::setState %d %d",bp->bp_id,bp->bp_state);
  }
}

void BrkPtTable_disable(NHash *bpm,int id)
{
  SBreakPoint *bp = NHash_find(bpm,id);
  if (!bp) return;

  if (TkGate.circuit->simulator.active) {
    sendSimCmd("$delscript break:%d",bp->bp_id);
  }
   
  ob_touch(bp);
  bp->bp_state = BPS_IGNORE;
  DoTcl("Breakpoint::setState %d %d",bp->bp_id,bp->bp_state);
}


/*****************************************************************************
 *
 * Send all registered breakpoints to simulator.
 *
 *****************************************************************************/
void BrkPtTable_sendAll(NHash *bpm)
{
  if (TkGate.circuit->simulator.active) {
    HashElem *he;

    for (he = Hash_first(bpm);he;he = Hash_next(bpm,he)) {
      SBreakPoint *bp = (SBreakPoint*)HashElem_obj(he);

      if (bp->bp_state != BPS_IGNORE) {
	SBreakPoint_enable(bp);
	bp->bp_state = BPS_GO;
	DoTcl("Breakpoint::setState %d %d",bp->bp_id,bp->bp_state);
      }
    }
  }
}

/*****************************************************************************
 *
 *  Show a breakpoint as having been activated.
 *
 *****************************************************************************/
void BrkPtTable_activate(NHash *bpm,int id,const char *value)
{
  char s_id[STRMAX];
  SBreakPoint *bp = NHash_find(bpm,id);
  if (!bp) return;

  ob_touch(bp);
  bp->bp_state = BPS_STOP;
  sprintf(s_id,"%d",id);
  DoTclL("Breakpoint::seeBreak",s_id,value,NULL);
}

/*****************************************************************************
 *
 * Mark a breakpoint as having an error.
 *
 *****************************************************************************/
void BrkPtTable_error(NHash *bpm,int id)
{
  SBreakPoint *bp = NHash_find(bpm,id);
  if (!bp) return;

  ob_touch(bp);
  bp->bp_state = BPS_INVALID;
  DoTcl("Breakpoint::seeError %d",id); 
}

/*****************************************************************************
 *
 * Clear the 'active' mark on all breakpoints.
 *
 *****************************************************************************/
void BrkPtTable_clearStop(NHash *bpm)
{
  HashElem *he;

  for (he = Hash_first(bpm);he;he = Hash_next(bpm,he)) {
    SBreakPoint *bp = (SBreakPoint*)HashElem_obj(he);

    if (TkGate.circuit->simulator.active) {
      if (bp->bp_state == BPS_STOP) {
	ob_touch(bp);
	bp->bp_state = BPS_GO;
	DoTcl("Breakpoint::seeGo %d",bp->bp_id);
      }
    } else {
      if (bp->bp_state == BPS_STOP || bp->bp_state == BPS_GO) {
	ob_touch(bp);
	bp->bp_state = BPS_STANDBY;
	DoTcl("Breakpoint::seeStandby %d",bp->bp_id);
      }
    }
  }
}

/*****************************************************************************
 *
 * Flush all breakpoints from C registry and from tcl spreadsheet
 *
 *****************************************************************************/
void BrkPtTable_flush(NHash *bpm)
{
  Hash_flush(bpm, breakpoint_flush);
  DoTcl("Breakpoint::flush");
}

/*****************************************************************************
 *
 * Load all registered breakpoints into the tcl spreadsheet.
 *
 *****************************************************************************/
void BrkPtTable_loadInterface(NHash *bpm)
{
  HashElem *he;
  SBreakPoint **bplist;
  int n = Hash_numElems(bpm);
  int i;

  if (n == 0) return;
  bplist = (SBreakPoint**) malloc(sizeof(SBreakPoint*)*n);

  i = 0;
  for (he = Hash_first(bpm);he;he = Hash_next(bpm,he), i++) {
    SBreakPoint *bp = (SBreakPoint*)HashElem_obj(he);
    bplist[i] = bp;
  }

  qsort(bplist,n,sizeof(SBreakPoint*),breakpoint_compare);

  for (i = 0;i < n;i++) {
    char id_str[STRMAX], state_str[STRMAX];
    int state;

    sprintf(id_str,"%d",bplist[i]->bp_id);

    state = bplist[i]->bp_state;

    if (!TkGate.circuit->simulator.active) {
      if (state == BPS_GO || state == BPS_STOP)
	state = BPS_STANDBY;
    }

    sprintf(state_str,"%d",state);
    DoTclL("Breakpoint::addEntry",id_str,state_str,bplist[i]->bp_condition,NULL);
  }


  free(bplist);
}
