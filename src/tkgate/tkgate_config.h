/****************************************************************************
    Copyright (C) 1987-2009 by Jeffery P. Hansen

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
/*
 *    Declarations for circuit representation structures.
 */
#ifndef __tkgate_config_h
#define __tkgate_config_h

/*
 * Initial tcl script to execute on start up
 */
#define TCLSCRIPT "scripts/tkgate.tcl"

/*
 * Various limitations
 */
#define STRMAX		1024		/* Maximum length of a string */
#define MINSIZE		40		/* Minumum size for a logic block */
#define NUMORIENTS	4		/* Number of orientations */
#define SNAPSIZE	6		/* */
#define DEFAULT_PADMAX	16		/* Maximum number of "pads" on a gate */
#define MAXKBINDS	16		/* Maximum number of key bindings for creating a gate */ 
#define MAKEENTRYMAX	128		/* Max entries on a make page */
#define NETALIASLIST_STEPSIZE 5		/* Allocation increment for net alias lists */
#define TKGATE_STUBLEN	15		/* Default length of wires on new gates */
#define MAXMODS		128		/* Maximum number of modules in text buffer */
#define MODULE_PATH_MAX	2048		/* Maximum module depth */


/*
  Distances for hit testing.  The values should be the square of
  the hit distance.
 */
#define MAXPORTRANGE	(12*12)		/* Maximum range for port hit */
#define MAXWIRERANGE	(3*3)		/* Maximum range for wire hit */
#define GATERANGE 	(15*15+1)	/* Maximum range for gate hit */
#define SELMODETHRESH	(15*15)		/* Drag distance to activate select mode */

/*
 * Default prefix 
 */
#define TKGATE_DEFAULT_PREFIX "_GG"	/* Prefix for tkgate primitive cells */

/*
 * Name of default technology
 */
#define TKGATE_DEFAULT_TECH "default"

/*
 * Debug options
 */
#define LOCALE_DEBUG	0		/* Debug of locale handling */

#endif

