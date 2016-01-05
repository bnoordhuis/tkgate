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
#ifndef __functions_h
#define __functions_h

#define NONSIM 0x1
#define SIM 0x2
#define EXMODE 0x80000000

#define NOPROC 0x4
#define PROCON 0x8
#define PROCOFF 0x10

#define CUTMODEBIT 0x20
#define MOVEMODEBIT 0x40
#define INVERTMODEBIT 0x80
#define DELETEMODEBIT 0x100
#define PAPERMODEBIT 0x200
#define NAMEMODEBIT 0x400
#define SETSIZEBIT 0x800

#define	MS1 0x3		    /* sim/no sim bits */
#define	MS2 0x1c	    /* Proc on/off bits */
#define MS3 0xfe0	    /* All mode bits */

#endif
