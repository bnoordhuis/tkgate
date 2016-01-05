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

    Last edit by hansen on Tue Jan 27 23:21:37 2009
****************************************************************************/
#include <stdlib.h>
#include "tcl.h"
#include "tk.h"

#define STRMAX 1024

void findTkGateHome(char *homeDir);
int Tkgate_Init(Tcl_Interp *tcl);

/*****************************************************************************
 *
 * This contains the inital setup for tcl/tk.
 *
 *****************************************************************************/
int Tcl_AppInit(Tcl_Interp *tcl)
{
  char buf[STRMAX];
  int r;

  r = Tcl_Init(tcl);
  if (r == TCL_ERROR) {
    fprintf(stderr,"Tcl_Init Error in tkgate:\n%s\n",tcl->result);
    fprintf(stderr,"Perhaps you could try setting the environment variable TCL_LIBRARY\n");
    fprintf(stderr,"to the directory in which init.tcl can be found.  You can also\n");
    fprintf(stderr,"set TCL_LIBRARY in options.h.\n");
    exit(1);
  }

  Tkgate_Init(tcl);

  return TCL_OK;
}

/*****************************************************************************
 *
 * This is the main() for tkgate.  We do some basic setup, parsing of command-
 * line options, then call Tk_Main() to start the application.  Tk_Main() will
 * call Tcl_AppInit to do tcl-related initialization then start the tcl script
 * interpreter to start the tkgate application script.
 *
 *****************************************************************************/
int main(int argc,char *argv[])
{
  char tkgate_startup_script[STRMAX];
  char homeDir[STRMAX];
  char **tk_argv;
  int i,tk_argc;

  /*
   * Figure out which directory we should use as the tkgate home.
   */
  findTkGateHome(homeDir);


  /*
   * If the tcl/tk library file path environment variables are not set, use
   * the ones that we found when tkgate was compiled.
   */
#ifdef TCL_LIBRARY
  if (!getenv("TCL_LIBRARY")) putenv("TCL_LIBRARY=" TCL_LIBRARY);
#endif
#ifdef TK_LIBRARY
  if (!getenv("TK_LIBRARY")) putenv("TK_LIBRARY=" TK_LIBRARY);
#endif

  /*
   * Build a mock command line with the main tcl script for tkgate and start up the tcl/tk
   * interpeter with that script.  All of the rest of the initialization will be controlled
   * by the tcl script.
   */
  tk_argc = 0;
  tk_argv = (char**) malloc(sizeof(char*)*(argc+4));
  tk_argv[tk_argc++] = argv[0];
  sprintf(tkgate_startup_script,"%s/scripts/tkgate.tcl",homeDir);
  tk_argv[tk_argc++] = tkgate_startup_script;
  tk_argv[tk_argc++] = "--";
  tk_argv[tk_argc++] = "-H";
  tk_argv[tk_argc++] = homeDir;
  for (i = 1;i < argc;i++)
    tk_argv[tk_argc++] = argv[i];

  /*
   * Here we go, off to tcl/tk land.
   */
  Tk_Main(tk_argc,tk_argv,Tcl_AppInit);

  return 0;
}
