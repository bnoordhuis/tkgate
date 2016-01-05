#--------------------------------------------------------------------
# TKG_SET_PREFIX
#
#	Set the default prefix value base on the OS
#
# Arguments:
#       None.
#
# Results:
#	Sets the variable TKGATE_USESIGSET
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_SET_PREFIX],[
  AC_MSG_CHECKING([installation prefix])
  os_name=`uname`
  case X$os_name in
    XFreeBSD)
      AC_PREFIX_DEFAULT(/usr/X11)
      install_dir=/usr/X11
      ;;
    XLinux)
      AC_PREFIX_DEFAULT(/usr/local)
      install_dir=/usr/local
      ;;
    *)
      AC_PREFIX_DEFAULT(/usr/local)
      install_dir=/usr/local
      ;;
  esac
  AC_MSG_RESULT([$os_name ... $install_dir])  
])

#--------------------------------------------------------------------
# TKG_USE_SIGSET
#
#	Check if we should use sigset() instead of signal()
#
# Arguments:
#       None.
#
# Results:
#	Sets the variable TKGATE_USESIGSET
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_USE_SIGSET],[
  AC_MSG_CHECKING([how to set signal handlers])
  AC_CACHE_VAL(signal_ok,
     AC_TRY_COMPILE([#include <signal.h>], [signal(0,0);], [signal_ok=yes], [signal_ok=no]))
  AC_CACHE_VAL(sigset_ok,
     AC_TRY_COMPILE([#include <signal.h>], [sigset(0,0);], [sigset_ok=yes], [sigset_ok=no]))

  if test "$signal_ok" = "yes"; then
    AC_DEFINE(TKGATE_SIGSET, 0, [Use sigset instead of signal if set.])
    AC_MSG_RESULT([signal()])
  else
    if test "$sigset_ok" = "yes"; then
      AC_DEFINE(TKGATE_SIGSET, 1, [Use sigset instead of signal if set.])
      AC_MSG_RESULT([sigset()])
    else
	AC_MSG_ERROR([I can't figure out how to set signal handlers.]);
    fi
  fi
])


#--------------------------------------------------------------------
# TKG_CHECK_GECOS
#
#	Check if struct passwd includes a gecos field.
#
# Arguments:
#       None.
#
# Results:
#	Sets the variable HAVE_PW_GECOS
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_CHECK_GECOS],[
  AC_CACHE_CHECK([pw_gecos in struct pwd], tcl_cv_pwd_pw_gecos,
    AC_TRY_COMPILE([#include <pwd.h>],
	    [struct passwd pwd; pwd.pw_gecos;],
	    tcl_cv_pwd_pw_gecos=yes, tcl_cv_pwd_pw_gecos=no))
if test $tcl_cv_pwd_pw_gecos = yes; then
   AC_DEFINE(HAVE_PW_GECOS, 1, [Does struct password have a pw_gecos field?])
else
   AC_DEFINE(HAVE_PW_GECOS, 0, [Does struct password have a pw_gecos field?])
fi
])

#--------------------------------------------------------------------
# TKG_CHECK_ICONV_H
#
#	Check to see if we can find iconv.h
#
# Arguments:
#       None.
#
# Results:
#	HAVE_ICONV_H
#	ICONV_IPATH
#	ICONV_LPATH
#	ICONV_LIB
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_CHECK_ICONV_H],[
  AC_MSG_CHECKING([for iconv.h])
  for p in $TKGATE_INCDIRS; do
    if test -f $p/iconv.h; then
      iconv_h_dir=$p
    fi
  done
  for p in $TKGATE_LIBDIRS; do
    if test -f $p/libiconv.a; then
      iconv_lib_dir=$p
    fi
  done
  if test "X$iconv_h_dir" = "X"; then
    AC_MSG_RESULT([not found])
  else
    AC_MSG_RESULT([found in $iconv_h_dir])
    AC_DEFINE(HAVE_ICONV_H, 1, [Does the iconv.h header exist on this machine?])
    ICONV_IPATH="-I$iconv_h_dir"
    if test "X$iconv_lib_dir" != "X"; then
      ICONV_LPATH="-L$iconv_lib_dir"
      ICONV_LIB="-liconv"
    fi
  fi
])


#--------------------------------------------------------------------
# TKG_WORDSIZE
#
#	Check the word size on the current machine
#
# Arguments:
#       None.
#
# Results:
#	Sets the variable TKGATE_WORDSIZE to 32 or 64
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_WORDSIZE],[
  AC_CACHE_CHECK([word size], wordsize,
    AC_RUN_IFELSE(AC_LANG_PROGRAM([#include <stdlib.h> #include <stdio.h>], [return sizeof(unsigned) == 8 ? 0 : 1;]),
	[wordsize=64], [wordsize=32]))
  if test $wordsize = 32; then
    AC_DEFINE(TKGATE_WORDSIZE, 32, [Word size of machine.])
  else
    AC_DEFINE(TKGATE_WORDSIZE, 64, [Word size of machine.])
  fi
])

#--------------------------------------------------------------------
# TKG_MERGE
#
# Arguments:
#       var			Variable to assign merged result to
#	values			List to merge
#
# Results:
#	Sets the variable TKGATE_WORDSIZE to 32 or 64
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_MERGE],[
   L=""
   for v in $2; do
     add_ok=1
     for q in $L; do
	if test X$q = X$v; then
	  add_ok=0
	  break
        fi
     done
     if test "$add_ok" = "1"; then
       L="$L $v"
     fi
   done
   $1=$L
])

#--------------------------------------------------------------------
# TKG_GETTCLTRYDIRS
#
# Arguments:
#	none
#
# Results:
#	try_dirs	List of directories to look for tclConfig.sh
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_GETTCLTRYDIRS],[
    try_dirs="$TKGATE_LIBDIRS"
    for v in $TKGATE_TCLTK_VERSIONS; do
      alt_v=`echo $v | sed s/\\\\.//`
      for d in $TKGATE_LIBDIRS; do
        try_dirs="$try_dirs $d/tcl$v $d/tcl$alt_v"
      done
    done
])

