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

    Last edit by hansen on Mon Feb  2 17:03:26 2009
****************************************************************************/
/*
    Postscript generator for tkgate.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/time.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "tkgate.h"

/***********************************************************************/


static void choose_trace_interval(simtime_t *e_per_l,simtime_t *e_inter)
{
  double n,m;
  int l,h;

  /*
   * Power of ten >= to e_per_l
   */
  n = pow(10.0,ceil(log((double)*e_per_l)/log(10.0)));

  m = (n-*e_per_l)/n;

  if (m >= 0.8)
    m = 0.02;
  else if (m >= 0.6)
    m = 0.05;
  else
    m = 0.1;

  n = m*n;

  *e_inter = (int)n;

  l = (*e_per_l / *e_inter)* *e_inter;
  h = l + *e_inter;

  if (*e_per_l-l < h-*e_per_l)
    *e_per_l = l;
  else
    *e_per_l = h;
}


void GPrint_setupTracePages(GPrint *P)
{
  extern GScope *Scope;		/* The scope trace */
  int hspace;			/* Horizontal space for traces on page */
  int vspace;			/* Vertical space for traces on page */
  int tsheight;			/* Height of a full trace set and scale */
  int pg;			/* Page # */
  int e_start;			/* Starting epoch # on a page */

  pg = 0;

  /*
   * If no traces, output an empty page.
   */
  if (!Scope || Scope->NumTraces <= 0 || P->p_trace.ti_start >= P->p_trace.ti_end) {
    P->p_numPages = 1;
    P->p_pages = (GPage**) ob_malloc(sizeof(GPage*),"GPage*");
    assert(P->p_pages);
    P->p_pages[0] = new_GPage(PT_TRACE,1);
    return;
  }

  vspace = P->p_uHeight-PTRC_TMARGIN-PTRC_BMARGIN;		/* Max usable vert. space*/
  hspace = P->p_uWidth-PTRC_LMARGIN-PTRC_RMARGIN;		/* Max usable horz. space*/
  tsheight = Scope->NumTraces*PTRC_TRHEIGHT+PTRC_SCHEIGHT;	/* Ver. space per trace set */

  P->p_trace.ti_simEnd = Scope->s_time;

  /*
   * Choose an interval and adjust epochs per line to be a multiple of the interval
   */
  choose_trace_interval(&P->p_trace.ti_scaleLength,&P->p_trace.ti_interval);

  /*
   * Compute translation from epochs to points.
   */
  P->p_trace.ti_pointsPerEpoch = (double)hspace/(double)P->p_trace.ti_scaleLength;

  /*
   * Round start down and end up to a multiple of the interval
   */
  P->p_trace.ti_istart = (P->p_trace.ti_start/P->p_trace.ti_interval)*P->p_trace.ti_interval;
  P->p_trace.ti_iend = ((P->p_trace.ti_end+P->p_trace.ti_interval-1)/P->p_trace.ti_interval)*P->p_trace.ti_interval;

  if (tsheight <= vspace) {					/* 1 or more trace sets on a page */

    /*
     * Compute the number of trace sets on a page, the number of epochs on a page and the
     * number of pages in the document.  The number of pages per trace set is one.
     */
    P->p_trace.ti_tracesPerPage = (vspace/tsheight);
    P->p_trace.ti_epochsPerPage = P->p_trace.ti_scaleLength*P->p_trace.ti_tracesPerPage;
    P->p_numPages = (P->p_trace.ti_iend-P->p_trace.ti_istart+P->p_trace.ti_epochsPerPage-1)/P->p_trace.ti_epochsPerPage;
    P->p_trace.ti_pagesPerTrace = 1;

    P->p_pages = (GPage**) ob_malloc(P->p_numPages*sizeof(GPage*),"GPage*[]");
    assert(P->p_pages);

    for (e_start = P->p_trace.ti_istart;e_start < P->p_trace.ti_iend;e_start += P->p_trace.ti_epochsPerPage) {
      GPage *G;

      assert(pg < P->p_numPages);

      G = new_GPage(PT_TRACE,pg+1);
      P->p_pages[pg++] = G;

      G->pg_traceNum = 0;
      G->pg_traceStart = e_start;
      G->pg_traceEnd = e_start + P->p_trace.ti_epochsPerPage - 1;
      if (G->pg_traceEnd > P->p_trace.ti_iend) 
	G->pg_traceEnd = P->p_trace.ti_iend;
    }
  } else {						/* multiple pages per trace set */
    int maxtr = (vspace-PTRC_SCHEIGHT)/PTRC_TRHEIGHT;	/*   Maximum number of traces per page */
    int j;

    P->p_trace.ti_pagesPerTrace = (Scope->NumTraces+maxtr-1)/maxtr;
    P->p_trace.ti_epochsPerPage = P->p_trace.ti_scaleLength;
    P->p_numPages = ((P->p_trace.ti_iend-P->p_trace.ti_istart+P->p_trace.ti_epochsPerPage-1)/P->p_trace.ti_epochsPerPage);
    P->p_numPages *= P->p_trace.ti_pagesPerTrace;
    P->p_trace.ti_tracesPerPage = 1;

    P->p_pages = (GPage**) ob_malloc(P->p_numPages*sizeof(GPage*),"GPage*[]");
    assert(P->p_pages);

    pg = 0;
    for (e_start = P->p_trace.ti_istart;e_start < P->p_trace.ti_iend;e_start += P->p_trace.ti_epochsPerPage) {
      for (j = 0;j < P->p_trace.ti_pagesPerTrace;j++) {
	GPage *G;

	assert(pg < P->p_numPages);

	G = new_GPage(PT_TRACE,pg);
	P->p_pages[pg++] = G;

	G->pg_traceNum = j*maxtr;
	G->pg_traceStart = e_start;
	G->pg_traceEnd = e_start + P->p_trace.ti_epochsPerPage - 1;
	if (G->pg_traceEnd > P->p_trace.ti_iend) 
	  G->pg_traceEnd = P->p_trace.ti_iend;

      }
    }
  }

#if 0 
  printf("actual pages (p=%d (%gx%g) s=%d r=[%d..%d] ---> %d\n",(P->p_size-paperSizes),P->p_uWidth,P->p_uHeight,P->p_trace.scale,P->p_trace.ti_start,P->p_trace.ti_end,P->p_numPages);
#endif
}

