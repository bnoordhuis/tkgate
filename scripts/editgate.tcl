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
# Last edit by hansen on Sat Sep 26 15:32:41 2009
#


#
# Wire Ops: Changes to the port list are passed back through the variable "edgat_wops".  The
# variable contains a list of "wire ops".  Wire ops can be one of the following:
#
#      {add I}			Add a wire to port group I
#      {delete I2}		Delete 2nd wire of port group I
#      {edit Pold w Pnew io n}	Edit port.  Pold is old port name, w is signal name,
#      				Pnew is new port name, io is the I/O type,
#				and n is the number of bits.


set edgat_wires {}
set edgat_wops {}
set edgat_portOpCount 0
set edgat_dtype "huh?"
set edgat_type "huh?"
set edgat_name "huh?"
set edgat_hideName 1
set edgat_cpbreak 0
set edgat_anchor 0
set edgat_xpos 0
set edgat_ypos 0
set edgat_blockFunc "huh?"
set edgat_newBlock 0
set edgat_newPort 0
set edgat_geom "+0+0"
set ecpOmega 100
set ecpPhi 50
set ecpDuty 50
set edgat_switch 0
set edgat_extbar 0
set edgat_dip 0
set edgat_tap 0
set edgat_memFile ""
set edgat_ok 0
set edgat_commentLen 0
set edgat_dolink 0
set edgat_link ""
set edport_hidename 0

set edport_ok 0
set edport_idx 0
set edport_sig "huh"
set edport_port "huh"
set edport_basePort "huh"
set edport_type "huh"
set edport_bits "huh"

set clkFrame [list]

array set editgate_caps {
  AND {sig size add}
  OR {sig size add}
  XOR {sig size add}
  MUX {sig size add}
  DEMUX {sig size add}
  CONCAT {sig size add}
  Module {sig size type port add side}
  TAP {}
}

proc trace_edgat_tech args {
  global edgat_tech edgatdelay_w edgat_techdelay
  gat_setGateTechDelays $edgat_tech
}
trace variable edgat_tech w trace_edgat_tech

#############################################################################
#
# Dialog box for editing a single port on a module.
#
namespace eval PortEdit {
  proc ok {} {
    global edport_ok

    set edport_ok 1
    destroy .edport 
  }

  ########################################
  #
  # Throw up a dialog box for changing port values (Now only used when creating ports)
  #
  proc post {pos args} {
    global edport_idx edport_sig edport_basePort edport_port edport_type edport_bits edport_hidename
    global edgat_type edgat_wops edport_ok
    global edgatio_w

    set showsig 1
    set interface 0

    parseargs $args {-ismodule -showsig}

    set edport_ok 0
    set oldPort $edport_port
    set edport_hidename 1

    set p [string first ":" $edport_port]
    if { $p > 0 } {
      set edport_basePort [string range $edport_port 0 $p]
      set edport_port [string range $edport_port [expr $p + 1] end]
      set isBlock 1
    } {
      set edport_basePort ""
      set isBlock 0
    }

    toplevel .edport
    wm transient .edport .
    wm resizable .edport 0 0
    wm title .edport [m port.title]
    wm geometry .edport $pos

    frame .edport.box -relief raised -bd 1

    dialogImage .edport.box.image -image [gifI port.gif] -caption [m port.parms]
    grid .edport.box.image -sticky nsew -column 0 -row 0 -rowspan 6
    grid rowconfigure .edport.box 5 -weight 1

    if { $showsig } {
      label .edport.box.siglab  -text "[m db.gate.signam]:"
      entry .edport.box.sig     -textvariable edport_sig -bg white
    }
    label .edport.box.portlab -text "[m db.gate.portnam]:"
    entry .edport.box.port    -textvariable edport_port -bg white

    label .edport.box.iolab   -text "[m db.gate.iotype]:"
    label .edport.box.bitlab  -text "[m db.gate.bitw]:"

    frame .edport.box.io
    radiobutton .edport.box.io.in -text [m b.in] -variable edport_type -value in
    radiobutton .edport.box.io.out -text [m b.out] -variable edport_type -value out
    radiobutton .edport.box.io.bdir -text [m b.inout] -variable edport_type -value inout
    pack .edport.box.io.in .edport.box.io.out .edport.box.io.bdir -side left

#    entry .edport.box.bit     -textvariable edport_bits -bg white
    bitsizeselector .edport.box.bit -variable edport_bits -width 3 


    if { $showsig } {
      bind .edport.box.sig <KeyPress-Return> PortEdit::ok
    }

    bind .edport.box.port <KeyPress-Return> PortEdit::ok
    bind .edport.box.bit <KeyPress-Return> PortEdit::ok

    frame .edport.box.pad -height 10
    grid .edport.box.pad -column 1 -row 0 

    grid .edport.box.portlab  -column 1 -row 1 -sticky e -padx 5 -pady 2
    grid .edport.box.port     -column 2 -row 1 -sticky w -padx 5 -pady 2
    grid .edport.box.iolab    -column 1 -row 2 -sticky e -padx 5 -pady 2
    grid .edport.box.io       -column 2 -row 2 -sticky w -padx 5 -pady 2
    grid .edport.box.bitlab   -column 1 -row 3 -sticky e -padx 5 -pady 2
    grid .edport.box.bit      -column 2 -row 3 -sticky w -padx 5 -pady 2

    if { $showsig } {
      grid .edport.box.siglab   -column 1 -row 4 -sticky e -padx 5 -pady 2
      grid .edport.box.sig      -column 2 -row 4 -sticky w -padx 5 -pady 2
    }

    frame  .edport.confirm -relief raised -bd 1
    button .edport.confirm.ok     -text [m b.ok] -command PortEdit::ok
    button .edport.confirm.cancel -text [m b.cancel] -command { destroy .edport }
    pack .edport.confirm.ok -side left -padx 5 -pady 5
    pack .edport.confirm.cancel -side right -padx 5 -pady 5

    pack .edport.box -fill x 
    pack .edport.confirm -fill both

    #
    # Select port name
    #
    .edport.box.port selection range 0 end
    .edport.box.port icursor end
    focus .edport.box.port

    if { $showsig } {
      helpon .edport.box.siglab  [m ho.edgat.signam]
    }
    helpon .edport.box.portlab [m ho.edgat.port]
    helpon .edport.box.iolab   [m ho.edgat.iolab]
    helpon .edport.box.bitlab  [m ho.edgat.bitlab]

    dialogWait .edport

    return $edport_ok
  }
}

#############################################################################
#
# Methods for editing module parameters.
#
namespace eval ModParmMgr {
  variable pmlb_w

  #############################################################################
  #
  # Entry manager for the module parameter manager
  #
  proc manager {cmd args} {
    variable pmlb_w

    if { [lindex $args 0] == "-args" } {
      set args [lindex $args 1]
    }

    switch $cmd {
      canenter {               	# check to see if we can enter data
	set c [lindex $args 2]
	if { $c == 1} {
	  return 1
	} else {
	  return 0
	}
      }
      entrywidget {
	return 0
      }
      close {
	set data [lindex $args 3]
	return $data
      }
      delete {			# respond to the delete key
      }
      canappend {
	return 0
      }
    }
  }

  proc seeDestroy args {
    variable pmlb_w
    global edgat_modParms

    set N [SpreadSheet::size $pmlb_w]
    for { set i 0 } { $i < $N } { incr i } {
      set entry [SpreadSheet::get $pmlb_w $i]
      set edgat_modParms([lindex $entry 0]) [lindex $entry 1]
    }
  }

  proc create {w} {
    variable pmlb_w
    global edgat_modParms

    frame $w.mparms
    set pmlb_w $w.mparms.lb

    #
    # Parameter names
    #
    set names [lsort [array names edgat_modParms]]

    if {[llength $names] > 0 } {
      SpreadSheet::create $w.mparms.lb  -height 15 -bd 1 -relief sunken -yscrollcommand "$w.mparms.vb set" -expandcol 1 -entrycommand ModParmMgr::manager
      scrollbar $w.mparms.vb -orient vertical -command "SpreadSheet::yview $w.mparms.lb"

      SpreadSheet::addcolumn $w.mparms.lb -width 25 -header [m db.gate.mparm.parameter]
      SpreadSheet::addcolumn $w.mparms.lb -width 20 -header [m db.gate.mparm.value]

      pack $w.mparms.lb $w.mparms.vb -side left -fill both

      foreach n $names {
	SpreadSheet::insert $w.mparms.lb end [list $n $edgat_modParms($n)]
      }

      bind $w.mparms.lb <Destroy> ModParmMgr::seeDestroy
    } else {
      label $w.mparms.exampleA -text [m db.gate.mparm.exampleA] -justify left
      pack $w.mparms.exampleA -padx 10 -pady 10
      label $w.mparms.exampleB -text [m db.gate.mparm.exampleB] -justify left -font hdlFont
      pack $w.mparms.exampleB -padx 10 -pady 10 -anchor w
    }

    pack $w.mparms -padx 10 -pady 10
  }
}

#############################################################################
#
# Methods for editing the port list of an instance.
#
namespace eval PortMgr {
  variable io_w ""
  variable iolb_w ""
  variable doIdleUpdate 0
  variable edcap_sig
  variable edcap_port
  variable edcap_type
  variable edcap_size
  variable edcap_add
  variable edcap_side

