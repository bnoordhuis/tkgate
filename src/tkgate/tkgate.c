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

    Last edit by hansen on Wed Mar 18 04:23:18 2009
****************************************************************************/
#include "tkgate.h"
#include <dirent.h>


void MSS_init();			/* Module symbol table initialization */

static char *tkgateHomeDir = 0;

int verify_only = 0;

extern int flashCPath;
extern int ob_max_undo;
extern int security_exec_level;

int do_panic_save = 1;
int want_exit = 0;
int is_verbose = 0;

static char *print_options = 0;
static char *print_file = 0;
static char *print_printer = 0;

int debugContinuousVerify = 0;
int debugSimInterface = 0;
int sync_Xserver = 0;

int doBackupOnSave = 0;
int doSafeSave = 0;
int wantCheckpoint = 0;
int checkpointenabled = 0;

int blockDropConnect = 0;

/*****************************************************************************
 *
 * Table of messages loaded from the locale-specific messages file.
 *
 *****************************************************************************/
SHash *message_table = 0;

/*
 * Global variables used in main/Tk_AppInit
 */
static  const char **loadList = 0;
static  int numLoads = 0;
static  const char **libLoadList = 0;
static  int numLibLoads = 0;
static  int start_sim = 0;
static  int do_splash = 1;
static  char *sim_script = 0;
static  int debug = 0;
extern int guiActive;

int quietMode = 0;

char *tkgate_exampledir = 0;
char *tkgate_tutorialdir = 0;

/*****************************************************************************
 *
 * Catch a ctrl-c interrupt and do something about it so we can shut down
 * cleanly.
 *
 *****************************************************************************/
static void catchInterrupt(int s)
{
  FlagRedraw();
  want_exit = 1;
}

/*****************************************************************************
 *
 * Test to see if 'path' is a valid tkgate home directory.  We do this by 
 * trying to open some files we expect to be there and returning non-zero
 * if we find all of the expected files.  Returns 0 if one or more expected
 * files are not found.
 *
 *****************************************************************************/
static int testHome(char *path)
{
  char buf[STRMAX];
  FILE *f;

  sprintf(buf,"%s/site-preferences",path);
  if (!(f = fopen(buf,"r"))) return 0;
  fclose(f);

  sprintf(buf,"%s/locale/en/messages",path);
  if (!(f = fopen(buf,"r"))) return 0;
  fclose(f);

  sprintf(buf,"%s/locale/en/tutorials/welcome.v",path);
  if (!(f = fopen(buf,"r"))) return 0;
  fclose(f);

  return 1;
}

/*****************************************************************************
 *
 * Figure out which directory we should use as the tkgate home.
 *
 * 1) Directory specified by TKGATE_HOME environment variable
 * 2) Current directory
 * 3) Directory specified by TKGATE_HOMEDIR in config.h
 * 4) Directory specified by TKGATE_SECONDARYHOME in config.h
 *
 * The #define value TKGATE_HOMEDIR is normally generated automatically by the
 * configure script using TKGATE_HOMEDIRBASE as a prefix and tkgate-x.y (where
 * x.y is the version number) the subdirectory.  The #define value TKGATE_SECONDARYHOME
 * is normally set to the directory in which tkgate was configured/compiled.
 *
 *****************************************************************************/
void findTkGateHome(char *homeDir)
{
  char buf[STRMAX];
  char *trydirs[4];
  int i;

  trydirs[0] = getenv("TKGATE_HOME");
  trydirs[1] = getcwd(buf,STRMAX);
  trydirs[2] = TKGATE_HOMEDIR;
  trydirs[3] = TKGATE_SECONDARYHOME;

  for (i = 0;i < 4;i++) {
    if (!trydirs[i]) continue;
    strcpy(homeDir,trydirs[i]);
    if (testHome(homeDir)) break;
  }

  if (i == 4) {
    printf("\n");
    printf("I could not locate the tkgate home directory.  I tried looking in:\n");
    if (trydirs[0]) printf("  %s  (environment variable)\n", trydirs[0]);
    if (trydirs[1]) printf("  %s  (current directory)\n",    trydirs[1]);
    if (trydirs[2]) printf("  %s  (primary location)\n",     trydirs[2]);
    if (trydirs[3]) printf("  %s  (secondary location)\n",   trydirs[3]);
    printf("\n");
    printf("Try setting the environment variable TKGATE_HOME.\n");
    exit(1);
  }
}

