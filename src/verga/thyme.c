/****************************************************************************
    Copyright (C) 1987-2007 by Jeffery P. Hansen

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

    Last edit by hansen on Fri Feb 13 20:25:04 2009
****************************************************************************/
#include "thyme.h"

int errCount = 0;
int warnCount = 0;
extern Place curPlace;			/* The current place */

VGSim vgsim;

/*
 * These undoable object functions are only used in the main tkgate code, but
 * since they are used in the common hash table code we provide stubs here for
 * the hash table code to use.
 */
void *ob_malloc(int s,char *x) { return malloc(s); }
void *ob_calloc(int n,int s,char *x) { return calloc(n,s); }
void ob_free(void *p) { free(p); }
char *ob_strdup(char *s) { return strdup(s); }
void ob_set_type(void *o,char *n) {}
void ob_touch(void *o) {}

/*****************************************************************************
 *
 * Initialize the security flags
 *
 * Parameters:
 *     vgs		VGSecurity object to be initialized.
 *     trusted		Non-zero to indicate full trust of input file.
 *
 *****************************************************************************/
void VGSecurity_init(VGSecurity *vgs,int trusted)
{
  vgs->vgs_send = trusted;
  vgs->vgs_fopen = trusted;
  vgs->vgs_writemem = trusted;
  vgs->vgs_queue = trusted;
  vgs->vgs_exec = trusted ? 2 : 0;
  vgs->vgs_handling = 0;
}

/*****************************************************************************
 *
 * Handle a security exception
 *
 * Parameters:
 *     vgs		VGSecurity object to be initialized.
 *
 *****************************************************************************/
void VGSecurity_handleException(VGSecurity *vgs,VGThread *t,const char *name)
{
  EvQueue *Q = Circuit_getQueue(t->t_modCtx->mc_circuit);

  switch (vgs->vgs_handling) {
  case 0 :		/* ignore */
    return;
  case 1 :		/* warning */
    vgio_printf("error run Execution of protected system task '%s' blocked.\n",name);
    return;
  case 2 :		/* stop */
    EvQueue_stopAfter(Q,0);
    VGThread_suspend(t);
    EvQueue_enqueueAfter(Q,new_EvThread(t), 0);
    vgio_printf("error run Simulation stopped on attempted execution of protected system task '%s'.\n",name);
    return;
  }
}

/*****************************************************************************
 *
 * Initialize the VGSim object
 *
 * Parameters:
 *     vg		VGSim object to be initialized.
 *
 *****************************************************************************/
void VGSim_init(VGSim *vg)
{
  vg->vg_baseDirectory = 0;
  vg->vg_topModuleName = 0;
  vg->vg_defaultTopModuleName = 0;
  vg->vg_interactive = 0;
  SHash_init(&vg->vg_modules);
  Circuit_init(&vg->vg_circuit);
  VGSecurity_init(&vg->vg_sec,0);
  vg->vg_timescale.ts_units = 0;
  vg->vg_timescale.ts_precision = 0;
  vg->vg_haveTScount = 0;
  vg->vg_delayType = DT_TYP;
  vg->vg_noTimeViolations = 0;
  vg->vg_initTime = 0;
}

static void usage()
{
  errorCmd(ERR_USAGE);
  if (errCount) {
    waitForExit();
  }
}

/*****************************************************************************
 *
 * Open an file in the current path context
 *
 * Parameters:
 *     name		Name of file to open.
 *
 * Open a file in the context of the current circuit.
 * If the first character of 'name' is '/', then the
 * file is opened "as is".  Otherwise the following
 * path is searched:
 *
 *    The base_directory spoecified by -B option
 *    current directory
 *    user's home directory (value of HOME)
 *****************************************************************************/
FILE *openInPath(const char *name)
{
  char buf[STRMAX];
  FILE *f;
  char *r;

  /*
   * If full path, just open the file.
   */
  if (*name == '/') return fopen(name,"r");

  /*
   * If a base directory is specified, look in that directory.
   */
  if ((r = vgsim.vg_baseDirectory)) {
    strcpy(buf,r);
    if ((r = strrchr(buf,'/'))) {
      if (r != buf) {
	strcpy(r+1,name);
	if ((f = fopen(buf,"r"))) return f;
      }
    }
  }

  /*
   * Look in the current directory.
   */
  if ((f = fopen(name,"r")))
    return f;

  /*
   * Look in the user's home directory.
   */
  if ((r = getenv("HOME"))) {
    strcpy(buf,r);
    strcat(buf,"/");
    strcat(buf,name);
    if ((f = fopen(buf,"r"))) return f;
  }

  return 0;
}