  #############################################################################
  #
  # Add a new port (used on initial load of gate)
  #
  proc add {p n} {
    variable iolb_w
    SpreadSheet::insert $iolb_w $p $n
  }

  #############################################################################
  #
  # Respond to a press of the "New" button
  #
  proc addNew {w} {
    set r [SpreadSheet::size $w]
    set r [SpreadSheet::getPhysicalRow $w $r]
    SpreadSheet::entrySelect $w $r 0
  }

  #############################################################################
  #
  # Respond to a press of the "Delete" button
  #
  proc deletePort {w} {
    variable edcap_add
    variable edport_type
    global edgat_wops

    #
    # Deletions only allowed when adds are also valid. 
    #
    if {!$edcap_add} { return }
    

    #
    # If nothing is selected, then do nothing
    #
    set items [SpreadSheet::getselection $w]
    if {[llength $items] == 0} { return }

    #
    # Count number of variable ports
    #
    set N [SpreadSheet::size $w]
    set Nvar 0
    for { set i 0 } { $i < $N } { incr i } {
      set E [SpreadSheet::get $w $i]
      set P [lindex $E 1]
      if {[regexp {^([A-Za-z]+)([0-9]+)$} $P X B pnum]} {
	incr Nvar
      }
    }

    #
    # If all remaining variable ports are selected, then do nothing.
    #
    if {[llength $items] == $Nvar} { return }


    #
    # Make sure selected ports are all of the same variable type
    #
    set ok 1
    set baseName ""
    set plist {}
    foreach i $items {
      set P [lindex [SpreadSheet::get $w $i] 1]
      if {![regexp {^([A-Za-z]+)([0-9]+)$} $P X B pnum]} {
	set ok 0
      }
      if {$baseName == "" } { set baseName $B }
      if { $baseName != $B } { set ok 0 }
      lappend plist $pnum
      
    }
    if { !$ok } { return }


    #
    # Deletion is ok
    #
    SpreadSheet::delete $w $items

    #
    # Create wire ops for deletions in reverse order of port number
    #
    foreach pn [lsort -integer -decreasing $plist] {
      lappend edgat_wops [list delete $baseName$pn]
    }


    #
    # Go get the undeleted entries from the variable pad and sort them
    #
    set N [SpreadSheet::size $w]
    set L {}
    for { set i 0 } { $i < $N } { incr i } {
      set E [SpreadSheet::get $w $i]
      set P [lindex $E 1]
      if {[regexp {^([A-Za-z]+)([0-9]+)$} $P X B pnum]} {
	lappend L [list $pnum $i]
      }
    }
    set L [lsort -index 0 $L]

    #
    # Now relable the variable pad ports
    #
    set n 0
    foreach p $L {
      set i [lindex $p 1]
      set E [SpreadSheet::get $w $i]

      if {[regexp {^([A-Za-z]+)([0-9]+)$} [lindex $E 1] X B pnum]} {
	set E [lreplace $E 1 1 $B$n]
	incr n
      }

      SpreadSheet::put $w $i $E
    }
  }


  #############################################################################
  #
  # Respond to a press of the "Edit" button
  #
  proc editSelected {w} {
    set items [SpreadSheet::getselection $w]

    if { [llength $items] < 1 } { return }

    set r [lindex $items 0]
    set r [SpreadSheet::getPhysicalRow $w $r]
    SpreadSheet::entrySelect $w $r 0
  }



  #############################################################################
  #
  # Generate a new port entry
  #
  proc newPortEntry {} {
    variable iolb_w
    variable edcap_side
    global edgat_wops edgatio_w edport_basePort 
    global edport_sig edport_port edport_type edport_bits edport_ok

    set Q -1
    set E ""

    set N [SpreadSheet::size $iolb_w] 
    for {set i 0} { $i < $N } {incr i} {
      set item [SpreadSheet::get $iolb_w $i]

      set P [lindex $item 1]

      if {[regexp {^([A-Za-z]+)([0-9]+)$} $P X B Nbits]} {
	if { $Nbits > $Q} { 
	  set edport_idx [expr $i + 1]
	  set Q $Nbits
	  set E $item
	}
      }
    }

    if { $Q < 0 } {
      errmsg [m err.badportadd]
      return
    }
    incr Q

    set portName  [string trimright [lindex $E 1] 0123456789]$Q

    set item [list "*autoname*" $portName [lindex $E 2] [lindex $E 3]]
    if { $edcap_side } {
      lappend item [lindex $E 4]
    }

    return $item
  }

  #############################################################################
  #
  # Entry manager for the port editor
  #
  proc manager {cmd args} {
    variable edcap_sig
    variable edcap_port
    variable edcap_type
    variable edcap_size
    variable edcap_side
    variable edcap_add
    variable iolb_w
    global edgat_wops
    global edgat_oldentry
    global edgat_portOpCount

    if { [lindex $args 0] == "-args" } {
      set args [lindex $args 1]
    }
    switch $cmd {
      canenter {               	# check to see if we can enter data
	set c [lindex $args 2]
	switch $c {
	  0 { return $edcap_sig }
	  1 { return $edcap_port }
	  2 { return $edcap_type }
	  3 { return $edcap_size }
	  4 { return $edcap_side }
	}
	return 0
      }
      entrywidget {		# use a special widget for some cells (return 0 for normal entry)
	set w [lindex $args 0]
	set r [lindex $args 1]
	set c [lindex $args 2]
	set W [lindex $args 3]
	set width [lindex $args 4]
	set variable [lindex $args 5]
	set edgat_oldentry [SpreadSheet::get $iolb_w [SpreadSheet::getEffectiveRow $iolb_w $r]]

	switch $c {
	  0 {
	    return 0
	  }
	  1 {
	    return 0
	  }
	  2 {
	    Dropbox::new $W -variable $variable -width [expr $width - 3] -bd 1 -highlightthickness 0
	    SpreadSheet::tabBindings $W.e $w $r $c
	    Dropbox::itemadd $W "in"
	    Dropbox::itemadd $W "out"
	    Dropbox::itemadd $W "inout"
	    update
	    focus $W.e
	    return 1
	  }
	  3 {
	    bitsizeselector $W -variable $variable -width [expr $width - 3]
#	    Dropbox::new $W -variable $variable -width [expr $width - 3] -bd 1 -highlightthickness 0
	    SpreadSheet::tabBindings $W.e $w $r $c
#	    for { set i 1 } { $i <= 32 } { incr i } {
#	      Dropbox::itemadd $W $i
#	    }
	    update
	    focus $W.e
	    return 1
	  }
	  4 {
	    Dropbox::new $W -variable $variable -width [expr $width - 3] -bd 1 -highlightthickness 0
	    SpreadSheet::tabBindings $W.e $w $r $c
	    Dropbox::itemadd $W "left"
	    Dropbox::itemadd $W "right"
	    Dropbox::itemadd $W "top"
	    Dropbox::itemadd $W "bottom"
	    update
	    focus $W.e
	    return 1
	  }

	}

	return 0
      }
      delete {			# respond to the delete key
	deletePort $iolb_w
      }
      closenotify {
	incr edgat_portOpCount
      }
      close {
	set r [SpreadSheet::getEffectiveRow $iolb_w [lindex $args 1]]
	set c [lindex $args 2]
	set data [lindex $args 3]

	if { [lindex $edgat_oldentry $c] != $data } {
	  set wop [concat [list edit [lindex $edgat_oldentry 1]] [lreplace $edgat_oldentry $c $c $data]]
	  lappend edgat_wops $wop
	}

	return $data
      }
      canappend {
	return $edcap_add
      }
      initentry {
	set w [lindex $args 0]
	set r [lindex $args 1]

	set new_ent [newPortEntry]

	set base_name  [string trimright [lindex $new_ent 1] 0123456789]
	lappend edgat_wops [list add $base_name]

	return $new_ent
      } 
    }
  }

  proc seeDestroy args {
    variable iolb_w
    global edgat_wires

    set N [SpreadSheet::size $iolb_w]
    set edgat_wires {}
    for { set i 0 } { $i < $N } { incr i } {
      lappend edgat_wires [SpreadSheet::get $iolb_w $i]
    }
  }

