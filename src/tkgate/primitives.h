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
#ifndef __primitives_h
#define __primitives_h

#define PARMSPEC_MAXPARMS	64	// Maximum number of parameters 

typedef struct Primitive_str {
  const char	*p_name;		// Name of this primitive built-in cell
  const char	*p_body;		// Full body of this primitive
  const char	*p_end;			// End of full body of this primitive
  const char	*p_nameStart;		// Pointer into body for start of module name
  const char	*p_nameEnd;		// Pointer into body after end of module name
  const char	*p_parmStart;		// Pointer into body for start of parameter list
  const char	*p_parmEnd;		// Pointer into body for end of parameter list
} Primitive;

typedef struct PrimParmSpec_str {
  char	pps_name[STRMAX];
  char	pps_value[STRMAX];
} PrimParmSpec;

typedef struct PrimParm_str {
  int			pp_size;
  PrimParmSpec		pp_specs[PARMSPEC_MAXPARMS];
} PrimParm;



void Primitive_write(FILE *f,const char *name,GCellSpec *gcs,PrimParm *primParm);

void PrimParm_init(PrimParm *primParm);
char *PrimParm_get(PrimParm *primParm,const char *name);
const char *PrimParm_nget(PrimParm *primParm,const char *name,int len);
void PrimParm_set(PrimParm *primParm,const char *name,const char *fmt,...);
void PrimParm_invSet(PrimParm *primParm,const char *name,int isInv);
void PrimParm_rangeSet(PrimParm *primParm,const char *name,int rangeMax);
void PrimParm_intSet(PrimParm *primParm,const char *name,int n);

#endif