static void PrintCopyright()
{
  extern char *release_date;

  printf("TkGate %s - %s (released %s)\n",VERSIONS[0].vd_name,TKGATE_DESCRIPTION,release_date);
  printf("[Compiled %s %s]\n",__DATE__,__TIME__);
  printf("%s\n",TKGATE_COPYRIGHT);
  printf("  TkGate comes with ABSOLUTELY NO WARRANTY;  see 'Help...License' menu\n");
  printf("  for license and warranty details.  Report problems to %s\n",TKGATE_MAILCONTACT);

  fflush(stdout);
}

static void LogCopyright()
{
  extern char *release_date;

  message(MC_MSGLOG|MC_SILENT,"TkGate %s - %s (released %s)",VERSIONS[0].vd_name,TKGATE_DESCRIPTION,release_date);
  message(MC_MSGLOG|MC_SILENT,"[Compiled %s %s]",__DATE__,__TIME__);
  message(MC_MSGLOG|MC_SILENT,"%s",TKGATE_COPYRIGHT);
  message(MC_MSGLOG|MC_SILENT,"  TkGate comes with ABSOLUTELY NO WARRANTY;  see 'Help...License' menu");
  message(MC_MSGLOG|MC_SILENT,"  for license and warranty details.  Report problems to %s",TKGATE_MAILCONTACT);
}

void init_linkvars(Tcl_Interp *tcl)
{
  extern int stepsize;
  static char randomstate[128];

  initstate(getpid() | time(0),randomstate,128);

  Tcl_SetVar(tcl,"tkg_progVer",VERSIONS[0].vd_name,TCL_GLOBAL_ONLY);
  Tcl_SetVar(tcl,"tkg_description",TKGATE_DESCRIPTION,TCL_GLOBAL_ONLY);
  Tcl_SetVar(tcl,"tkg_copyright",TKGATE_COPYRIGHT,TCL_GLOBAL_ONLY);
  Tcl_SetVar(tcl,"tkg_mailContact",TKGATE_MAILCONTACT,TCL_GLOBAL_ONLY);
  Tcl_SetVar(tcl,"tkg_homepage",TKGATE_HOMEPAGE,TCL_GLOBAL_ONLY);
  Tcl_SetVar(tcl,"tkg_localdoc",TKGATE_LOCALDOC,TCL_GLOBAL_ONLY);
  Tcl_SetVar(tcl,"tkg_gateHome",TkGate.homedir,TCL_GLOBAL_ONLY);
  Tcl_SetVar(tcl,"tkg_doSplash",binstr(do_splash),TCL_GLOBAL_ONLY);

  Tcl_LinkVar(tcl,"tkg_errorLogFile",(char*)&TkGate.errorLogFile,TCL_LINK_STRING);
  Tcl_LinkVar(tcl,"tkg_siteName",(char*)&TkGate.siteName,TCL_LINK_STRING);
  Tcl_LinkVar(tcl,"tkg_defaultTech",(char*)&TkGate.defaultTech,TCL_LINK_STRING);
  Tcl_LinkVar(tcl,"tkg_browserCommand",(char*)&TkGate.browserCommand,TCL_LINK_STRING);
  Tcl_LinkVar(tcl,"tkg_emailCommand",(char*)&TkGate.emailCommand,TCL_LINK_STRING);

  Tcl_LinkVar(tcl,"tkg_contVerify",(char*)&debugContinuousVerify,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_simStepSize",(char*)&stepsize,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_checkpointEnabled",(char*)&checkpointenabled,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_wantCheckpoint",(char*)&wantCheckpoint,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_simDebugInterface",(char*)&debugSimInterface,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_baderMode",(char*)&TkGate.baderp,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_trekMode",(char*)&TkGate.startrekp,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_batMode",(char*)&TkGate.batp,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_modifiedFlag",(char*)&TkGate.changedp,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_regionUpdate",(char*)&TkGate.regionUpdate,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_doBackupOnSave",(char*)&doBackupOnSave,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_doSafeSave",(char*)&doSafeSave,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_smoothScroll",(char*)&TkGate.smoothScroll,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_flashCPath",(char*)&TkGate.flashCPath,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_undoLength",(char*)&ob_max_undo,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_securityExec",(char*)&security_exec_level,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_blockDropConnect",(char*)&blockDropConnect,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_extendBars",(char*)&TkGate.extendBars,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_showSwitchNets",(char*)&TkGate.showSwitchNets,TCL_LINK_INT);

  Tcl_LinkVar(tcl,"simOn",(char*)&TkGate.circuit->simulator.active,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_discardChanges",(char*)&TkGate.circuit->discardChanges,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_useExtBars",(char*)&TkGate.circuit->useExtBars,TCL_LINK_INT);

  Tcl_LinkVar(tcl,"tkg_saveTraces",(char*)&TkGate.saveTraces,TCL_LINK_INT);
  Tcl_LinkVar(tcl,"tkg_simSortTraces", (char*)&TkGate.sortTraces, TCL_LINK_INT);
  
  DoTcl("namespace eval HdlEditor {}");
  Tcl_LinkVar(tcl,"HdlEditor::isActive",(char*)&hdl_isactive, TCL_LINK_INT);
}

