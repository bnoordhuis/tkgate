/****************************************************************************
    Copyright (C) 1987-2008 by Jeffery P. Hansen

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
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include "tkgate.h"


static SHash *primitiveTable = 0;

static void Primitive_generator(FILE *f,const char *vsStart,const char *vsEnd,PrimParm *primParm);


/*****************************************************************************
 *
 * Load the definition for primitive "name" from the appropriate file.
 *
 * Parameters:
 *    name		Name of the primitive to load
 *
 * Returns:		Object with Primitive.
 *
 * Looks in the file "${TKGATE_HOME}/primitives/name.v" for the primitive
 * cell definition.
 *
 *****************************************************************************/
static Primitive *Primitive_load(const char *name)
{
  Primitive *p = (Primitive*)malloc(sizeof(Primitive));
  char path[STRMAX];
  char *body,*s;
  FILE *f;
  struct stat sb;  

  p->p_name = strdup(name);
  p->p_body = 0;
  p->p_end = 0;

  sprintf(path,"%s/primitives/%s.v",TkGate.homedir,name);

  if (stat(path,&sb) != 0 || !(f = fopen(path,"r"))) {
    message(1,msgLookup("primitive.err.missing"),name);
    return p;
  }
  body = (char*)malloc(sb.st_size+1);

  fread(body, 1, sb.st_size, f);
  fclose(f);
  body[sb.st_size] = 0;

  //
  // Skip to "module" keyword.
  //
  body = strstr(body,"module");
  if (!body) {
    message(1,msgLookup("primitive.err.nomodule"),name);
    return p;
  }

  s = body + 6;				// Skip over "module"
  while (isspace(*s))s++;		//    and whitespace
  p->p_nameStart = s;			// this is the start of the module name
  while (*s && !isspace(*s))s++;	//    Skip to end of the module name
  p->p_nameEnd = s;

  if (p->p_nameStart == p->p_nameEnd || !isalpha(*p->p_nameStart)) {
    message(1,msgLookup("primitive.err.nomodname"),name);
    return p;
  }
  if (strncmp(name,p->p_nameStart,p->p_nameEnd-p->p_nameStart) != 0) {
    message(1,msgLookup("primitive.err.wrongname"),name);
    return p;
  }

  p->p_parmStart = 0;
  p->p_parmEnd = 0;

  while (isspace(*s)) s++;
  if (*s == '#') {
    char *start = s;
    char *end = 0;
    s++;
    while (isspace(*s)) s++;
    if (*s == '(') {
      int c = 1; 
      for (s++;*s && c > 0;s++) {
	if (*s == '(') c++;
	if (*s == ')') c--;
      }
      if (*s)
	end = s;
    }

    if (start && end) {
      char buf[STRMAX];
      memmove(buf,start,end-start);
      buf[end-start] = 0;

      p->p_parmStart = start;
      p->p_parmEnd = end;
    }
  }

  p->p_body = body;
  p->p_end = body + strlen(body);

  return p;
}

/*****************************************************************************
 *
 * Lookup the definition for primitive "name", loading if necessary 
 *
 * Parameters:
 *    name		Name of the primitive to load
 *
 * Returns:		Object with Primitive.
 *
 *****************************************************************************/
static Primitive *Primitive_lookup(const char *name)
{
  Primitive *p;

  if (!primitiveTable) primitiveTable = new_SHash_noob();

  p = (Primitive*)SHash_find(primitiveTable,name);
  if (!p) {
    p = Primitive_load(name);
    SHash_insert(primitiveTable,name,p);
  }

  return p;
}

/*****************************************************************************
 *
 * Find the next variable call sequence in a string.
 *
 * Parameters:
 *    begin		Beginning of string to search 
 *    end		End of string to search 
 *    vsStart		Beginning of string to search for
 *    vsEnd		End of string to search for
 *
 * Returns:		Non-zero if a variable call was found
 *
 *****************************************************************************/
static int Primitive_nextVariable(const char *begin,const char *end,
				  const char **vsStart,const char **vsEnd)
{
  const char *s = begin;

  for (;;) {
    //
    // Advance to the next $ or % character 
    //
    s = s+strcspn(s, "$%\\");
    if (!*s) return 0;
    if (s >= end) return 0;


    if (strncmp(s,"${",2) == 0 || strncmp(s,"$${",3) == 0) {
      int count = 1;

      *vsStart = s;
      while (*s != '{')s++;
      while (count > 0 && *++s) {
	if (*s == '{') count++;
	if (*s == '}') count--;
      }
      *vsEnd = s+1;
      return 1;
    } else if (*s == '%') {
      *vsStart = s;
      *vsEnd = s+1;
      return 1;
    } else if (*s == '\\') {
      *vsStart = s;
      if (s[1])
	*vsEnd = s+2;
      else
	*vsEnd = s+1;
      return 1;
    }

    //
    // This wasn't a special sequence, skip a character and keep looking.
    //
    s++;
  }

  return 0;
}

void Primitive_substitute(FILE *f,int id,const char *vsStart,const char *vsEnd,PrimParm *primParm)
{
  //
  // We are assuming the variable call begins with this sequence
  //
  if (strncmp(vsStart,"${",2) != 0)
    return;
  vsStart += 2;


  //
  // We are assuming the variable call ends with this sequence
  //
  if (vsEnd[-1] != '}')
    return;
  vsEnd--;

  fprintf(f,"%s",PrimParm_nget(primParm,vsStart,vsEnd-vsStart));
}

