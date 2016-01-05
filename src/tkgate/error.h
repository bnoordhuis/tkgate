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
 *
 * Embedded script declaration (affects error reporting)
 *
 *****************************************************************************/
struct GEScript_str {
  char		*es_name;		/* Name of script */
  int		es_beginLine;		/* Starting line number */
  int		es_endLine;		/* ending line number (or 0 if still parsing script) */
  GEScript	*es_next;		/* next in list of scripts */
};

/*****************************************************************************
 *
 * List of errors
 *
 *****************************************************************************/
struct ErrorList_str {
  GError *errlist;
  GError *curerr;
  GError **errors;
  int ErrorCount;

  GEScript *el_esList;
  
  int ErrorXPos;
  int ErrorYPos;
};


/*****************************************************************************
 *
 * Simulator error object
 *
 *****************************************************************************/
struct GError_str {
  int 		e_id;		/* Unique identifier for error report */
  int		e_level;	/* Level of error 0=warning, 1=error */
  char		*e_fileName;	/* Name of file in which error occured */
  int		e_fileLine;	/* Line number within file */
  char		*e_modName;	/* Name of module in which error occured */
  int		e_modLine;	/* Line number within module */
  char		*e_message;	/* Error message text */
  char		*e_instName;	/* Name of instance causing error */
  char		*e_portName;	/* Port on instance where error occurred */
  GCElement	*e_inst;	/* Instance associated with error */
  GError	*e_next;	/* Next error in error list */
  GError	*e_last;	/* Previous error in error list */
};

ErrorList *new_ErrorList();

void Error_scriptBegin(const char *name,int line);
void Error_scriptEnd(const char *name,int line);
void Error_report(const char *errText);
void Error_close();
void Error_purge();
EditState *Error_open(GError *err,EditState *es);
void Error_navigateToModule(EditState **es,const char *path);
void Error_decode(char *emsg);

#endif