  #############################################################################
  #
  # Create the port manager window.
  #
  proc create {w args} {
    variable io_w
    variable iolb_w
    variable edcap_sig
    variable edcap_port
    variable edcap_type
    variable edcap_size
    variable edcap_add
    variable edcap_side
    global edgat_wires

    set edcap_sig  0
    set edcap_port 0
    set edcap_type 0
    set edcap_size 0
    set edcap_add  0
    set edcap_side  0

    parseargs $args {-editcaps}
    foreach cap $editcaps {
      set edcap_$cap 1
    }


    set io_w $w.io
    set iolb_w $w.io.ports.lb

    frame $w.io

    frame $w.io.but
    button $w.io.but.edit -text [m db.gate.edit] -command "PortMgr::editSelected  $iolb_w"
    button $w.io.but.add -text [m db.gate.add] -command "PortMgr::addNew $iolb_w"
    button $w.io.but.delete -text [m db.gate.delete] -command "PortMgr::deletePort $iolb_w"
    pack $w.io.but.edit $w.io.but.add $w.io.but.delete -padx 5 -pady 5 -side left
    pack $w.io.but -side bottom -anchor e
    if { !$edcap_add } {
      $w.io.but.add configure -state disabled
      $w.io.but.delete configure -state disabled
    }
    pack $w.io -expand 1 -fill both


    frame $w.io.ports
    SpreadSheet::create $w.io.ports.lb  -height 10 -bd 1 -relief sunken -yscrollcommand "$w.io.ports.vb set"  -entrycommand PortMgr::manager -expandcol 1

    SpreadSheet::addcolumn $iolb_w -width 13 -header [m db.gate.signam]
    SpreadSheet::addcolumn $iolb_w -width 8 -header [m db.gate.port]
    SpreadSheet::addcolumn $iolb_w -width 6  -header [m db.gate.iotype]
    SpreadSheet::addcolumn $iolb_w -width 6  -header [m db.gate.bitws]
    if { $edcap_side } {
      SpreadSheet::addcolumn $iolb_w -width 6  -header [m db.gate.side]
    }

    scrollbar $w.io.ports.vb -orient vertical -command "SpreadSheet::yview $iolb_w"
    pack $w.io.ports.lb -fill both -side left -expand 1
    pack $w.io.ports.vb -fill both -side left
    pack $w.io.ports -padx 10 -pady 10 -fill x -expand 1

    foreach n $edgat_wires {
      PortMgr::add end $n
    }
#    updateWhenIdle

    bind $w.io.ports.lb <Destroy> PortMgr::seeDestroy
  }
}

namespace eval EditGate {
  variable clock_line_len	320.0
  variable tap_size
  variable wire_size
  variable tap_lsbMax

  #################################################
  #
  # Select interval and total width from an omega value.
  #
  #     P	Power of ten >= to omega
  #     Q	Normalized omega value
  #     V	Size of interval
  #
  proc chooseW {omega} {

    set omega [expr 1.2*$omega ]
    if { $omega < 10 } {set omega 10 }

    for {set P 1.0} {$P < $omega} {set P [expr $P * 10.0]} {}

    set Q [expr ($P - $omega)/$P ]

    if { $Q >= 0.8 } {
      set V $P*0.02
    } elseif { $Q >= 0.6 } {
      set V $P*0.05
    } else {
      set V $P*0.1
    }

    set W 0
    for {set I 0} { $W <= $omega } { incr I } {
      set W [expr int($I * $V) ]
    }
    set W [expr int($I * $V) ]

    return [list $I $W]
  }

  #############################################################################
  #
  # Create the frame for the clock diagram.
  #
  proc clockFrame {w} {
    global ecpI ecpW 
    variable clock_line_len

    $w.wave delete frame
    set clkFrame [list]
    for {set i 0} {$i <= $ecpI } {incr i} {
      set x [expr 15 + ($i)*$clock_line_len/$ecpI ]
      $w.wave create line ${x} 75 ${x} 85 -tag frame
      $w.wave create text ${x} 100 -text [expr $i*$ecpW/$ecpI ] -anchor s -tag frame
    }
  }

  ########################################
  #
  # Update clock display from value change
  #
  proc setClockTrace {w} {
    global ecpl1 ecpl2 ecpl3 ecpl4 ecpl5
    global ecpE0 ecpE1 ecpE2
    $w.wave coords $ecpl1 $ecpE0  5 $ecpE0 75
    $w.wave coords $ecpl2 15     65 $ecpE1 65
    $w.wave coords $ecpl3 $ecpE1 65 $ecpE1 15
    $w.wave coords $ecpl4 $ecpE1 15 $ecpE2 15
    $w.wave coords $ecpl5 $ecpE2 15 $ecpE2 65
  }

  ########################################
  #
  # Translate movement of a clock graph component to actual new positons
  # and new parameter values.
  #
  #
  proc moveClock {w x} {
    global ecpS ecpE0 ecpE1 ecpE2
    global ecpl1 ecpl2 ecpl3 ecpl4 ecpl5
    global ecpPhi ecpDuty ecpOmega ecpW 
    variable clock_line_len

    trace vdelete ecpOmega w EditGate::clockPUpdate
    trace vdelete ecpPhi w EditGate::clockPUpdate
    trace vdelete ecpDuty w EditGate::clockPUpdate

    if { $ecpS == $ecpl1 } {
      set ecpE0 $x
      if { $ecpE0 < 15 }      { set ecpE0 15 }
      if { $ecpE0 >= $ecpE2 } { set ecpE0 $ecpE2 }
      set ecpPhi [expr int(100*($ecpE0 - 15)/($ecpE2 - 15)) ]
    } elseif { $ecpS == $ecpl3 } {
      set ecpE1 $x
      if { $ecpE1 <= 15 }     { set ecpE1 15 }
      if { $ecpE1 >= $ecpE2 } { set ecpE1 [expr $ecpE2 - 1] }
      set ecpDuty [expr int(100*($ecpE1 - 15)/($ecpE2 - 15)) ]
    } elseif { $ecpS == $ecpl5 } {
      set ecpE2 $x
      if { $ecpE2 <= $ecpE1 } { set ecpE2 [expr $ecpE1 + 1] }
      if { $ecpE2 <= $ecpE0 } { set ecpE2 [expr $ecpE0 + 1] }
      if { $ecpE2 >= [expr $clock_line_len + 15] }    { set ecpE2 [expr $clock_line_len + 15] }
      set ecpE0 [expr 15 + int($ecpPhi*($ecpE2-15.0)/100.0)]
      set ecpE1 [expr 15 + int($ecpDuty*($ecpE2-15.0)/100.0)]
      set ecpOmega [expr int($ecpW*($ecpE2 - 15)/$clock_line_len) ]
    }

    trace variable ecpOmega w EditGate::clockPUpdate
    trace variable ecpPhi w EditGate::clockPUpdate
    trace variable ecpDuty w EditGate::clockPUpdate

    setClockTrace $w
  }


  ########################################
  #
  # Translate change in parameter values to change in graph
  #
  proc clockPUpdate args {
    global ecpl1 ecpl2 ecpl3 ecpl4 ecpl5
    global ecpE0 ecpE1 ecpE2
    global ecpW ecpI ecpOmega ecpPhi ecpDuty
    global edgatclk_w 
    variable clock_line_len

    catch {

      if { [scan $ecpPhi "%d" P] != 1 } { set P 0 }
      if { [scan $ecpDuty "%d" D] != 1 } { set D 50 }
      if { [scan $ecpOmega "%d" O] != 1 } { set O $ecpW }

      set IW [chooseW $ecpOmega]
      set ecpI [lindex $IW 0]
      set ecpW [lindex $IW 1]

      if { $P <= 0 } { set P 0 }
      if { $P >= 99 } { set P 99 }
      if { $D <= 0 } { set D 1 }
      if { $D >= 99 } { set D 99 }
      if { $O <= 0 } { set O 1 }

      set X [expr $clock_line_len*$O/$ecpW ]

      set ecpE2 [expr 15 + $X ]
      set ecpE0 [expr 15 + $X*$P/100 ]
      set ecpE1 [expr 15 + $X*$D/100 ]

      clockFrame $edgatclk_w
      setClockTrace $edgatclk_w
    }
  }

  ########################################
  #
  # Clock parameter edit window
  #
  proc  tabClockParms {w omega phi duty} {
    global ecpS ecpl1 ecpl2 ecpl3 ecpl4 ecpl5
    global ecpE0 ecpE1 ecpE2
    global ecpW ecpI ecpOmega ecpPhi ecpDuty
    global edgatclk_w 
    variable clock_line_len

    set edgatclk_w $w

    set IW [chooseW $omega]
    set ecpI [lindex $IW 0]
    set ecpW [lindex $IW 1]

    set ecpOmega $omega
    set ecpPhi $phi
    set ecpDuty $duty

    frame $edgatclk_w.cycle
    label $edgatclk_w.cycle.lab -text "[m db.gate.cyclew]: " -anchor e -width 18
    incdecEntry $edgatclk_w.cycle.ent -width 6 -variable ecpOmega -format %d
    pack $edgatclk_w.cycle.lab $edgatclk_w.cycle.ent -side left -pady 2

    frame $edgatclk_w.phase
    label $edgatclk_w.phase.lab -text "[m db.gate.phase](%): " -anchor e -width 18
    incdecEntry $edgatclk_w.phase.ent -width 6 -variable ecpPhi -format %d
    pack $edgatclk_w.phase.lab $edgatclk_w.phase.ent -side left -pady 2

    frame $edgatclk_w.duty
    label $edgatclk_w.duty.lab -text "[m db.gate.duty](%): " -anchor e -width 18
    incdecEntry $edgatclk_w.duty.ent -width 6 -variable ecpDuty -format %d
    pack $edgatclk_w.duty.lab $edgatclk_w.duty.ent -side left -pady 2

    bind $edgatclk_w.cycle.ent <KeyPress-Return> "EditGate::ok"
    bind $edgatclk_w.phase.ent <KeyPress-Return> "EditGate::ok"
    bind $edgatclk_w.duty.ent <KeyPress-Return> "EditGate::ok"

    canvas $edgatclk_w.wave -width [expr  $clock_line_len + 30] -height 110 -closeenough 5.0 -bg white -bd 2 -relief sunken
    $edgatclk_w.wave create line 15 75 [expr $clock_line_len + 15] 75
    clockFrame $w

    set ecpl1 [$edgatclk_w.wave create line 0 0 0 0 -tags edge ]
    set ecpl2 [$edgatclk_w.wave create line 0 0 0 0 ]
    set ecpl3 [$edgatclk_w.wave create line 0 0 0 0 -tags edge ]
    set ecpl4 [$edgatclk_w.wave create line 0 0 0 0 ]
    set ecpl5 [$edgatclk_w.wave create line 0 0 0 0 -tags edge ]

    clockPUpdate

    $edgatclk_w.wave bind edge <Button-1> {
      set ecpS [$edgatclk_w.wave find withtag current]
    }
    $edgatclk_w.wave bind edge <B1-Motion> "EditGate::moveClock $w %x"
    $edgatclk_w.wave bind edge <ButtonRelease-1> "EditGate::clockPUpdate"

    label $edgatclk_w.l -text [m db.gate.waveform]

    pack $edgatclk_w.l -anchor w 
    pack $edgatclk_w.wave 
    pack $edgatclk_w.cycle $edgatclk_w.phase $edgatclk_w.duty -anchor w

    pack $edgatclk_w -fill both -padx 5 -pady 20

    helpon $edgatclk_w.cycle.lab [m ho.edgat.cycle]
    helpon $edgatclk_w.phase.lab [m ho.edgat.phase]
    helpon $edgatclk_w.duty.lab [m ho.edgat.duty]
  }

