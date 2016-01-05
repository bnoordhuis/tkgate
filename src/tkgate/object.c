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
/*###########################################################################*
 *
 * The operations declared in this file are used to implement the
 * undo/redo features of tkgate. The basic approach is to make backup
 * copies of all objects which are modified over the course of executing
 * a command.  From the perspective of the undo manager, a command is all
 * of the operations that occur within a frame,  A frame is started by
 * a call to ob_begin_frame() (or related frame starter) and ended by
 * a call to ob_end_frame().
 *
 * Guidelines for frame names.
 * 1) Frame names that begin with a '-' are weak, frame names that begin
 *    with a '+' are strong, and frame names with no qualifier are normal. 
 * 2) The ob_suggest_name() function can be used to override the name of
 *    the current frame, but only if the supplied name is "stronger" than
 *    the current name.  This can be used to delay naming of a frame until
 *    after the frame has started.
 *
 * Guidelines for code modification.
 * 
 * 1) All objects should be created with ob_malloc(), ob_calloc(),
 *    ob_realloc() or ob_stdrup().
 * 2) All objects should be freed with ob_free()
 * 3) ob_touch() should be called on all objects before modification.
 * 4) The code for one undoable action should be enclosed in ob_begin_frame("name")
 *    and ob_end_frame() calls.  Alternatively, ob_append_frame("name") can be
 *    used in place of ob_begin_frame to cause the new frame to be appended to
 *    the previous frame.  For example, individual scrolling steps are appended into
 *    a single undoable scrolling action.
 * 
 * Here are some examples of how to use ob_touch()
 *
 *    Example 1)
 * 	ob_touch(g);
 * 	g->xpos += 10;
 * 
 *    Example 2)
 * 	ob_touch(g);
 * 	g->wires[i] = w;
 * 
 * 	g->wires[] is an static array declared as part of the 'g' (GCElement) object
 *      so no ob_touch() should be used.
 * 
 *    Example 3)
 * 	ob_touch(w->nodes);
 * 	w->nodes->x = x;
 * 
 *    Example 4)
 * 	ob_touch(w->nodes);
 * 	set_position(&w->nodes->x,&w->nodes->y);
 * 
 *    Example 5)
 * 	ob_touch(g)
 * 	g->u.basic.extbar = n;
 * 
 * 	u.basic.extbar is part of g (GCElement).
 *
 * Any memory that is allocated and freed wholy within the execution of a command
 * can use the standard malloc()/free() to reduce the amount of data that needs
 * to be backed up for a command.
 * 
 *###########################################################################*/
#include "tkgate.h"

#define OBJECT_DEBUG		0	/* Debugging display of object handling */ 
#define OBJECT_SHOWMODS		0	/* More debugging display of object handling */ 
#define OBJECT_STOPONERROR	0	/* Abort as soon as we hit an object error */
#define OBJECT_MEGADEBUG	0	/* Print out all calls to malloc/free */
#define OBJECT_HARSHMEM		0	/* Put bogus data in malloc/free'ed memory */
#define OBJECT_TRACKUSAGE	1	/* Track memory usage */

#define OBJECT_UNNAMED		"Action"	/* Name to use when we get an action without a name */

void undo_sync(int is_after);


/*****************************************************************************
 * Object flags.
 *****************************************************************************/
#define OF_DELETED		0x1	/* Object is marked for deletion */
#define OF_NOUNDO		0x2	/* Object can not be undone */ 

/*****************************************************************************
 * Object change codes
 *****************************************************************************/
#define OC_MALLOC		1
#define OC_FREE			2
#define OC_MODIFY		3

/*****************************************************************************
 *
 * typedef classes for object handler (used only in this file).
 *
 *****************************************************************************/
typedef struct objectmanager_str ObjectManager;
typedef struct objectframe_str ObjectFrame;
typedef struct objectchange_str ObjectChange;
typedef struct object_str Object;

/*****************************************************************************
 * The object manager.
 *****************************************************************************/
