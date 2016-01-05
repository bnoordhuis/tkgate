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

    Last edit by hansen on Wed Dec 31 17:16:16 2008
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "tkgate.h"
#include <X11/Xutil.h>

/*
 *  Module Symbol Handling
 *
 *  The main class for describing a module symbol is a GModSymbol.  The GModSymbol
 *  object describes the locations of ports and contains the bitmap data for the
 *  normal and bold symbols.  However, it does not directly contain the X11 Pixmap
 *  or Icon objects.  The reason for this is that the standard undo/redo functions
 *  can not be applied to Pixmaps.  The Pixmap and Icon objects are instead managed
 *  by the MSS (Module Symbol Sever).  The MSS manages a set of MssSymbol objects,
 *  each of which contains the Pixmap and Icon objects corresponding to a single
 *  GModSymbol.  The MSS uses standard malloc/free memory management instead of the
 *  ob_malloc/ob_free management used by the rest of tkgate.  Each entry in the
 *  MSS is identified by an integer "gid" which is used by the GModSymbol object
 *  to look up its corresponding MssSymbol object.  The MSS is managed as a cache
 *  with only the last MSS_SIZE most recently used entries being kept.  If an antry
 *  is not used for a long time, it will be flushed and the associated Pixmap
 *  objects deleted.  This way, MssSymbol objects corresponding to GModSymbol
 *  objects which have been deleted will eventually be reclaimed.
 */

#define DEBUG_MOD_SYM 0


#define MSS_SIZE	128		/* Hash table size for MSS */
#define MSS_MAXENTRIES	128		/* Maximum number of entries in MSS table */

typedef struct mss_symbol MssSymbol;

struct mss_symbol {
  unsigned	me_gid;			/* Id of symbol data */
  Pixmap	me_pixmap;		/* Pixmap data for this entry */
  Icon		*me_normal[NUMORIENTS];	/* Normal icons for this entry */
  Icon		*me_select[NUMORIENTS];	/* Bold icons for this entry */
  MssSymbol	*me_next;		/* Next entry in hash chain */
  MssSymbol	*me_luNext;		/* Next entry in last use chain */
  MssSymbol	*me_luPrev;		/* Previous entry in last use chain */
};

typedef struct {
  unsigned	mss_idCount;		/* ID counter */
  unsigned	mss_numEntries;		/* Current number of entries */
  MssSymbol	*mss_ents[MSS_SIZE];	/* Table entries */
  MssSymbol	*mss_mru;		/* Most recently used */
  MssSymbol	*mss_lru;		/* Least recently used */
} MSS;

/*
 * Table of module sever symbols.
 */
static MSS mss;

/*
 * Table of symbols
 */
static PHash *symbolTable = 0;

static int defaultSymbol_x = 5;
static int defaultSymbol_y = 5;
static char *defaultSymbol_data =
"#define defaultSymbol_width 29\n"
"#define defaultSymbol_height 29\n"
"static unsigned char defaultSymbol_bits[] = {\n"
"   0xff, 0xff, 0x07, 0x00, 0x01, 0x00, 0x18, 0x00, 0x01, 0x00, 0x60, 0x00,\n"
"   0x01, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x02,\n"
"   0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x08,\n"
"   0x01, 0x00, 0x00, 0x08, 0x11, 0x39, 0x18, 0x10, 0x11, 0x49, 0x24, 0x10,\n"
"   0x11, 0x89, 0x04, 0x10, 0x11, 0x89, 0x18, 0x10, 0x11, 0x89, 0x20, 0x10,\n"
"   0x11, 0x49, 0x24, 0x10, 0xe1, 0x38, 0x18, 0x10, 0x01, 0x00, 0x00, 0x10,\n"
"   0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x08,\n"
"   0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x04, 0x01, 0x00, 0x00, 0x02,\n"
"   0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x80, 0x00, 0x01, 0x00, 0x60, 0x00,\n"
"   0x01, 0x00, 0x18, 0x00, 0xff, 0xff, 0x07, 0x00};\n";


