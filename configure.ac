#! /bin/bash -norc
dnl	This file is an input file used by the GNU "autoconf" program to
dnl	generate the file "configure", which is run during TkGate installation
dnl	to configure the system for the local environment.

AC_INIT([tkgate],[2.0-b10])
AM_INIT_AUTOMAKE
AC_PREREQ(2.57)
AC_CONFIG_HEADER(config.h)
AC_LANG(C)

#############################################################################
# 
# TkGate version and identifying information.
#
AC_DEFINE(TKGATE_VERSION, 	"2.0",					[Base TkGate version number])
AC_SUBST(TKGATE_VERSION,	2.0)
AC_DEFINE(TKGATE_VERSION_SUFFIX,"-b10",					[TkGate version suffix])
AC_DEFINE(TKGATE_FULL_VERSION,	(TKGATE_VERSION TKGATE_VERSION_SUFFIX), [Full TkGate version number])
AC_DEFINE(TKGATE_DESCRIPTION,	"Digital Circuit Editor and Simulator",	[Tkgate short description])
AC_DEFINE(TKGATE_LOCALDOC,	"file:@H/doc/index.html", 		[Local documentation location])
AC_DEFINE(TKGATE_COPYRIGHT, 	"Copyright (C) 1987-2009 by Jeffery P. Hansen", [Copyright notice])
AC_DEFINE(TKGATE_MAILCONTACT,	"hansen@tkgate.org",			[Email address for support])
AC_DEFINE(TKGATE_HOMEPAGE,	"http://www.tkgate.org",		[TkGate web page])


#############################################################################
#
# Configuration options.
#

#
# Tcl/Tk version we will try to use (from most favored to least favored)
#
TKGATE_TCLTK_VERSIONS="8.5 8.4 8.3"

#
# Directories to search for include files
#
TKGATE_INCDIRS="/usr/X11R6/include /usr/X11/include /pkgs/include /usr/local/include /usr/openwin/include /usr/X/include /usr/include /sw/include /opt/local/include"

#
# Directories to search for library files
#
TKGATE_LIBDIRS="/usr/X11R6/lib /usr/X11/lib /pkgs/lib /usr/local/lib /usr/lib /sw/lib /opt/local/lib"

#
# Libraries we may need if available.
#
#TKGATE_OPTLIBNAMES="dl socket nsl m"


#############################################################################
#
# Default prefix directory for installation by OS 
#
AC_PREFIX_DEFAULT(/usr/local)


#############################################################################
#
# Header and libraries
#
TKGATE_INCLUDE_SPEC=""
TKGATE_LIB_SPEC=""
TKGATE_LIBS=""

#############################################################################
#
# Supported locales
#
TKGATE_LOCALES="ca cs cy de en es fr it ja"
AC_SUBST(TKGATE_LOCALES)

#############################################################################
#
# Top and bottom of config.h
#
AH_TOP([#ifndef __config_h
#define __config_h
#include "options.h"])
AH_BOTTOM([
/*
 * Default font encode for display.
 */
#define TKGATE_DEFAULT_FONT_CODE      "iso8859-1"

#if TKGATE_NOYYRESTART
#define yyrestart(f) { extern FILE *yyin; yyin = f; }
#endif

#if TKGATE_SIGSET
#define signal(a,b)	sigset(a,b)
#endif

#if !HAVE_PW_GECOS
#define pe_gecos pw_name
#endif

#endif /* __config_h */
])

#############################################################################
#
# TkGate Installation Directories
#
currentDirectory=`pwd`
AC_DEFINE_UNQUOTED(TKGATE_SECONDARYHOME, "$currentDirectory", [Backup location for TkGate home directory])

#------------------------------------------------------------------------
# Standard compiler checks
#------------------------------------------------------------------------

#
# If the user did not set CFLAGS, set it now to keep
# the AC_PROG_CC macro from adding "-g -O2".
#
# Set TKGATE_CFLAGS in options.h to set compiler options.
# 
if test "${CFLAGS+set}" != "set" ; then
    CFLAGS=`grep "^#define.*TKGATE_CFLAGS" options.h | sed 's/#define.*TKGATE_CFLAGS//'`
fi

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_YACC
AM_PROG_LEX
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for header files.
AC_PATH_XTRA
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([limits.h malloc.h stdlib.h string.h sys/param.h sys/time.h unistd.h])
TKG_CHECK_ICONV_H

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_STRUCT_TM
AC_CHECK_TYPES(unsigned long long)

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([getcwd gettimeofday memmove memset putenv select strcasecmp strchr strcspn strdup strncasecmp strrchr strspn strstr strcasestr])

#############################################################################
#
# Find out about Tcl/Tk and request substitutions
#
TKG_FINDTCLTK

#############################################################################
#
# Check for gecos field
#
TKG_CHECK_GECOS

#############################################################################
#
# Check for signal handler function
#
TKG_USE_SIGSET

#############################################################################
#
# Check for word size and Endianess
#
AC_C_BIGENDIAN
TKG_WORDSIZE

#############################################################################
#
# Merge all path information
#
TKG_MERGE(TKGATE_IPATH, [$TCL_IPATH $TK_IPATH $X_CFLAGS $ICONV_IPATH])
TKG_MERGE(TKGATE_LPATH, [$TCL_LPATH $TCL_LPATH $ICONV_LPATH])
TKG_MERGE(TKGATE_LIB, [$TCL_LIB $TK_LIB $X_PRE_LIBS -lX11 $X_EXTRA_LIBS $ICONV_LIB])

#############################################################################
#
# TKGate variable substitutions
#
AC_SUBST(TKGATE_IPATH)
AC_SUBST(TKGATE_LPATH)
AC_SUBST(TKGATE_LIB)

#############################################################################
#
# Header file paths we may need
#
AC_SUBST(COMMON_IPATH, ['-I$(top_srcdir)/src/common'])
AC_SUBST(TOP_IPATH, ['-I$(top_srcdir)'])
AC_SUBST(COMMON_LPATH, ['-L$(top_builddir)/src/common'])

#############################################################################
#
# Make sure the libexec directory exists
#
TKG_MAKELIBEXEC

#############################################################################
#
# Substitution variables
#
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/common/Makefile
                 src/tkgate/Makefile
                 src/verga/Makefile
                 src/gmac/Makefile
                 src/tools/Makefile])

AC_OUTPUT
