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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "thyme.h"

/*

   root
    |
    v
  +---+MemNode
  |   |
  +---+      +---+MemNode
  | *-+----->|   |
  +---+      +---+
  |   |      |   |
  +---+      +---+      +---+MemNode
  |   |      | *-+----->|   |
  +---+      +---+      +---+
             |   |      |   |
             +---+      +---+
                        |   |
                        +---+      +---+MemPage
                        | *-+----->| D |
                        +---+      +---+
                                   | D |
                                   +---+
                                   | D |
                                   +---+
                                   | D |
                                   +---+
 */

static MemPage *new_MemPage(unsigned base,int bpw)
{
  MemPage *P = (MemPage *) malloc(sizeof(MemPage));
  int n = (bpw<< MEM_BYTEBITS);
  int i;

#if 0
  sendMsg("comment create page @ %x",base);
#endif

  P->motype = MEM_PAGE;
  P->baseAddr = base;
  P->data1 = (char*)malloc(n);
  P->data0 = (char*)malloc(n);
  P->dataz = (char*)malloc(n);

  for (i = 0;i < n;i++) {
    P->data1[i] = MEM_BYTEMASK;
    P->data0[i] = MEM_BYTEMASK;
    P->dataz[i] = MEM_BYTEMASK;
  }

  return P;
}

static void delete_MemPage(MemPage *P)
{
  free(P->data1);
  free(P->data0);
  free(P->dataz);
  free(P);
}

static MemNode *new_MemNode()
{
  int i;
  MemNode *N = (MemNode *) malloc(sizeof(MemNode));

#if 0
  sendMsg("comment create node");
#endif

  N->motype = MEM_NODE;
  for (i = 0;i < MEM_BYTEMAX;i++)
    N->ptr[i] = 0;

  return N;
}

static void delete_MemNode(MemNode *N)
{
  int i;
  for (i = 0;i < MEM_BYTEMAX;i++) {
    if (N->ptr[i]) {
      MemBase *C =  (MemBase *) N->ptr[i];
      if (C->motype == MEM_NODE)
	delete_MemNode((MemNode*)C);
      else if (C->motype == MEM_PAGE)
	delete_MemPage((MemPage*)C);
    }
  }
  free (N);
}

static MemPage *Memory_getPage(Memory *M,unsigned A)
{ 
  int addr[4];
  MemNode *N;
  int i;

  addr[0] = (A >> 24) & MEM_BYTEMASK;
  addr[1] = (A >> 16) & MEM_BYTEMASK;
  addr[2] = (A >> 8) & MEM_BYTEMASK;
  addr[3] = (A     ) & MEM_BYTEMASK;

  N = M->root;
  for (i = 0;i < 2;i++) {
    if (!N->ptr[addr[i]])
      N->ptr[addr[i]] = new_MemNode();
    N = (MemNode*)N->ptr[addr[i]];
  }

  if (!N->ptr[addr[2]])
    N->ptr[addr[2]] = new_MemPage(A&~MEM_BYTEMASK,M->bpw);
  return (MemPage*)N->ptr[addr[2]];
}

static void Memory_lookup(Memory *M,unsigned A,unsigned char **D1,unsigned char **D0,unsigned char **Dz)
{
  MemPage *P = Memory_getPage(M,A);
  unsigned w = (A&MEM_BYTEMASK);
  unsigned b = w << M->byte_shift;

  *D1 = P->data1 + b;
  *D0 = P->data0 + b;
  *Dz = P->dataz + b;

  //  *U = P->datax + w;
}

static void Memory_monitorWrite(Memory *M,unsigned A)
{
  static Value *data = 0;

  if (A >= M->m_monitor.startAddr && A <= M->m_monitor.stopAddr) {
    char dstr[STRMAX];

    if (!data)
      data = new_Value(M->m_dbits);
    else
      Value_resize(data, M->m_dbits);

    Memory_get(M,A,data);
    Value_getstr(data,dstr);
    vgio_printf("memory %s %u : %s\n",Net_getName(M->m_net),A,dstr);
  }
}

/******************************************************************************
 *
 * Called to indicated that the simulation accessed memory m at address addr.
 *
 ******************************************************************************/