#--------------------------------------------------------------------
# TKG_GETTKTRYDIRS
#
# Arguments:
#	ver		Tcl version number being used
#
# Results:
#	try_dirs	List of directories to look for tkConfig.sh
#
# We will try to find tkConfig.sh in the tcl directories as well since
# some OSes might stuff it there.
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_GETTKTRYDIRS],[
  v=$1
  alt_v=`echo $v | sed s/\\\\.//`
  try_dirs="$TKGATE_LIBDIRS"
  for d in $TKGATE_LIBDIRS; do
    try_dirs="$try_dirs $d/tk$v $d/tk$alt_v $d/tcl$v $d/tcl$alt_v"
  done
])


#--------------------------------------------------------------------
# TKG_FINDTCLTK
#
#	Figure out where Tcl is installed and set paths to it.
#
# Arguments:
#       None.
#
# Results:
#	TCLTK_VERSION		Version of Tcl being used
#	TCL_LIB			Libraries (-l options) needed to link with tcl 
#	TK_LIB			Libraries (-l options) needed to link with tk 
#	TCL_IPATH		Include paths (-I options) needed to compile with tcl
#	TK_IPATH		Include paths (-I options) needed to compile with tk
#	TCL_LPATH		Library paths (-L options) needed to link with tcl
#	TK_LPATH		Library paths (-L options) needed to link with tk
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_FINDTCLTK],[
  AC_MSG_CHECKING([tcl/tk version])

    TKG_GETTCLTRYDIRS
    for d in $try_dirs; do
      if test -f $d/tclConfig.sh; then
	. $d/tclConfig.sh
	break
      fi
    done

    if test "X$TCL_VERSION" = "X"; then
      AC_MSG_ERROR([could not find tclConfig.sh])
    else
      TKG_GETTKTRYDIRS($TCL_VERSION)

      for d in $try_dirs; do
        if test -f $d/tkConfig.sh; then
          . $d/tkConfig.sh
	  break
	fi
      done

      if test "X$TK_VERSION" = "X"; then
        AC_MSG_ERROR([could not find tkConfig.sh])
      else
        if test "X$TCL_VERSION" != "X$TK_VERSION"; then
          AC_MSG_ERROR([versions of tcl ($TCL_VERSION) and tk ($TK_VERSION) do not match.])
        else
	  TCLTK_VERSION=$TCL_VERSION
	  TKG_SETTCLTK
        fi
      fi
    fi

    AC_MSG_RESULT([$TCLTK_VERSION])
  ])
])


