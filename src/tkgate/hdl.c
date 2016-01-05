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

    Last edit by hansen on Thu Feb 12 14:50:38 2009
****************************************************************************/
#include "tkgate.h"

static GModuleDef *hdl_active_module = 0;

int hdl_isactive;

/*****************************************************************************
 *
 * Data structure for holding information about a module definition from the
 * HDL text buffer.  This data structure is used only in this file and only
 * as a return value from hdl_findPartitions().
 *
 *****************************************************************************/
typedef struct {
  char *comment_start;		// Start of comments for module.
  char *module_line;		// Pointer to beginning of line with "module" in it.
  char *name;			// Pointer to beginning of module name
  int name_len;			// Number of characters in module name.
  int module_length;		// Total length of module
  char *endmodule_line;		// Pointer to beginning of line with "endmodule" in it.
  char *module_end;		// End of module definition
} HdlModuleData;


/*****************************************************************************
 * 
 * Check to make sure that s1 matches the first n characters of s2.
 *
 *****************************************************************************/
static int ismatch(const char *s1,const char *s2,int n)
{

  if (strlen(s1) != n) return 0;
  return strncmp(s1,s2,n) == 0; 
}

/*****************************************************************************
 *
 * Load HDL text from module definition to editor.
 *
 * Parameters:
 *     M		Module to be loaded into hdl editor.
 *
 *****************************************************************************/
void hdl_load(GModuleDef *M)
{
  hdl_active_module = M;

  if (M->m_text) {
    DoTcl("HdlEditor::enable");
    DoTcl("HdlEditor::clear");

    DoTclL("HdlEditor::loadText",M->m_text,NULL);

    if (GModuleDef_isDataProtected(M))
      DoTcl("HdlEditor::disable");
  } else {
    printf("hdl_load ignored for non-HDL module [%s]\n",M->m_name);
  }

}

/*****************************************************************************
 *
 * Save HDL text in editor to module definition.
 *
 * Parameters:
 *     name		Expected name of module to save to or null for current module.
 *****************************************************************************/
int hdl_save(const char *name)
{
  GModuleDef *M;
  int rvalue = 0;
  char *text;

  if (name)
    M = env_findModule(name);
  else
    M = hdl_active_module;

  if (!M) {
    /* We must have tried to close the HDL editor while not editing an HDL module */
    return rvalue;
  }
  if (M->m_type != MT_TEXTHDL) return 0;

  DoTcl("HdlEditor::dumpText");
  text = TkGate.tcl->result;

#if 0
  printf("hdl_save(%s)\n",M->m_name);
#endif

  if (strcmp(text, M->m_text) == 0) return 0;

  /*
   * Move text from the text widget into tcl variables
   */
  ob_touch(M);
  GModuleDef_saveText(M,text);
  M->m_needScan = 1;

  return rvalue;
}
 
/*****************************************************************************
 *
 * Find the points in a text block at which modules are defined.
 *
 * Parameters:
 *     text			Text block to partition
 *     module_data[]		Array of lines containing 'module' or 'primitive'
 *     n			Number of element in module data array.
 *
 *****************************************************************************/
static int hdl_findPartitions(char *text,HdlModuleData *module_data,int n)
{
  char *token_begin,*token_end;
  char *p = text;
  int r;
  int state = 0; /* 0 = outside module, 1 = inside module */
  int count = 0;


  module_data[0].comment_start = text;

  while ((r = getNextToken(&p, &token_begin, &token_end)) != ST_END) {
    int len = token_end-token_begin;

    if (r != ST_LITERAL) continue;

    if (ismatch("module",token_begin,len) || ismatch("primitive",token_begin,len)) {
      if (state == 1) {
	/* got a module begin while still in a module */
      } else {
	module_data[count].module_line = getLineStart(token_begin,text);

	if (getNextToken(&p, &token_begin, &token_end) == ST_END) break;

	module_data[count].name = token_begin;
	module_data[count].name_len = token_end - token_begin;
	state = 1;
      }
    } else if (ismatch("endmodule",token_begin,len) || ismatch("endprimitive",token_begin,len)) {
      if (state == 0) {
	/* got an endmodule while not in a module definition. */
      } else {
	state = 0;
	module_data[count].endmodule_line = getLineStart(token_begin,text);
	module_data[count].module_end = getLineEnd(token_end);
	if (*module_data[count].module_end == '\n') module_data[count].module_end++;
	module_data[count].module_length = module_data[count].module_end - module_data[count].comment_start;
	if (++count < n) {
	  module_data[count].comment_start = module_data[count-1].module_end;
	} else
	  return count;
      }
    }
  }

  /*
   * No endmodule was found.
   */
  if (state) {
    module_data[count].endmodule_line = p;
    module_data[count].module_end = p;
    module_data[count].module_length = module_data[count].module_end - module_data[count].comment_start;
    count++;
  }

  /*
   * Last module should snarf text to the end of the buffer.
   */
  if (count > 0) {
    module_data[count-1].module_end = p;
    module_data[count-1].module_length = module_data[count-1].module_end - module_data[count-1].comment_start;
  }

  return count;
}