  #############################################################################
  #
  # Gate delay properties
  #
  proc tabDelay {w} {
    global edgat_delays edgat_dtype edgat_techList edgat_tech edgat_techdelay

    frame $w.delay
    set wd $w.delay

    frame $wd.pad
    pack $wd.pad -pady 3

    frame $wd.tech_label
    radiobutton $wd.tech_label.b -text [m db.gate.stddelay] -variable edgat_dtype -value tech -command "EditGate::dvalSetState $wd"
#    eval "tk_optionMenu $wd.tech_label.opt edgat_tech $edgat_techList"

    Dropbox::new $wd.tech_label.opt -variable edgat_tech -width 10
    foreach item $edgat_techList {
      Dropbox::itemadd $wd.tech_label.opt $item
    }

    pack $wd.tech_label.b -side left
    pack $wd.tech_label.opt -side left -padx 3

#    radiobutton $wd.tech_label -text [m db.gate.stddelay] -variable edgat_dtype -value tech -command "EditGate::dvalSetState $wd"
    

    windowframe $wd.tech $wd.tech_label -bd 2 -relief groove -dy -3

#    radioframe $wd.tech [m db.gate.stddelay] -bd 2 -relief groove -variable edgat_dtype -value tech -command "EditGate::dvalSetState $wd"
#    radiobutton $wd.tech.b -text [m db.gate.stddelay] -variable edgat_dtype -value tech -command "EditGate::dvalSetState $wd"
#    eval "tk_optionMenu $wd.tech.opt edgat_tech $edgat_techList"

    frame $wd.tech.pad
    pack $wd.tech.pad -pady 5


    set idx 0
    set r 0
    set c 0
    frame $wd.tech.dtab
    set dtw $wd.tech.dtab
    foreach d $edgat_delays {
      label $dtw.l${r}_$c -text "$d:"
      label $dtw.e${r}_$c -textvariable edgat_techdelay($d) -width 4 -justify left

      grid $dtw.l${r}_$c -row $r -column [expr 3*$c] -padx 2 -pady 3  -sticky  e
      grid $dtw.e${r}_$c -row $r -column [expr 3*$c+1] -padx 2 -pady 3 -sticky w

      incr idx
      incr c
      if { $c == 3 } {
	set c 0
	incr r
      }
    }
    grid columnconfigure $wd.tech.dtab 2 -minsize 20
    grid columnconfigure $wd.tech.dtab 5 -minsize 20
    pack $wd.tech.dtab -padx 5 -pady 5 -side bottom
#    pack $wd.tech.opt  -padx 5 -pady 5 -anchor w -side left

    radioframe $wd.cust [m db.gate.custdelay] -bd 2 -relief groove -variable edgat_dtype -value cust -command  "EditGate::dvalSetState $wd"
#    radiobutton $wd.cust.b -text [m db.gate.custdelay] -variable edgat_dtype -value cust -command  "EditGate::dvalSetState $wd"
    frame $wd.cust.pad
    pack $wd.cust.pad -pady 5


    set r 0
    set c 0
    frame $wd.cust.dtab
    set dtw $wd.cust.dtab
    foreach d $edgat_delays {
      label $dtw.l${r}_$c -text "$d:" 
      entry $dtw.e${r}_$c -width 4 -textvariable edgat_delayvalue($d) -bg white

      grid $dtw.l${r}_$c -row $r -column [expr 3*$c] -padx 2 -pady 3 -sticky e
      grid $dtw.e${r}_$c -row $r -column [expr 3*$c+1] -padx 2 -pady 3

      incr c
      if { $c == 3 } {
	set c 0
	incr r
      }
    }
    grid columnconfigure $wd.cust.dtab 2 -minsize 20
    grid columnconfigure $wd.cust.dtab 5 -minsize 20
    pack $wd.cust.dtab -padx 5 -pady 5

    frame $wd.pad2
    pack $wd.tech $wd.pad2 $wd.cust -padx 5 -pady 3 -fill both
    pack $wd -padx 5 -pady 20 -fill both

    dvalSetState $wd
    gat_setGateTechDelays $edgat_tech

    helpon $wd.tech_label [m ho.techbut]
    helpon $wd.cust_label [m ho.custbut]
  }

  #############################################################################
  #
  # Gate properties for general properties shared by all gate types.
  #
  proc tabBasic {w args} {
    global edgat_type edgat_name edgat_hideName edgat_blockFunc

    if { [lindex $args 0] == "-raised" } {
      frame $w.basic -bd 1 -relief raised
    } else {
      frame $w.basic 
    }
    pack $w.basic

    frame $w.basic.props
    set wb $w.basic.props

    label $wb.type_l -text "[m db.gate.gtype]:" -anchor e
    label $wb.type_e -text "$edgat_type" -width 12 -anchor w
    grid $wb.type_l -row 0 -column 0 -sticky e -padx 5 -pady 3
    grid $wb.type_e -row 0 -column 1 -sticky w -padx 5 -pady 3

    label $wb.name_l -text "[m db.gate.gname]:" -anchor e
    entry $wb.name_e -width 15 -textvariable edgat_name -bg white
    grid $wb.name_l -row 1 -column 0 -sticky e -padx 5 -pady 3
    grid $wb.name_e -row 1 -column 1 -sticky w -padx 5 -pady 3

    if { $edgat_type != "COMMENT" &&  $edgat_type != "FRAME"  &&  $edgat_type != "SCRIPT" } {
      label $wb.hide_l -text "[m db.gate.hidenam]:" -anchor e
      checkbutton $wb.hide_e -variable edgat_hideName
      grid $wb.hide_l -row 2 -column 0 -sticky e -padx 5 -pady 3
      grid $wb.hide_e -row 2 -column 1 -sticky w -padx 5 -pady 3

      helpon $wb.hide_l [m ho.edgat.hide]
    }

    label $wb.anchor_l -text "[m db.gate.anchor]:" -anchor e
    frame $wb.anchor_f
    grid $wb.anchor_l -row 3 -column 0 -sticky e -padx 5 -pady 3
    grid $wb.anchor_f -row 3 -column 1 -sticky w -pady 3

    checkbutton $wb.anchor_f.ent -variable edgat_anchor
    label $wb.anchor_f.xlab -text "X:"
    entry $wb.anchor_f.xent -width 4 -textvariable edgat_xpos -bg white
    label $wb.anchor_f.ylab -text "Y:"
    entry $wb.anchor_f.yent -width 4 -textvariable edgat_ypos -bg white
    pack  $wb.anchor_f.ent $wb.anchor_f.xlab $wb.anchor_f.xent $wb.anchor_f.ylab $wb.anchor_f.yent -padx 5 -pady 3 -side left 

    if { $edgat_type != "Module" } {
      label $wb.cpbreak_l -text "[m db.gate.cpbreak]:" -anchor e
      checkbutton $wb.cpbreak_e -variable edgat_cpbreak
      grid $wb.cpbreak_l -row 4 -column 0 -sticky e -padx 5 -pady 3
      grid $wb.cpbreak_e -row 4 -column 1 -sticky w -padx 5 -pady 3
    }

    #  checkbutton $w.  

    bind $wb.name_e <KeyPress-Return> "EditGate::ok"

    
    if { [lindex $args 0] == "-raised" } {
      pack $wb -fill both
    } else {
      pack $wb -padx 5 -pady 20
    }

    helpon $wb.type_l [m ho.edgat.gtype]
    helpon $wb.name_l [m ho.edgat.gname]
    helpon $wb.anchor_l [m ho.edgat.ganchor]
    helpon $wb.anchor_f.xlab [m ho.edgat.gx]
    helpon $wb.anchor_f.ylab [m ho.edgat.gy]
  }