/*****************************************************************************
 *
 * Add a module declaration to the module table.
 *
 * Parameters:
 *     vg		The vgsim object
 *     m		Module declatation object
 *
 *****************************************************************************/
void VGSim_addModule(VGSim *vg, ModuleDecl *m)
{
  if (!vg->vg_defaultTopModuleName && List_numElems(&m->m_ports) == 0)
    vg->vg_defaultTopModuleName = m->m_name;

  SHash_insert(&vg->vg_modules, m->m_name, m);

  /* Do post-definition processing of module */
  if (m->m_specify && Specify_numStats(m->m_specify) > 0) {
    ModuleDecl_makeFaninTree(m);
  }

  /* Update global timescale */
  if (m->m_timescale.ts_units != 0) {
    if (vg->vg_timescale.ts_units == 0 || m->m_timescale.ts_units < vg->vg_timescale.ts_units)
      vg->vg_timescale.ts_units = m->m_timescale.ts_units;

    vg->vg_haveTScount++;
  }
  if (m->m_timescale.ts_precision != 0) {
    if (vg->vg_timescale.ts_precision == 0 || m->m_timescale.ts_precision < vg->vg_timescale.ts_precision)
      vg->vg_timescale.ts_precision = m->m_timescale.ts_precision;
  }
}

/*****************************************************************************
 *
 * Lookup a module declaration in the module table.
 *
 * Parameters:
 *     vg		The vgsim object
 *     name		Name of the module to find
 *
 * Returns:		Module declaration object or null if not found.
 *
 *****************************************************************************/
ModuleDecl *VGSim_findModule(VGSim *vg, const char *name)
{
  return (ModuleDecl*) SHash_find(&vg->vg_modules, name);
}

/*****************************************************************************
 *
 * Load a list of files
 *
 * Parameters:
 *     vg		The vgsim object
 *     load_files	List of files to be loaded.
 *
 *****************************************************************************/
void VGSim_loadFiles(VGSim *vg, List *load_files)
{
  ListElem *E;

  for (E = List_first(load_files);E;E = List_next(load_files,E)) {
    const char *fileName = (const char*)ListElem_obj(E);

    if (VerilogLoad(fileName) < 0) {
      errorCmd(ERR_NOREAD,fileName);
    }
  }
}

/*****************************************************************************
 *
 * Print specified modules for debugging purposes
 *
 * Parameters:
 *     vg		The vgsim object
 *     show_modules	List of modules to be printed
 *
 *****************************************************************************/
void VGSim_printModules(VGSim *vg,List *show_modules)
{
  ListElem *E;

  for (E = List_first(show_modules);E;E = List_next(show_modules,E)) {
    const char *modName = (const char*)ListElem_obj(E);

    if (strcmp(modName,"-") == 0) {
      HashElem *HE;
      for (HE = Hash_first(&vg->vg_modules);HE;HE = Hash_next(&vg->vg_modules,HE)) {
	ModuleDecl *m = (ModuleDecl *) HashElem_obj(HE);
	printf("\n");
	ModuleDecl_print(m, stdout);
      }
    } else {
      ModuleDecl *m = VGSim_findModule(vg, modName);
      if (m) {
	printf("\n");
	ModuleDecl_print(m, stdout);
      }
    }
  }
}

/*****************************************************************************
 *
 * Show useful information about defined modules.
 *
 * Parameters:
 *     vg		The vgsim object
 *
 * This function is invoked with the '-s' switch and is used by tkgate to
 * check the integrety of text modules.
 *
 *****************************************************************************/
void VGSim_displayModuleData(VGSim *vg)
{
  HashElem *he;

  for (he = Hash_first(&vg->vg_modules);he;he = Hash_next(&vg->vg_modules,he)) {
    ModuleDecl *m = (ModuleDecl *) HashElem_obj(he);

    ModuleDecl_printData(m);
  }
}

/*****************************************************************************
 *
 * Set the default timescale for all modules to 1ns / 1ns.
 *
 * Parameters:
 *     vg		The vgsim object
 *
 *****************************************************************************/
void VGSim_useDefaultTimescale(VGSim *vg)
{
  HashElem *he;
  Timescale ts;

  ts.ts_units = ts.ts_precision = Directive_parseTimescale(1,"ns");

  vg->vg_timescale = ts;

  for (he = Hash_first(&vg->vg_modules);he;he = Hash_next(&vg->vg_modules,he)) {
    ModuleDecl *m = (ModuleDecl *) HashElem_obj(he);
    m->m_timescale = ts;
  }
}

void exitIfError()
{
  if (vgsim.vg_interactive) {
    if (errCount > 0) {
      waitForExit();
      exit(0);
    }
  }
}

