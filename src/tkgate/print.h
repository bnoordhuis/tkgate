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
#ifndef __print_h
#define __print_h

#define MODMAX		2048		/* Maximum number of modules in circuit */

#define MODPGMAX	4		/* Maximum # of modules on a page */

#define INCH(s)		(72.0*(s))
#define MM(s)		(2.8346*(s))
#define CM(s)		(28.346*(s))

#define PT2INCH(s)	((s)*0.01389)
#define PT2MM(s)	((s)*0.35278)
#define PT2CM(s)	((s)*0.035278)

#define YCONV(L,y)	((L)->l_ymax-(y)+(L)->l_ymin)

#define EPSF_MINX	10
#define EPSF_MINY	10

/*
  Page metrics (in points unless otherwise state)
 */
#define PAGE_LMARGIN	25	/* Distance between edge of page and left edge of drawing */
#define PAGE_RMARGIN	25	/* Distance between edge of page and right edge of drawing */
#define PAGE_TMARGIN	25	/* Distance between edge of page and top edge of drawing */
#define PAGE_BMARGIN	25	/* Distance between edge of page and bottom edge of drawing */
#define PAGE_LBLOCK	60	/* Size of label block */
#define PAGE_MODMARGIN	10	/* Margin around module */
#define PAGE_OVERLAP	20	/* Portion of ciruit which overlaps on edges */

/*
  Font encoding vector ids
 */
#define PE_STANDARD	0	/* Standard encoding */
#define PE_LATIN1	1	/* Latin1 encoding */


/*
  Page types
 */
#define PT_MODULE	0	/* Page with a single (or portion of a) module */
#define PT_MOD4		1	/* Page with 4 modules */
#define PT_INDEX	2	/* Index page */
#define PT_GRAPH	3	/* Module hierarchy graph page */
#define PT_TRACE	4	/* Scope trace page */

/*
  Paging styles
 */
#define PS_FIT		0
#define PS_PARTITION	1

/*
  Layout for index pages
 */
#define PIDX_FONT	12		/* Font size for index */
#define PIDX_LMARGIN	20		/* Space from left edge to text */
#define PIDX_TMARGIN	20		/* Space from top edge to text baseline */
#define PIDX_HDRSEP	20		/* Space from header baseline to first entry */
#define PIDX_HDRLINEPOS	5		/* Position for header underline */
#define PIDX_LINELEN	140		/* Length of header underline */
#define PIDX_LINESEP	14		/* Space between lines */
#define PIDX_PGCOLSEP	100		/* Space from module name to page number column */
#define PIDX_COLSEP	150		/* Space to next column */

/*
  Trace metrics
 */

#define PTRC_TXTMARGIN	5		/* Distance from left edge to trace label */
#define PTRC_LMARGIN	100		/* Distance from left edge to trace */
#define PTRC_RMARGIN	10		/* Distance from right edge to trace */
#define PTRC_TRHEIGHT	22		/* Height of a trace */
#define PTRC_TRLOW	5		/* Position of low trace line */
#define PTRC_TRMID	11		/* Position of middle trace line */
#define PTRC_TRHIGH	17		/* Position of high trace line */
#define PTRC_TMARGIN	10		/* Margin on top of page */
#define PTRC_BMARGIN	10		/* Margin on bottom of page */
#define PTRC_FONTSIZE	10		/* Size of labels */
#define PTRC_SCBXHEIGHT	25		/* Height of the trace box */
#define PTRC_SCHEIGHT	30		/* Height of the trace scale */
#define PTRC_SCTICK	5		/* Length of scale tick */
#define PTRC_SCLABPOS	18		/* Position of scale numbers */
#define PTRC_HEXPOS	3		/* Offset for hex values in traces */

typedef struct {
  const char	*po_title;
  const char	*po_cmd;
  const char	*po_file;
  const char	*po_paper;
  const char	*po_orient;
  const char	*po_style;
  const char	*po_select;
  const char	*po_modlist;
  const char	*po_epsf;
  const char	*po_index;
  const char	*po_graph;
  const char	*po_4up;
  const char	*po_isDuplex;
  const char	*po_start;
  const char	*po_end;
  const char	*po_scaleLength;
  const char	*po_incLib;
} GPrintOpt; 

void GPrintOpt_init(GPrintOpt*);
void GPrintOpt_clDefault(GPrintOpt*);
void GPrintOpt_print(GPrintOpt*);
void GPrintOpt_tracePrint(GPrintOpt*);

