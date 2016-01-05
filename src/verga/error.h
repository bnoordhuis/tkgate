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
#ifndef __error_h
#define __error_h

/*****************************************************************************
 * placemode_t - Type of place reporting
 *****************************************************************************/
typedef enum {
  PM_FILE = 0x1,
  PM_LINE = 0x2,
  PM_MODULE = 0x4,
  PM_MODLINE = 0x8,
  PM_PRETTY = 0x10
} placemode_t;

typedef enum {
  ERR_OK,		
  WRN_INPORT,		
  WRN_OUTPORT,		
  WRN_FLOATNET,		
  ERR_MEMADDR,		
  ERR_MEMBITS,		
  ERR_BADARRAYUSG,	
  ERR_BADARRAYLHS,	
  ERR_BADCLOSE,	
  ERR_BADAUTORNG,	
  ERR_BADGATERNG,	
  ERR_DIVZERO,		
  ERR_BADEDGEEVENT,	
  ERR_ASGNEVENT,	
  ERR_PROTTASK,	
  ERR_NEEDIDENT,	
  ERR_BADOP,		
  ERR_NOREAD,		
  ERR_MEMFILE,		
  ERR_WRONGMOD,	
  ERR_NOTPARM,		
  ERR_BADADDR,		
  ERR_BADADDRSPEC,	
  ERR_BADCHAR,		
  ERR_BADEVENT,	
  ERR_BADCONSTOP,	
  ERR_BADXOP,		
  ERR_NETREDEF,	
  ERR_BADINOUT,	
  ERR_MODUNDEF,	
  ERR_BADASGNLHS,	
  ERR_BADLHS,		
  ERR_BADOUT,		
  ERR_NOTREG,		
  ERR_PORTMIX,		
  ERR_REPCASE,		
  ERR_NOMEM,		
  ERR_BADCMD,		
  ERR_CMDNOTNET,	
  ERR_MEMNONBLK,	
  ERR_NOTPPORT,	
  ERR_REDEFP,		
  ERR_PORTNOTDEF,	
  ERR_NOCONN,		
  ERR_MULTCONN,	
  ERR_PORTCOUNT,	
  ERR_BADPRTRANGE,	
  ERR_BADARRAYRNG,	
  ERR_LHSNOTREG,	
  ERR_CMDMODREDEF,	
  ERR_REDEF,		
  ERR_PROTTASKSTOP,	
  ERR_NOTMEM,		
  ERR_CMDNOTMEM,	
  ERR_SYNTAX,		
  ERR_BADHIER,
  ERR_TOOFEWPP,	
  ERR_OPENTOOMANY,	
  ERR_TOOMANYPP,	
  ERR_NOTOP,		
  ERR_BADOPEN,		
  ERR_CMDNOMOD,	
  ERR_BADEVENTNET,	
  ERR_NOTASK,		
  ERR_NOTDEF,		
  ERR_BADRANGE,	
  ERR_GATEUNIMP,	
  ERR_USAGE,		
  ERR_CLSDWRITE,	
  ERR_PRIMPTCOUNT,	
  ERR_CMDARGS,		
  ERR_YYERROR,		
  ERR_TASKARGS,	
  ERR_BADSTART,	
  ERR_BADSTOP,		
  ERR_TIMING,		
  ERR_SPECTASKUSG,
  ERR_BADSPECTASK,
  ERR_NEEDEDGE,
  ERR_NOIFDEF,
  ERR_BADSPECLVAL,
  WRN_DIRECTCONN,
  ERR_PATHDMEM,
  ERR_PATHDINOUT,
  ERR_PATHDLOOP,
  ERR_PATHDREASSGN,
  ERR_PATHDNOTIN,
  ERR_PATHDCOND,
  ERR_TASKREDEF,
  ERR_TASKASFUNC,
  ERR_FUNCASTASK,
  ERR_TASKBADTYPE,
  ERR_TASKBADPORT,
  ERR_TIMESCALEU,
  ERR_TIMESCALEN,
  ERR_TIMESCALES,
  ERR_TIMESCALEX,
  ERR_TIMESCALEAN,
  ERR_PARAMMIX,
  ERR_BADPORTTYPE,	
  ERR_BADVALUE,
  ERR_NONEXPCTL,	
  ERR_NONSTATCTL,	
  ERR_MODREDEF,
  ERR_IE_NONET,	
  ERR_IE_NOOP,		
  ERR_IE_BADEXP,	
  ERR_IE_BADVAR,	
  ERR_IE_BADSTATE,	
  ERR_IE_RETURN,	
  ERR_IE_TASK,
  ERR_IE_UNIMP,
  ERR_UNKNOWN,
} errorcode_t;


typedef struct ErrorDescriptor_str {
  errorcode_t	ed_code;		/* Error code number */
  int		ed_level;		/* Error level */
  char		*ed_tag;		/* Message tag for error */
  char		*ed_text;		/* Default text of error */
  int		ed_numArgs;		/* Number of arguments */
} ErrorDescriptor;

/*****************************************************************************
 *
 * Place - A "Place" is used to represent a place in a source file with
 * information about the line number,
 *
 *****************************************************************************/
typedef struct Place_str {
  char		*p_fileName;		/* Name of file */
  char		*p_moduleName;		/* Name of module */ 
  int		p_mtag;			/* non-zero if in tagged module */
  ModuleItem	*p_mitem;		/* Module item if appropriate */
  int		p_lineNo;		/* Line number within file */
  int		p_modLineNo;		/* Line number within module */
} Place;

/*****************************************************************************
 * Place member functions
 *****************************************************************************/
void Place_init(Place *p, const char *fileName);
void Place_copy(Place *p, Place *srcP);
void Place_incLineno(Place *p, int delta);
void Place_startModule(Place *p, const char *modName);
void Place_endModule(Place *p);
void Place_startMTag(Place *p, const char *modName);
void Place_endMTag(Place *p);
char *Place_report(Place *p, const char *etype, const char *netName,char *buf);
void Place_setMode(placemode_t);
void Place_setCurrent(Place*);
void Place_resetModLine(Place*);
Place *Place_getCurrent();

/*****************************************************************************
 * Error reporting functions
 *****************************************************************************/
void errorCmd(errorcode_t,...);
void errorRun(errorcode_t,...);
void errorFile(Place*,errorcode_t,...);
void errorModule(ModuleDecl*,Place*,errorcode_t,...);
void errorNet(ModuleDecl *m,const char *net,Place*,errorcode_t,...);
//void errorNet(ModuleDecl*,NetDecl*,Place*,errorcode_t,...);

void dumpErrorMessages();
void initErrorMessages();
void flushErrors();

#endif