struct objectmanager_str {
  unsigned	om_started;		/* Object management has been started */
  unsigned	om_mode;		/* Current manager mode */
  unsigned	om_mark;		/* Current mark value */
  int		om_numUndo;		/* Number of "visible" undo frames */
  List		om_undo;		/* The undo stack */
  List		om_redo;		/* The redo stack */
  ObjectFrame	*om_cur;		/* The current frame */
};

/*****************************************************************************
 * ObjectFrame - A collection of modifications to objects that constitute
 * a single undoable action.
 *****************************************************************************/
struct objectframe_str {
  unsigned	of_flags;		/* Frame flags */
  char		*of_name;		/* Command name of this frame */
  int		of_level;		/* Level of frame */
  ObjectFrame	*of_next;		/* The next item on the stack */
  int		of_num_changes;		/* Number of changes */
  ObjectChange	*of_changes;		/* Changes in this frame */
};

/*****************************************************************************
 * ObjectChange - Represents the changes that occured to a single object
 * within a frame.
 *****************************************************************************/
struct objectchange_str {
  unsigned	oc_type;		/* Type of change */
  Object	*oc_ptr;		/* Pointer to object location */
  Object	*oc_backup;		/* Backup copy of object */
  ObjectChange	*oc_next;		/* Next in list of changes */
};

/*****************************************************************************
 * Object - This structure contains "hidden" information about an object that
 * is stored in front of each object returned by ob_malloc()
 *****************************************************************************/
struct object_str {
  const char	*o_type;		/* Object type */
  unsigned	o_size;			/* Size of this object */
  unsigned	o_mark;			/* Object change mark */
  unsigned	o_flags;		/* Change type */
};

/*****************************************************************************/

int ob_max_undo = 10;

static ObjectManager objm;

struct {
  unsigned	ob;		/* Allocation of ob_ memory */ 
  unsigned	actual;		/* Actual outstanding memory usage */ 
} memusage = {0,0};


#if OBJECT_MEGADEBUG
void *xmalloc(size_t s)
{
  void *o = malloc(s);
  printf("%p = malloc(%d)\n",o,s);
  return o;
}

void xfree(void *o)
{
  printf("free(%p)\n",o);
  free(o);
}

#define malloc(s) xmalloc(s)
#define free(o) xfree(o)
#endif

/*****************************************************************************
 *
 * Initialize undo/redo object management.  Must be called on program start
 * up before any calls to object handling functions.
 *
 *****************************************************************************/
void ob_init()
{
#if OBJECT_DEBUG
  printf("ob_init()\n");
#endif
  objm.om_started = 0;
  objm.om_mode = OM_DISABLED;
  objm.om_mark = 1;
  List_init(&objm.om_undo);
  List_init(&objm.om_redo);
  objm.om_numUndo = 0;
  objm.om_cur = 0;
}

/*****************************************************************************
 *
 * Create a new object frame.
 *
 *****************************************************************************/
ObjectFrame *new_ObjectFrame(const char *name,unsigned flags)
{
  ObjectFrame *o = (ObjectFrame*) malloc(sizeof(ObjectFrame));

  memusage.actual += sizeof(ObjectFrame);
  memusage.actual += name ? strlen(name)+1 : 0;

  o->of_flags = flags;
  o->of_name = name ? strdup(name) : 0;
  o->of_next = 0;
  o->of_level = 1;
  o->of_num_changes = 0;
  o->of_changes = 0;

  return o;
}

/*****************************************************************************
 *
 * Check to see if this is an "invisible" frame.  An invisible frame
 * is one that is explicitly marked "transparent", is weakly named,
 * or has no object modifications.  Invisible frames do not appear
 * in the undo/redo lists, and are automatically undone/redone when
 * applying doing an undo or redo.
 *
 *****************************************************************************/
static int ObjectFrame_isInvisible(ObjectFrame *F)
{
  if ((F->of_flags & FF_TRANSPARENT)) return 1;
  if (F->of_name && *F->of_name == '-') return 1;
  if (!F->of_changes) return 1;
  return 0;
}

/*****************************************************************************
 *
 * Discard an edit frame and reclaim memory for it.
 *
 *****************************************************************************/