static char *defaultBoldSymbol_data =
"#define y_width 29\n"
"#define y_height 29\n"
"static unsigned char y_bits[] = {\n"
"   0xff, 0xff, 0x07, 0x00, 0xff, 0xff, 0x1f, 0x00, 0x03, 0x00, 0x78, 0x00,\n"
"   0x03, 0x00, 0xe0, 0x00, 0x03, 0x00, 0x80, 0x01, 0x03, 0x00, 0x00, 0x03,\n"
"   0x03, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x0c,\n"
"   0x03, 0x00, 0x00, 0x0c, 0x33, 0x7b, 0x38, 0x18, 0x33, 0xdb, 0x6c, 0x18,\n"
"   0x33, 0x9b, 0x0d, 0x18, 0x33, 0x9b, 0x39, 0x18, 0x33, 0x9b, 0x61, 0x18,\n"
"   0x33, 0xdb, 0x6c, 0x18, 0xe3, 0x79, 0x38, 0x18, 0x03, 0x00, 0x00, 0x18,\n"
"   0x03, 0x00, 0x00, 0x18, 0x03, 0x00, 0x00, 0x0c, 0x03, 0x00, 0x00, 0x0c,\n"
"   0x03, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x06, 0x03, 0x00, 0x00, 0x03,\n"
"   0x03, 0x00, 0x80, 0x01, 0x03, 0x00, 0xe0, 0x00, 0x03, 0x00, 0x78, 0x00,\n"
"   0xff, 0xff, 0x1f, 0x00, 0xff, 0xff, 0x07, 0x00};\n";

/*
 * Rotate (x,y) around the point (cx, cy)
 */
static void rotatePoint(int *x,int *y,int cx,int cy,int r)
{
  static int isin[] = {0,1,0,-1};
  static int icos[] = {1,0,-1,0};
  int nx = *x - cx;
  int ny = -(*y - cy);

  *x = nx*icos[r] - ny*isin[r];
  *y = -(nx*isin[r] + ny*icos[r]);

  *x += cx;
  *y += cy;
}

/*****************************************************************************
 *
 * Reverse the bits in a byte.
 *
 * Parameters:
 *      x		Number in which to reverse bytes
 * 
 * Returns:		Byte reversed value.
 *
 *****************************************************************************/
static unsigned char reverseBits(unsigned char x)
{
  static unsigned char revTab[] = {
  //0  1  2   3  4  5   6   7  8  9  10  11  12  13  14  15
    0, 8, 4, 12, 2, 10, 6, 14, 1, 9,  5, 13,  3, 11,  7, 15};

  return (revTab[(x&0xf)]<<4) | revTab[((x>>4)&0xf)];
  
}

/*****************************************************************************
 *
 * Extract the image data in 'data' which is in standard bitmap format to bitmap
 * data in idata.  The image in data is WxH.
 *
 * Parametes:
 *     idata		Extracted data
 *     data             Data to be extracted in X11 bitmap format.
 *     W                Width of buffer
 *     H                Height of buffer
 *     bpl              Bytes per line.
 * 
 *****************************************************************************/
static void extractImageData(char *idata,const char *data,unsigned W,unsigned H,int bpl)
{
  const char *p;
  int x,y;
  unsigned byte;
  int bw = (W+7) >> 3;

  p = strchr(data,'{');
  if (!p) {
    return;
  }

  p++;
  x = y = 0;
  for (;;p++) {
    if (!*p) {
      return;
    }
    if (sscanf(p,"0x%x",&byte) == 1) {
      if (TkGate.bitorder == MSBFirst)
	byte = reverseBits(byte);
      idata[y*bpl+x] = byte;
      x++;
      if (x >= bw) {
	x = 0;
	y++;
      }
      if (y >= H) break;
    }
  }
}

/*
 * Create the rotations of the standard orientation image.
 */
static void makeRotations(XImage *image,unsigned yoffset,unsigned W,unsigned H)
{
  int x,y;

  for (x = 0;x < W;x++)
    for (y = 0;y < H;y++) {
      int p = XGetPixel(image,x,y+yoffset);
      if (p) {
	/*
	 * Assume we have already cleared the entire ximage and only need to set those
	 * pixels that are set.
	 */
	XPutPixel(image,W+y,	yoffset+W-x-1,	1);	/* orientation 1 */
	XPutPixel(image,W+H-x-1,yoffset+W+H-y-1,1);	/* orientation 2 */
	XPutPixel(image,H-y-1,	yoffset+H+x,	1);	/* orientation 3 */
      }
    }
}

/*
 * Create a new MssSymbol with the given identifier, normal icon data and select icon data.
 */