typedef struct {
  const char	*ps_size;		/* Name of size */
  const char	*ps_tray;		/* Name of tray */
  double	ps_width;		/* Width in points */
  double	ps_height;		/* Height in points */
} PaperSize;

/*
  A GModLayout specifies layout information for a module.  Normally there is
  one GModLayout per page, but if the 4-per-page option is enabled, there may
  be up to 4 GModLayout per page.
 */
struct modlayout {
  /*
    Information about the module
   */
  GModuleDef	*l_mod;			/* The module in this layout */
  int		l_isSmall;		/* Is this module 4-up-able */
  int		l_xmin,	l_xmax;		/* The minimum and maximum X values (in pixels) */
  int		l_ymin,	l_ymax;		/* The minimum and maximum Y values (in pixels) */

  /*
    Where on the page is the circuit
   */
  int		l_xbase;		/* The base x coordinate of the layout (in points) */
  int		l_ybase;		/* The base y coordinate of the layout (in points)  */
  int		l_width;		/* The width of the layout (in points) */
  int		l_height;		/* The height of the layout (in points) */

  /*
    Information about the partition if partitioned.
   */
  int		l_numRows;		/* Total number of row partitions */
  int		l_numCols;		/* Total number of column partitions */
  int		l_r,l_c;		/* Position of this partition */
  int		l_x,l_y;		/* Base coordinates */
  int		l_pWidth,l_pHeight;	/* Width an height of this partition */

};

typedef struct {
  int		pg_num;			/* Page number of this page */
  int		pg_type;		/* Type of page */
  int		pg_nmod;		/* Number of modules on this page */

  simtime_t	pg_traceStart;		/* Start time for traces on this page */
  simtime_t	pg_traceEnd;		/* End time for traces on this page */
  int		pg_traceNum;		/* Index of first trace on this page */ 

  GModLayout	*pg_mods[MODPGMAX];	/* Module and layout data */
} GPage;

typedef struct trace_info {
  simtime_t	ti_start;		/* Starting epoch of trace */
  simtime_t	ti_end;			/* Ending epoch of trace */
  simtime_t	ti_istart;		/* Starting epoch of trace (rounded to interval) */
  simtime_t	ti_iend;		/* Ending epoch of trace (rounded to interval) */
  simtime_t	ti_simEnd;		/* Last simulated epoch */

  simtime_t	ti_scaleLength;		/* # of time units per line */

  simtime_t	ti_interval;		/* The interval for tick marks */
  simtime_t	ti_epochsPerPage;	/* Epochs per page */
  double	ti_pointsPerEpoch;	/* Points per epoch */

  int		ti_tracesPerPage;	/* Number of trace sets per page */
  int		ti_pagesPerTrace;	/* Number of pages per trace set */
} GTraceInfo;

struct GPrint_str {
  GPrintOpt	p_opts;			/* Raw printer options */
  int		p_isFile;		/* Output is to a file */
  FILE		*p_f;			/* File to print to */
  int		p_orient;		/* 0=portait, 1=landscape */
  int		p_style;		/* 0=scale, 1=partition */
  int		p_isDuplex;		/* Do double-sided printing */

  PaperSize	*p_size;		/* Page size */

  double	p_width;		/* Width of page */
  double	p_height;		/* Height of page */
  int		p_isepsf;		/* Output as EPSF document */

  double	p_uWidth;		/* Usable width */
  double	p_uHeight;		/* Usable height */

  GTraceInfo	p_trace;		/* Global data for traces */

  int		p_numMods;		/* Number of modules to print */
  GModLayout	**p_mods;		/* List of module layouts to print */

  int		p_numPages;		/* Number of pages */
  GPage		**p_pages;		/* List of pages to print */

  PHash		p_genSymbols;		/* Set of symbols that have been generated. */
};

GPage *new_GPage(int tp,int pn);
GPrint *new_GPrint(GPrintOpt *PO);
void GPrint_outputPreamble(GPrint *P,int do_gates);
void GPrint_outputPages(GPrint *P);
void GPrint_outputTrailer(GPrint *P);
void GPrint_printTracePage(GPrint *P,GPage *PG);
void delete_GPrint(GPrint *P);

extern PaperSize paperSizes[];

void PSSetFont(GPrint *P,HtmlFont *F);
void PSDrawText(GPrint *P,HtmlFont *F,int x,int y,const char *text,int just);
int PSStringWidth(HtmlFont *F,const char *s,int len);			/* Width of a postscript string in points. */




#endif