#--------------------------------------------------------------------
# TKG_SETTCLTK
#
# Arguments:
#       ver		Version number
#	dir		Directory in which to look for package (tcl or tk)
#	pkg		Which package to look for (tcl or tk)
#
# Results:
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_FINDTCLTKIPATH],[
  v=$1
  alt_v=`echo $v | sed s/\\\\.//`

  ipath=""
  for d in $TKGATE_INCDIRS; do
    if test -f $d/$2$v/$3.h; then
      ipath=-I$d/$2$v
    else
      if test -f $d/$2$alt_v/$3.h; then
	ipath=-I$d/$2$alt_v
      else
	if test -f $d/$3.h; then
	  ipath=-I$d
        fi
      fi
    fi
  done
])


#--------------------------------------------------------------------
# TKG_SETTCLTK
#
#	Set the variables for tcl/tk (helping function for TKG_FINDTCLTK)
#
# Arguments:
#       None.
#
# Results:
#	TCL_LIB			Libraries (-l options) needed to link with tcl 
#	TK_LIB			Libraries (-l options) needed to link with tk 
#	TCL_IPATH		Include paths (-I options) needed to compile with tcl
#	TK_IPATH		Include paths (-I options) needed to compile with tk
#	TCL_LPATH		Library paths (-L options) needed to link with tcl
#	TK_LPATH		Library paths (-L options) needed to link with tk
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_SETTCLTK],[
  TCL_LIB=$TCL_LIB_SPEC
  TCL_LPATH=""
  TCL_IPATH=$TCL_INCLUDE_SPEC
  TK_LIB=$TK_LIB_SPEC
  TK_LPATH=""
  TK_IPATH=$TK_INCLUDE_SPEC

  if test "X$TCL_IPATH" = "X"; then
    TKG_FINDTCLTKIPATH($TCLTK_VERSION, 'tcl', 'tcl')
    TCL_IPATH=$ipath
	echo TCL_IPATH=$ipath
  fi 

  if test "X$TK_IPATH" = "X"; then
    TKG_FINDTCLTKIPATH($TCLTK_VERSION, 'tk', 'tk')
    if test "X$ipath" = "X"; then
      TKG_FINDTCLTKIPATH($TCLTK_VERSION, 'tcl', 'tk')
    fi
    TK_IPATH=$ipath
  fi 

  if test "X$TCL_IPATH" = "X-I/usr/include"; then
    TCL_IPATH=""
  fi 
  if test "X$TK_IPATH" = "X-I/usr/include"; then
    TK_IPATH=""
  fi 
])