static MssSymbol *new_MssSymbol(unsigned gid,GMIconData *normal,GMIconData *select)
{
  int i;
  MssSymbol *me = (MssSymbol *) malloc(sizeof(MssSymbol));
  unsigned cwidth,cheight;		/* Cell width and height */
  unsigned width,height;		/* Total width and height */
  XImage *image;
  char *idata;
  int boldOffset;
  int bpl;
  int idata_len;
  int xoffset[4],yoffset[4];

  me->me_gid = gid;
  me->me_next = 0;
  me->me_luNext = 0;
  me->me_luPrev = 0;

  /*
   * Setup sizing information for icons
   */
  cwidth = imax(normal->w,select->w);
  cheight = imax(normal->h,select->h);

  width = cwidth + cheight + 1;
  width = (width+7) & ~0x7;				/* Round up to multiple of 8 */
  height = 2*(cwidth + cheight + 1) + 1;
  bpl = width >> 3;
  boldOffset = cwidth + cheight + 1;

  /*
   * Create image structure
   */
  idata = malloc(height*bpl);
  idata_len = height*bpl;
  for (i = 0;i < idata_len;i++)
    idata[i] = 0;

  extractImageData(idata,normal->data,normal->w,normal->h,bpl);
  extractImageData(idata+boldOffset*bpl,select->data,select->w,select->h,bpl);

  image = XCreateImage(TkGate.D,					/* Display */
		       DefaultVisualOfScreen(TkGate.S),		/* Visual */
		       1,					/* Depth */
		       XYPixmap,				/* format */
		       0,					/* offset */
		       idata,					/* data */
		       width,height,				/* width and height */
		       8,					/* bitmap pad */
		       0);					/* bytes per line */

  if (!image) {
    logError(ERL_ERROR,"XCreateImage(%d,%d) failed.",width,height);
    return 0;
  }

  makeRotations(image,0,cwidth,cheight);
  makeRotations(image,boldOffset,cwidth,cheight);

  me->me_pixmap = XCreatePixmap(TkGate.D,TkGate.root,width,height,1);
  XPutImage(TkGate.D,me->me_pixmap,TkGate.bitGC,image,0,0,0,0,width,height);

  Pixmap_initZoomSet(me->me_pixmap,width,height);

  xoffset[0] = 0; 	yoffset[0] = 0;
  xoffset[1] = cwidth;	yoffset[1] = 0;
  xoffset[2] = cheight;	yoffset[2] = cwidth;
  xoffset[3] = 0;	yoffset[3] = cheight;
  for (i = 0;i < 4;i++) {
    int W = (i&1) ? cheight : cwidth;
    int H = (i&1) ? cwidth : cheight;

    me->me_normal[i] = new_Icon(me->me_pixmap,xoffset[i],yoffset[i],W,H,0,0);
    me->me_select[i] = new_Icon(me->me_pixmap,xoffset[i],yoffset[i]+boldOffset,W,H,0,0);
  }

  XDestroyImage(image);

  return me;
}

/*
 * Delete and MssSymbol object.
 */
static void delete_MssSymbol(MssSymbol *me)
{
  int i;

  printf("delete_MssSymbol(%p)\n",me);

  if (me->me_pixmap != None) {
    Pixmap_deleteZoomSet(me->me_pixmap);
  }

  for (i = 0;i < 4;i++) {
    if  (me->me_normal[i])
      free(me->me_normal[i]);
    if  (me->me_select[i])
      free(me->me_select[i]);
  }
  free(me);
}

/*
 * Register a new MssSymbol object with the given normal and select icon data.  Return the
 * identifier for the new object.
 */
static unsigned MSS_register(GMIconData *normalData,GMIconData *selectData)
{
  MssSymbol *me = new_MssSymbol(++mss.mss_idCount,normalData,selectData);
  unsigned k = mss.mss_idCount & (MSS_SIZE-1);

  /*
   * Insert into lookup bucket
   */
  me->me_next = mss.mss_ents[k];
  mss.mss_ents[k] = me;


  /*
   * Insert into most-recently-used list.
   */
  me->me_luNext = mss.mss_mru;
  if (mss.mss_mru) {
    mss.mss_mru->me_luPrev = me;
  }
  mss.mss_mru = me;
  if (!mss.mss_lru) mss.mss_lru = me;
  mss.mss_numEntries++;

  if (mss.mss_numEntries > MSS_MAXENTRIES) {
    MssSymbol *xe = mss.mss_lru;
    mss.mss_lru = xe->me_luPrev;
    mss.mss_lru->me_luNext = 0;
    delete_MssSymbol(xe);
    mss.mss_numEntries--;
  }

  return mss.mss_idCount;
}

/*
 * Find the MssSymbol object with the given identifier.
 */
static MssSymbol *MSS_find(unsigned gid)
{
  unsigned k = gid & (MSS_SIZE-1);
  MssSymbol *e;

  for (e = mss.mss_ents[k];e;e = e->me_next) {
    if (e->me_gid == gid)
      return e;
  }
  return 0;
}

/*
 * Lookup the Icon object with the given identifier (gid), orientation (r) and boldness (isBold).
 */
static Icon *MSS_lookup(int gid,int r,int isBold)
{
  MssSymbol *e = 0;

  if (gid == 0) return 0;
  e = MSS_find(gid);
  if (!e) return 0;

  /*
   * Update usage position
   */
#if 0
  if (e->me_luNext)
    e->me_luNext->me_luPrev = e->me_luPrev;
  else
    mss.mss_lru = e->me_luPrev;

  if (e->me_luPrev)
    e->me_luPrev->me_luNext = e->me_luNext;
  else
    mss.mss_mru = e->me_luNext;

  e->me_luPrev = 0;
  e->me_luNext = mss.mss_mru;
  mss.mss_mru = e;
#endif

  return isBold ? e->me_select[r] :  e->me_normal[r];
}

/*
 * Create a port for a GModSymbol
 */
GSymPort *new_GSymPort(const char *name,int x,int y,int t,int r,int s)
{
  GSymPort *msp;
  msp = (GSymPort*) ob_malloc(sizeof(GSymPort),"GSymPort");
  msp->msp_name = ob_strdup(name);
  msp->msp_x = x;
  msp->msp_y = y;
  msp->msp_type = t;
  msp->msp_orient = r;
  msp->msp_size = s;

  return msp;
}

/*
 * Initialize the module symbol server.
 */
void MSS_init()
{
  int i;

  mss.mss_idCount = 0;
  mss.mss_numEntries = 0;
  mss.mss_mru = 0;
  mss.mss_lru = 0;

  for (i = 0;i < MSS_SIZE;i++)
    mss.mss_ents[i] = 0;
}


void delete_GSymPort(GSymPort *msp)
{
  ob_free(msp->msp_name);
  ob_free(msp);
}

void GMIconData_init(GMIconData *id)
{
  id->data = ob_strdup("");
  id->x = id->y = 0;
  id->w = id->h = 0;
}

void GMIconData_copy(GMIconData *id,GMIconData *src_id)
{
  id->data = ob_strdup(src_id->data);
  id->x = src_id->x;
  id->y = src_id->y;
  id->w = src_id->w;
  id->h = src_id->h;
}

void GMIconData_uninit(GMIconData *id)
{
  ob_free(id->data);
  id->data = 0;
}

void GMIconData_set(GMIconData *id,const char *data,int x,int y)
{
  unsigned w,h;

  ob_free(id->data);
  id->data = ob_strdup(data);
  id->x = x;
  id->y = y;

  if (sscanf(data,"%*s %*s %u %*s %*s %u",&w,&h) == 2) {
    id->w = w;
    id->h = h;
  } else {
    id->w = 0;
    id->h = 0;
  }
}

/*
 * Create a new module symbol
 */
GModSymbol *new_GModSymbol()
{
  GModSymbol *ms;
  int i;

  ms = (GModSymbol*) ob_malloc(sizeof(GModSymbol),"GModSymbol");
  ms->ms_gid = 0;
  ms->ms_refCount = 0;
  ms->ms_numPorts = 0;
  ms->ms_ports = 0;
  for (i = 0;i < 4;i++) {
    ms->ms_ix[0][i] = ms->ms_iy[0][i] = 0;
    ms->ms_ix[1][i] = ms->ms_iy[1][i] = 0;
  }

  GMIconData_init(&ms->ms_normal);
  GMIconData_init(&ms->ms_select);

  if (!symbolTable) symbolTable = new_PHash();
  PHash_insert(symbolTable,ms,ms);

  GModSymbol_setNormalIcon(ms, defaultSymbol_data, defaultSymbol_x, defaultSymbol_y);
  GModSymbol_setSelectIcon(ms, defaultBoldSymbol_data, defaultSymbol_x, defaultSymbol_y);

  return ms;
}

