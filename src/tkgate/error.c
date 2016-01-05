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

    Last edit by hansen on Thu Feb 12 14:48:49 2009
****************************************************************************/
/*
* Error package for editor.
*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tkgate.h"

#define ERRORHEADLINES 2

#define ERROR_DEBUG 1

int exit_flag = 0;		/* Flag to see if we are currently exiting */

/*
    Set the position of an error
*/
void SetErrorPosition(int x,int y)
{
  ob_touch(TkGate.errl);
  TkGate.ErrorMarkTimeout = 2;
  TkGate.errl->ErrorXPos = x;
  TkGate.errl->ErrorYPos = y;
}

void DrawErrorPositionMark()
{
  int x = TkGate.errl->ErrorXPos;
  int y = TkGate.errl->ErrorYPos;

  line(x-20,y-20,x+20,y+20);
  line(x-20,y+20,x+20,y-20);
}

void ClearErrorMark()
{
  if (TkGate.ErrorMarkTimeout == 1) {
    DrawErrorPositionMark();
    TkGate.ErrorMarkTimeout = 0;
  }
}


ErrorList *new_ErrorList()
{
  ErrorList *el = (ErrorList*) ob_malloc(sizeof(ErrorList),"ErrorList");
  el->errlist = 0;
  el->curerr = 0;
  el->errors = 0;
  el->ErrorCount = 0;
  el->ErrorXPos = 0;
  el->ErrorYPos = 0;
  el->el_esList = 0;
  return el;
}

/*****************************************************************************
 *
 * If 'line' is part of a script, return the object for that script
 *
 *****************************************************************************/
static GEScript *ErrorList_getScript(ErrorList *el, int line)
{
  GEScript *es;

  for (es = el->el_esList;es;es = es->es_next) {
    if (line >= es->es_beginLine && (es->es_endLine == 0 || line <= es->es_endLine))
      return es;
  }

  return 0;
}


static GError *new_GError(const char *errText)
{
  GError *e = (GError*) ob_malloc(sizeof(GError),"GError");

  e->e_id = 0;
  e->e_level = 0;
  e->e_fileName = 0;
  e->e_fileLine = 0;
  e->e_modName = 0;
  e->e_modLine = 0;
  e->e_instName = 0;
  e->e_message = 0;
  e->e_inst = 0;
  e->e_portName = 0;
  e->e_next = e->e_last = 0;

  if (errText) {
    char etype[STRMAX],fileName[STRMAX],modName[STRMAX],instName[STRMAX],portName[STRMAX],msg[STRMAX];
    int fileLine, modLine;
    GEScript *es;


    if (sscanf(errText,"%s file %s %d %s %d %s %s : %[^\n]",
	       etype,fileName,&fileLine,modName,&modLine,instName,portName,msg) == 8) {

      es = ErrorList_getScript(TkGate.errl,fileLine);

      Error_decode(msg);

      if (strcmp(etype,"warning") == 0) {
	e->e_level = 0;
      } else
	e->e_level = 1;

      e->e_fileName = ob_strdup(fileName);
      e->e_fileLine = fileLine;
      e->e_modName = ob_strdup(modName);
      e->e_modLine = modLine;
      e->e_instName = (*instName != '-') ? ob_strdup(instName) : 0;
      e->e_portName = (*portName != '-') ? ob_strdup(portName) : 0;

      if (es) {
	char msg2[STRMAX];

	sprintf(msg2,"script <%s> - line %d, %s",es->es_name, fileLine - es->es_beginLine + 1, msg);

	e->e_instName = ob_strdup(es->es_name);
	e->e_message = ob_strdup(msg2);
      } else
	e->e_message = ob_strdup(msg);
    }
  }

  return e;
}

static void delete_GError(GError *e)
{
  if (e->e_fileName) ob_free(e->e_fileName);
  if (e->e_modName) ob_free(e->e_modName);
  if (e->e_instName) ob_free(e->e_instName);
  if (e->e_portName) ob_free(e->e_portName);
  if (e->e_message) ob_free(e->e_message);
  ob_free(e);
}

