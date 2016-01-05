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

    Last edit by hansen on Sat Jan 17 00:30:11 2009
****************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdarg.h>
#include "tkgate.h"

int istruevalue(const char *s);

/*****************************************************************************
 *
 * 
 *
 *****************************************************************************/
static struct {
  GCElement *g;			/* Gate on which hyperlink is found */
  char *link;			/* Text of the link */
  int isConfirmed;		/* hyperlink is confirmed (mouse up seen) */	
} pending_jump = {0,0,0};

static void setPendingLink(GCElement *g,const char *link)
{
  pending_jump.g = g;
  pending_jump.isConfirmed = 0;
  if (pending_jump.link) free(pending_jump.link);
  pending_jump.link = link ? strdup(link) : 0;
}

/*****************************************************************************
 *
 * Replace certain special directory identifiers with the actual directory.
 *
 *****************************************************************************/
void expandSpecialDirs(char *file)
{
  const char *home    = Tcl_GetVar(TkGate.tcl,"tkg_gateHome",TCL_GLOBAL_ONLY);
  char *curFileDir = CurrentFile_getDir(TkGate.circuit->currentFile);
  char *exampleDir = tkgate_exampledir;
  char *tutorialDir = tkgate_tutorialdir;

  strreplace(file,"@T",tutorialDir,1);
  strreplace(file,"@E",exampleDir,1);
  strreplace(file,"@H",home,1);
  strreplace(file,"@C",curFileDir,1);
  strreplace(file,"@",home,1);
}

int Hyperlink_inSimulator()
{
  if (tkgate_currentMode() == MM_SIMULATE) {
    const char *result;
    DoTcl("yesno {%s}",msgLookup("sim.hyperlinkstop"));
    result = Tcl_GetStringResult(TkGate.tcl);
    if (!istruevalue(result)) {
      Hyperlink_cancel();
      return 1;
    }

    DoTcl("Action::editMode");
  }
  return 0;
}

/******************************************************************************
 *
 * Invoke a browser handled url.
 *
 ******************************************************************************/
void Html_invoke_browser(char *protocol,char * file)
{
  char url[STRMAX];
  char command[STRMAX];
  char *p;
    
  sprintf(url,"%s:%s",protocol,file);
  sprintf(command,"%s",TkGate.browserCommand);

  p = strchr(command,'%');
  if (strncmp(p,"%s",2) == 0) {
    memmove(p+strlen(url),p+2,strlen(p)-1);
    memmove(p,url,strlen(url));
  }

  if (!fork()) {
    close(0);
    close(1);
    close(2);
    fopen("/dev/null","r");
    fopen("/dev/null","w");
    fopen("/dev/null","w");
    system(command);
    exit(0);
  }
}

/******************************************************************************
 *
 * Invoke an email handled url.
 *
 ******************************************************************************/
void Html_invoke_email(char *protocol,char * file)
{
  char url[STRMAX];
  char command[STRMAX];
  char *p;
    
  sprintf(url,"%s:%s",protocol,file);
  sprintf(command,"%s",TkGate.emailCommand);

  p = strchr(command,'%');
  if (strncmp(p,"%s",2) == 0) {
    memmove(p+strlen(url),p+2,strlen(p)-1);
    memmove(p,url,strlen(url));
  }

  if (!fork()) {
    close(0);
    close(1);
    close(2);
    fopen("/dev/null","r");
    fopen("/dev/null","w");
    fopen("/dev/null","w");
    system(command);
    exit(0);
  }
}

/******************************************************************************
 *
 * Invoke a "file:" protocol url.
 *
 ******************************************************************************/
void Html_invoke_file(char * file)
{
  char *label = 0;

  /*
   * If this appears to be a relative link, prepend the current file directory marker.
   */
  if (*file != '/' && *file != '@' && *file != '#') {
    memmove(file+3,file,strlen(file)+1);
    memmove(file,"@C/",3);
  }
  expandSpecialDirs(file);

  /*
   * See if there is a label
   */
  label = strchr(file,'#');
  if (label) *label++ = 0;

  /*
   * If there is still a file name, load the file if it is a .v file, or display
   * it in a viewer if it is any other type of file.  
   */
  if (*file) {
    char *ext = strrchr(file,'.');
    if (!ext) ext = "";

    if (isVerilogFile(file)) {
      if (Hyperlink_inSimulator()) return;
      DoTclL("Action::loadNamedFile",file,NULL);
    } else if (strcasecmp(ext,".html") == 0 || strcasecmp(ext,".htm") == 0) {
      Html_invoke_browser("file",file);
    } else {
      char *title;

      title = strrchr(file,'/');
      if (title) 
	title++;
      else
	title = file;

      DoTclL("viewFile",title,file,NULL);
    }
  }


  if (label && *label) {
    if (Hyperlink_inSimulator()) return;
    if (*label == '/') {
      DoTcl("gat_openBox %s",label+1);
    } else {
      ob_touch(TkGate.circuit);
      if (editstate_checkPath(&TkGate.circuit->es,label))
	editstate_setPath(&TkGate.circuit->es,label);
      else
	message(0,msgLookup("err.badfind"),label);
    }
  }
}

