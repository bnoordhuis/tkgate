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
#ifndef __options_h
#define __options_h

/******************************************************************************
 *
 * Tcl Library - If TkGate can not find init.tcl, uncomment this line and set
 * it to the directory that contains the "init.tcl" file for your tcl
 * installation.
 *
 ******************************************************************************/
/* #define TCL_LIBRARY "/usr/lib/tcl8.4" */

/******************************************************************************
 *
 * Tk Library - If TkGate can not find the Tk library, uncomment this line and
 * set it to the directory that contains the Tk library.  This directory should
 * include files such as "tk.tcl", "button.tcl", etc.
 *
 ******************************************************************************/
/* #define TK_LIBRARY "/usr/lib/tk8.4" */


/******************************************************************************
 *
 * Compiler "CFLAG" options.  Need to rerun "./configure" to become effective.
 *
 ******************************************************************************/
#define TKGATE_CFLAGS -O2 -g


/******************************************************************************
 *
 * Set this to '1' if you get an error about Tk_ConfigureWidget
 * when running tkgate.
 *
 ******************************************************************************/
#define TKGATE_BROKENCONFIGWIDGET 0


/******************************************************************************
 *
 *  Maximum number of delay parameters for a gate
 *
 ******************************************************************************/
#define MAXDELAYS               16

/******************************************************************************
 *
 *  Maximum number of parameters (other than delay) for a module
 *
 ******************************************************************************/
#define MAXPARMS                16

/******************************************************************************
 *
 * Maximum number of arguments in an error message
 *
 ******************************************************************************/
#define MAXARGS                 16

/******************************************************************************
 *
 * Bus width symbol adjustment.  These values are used to tweek the position
 * of the bus width "slash" on vertical and horizontal busses due to variations
 * in X implementations.
 *
 ******************************************************************************/
#define TKGATE_BUSW_VERT 0      /* x tweek for vertical busses */
#define TKGATE_BUSW_HORZ 0      /* y tweek for horizontal busses */

/******************************************************************************
 *
 * Wire tweeks.  These tweeks are used to adjust the length of wire endpoints
 * to compensate for variations in X implementations.
 *
 ******************************************************************************/
#define TKGATE_WIRETWEEK_RIGHT          1
#define TKGATE_WIRETWEEK_LEFT           0
#define TKGATE_WIRETWEEK_TOP            0
#define TKGATE_WIRETWEEK_BOTTOM         1

/******************************************************************************
 *
 * Default font encoding for display.
 *
 ******************************************************************************/
#define TKGATE_DEFAULT_FONT_CODE      "iso8859-1"

#endif