static void Error_add(GError *e)
{
  char *prefix = (e->e_level == 0) ? "Warning, " : "";
  GModuleDef *m;
  char msgbuf[STRMAX];

  if (TkGate.errl->errors) {
    DoTcl("ErrBox::unpost");
  }

  if (!TkGate.errl->curerr) {
    DoTcl("ErrBox::post");
    ob_touch(TkGate.errl);
    TkGate.errl->ErrorCount = 0;
    TkGate.errl->errlist = TkGate.errl->curerr = e;
    ob_touch(e);
    ob_touch(TkGate.errl->errlist);

    e->e_id = TkGate.errl->ErrorCount = 1;
    TkGate.errl->errlist->e_last = 0;
    TkGate.errl->errlist->e_next = 0;
  } else {
    ob_touch(e);
    ob_touch(TkGate.errl);
    ob_touch(TkGate.errl->curerr);

    e->e_id = TkGate.errl->ErrorCount++;
    TkGate.errl->curerr->e_next = e;
    TkGate.errl->curerr->e_next->e_last = TkGate.errl->curerr;
    TkGate.errl->curerr = TkGate.errl->curerr->e_next;
    TkGate.errl->curerr->e_next = 0;
  }

  if (e->e_modName)
    m = env_findModule(e->e_modName);
  else
    m = 0;

  if (m && m->m_type == MT_TEXTHDL)
    sprintf(msgbuf,"%s, line %d: %s%s",e->e_modName,e->e_modLine,prefix,e->e_message);
  else
    sprintf(msgbuf,"%s: %s%s",e->e_modName,prefix,e->e_message);

  DoTclL("ErrBox::addError",msgbuf,NULL);
}

void Error_report(const char *errText)
{
  GError *e = new_GError(errText);
  Error_add(e);
}

/*****************************************************************************
 *
 * Translate list format error list into an array for easy access.
 *
 *****************************************************************************/
void Error_close()
{
  GError *e;
  int i = 0;

  ob_touch(TkGate.errl);
  TkGate.errl->errors = (GError**) ob_malloc(sizeof(GError*)*TkGate.errl->ErrorCount,"GError*[]");
  for (e = TkGate.errl->errlist;e;e = e->e_next, i++) {
    TkGate.errl->errors[i] = e;
  }
}

/*****************************************************************************
 *
 * Delete all messages from the error list
 *
 *****************************************************************************/
void Error_purge()
{
  GError *e;

  ob_touch(TkGate.errl);

  ClearErrorMark();
  while (TkGate.errl->errlist) {
    e = TkGate.errl->errlist;
    TkGate.errl->errlist = TkGate.errl->errlist->e_next;
    delete_GError(e);
  }

  ob_free(TkGate.errl->errors);
  TkGate.errl->errors = 0;
  TkGate.errl->curerr = TkGate.errl->errlist = 0;

  TkGate.ErrorMarkTimeout = 0;
  FlagRedraw();
}

/*****************************************************************************
 *
 * Open the specified error in the current editstate.
 *
 * Parameter:
 *      e		Error object to open
 *      es		Current edit state
 *
 * Returns:		New edit state
 *
 *****************************************************************************/