/*
 * Set handlers on various signals we might receive.  Most abnormal signals
 * are caught by panicSave which attempts to save the current circuit.
 */
void tkgate_signalSetup()
{
  /*
    Make sure they really want to exit when ctl-c is typed.
   */
  signal(SIGINT,catchInterrupt);

  /*
    Do a panic save on any core dump producing signal, or
    on a SIGHUP.
   */
  if (do_panic_save) {
    signal(SIGHUP,panicSave);
    signal(SIGQUIT,panicSave);
    signal(SIGILL,panicSave);
    signal(SIGTRAP,panicSave);
    signal(SIGFPE,panicSave);
    signal(SIGBUS,panicSave);
    signal(SIGSEGV,panicSave);
#ifdef SIGEMT
    signal(SIGEMT,panicSave);
#endif
#ifdef SIGSYS
    signal(SIGSYS,panicSave);
#endif
  }
}

void undoSignals()
{
#ifdef SIGABRT
  signal(SIGABRT,SIG_DFL);
#endif
  signal(SIGHUP,SIG_DFL);
  signal(SIGQUIT,SIG_DFL);
  signal(SIGILL,SIG_DFL);
  signal(SIGTRAP,SIG_DFL);
  signal(SIGFPE,SIG_DFL);
  signal(SIGBUS,SIG_DFL);
  signal(SIGSEGV,SIG_DFL);
#ifdef SIGEMT
  signal(SIGEMT,SIG_DFL);
#endif
#ifdef SIGSYS
  signal(SIGSYS,SIG_DFL);
#endif
}


/*****************************************************************************
 *
 * Load the circuits on the command line only to check their integrety.
 *
 *****************************************************************************/
void doVerifyLoad()
{
  extern int VerilogErrorCount;
  int i;

  TkGate.tcl = 0;

  MSS_init();
  init_gates();


  TkGate.circuit = new_Circuit();
  TkGate.ed = (EditData*) malloc(sizeof(EditData));
  TkGate.errl = (ErrorList*) malloc(sizeof(ErrorList));

  for (i = 0;i < numLoads;i++) {
    VerilogOpen(&TkGate.circuit->es,loadList[i],(i > 0));

    /*
     * We got an error reading a circuit file.
     */
    if (VerilogErrorCount > 0)
      exit(1);
  }

  /*
   * We found inconsistencies in the file.
   */
  if (verify_circuit() > 0)
    exit(2);

  exit(0);
}

void trimQuotes(char *str)
{
  if (*str == '"') {
    int l;

    memmove(str,str+1,strlen(str));
    l = strlen(str);
    if (l > 0 && str[l-1] == '"')
      str[l-1] = 0;
  }
}

/*
 * Do defaults setup for command line printing.  We need to find out what the site
 * name is and get the options from the user's preferences file.  We need to read
 * the preferences file manually since we are not starting the tcl/tk interpreter.
 */