static void ob_discard_frame(ObjectFrame *F)
{
  ObjectChange *C,*NC;

  for (C = F->of_changes;C;C = NC) {
    NC = C->oc_next;
    if (C->oc_type == OC_FREE && C->oc_ptr) {
      free(C->oc_ptr);
    }
    if (C->oc_backup) {
      free(C->oc_backup);
      memusage.actual -= C->oc_ptr->o_size;
    }
    free(C);
    memusage.actual -= sizeof(ObjectChange);
  }
  free(F);
  memusage.actual -= sizeof(ObjectFrame);
}

/*****************************************************************************
 *
 * Pop an undo frame off the stack and return it.
 *
 *****************************************************************************/
static ObjectFrame *ob_popUndoFrame()
{
  ObjectFrame *of = List_popHead(&objm.om_undo);

  if (of && !ObjectFrame_isInvisible(of)) objm.om_numUndo--;

  return of;
}

/*****************************************************************************
 *
 * Push an undo frame on the stack.
 *
 *****************************************************************************/
static void ob_pushUndoFrame(ObjectFrame *of)
{
  if (!ObjectFrame_isInvisible(of)) objm.om_numUndo++;
  List_addToHead(&objm.om_undo,of);

  while (objm.om_numUndo > ob_max_undo) {
    of = List_popTail(&objm.om_undo);
    if (!ObjectFrame_isInvisible(of)) objm.om_numUndo--;
    ob_discard_frame(of);
  }
}


/*****************************************************************************
 *
 * Add a change descriptor of the specified type for the specified object.
 *
 *****************************************************************************/
static void ObjectFrame_add_change(ObjectFrame *F,int chg_type,Object *o)
{
  ObjectChange *C = (ObjectChange*) malloc(sizeof(ObjectChange));

  memusage.actual += sizeof(ObjectChange);

  C->oc_type = chg_type;
  C->oc_ptr = o;
  C->oc_backup = 0;
  C->oc_next = F->of_changes;
  F->of_changes = C;
  F->of_num_changes++;

  switch (chg_type) {
  case OC_MALLOC :
    break;
  case OC_FREE :
    break;
  case OC_MODIFY :
    C->oc_backup = (Object*) malloc(o->o_size);
    memusage.actual += o->o_size;

    memcpy(C->oc_backup,o,o->o_size);
    o->o_mark = objm.om_mark;			/* Mark the object as saved */
    break;
  }
}

/*****************************************************************************
 *
 * Set the object handling mode.
 *
 *****************************************************************************/
void ob_mode(int m)
{
  if (!objm.om_started) {
    if (m == OM_START)
      m = OM_ENABLED;
    else
      return;
  }
  objm.om_started = 1;

  if (objm.om_mode != m)
    ob_clear();

  objm.om_mode = m;

#if OBJECT_DEBUG
  printf("ob_mode(%d)\n",m);
#endif
}

/*****************************************************************************
 *
 * Get the current object handling mode
 *
 *****************************************************************************/
unsigned ob_get_mode()
{
  return objm.om_mode;
}


/*****************************************************************************
 *
 * Clear all undo actions and do the deferred free() calls.
 *
 *****************************************************************************/
static void ob_clear_list(List *L)
{
  ListElem *E;

  for (E = List_first(L);E;E = List_next(L,E)) {
    ObjectFrame *F = (ObjectFrame*)ListElem_obj(E);
    ob_discard_frame(F);
  }
  List_flush(L);
}

/*****************************************************************************
 *
 * Clear all undo/redo objects on the undo stack.
 *
 *****************************************************************************/
void ob_clear()
{
  ob_clear_list(&objm.om_undo);
  ob_clear_list(&objm.om_redo);
  objm.om_numUndo = 0;
}

/*****************************************************************************
 *
 * Unapply the changes in a frame, and return a new frame which
 * is the inverse of the applied frame.
 *
 *****************************************************************************/
