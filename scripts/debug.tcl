#   Copyright (C) 1987-2004 by Jeffery P. Hansen
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Last edit by hansen on Wed Jun  8 08:49:57 2005
#
#
# Procedures in this file are only used for debugging purposes
#

namespace eval DebugModSym {
  variable dms_w .debug_modsym

  proc update {ms} {
    del $ms
    add $ms
  }

  proc add {ms} {
    variable dms_w

    puts "DebugModSym::add $ms"
    $dms_w.lb insert end $ms
  }

  proc del {ms} {
    variable dms_w

    set ms [lindex $ms 0]
    puts "DebugModSym::del $ms"
    set n [$dms_w.lb size]
    for { set i 0 } {$i < $n} {incr i} {
      if {[lindex [$dms_w.lb get $i] 0] == $ms} {
	$dms_w.lb delete $i
	break
      }
    }
  }


  proc post args {
    variable dms_w

    toplevel $dms_w

    listbox $dms_w.lb -width 40 -yscrollcommand "$dms_w.vb set"
    scrollbar $dms_w.vb -orient vertical -command "$dms_w.lb yview"

    pack $dms_w.lb -fill both -side left -padx 1 -pady 1
    pack $dms_w.vb -fill y -side left -padx 1 -pady 1
  }
}

#DebugModSym::post