void Memory_accessNotify(Memory *m, unsigned addr,int isWrite)
{
  EvQueue *Q = vgsim.vg_circuit.c_evQueue;
  char sdata[STRMAX];
  Value *data = 0;

  m->m_monitor.access = addr;
  if (m->m_monitor.active) {
    if (!data)
      data = new_Value(m->m_dbits);
    else
      Value_resize(data, m->m_dbits);
    Memory_get(m, addr, data);
    Value_getstr(data,sdata);
    vgio_printf("memory-addr %s %u \"%s\" %s @ %llu\n",
		Net_getName(m->m_net),addr,sdata,
		isWrite?"write":"read",
		EvQueue_getCurTime(Q)
		);
  }
}

void Memory_get(Memory *M,unsigned A,Value *data)
{
  unsigned char *D1, *D0, *Dz;
  int bytesPerWord = (Value_nbits(data)+7)/8;

  Memory_lookup(M,A,&D1, &D0,&Dz);

  memcpy(data->one, D1, bytesPerWord);
  memcpy(data->zero, D0, bytesPerWord);
  memcpy(data->flt, Dz, bytesPerWord);
}

void Memory_put(Memory *M,unsigned A,Value *data)
{
  unsigned char *D1, *D0, *Dz;
  int nbits = Value_nbits(data);
  int bytesPerWord;

  /*
   * Truncate data if it is bigger then memory word size
   */
  if (M->m_dbits < nbits) nbits = M->m_dbits;

  /*
   * Compute number of bytes to store
   */
  bytesPerWord = (nbits+7)/8;

#if 0
  {
    char buf[STRMAX];
    Value_getstr(data,buf);
    vgio_echo("* Memory_put(%x, %s)\n",A,buf);
  }
#endif
 
  Memory_lookup(M,A,&D1,&D0,&Dz);

  memcpy(D1,data->one, bytesPerWord);
  memcpy(D0,data->zero, bytesPerWord);
  memcpy(Dz,data->flt, bytesPerWord);

  if (M->m_monitor.active)
    Memory_monitorWrite(M,A);
}

void Memory_putRange(Memory *M,unsigned A,unsigned memLsb,Value *data,unsigned vMsb,unsigned vLsb)
{
  unsigned char *D1, *D0, *Dz;
  int bytesPerWord = (M->m_dbits+7)/8;
  Value *temp = new_Value(M->m_dbits);

  Memory_lookup(M,A,&D1,&D0,&Dz);

  memcpy(temp->one, D1, bytesPerWord);
  memcpy(temp->zero, D0, bytesPerWord);
  memcpy(temp->flt, Dz, bytesPerWord);

  Value_copyRange(temp,memLsb,data,vMsb,vLsb);

  memcpy(D1,temp->one, bytesPerWord);
  memcpy(D0,temp->zero, bytesPerWord);
  memcpy(Dz,temp->flt, bytesPerWord);

  delete_Value(temp);

  if (M->m_monitor.active)
    Memory_monitorWrite(M,A);
}

void Memory_flush(Memory *M)
{
  delete_MemNode(M->root);
  M->root = new_MemNode();
}

/*
  Stores a line containing an address and values into a memory.

  Example line: "1000/	10 4e 9a 23 8e"
 */
int Memory_putLine(Memory *M,char *line)
{
  char buf[STRMAX];
  unsigned addr;
  char *T;
  Value *data = new_Value(M->m_dbits);
  int ret_code = 0;

  if (sscanf(line,"%x / %[^\n]",&addr,buf) != 2) {
    if (sscanf(line,"%s",buf) == 1 && *buf != '#') {
      ret_code = -1;
    } else
      ret_code = 0;
  } else {
    for (T = strtok(buf," \t");T;T = strtok(0," \t")) {

      if (Value_convertHex(data,T,M->m_dbits) != 0)
	ret_code = -1;
      Memory_put(M,addr, data);
	
      addr++;
    }
  }

  delete_Value(data);
  return ret_code;
}

/*

 */
int Memory_readFile(Memory *M,const char *fileName)
{
  FILE *f;
  char buf[STRMAX];

  if (!(f = openInPath(fileName)))
    return -1;

  while (fgets(buf,STRMAX,f)) {
    int r = Memory_putLine(M,buf);
    if (r != 0) return r;
  }
  return 0;
}