/*****************************************************************************
 * 
 * Replace the name of a module.
 *
 * Parameters:
 *      M		Module to be renamed.
 *      new_name	New module name.  
 *
 * Returns:		Non-zero on error.	
 *
 *****************************************************************************/
int hdl_replaceName(GModuleDef *M,const char *new_name)
{
  int new_len = strlen(new_name);
  char *src_text = M->m_text;
  int src_len = strlen(src_text);
  int name_len;
  char *name_ptr;
  char *name_end;
  int name_offset;
  HdlModuleData module_data[2];
  int npart;
  char *text;

  npart = hdl_findPartitions(M->m_text,module_data,2);
  if (npart != 1) return -1;

  name_ptr = module_data[0].name;
  name_len = module_data[0].name_len;
  name_end = name_ptr + name_len;
  name_offset = (name_ptr-src_text);

  text = (char*) malloc(src_len+new_len-name_len+1);

  memmove(text, src_text, name_offset);
  memmove(text+name_offset, new_name, new_len);
  strcpy(text+name_offset+new_len, name_end);

  GModuleDef_saveText(M,text);
  free(text);

  return 0;
}




/*****************************************************************************
 *
 * Split HDL text in module M into multiple modules.
 *
 * Parameters:
 *      M			Module that this text is a part of.
 *      module_data[]		Array of module data descriptors
 *      module_count		Number of modules.
 *
 *****************************************************************************/
int hdl_splitModules(GModuleDef *M,HdlModuleData *module_data,int module_count)
{
  int i,keep_idx = 0;

  /*
   * Find the module that we are going to keep as part of M's definiton, or use
   * the first module if we can't find a matching name.
   */
  for (i = 0;i < module_count;i++) {
    if (module_data[i].name_len > (STRMAX-10)) {
      /* Found a very long module name. */
      return -1;
    }
    if (ismatch(M->m_name, module_data[i].name, module_data[i].name_len)) {
      keep_idx = i;
    }
  }

  for (i = 0;i < module_count;i++) {
    char modName[STRMAX],*p;
    int count = 0;
    int length = module_data[i].module_length;
    GModuleDef *new_M;

    if (i == keep_idx) continue;

    /*
     * Make sure the target name does not exist.  If it does append a number
     * to the end until we find an available module name.
     */
    strncpy(modName, module_data[i].name, module_data[i].name_len);
    modName[module_data[i].name_len] = 0;
    p = modName + strlen(modName);
    while (env_findModule(modName)) {
      sprintf(p,"_%d",count++);
    }
    new_M = env_defineModule(modName,0);
    new_M->m_type = MT_TEXTHDL;
    GModuleDef_allocText(new_M,length+1);
    memmove(new_M->m_text, module_data[i].comment_start, length);
    new_M->m_text[length] = 0;
    if (count != 0)
      hdl_replaceName(new_M,modName);
  }

  memmove(M->m_text, module_data[keep_idx].comment_start, module_data[keep_idx].module_length);
  M->m_text[module_data[keep_idx].module_length] = 0;
  if (!ismatch(M->m_name,module_data[keep_idx].name,module_data[keep_idx].name_len))
      hdl_replaceName(M,M->m_name);

#if 0
  printf("BEGIN KEEP\n%s\nEND KEEP\n",M->m_text);
#endif

  return 0;
}

/*****************************************************************************
 *
 * Save HDL text in editor to module definition with consitency checks.
 *
 * Parameters:
 *     name		Expected name of module to save to or null for current module.
 *
 * Save back hdl text to a module.  The HDL text is scanned to make sure it
 * is being saved with the correct module.  The following actions are also
 * taken as appropriate:
 *
 *   1) If the name of the module in the text definition does not match the
 *   current name of the module, rename the module.
 *
 *   2) If there is more than one module definition, split the text into
 *   multiple modules.
 *  
 *   3) If there are is no module definition, add a simple empty definition.
 *
 *   4) If any module names in 1) or 2) are in use, manipulate the module names.
 *
 *****************************************************************************/