void commandLinePrintSetup(GPrintOpt *PO)
{
  FILE *f;
  char buf[STRMAX];
  char *p;

  /*
   * Get the default site name from the
   */
  sprintf(buf,"%s/site-preferences",TkGate.homedir);
  f = fopen(buf,"r");
  if (f) {
    char str[STRMAX];

    while (fgets(buf,STRMAX,f)) {
      if (sscanf(buf,"Option::value siteName %[^\n]",str) == 1) {
	trimQuotes(str);
	TkGate.siteName = strdup(str);
      }
    }

    fclose(f);
  }

  GPrintOpt_clDefault(PO);

  /*
   * If we have a home directory, open the preferences file and read
   * printing related defaults from it.
   */
  p = getenv("HOME");
  if (p) {
    extern PaperSize paperSizes[];

    sprintf(buf,"%s/.tkgate2-preferences",p);
    f = fopen(buf,"r");
    if (f) {
      const char *lang = getenv("LANG");
      char str[STRMAX];
      int d;

      while (fgets(buf,STRMAX,f)) {
	if (sscanf(buf,"Option::value siteName %[^\n]",str) == 1) {
	  trimQuotes(str);
	  TkGate.siteName = strdup(str);
	} else if (sscanf(buf,"Option::value printCommand %[^\n]",str) == 1) {
	  trimQuotes(str);
	  PO->po_cmd = strdup(str);
	} else if (sscanf(buf,"Option::value printPgSizeA4 %d",&d) == 1) {
	  if (strncmp(lang,"en",2) != 0)
	    PO->po_paper = strdup(paperSizes[d].ps_size);
	} else if (sscanf(buf,"Option::value printPgSize %d",&d) == 1) {
	  if (strncmp(lang,"en",2) == 0)
	    PO->po_paper = strdup(paperSizes[d].ps_size);
	} else if (sscanf(buf,"Option::value printPgOrient %s",str) == 1) {
	  PO->po_orient = strdup(str);
	} else if (sscanf(buf,"Option::value printStyle %s",str) == 1) {
	  PO->po_style = strdup(str);
	} else if (sscanf(buf,"Option::value printDuplex %s",str) == 1) {
	  PO->po_isDuplex = strdup(str);
	} else if (sscanf(buf,"Option::value printIndex %s",str) == 1) {
	  PO->po_index = strdup(str);
	} else if (sscanf(buf,"Option::value printGraph %s",str) == 1) {
	  PO->po_graph = strdup(str);
	}
      }
      fclose(f);
    }
  }

  if (!TkGate.siteName)
    TkGate.siteName = strdup("CLP");

}

/*****************************************************************************
 *
 * Set up for doing printing from the command line.
 *
 *****************************************************************************/
void commandLinePrint()
{
  extern int VerilogErrorCount;
  char *p;
  int i;
  GPrintOpt PO;
  extern PaperSize paperSizes[];


  TkGate.tcl = 0;
  init_gates();
  TkGate.ed = (EditData*) malloc(sizeof(EditData));
  TkGate.errl = (ErrorList*) malloc(sizeof(ErrorList));
  TkGate.circuit = new_Circuit(0);


  for (i = 0;i < numLoads;i++) {
    VerilogOpen(&TkGate.circuit->es,loadList[i],(i > 0));

    /*
     * We got an error reading a circuit file.
     */
    if (VerilogErrorCount > 0)
      exit(1);
  }

  /*
   * Apply default options
   */
  commandLinePrintSetup(&PO);


  if (print_file) {
    PO.po_file = print_file;
    PO.po_cmd = 0;
  } else {
    PO.po_cmd = print_printer;
    PO.po_file = 0;
  }

  /*
   * Apply printer options
   */
  for (p = strtok(print_options,":");p;p = strtok(0,":")) {
    char name[STRMAX];
    char sval[STRMAX];
    int n;

    if (!*p) continue;	/* Ignore empty options */

    if (sscanf(p,"%[^=]=%s",name,sval) == 2) {
    } else if (sscanf(p,"%[^=]",name) == 1) {
      strcpy(sval,"1");
    } else {
      fprintf(stderr,"tkgate: Badly formatted print option '%s'\n",p);
      fprintf(stderr,"  run 'tkgate -h' for help.\n");
      exit(1);
    }

    n = strlen(name);
    if (strncmp(name,"duplex",n) == 0) {
      PO.po_isDuplex = strdup(sval);
    } else if (strncmp(name,"epsf",n) == 0) {
      PO.po_epsf = strdup(sval);
    } else if (strncmp(name,"index",n) == 0) {
      PO.po_index = strdup(sval);
    } else if (strncmp(name,"hierarchy",n) == 0) {
      PO.po_graph = strdup(sval);
    } else if (strncmp(name,"paper",n) == 0) {
      PO.po_paper = strdup(sval);
    } else if (strncmp(name,"modules",n) == 0) {
      PO.po_select = "sel";
      PO.po_modlist = strdup(sval);
    } else if (strncmp(name,"4up",n) == 0) {
      PO.po_4up = strdup(sval);
    } else {
      fprintf(stderr,"tkgate: Unknown print option '%s'\n",name);
      fprintf(stderr,"  run 'tkgate -h' for help.\n");
      exit(1);
    }
  }


  /*
   * Check for valid paper size and convert to expected case if necessary.
   */
  for (i = 0;paperSizes[i].ps_size;i++)
    if (strcasecmp(PO.po_paper,paperSizes[i].ps_size) == 0) {
      PO.po_paper = paperSizes[i].ps_size;
      break;
    }
  if (!paperSizes[i].ps_size) {
      fprintf(stderr,"tkgate: Unknown paper size '%s'\n",PO.po_paper);
      exit(1);
  }


  ob_begin_frame("Print");
  /*  Html_setFormatTarget(HT_FORCEPRINT);*/
  GPrintOpt_print(&PO);
  ob_end_frame();

  exit(0);
}