#--------------------------------------------------------------------
# TKG_FINDTCLTK_OBSOLETE
#
#	Figure out where Tcl is installed and set paths to it.
#
# Arguments:
#       None.
#
# Results:
#	TCLTK_VERSION		Version of Tcl being used
#	TCL_LIBS		Libraries (-l options) needed to link with tcl 
#	TCL_IPATH		Include paths (-I options) needed to compile with tcl
#	TCL_LPATH		Library paths (-L options) needed to link with tcl
#	TK_LIBS			Libraries (-l options) needed to link with tk 
#	TK_IPATH		Include paths (-I options) needed to compile with tk
#	TK_LPATH		Library paths (-L options) needed to link with tk
#
#--------------------------------------------------------------------
AC_DEFUN([TKG_FINDTCLTK_OBSOLETE],[
  AC_MSG_CHECKING([tcl/tk version])

    for v in $TKGATE_TCLTK_VERSIONS; do
      alt_v=`echo $v | sed s/\\\\.//`

      for d in $TKGATE_INCDIRS; do
	TK_VERSION=""
	TCL_VERSION=""
 	if test -f $d/tcl$v/tcl.h; then
	  TCL_VERSION=$v
	  TCL_IPATH=-I$d/tcl$v
	else
 	  if test -f $d/tcl$alt_v/tcl.h; then
	    TCL_VERSION=$v
	    TCL_IPATH=-I$d/tcl$alt_v
          else
	    if test -f $d/tcl.h; then
	      TCL_VERSION=$v
	      TCL_IPATH=-I$d
	    fi
	  fi
        fi

 	if test -f $d/tcl$v/tk.h; then
	  TK_VERSION=$v
	  TK_IPATH=-I$d/tcl$v
        else
	  if test -f $d/tcl$alt_v/tk.h; then
	    TK_VERSION=$v
	    TK_IPATH=-I$d/tcl$alt_v
	  fi
	fi


 	if test -f $d/tk$v/tk.h; then
	  TK_VERSION=$v
	  TK_IPATH=-I$d/tk$v
        else
	  if test -f $d/tk$alt_v/tk.h; then
	    TK_VERSION=$v
	    TK_IPATH=-I$d/tk$alt_v
          else
	    if test -f $d/tk.h; then
	      TK_VERSION=$v
	      TK_IPATH=-I$d
	    fi
	  fi
	fi

	if test "X$TCL_VERSION" != "X" -a "X$TK_VERSION" != "X"; then
	  break
        fi
      done
      if test "X$TCL_VERSION" != "X" -a "X$TK_VERSION" != "X"; then
	break
      fi
    done

    if test "X$TCL_VERSION" = "X" -o "X$TK_VERSION" = "X"; then
      AC_MSG_ERROR([failed])
    fi

    TCLTK_VERSION=$TCL_VERSION

    if test "X$TCL_IPATH" = "X-I/usr/include"; then
      TCL_IPATH=""
    fi 
    if test "X$TK_IPATH" = "X-I/usr/include"; then
      TK_IPATH=""
    fi 

    for d in $TKGATE_LIBDIRS; do
      if test -f $d/libtcl$v.a -o -f $d/libtcl$v.so -o -f $d/libtcl$v.dylib -o -f $d/libtcl$v.dll.a; then
	TCL_LIB=-ltcl$v
	TCL_LPATH=-L$d
      fi
      if test -f $d/libtcl$alt_v.a -o -f $d/libtcl$alt_v.so -o -f $d/libtcl$alt_v.dylib -o -f $d/libtcl$alt_v.dll.a; then
	TCL_LIB=-ltcl$alt_v
	TCL_LPATH=-L$d
      fi
    done

    for d in $TKGATE_LIBDIRS; do
      if test -f $d/libtk$v.a -o -f $d/libtk$v.so -o -f $d/libtk$v.dylib -o -f $d/libtk$v.dll.a; then
	TK_LIB=-ltk$v
	TK_LPATH=-L$d
        break
      fi
    done
    for d in $TKGATE_LIBDIRS; do
      if test -f $d/libtk$alt_v.a -o -f $d/libtk$alt_v.so -o -f $d/libtk$alt_v.dylib -o -f $d/libtk$alt_v.dll.a; then
	TK_LIB=-ltk$alt_v
	TK_LPATH=-L$d
	break
      fi
    done

    if test "X$TCL_LIB" = "X" -o "X$TK_LIB" = "X"; then
      AC_MSG_ERROR([could not find library])
    fi

    AC_MSG_RESULT([$TCLTK_VERSION])
  ])
])

AC_DEFUN([TKG_MAKELIBEXEC],[
  if test ! -d libexec; then
    mkdir libexec
  fi
  cd libexec

  rm -f tkgate
  if test -f ../src/tkgate/tkgate; then
    $LN_S ../src/tkgate/tkgate tkgate
  else
    touch ../src/tkgate/tkgate
    $LN_S ../src/tkgate/tkgate tkgate; \
    rm -f ../src/tkgate/tkgate
  fi

  rm -f verga
  if test -f ../src/verga/verga; then
    $LN_S ../src/verga/verga verga
  else
    touch ../src/verga/verga
    $LN_S ../src/verga/verga verga; \
    rm -f ../src/verga/verga
  fi

  cd ..

])

# Local Variables:
# mode: autoconf
# End:

