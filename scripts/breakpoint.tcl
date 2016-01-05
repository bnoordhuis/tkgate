#############################################################################
#
# breakpoint handling
#
namespace eval Breakpoint {
  variable bp_w
  variable bplb_w
  variable stateImage

  array set stateImage {
    -1	bp_bad.gif
    0	bp_go.gif
    1	bp_stop.gif
    2	bp_disabled.gif
    3	bp_standby.gif
  }
  #
  # Images for various states
  #
  #      bp_stop.gif
  #      bp_go.gif
  #      bp_bad.gif
  #      bp_disabled.gif
  #      bp_standby.gif
  #


  #############################################################################
  #
  # Create the breakpoint editor.
  #
  proc create {w args} {
    variable bp_w
    variable bplb_w

    set bp_w $w
    frame $w

    frame $w.actions
    button $w.actions.add -text [m mm.add] -command Breakpoint::add -width [m @mm.width]
    button $w.actions.delete -text [m mm.delete] -command Breakpoint::deleteSelection -width [m @mm.width]
    button $w.actions.edit -text [m mm.edit] -command Breakpoint::edit -width [m @mm.width]
    button $w.actions.enable -text [m mm.enable] -command Breakpoint::enableSelection -width [m @mm.width]
    button $w.actions.disable -text [m mm.disable] -command Breakpoint::disableSelection -width [m @mm.width]
    set i 0
    foreach {b1 b2} {add enable delete disable edit .} {
      if { $b1 != "." } {
	grid $w.actions.$b1 -row $i -column 0 -padx 1 -pady 1 -sticky ew
      }
      if { $b2 != "." } {
	grid $w.actions.$b2 -row $i -column 1 -padx 1 -pady 1 -sticky ew
      }
      incr i
    }

#    frame $w.actions
#    button $w.actions.enable -text [m mm.enable] -command Breakpoint::enableSelection -width [m @mm.width]
#    button $w.actions.disable -text [m mm.disable] -command Breakpoint::disableSelection -width [m @mm.width]
#    button $w.actions.delete -text [m mm.delete] -command Breakpoint::deleteSelection -width [m @mm.width]
#    pack $w.actions.enable $w.actions.disable $w.actions.delete -fill x -padx 1 -pady 1


    pack $w.actions -side right -fill x -anchor nw

    frame $w.t
    pack $w.t -fill both

    frame $w.t.bplist
    
    set bplb_w $w.t.bplist.lb
    SpreadSheet::create $w.t.bplist.lb  -height 12 -bd 2 -relief sunken -yscrollcommand "$w.t.bplist.vb set" \
	  -entrycommand Breakpoint::manager -expandcol 2 -resize 1
    SpreadSheet::addcolumn $w.t.bplist.lb -width 4 -header [m break.id]
    SpreadSheet::addcolumn $w.t.bplist.lb -width 3 -header [m break.state] -type image
    SpreadSheet::addcolumn $w.t.bplist.lb -width 50 -header [m break.condition]
    SpreadSheet::addcolumn $w.t.bplist.lb -width 12 -header [m break.value]
    pack $w.t.bplist.lb -side left -fill both -expand 1

    scrollbar $w.t.bplist.vb -orient vertical -command "SpreadSheet::yview $w.t.bplist.lb"
    pack $w.t.bplist.vb -side left -fill y

    pack $w.t.bplist -fill both  -expand 1

    gat_breakpoint -load
  }

  proc post {} {
#    DVM::post
#    DVM::setactive Breakpoint
  }


  #############################################################################
  #
  # Called in response to pressing the 'Add' button on the breakpoint editor.
  #
  proc add {} {
    variable bplb_w

    set r [SpreadSheet::size $bplb_w]
    SpreadSheet::see $bplb_w $r 
    set r [SpreadSheet::getPhysicalRow $bplb_w $r]
    SpreadSheet::entrySelect $bplb_w $r 2
  }

  #############################################################################
  #
  # Called in response to pressing the 'Edit' button on the breakpoint editor.
  #
  proc edit {} {
    variable bplb_w

    set items [SpreadSheet::getselection $bplb_w]

    if { [llength $items] < 1 } { return }

    set r [lindex $items 0]
    set r [SpreadSheet::getPhysicalRow $bplb_w $r]
    SpreadSheet::entrySelect $bplb_w $r 2
  }

  #############################################################################
  #
  # Add a new breakpoint entry on the tcl side only
  #
  proc addEntry {id state cond} {
    variable bplb_w
    variable stateImage

    SpreadSheet::insert $bplb_w end [list $id [gifI $stateImage($state)] $cond]
    
  }

  #############################################################################
  #
  # Delete the specified items from both tcl and C.
  #
  proc delete {items} {
    variable bplb_w

    action DelBP { 
      foreach i $items {
	gat_breakpoint -delete [SpreadSheet::getcell $bplb_w $i 0]
      }
    }
    SpreadSheet::delete $bplb_w $items
  }

  #############################################################################
  #
  # Delete the selected breakpoints.
  #
  proc deleteSelection {} {
    variable bplb_w

    SpreadSheet::clearEntrySelect $bplb_w
    delete [SpreadSheet::getselection $bplb_w]
  }

