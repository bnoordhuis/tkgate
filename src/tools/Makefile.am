## Process this file with automake to produce Makefile.in
#
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

AM_CFLAGS=@COMMON_IPATH@

noinst_PROGRAMS=colors msgdiff mvtag

colors_SOURCES=colors.c
colors_LDFLAGS=@COMMON_LPATH@
colors_LDADD=-lcommon

msgdiff_SOURCES=msgdiff.c
msgdiff_LDFLAGS=@COMMON_LPATH@
msgdiff_LDADD=-lcommon

mvtag_SOURCES=mvtag.c
mvtag_LDFLAGS=@COMMON_LPATH@
mvtag_LDADD=-lcommon

dist_noinst_DATA=blue.txt cyan.txt magenta.txt yellow.txt colors.txt green.txt red.txt
dist_noinst_SCRIPTS=genmsg