int startSimulation(const char *topName,int warning_mode,List *load_scripts,const char *initTimeSpec)
{
  ModuleDecl *m = VGSim_findModule(&vgsim, topName);
  ListElem *le;

  /*
   * We have a top module name, but if we can't find it we are toast.
   */
  if (!m) {
    errorFile(&curPlace,ERR_NOTOP, topName);
    exitIfError();
    return 1;
  }



  /*
   * Build the circuit and sort threads for proper initialization.
   */
  Circuit_build(&vgsim.vg_circuit,m);
  Circuit_sortThreads(&vgsim.vg_circuit);
  Circuit_check(&vgsim.vg_circuit);

  if (vgsim.vg_interactive) {
    exitIfError();

    /*
     * warning_mode codes:
     *    1 - ignore warnings
     *    2 - report warnings only if there were errors
     *    3 - always report warnings
     *    4 - always report warnings and stop simulator even if only warnings
     */
    switch (warning_mode) {
    case 3 :
    case 2 :
    case 1 :
      break;
    case 4 :
    default :
      if (warnCount > 0) {
	waitForExit();
	exit(0);
      }
    }

    /*
      Why was this here?
      Circuit_update(&vgsim.vg_circuit);
    */


    /*
     * Report that we are ready to simulate and tell TkGate what time
     * units and precision we are currently using.
     */
    {
      int n1,n2;
      char u1[STRMAX],u2[STRMAX];

      Timescale_decode(vgsim.vg_timescale.ts_units, &n1, u1);
      Timescale_decode(vgsim.vg_timescale.ts_precision, &n2, u2);

      vgio_printf("ok %d %s / %d %s\n",n1,u1,n2,u2);
    }
  } else {
    if (errCount > 0) {
      vgio_printf("Total of %d errors in input file(s).\n",errCount);
      exit(0);
    }
  }


  /*
   * Check to see if there is an initialization time specification.  If so,
   * use this to supress timing violations at the beginning of the simulation.
   */
  if (initTimeSpec) {
    double n;
    char units[STRMAX];
    if (sscanf(initTimeSpec,"%lf%s",&n,units) == 2 && n >= 0) {
      vgsim.vg_initTime = Timescale_toSimtime(&vgsim.vg_timescale,n,units);
    } else if (strcmp(initTimeSpec,"none") == 0)
      vgsim.vg_noTimeViolations = 1;
  }


  /*
   * If there were any simulation scripte specified on the command line,
   * load them now.
   */
  for (le = List_first(load_scripts);le;le = List_next(load_scripts,le)) {
    char *fileName = (char*)ListElem_obj(le);
    int argc = 3;
    char *argv[3];
    argv[1] = "1";
    argv[2] = fileName;
    Circuit_execScript(&vgsim.vg_circuit,argc,argv);
  }


  /*
   * Start the actual simulation.
   */
  Circuit_run(&vgsim.vg_circuit);

  return 0;
}

/*****************************************************************************
 *
 * Display the license for Verga.
 *
 *****************************************************************************/