/******************************************************************************
 *
 * Invoke a "action:" protocol url.
 *
 ******************************************************************************/
void Html_invoke_action(char * file)
{
  static char **proc_table = 0;
  static int N = 0;
  int i;

  if (Hyperlink_inSimulator()) return;

  if (!proc_table) {
    char *temp_s,*t;
    char **temp_a;

    DoTcl("info procs Action::*");
    temp_s = strdup(Tcl_GetStringResult(TkGate.tcl));
    temp_a = (char**)malloc(sizeof(char*)*strlen(temp_s));	// Probably way more than we need

    for (t = strtok(temp_s," \n\t");t;t = strtok(0," \n\t"))
      temp_a[N++] = t;

    proc_table = (char**)malloc(sizeof(char*)*N);
    for (i = 0;i < N;i++) {
      t = strstr(temp_a[i],"Action::");
      assert(t);						// Everything should be an Action:: proc.
      proc_table[i] = t + 8;
    }

    free(temp_a);
  }

  for (i = 0;i < N;i++)
    if (strcmp(file,proc_table[i]) == 0) {
      DoTcl("Action::%s",proc_table[i]);
      break;
    }
}

/*****************************************************************************
 *
 * Parse and invoke a hyperlink
 *
 *****************************************************************************/
void dohyperlink(const char *url)
{
  char protocol[STRMAX];
  char file[STRMAX];

  if (sscanf(url," %[^:]:%[^\n ]",protocol,file) == 2) {
  } else if (sscanf(url," %[^\n ]",file) == 1) {
    strcpy(protocol,"file");
  } else {
    return;			/* Bad url */
  }

  if (strcasecmp(protocol,"file") == 0) {
    Html_invoke_file(file);
  } else if (strcasecmp(protocol,"action") == 0) {
    Html_invoke_action(file);
  } else if (strcasecmp(protocol,"http") == 0) {
    Html_invoke_browser(protocol,file);
  } else if (strcasecmp(protocol,"mailto") == 0) {
    Html_invoke_email(protocol,file);
  }
#if 0
  ob_touch(TkGate.circuit);
  TkGate.circuit->mode = MODE_MOVENULL;
#endif
}

void Hyperlink_activatePending(ClientData data)
{
  if (pending_jump.isConfirmed) {
    ob_begin_framef("Hyperlink",0);
    dohyperlink(pending_jump.link);
    Hyperlink_cancel();
    ob_end_frame();
  }
}

int Hyperlink_selectAt(int x,int y)
{
  GCElement *g = gate_hit(TkGate.circuit->es->env,x,y);
  const char *link;

  setPendingLink(0,0);

  if (!g || g->typeinfo->Code != GC_COMMENT) {
    return 0;
  }

  link =  Comment_getHyperlink(g, x, y);
  if (!link) {
    return 0;
  }

  setPendingLink(g,link);

  return 1;
}

const char *Hyperlink_getAt(int x,int y)
{
  GCElement *g = gate_hit(TkGate.circuit->es->env,x,y);

  if (!g || g->typeinfo->Code != GC_COMMENT) {
    return 0;
  }

  return Comment_getHyperlink(g,x,y);
}

int Hyperlink_confirmAt(int x,int y)
{
  GCElement *g;
  const char *link;
  int failed = 0;

  if (!pending_jump.link) return 0;

  if (!pending_jump.link) {
    failed = 1;
  } else if (!(g = gate_hit(TkGate.circuit->es->env,x,y))) {
    failed = 1;
  } else if (g->typeinfo->Code != GC_COMMENT) {
    failed = 1;
  } else if (g != pending_jump.g) {
    failed = 1;
  } else if (!(link =  Comment_getHyperlink(g,TkGate.ed->tx,TkGate.ed->ty))) {
    failed = 1;
  } else if (strcmp(link,pending_jump.link) != 0) {
    failed = 1;
  } else {
    pending_jump.isConfirmed = 1;
  }

  if (failed) {
    setPendingLink(0,0);
    return 0;
  } else {
    Tk_DoWhenIdle(Hyperlink_activatePending, 0);
    return 1;
  }
}


void Hyperlink_cancel()
{
  setPendingLink(0,0);
}

int Hyperlink_isPending()
{
  return (pending_jump.link != 0);
}