EditState *Error_open(GError *e,EditState *es)
{
  GModuleDef *m;
  int x,y;

  if (!e) return es;				/* No error object specified */

  m = env_findModule(e->e_modName);		/* Find module the error ocurrs in */
  if (!m) {
    message(0,msgLookup("err.noerr"));
    return es;
  }


  if (tkgate_currentMode() == MM_SIMULATE) {
    SimInterface_navigateToModule(&es,e->e_modName);
  } else {
    /*
     * Navigate to the appropriate module.
     */
    if (es->env != m) {
      while (es)
	editstate_pop(&es);
      editstate_push(&es,m,0);
      editstate_setCurrent(es);
      ClearErrorMark();
      net_unselect(0);
      cpath_reshow();
      SetModified(MF_MODULE);
    }
  }

  /*
   * For errors in HDL modules we just go to the offending line in the selected module.
   */
  if (m->m_type == MT_TEXTHDL) {
    SynchronizeInterface();
    FlagRedraw();

    DoTcl("HdlEditor::highlightLine %d",e->e_modLine);
    return es;
  }

  x = TkGate.width/2;
  y = TkGate.height/2;

  if (e->e_instName) {
    /*
     * Error is on a specific instance, or on a port of that instance.
     */
    GCElement *g = GModuleDef_findGate(m,e->e_instName);
    if (!g) return es;

    x = g->xpos;
    y = g->ypos;
    if (g->typeinfo->Code == GC_BLOCK) {
      x += g->u.block.gwidth/2;
      y += g->u.block.gheight/2 + 10;
    }

    if (e->e_portName) {
      GWire *w = GCElement_getPort(g,e->e_portName);
      if (w) {
	x = w->nodes->x;
	y = w->nodes->y;
      }
    }
  } else if (e->e_portName) {
    /*
     * If no instance was specified, then e_portName is actually the name of a net.
     */
    GNet *net;
    char *localName = strrchr(e->e_portName,'.');
    if (!localName) localName = e->e_portName; else localName++;

    net = GModuleDef_findNet(m,localName);
    if (net) {
      x = net->n_driver->nodes->x;
      y = net->n_driver->nodes->y;
    }
  } else {
    /* don't know how to navigate to error */
    return es;
  }

  TkGate_setOrigin(TkGate.width/2 - x,TkGate.height/2 - y);
  ob_touch(es);
  editstate_saveOrig(es);
  editstate_setCurrent(es);
  SetErrorPosition(x,y);

  SynchronizeInterface();
  FlagRedraw();

  return es;
}

/*****************************************************************************
 *
 * Generate pointless messages if enabled.
 *
 * Parameters:
 *      m		Argument for pointless messages
 *
 * 
 *
 *****************************************************************************/
void badermessage(const char *m)
{
  int which;

  which = random() & 63;
  if (which > 7) return;

  /*
   * Be mean to Miles Bader.
   */
  if (TkGate.baderp) {
    switch (which) {
    case 0 :
      message(1,msgLookup("miles.msg1"),article(m),m);
      return;
    case 1 :
      message(1,msgLookup("miles.msg2"),article(m),m);
      return;
    case 2 :
      message(1,msgLookup("miles.msg3"),article(m),m);
      return;
    case 3 :
      message(1,msgLookup("miles.msg4"),article(m),m);
      return;
    case 4 :
      message(1,msgLookup("miles.msg5"),article(m),m);
      return;
    case 5 :
      message(1,msgLookup("miles.msg6"),article(m),m);
      return;
    case 6 :
      message(1,msgLookup("miles.msg7"));
      return;
    case 7 :
      message(1,msgLookup("miles.msg8a"));
      message(1,msgLookup("miles.msg8b"));
      return;
    }
    return;
  }

  /*
   * In bat mode liven things up a little
   */
  if (TkGate.batp) {
    switch (which) {
    case 0 :
      message(1,"** BIFF **");
      return;
    case 1 :
      message(1,"** POW **");
      return;
    case 2 :
      message(1,"** CRASH **");
      return;
    case 3 :
      message(1,"** OOOF **");
      return;
    case 4 :
      message(1,"** WHAM **");
      return;
    case 5 :
      message(1,"** CRUNCH **");
      return;
    case 6 :
      message(1,"** BANG **");
      return;
    case 7 :
      message(1,"** SOCK **");
      return;
    default :
      break;
    }
    return;
  }

  /*
   * Generate a star trek message.
   */
  if (TkGate.startrekp) {
    switch (which) {
    case 0 :
      message(1,"Beam me up Scotty!");
      return;
    case 1 :
      message(1,"Engage!");
      return;
    case 2 :
      message(1,"He's dead Jim!");
      return;
    case 3 :
      message(1,"Earl Grey Tea Hot!");
      return;
    case 4 :
      message(1,"Every sixteenth hour, I turn into a liquid!");
      return;
    case 5 :
      message(1,"Ugly bags of mostly water.");
      return;
    case 6 :
      message(1,"Microbrain! Growl for me, let me know you still care!");
      return;
    case 7 :
      message(1,"I'm a doctor, not an engineer!");
      return;
    default :
      break;
    }
    return;
  }
}