int hdl_checkSave(const char *name)
{
  int module_count = 0;
  HdlModuleData module_data[MAXMODS];
  GModuleDef *M;
  char *text;
  int rvalue = 0;

  if (name)
    M = env_findModule(name);
  else
    M = hdl_active_module;

  if (!M) {
    /* We must have tried to close the HDL editor while not editing an HDL module */
    return rvalue;
  }

  if (M->m_type != MT_TEXTHDL) return 0;

  ob_touch(M);
  M->m_needScan = 1;

  DoTcl("HdlEditor::dumpText");
  text = TkGate.tcl->result;

  /*
   * Turn text into a copy.  Use non-ob functions since we only use it here
   */
  text = strdup(text);

  /*
   * Find the partition points for modules
   */
  module_count = hdl_findPartitions(text,module_data,MAXMODS);

  ob_touch(M);

  if (module_count == 0) {
    /*
     * If no modules where found, copy any available text and add a module/endmodule
     * block with the appropriate name.
     */
    char buf[STRMAX];

    sprintf(buf,"module %s;\n\nendmodule\n",M->m_name);
    GModuleDef_saveText(M,buf);

    module_data[0].comment_start = M->m_text;
    module_data[0].module_line = M->m_text;
    module_data[0].name = M->m_text + 7;	/* Position of module name */
    module_data[0].name_len = strlen(M->m_name);
    module_data[0].endmodule_line = M->m_text + strlen(M->m_text);
    module_data[0].module_end = module_data[0].endmodule_line;
    module_count = 1;
    DoTclL("HdlEditor::loadText",M->m_text,NULL);
  } else if (module_count == 1) {
    /*
     * If a single module was found, store the HDL text back in the module.  Rename the
     * module if it was changed in the HDL text.
     */
  
    GModuleDef_saveText(M,text);

    if (!ismatch(M->m_name,module_data[0].name,module_data[0].name_len)) {
      DoTcl("HdlEditor::askRename");
      if (strcmp(TkGate.tcl->result,"autoedit") == 0) {
	hdl_replaceName(M,M->m_name);
	DoTclL("HdlEditor::loadText",M->m_text,NULL);
      } else if (strcmp(TkGate.tcl->result,"ignore") == 0) {
	/* Do nothing */
      } else {	/* cancel */
	rvalue = -1;
      }
    }
  } else {
    /*
     * If there is more than one module in the text block, we can either split them into
     * separate modules, or comment out everything after the first module.  Ask the user
     * which action they want to take.
     */
    DoTcl("HdlEditor::askSaveOption");
    if (strcmp(TkGate.tcl->result,"split") == 0) {
      GModuleDef_saveText(M, text);
      hdl_splitModules(M,module_data,module_count);
      DoTclL("HdlEditor::loadText",M->m_text,NULL);
    } else if (strcmp(TkGate.tcl->result,"ignore") == 0) {
      GModuleDef_saveText(M, text);
    } else {
      /* "cancel" or unknown value */
      GModuleDef_saveText(M, text);
      rvalue = -1;
    }
  }


  if (text) free(text);

  return rvalue;
}

/*****************************************************************************
 *
 * Save the cursor position in the specified module.
 *
 * Parameters:
 *     name		Name of module
 *     line		Line number in buffer
 *     pos		Character position on line.
 *
 *****************************************************************************/
void hdl_saveCursor(const char *name,int line,int pos)
{
  GModuleDef *M = hdl_active_module;

  if (name) M = env_findModule(name);
  if (M && M->m_type == MT_TEXTHDL) {
    ob_touch(M);
    M->m_curLine = line;
    M->m_curChar = pos;
  }
}

/*****************************************************************************
 *
 * Get the saved cursor position from the current module
 *
 * Parameters:
 *     *line		Line number in buffer
 *     *pos		Character position on line.
 *
 *****************************************************************************/
int hdl_getCursor(int *line,int *pos)
{
  GModuleDef *M = hdl_active_module;

  if (!M) return -1;
  if (M->m_type != MT_TEXTHDL) return -1;

  *line = M->m_curLine;
  *pos = M->m_curChar;

  return 0;
}

/*****************************************************************************
 *
 * Save the current HDL text and end HDL editing.
 *
 *****************************************************************************/
int hdl_close(void)
{
  int rvalue;

#if 0
  printf("hdl_close()\n");
#endif

  rvalue = hdl_save(0);
  hdl_active_module = 0;
  return rvalue;
}