/*****************************************************************************
 *
 * Format a time value for printing
 *
 *****************************************************************************/
char *GPrint_formatTime(GPrint *P,char *buf,simtime_t t)
{
  SimInterface *si = &TkGate.circuit->simulator;
  double n = t*si->si_tsmult/(double)si->si_precision;
  double interval = P->p_trace.ti_interval*si->si_tsmult/(double)si->si_precision;
  int units = si->si_units;


  while (interval > 1000) {
    interval /= 1000;
    n /= 1000;
    units--;
  }

  switch (si->si_precision) {
  case 1 :
    sprintf(buf,"%0.0f%s",n,SimInterface_unitsToStr(units));
    break;
  case 10 :
    sprintf(buf,"%0.1f%s",n,SimInterface_unitsToStr(units));
    break;
  case 100 :
    sprintf(buf,"%0.2f%s",n,SimInterface_unitsToStr(units));
    break;
  default :
    sprintf(buf,"%0.3f%s",n,SimInterface_unitsToStr(units));
    break;
  }


  return buf;
}

/*
 * Print a scope trace document
 */
void GPrintOpt_tracePrint(GPrintOpt *PO)
{
  GPrint *P = new_GPrint(PO);

  if (!P) return;

  GPrint_setupTracePages(P);
  GPrint_outputPreamble(P,0);
  GPrint_outputPages(P);
  GPrint_outputTrailer(P);

  delete_GPrint(P);
}


/*****************************************************************************
 *
 * Returns an estimate of the number of lines of trace groups that can
 * fit on a page.  If more than one page is needed, a value less than one
 * will be returned.
 *
 * Parameters
 *   orient			Page orientation
 *   paper			Paper type
 *
 *****************************************************************************/
