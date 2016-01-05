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
#ifndef __object_h
#define __object_h

/*
 * Object manager modes
 */
#define OM_DISABLED		0	/* No object management */
#define OM_ENABLED		1	/* Undoable object management */
#define OM_START		2	/* Start doing object management */

/*
 * Frame flags
 */
#define FF_TRANSPARENT		0x1	/* Transparent frames are not directly visible */
#define FF_STICKY		0x2	/* Sticky frames stick together */
#define FF_BACKGROUND		0x4	/* Background frames do not clear redo stack */


void *ob_malloc(int,const char*);	/* Allocate an undoable object */
void *ob_calloc(int,int,const char*);	/* Allocate an undoable object */
void *ob_realloc(void*,int);		/* Reallocate an undoable object */
void ob_free(void*);			/* Free an undoable object */
char *ob_strdup(const char*);		/* Duplicate a string object */

void ob_init();				/* Initialize object handling */
void ob_mode(int);			/* Set object handling mode */
unsigned ob_get_mode();			/* Get object handling mode */
void ob_flush_undo();			/* Flush all undo data */
void ob_touch(void*);			/* Mark an object as touched */
void ob_undo(int);			/* Undo all changes in change group */
void ob_redo(int);			/* Redo all changes in change group */
void ob_clear();			/* Clear all undo/redo data */

void ob_begin_frame(const char*);	/* Begin a set of changes */
void ob_begin_framef(const char*,unsigned);	/* Begin a set of changes */
void ob_append_frame(const char*);	/* Append a set of changes */
void ob_suggest_name(const char*);	/* Suggest a new name for current frame */
void ob_end_frame();			/* End a set of changes */
void ob_unstick();			/* Un stick the top undo frame */

int ob_getUndoList(const char **L,int N);	/* Get list of undo commands */
int ob_getRedoList(const char **L,int N);	/* Get list of undo commands */

#endif
