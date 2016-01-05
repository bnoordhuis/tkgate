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
#ifndef __text_h
#define __text_h

/*****************************************************************************
 *
 * A line of text with a link to the next line.
 *
 *****************************************************************************/
typedef struct text_line {
  char	*text;			/* Text on the line */
  struct text_line *next;	/* Next line */
} TextLine;

/*****************************************************************************
 *
 * A block of text lines
 *
 *****************************************************************************/
typedef struct {
  TextLine	*tb_first;		/* First line of text */
  TextLine	*tb_last;		/* Last line of text */
  int		tb_curLine;		/* Saved line number of cursor */
  int		tb_curChar;		/* Saved character number of cursor */
} TextBlock;

/*****************************************************************************
 *
 * A position in the text buffer.
 *
 *****************************************************************************/
typedef struct {
  TextLine	*tp_line;
  int		tp_pos;
} TextPos;

TextBlock *new_TextBlock();
void delete_TextBlock(TextBlock *tb);
void TextBlock_copy(TextBlock *dst,TextBlock *src);
void TextBlock_copyRange(TextBlock *dst,TextLine *start,TextLine *stop);
void TextBlock_addLine(TextBlock *tb,const char *text);
void TextBlock_prependLine(TextBlock *tb,const char *text);
void TextBlock_flush(TextBlock *tb);
#define TextBlock_first(tb) (tb)->tb_first

void TextBlock_getStartTP(TextBlock *tb,TextPos *tp);
char *TextPos_getToken(TextPos *tp,char *buf);

#define TextLine_next(tl) (tl)->next
#define TextLine_text(tl) (tl)->text

#endif
