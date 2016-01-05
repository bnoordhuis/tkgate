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
#ifndef __memory_h
#define __memory_h

#define MEM_BYTEMAX	256
#define MEM_BYTEBITS	8
#define MEM_BYTEMASK	0xff

#define MEM_NODE	0
#define MEM_PAGE	1

/*
 * Flags for memory devices
 */
#define MF_INITIALIZED	0x1		/* Memory has been initialized */

typedef struct {
  int		motype;
} MemBase;

typedef struct {
  int		motype;
  unsigned 	baseAddr;		/* Base address of page */
  unsigned char *data1;			/* 1-data */
  unsigned char *data0;			/* 0-data */
  unsigned char *dataz;			/* z-data */
} MemPage;

typedef struct memnode {
  int		motype;
  void		*ptr[MEM_BYTEMAX];
} MemNode;

/*****************************************************************************
 *
 * Memory - represents a memory device.
 *
 *****************************************************************************/
typedef struct memory {
  unsigned	m_flags;			/* Memory device flags */
  unsigned	m_beginAddr;			/* beginning address */
  unsigned	m_endAddr;			/* ending address */
  int		m_dbits;			/* data bits */
  int		byte_shift;			/*  */
  int		nbytes;				/* Number of bytes needed for data words */
  int		bpw;				/* Number of bytes per word */
  MemNode	*root;				/* Top-level memory node */
  Net		*m_net;				/* Net associated with this memory */
  struct {
    int		active;				/* Non-zero if monitor is active */
    unsigned	startAddr;			/* Start address of monitored range */
    unsigned	stopAddr;			/* Stop address of monitored range */
    unsigned	access;				/* Last accessed address */
  } m_monitor;
} Memory;

Memory *new_Memory(unsigned beginAddr,unsigned endAddr,int dbits,Net *net);
void delete_Memory(Memory*);
void Memory_init(Memory *,unsigned beginAddr,unsigned endAddr,int dbits,Net *net);
void Memory_flush(Memory*);

void Memory_get(Memory *M,unsigned A,Value *v);
void Memory_put(Memory *M,unsigned A,Value *v);
void Memory_putRange(Memory *M,unsigned A,unsigned memLsb,Value *v,unsigned vMsb,unsigned vLsb);

void Memory_setMonitor(Memory *m,unsigned startAddr,unsigned stopAddr);
void Memory_unsetMonitor(Memory *m);

int Memory_readFile(Memory*,const char*); 
int Memory_writeFile(Memory*,const char*); 
void Memory_accessNotify(Memory *m, unsigned addr,int isWrite);

void Memory_dump(Memory *M,FILE *f,unsigned flags,unsigned start,unsigned stop);

int Memory_putLine(Memory *M,char *line);

#define Memory_beginAddr(m) (m)->m_beginAddr
#define Memory_endAddr(m) (m)->m_endAddr
#define Memory_dataNBits(m) (m)->m_dbits
#define Memory_getFlags(m) (m)->m_flags
#define Memory_setFlags(m,f) ((m)->m_flags |= (f))
#define Memory_clearFlags(m,f) ((m)->m_flags &= ~(f))

#endif