  proc dvalSetState {w} {
    global edgat_delays edgat_dtype

    catch {
      if { $edgat_dtype == "tech" } {
	set state "disabled"
      } else {
	set state "normal"
      }

      set r 0
      set c 0
      set tdtw $w.tech.dtab
      set dtw $w.cust.dtab
      foreach d $edgat_delays {
	$dtw.e${r}_$c configure -state $state

	if { $state == "normal" } {
	  $dtw.e${r}_$c configure -foreground black
	  $dtw.l${r}_$c configure -foreground black
	  $tdtw.e${r}_$c configure -foreground dimgray
	  $tdtw.l${r}_$c configure -foreground dimgray
	} else {
	  $dtw.e${r}_$c configure -foreground dimgray
	  $dtw.l${r}_$c configure -foreground dimgray
	  $tdtw.e${r}_$c configure -foreground black
	  $tdtw.l${r}_$c configure -foreground black
	}

	incr c
	if { $c == 3 } {
	  set c 0
	  incr r
	}
      }
    }
  }

  #
  # Pin placement properties of mux and demux.
  #
  proc tabMux {w} {
    global edgat_type

    switch $edgat_type {
      MUX {
	set prefix db.gate.mux
	set ord1 [gifI muxord1.gif]
	set ord2 [gifI muxord2.gif]
	set sel1 [gifI muxsel1.gif]
	set sel2 [gifI muxsel2.gif]

	set uord1 [gifI Umuxord1.gif]
	set uord2 [gifI Umuxord2.gif]
	set usel1 [gifI Umuxsel1.gif]
	set usel2 [gifI Umuxsel2.gif]
      }
      DECODER {
	set prefix db.gate.decoder
	set ord1 [gifI demuxord1.gif]
	set ord2 [gifI demuxord2.gif]
	set sel1 [gifI demuxsel1.gif]
	set sel2 [gifI demuxsel2.gif]

	set uord1 [gifI Udemuxord1.gif]
	set uord2 [gifI Udemuxord2.gif]
	set usel1 [gifI Udemuxsel1.gif]
	set usel2 [gifI Udemuxsel2.gif]
      }
      DEMUX {
	set prefix db.gate.demux
	set ord1 [gifI demuxord1.gif]
	set ord2 [gifI demuxord2.gif]
	set sel1 [gifI demuxsel1.gif]
	set sel2 [gifI demuxsel2.gif]

	set uord1 [gifI Udemuxord1.gif]
	set uord2 [gifI Udemuxord2.gif]
	set usel1 [gifI Udemuxsel1.gif]
	set usel2 [gifI Udemuxsel2.gif]
      }
    }

    set wraplength 130

    #############################################################################
    labelframe $w.dataord [m $prefix.dataorder]

    imageRadioButton $w.dataord.ascend -onimage $ord1 -offimage $uord1 -wraplength $wraplength \
	-variable edgat_dataOrder -value 0 -label [m $prefix.ord1.hdr] -description [m $prefix.ord1.exp]

    imageRadioButton $w.dataord.descend -onimage $ord2 -offimage $uord2 -wraplength $wraplength \
	-variable edgat_dataOrder -value 1 -label [m $prefix.ord2.hdr] -description [m $prefix.ord2.exp]

    frame $w.dataord.toppad -height 10
    frame $w.dataord.botpad -height 5
    pack $w.dataord.toppad
    pack $w.dataord.botpad -side bottom
    pack $w.dataord.ascend $w.dataord.descend -padx 4 -side left

    #############################################################################
    labelframe $w.selsty [m $prefix.select]


    imageRadioButton $w.selsty.onleft -onimage $sel1 -offimage $usel1 -wraplength $wraplength \
	-variable edgat_selectSide -value 0 -label [m $prefix.sel1.hdr] -description [m $prefix.sel1.exp]

    imageRadioButton $w.selsty.onright -onimage $sel2 -offimage $usel2 -wraplength $wraplength \
	-variable edgat_selectSide -value 1 -label [m $prefix.sel2.hdr] -description [m $prefix.sel2.exp]

    frame $w.selsty.toppad -height 10
    frame $w.selsty.botpad -height 5
    pack $w.selsty.toppad
    pack $w.selsty.botpad -side bottom
    pack $w.selsty.onleft $w.selsty.onright -padx 4 -side left

    #############################################################################

    frame $w.pad -height 10
    frame $w.pad2 -height 10

    pack $w.pad
    pack $w.dataord -anchor w -fill both -padx 5
    pack $w.pad2
    pack $w.selsty -anchor w -fill both -padx 5
  }

  #############################################################################
  #
  # Gate property tab for flip-flop gates.
  #
  proc tabFF {w ff} {
    global edgat_ffmirror

    labelframe $w.fftype [m db.gate.$ff.type]

    set wraplength 130

    imageRadioButton $w.fftype.normal -onimage [gifI $ff.gif] -offimage [gifI U$ff.gif] -wraplength $wraplength \
	-variable edgat_ffmirror -value 0 -label [m db.gate.$ff.normal.hdr] -description [m db.gate.$ff.normal.exp]

    imageRadioButton $w.fftype.reverse -onimage [gifI r$ff.gif] -offimage [gifI Ur$ff.gif] -wraplength $wraplength \
	-variable edgat_ffmirror -value 1 -label [m db.gate.$ff.reverse.hdr] -description [m db.gate.$ff.reverse.exp]

    frame $w.fftype.toppad -height 10
    frame $w.fftype.botpad -height 5
    pack $w.fftype.toppad
    pack $w.fftype.botpad -side bottom
    pack $w.fftype.normal $w.fftype.reverse -padx 4 -side left

    pack $w.fftype -fill both -padx 5 -pady 20
  }

  #############################################################################
  #
  # Gate property tab for concat gates.
  #
  proc tabConcatPlace {w} {
    labelframe $w.catdir [m db.gate.catdiruse]

    set wraplength 130

    #
    # The "ascending" button
    #
    imageRadioButton $w.catdir.ascend -onimage [gifI cat_asc.gif] -offimage [gifI Ucat_asc.gif] -wraplength $wraplength \
	-variable edgat_catdir -value 1 -label [m db.gate.cat.asc.hdr] -description [m db.gate.cat.asc.exp]

    #
    # The "descending" button
    #
    imageRadioButton $w.catdir.descend -onimage [gifI cat_dec.gif] -offimage [gifI Ucat_dec.gif] -wraplength $wraplength \
	-variable edgat_catdir -value 0 -label [m db.gate.cat.dec.hdr] -description [m db.gate.cat.dec.exp]

    #
    # Button layout
    #
    grid $w.catdir.ascend -row 0 -column 0 -padx 3 -pady 8 -sticky nw
    grid $w.catdir.descend -row 0 -column 1 -padx 3 -pady 8 -sticky nw

    #############################################################################
    labelframe $w.sigdir [m db.gate.cat.sigdir]


    frame $w.sigdir.toppad -height 5
    frame $w.sigdir.botpad -height 5

    #
    # The "auto" button
    #
    imageRadioButton $w.sigdir.auto -onimage [gifI cat_auto.gif] -offimage [gifI Ucat_auto.gif] -wraplength $wraplength \
	-variable edgat_sigdir -value 2 -label [m db.gate.cat.auto.hdr] -description [m db.gate.cat.auto.exp]

    #
    # The "Multi-Input" button
    #
    imageRadioButton $w.sigdir.multin -onimage [gifI cat_multin.gif] -offimage [gifI Ucat_multin.gif] -wraplength $wraplength \
	-variable edgat_sigdir -value 1 -label [m db.gate.cat.multin.hdr] -description [m db.gate.cat.multin.exp]

    #
    # The "Single-Input" button
    #
    imageRadioButton $w.sigdir.singin -onimage [gifI cat_singin.gif] -offimage [gifI Ucat_singin.gif] -wraplength $wraplength \
	-variable edgat_sigdir -value 0 -label [m db.gate.cat.singin.hdr] -description [m db.gate.cat.singin.exp]

    #
    # The "tran" button
    #
    imageRadioButton $w.sigdir.tran -onimage [gifI cat_tran.gif] -offimage [gifI Ucat_tran.gif] -wraplength $wraplength \
	-variable edgat_sigdir -value 3 -label [m db.gate.cat.tran.hdr] -description [m db.gate.cat.tran.exp]

    grid $w.sigdir.toppad -row 0
    grid $w.sigdir.auto   -row 1 -column 0 -padx 3 -pady 3 -sticky nw
    grid $w.sigdir.multin	-row 1 -column 1 -padx 3 -pady 3 -sticky nw
    grid $w.sigdir.tran   -row 2 -column 0 -padx 3 -pady 3 -sticky nw
    grid $w.sigdir.singin -row 2 -column 1 -padx 3 -pady 3 -sticky nw
    grid $w.sigdir.botpad -row 3

    #############################################################################

    frame $w.pad -height 10
    frame $w.pad2 -height 10

    pack $w.pad
    pack $w.catdir -anchor w -fill both -padx 5
    pack $w.pad2
    pack $w.sigdir -anchor w -fill both -padx 5
  }