ObjectFrame *ob_apply(ObjectFrame *f)
{
  ObjectFrame *inv_f = new_ObjectFrame(f->of_name,f->of_flags);
  ObjectChange *oc;

#if OBJECT_DEBUG
  printf("ob_apply(%s) -> %p\n",f->of_name, inv_f);
#endif

  /*
   * Inverse frames should be marked as level 0.
   */
  inv_f->of_level = 0;


  for (oc = f->of_changes;oc;oc = oc->oc_next) {
    switch (oc->oc_type) {
    case OC_MALLOC :
      ObjectFrame_add_change(inv_f,OC_FREE,oc->oc_ptr);
      oc->oc_ptr->o_flags |= OF_DELETED;	/* Undo the malloc() */
      break;
    case OC_FREE :
      ObjectFrame_add_change(inv_f,OC_MALLOC,oc->oc_ptr);
      oc->oc_ptr->o_flags &= ~OF_DELETED;	/* Undo the free() */
      break;
    case OC_MODIFY :
      ObjectFrame_add_change(inv_f,OC_MODIFY,oc->oc_ptr);
      memcpy(oc->oc_ptr,oc->oc_backup,oc->oc_backup->o_size);
      break;
    }
  }

  free(f);
  memusage.actual -= sizeof(ObjectFrame);

  return inv_f;
}

/*****************************************************************************
 *
 * Synchronize states of undo buttons
 *
 *****************************************************************************/
void ob_sync_undo_buttons()
{
  DoTcl("Menu::setFlags %s U %s R",
	(ob_getUndoList(0,0) != 0 ? "-set" : "-clear"),
	(ob_getRedoList(0,0) != 0 ? "-set" : "-clear"));
}

/*****************************************************************************
 *
 * Undo last 'count' operations.
 *
 *****************************************************************************/
void ob_undo(int count)
{
  undo_sync(0);				/* Synchronize Tcl/Tk undo/redo list */
  while (count-- > 0) {
    ObjectFrame *of;

    if (List_numElems(&objm.om_undo) == 0) break;
    
    of = (ObjectFrame*) ob_popUndoFrame();
    if (ObjectFrame_isInvisible(of)) count++;			/* Do the next frame too */
    of = ob_apply(of);
    if (of) List_addToHead(&objm.om_redo,of);
  }
  undo_sync(1);				/* Synchronize Tcl/Tk undo/redo list */
  ob_sync_undo_buttons();
}

/*****************************************************************************
 *
 * Redo last 'count' operations.  Before we do the redo operation, we must
 * undo any background frames to ensure we are in the correct state to do the
 * redo.
 *
 *****************************************************************************/
void ob_redo(int count)
{
  ObjectFrame *of;

  undo_sync(0);

  /*
   * Undo any background frames on the undo stack.
   */
  while (List_numElems(&objm.om_undo) == 0) {
    of = (ObjectFrame*) List_nth(&objm.om_undo,0);
    if ((of->of_flags & FF_BACKGROUND)) {
#if OBJECT_DEBUG
      printf("ob_redo: undoing background %s\n",of->of_name);
#endif
      of = (ObjectFrame*) List_popHead(&objm.om_undo);
      of = ob_apply(of);
      ob_discard_frame(of);
    } else
      break;
  }


  while (count-- > 0) {
    if (List_numElems(&objm.om_redo) == 0) break;
    
    of = (ObjectFrame*) List_popHead(&objm.om_redo);
    if (ObjectFrame_isInvisible(of)) count++;			/* Do the next frame too */
    of = ob_apply(of);
    if (of) ob_pushUndoFrame(of);
  }
  undo_sync(1);
  ob_sync_undo_buttons();
}

/*****************************************************************************
 *
 * Get the list of undo actions and place them in the array L which
 * contains at least N elements.  Returns the actual number of undo
 * actions.
 *
 *****************************************************************************/