/*
 * Create a new module symbol
 */
GModSymbol *copy_GModSymbol(GModSymbol *src_ms)
{
  GModSymbol *ms;
  int i;


  ms = (GModSymbol*) ob_malloc(sizeof(GModSymbol),"GModSymbol");
  ms->ms_gid = 0;
  ms->ms_refCount = 0;
  ms->ms_numPorts = 0;
  ms->ms_ports = 0;
  for (i = 0;i < 4;i++) {
    ms->ms_ix[0][i] = src_ms->ms_ix[0][i];
    ms->ms_ix[1][i] = src_ms->ms_ix[1][i];
    ms->ms_iy[0][i] = src_ms->ms_iy[0][i];
    ms->ms_iy[1][i] = src_ms->ms_iy[1][i];
  }

  GMIconData_copy(&ms->ms_normal,&src_ms->ms_normal);
  GMIconData_copy(&ms->ms_select,&src_ms->ms_select);
  ms->ms_gid = 0;
  ms->ms_finalized = 0;

  for (i = 0;i < src_ms->ms_numPorts;i++) {
    GSymPort *p = src_ms->ms_ports[i];
    GModSymbol_addPort(ms,new_GSymPort(p->msp_name,p->msp_x,p->msp_y,p->msp_type,p->msp_orient,p->msp_size));
  }

  if (!symbolTable) symbolTable = new_PHash();
  PHash_insert(symbolTable,ms,ms);

#if DEBUG_MOD_SYM
  {
    char buf[STRMAX];
    sprintf(buf,"%p - rc=%d gid=%d nports=%d",ms,ms->ms_refCount,ms->ms_gid,ms->ms_numPorts);
    DoTclL("DebugModSym::add",buf,NULL);
  }
#endif

  return ms;
}

/*
 * Delete the module symbol
 */
void delete_GModSymbol(GModSymbol *ms)
{
  GMIconData_uninit(&ms->ms_normal);
  GMIconData_uninit(&ms->ms_select);
  GModSymbol_flushPorts(ms);

#if DEBUG_MOD_SYM
  {
    char buf[STRMAX];
    sprintf(buf,"%p - rc=%d gid=%d nports=%d",ms,ms->ms_refCount,ms->ms_gid,ms->ms_numPorts);
    DoTclL("DebugModSym::del",buf,NULL);
  }
#endif

  ob_free(ms);

  if (!symbolTable) symbolTable = new_PHash();
  PHash_remove(symbolTable,ms);
}

void GModSymbol_attach(GModSymbol *ms)
{
  ob_touch(ms);
  ms->ms_refCount++;

#if DEBUG_MOD_SYM
  {
    char buf[STRMAX];
    sprintf(buf,"%p - rc=%d gid=%d nports=%d",ms,ms->ms_refCount,ms->ms_gid,ms->ms_numPorts);
    DoTclL("DebugModSym::update",buf,NULL);
  }
#endif
}

void GModSymbol_detach(GModSymbol *ms)
{
  ob_touch(ms);
  if (--ms->ms_refCount <= 0) {
    delete_GModSymbol(ms);
  } else {
#if DEBUG_MOD_SYM
    char buf[STRMAX];
    sprintf(buf,"%p - rc=%d gid=%d nports=%d",ms,ms->ms_refCount,ms->ms_gid,ms->ms_numPorts);
    DoTclL("DebugModSym::update",buf,NULL);
#endif
  }
}

/*
 * Flush all of the module ports
 */
void GModSymbol_flushPorts(GModSymbol *ms)
{
  int i;

  ob_touch(ms);
  for (i = 0;i < ms->ms_numPorts;i++)
    delete_GSymPort(ms->ms_ports[i]);
  if (ms->ms_ports) ob_free(ms->ms_ports);
  ms->ms_numPorts = 0;
  ms->ms_ports = 0;

#if DEBUG_MOD_SYM
  {
    char buf[STRMAX];
    sprintf(buf,"%p - rc=%d gid=%d nports=%d",ms,ms->ms_refCount,ms->ms_gid,ms->ms_numPorts);
    DoTclL("DebugModSym::update",buf,NULL);
  }
#endif
}

static void GModSymbol_updateCenter(GModSymbol *ms)
{
  int minx = imin(ms->ms_normal.x, ms->ms_select.x);
  int miny = imin(ms->ms_normal.y, ms->ms_select.y);
  int maxx = imax(ms->ms_normal.x + ms->ms_normal.w, ms->ms_select.x + ms->ms_select.w);
  int maxy = imax(ms->ms_normal.y + ms->ms_normal.h, ms->ms_select.y + ms->ms_select.h);
  //  int width = maxx - minx - 1;
  //  int height = maxy - miny - 1;
  int i,j;

  ob_touch(ms);

#if 0
  printf("updateCenter %dx%d  N:%dx%d  S:%dx%d\n",
	 width,height,
	 ms->ms_normal.w,ms->ms_normal.h,
	 ms->ms_select.w,ms->ms_select.h
	 );
#endif

  ms->ms_cx = (maxx+minx)/2;
  ms->ms_cy = (maxy+miny)/2;

  ms->ms_basex = minx;
  ms->ms_basey = miny;

  ms->ms_ix[0][0] = ms->ms_normal.x - ms->ms_cx;
  ms->ms_iy[0][0] = ms->ms_normal.y - ms->ms_cy;
  ms->ms_ix[1][0] = ms->ms_select.x - ms->ms_cx;
  ms->ms_iy[1][0] = ms->ms_select.y - ms->ms_cy;

  ms->ms_ix[0][1] = ms->ms_normal.x + ms->ms_normal.w - ms->ms_cx - 1;
  ms->ms_iy[0][1] = ms->ms_normal.y - ms->ms_cy;
  ms->ms_ix[1][1] = ms->ms_select.x + ms->ms_select.w - ms->ms_cx - 1;
  ms->ms_iy[1][1] = ms->ms_select.y - ms->ms_cy;

  ms->ms_ix[0][2] = ms->ms_normal.x + ms->ms_normal.w - ms->ms_cx - 1;
  ms->ms_iy[0][2] = ms->ms_normal.y + ms->ms_normal.h - ms->ms_cy - 1;
  ms->ms_ix[1][2] = ms->ms_select.x + ms->ms_select.w - ms->ms_cx - 1;
  ms->ms_iy[1][2] = ms->ms_select.y + ms->ms_select.h - ms->ms_cy - 1;

  ms->ms_ix[0][3] = ms->ms_normal.x - ms->ms_cx;
  ms->ms_iy[0][3] = ms->ms_normal.y + ms->ms_normal.h - ms->ms_cy - 1;
  ms->ms_ix[1][3] = ms->ms_select.x - ms->ms_cx;
  ms->ms_iy[1][3] = ms->ms_normal.y + ms->ms_normal.h - ms->ms_cy - 1;

  for (i = 1;i < 4;i++) {
    rotatePoint(&ms->ms_ix[0][i],&ms->ms_iy[0][i],0,0,i);
    rotatePoint(&ms->ms_ix[1][i],&ms->ms_iy[1][i],0,0,i);
  }


  for (i = 0;i < ms->ms_numPorts;i++) {
    GSymPort *msp = ms->ms_ports[i];

    for (j = 0;j < 4;j++) {
      GPadLoc *l = &msp->msp_loc[j];
      int x = msp->msp_x - ms->ms_cx;
      int y = msp->msp_y - ms->ms_cy;

      rotatePoint(&x,&y,0,0,j);
      l->x1 = l->x2 = x;
      l->y1 = l->y2 = y;
      l->dir = (msp->msp_orient + j) % 4;
    }
  }

  ms->ms_finalized = 1;
}

/*
 * Add a port to a module 
 */
void GModSymbol_addPort(GModSymbol *ms,GSymPort *p)
{
#if 0
  printf("addPort %s[%d]\n",p->msp_name,p->msp_size);
#endif

  ob_touch(ms);
  if ((ms->ms_numPorts % MSP_STEPSIZE) == 0) {
    if (ms->ms_numPorts > 0)
      ms->ms_ports = ob_realloc(ms->ms_ports,
				sizeof(GSymPort*)*(ms->ms_numPorts+MSP_STEPSIZE));
    else
      ms->ms_ports = ob_malloc(sizeof(GSymPort*)*MSP_STEPSIZE,"GSymPort*");
  }
  ob_touch(ms->ms_ports);
  ms->ms_ports[ms->ms_numPorts++] = p;

  ms->ms_finalized = 0;

#if DEBUG_MOD_SYM
  {
    char buf[STRMAX];
    sprintf(buf,"%p - rc=%d gid=%d nports=%d",ms,ms->ms_refCount,ms->ms_gid,ms->ms_numPorts);
    DoTclL("DebugModSym::update",buf,NULL);
  }
#endif
}

void GModSymbol_setNormalIcon(GModSymbol *ms,const char *data,int x,int y)
{
  ob_touch(ms);
  GMIconData_set(&ms->ms_normal,data,x,y);
  ms->ms_gid = 0;
  ms->ms_finalized = 0;
}

void GModSymbol_setSelectIcon(GModSymbol *ms,const char *data,int x,int y)
{
  ob_touch(ms);
  GMIconData_set(&ms->ms_select,data,x,y);
  ms->ms_gid = 0;
  ms->ms_finalized = 0;
}

const char *GModSymbol_getNormalIcon(GModSymbol *ms,int *x,int *y)
{
  *x = ms->ms_normal.x;
  *y = ms->ms_normal.y;
  return ms->ms_normal.data;
}

const char *GModSymbol_getSelectIcon(GModSymbol *ms,int *x,int *y)
{
  *x = ms->ms_select.x;
  *y = ms->ms_select.y;
  return ms->ms_select.data;
}

/*
 * Get the icon for a symbol with orientation r, and boldness isBold.
 */
static Icon *GModSymbol_getIcon(GModSymbol *ms,int r,int isBold)
{
  Icon *I = 0;

  I = MSS_lookup(ms->ms_gid,r,isBold);
  if (!I) {
    ms->ms_gid = MSS_register(&ms->ms_normal,&ms->ms_select);
    I = MSS_lookup(ms->ms_gid,r,isBold);
#if 0
    printf("gotIcon: (%d, %d) %dx%d pm=%x\n",I->x,I->y,I->width,I->height,(unsigned)I->pm);
#endif
  }


  return I;
}

/*****************************************************************************
 *
 * Get the extents of a module symbol
 *
 * Parameters:
 *    ms		Symbol to get extentds of
 *    *minx		Return for minimum x value
 *    *miny		Return for minimum y value
 *    *maxx		Return for maximum x value
 *    *maxy		Return for maximum y value
 *
 *****************************************************************************/
void GModSymbol_getExtents(GModSymbol *ms,int r,int *minx,int *miny,int *maxx,int *maxy)
{
  int w = ms->ms_normal.w;
  int h = ms->ms_normal.h;

  *minx = ms->ms_ix[0][r];
  *miny = ms->ms_iy[0][r];
  *maxx = ms->ms_ix[0][r] + w;
  *maxy = ms->ms_iy[0][r] + h;
}

/*****************************************************************************
 *
 * Draw a symbol at the specified position.
 *
 * Parameters:
 *      ms		Symbol to draw
 *      x		x-coordinate at which to draw symbol
 *      y		y-coordinate at which to draw symbol
 *      r		rotation for symbol
 *      isBold		Non-zero for bold version of symbol
 *
 *****************************************************************************/
void GModSymbol_draw(GModSymbol *ms,int x,int y,int r,int isBold)
{
  Icon *I = GModSymbol_getIcon(ms,r,isBold);
  int ix,iy;

  isBold = (isBold != 0);	/* Make sure isBold is only 0 or 1 */

#if 0
  printf("GModSymbol_draw(x=%d, y=%d, r=%d, b=%d)\n",x,y,r,isBold);
#endif

  if (!ms->ms_finalized) GModSymbol_updateCenter(ms);

  ix = ctow_x(x) + ms->ms_ix[isBold][r];
  iy = ctow_y(y) + ms->ms_iy[isBold][r];

  Icon_draw(TkGate.D,TkGate.W,TkGate.moduleGC,ix,iy,I);
}

HashElem *GModSymbol_first()
{
  if (!symbolTable) 
    return 0;
  else
    return Hash_first(symbolTable);
}

HashElem *GModSymbol_next(HashElem *e)
{
  if (!symbolTable)
    return 0;
  else
    return Hash_next(symbolTable,e);
}


int GModSymbol_numPorts(GModSymbol *ms)
{
  if (!ms->ms_finalized) GModSymbol_updateCenter(ms);
  return ms->ms_numPorts;
}

GSymPort *GModSymbol_getPort(GModSymbol *ms,int p)
{
  if (!ms->ms_finalized) GModSymbol_updateCenter(ms);
  return ms->ms_ports[p];
}