void showLicense()
{
  printf("%s %s - Verilog Simulator for TkGate\n",VGSIM_NAME,VGSIM_VERSION);
  printf("%s\n",VGSIM_COPYRIGHT);
  printf("\n");
  printf("    This program is free software; you can redistribute it and/or modify\n");
  printf("    it under the terms of the GNU General Public License as published by\n");
  printf("    the Free Software Foundation; either version 2 of the License, or\n");
  printf("    (at your option) any later version.\n");
  printf("\n");
  printf("    This program is distributed in the hope that it will be useful,\n");
  printf("    but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  printf("    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  printf("    GNU General Public License for more details.\n");
  printf("\n");
  printf("    You should have received a copy of the GNU General Public License\n");
  printf("    along with this program; if not, write to the Free Software\n");
  printf("    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.\n");
  printf("\n");
  exit(0);
}

/*****************************************************************************
 *
 * main program
 *
 *****************************************************************************/
int main(int argc,char *argv[])
{
  extern char *optarg;
  extern int optind;
  extern int yy_is_editor;
  extern int warning_mode;
  List load_files;
  List load_scripts;
  List show_modules;
  int scan_mode = 0;
  int c;
  int quiet = 0;
  int delete_on_load = 0;
  unsigned delete_hash_code = 0;
  ListElem *le;
  const char *initTimeSpec = 0;

  initErrorMessages();
  List_init(&load_files);
  List_init(&load_scripts);
  List_init(&show_modules);
  VGSim_init(&vgsim);

  yy_is_editor = 0;

  /*
   * Parse the command-line options.
   */
  while (argc > 0) {
    while ((c = getopt(argc,argv,"eslqid:S:P:t:B:D:W:I:")) != EOF) {
      switch (c) {
      case 'e' :
	dumpErrorMessages();
	exit(0);
	break;
      case 'l' :
	showLicense();
	break;
      case 'S' :
	List_addToTail(&load_scripts,optarg);
	break;
      case 'D' :
	if (sscanf(optarg,"%u",&delete_hash_code) == 1)
	  delete_on_load = 1;
	break;
      case 'W' :
	sscanf(optarg,"%d",&warning_mode);
	break;
      case 'q' :
	quiet = 1;
	break;
      case 's' :
	scan_mode = 1;
	Place_setMode(PM_MODULE|PM_MODLINE);
	break;
      case 'i' :
	vgsim.vg_interactive = 1;
	Place_setMode(PM_FILE|PM_LINE|PM_MODULE|PM_MODLINE);
	break;
      case 'P' :
	List_addToTail(&show_modules,optarg);
	break;
      case 'B' :
	vgsim.vg_baseDirectory = optarg;
	break;
      case 't' :
	vgsim.vg_topModuleName = optarg; 
	break;
      case 'd' :
	if (strcasecmp(optarg,"min") == 0)
	  vgsim.vg_delayType = DT_MIN;
	else if (strcasecmp(optarg,"max") == 0)
	  vgsim.vg_delayType = DT_MAX;
	else if (strcasecmp(optarg,"typical") == 0)
	  vgsim.vg_delayType = DT_TYP;
	break;
      case 'I' :
	initTimeSpec = optarg;
	break;
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
      List_addToTail(&load_files,argv[0]);
      argc--;
      argv++;
    }
  }

  /*
   * Disable security guards if not running in interactive mode.
   */
  if (!vgsim.vg_interactive)
    VGSecurity_init(&vgsim.vg_sec,1);

  if (!quiet) {
    vgio_comment("%s %s - Verilog Simulator for TkGate\n",VGSIM_NAME,VGSIM_VERSION);
    vgio_comment("%s\n",VGSIM_COPYRIGHT);
    vgio_comment("  %s comes with ABSOLUTELY NO WARRANTY;  run with -l switch\n",VGSIM_NAME);
    vgio_comment("  for license and warranty details.  Report problems to hansen@tkgate.org\n");
    vgio_comment("  [compiled on %s %s]\n",__DATE__,__TIME__);
  }

  /*
   * Load design files specified on the command line.
   */
  VGSim_loadFiles(&vgsim,&load_files);

  /*
   * If no timescale specified, assume a default.
   */
  if (vgsim.vg_timescale.ts_units == 0) {
    VGSim_useDefaultTimescale(&vgsim);
  }


  /*
   * When thyme is executed by tkgate, the circuit files are passed indirectly as
   * temporary files and after loading the files, it is ok to delete them.  The '-D n'
   * switch can be used to delete the files after reading, but in order to prevent
   * accidental deletion of user files, we require that a hash code computed from
   * the file name matches the value given to the -D switch.  This value can be computed
   * within tkgate, but it is unlikely that a correct value can be given accidentally.
   */
  if (delete_on_load) {
    for (le = List_first(&load_files);le;le = List_next(&load_files,le)) {
      char *fileName = (char*)ListElem_obj(le);
      unsigned fileHash = computestrhash(fileName);
      if (fileHash == delete_hash_code)
	unlink(fileName);
    }
  }

  /*
   * If the -s switch was used, we are just doing a scan, so just do the scan
   * and exit.
   */
  if (scan_mode) {
    VGSim_displayModuleData(&vgsim);
    exit(0);
  }

  /*
   * If there are modules to display, display them and exit.
   */
  if (List_numElems(&show_modules) > 0) {
    VGSim_printModules(&vgsim,&show_modules);
    exit(0);
  }

  /*
   * No explicit top-module was given, try the default top module.
   */
  if (!vgsim.vg_topModuleName)
    vgsim.vg_topModuleName = vgsim.vg_defaultTopModuleName;

  /*
   * Either all modules must have a `timescale or none of them should have
   * a `timescale.  Do this check here.
   */
  if (vgsim.vg_haveTScount != 0
      && vgsim.vg_haveTScount != Hash_numElems(&vgsim.vg_modules)) {
    errorFile(&curPlace,ERR_TIMESCALEAN);
    exitIfError();
    return 1;
  }

  /*
   * If we do not have a top module name, we must exit. 
   */
  if (!vgsim.vg_topModuleName) {
    errorFile(&curPlace,ERR_NOTOP, "<none>");
    exitIfError();
    return 1;
  }

  startSimulation(vgsim.vg_topModuleName,warning_mode,&load_scripts,initTimeSpec);

  return 0;
}