int ob_getUndoList(const char **L,int N)
{
  ListElem *E;
  int c = 0;

#if OBJECT_DEBUG
  printf("getUndoList:\n");
#endif
  for (E = List_first(&objm.om_undo);E;E = List_next(&objm.om_undo,E)) {
    ObjectFrame *F = (ObjectFrame*)ListElem_obj(E);

#if OBJECT_DEBUG
    printf("  %2d: %8s - level=%d  changes=%d  invisible=%d -\n",c,F->of_name,F->of_level,F->of_num_changes,
	   ObjectFrame_isInvisible(F));
#endif

    if (ObjectFrame_isInvisible(F)) continue;			/* Ignore this frame */
    if (c < N) {
      L[c] = F->of_name ? F->of_name : OBJECT_UNNAMED;
      if (*L[c] == '+' || *L[c] == '-') L[c]++;
    }
    c++;
  }

  if (objm.om_numUndo != c) {
    printf("[unexpected frame count (numUndo=%d c=%d)]\n",objm.om_numUndo,c);
  }

  return c;
}

/*****************************************************************************
 *
 * Get the list of redo actions and place them in the array L which
 * contains at least N elements.  Returns the actual number of undo
 * actions.
 *
 *****************************************************************************/
int ob_getRedoList(const char **L,int N)
{
  ListElem *E;
  int c = 0;

#if OBJECT_DEBUG
  printf("getRedoList:\n");
#endif
  for (E = List_first(&objm.om_redo);E;E = List_next(&objm.om_redo,E)) {
    ObjectFrame *F = (ObjectFrame*)ListElem_obj(E);

#if OBJECT_DEBUG
    printf("  %2d: %8s - level=%d  changes=%d  invisible=%d -\n",c,F->of_name,F->of_level,F->of_num_changes,
	   ObjectFrame_isInvisible(F));
#endif

    if (ObjectFrame_isInvisible(F)) continue;			/* Ignore this frame */
    if (c < N) {
      L[c] = F->of_name ? F->of_name : OBJECT_UNNAMED;
      if (*L[c] == '+' || *L[c] == '-') L[c]++;
    }
    c++;
  }
  return c;
}

/*****************************************************************************
 *
 * Allocates memory for an undoable object and tags it with a string for its
 * type.  The string passed is used as is and may not be modified or freed.
 * by the calling program.  The string is only for informational purposes and
 * a null may be passed in its place.
 *
 * Typical usage is:
 *
 * Fooby *f = (Fooby*) ob_malloc(sizeof(Fooby),"Fooby");
 * 
 *****************************************************************************/
void *ob_malloc(int s,const char *name)
{
  Object *o = (Object*) malloc(s+sizeof(Object));
  memusage.actual += s + sizeof(Object);

  memusage.ob += s+sizeof(Object);

  o->o_size = s+sizeof(Object);
  o->o_type = name;
  o->o_mark = objm.om_mark;				/* Objects start out marked */
  o->o_flags = 0;

#if OBJECT_HARSHMEM
  memset(o+1,0xa7,s);
#endif

#if OBJECT_SHOWMODS
  printf("[malloc %s::%p (%d)]\n",(o->o_type?o->o_type:"?"),o,s);
#endif

  if (objm.om_mode == OM_DISABLED || !objm.om_cur)
    return o+1;

  ObjectFrame_add_change(objm.om_cur,OC_MALLOC,o);

  return o+1;
}

/*****************************************************************************
 *
 * Undoable object replacement for calloc()
 *
 *****************************************************************************/
void *ob_calloc(int n,int s,const char *name)
{
  void *o = ob_malloc(s*n,name);
  memset(o,0,s*n);
  return o;
}

/*****************************************************************************
 *
 * Undoable object replacement for realloc()
 *
 *****************************************************************************/
void *ob_realloc(void *vo,int s)
{
  Object *o = ((Object*)vo)-1;
  void *c;
  if (o->o_size-sizeof(Object) >= s) return vo;

  c = ob_malloc(s,o->o_type);
  memcpy(c,vo,imin(o->o_size,s));
  ob_free(vo);

  return c;
}

/*****************************************************************************
 *
 * Undoable object replacement for free()
 *
 *****************************************************************************/