static void Primitive_writeText(FILE *f,const char *begin,const char *end,
				int id,PrimParm *primParm)
{
  const char *vsStart,*vsEnd, *s;

  s = begin;

  while (Primitive_nextVariable(s,end,&vsStart,&vsEnd)) {
    //
    // Make sure variable isn't after end of target area
    //
    if (vsEnd > end) break;

    //
    // Write everything up to the start of the variable
    //
    fwrite(s,1,vsStart-s,f);

    //
    // Write the special text
    //
    if (strncmp(vsStart,"${",2) == 0)
      Primitive_substitute(f,id,vsStart,vsEnd,primParm);
    else if (strncmp(vsStart,"$${",3) == 0)
      Primitive_generator(f,vsStart,vsEnd,primParm);
    else if (*vsStart == '%' && id >= 0 ) {
      fprintf(f,"%d",id);
    } else if (*vsStart == '\\' && id >= 0) {
      if (vsStart[1] == 'n')
	fprintf(f,"\n");
    } else {
      fwrite(vsStart,1,vsEnd-vsStart,f);
    }

    //
    // Move us after the variable
    //
    s = vsEnd;
  }

  //
  // If there is anything left after the last variable, print it.
  //
  if (s < end)
    fwrite(s,1,end-s,f);
}

static void Primitive_generator(FILE *f,const char *vsStart,const char *vsEnd,PrimParm *primParm)
{
  char *k1,*k2;
  const char *strcount;
  int count = 0;
  int i;

  //
  // We are assuming the variable call begins with this sequence
  //
  if (strncmp(vsStart,"$${",3) != 0)
    return;
  vsStart += 3;


  //
  // We are assuming the variable call ends with this sequence
  //
  if (vsEnd[-1] != '}')
    return;
  vsEnd--;

  //
  // Find first mandatory colon
  //
  k1 = strchr(vsStart,':');
  if (!k1) return;

  //
  // Find second optional colon
  //
  if (k1 != vsEnd) {
    k2 = strchr(k1+1,':');
    if (k2 >= vsEnd) k2 = 0;
  } else
    k2 = 0;

  strcount = PrimParm_nget(primParm,vsStart,k1-vsStart);
  if (strcount)
    sscanf(strcount,"%d",&count);

#if 0
  fprintf(f,"GENERATE [");
  fwrite(vsStart,1,k1-vsStart,f);
  fprintf(f,"] [");
  if (k2) {
    fwrite(k1+1,1,k2-k1-1,f);
    fprintf(f,"] [");
    fwrite(k2+1,1,vsEnd-1-k2,f);
  } else {
    fwrite(k1+1,1,vsEnd-k1-1,f);
  }

  fprintf(f,"]");
#endif

#if 1
  if (k2) {
    for (i = 0;i < count;i++) {
      if (i != 0) {
	Primitive_writeText(f,k1+1,k2,i,primParm);
      }
      Primitive_writeText(f,k2+1,vsEnd,i,primParm);
    }
  } else {
    for (i = 0;i < count;i++) {
      //fwrite(k1+1,1,vsEnd-k1-1,f);
      Primitive_writeText(f,k1+1,vsEnd,i,primParm);
    }
  }
#endif
}

void Primitive_write(FILE *f,const char *name,GCellSpec *gcs,PrimParm *primParm)
{
  Primitive *p = Primitive_lookup(name);

  if (!p || !p->p_body) return;

  fprintf(f,"//: /builtinBegin\n");
  fprintf(f,"module %s",gcs->gc_name);

  Primitive_writeText(f,p->p_nameEnd,p->p_end,-1,primParm);

  fprintf(f,"//: /builtinEnd\n\n");
}

void PrimParm_init(PrimParm *primParm)
{
  primParm->pp_size = 0;
}

char *PrimParm_get(PrimParm *primParm,const char *name)
{
  int i;

  for (i = 0;i < primParm->pp_size;i++) {
    if (strcmp(primParm->pp_specs[i].pps_name,name) == 0)
      return primParm->pp_specs[i].pps_value;
  }

  if (primParm->pp_size > (PARMSPEC_MAXPARMS-2)) {
    *primParm->pp_specs[primParm->pp_size].pps_value = 0;
    return primParm->pp_specs[primParm->pp_size].pps_value;
  }

  strcpy(primParm->pp_specs[primParm->pp_size].pps_name,name);
  *primParm->pp_specs[primParm->pp_size].pps_value = 0;
  return primParm->pp_specs[primParm->pp_size++].pps_value;
}

const char *PrimParm_nget(PrimParm *primParm,const char *name,int n)
{
  int i;

  for (i = 0;i < primParm->pp_size;i++) {
    if (strncmp(primParm->pp_specs[i].pps_name,name,n) == 0)
      return primParm->pp_specs[i].pps_value;
  }

  return "";
}

void PrimParm_set(PrimParm *primParm,const char *name,const char *fmt,...)
{
  va_list ap;

  va_start(ap,fmt);
  vsprintf(PrimParm_get(primParm,name),fmt,ap);
  va_end(ap);
}

void PrimParm_invSet(PrimParm *primParm,const char *name,int isInv)
{
  strcpy(PrimParm_get(primParm,name),isInv ? "~" : "");
}

void PrimParm_rangeSet(PrimParm *primParm,const char *name,int rangeMax)
{
  strcpy(PrimParm_get(primParm,name),bitrangeSpec(rangeMax));
}

void PrimParm_intSet(PrimParm *primParm,const char *name,int n)
{
  PrimParm_set(primParm, name, "%d", n);
}

