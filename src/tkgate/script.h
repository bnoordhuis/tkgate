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
#ifndef __script_h
#define __script_h

#define SCRIPT_ERRORBLOCKSIZE 32	/* Error list array allocation block size */

/* Script states */
#define SCS_GO	 	0	/* The script it currently executing. */
#define SCS_PAUSE	1	/* The script is paused. */
#define SCS_STOP	2	/* The script has terminated. */
#define SCS_DISABLE	0x101	/* The script is disabled */
#define SCS_UNLOADED	0x102	/* The script has not been loaded to simulator */
#define SCS_INVALID	0x103	/* The script file was invalid. */
#define SCS_UNLOADED_F	0x100	/* Flag for all states that are not loaded */

/*****************************************************************************
 *
 * Simulator breakpoint 
 *
 *****************************************************************************/
struct Script_str {
  char		*s_fileName;	/* Name of script file */
  int		s_id;		/* Identifier of the script invocation */
  int		s_state;	/* State of the script invocation */
  int		s_numErrors;	/* Number of reported errors */
  char		**s_errors;	/* List of error messages for this script */
};

int ScriptTable_insert(NHash *stab,int idx,const char *fileName);
void ScriptTable_delete(NHash *stab,int idx);
void ScriptTable_load(NHash *stab,int idx);
void ScriptTable_unload(NHash *stab,int idx);
void ScriptTable_loadAll(NHash *stab);
void ScriptTable_pause(NHash *stab,int idx);
void ScriptTable_stop(NHash *stab,int idx);
void ScriptTable_error(NHash *stab,int id,const char *message);
void ScriptTable_flush(NHash *stab);

void delete_Script(Script *bp);
Script *new_Script(int idx,int state, const char *fileName);

#endif