char *lookupSignalName(int s)
{
  static char buf[16];

  switch (s) {
  case SIGHUP : return "SIGHUP"; 
  case SIGQUIT : return "SIGQUIT"; 
  case SIGILL : return "SIGILL"; 
  case SIGTRAP : return "SIGTRAP"; 
  case SIGFPE : return "SIGFPE"; 
  case SIGBUS : return "SIGBUS"; 
  case SIGSEGV : return "SIGEGV"; 
#ifdef SIGEMT
  case SIGEMT: return "SIGEMT"; 
#endif
#ifdef SIGSYS
  case SIGSYS : return "SIGSYS"; 
#endif
  default:
    sprintf(buf,"UNKNOWN-%d",s);
    return buf;
  }
}


/*****************************************************************************
 *
 * Make a last ditch effort to save the users circuit in a panic file.
 *
 * Parameters:
 *      s 			Signal number that caused panic
 *
 * This function is called when an internal problem causes an unrecoverable
 * signal such as a segmentation fault to occur.  When called, an attempt will
 * be made to save the current circuit to a file named "PANIC.v".  We try to
 * use only the bare minimum of available functions to avoid causing a second
 * panic while doing the save. For this reason, we deliberately avoid using
 * localized messages and have the English messages hard coded for this function.
 * If we do get a secondary call to panicSave(), then we give up.
 *
 *****************************************************************************/
void panicSave(int s)
{
  static int in_panic_save = 0;

  if (exit_flag) {
    /*
     * We got a segfault, etc. while trying to exit TkGate.  Just go ahead and
     * exit leaving a message to remind me about the problem.  The segfault seems
     * to occur intermittently deep in the Tcl internals and I do not know exactly
     * what is causing it.
     */

#if 0
    printf("cheated death again!\n");
#endif
    exit(0);
  }


  ob_mode(0);			/* Turn off undo/redo object management */

  /*
   * If we get a panic while saving a panic file we will just have to give up.
   */
  if (in_panic_save) {
    printf("**** Signal %s received during panic save.  Sorry...\n",lookupSignalName(s));
    undoSignals();
    abort();
  }
  in_panic_save = 1;


  /*
   * Print signal number if panic was caused by a signal.
   */
  if (s) {
    printf("**** Signal %s received ****\n",lookupSignalName(s));
  }


  /*
   * Tell the user the bad news.
   */
  if (TkGate.baderp) {
    printf("Look what you did *now*, Miles!\n");
    printf("Even though this is your fault, I'll try to save in 'PANIC.v' anyway.\n");
  } else if (TkGate.startrekp) {
    printf("Warp core breach in progress!  All hands abandon ship!\n");
    printf("Launching escape pod 'PANIC.v'.\n");
  } else {
    printf("TkGate has encountered an unrecoverable error and must exit.  Before doing this\n");
    printf("we will make one last attempt to save your circuit in a panic file.  If you have\n");
    printf("checkpointing enabled, you can also try using your checkpoint file as a backup.\n\n");
    printf("Attempting to save circuit in file 'PANIC.v'...\n");
  }

  VerilogWriteModules("PANIC.v",VSO_NOHDLCHECK);

  if (TkGate.baderp) {
    printf("Looks like I bailed you out once again Miles.\n");
  } else if (TkGate.startrekp) {
    printf("Escape pod is away.\n");
  } else {
    printf("Panic save completed.\n");
  }

  undoSignals();
  abort();
}

