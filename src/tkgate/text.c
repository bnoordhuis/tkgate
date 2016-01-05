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

    Last edit by hansen on Fri Mar 18 22:21:38 2005
****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "tkgate.h"

TextBlock *new_TextBlock()
{
  TextBlock *tb = (TextBlock*) ob_malloc(sizeof(TextBlock),"TextBlock");

  tb->tb_first = tb->tb_last = 0;
  tb->tb_curLine = 1;
  tb->tb_curChar = 0;

  return tb;
}

void delete_TextBlock(TextBlock *tb)
{
  TextBlock_flush(tb);
  ob_free(tb);
}


void TextBlock_copyRange(TextBlock *dst,TextLine *start,TextLine *stop)
{
  TextLine *line;

  TextBlock_flush(dst);
  for (line = start;line;line = line->next) {
    TextBlock_addLine(dst,line->text);
    if (line == stop) break;
  }
}

void TextBlock_copy(TextBlock *dst, TextBlock *src)
{
  TextLine *line;

  TextBlock_flush(dst);
  for (line = src->tb_first;line;line = line->next)
    TextBlock_addLine(dst,line->text);
}

/*****************************************************************************
 *
 * Initialize a text position (TextPos) to beginning of a text block.
 *
 *****************************************************************************/
void TextBlock_getStartTP(TextBlock *tb,TextPos *tp)
{
  tp->tp_line = tb->tb_first;
  tp->tp_pos = 0;
}


/*****************************************************************************
 *
 * Append a line onto a text block.
 * 
 * Parameters:
 *      tb		Text block in which to add text 
 *      text		Line of text to be added
 *
 *****************************************************************************/
void TextBlock_addLine(TextBlock *tb,const char *text)
{
  TextLine *tl = (TextLine*) ob_malloc(sizeof(TextLine),"TextLine");

  ob_touch(tb);

  tl->text = ob_strdup(text);
  tl->next = 0;

  if (!tb->tb_first) 
    tb->tb_first = tb->tb_last = tl;
  else {
    ob_touch(tb->tb_last);
    tb->tb_last->next = tl;
    tb->tb_last = tl;
  }
      
}

/*****************************************************************************
 *
 * Prepend a line onto a text block.
 * 
 * Parameters:
 *      tb		Text block in which to add text 
 *      text		Line of text to be added
 *
 *****************************************************************************/
void TextBlock_prependLine(TextBlock *tb,const char *text)
{
  TextLine *tl = (TextLine*) ob_malloc(sizeof(TextLine),"TextLine");

  ob_touch(tb);

  tl->text = ob_strdup(text);
  tl->next = 0;

  if (!tb->tb_first) 
    tb->tb_first = tb->tb_last = tl;
  else {
    tl->next = tb->tb_first;
    tb->tb_first = tl;
  }
      
}

/*****************************************************************************
 *
 * Flush the contents of a text buffer
 * 
 * Parameters:
 *      tb		Text block to be flushed
 *
 *****************************************************************************/
void TextBlock_flush(TextBlock *tb)
{
  TextLine *tl,*next_tl;

  ob_touch(tb);

  for (tl = tb->tb_first;tl;tl = next_tl) {
    next_tl = tl->next;
    ob_free(tl->text);
    ob_free(tl);
  }

  tb->tb_first = tb->tb_last = 0;
}
