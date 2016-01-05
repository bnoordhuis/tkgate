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
#ifndef __message_h
#define __message_h

#define MC_ERRBOX	0x1		/* Display message in an error box */	
#define MC_STATUS	0x2		/* Display message on status line */
#define MC_MSGLOG	0x4		/* Display mesage in log */
#define MC_SILENT	0x1000		/* Do MSGLOG silently (do not auto open) */
#define MC_WARNING	0x2000		/* Use warning coloring */
#define MC_URGENT	0x4000		/* Use error or urgent coloring */
#endif