  #############################################################################
  #
  # Gate property tab for gates that can have extender bars.
  #
  proc tabCrunchExtGate {w} {
    global edgat_extbar

    frame $w.pad -height 10
    pack $w.pad

    set wraplength 130

    #############################################################################
    labelframe $w.extbar [m db.gate.aox.extbaruse]

    imageRadioButton $w.extbar.use -onimage [gifI extbarand.gif] -offimage [gifI Uextbarand.gif] -wraplength $wraplength \
	-variable edgat_extbar -value 1 -label [m db.gate.aox.use.hdr] -description [m db.gate.aox.use.exp]

    imageRadioButton $w.extbar.nouse -onimage [gifI crunchand.gif] -offimage [gifI Ucrunchand.gif] -wraplength $wraplength \
	-variable edgat_extbar -value 0 -label [m db.gate.aox.nouse.hdr] -description [m db.gate.aox.nouse.exp]

    frame $w.extbar.toppad -height 10
    frame $w.extbar.botpad -height 10
    pack $w.extbar.toppad
    pack $w.extbar.botpad -side bottom
    pack $w.extbar.use $w.extbar.nouse -padx 4 -side left

    #############################################################################
    pack $w.extbar -anchor w -fill both -padx 5

    #  helpon $w.extbar.use.b [m ho.edgat.extbar]
    #  helpon $w.extbar.nouse.b [m ho.edgat.extbar]
    #  helpon $w.extbar.use.i [m ho.edgat.extbar]
    #  helpon $w.extbar.nouse.i [m ho.edgat.extbar]
  }

  #############################################################################
  #
  # Gate property tab for setting Switch properties.
  #
  proc tabSwitch {w} {
    global edgat_switch

    set wraplength 130

    labelframe $w.state [m db.gate.switch.state]
    packPad $w.state
    imageRadioButton $w.state.on -onimage [gifI onswitch.gif] -offimage [gifI Uonswitch.gif] \
	-wraplength $wraplength -variable edgat_switch -value 1 \
	-label [m db.gate.switch.state.on.hdr] -description [m db.gate.switch.state.on.exp]

    imageRadioButton $w.state.off -onimage [gifI offswitch.gif] -offimage [gifI Uoffswitch.gif] \
	-wraplength $wraplength -variable edgat_switch -value 0 \
	-label [m db.gate.switch.state.off.hdr] -description [m db.gate.switch.state.off.exp]
    pack $w.state.on $w.state.off -padx 4 -pady 5 -side left

    labelframe $w.shownet [m db.gate.switch.shownet]
    packPad $w.shownet
    imageRadioButton $w.shownet.on -onimage [gifI wsswitch.gif] -offimage [gifI Uwsswitch.gif] \
	-wraplength $wraplength -variable edgat_shownet -value 1 \
	-label [m db.gate.switch.shownet.on.hdr] -description [m db.gate.switch.shownet.on.exp]
    imageRadioButton $w.shownet.off -onimage [gifI nwsswitch.gif] -offimage [gifI Unwsswitch.gif] \
	-wraplength $wraplength -variable edgat_shownet -value 0 \
	-label [m db.gate.switch.shownet.off.hdr] -description [m db.gate.switch.shownet.off.exp]
    pack $w.shownet.on $w.shownet.off -padx 4 -pady 5 -side left -anchor n

    packPad $w
    pack $w.state -fill both -padx 5 -pady 5
    pack $w.shownet -fill both -padx 5 -pady 5

#    helpon $w.switch.state [m ho.edgat.swstate]
  }

  proc dipValChange {w args} {
    global edgat_dip
    
    set n $edgat_dip
    if { $n == ""} { set n "0" }

    $w.dip.c itemconfigure value -text $n
  }

  #############################################################################
  #
  # Gate property tab for setting wire tab properties.
  #
  proc tabDip {w} {
    global edgat_dip

    #
    # State Property 
    #
    labelframe $w.state [m db.gate.dip.value]
    packPad $w.state

    canvas $w.state.c -width 150 -height 70 -bg white -bd 1 -relief sunken
    $w.state.c create image 100 35 -image [gifI setdip.gif]
    $w.state.c create text 100 18 -text $edgat_dip -font dipFont -fill blue -tags value

    set wrap [rescale [m @db.gate.dip.value.exp.width]]
    label $w.state.e -text [m db.gate.dip.value.exp] -justify left -wraplength $wrap -font dialogExpFont
    label $w.state.l -text [m db.gate.dip.value.hex]
    incdecEntry $w.state.ent -width 12 -variable edgat_dip -format %x -validatecommand hexValidate

    pack $w.state.e -side left -fill both -padx 5
    pack $w.state.c -padx 5 -pady 5 -side bottom -fill x

    pack $w.state.ent -padx 5 -pady 5 -side right -anchor w
    pack $w.state.l -side right -anchor w

    trace variable ::edgat_dip w "EditGate::dipValChange $w"
    bind $w.state <Destroy> "trace vdelete ::edgat_dip w \"EditGate::dipValChange $w\""

    #
    # Show Net Property 
    #
    set wraplength 130
    labelframe $w.shownet [m db.gate.dip.shownet]
    packPad $w.shownet
    imageRadioButton $w.shownet.on -onimage [gifI wsdip.gif] -offimage [gifI Uwsdip.gif] \
	-wraplength $wraplength -variable edgat_shownet -value 1 \
	-label [m db.gate.dip.shownet.on.hdr] -description [m db.gate.dip.shownet.on.exp]
    imageRadioButton $w.shownet.off -onimage [gifI nwsdip.gif] -offimage [gifI Unwsdip.gif] \
	-wraplength $wraplength -variable edgat_shownet -value 0 \
	-label [m db.gate.dip.shownet.off.hdr] -description [m db.gate.dip.shownet.off.exp]
    pack $w.shownet.on $w.shownet.off -padx 4 -pady 5 -side left -anchor n

    #
    # Final layout
    #
    packPad $w
    pack $w.state -fill both -padx 5 -pady 5
    pack $w.shownet -fill both -padx 5 -pady 5

#    frame $w.dip
#    label $w.dip.label -text "[m db.gate.dipval]:"
#    entry $w.dip.state -textvariable edgat_dip -bg white
#    pack $w.dip.label $w.dip.state -padx 5 -pady 5 -side left -anchor w
#    bind $w.dip.state <KeyPress-Return> "EditGate::ok"

#    helpon $w.dip.label [m ho.edgat.dipstate]
  }


  proc tabTapUpdateRange {w args} {
    global edgat_tap
    $w.tap.c itemconfig range -text $edgat_tap
  }

  proc tabTapBuild {w args} {
    global edgat_tap edgatTap_lsb
    global tkg_wireColor tkg_busColor tkg_instColor
    variable tap_size
    variable wire_size
    variable tap_lsbMax

    set n [SpreadSheet::size $PortMgr::iolb_w]
    for { set i 0} {$i < $n} {incr i} {
      set row [SpreadSheet::get $PortMgr::iolb_w $i]
      set name [lindex $row 1]
      set bits [lindex $row 3]
      if { $name == "Z" } { set tap_size $bits }
      if { $name == "D" } { set wire_size $bits }
    }

    #
    # The wire sizing is currently illegal, so just treat
    # tap as the same size as the wire.
    #
    if { $tap_size > $wire_size} {
      set tap_size $wire_size
    }

    #
    # Maximum value for LSB
    #
    set tap_lsbMax [expr $wire_size-$tap_size]

    #
    # Fix LSB if it is out of range
    #
    if {$edgatTap_lsb > $tap_lsbMax} {
      set edgatTap_lsb $tap_lsbMax
    }

    incdecEntry_configure $w.tap.lsb -max $tap_lsbMax

    $w.tap.c delete all

    #
    # wire tap position
    #
    set width [$w.tap.c cget -width]
    set height [$w.tap.c cget -height]
    set x 20
    set y [expr $height/2]
    set tap_len [expr $width - 25]
    set wire_len [expr $height/2 -5]
    set x1 [expr $x + $tap_len]
    set y1 [expr $y - $wire_len]
    set y2 [expr $y + $wire_len]

    if { $tap_size > 1 } {
      $w.tap.c create line $x $y $x1 $y -width 2 -fill $tkg_busColor

      set x3 [expr $x + $tap_len - 15]
      
      $w.tap.c create line [expr $x3-4] [expr $y+4] [expr $x3+4] [expr $y-4] -fill red
      $w.tap.c create text $x3 [expr $y-8] -text $tap_size -fill red -anchor s

      $w.tap.exp configure -text [m db.gate.tap.exp.bus]
      $w.tap.l configure -text [m db.gate.tap.lbl.bus]
    } else {
      $w.tap.c create line $x $y $x1 $y -width 1 -fill $tkg_wireColor

      $w.tap.exp configure -text [m db.gate.tap.exp.bit]
      $w.tap.l configure -text [m db.gate.tap.lbl.bit]
    }
    if { $wire_size > 1 } {
      $w.tap.c create line $x $y1 $x $y2 -width 2 -fill $tkg_busColor

      set y3 [expr ($y+$y2)/2]

      $w.tap.c create line [expr $x-4] [expr $y3+4] [expr $x+4] [expr $y3-4] -fill red
      $w.tap.c create text [expr $x+8] $y3 -text $wire_size -fill red -anchor w
    } else {
      $w.tap.c create line $x $y1 $x $y2 -width 1 -fill $tkg_wireColor
    }
    $w.tap.c create polygon [expr $x+1] [expr $y -5]  [expr $x + 5] $y  [expr $x+1] [expr $y + 5] -fill $tkg_instColor
    $w.tap.c create text [expr $x+8] [expr $y-5] -text $edgat_tap -anchor sw -fill $tkg_instColor -tags range
  }

  proc tabTapUpdateLSB {w args} {
    global edgat_tap edgatTap_lsb
    variable tap_size
    variable wire_size
    variable tap_lsbMax

    #
    # Do range checking of lsb before we use it for bit-tap expression
    #
    set lsb $edgatTap_lsb

    #
    # Fix LSB if it is out of range
    #
    if {$lsb > $tap_lsbMax} {
      set lsb $tap_lsbMax
    }
    if {$lsb < 0} {
      set lsb 0
    }


    if { $tap_size > 1 } {
      set edgat_tap [expr $lsb + $tap_size - 1]:$lsb
    } else {
      set edgat_tap $lsb
    }
  }

  #############################################################################
  #
  # Gate property tab for setting wire tab properties.
  #
  proc tabTap {w} {
    global edgat_tap edgatTap_lsb
    variable tap_size
    variable wire_size

    set tap_size 0
    set wire_size 0

    labelframe $w.tap [m db.gate.tap.select]
    packPad $w.tap

    if { [scan $edgat_tap %*d:%d edgatTap_lsb] == 1} {
    } else {
      set edgatTap_lsb $edgat_tap
    }

    label $w.tap.exp -wraplength [mr @db.gate.tap.exp.width] -justify left
    label $w.tap.l
    incdecEntry $w.tap.lsb -width 5 -variable edgatTap_lsb -format %d -min 0

    canvas $w.tap.c -width 100 -height 100 -bg white -bd 1 -relief sunken
    tabTapBuild $w

    pack $w.tap.c -padx 5 -pady 5 -side right
    pack $w.tap.exp -padx 5 -pady 5 -side top -anchor w
    pack $w.tap.l -padx 5 -pady 5 -side left -anchor w
    pack $w.tap.lsb -padx 5 -pady 5 -side left -anchor w


    pack $w.tap -fill both -padx 5 -pady 20

    trace variable ::edgat_portOpCount w "EditGate::tabTapBuild $w"
    trace variable ::edgat_tap w "EditGate::tabTapUpdateRange $w"
    trace variable ::edgatTap_lsb w "EditGate::tabTapUpdateLSB $w"
    bind $w <Destroy> "trace vdelete ::edgat_portOpCount w \"EditGate::tabTapBuild $w\""
    bind $w <Destroy> "+ trace vdelete ::edgat_tap w \"EditGate::tabTapUpdateRange $w\""
    bind $w <Destroy> "+ trace vdelete ::edgatTap_lsb w \"EditGate::tabTapUpdateLSB $w\""
  }

  #############################################################################
  #
  # Gate property tab for setting Memory properties.
  #
  proc tabMem {w} {
    global edgat_memFile edgatmem_w

    set edgatmem_w $w.mem
    frame $w.mem
    label $w.mem.label -text "[m db.gate.memfilenam]:"
    entry $w.mem.file -textvariable edgat_memFile -bg white
    button $w.mem.browse -text [m b.browse]  -command {
      global mem_filter mem_filetypes edgate_memFile edgatmem_w
      set file [tk_getOpenFile -defaultextension $mem_filter -filetypes $mem_filetypes -parent .edgat]
      if { $file != "" } { 
	set edgat_memFile $file
	$edgatmem_w.file icursor end
	$edgatmem_w.file xview end
      }
    }

    pack $w.mem.label $w.mem.file $w.mem.browse -padx 5 -pady 5 -side left -anchor w
    pack $w.mem -fill both -padx 5 -pady 20
    bind $w.mem.file <KeyPress-Return> "EditGate::ok"

    helpon $w.mem.label [m ho.edgat.memfile]
  }

  #############################################################################
  #
  # User requested a new module on the edgat box
  #
  proc newModule {w} {
    global edgat_blockFunc
    BlockOp::new -parent .edgat

    if { $BlockOp::props(ok) == 1 } {
      Dropbox::flush $w.mod.funcF.name
      foreach m [BlockList::getallmodules -types {module part}] {
	Dropbox::itemadd $w.mod.funcF.name     $m
      }
      set edgat_blockFunc $BlockOp::props(name)
    }
  }

  #############################################################################
  #
  # Gate property tab for setting Module properties.
  #
  proc tabModule {w} {
    global edgat_newBlock edgat_blockFunc

    frame $w.mod


    label $w.mod.funcL -text "[m db.gate.modname]:" -width 12 -anchor e
    frame $w.mod.funcF

    tkg_moduleSelector $w.mod.funcF.name -width 20 -variable edgat_blockFunc -value $edgat_blockFunc -entry 0 -nomain 1
    button $w.mod.funcF.new -text [m b.new]... -command "EditGate::newModule $w"
    pack $w.mod.funcF.name $w.mod.funcF.new -anchor w -side left -padx 5

    pack $w.mod.funcL -padx 5 -pady 5 -side left 
    pack $w.mod.funcF -pady 5 -side left

    focus $w.mod.funcF.name.e

    helpon $w.mod.funcL [m ho.edgat.func]
    pack $w.mod -fill both -padx 5 -pady 20
  }

  #############################################################################
  #
  # Gate property tab for setting Frame properties.
  #
  proc tabFrame {w} {
    global edgat_newBlock

    frame $w.frm
    label $w.frm.lab -text "[m b.label]:" -anchor e
    entry $w.frm.ent -width 30 -textvariable edgat_frameLabel -bg white
    bind $w.frm.ent <KeyPress-Return> "EditGate::ok"
    pack $w.frm.lab -padx 5 -pady 5 -side left
    pack $w.frm.ent -padx 5 -pady 5 -side left -fill both -expand 1
    pack $w.frm -anchor w
    if { $edgat_newBlock } {
      #    $w.frm.ent selection range 0 end
      focus $w.frm.ent
    }
    helpon $w.frm.lab [m ho.edgat.frame]
    pack $w.frm -fill both -padx 5 -pady 20

    focus $w.frm.ent
    $w.frm.ent selection range 0 end
    $w.frm.ent icursor end
  }

  #############################################################################
  #
  # Gate property tab for setting LED properties.
  #
  proc tabLED {w} {
    global edgat_newBlock

    set wraplength 130

    labelframe $w.led [m db.gate.led.type]

    imageRadioButton $w.led.bit -onimage [gifI led_bit.gif] -offimage [gifI Uled_bit.gif] -wraplength $wraplength \
	-variable edgat_ledType -value 0 -label [m db.gate.led.bit.hdr] -description [m db.gate.led.bit.exp]

    imageRadioButton $w.led.bar -onimage [gifI led_bar.gif] -offimage [gifI Uled_bar.gif] -wraplength $wraplength \
	-variable edgat_ledType -value 1 -label [m db.gate.led.bar.hdr] -description [m db.gate.led.bar.exp]

    imageRadioButton $w.led.hex -onimage [gifI led_hex.gif] -offimage [gifI Uled_hex.gif] -wraplength $wraplength \
	-variable edgat_ledType -value 2 -label [m db.gate.led.hex.hdr] -description [m db.gate.led.hex.exp]

    imageRadioButton $w.led.dec -onimage [gifI led_dec.gif] -offimage [gifI Uled_dec.gif] -wraplength $wraplength \
	-variable edgat_ledType -value 3 -label [m db.gate.led.dec.hdr] -description [m db.gate.led.dec.exp]

    imageRadioButton $w.led.seg -onimage [gifI led_direct.gif] -offimage [gifI Uled_direct.gif] -wraplength $wraplength \
	-variable edgat_ledType -value 4 -label [m db.gate.led.seg.hdr] -description [m db.gate.led.seg.exp]


    frame $w.led.pad1 -height 10
    frame $w.led.pad2 -height 10
    frame $w.led.pad3 -height 10
    frame $w.led.pad4 -height 10

    grid $w.led.pad1 -row 0 -column 0
    grid $w.led.bit  -row 1 -column 0 -sticky w
    grid $w.led.bar  -row 1 -column 1 -sticky w

    grid $w.led.pad2 -row 2 -column 0
    grid $w.led.hex  -row 3 -column 0 -sticky w
    grid $w.led.dec  -row 3 -column 1 -sticky w

    grid $w.led.pad3 -row 4 -column 0
    grid $w.led.seg  -row 5 -column 0 -sticky w

    grid $w.led.pad4 -row 6 -column 0

    pack $w.led -fill both -padx 5 -pady 20
  }

  #############################################################################
  #
  # This procedure is called when the OK button is pressed
  #
  proc ok {} {
    global edgat_ok edgat_type edgat_blockFunc

    set edgat_ok 1

    switch $edgat_type {
      COMMENT {
	# The saveComment will fail if the Text tab is not active, but that is ok
	# since in this case the edgat_commentLines() array will be up to date. 
	# JPH 1/19/09 - The above statement may no longer be true due to the new
	# tabbox implementation.  We may not need the "catch" anymore.
	catch { saveComment }
      }
      SCRIPT {
	# Reusing comment editing for script editing
	catch { saveComment }
      }
      MODULE {
	if { $edgat_blockFunc == "" } {
	  errmsg [m err.nofunc]
	  set edgat_ok 0
	}
      }
      SYMMODULE {
	if { $edgat_blockFunc == "" } {
	  errmsg [m err.nofunc]
	  set edgat_ok 0
	}
      }
    }

    destroy .edgat
  }