void usage()
{
  fprintf(stderr,"Usage: tkgate [options][file]\n\n");
  fprintf(stderr,"  Options:\n");
  fprintf(stderr,"    -h         Print this summary of options.\n");
  fprintf(stderr,"    -s         Run in X11 synchronous mode (slow).\n");
  fprintf(stderr,"    -q         Suppress some messages to the tty.\n");
  fprintf(stderr,"    -x         Start simulator immediately.\n");
  fprintf(stderr,"    -n         Do not display the splash window on startup.\n");
  fprintf(stderr,"    -V         Verify a save file..\n");
  fprintf(stderr,"    -X file    Start simulator immediately and execute script.\n");
  /*  fprintf(stderr,"    -l file    Load file as a library.\n");*/
  fprintf(stderr,"    -L lang    Specify language (if configured).\n");
  fprintf(stderr,"    -p file    Print to a file.\n");
  fprintf(stderr,"    -P prn     Output to a printer.\n");
  fprintf(stderr,"    -O opts    Printer options.\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"  Printer options are specified as a colon separated list of\n");
  fprintf(stderr,"  the following items (identifiers can be abreviated):\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"    epsf=bool      Use encapsulated Postscript format\n");
  fprintf(stderr,"    duplex=bool    Use duplex printing\n");
  fprintf(stderr,"    index=bool     Include index\n");
  fprintf(stderr,"    hier=bool      Include hierarchy graph\n");
  fprintf(stderr,"    paper=type     Use specified paper type\n");
  fprintf(stderr,"    4up=bool       Do 4-up printing of small modules\n");
  fprintf(stderr,"    modules=list   List of modules to print (comma separated)\n");
  fprintf(stderr,"\n");
  exit(0);
}

void setLanguage(const char *lang)
{
  static char buf[128];

  if (strlen(lang) > 100) {
    putenv("TKGATE_LANG=ASCII");
    return;
  }

  /*
   * Solaris putenv man page claims the actual string passed
   * to putenv is used in the environment so we must make a
   * a copy for the putenv().
   */
  sprintf(buf,"TKGATE_LANG=%s",lang);
  putenv(strdup(buf));

  if (strcasecmp(lang,"ja") == 0) {
    lang = "ja_JP.eucJP";

    sprintf(buf,"LC_ALL=%s",lang);
    putenv(strdup(buf));

    sprintf(buf,"LC_CTYPE=%s",lang);
    putenv(strdup(buf));

    sprintf(buf,"LANG=%s",lang);
    putenv(strdup(buf));
  }
}

/*****************************************************************************
 *
 * Parse the command-line options to tkgate.
 *
 * Parameters: 
 *      argc			Number of command-line arguments
 *      argv			Command-line argument array
 *
 *****************************************************************************/
void parse_options(int argc,const char *argv[])
{
  int c;
  extern char *optarg;
  extern int optind;
#if OPTRESET
  extern int optreset;
#endif
  char *lib_env;

  loadList = (const char**) calloc(argc,sizeof(char*));
  libLoadList = (const char**) calloc(argc,sizeof(char*));

  /*
   * Check environment variable for libraries to load
   */
  lib_env = getenv("TKGATE_LIBS");
  if (lib_env) {
    char *t;

    lib_env = strdup(lib_env);
    for (t = strtok(lib_env,":");t;t = strtok(0,":")) {
      libLoadList[numLibLoads++] = strdup(t);
    }
    free(lib_env);
  }

  while (argc > 0) {
    /*
     * NOTE: The '-l' option has been disabled
     */
    while ((c = getopt(argc,(char**)argv,"vVAhdxqsnX:L:p:P:O:H:")) != EOF) {
      switch (c) {
      case 'H' :
	tkgateHomeDir = strdup(optarg);
	break;
      case 'v' :
	is_verbose = 1;
	break;
      case 'V' :
	verify_only = 1;
	break;
      case 'A' :
	do_panic_save = 0;
	break;
      case 'n' :
	do_splash = 0;
	break;
      case 'X' :
	start_sim = 1;
	sim_script = strdup(optarg);
	break;
      case 'O' :
	if (print_options) {
	  print_options = realloc(print_options,strlen(print_options)+strlen(optarg)+2);
	  strcat(print_options,":");
	  strcat(print_options,optarg);
	} else
	  print_options = strdup(optarg);
	break;
      case 'p' :
	print_file = strdup(optarg);
	break;
      case 'P' :
	print_printer = strdup(optarg);
	break;
      case 'x' :
	start_sim = 1;
	break;
      case 'q' :
	quietMode = 1;
	break;
      case 's' :
	sync_Xserver = 1;
	break;
      case 'd' :
	debug = 1;
	break;
      case 'l' :
	libLoadList[numLibLoads++] = strdup(optarg);
	break;
      case 'L' :
	setLanguage(optarg);
	break;
      case 'h' :
      default :
	usage();
	break;
      }
    }
    argc -= optind;
    argv += optind;
#if OPTRESET
    optreset = 1;
#endif
    optind = 0;
    if (argc > 0) {
      loadList[numLoads++] = strdup(argv[0]);
      argc--;
      argv++;
    }
  }
}

/*****************************************************************************
 *
 * See if Miles Bader is running this program and harass him just a bit.
 *
 *****************************************************************************/
void bader_check()
{
  struct passwd *pw = getpwuid(getuid());
  char buf[STRMAX],*p;

  strcpy(buf,pw->pw_gecos);
  for (p = buf;*p;p++)
    if (isupper(*p)) *p = tolower(*p);

  if (strcasestr(buf,"miles") != 0 && strcasestr(buf,"bader") != 0) {
    TkGate.baderp = 1;
    printf("Hi sMiles!\n");
  } else if ((strcasestr(buf,"james") != 0 || strcasestr(buf,"jim") != 0) && strcasestr(buf,"kirk") != 0) {
    TkGate.startrekp = 1;
    printf("Kirk, this place is swarming with Klingons!\n");
  } else if ((strcasestr(buf,"william") != 0 || strcasestr(buf,"bill") != 0) && strcasestr(buf,"shatner") != 0) {
    TkGate.startrekp = 1;
    printf("Do you have a point Spock?\n");
  } else if (strcasestr(buf,"bruce") != 0 && strcasestr(buf,"wayne") != 0) {
    TkGate.batp = 1;
    printf("Let's go, Robin. The longer we tarry, the more dire the peril.\n");
  }
}

/*****************************************************************************
 *
 * Read delay files and associate them with gates
 *
 *****************************************************************************/
void TkGate_initDelay()
{
  char buf[STRMAX];
  HashElem *E;
  int i;
  const char *p;

  GDelayDef_flush();

  p = Tcl_GetVar(TkGate.tcl,"tkg_simDefaultDelayFile",TCL_GLOBAL_ONLY);
  if (GDelayDef_readFile(p) < 0)
    message(1,"Can not open default delay file '%s'.",p);

  p = Tcl_GetVar(TkGate.tcl,"tkg_simDelayFile",TCL_GLOBAL_ONLY);
  strcpy(buf,p);
  for (p = strtok(buf," ");p;p = strtok(0," ")) {
    if (GDelayDef_readFile(p) < 0)
      message(1,"tkgate: Can not open delay file '%s'.",p);
  }

  for (E = Hash_first(GateIdxHash);E;E = Hash_next(GateIdxHash,E)) {
    GGateInfo *gi = (GGateInfo*) HashElem_obj(E);

    for (i = 0;gi->delayNames[i];i++);
    gi->num_delays = i;

    if (!gi->num_delays) continue;

    gi->delay_defs = GDelayDef_findList(gi->name);
    if (!gi->delay_defs) {
      message(1,"No delay definitions found for primitive '%s'.",gi->name);
    } else {
      GDelayDef *dd = GDelayDef_findTech(gi->delay_defs,TKGATE_DEFAULT_TECH);
      if (strcmp(dd->dd_tech,TKGATE_DEFAULT_TECH) != 0)
	message(1,"No 'default' delay definition found for primitive '%s'.",gi->name);
    }
  }

  DoTcl("ToolBar::resetTechList");
}

/*****************************************************************************
 *
 * Called to do final tkgate initilization (called from tcl script)
 *
 *****************************************************************************/
void TkGate_init(Tcl_Interp *tcl)
{
  int i;

  TkGate_initDelay();

  sel_updateMenuState();

  LogCopyright();

  /*
   * Load first file on command line.  If the load fails, treat this as a new
   * circuit using the specified file. 
   */
  if (numLoads > 0) {
    const char *result;

    DoTclL("gat_load",loadList[0],NULL);
    result = Tcl_GetStringResult(tcl);
    if (*result == '0')
      DoTclL("gat_new",loadList[0],"main",NULL);
  }

  if (numLoads == 0)
    DoTcl("File::loadWelcomeCircuit");


  /*
   * Load modules from additional files as libraries
   */
  for (i = 1;i < numLoads;i++) {
    DoTclL("gat_loadMore",loadList[i],NULL);
  }
  for (i = 0;i < numLibLoads;i++) {
    DoTclL("gat_loadLibrary",libLoadList[i],NULL);
  }


  /*
   * Open the root module in the module tree list.
   */
  DoTcl("BlockTree::openRoot");

  free(libLoadList);
  free(loadList);

  guiActive = 1;

  if (start_sim) {
    if (sim_script)
      Tcl_SetVar(TkGate.tcl,"sopts_simInitScript",sim_script,TCL_GLOBAL_ONLY);
    tkgate_setMajorMode(MM_SIMULATE);
  }


  /*
   * If a print file has been specified.  Print the file and exit tkgate.
   */
  if (print_file) {
    DoTcl("tkg_printWithDefaults 1 %s",print_file);
    exit(0);
  }

  /*
   * If a printer has been specified.  Print to specified printer and
   * exit tkgate.
   */
  if (print_printer) {
    DoTcl("tkg_printWithDefaults 0 %s",print_printer);
    exit(0);
  }

  bader_check();
  tkgate_signalSetup();			/* set traps on signals */
}

/*****************************************************************************
 *
 * Look for the tutorials based on the current locale, or fall back on the
 * English locale tutorials if they can not be located.
 *
 *****************************************************************************/
void init_tutorials(Tcl_Interp *tcl)
{
  char buf[STRMAX];
  const char *lang = Tcl_GetVar(tcl,"lang",TCL_GLOBAL_ONLY);
  struct stat sb;

  sprintf(buf,"%s/locale/%s/tutorials",TkGate.homedir,lang);
  if (stat(buf,&sb) || ((sb.st_mode & S_IFMT) != S_IFDIR)) {
    sprintf(buf,"%s/locale/en/tutorials",TkGate.homedir);
    printf("[No tutorials found for locale '%s'.  Using English tutorials.]\n",lang);
    if (stat(buf,&sb) || ((sb.st_mode & S_IFMT) != S_IFDIR)) {
      printf("[No tutorials found for English either'.  Help!]\n");
      exit(1);
    }
  }

  tkgate_tutorialdir = ob_strdup(buf);
  Tcl_SetVar(tcl,"tkgate_tutorialdir",buf,TCL_GLOBAL_ONLY);

  sprintf(buf,"%s/locale/%s/examples",TkGate.homedir,lang);
  if (stat(buf,&sb) || ((sb.st_mode & S_IFMT) != S_IFDIR)) {
    sprintf(buf,"%s/locale/en/examples",TkGate.homedir);
    printf("[No examples found for locale '%s'.  Using English examples.]\n",lang);
    if (stat(buf,&sb) || ((sb.st_mode & S_IFMT) != S_IFDIR)) {
      printf("[No examples found for English either'.  Help!]\n");
      exit(1);
    }
  }

  tkgate_exampledir = ob_strdup(buf);
  Tcl_SetVar(tcl,"tkgate_exampledir",buf,TCL_GLOBAL_ONLY);
}

/*****************************************************************************
 *
 * Initialize the application description object to all zeros.
 *
 *****************************************************************************/
void init_TkGate()
{
  memset(&TkGate,0,sizeof(TkGate));
  TkGate.libraries = new_SHash();
  TkGate.homedir = tkgateHomeDir;
}

/*****************************************************************************
 *
 * Create a new current file object
 *
 *****************************************************************************/
CurrentFile *new_CurrentFile()
{
  CurrentFile *cf = (CurrentFile*)ob_malloc(sizeof(CurrentFile),"CurrentFile");

  cf->baseName = ob_strdup("");
  cf->dirName = ob_strdup("");
  cf->fullPathName = ob_strdup("");

  return cf;
}

/*****************************************************************************
 *
 * Convert a file path name to a full path name.
 *
 * Parameters:
 *     ipath			Specified path name (full or relative)
 *     rpath			Return for full path name
 *
 *****************************************************************************/
char *canonical_path(const char *ipath,char *rpath)
{
  char *p;

  /*
   * If no leading '/', look up the current directory and append it to the file name.
   */
  if (*ipath == '/') {
    strcpy(rpath, ipath);
  } else {
    char cwd[PATH_MAX];

    getcwd(cwd,PATH_MAX);
    sprintf(rpath,"%s/%s",cwd,ipath);
  }

  /*
   * Replace any // with /
   */
  while ((p = strstr(rpath,"//"))) {
    memmove(p,p+1,strlen(p+1)+1);
  }



  /*
   * Handle any
   */
  while ((p = strstr(rpath,"/../"))) {
    if (p == rpath)
      memmove(p,p+3,strlen(p+3)+1);
    else {
      char *q;

      for (q = p-1;q > rpath;q--)
	if (*q == '/') break;

      memmove(q,p+3,strlen(p+3)+1);
    }
  }

  while ((p = strstr(rpath,"/./"))) {
    memmove(p,p+2,strlen(p+2)+1);
  }

  return rpath;
}

/*****************************************************************************
 *
 * Change the file name in a current file object.
 *
 * Parameters:
 *      cf			Current file object to set
 *      name			Name of file.
 *
 *****************************************************************************/
int CurrentFile_set(CurrentFile *cf,const char *name)
{
  char full_path[PATH_MAX];
  char *base_point;

  ob_touch(cf);

  if (!canonical_path(name, full_path))
    return -1;

  base_point = strrchr(full_path,'/');
  if (!base_point && base_point[1])		/* There is a '/' and there are chars after it */
    return -1;

  ob_free(cf->fullPathName);
  ob_free(cf->dirName);
  ob_free(cf->baseName);

  cf->fullPathName = ob_strdup(full_path);
  cf->dirName = ob_strdup(full_path);
  cf->dirName[base_point-full_path] = 0;
  cf->baseName = ob_strdup(base_point+1);

  if (TkGate.tcl) {
    Tcl_SetVar(TkGate.tcl,"tkg_currentFile",cf->fullPathName,TCL_GLOBAL_ONLY);
  }

  return 0;
}

static void init_tk(Tcl_Interp *tcl)
{
  int r;

  /*
   * Start up Tk
   */
  r = Tk_Init(tcl);
  if (r == TCL_ERROR) {
    fprintf(stderr,"Tk_Init Error in tkgate:\n%s\n",tcl->result);
    fprintf(stderr,"Perhaps you could try setting the environment variable TK_LIBRARY\n");
    fprintf(stderr,"to the directory in which tk init files can be found.  You can also\n");
    fprintf(stderr,"set TK_LIBRARY in options.h.\n");
    exit(1);
  }
}

/*****************************************************************************
 *
 * This is the beginning of the tkgate initialization
 *
 *****************************************************************************/
static int tkgate_main(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  char buf[STRMAX];

  parse_options(argc,argv);		/* Parse the command line options */

  init_TkGate();				/* tkgate "main" object */ 
  ob_init();				/* Object memory handeling */
  init_gateHashTables();		/* Hash tables for gates */
  init_tclProcs(tcl);			/* install tkgate tcl commands */
  init_localeSet();			/* Get set of supported locales, and set English as default */
  init_iconTables();			/* Tables for storing icons */
  init_cpathNetDelayTable();		/* Initialize delay tables for critical path analysis */

  localization_Setup(tcl);		/* Set up for the selected locale */
  if (verify_only)			/* Do verify and exit if we had the -V switch */ 
    doVerifyLoad();
  if (print_file || print_printer)	/* Print file and exit if we have the -p or -P switches */
    commandLinePrint();
  if (!quietMode) 			/* Print the copyright notice (unless we are in quiet mode) */
    PrintCopyright();

  return TCL_OK;
}

static int tkgate_starttk(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  init_tk(tcl);
  return TCL_OK;
}

static int tkgate_posttk(ClientData _d,Tcl_Interp *tcl,int argc,const char *argv[])
{
  init_tutorials(tcl);			/* Set up tutorials for selected locale. */
  init_mainWindow(tcl);			/* The main window for all your editing needs */
  init_linkvars(tcl);			/* bindings between tcl and C variables */
  init_cursors();			/* various mouse cursors we need*/
  init_specials();			/* special images that we need (arrows, etc.) */
  init_gates();				/* initialized built-in component types */

  return TCL_OK;
}

/*****************************************************************************
 *
 * Register the main proceedure for tkgate
 *
 *****************************************************************************/
int Tkgate_Init(Tcl_Interp *tcl)
{
  Tcl_CreateCommand(tcl,"tkgate_main",tkgate_main,0,0);
  Tcl_CreateCommand(tcl,"tkgate_starttk",tkgate_starttk,0,0);
  Tcl_CreateCommand(tcl,"tkgate_posttk",tkgate_posttk,0,0);
  return 0;
}