/*****************************************************************************
 *
 * Test a pointer to see if it is an obviously bad address.
 *
 *****************************************************************************/
int badaddr(void *S)
{
  return ((unsigned)S & 0x1) != 0;
}

/*****************************************************************************
 *
 * Change the editstate/current module to a specified path.
 *
 *****************************************************************************/
void Error_navigateToModule(EditState **es,const char *path)
{
  char buf[STRMAX],*T,*N;
  GModuleDef *M;

  while (*es) editstate_pop(es);

  M = TkGate.circuit->root_mod;
  strcpy(buf,path);
  editstate_push(es,M,0);
  for (T = strtok(buf,".");T;T = N) {
    N = strtok(0,".");
    if (N) {
      GCElement *g = (GCElement*) SHash_find(M->m_gates,T);
      if (g && GCElement_isModule(g)) {
	M = env_findModule(g->u.block.moduleName);
	editstate_push(es,M,g);
      }
    }
  }
  cpath_reshow();
}

/*****************************************************************************
 *
 * Decode an error message.
 *
 * Parameters:
 *      emsg			Error message to be decode.
 *
 *****************************************************************************/
void Error_decode(char *emsg)
{
  char argbuf[STRMAX];
  char name[STRMAX];
  char *args[MAXARGS];
  char *p;
  int n = 0;
  int i;

  /*
   * If message does not start with a '`', then don't do any decoding.
   */
  if (*emsg != '`')
    return;


  strcpy(argbuf,emsg+1);

  /*
   * Break the string in argbuf into arguments.  Adjust args[] to
   * point to each argument and insert null characters between arguments.
   */
  for (p = argbuf;*p;p++) {
    if (isgraph(*p)) {
      args[n++] = p;
      if (*p == '"') {
	p++;
	while (*p && *p != '"')
	  if (*p == '\\') {
	    p++;
	    if (*p) p++;
	  } else
	    p++;
	if (*p) p++;
      } else {
	while (isgraph(*p)) p++;
      }
      if (!*p) break;
      *p = 0;
    }
  }

  /*
   * Unmarshall any quoted strings
   */
  for (i = 0;i < n;i++) {
    if (*args[i] != '"') continue;

    args[i]++;
    for (p = args[i];*p && *p != '"';p++)
      if (*p == '\\')
	memmove(p,p+1,strlen(p));
    *p = 0;
  }

  if (n == 0) {
    strcpy(emsg,"<null-message>");
    return;
  }

  /*
   * Contruct the error message here.  We use a kludgy sprintf with all
   * of the possible arguments even if we don't use most of them.
   */
  sprintf(name,"verga.err.%s",args[0]);
  sprintf(emsg,msgLookup(name),args[1],args[2],args[3],args[4],args[5]
	  ,args[6],args[7],args[8],args[9],args[10],args[11],args[12]
	  ,args[13],args[14],args[15]);
#if MAXARGS != 16
#error Expecting MAXARGS to be 16 -- Fix code here if changing MAXARGS
#endif
}

void Error_scriptBegin(const char *name,int line)
{
  GEScript *es = (GEScript*) ob_malloc(sizeof(GEScript),"GEScript");

  es->es_name = ob_strdup(name);
  es->es_beginLine = line;
  es->es_endLine = 0;

  ob_touch(TkGate.errl);
  es->es_next = TkGate.errl->el_esList;
  TkGate.errl->el_esList = es;
}

void Error_scriptEnd(const char *name,int line)
{
  if (!TkGate.errl->el_esList) {
    logError(ERL_ERROR,"got embedded script end <%s> but never got a begin.",name);
    return;
  }
  if (strcmp(TkGate.errl->el_esList->es_name,name) != 0) {
    logError(ERL_ERROR,"embedded script end <%s> does not match script begin <%s>.",name,TkGate.errl->el_esList->es_name);
    return;
  }
  ob_touch(TkGate.errl->el_esList);
  TkGate.errl->el_esList->es_endLine = line; 
}