void ob_free(void *vo)
{
  Object *o = ((Object*)vo)-1;
  int do_ack_frame = 0;

  memusage.ob -= o->o_size;

#if OBJECT_SHOWMODS
  printf("[free %s::%p (%d)]\n",(o->o_type?o->o_type:"?"),o,o->o_size);
#endif

#if OBJECT_HARSHMEM
  ob_touch(vo);
  memset(vo,0xa7,o->o_size-sizeof(Object));
#endif

  if ((o->o_flags & OF_DELETED)) {
    printf("[ob_free of deleted %s object!]\n",o->o_type);
    assert(!(o->o_flags & OF_DELETED));
  }

  if (objm.om_mode == OM_DISABLED) {
    free(o);
    memusage.actual -= sizeof(Object) + o->o_size;
    return;
  }
  if (!objm.om_cur) {
    logError(ERL_ERROR,"object freed outside frame context");
    ob_begin_framef("BadFree",FF_STICKY);
    do_ack_frame = 1;
#if OBJECT_STOPONERROR
    abort();
#endif
  }

  ObjectFrame_add_change(objm.om_cur,OC_FREE,o);

  if (do_ack_frame)
    ob_end_frame();
}

/*****************************************************************************
 *
 * Suggest an alternate name for the current frame.
 *
 *****************************************************************************/
void ob_suggest_name(const char *name)
{
  if (objm.om_mode == OM_DISABLED) return;
  if (!objm.om_cur) return;
  if (!name) return;

  if (objm.om_cur->of_name) {
    int x1 =(*objm.om_cur->of_name == '+') ? 1 : (*objm.om_cur->of_name == '-' ? -1 : 0);
    int x2 =(*name == '+') ? 1 : (*name == '-' ? -1 : 0);

    if (x2 <= x1) {
#if OBJECT_DEBUG
      printf("ob_suggest_name(%s) [ignored]\n",name);
#endif
      return;
    }
    free(objm.om_cur->of_name);
    memusage.actual -= strlen(objm.om_cur->of_name)+1;
  }

#if OBJECT_DEBUG
  printf("ob_suggest_name(%s) [updated]\n",name);
#endif
  objm.om_cur->of_name = strdup(name);
  memusage.actual += strlen(name)+1;
}

/*****************************************************************************
 *
 * Unset the sticky flag on the top object of the undo stack.
 *
 *****************************************************************************/
void ob_unstick()
{
  if (objm.om_mode == OM_DISABLED) return;

  if (List_first(&objm.om_undo)) {
    ObjectFrame *undo_top = (ObjectFrame*) ListElem_obj(List_first(&objm.om_undo));
    undo_top->of_flags &= ~FF_STICKY;
  }
}


/*****************************************************************************
 *
 * Begin a set of undoable changes with the specified name.  Options can be
 * specified in the 'flags' parameter.   If the FF_STICKY flag is present and
 * the frame on the top of the stack is a sticky frame, then the new set of
 * actions will be appended to the current frame rather than creating a new
 * frame.  This is useful for actions such as dragging an object to prevent
 * each individual movement from becoming a separate frame.
 *
 *****************************************************************************/
void ob_begin_framef(const char *name,unsigned flags)
{
  ObjectFrame *undo_top = 0;

  if (objm.om_mode == OM_DISABLED) return;

  /*
   * If this is not a background frame, then clear the redo stack, and clear
   * the background flag of any other frames on the undo stack.  We assume that
   * any background frames are on the top of the stack.
   */
  if (!(flags & FF_BACKGROUND)) {
    ListElem *E;

    ob_clear_list(&objm.om_redo);

    for (E = List_first(&objm.om_undo);E;E = List_next(&objm.om_undo,E)) {
      ObjectFrame *F = (ObjectFrame*)ListElem_obj(E);
      if ((F->of_flags & FF_BACKGROUND))
	F->of_flags &= ~FF_BACKGROUND;
      else
	break;
    }
  }


  if (List_first(&objm.om_undo))
    undo_top = (ObjectFrame*) ListElem_obj(List_first(&objm.om_undo));

  if ((flags & FF_STICKY) && undo_top && (undo_top->of_flags & FF_STICKY)) {
    ob_append_frame(name);
    return;
  }

  if (objm.om_cur) {
    objm.om_cur->of_level++;
#if OBJECT_DEBUG
    printf("(%d) ob_begin_frame(%s, 0x%x) in %s\n",objm.om_cur->of_level,name,flags,objm.om_cur->of_name);
#endif
    return;
  }

  objm.om_mark++;			/* Clears all marked objects */
  objm.om_cur = new_ObjectFrame(name,flags);
#if OBJECT_DEBUG
  printf("(%d) ob_begin_frame(%s, 0x%x)\n",objm.om_cur->of_level,name,flags);
#endif
}