  #############################################################################
  #
  # Enable the selected breakpoints.
  #
  proc enableSelection {} {
    variable bplb_w

    SpreadSheet::clearEntrySelect $bplb_w

    action EnableBP { 
      set select [SpreadSheet::getselection $bplb_w]
      foreach i $select {
	gat_breakpoint -enable [SpreadSheet::getcell $bplb_w $i 0]
      }
    }
  }

  #############################################################################
  #
  # Disable the selected breakpoints.
  #
  proc disableSelection {} {
    variable bplb_w

    SpreadSheet::clearEntrySelect $bplb_w

    action DisableBP { 
      set select [SpreadSheet::getselection $bplb_w]
      foreach i $select {
	gat_breakpoint -disable [SpreadSheet::getcell $bplb_w $i 0] 
      }
    }
  }

  #############################################################################
  #
  # Synchronize tcl brekpoint list with C list.
  #
  proc undoSync {} {
    catch {
      flush
      gat_breakpoint -load
    }
  }

  #############################################################################
  #
  # Pick an ID for a new breakpoint
  #
  proc pickID {} {
    variable bplb_w
    global simOn

    set max_id 0

    set n [SpreadSheet::size $bplb_w]

    #
    # Don't look at the row we just created
    #
    incr n -1

    for {set i 0} {$i < $n} {incr i} {
      catch {
	set id [lindex [SpreadSheet::get $bplb_w $i] 0]
	if {$id > $max_id} {set max_id $id}
      }
    }

    return [expr $max_id + 1]
  }

  proc find {id} {

    variable bplb_w

    set n [SpreadSheet::size $bplb_w]
    for {set i 0} {$i < $n} {incr i} {
      set e [SpreadSheet::get $bplb_w $i]
      if { [lindex $e 0] == $id} {
	return $i
      }
    }
    return -1
  }

  #############################################################################
  #
  # Flush all breakpoints from tcl spreadsheet
  #
  proc flush {} {
    variable bplb_w

    SpreadSheet::flush $bplb_w
  }

  #############################################################################
  #
  # Set states for breakpoint events
  #
  proc seeError {id} { setState $id -1 }
  proc seeGo {id} { setState $id 0 }
  proc seeStandby {id} { setState $id 3 }

  #############################################################################
  #
  # See a trigger of a breakpoint.  Make breakpoint list visible if necessary.
  #
  proc seeBreak {id value} { 
    variable bplb_w

    setState $id 1
    set er [find $id]
    if {$er >= 0 } {
      SpreadSheet::putcell $bplb_w $er 3 $value
    }
    InfoPanel::notify Breakpoints
  }


  #############################################################################
  #
  # Set the state of breakpoint in the tcl spreadsheet.
  #
  proc setState {id state} {
    variable stateImage
    variable bplb_w

    catch {
      set er [find $id]
      if {$er >= 0 } {
	set gif $stateImage($state)
	SpreadSheet::putcell $bplb_w $er 1 [gifI $gif]
      }
      InfoPanel::notify Breakpoints
    }
  }

  #############################################################################
  #
  # Spreadsheet entry manager for breakpoint editor.
  #
  proc manager {cmd args} {
    global simOn

    if { [lindex $args 0] == "-args" } {
      set args [lindex $args 1]
    }

    switch $cmd {
      canenter {                	# check to see if we can enter data
	set c [lindex $args 2]
	if {$c == 2} { 
	  return 1
	} else {
	  return 0
	}
      }
      entrywidget {			# use a special widget for some cells (return 0 for normal entry)
	return 0
      }
      rightclick {			# respond to right mouse click
      }
      deletenotify {			# notification of a deletion
      }
      delete {				# respond to the delete key
	deleteSelection
      }
      close {				# called when a cell editing is finish to do any post processing
	set w [lindex $args 0]
	set r [lindex $args 1]
	set c [lindex $args 2]
	set data [lindex $args 3]
	set er [SpreadSheet::getEffectiveRow $w $r]

	set entry [SpreadSheet::get $w $er]

	if {$c == 2} {
	  action AddBP { gat_breakpoint -replace [lindex $entry 0] $data }

	  if { $simOn } {
	    set gif bp_go.gif
	  } else {
	    set gif bp_standby.gif
	  }
	  SpreadSheet::put $w $er [list [lindex $entry 0] [gifI $gif] $data]
	}

	return $data
      }
      reorder {				# Some list entries may have been moved
      }
      canappend {			# return 1 if we can append to the table
	return 1
      }
      initentry {			# create data for a new entry in a table
	set id [pickID]
	switch [expr $id % 5] {
	  0 { set gif bp_standby.gif}
	  1 { set gif bp_go.gif}
	  2 { set gif bp_stop.gif}
	  3 { set gif bp_bad.gif}
	  4 { set gif bp_disabled.gif}
	}
	if { $simOn } {
	  set gif bp_go.gif
	} else {
	  set gif bp_standby.gif
	}
	return [list $id [gifI $gif] "" ""]
      }
    }
  }

}
