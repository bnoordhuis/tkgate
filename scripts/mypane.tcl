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
# Last edit by hansen on Thu Jan 22 08:45:53 2004
#############################################################################
#
# Panes are used to do resizing of two windows
#
#

namespace eval Pane {
  variable details
  variable grabpos

  proc resize {w x y} {
    variable details
    variable grabpos

    set variable [assoc variable $details($w) ]

    if {$variable == "vertical"} {
      set old_y [lindex $grabpos($w) 1]
      $w.p1 configure -height [expr [winfo height $w.p1] - ($old_y - $y)]

      set grabpos($w) [list $x $y]
    }

    puts resize
  }

  proc makeGrabber {parent w gwidth isVert} {
    variable grabpos

    set grabpos($w) {}

    if {$isVert} {
      set width 50
      set height 2
      frame $w -width $gwidth
    } else {
      set width 2
      set height 50
      frame $w -height $gwidth
    }

    bindtags $w pane$w
    bindtags $w.p1 pane$w
    bindtags $w.p2 pane$w
    bindtags $w.p3 pane$w
    bindtags $w.p4 pane$w
    bindtags $w.v1 pane$w
    bindtags $w.v2 pane$w
    bindtags $w.v3 pane$w

    if { $isVert } {
      pack $w.p1 -padx 1 -side left 
      pack $w.p2 -padx 1 -side right 
      pack $w.p3 -padx 0 -side top -fill both -expand 1
      pack $w.v1 -padx 0 -side top
      pack $w.v2 -padx 0 -side top
      pack $w.v3 -padx 0 -side top
      pack $w.p4 -padx 0 -side top -fill both -expand 1
    } else {
      pack $w.p1 -pady 1 -side top
      pack $w.p2 -pady 1 -side bottom
      pack $w.p3 -pady 0 -side left -fill both -expand 1
      pack $w.v1 -pady 0 -side left
      pack $w.v2 -pady 0 -side left
      pack $w.v3 -pady 0 -side left
      pack $w.p4 -pady 0 -side left -fill both -expand 1
    }

    bind pane$w <1> "set grabpos($w) {%X %Y}"
    bind pane$w <B1-Motion> "Pane::resize $parent %X %Y"
  }

  proc create {w args} {
    variable details

    set orientation vertical
    set width 175
    parseargs $args {-orientation -width}

    set details($w) {}
    lappend details($w) [list orientation $orientation]
    lappend details($w) [list width $width]

    if { $orientation ==  "vertical" } {
      frame $w
      frame $w.w1
      makeGrabber $w $w.grab $width 1
      frame $w.w2

      pack $w.w1 -fill both -expand 1
      pack $w.grab -ipady 4
      pack $w.w2 -fill both -expand 1
    } else {
      frame $w
      frame $w.w1
      makeGrabber $w $w.grab $width 0
      frame $w.w2

      pack $w.w1 -side left left -fill both -expand 1
      pack $w.grab -side left
      pack $w.w2 -side left -fill both -expand 1
    }
  }
}