double traceLinesPerPage(const char *orient,const char *paper)
{
  extern GScope *Scope;		/* The scope trace */
  PaperSize *size = 0;
  double uHeight, uWidth;
  double vspace,tsheight;
  int i;

  /*
   * If no active scope, return 1 page.  
   */
  if (!Scope || Scope->NumTraces <= 0)
    return 1.0;

  /*
   * Search for the paper size
   */
  for (i = 0;paperSizes[i].ps_size;i++) {
    if (strcmp(paperSizes[i].ps_size,paper) == 0) {
      size = &paperSizes[i];
      break;
    }
  }
  if (!size) size = &paperSizes[0];

  /*
   * Figure out height and width from paper size and orientation.
   */
  if (strcmp(orient,"landscape") == 0) {
    uWidth = size->ps_height - PAGE_LMARGIN - PAGE_RMARGIN;
    uHeight = size->ps_width - PAGE_TMARGIN - PAGE_BMARGIN - PAGE_LBLOCK;;
  } else {
    uWidth = size->ps_width - PAGE_LMARGIN - PAGE_RMARGIN;;
    uHeight = size->ps_height- PAGE_TMARGIN - PAGE_BMARGIN - PAGE_LBLOCK;
  }

  vspace = uHeight-PTRC_TMARGIN-PTRC_BMARGIN;			/* Max usable vert. space*/
  tsheight = Scope->NumTraces*PTRC_TRHEIGHT+PTRC_SCHEIGHT;	/* Ver. space per trace set */

  if (tsheight <= vspace) {
    return floor(vspace/tsheight);		/* One or more trace groups fit on a page */
  } else {
    return 1.0/ceil(tsheight/vspace);		/* More than one page needed for a trace group */
  }
}

/*
 * Output a single line of trace data for a single signal.
 *
 * Parameters:
 *   P		Page on which to print
 *   T		Trace data to print
 *   y		y position of page of trace
 *   pg_tstart	time value at left edge of page.
 */
static void GPrint_outputTrace(GPrint *P,GTrace *T,int y,int pg_tstart)
{
  simtime_t tend = imin(pg_tstart + P->p_trace.ti_scaleLength,P->p_trace.ti_end);
  simtime_t tstart = imax(pg_tstart,P->p_trace.ti_start);
  char buf[STRMAX];
  GValue *pV,*V;
  const char *name;

  name = strchr(T->t_name,'.');
  if (name)
    name++;
  else
    name = T->t_name;

  fprintf(P->p_f,"%d %d %f %d gline\n",PTRC_TXTMARGIN,y,P->p_uWidth-PTRC_RMARGIN,y);
  fprintf(P->p_f,"%d bfont\n",PTRC_FONTSIZE);
  fprintf(P->p_f,"(%s) %d %d prshow\n",filterParen(buf,name),
	  PTRC_TXTMARGIN,y+PTRC_TRHEIGHT/2-PTRC_FONTSIZE/2);
  fprintf(P->p_f,"%d rfont\n",PTRC_FONTSIZE);

  /*
   * Scan for first value
   */
  pV = 0;
  for (V = T->t_first;V->v_next && V->v_next->v_time < tstart;pV = V, V = V->v_next);

  fprintf(P->p_f,"%d %d %d %d BT\n",
	  PTRC_LMARGIN-1,y,(int)P->p_uWidth-PTRC_LMARGIN-PTRC_RMARGIN+2,PTRC_TRHEIGHT);

  for (;V && V->v_time <= tend;pV = V, V = V->v_next) {
    simtime_t seg_start,seg_end;
    simtime_t subseg_start, subseg_end;
    double x1,x2;

    seg_start = V->v_time;
    seg_end = V->v_next ? V->v_next->v_time : imin(tend,P->p_trace.ti_simEnd);
    subseg_start = imax(imax(seg_start,pg_tstart),tstart);
    subseg_end = imin(seg_end,tend);

    x1 = PTRC_LMARGIN+(subseg_start-pg_tstart)*P->p_trace.ti_pointsPerEpoch;
    x2 = PTRC_LMARGIN+(subseg_end-pg_tstart)*P->p_trace.ti_pointsPerEpoch;

    if (V->v_hexValue) {
      if (pV && (seg_start == subseg_start || pV->v_time > pg_tstart))
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRHIGH,x1,y+PTRC_TRLOW);
      fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRLOW,x2,y+PTRC_TRLOW);
      fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRHIGH,x2,y+PTRC_TRHIGH);

      fprintf(P->p_f,"(%s) %g %d %g trshow\n",V->v_hexValue,x1+PTRC_HEXPOS,y+PTRC_TRLOW+3,x2-x1-PTRC_HEXPOS);
    } else {
      if (pV && (seg_start == subseg_start || pV->v_time > pg_tstart)) {
	unsigned tt = (T->t_nBits == 1) ? transition_type(pV->v_code,V->v_code) : 0x3;
	switch (tt) {
	case 0x1 :
	  fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRLOW,x1,y+PTRC_TRMID);
	  break;
	case 0x2 :
	  fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRHIGH,x1,y+PTRC_TRMID);
	  break;
	case 0x3 :
	  fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRHIGH,x1,y+PTRC_TRLOW);
	  break;
	}
      }

      switch (V->v_code) {
      case VC_ZERO :
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRLOW,x2,y+PTRC_TRLOW);
	break;
      case VC_ONE :
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRHIGH,x2,y+PTRC_TRHIGH);
	break;
      case VC_UNKNOWN :
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRLOW,x2,y+PTRC_TRLOW);
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRHIGH,x2,y+PTRC_TRHIGH);
	break;
      case VC_FLOAT :
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRMID,x2,y+PTRC_TRMID);
	break;
      case VC_HIGH :
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRHIGH,x2,y+PTRC_TRHIGH);
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRMID,x2,y+PTRC_TRMID);
	break;
      case VC_LOW :
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRLOW,x2,y+PTRC_TRLOW);
	fprintf(P->p_f,"%g %d %g %d line\n",x1,y+PTRC_TRMID,x2,y+PTRC_TRMID);
	break;
      }
    }
  }

  fprintf(P->p_f,"ET\n");
}

