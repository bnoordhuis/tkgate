#    Copyright (C) 1987-2006 by Jeffery P. Hansen
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program; if not, write to the Free Software
#    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#

DISTCLEANFILES=vgrammar.h vgrammar.c luthor.c

BUILT_SOURCES = vgrammar.h
AM_CFLAGS=@TOP_IPATH@ @COMMON_IPATH@
AM_YFLAGS=-d

bin_PROGRAMS=verga
verga_SOURCES=bytecode.c channel.c circuit.c commands.c directive.c error.c evqueue.c expr.c \
	io.c memory.c mitem.c module.c multint.c net.c operators.c pathmod.c paux.c specify.c \
	statement.c systask.c task.c thyme.c trigger.c value.c verilog.c vgrammar.y luthor.l \
	bytecode.h directive.h evqueue.h memory.h multint.h specify.h task.h trigger.h \
	channel.h dynitem.h expr.h mitem.h net.h  statement.h thyme.h value.h yybasic.h \
	circuit.h error.h io.h module.h operators.h systask.h thyme_config.h verilog.h

verga_LDFLAGS=@COMMON_LPATH@
verga_LDADD=-lcommon

