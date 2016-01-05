## Process this file with automake to produce Makefile.in
#
#    Copyright (C) 1987-2007 by Jeffery P. Hansen
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
AM_CFLAGS=-DTKGATE_HOMEDIR=\"$(pkgdatadir)\" @TOP_IPATH@ @COMMON_IPATH@ @TKGATE_IPATH@
AM_YFLAGS=-d

BASE_OBJS=adder.c and.c arshift.c ascii.c block.c breakpoint.c buffer.c circuit.c clock.c \
	comment.c concat.c cpath.c cursors.c delay.c demux.c decoder.c dip.c divide.c \
	edit.c editstate.c error.c expr.c flipflop.c fonts.c frame.c functions.c gates.c \
	generic.c ground.c hdl.c html.c hyperlink.c icon.c igenerate.c in.c jkff.c join.c joint.c \
	led.c locale.c lshift.c metrics.c modint.c modsym.c module.c mult.c mux.c net.c nmos.c \
	object.c or.c out.c paux.c pmos.c primitives.c print.c ram.c register.c release.c \
	roll.c rom.c rshift.c scope.c scopewin.c script.c search.c selection.c simulate.c \
	switch.c symblock.c tap.c tclfuncs.c text.c tkgate.c tkgatewin.c traceprint.c tri.c \
	tribuffer.c vdd.c verify.c verilog_in.c verilog_out.c wiresadd.c wiresdraw.c \
	wiresedit.c wiresmove.c wiresnap.c wmstuff.c xor.c zoom.c vgrammar.y luthor.l \
	breakpoint.h gates.h object.h verilog.h circuit.h fonts.h generic.h primitives.h print.h \
	cpath.h html.h psprolog.h vparser.h delay.h icon.h scope.h wires.h \
	editstate.h igenerate.h script.h yybasic.h elements.h message.h simulate.h zoom.h \
	error.h modsym.h text.h expr.h module.h tkgate.h functions.h net.h tkgate_config.h tkgate_misc.h


#lib_LTLIBRARIES=libtkgate.la
#libtkgate_la_SOURCES=$(BASE_OBJS)
#libtkgate_la_LDFLAGS=@TKGATE_LPATH@ @X_LIBS@ @COMMON_LPATH@
#libtkgate_la_LIBADD=@TKGATE_LIB@ -lcommon


bin_PROGRAMS=tkgate
tkgate_LDFLAGS=@TKGATE_LPATH@ @X_LIBS@ @COMMON_LPATH@
tkgate_SOURCES=$(BASE_OBJS) main.c
#tkgate_LDADD=libtkgate.la
tkgate_LDADD=@TKGATE_LIB@ -lcommon
