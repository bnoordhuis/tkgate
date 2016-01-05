#   Copyright (C) 1987-2009 by Jeffery P. Hansen
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
# Last edit by hansen on Thu Jan  8 11:44:04 2009
#############################################################################


#
# This class manages a set of tabs that can be used to switch the frame that
# is displayed.  It is meant as a replacement for ttk::notebook when tkgate
# is run with earlier versions of tcl/tk.  It also replaced the TabBox class
# that was used in earlier versions of TkGate.
#
namespace eval NoteBook {
  proc settabstate {w tab state} {
    $w tab $w.t$tab -state $state
  }

  proc new {w args} {
    set tabs {}
    set tablabels {}
    set tabimages {}
    set command ""
    set startpage ""
    set compound none
    parseargs $args {-tabs -tablabels -tabimages -command -startpage -compound}

    ttk::notebook $w -takefocus 0

    set n [llength $tabs]
    for { set i 0 } { $i < $n } { incr i } {
      set tab_name [lindex $tabs $i]
      set tab_label [lindex $tablabels $i]
      set tab_image [lindex $tabimages $i]
      set tab_w $w.t$tab_name

      $command $tab_w $tab_name
      $w add $tab_w -text $tab_label -image $tab_image -compound $compound
    }

    if { $startpage != ""} {
      $w select $w.t$startpage
    }
  }
}