int MemPage_dump(MemPage *P,Memory *M,unsigned base,FILE *f,unsigned flags,unsigned start,unsigned stop)
{
  char buf[STRMAX];
  int i;
  int need_addr = 1;
  int num_on_line = 0;
  Value *data = new_Value(M->m_dbits);
  Value *unknown = new_Value(M->m_dbits);

  Value_unknown(unknown);

  /*
   * This block is entirely before the start address
   */
  if (base+(MEM_BYTEMAX-1) < start)
    return 0;


  for (i = 0;i < MEM_BYTEMAX;i++) {
    unsigned addr = base|i;

    /*
     * We have not yet reached the start address.
     */
    if (addr < start)
      continue;

    /*
     * We are already past the highest address
     */
    if (addr > stop) break;

    Memory_get(M,addr,data);

    if (Value_isEqual(data,unknown)) {
      if (num_on_line)
	fprintf(f,"\n");
      num_on_line = 0;

      need_addr = 1;
      continue;
    }

    if (need_addr) {
      fprintf(f,"\n@%x\n",addr);
      need_addr = 0;
    }

    if ((flags & SF_BIN))
      Value_format(data,"%b",buf);
    else if ((flags & SF_OCT))
      Value_format(data,"%o",buf);
    else if ((flags & SF_DEC))
      Value_format(data,"%d",buf);
    else
      Value_format(data,"%h",buf);

    fprintf(f," %s",buf);
    num_on_line++;

    if (num_on_line >= 16) {
      fprintf(f,"\n");
      num_on_line = 0;
    }
  }

  if (num_on_line)
    fprintf(f,"\n");

  delete_Value(data);
  delete_Value(unknown);

  return 0;
}

void MemNode_dump(MemNode *N,Memory *M,unsigned base,FILE *f,unsigned flags,unsigned start,unsigned stop)
{
  int i;

  if (N->motype == MEM_PAGE) {
    MemPage_dump((MemPage*)N,M,base,f,flags,start,stop);
    return;
  }

  for (i = 0;i < MEM_BYTEMAX;i++)
    if (N->ptr[i])
      MemNode_dump((MemNode*)N->ptr[i],M,(base|i)<<8,f,flags,start,stop);
}

void Memory_dump(Memory *M,FILE *f,unsigned flags,unsigned start,unsigned stop)
{
  if (M->root)
    MemNode_dump(M->root,M,0x0,f,flags,start,stop);
}

int Memory_writeFile(Memory *M,const char *fileName)
{
  FILE *f;

  if (!(f = fopen(fileName,"w")))
    return -1;

  Memory_dump(M,f,SF_HEX,0,~0);

  fclose(f);
  return 0;
}

Memory *new_Memory(unsigned beginAddr,unsigned endAddr,int dbits,Net *net)
{
  Memory *M = (Memory*) malloc(sizeof(Memory));

  Memory_init(M,beginAddr,endAddr,dbits,net);

  return M;
}

void Memory_init(Memory *M,unsigned beginAddr,unsigned endAddr,int dbits,Net *net)
{
  int i,n;

  M->m_flags = 0; 
  M->m_beginAddr = beginAddr;
  M->m_endAddr = endAddr;
  M->m_dbits = dbits;
  M->m_net = net;

  for (i = 0,n = 1;n < dbits;n <<= 1,i++);
  M->byte_shift = (i >= 3) ? (i-3) : 0;
  M->bpw = n;
  M->nbytes = (n+7)/8;
  M->root = new_MemNode();
  M->m_monitor.active = 0;
  M->m_monitor.access = 0;
}

void Memory_setMonitor(Memory *m,unsigned startAddr,unsigned stopAddr)
{
  m->m_monitor.active = 1;
  m->m_monitor.startAddr = startAddr;
  m->m_monitor.stopAddr = stopAddr;
}

void Memory_unsetMonitor(Memory *m)
{
  m->m_monitor.active = 0;
}

void memory_test()
{
  Memory *M = new_Memory(0,65535,32,0);
  char buf[1024],val[1024];
  unsigned addr;
  Value *data;

  data = new_Value(32);
  while (printf(">>> "),fgets(buf,1024,stdin)) {
    if (sscanf(buf,"g %x",&addr)) {

      Memory_get(M,addr,data);
      printf("data = ");
      Value_print(data,stdout);
      printf("\n");
    } else if (sscanf(buf,"s %x %s",&addr,val)) {
      Value_convert(data, val);
      Memory_put(M,addr,data);
    } else if (sscanf(buf,"l %s",val)) {
      Memory_readFile(M,val);
    } else if (sscanf(buf,"d %s",val)) {
      Memory_writeFile(M,val);
    }
  }
}