/*****************************************************************************
 *
 * Append a frame to the current top-level frame.  If there is no current frame
 * then we start a new frame.
 *
 *****************************************************************************/
void ob_append_frame(const char *name)
{
  if (objm.om_mode == OM_DISABLED) return;

  ob_clear_list(&objm.om_redo);

  if (objm.om_cur) {
    ob_begin_frame(name);
    return;
  }

  objm.om_cur = (ObjectFrame*) ob_popUndoFrame();

  if (!objm.om_cur) {
    ob_begin_frame(name);
    return;
  }

  objm.om_cur->of_level++;
  if (name) {
    if (objm.om_cur->of_name) {
      free(objm.om_cur->of_name);
      memusage.actual -= strlen(objm.om_cur->of_name)+1;
    }
    objm.om_cur->of_name = strdup(name);
    memusage.actual += strlen(name)+1;
  }
#if OBJECT_DEBUG
  printf("(%d) ob_append_frame(%s)\n",objm.om_cur->of_level,objm.om_cur->of_name);
#endif
}

/*****************************************************************************
 *
 * Begin a frame with default flag values.
 *
 *****************************************************************************/
void ob_begin_frame(const char *name)
{
  ob_begin_framef(name,0);
}

/*****************************************************************************
 *
 * End a frame.
 *
 *****************************************************************************/
void ob_end_frame()
{
  if (objm.om_mode == OM_DISABLED) return;

  if (!objm.om_cur) {
    printf("ob_error: ob_end_frame with no matching ob_begin_frame()");
    abort();
  }

  if (!(objm.om_cur->of_flags & FF_BACKGROUND)) {
    ob_clear_list(&objm.om_redo);
  }


#if OBJECT_DEBUG
  printf("(%d) ob_end_frame(%s) [%d]\n",objm.om_cur->of_level,objm.om_cur->of_name,objm.om_cur->of_num_changes);
#endif


  if (--objm.om_cur->of_level != 0) 	/* End of nested frame */
    return;

  ob_pushUndoFrame(objm.om_cur);
  objm.om_cur = 0;
  ob_sync_undo_buttons();
}

/*****************************************************************************
 *
 * This function is called on an object before making any changes to it.  If
 * this is the first call to ob_touch in the current frame, then we make a
 * backup copy of the object.  If the object has already been touched in
 * the current frame, we do nothing.
 *
 *****************************************************************************/
void ob_touch(void *vo)
{
  Object *o;
  int do_ack_frame = 0;

  if (objm.om_mode == OM_DISABLED) return;

  if (!vo) return;

  if (!objm.om_cur) {
    ob_begin_framef("BadTouch",FF_STICKY);
    do_ack_frame = 1;
    logError(ERL_ERROR,"object modified outside frame context");
#if OBJECT_STOPONERROR
    abort();
#endif
  }

  o = ((Object*)vo)-1;
  if (o->o_mark == objm.om_mark)
    return;

#if OBJECT_SHOWMODS
  printf("[touch %s::%p (%d)]\n",(o->o_type?o->o_type:"?"),o,o->o_size);
#endif

  if (!o->o_type) {
    printf("[object with no type tag!]\n");
    assert (o->o_type);
  }

  ObjectFrame_add_change(objm.om_cur,OC_MODIFY,o);

  if (do_ack_frame)
    ob_end_frame();
}

/*****************************************************************************
 *
 * An "undoable" version of strdup()
 *
 *****************************************************************************/
char *ob_strdup(const char *s)
{
  int l = strlen(s);
  char *d = ob_malloc(l+1,"char*");
  strcpy(d,s);
  return d;
}