static void GPrint_outputTraceScale(GPrint *P,int y,simtime_t tstart)
{
  simtime_t tend = tstart + P->p_trace.ti_scaleLength;
  simtime_t t;
  char buf[STRMAX];

  fprintf(P->p_f,"%d bfont\n",PTRC_FONTSIZE);
  for (t = tstart;t <= tend;t += P->p_trace.ti_interval) {
    double x =  PTRC_LMARGIN + (t-tstart)*P->p_trace.ti_pointsPerEpoch;
    fprintf(P->p_f,"%g %d %g %d line\n",x,y,x,y-PTRC_SCTICK);

    if (t + P->p_trace.ti_interval <= tend)
      fprintf(P->p_f,"(%s) %g %d rCT\n",GPrint_formatTime(P,buf,t),x,y-PTRC_SCLABPOS);
  }
}


void GPrint_printTracePage(GPrint *P,GPage *PG)
{
  extern GScope *Scope;
  int t,ts,top_y,y;
  char buf1[STRMAX],buf2[STRMAX];

  fprintf(P->p_f,"(%d of %d) (%s-%s) BP_T\n",
	  PG->pg_num,P->p_numPages,
	  GPrint_formatTime(P,buf1,imax(PG->pg_traceStart,P->p_trace.ti_start)),
	  GPrint_formatTime(P,buf2,imin(PG->pg_traceEnd,P->p_trace.ti_end)));

  y = P->p_uHeight - PTRC_TMARGIN;

  for (ts = 0;ts < P->p_trace.ti_tracesPerPage;ts++) {
    simtime_t l_start = PG->pg_traceStart+ts*P->p_trace.ti_scaleLength;

    if (l_start >= PG->pg_traceEnd) break;

    top_y = y;
    for (t = PG->pg_traceNum;t < Scope->NumTraces;t++) {
      GTrace *T = Scope->Traces[t];				/* Trace to display */
      y -= PTRC_TRHEIGHT;					/* Position of trace baseline */
      if (y - PTRC_SCHEIGHT < 0) break;				/* Page is full */

      GPrint_outputTrace(P,T,y,l_start);
    }

    fprintf(P->p_f,"%d %d %d %d bold_box\n",0,y-PTRC_SCBXHEIGHT,(int)P->p_uWidth,top_y-y+PTRC_SCBXHEIGHT);

    GPrint_outputTraceScale(P,y,l_start);

    y -= PTRC_SCHEIGHT;
  }

  fprintf(P->p_f,"EP\n");
}
