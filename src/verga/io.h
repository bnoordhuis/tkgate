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

    Last edit by hansen on Wed Sep 29 08:46:48 2004
****************************************************************************/
#ifndef __io_h
#define __io_h

extern int do_input_check;		/* Flag to indicate input is ready */

void vgio_echo(const char *fmt,...);
void vgio_comment(const char *fmt,...);
void vgio_printf(const char *fmt,...);
void waitForExit(void);
int get_data(void);
int input_ready(int doWait);
int get_line(char *s,int n);
void timer_event(int p);
void input_setup(void);

#endif