  #############################################################################
  #
  # This procedure is called when the Cancel button is pressed
  #
  proc cancel {} {
    global edgat_ok

    set edgat_ok 0
    destroy .edgat
  }

  proc saveScript {w} { 
    global edgat_commentLen edgat_commentLines edgat_ok

    for { set i 1 } { [$w compare $i.end < end] } { incr i } {
      set x [$w get $i.0 $i.end]
      set j [expr $i - 1]
      set edgat_commentLines($j) $x
    }
    set edgat_commentLen [expr $i - 1]

    HdlEditor::dbHide
  }

  proc tabScriptText {w} {
    global edgat_commentLines edgat_commentLen
    global edgat_comment_w

    HdlEditor::new $w.script -width 1 -height 1 -keygroup hdledit
    pack $w.script -expand 1 -fill both
    HdlEditor::dbShow $w.script


    set tw $w.script.pane.main.t

    for {set i 0 } { $i < $edgat_commentLen } { incr i } {
      if { $i > 0 } { $tw insert end "\n" }
      $tw insert end  $edgat_commentLines($i)
    }
    HdlEditor::recolorize 1.0 end 

    frame $tw.destroyDetect
    bind $tw.destroyDetect <Destroy> "EditGate::saveScript $tw"
  }

  #############################################################################
  #
  # Save contents of the comment text
  #
  proc saveComment {} {
    global edgat_commentLen edgat_commentLines edgat_ok
    global edgat_comment_w

    set w $edgat_comment_w

    for { set i 1 } { [$w.f.t compare $i.end < end] } { incr i } {
      set x [$w.f.t get $i.0 $i.end]
      set j [expr $i - 1]
      set edgat_commentLines($j) $x
    }
    set edgat_commentLen [expr $i - 1]
  }

  #############################################################################
  #
  # Gate property tab for editing comment text.
  #
  proc tabCommentText {w} {
    global edgat_commentLines edgat_commentLen
    global edgat_comment_w

    frame $w.ctext
    set edgat_comment_w $w.ctext

    frame $w.ctext.f
    text $w.ctext.f.t -yscrollcommand "$w.ctext.f.vb set" -xscrollcommand "$w.ctext.f.hb set" -width 1 -height 1 -bg white
# -width [m @db.gate.comment.width] -height [m @db.gate.comment.height]
    scrollbar $w.ctext.f.vb -command "$w.ctext.f.t yview"
    scrollbar $w.ctext.f.hb -command "$w.ctext.f.t xview" -orient horizontal

    grid rowconfig $w.ctext.f 0  -weight 1 -minsize 0
    grid columnconfig $w.ctext.f 0 -weight 1 -minsize 0

    grid $w.ctext.f.t -row 0 -column 0 -sticky news
    grid $w.ctext.f.vb -row 0 -column 1 -sticky news
    grid $w.ctext.f.hb -row 1 -column 0 -sticky news
    pack $w.ctext.f -ipadx 5 -ipady 5 -fill both -expand 1 -padx 5 -pady 5

    for {set i 0 } { $i < $edgat_commentLen } { incr i } {
      if { $i > 0 } { $w.ctext.f.t insert end "\n" }
      $w.ctext.f.t insert end  $edgat_commentLines($i)
    }

    #
    # This sub-window is created but not packed.  It is used to detect a destroy of the
    # text window and get control before the text window is actually destroyed.
    #
    frame $w.ctext.f.t.destroyDetect
    bind $w.ctext.f.t.destroyDetect <Destroy> EditGate::saveComment

    pack $w.ctext -fill both -expand 1

    focus $w.ctext.f.t
  }

  #############################################################################
  #
  # Handle a tab selection in the edit gate dialog box
  #
  proc dotab {w tab} {
    global edgat_type edgat_name edgat_hideName edgat_geom
    global ecpOmega ecpPhi ecpDuty
    global editgate_caps

    frame $w
    frame $w.props
    pack $w.props -side right -fill both -expand 1

    set gif general.gif
    set explaination [m db.gate.tab.${tab}.exp]
    set caption [m db.gate.tab.${tab}.cap]

    switch $tab {
      General {
	tabBasic $w.props
	set gif gateprops.gif
      }
      Ports {
	if { [catch { set caplist $editgate_caps($edgat_type)}]} {
	  set caplist { sig size }
	}
	PortMgr::create $w.props -editcaps $caplist
	#      edgatIO $w.props
	set gif port.gif
      }
      Delay {
	tabDelay $w.props
	set gif delay.gif
      }
      Parameters {
	ModParmMgr::create $w.props
      }
      CLOCK {
	tabClockParms $w.props $ecpOmega $ecpPhi $ecpDuty
	set gif details.gif
      }
      SWITCH {
	tabSwitch $w.props
	set gif details.gif
      }
      DIP {
	tabDip $w.props
	set gif details.gif
      }
      TAP { 
	tabTap $w.props
	set gif details.gif
      }
      RAM {
	tabMem $w.props
	set gif details.gif
      }
      ROM {
	tabMem $w.props
	set gif details.gif
      }
      MODULE  {
	tabModule $w.props
	set gif details.gif
      }
      FRAME {
	tabFrame $w.props
	set gif details.gif
      }
      LED {
	tabLED $w.props
	set gif details.gif
      }
      AND {
	tabCrunchExtGate $w.props
	set gif details.gif
      }
      OR {
	tabCrunchExtGate $w.props
	set gif details.gif
      }
      XOR {
	tabCrunchExtGate $w.props
	set gif details.gif
      }
      MUX {
	tabMux $w.props
	set gif details.gif
      }
      DEMUX {
	tabMux $w.props
	set gif details.gif
      }
      DECODER {
	tabMux $w.props
	set gif details.gif
      }
      FF {
	tabFF $w.props ff
	set gif details.gif
      }
      JKFF {
	tabFF $w.props jkff
	set gif details.gif
      }
      CONCAT {
	tabConcatPlace $w.props
	set gif details.gif
      }
      COMMENT {
	tabCommentText $w.props
	set gif details.gif
      }
      SCRIPT {
	tabScriptText $w.props
	set gif details.gif
      }
    }

    dialogImage $w.image -image [gifI $gif] -caption $caption -explaination $explaination
    pack $w.image -fill both -side left -pady 10
  }

  #############################################################################
  #
  # Post the gate parameters dialog box
  #
  proc post args {
    global edgat_type edgat_name edgat_hideName edgat_geom edgat_wops edgat_dtype
    global ecpOmega ecpPhi ecpDuty

#    puts "EditGate::post type=$edgat_type"

    set edgat_wops {}

    set start_page General

    if { $args != "" } {
      set start_page $args
    }

    set w .edgat
    if { [catch { toplevel $w }] } { return }
    wm withdraw $w

    wm title $w [m db.gate.title]
    wm geometry $w $edgat_geom
    wm transient $w .

    switch $edgat_type {
      COMMENT {
	set start_page COMMENT
	wm geometry $w [mr @db.gate.comment.width]x[mr @db.gate.comment.height]
      }
      SCRIPT {
	set start_page SCRIPT
	wm geometry $w [mr @db.gate.comment.width]x[mr @db.gate.comment.height]
      }
      FRAME {
	set start_page FRAME
	wm resizable $w 0 0
      }
      default {
	wm resizable $w 0 0
      }
    }


    set tabs { General }
    set labels [list [m db.gate.general]]

    if { $edgat_type != "FRAME" && $edgat_type != "COMMENT"  && $edgat_type != "SCRIPT" } {
      lappend tabs Ports
      lappend labels [m db.gate.ports]
    }

    #
    # If this is a gate type that has a details tab, add the tab
    #
    if {[lsearch {CLOCK SWITCH DIP TAP RAM ROM MODULE FRAME LED AND OR XOR MUX DECODER DEMUX JKFF FF CONCAT} $edgat_type] >= 0 } {
      lappend tabs $edgat_type; lappend labels [m db.gate.details]
    }

    if { $edgat_type == "MODULE" || $edgat_type == "SYMMODULE" } { 
      lappend tabs Parameters
      lappend labels [m db.gate.parameters]
    }

    #
    # Add special tab for comments
    #
    if { $edgat_type == "COMMENT" } {
      lappend tabs COMMENT
      lappend labels [m db.gate.text]
    }

    if { $edgat_type == "SCRIPT" } {
      lappend tabs SCRIPT
      lappend labels [m db.gate.text]
    }

    if { $edgat_dtype != "none" } {
      lappend tabs Delay
      lappend labels [m db.gate.delay]
    }

    TabBox::new $w.tbox -command EditGate::dotab -tabs $tabs -tablabels $labels -startpage $start_page -takefocus 1
    pack $w.tbox -fill both -expand 1

    okcancel $w.ok -okcommand EditGate::ok -cancelcommand EditGate::cancel
    pack $w.ok -fill both

    trace variable ecpOmega w EditGate::clockPUpdate
    trace variable ecpPhi w EditGate::clockPUpdate
    trace variable ecpDuty w EditGate::clockPUpdate

    wm deiconify $w
    dialogWait $w

    trace vdelete ecpOmega w EditGate::clockPUpdate
    trace vdelete ecpPhi w EditGate::clockPUpdate
    trace vdelete ecpDuty w EditGate::clockPUpdate
  }
}
