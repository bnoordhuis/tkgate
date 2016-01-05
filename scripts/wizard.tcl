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
# Last edit by hansen on Sat Jul 31 15:13:16 2004
#

#
# If we run this file standalone, enter test mode.
#
if {![info exists tkg_progName]} {
  set wizard_test 1
  source "misc.tcl"
  source "dropbox.tcl"
} else {
  set wizard_test 0
}

namespace eval Wizard {
  variable props

  proc buttons {w n1 s1 n2 s2} {
    $w.bb.back configure -text $n1 -state $s1
    $w.bb.next configure -text $n2 -state $s2
  }

  ##############################################################################
  #
  # Generic function for setting the current page in a sequence box
  #
  proc setpage {w page} {
    variable props

    set props($w:current) $page

    if { $props($w:current) == "finish" } {
      destroy $w
      frame $w
      pack $w 
      frame $w.h -width [lindex $props($w:size) 0] -height 0
      frame $w.v -width 0 -height [lindex $props($w:size) 1]
      pack $w.h
      pack $w.v -side left

      $props($w:finish) $w 1
      return
    }

    focus $w.bb.next

    catch { destroy $w.f.p }

    frame $w.f.p
    $props($w:command) $w $w.f.p $props($w:current)
    pack $w.f.p -fill both -expand 1
  }

  ##############################################################################
  #
  # Advance to the next page in a sequence
  #
  proc next {w} {
    variable props

    $props($w:command) $w $w.f.p $props($w:current)+
  }

  ##############################################################################
  #
  # Go back a page in a sequence
  #
  proc back {w} {
    variable props

    $props($w:command) $w $w.f.p $props($w:current)-
  }

  proc cancel {w} {
    destroy .
  }

  proc create {w args} {
    variable props

    set width 400
    set height 300
    set bd 0
    set relief flat
    set command ""
    set finishcommand ""
    set cancelname Cancel

    parseargs $args {-width -height -bd -relief -command -finishcommand }

    set props($w:current) first
    set props($w:command) $command
    set props($w:finish) $finishcommand
    set props($w:size) [list $width $height]

    frame $w -bd $bd -relief $relief

    frame $w.h -width $width -height 0
    frame $w.v -height $height -width 0
    frame $w.x -height 4 -width [expr $width - 20] -bd 2 -relief sunken 
    pack $w.h -side top
    pack $w.v -side left

    frame $w.f
    frame $w.bb
    pack $w.f -fill both -expand 1
    pack $w.x -pady 5
    pack $w.bb -fill x

    button $w.bb.back   -text "<Back" -command "Wizard::back $w"
    button $w.bb.next   -text "Next>" -command "Wizard::next $w"
    button $w.bb.cancel -text "Cancel" -command "$Wizard::props($w:finish) $w 0"
    pack $w.bb.cancel $w.bb.next $w.bb.back -side right -anchor e -padx 5 -pady 5

    bind $w.bb.back <Return> "Wizard::back $w"
    bind $w.bb.next <Return> "Wizard::next $w"
    bind $w.bb.cancel <Return> "Wizard::cancel $w"

    Wizard::setpage $w start+
  }

  proc testpager {rw w page} {
    switch $page {
      start+ {
	Wizard::setpage $rw Fee
      }
      Fee {
	Wizard::buttons $rw "<Back" disabled "Next>" normal

	label $w.l -text Fee
	pack $w.l
      }
      Fee+ {
	Wizard::setpage $rw Fei
      }
      Fei {
	Wizard::buttons $rw "<Back" normal "Next>" normal

	label $w.l -text Fei
	pack $w.l
      }
      Fei+ {
	Wizard::setpage $rw Foe
      }
      Fei- {
	Wizard::setpage $rw Fee
      }
      Foe {
	Wizard::buttons $rw "<Back" normal "Next>" normal

	label $w.l -text Foe
	pack $w.l
      }
      Foe+ {
	Wizard::setpage $rw Fum
      }
      Foe- {
	Wizard::setpage $rw Fei
      }
      Fum {
	Wizard::buttons $rw "<Back" normal "Finish" normal

	label $w.l -text Fum
	pack $w.l
      }
      Fum+ {
	Wizard::setpage $rw finish
      }
      Fum- {
	Wizard::setpage $rw Foe
      }
    }
  }

  proc testdone {w isok} {
    puts "testdone $w"
    destroy .
  }


  #
  # Test the wizard widget
  #
  proc tester {} {
    Wizard::create .seq -width 650 -height 525 -bd 2 -relief raised -command Wizard::testpager -finishcommand Wizard::testdone
    pack .seq
  }
}

if { $wizard_test } {
  Wizard::tester
}

