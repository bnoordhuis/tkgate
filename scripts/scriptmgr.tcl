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
# Last edit by hansen on Sat Jul 28 00:20:29 2007
#

#############################################################################
#
# Functions for the "script manager" implemented as a sub-page of the DVM
# (dynamic verilog module) dialog box.
#
namespace eval ScriptMgr {
  variable sm_w
  variable smlb_w
  variable stateImage
  variable errList
  variable scriptName
  variable scriptState

  array set stateImage {
    -1	bp_bad.gif
    0	bp_go.gif
    1	ss_done.gif
    2	bp_disabled.gif
    3	bp_standby.gif
  }


  #############################################################################
  #
  # Pick an ID for a new breakpoint
  #
  proc pickID {} {
    variable smlb_w
    global simOn

    set max_id 0

    set n [SpreadSheet::size $smlb_w]
    for {set i 0} {$i < $n} {incr i} {
      catch {
	set id [lindex [SpreadSheet::get $smlb_w $i] 0]
	if {$id > $max_id} {set max_id $id}
      }
    }

    return [expr $max_id + 1]
  }

  #############################################################################
  #
  # Report an error message for a script
  #
  proc reportError {id name lineno msg} {
    variable errList

    lappend errList($id) "$name $lineno $msg"
#    puts "reportError id=$id name=$name lineno=$lineno msg=$msg"
  }

  proc highlightError {w id} {
    variable errList
    global tkg_pastelHighlight

    catch {
      set n [$w.pane.errlist.box.lb curselection]
      set lineno [lindex [lindex $errList($id) $n] 1]

      $w.pane.main.text tag remove highlight 1.0 end
      $w.pane.main.text tag add highlight $lineno.0 $lineno.end

      $w.pane.main.text tag configure highlight -background $tkg_pastelHighlight

    }
  }

  #############################################################################
  #
  # Display a script file and any errors it may have received
  #
  proc viewScriptFile {id} {
    variable scriptName
    variable errList

    set file $scriptName($id)

    if {[catch { set f [open $file]}]} {
      errmsg [format [m err.viewfile] $file]
      return
    }

    if {[scan $id "script:%d" n] != 1} return
    set w .vsf$n

    #
    # Abort if already visible
    #
    if {[catch {toplevel $w -height 400 -width 600}]} {
      return
    }

    wm title $w [file tail $file]

    frame $w.pane

    frame $w.pane.main
    text $w.pane.main.text -yscrollcommand "$w.pane.main.vb set" -xscrollcommand "$w.pane.main.hb set" -bg white
    scrollbar $w.pane.main.vb -command "$w.pane.main.text yview"
    scrollbar $w.pane.main.hb -orient horizontal -command "$w.pane.main.text xview"

    grid rowconfigure $w.pane.main 0 -weight 1
    grid columnconfigure $w.pane.main 0 -weight 1
    grid $w.pane.main.text -row 0 -column 0 -sticky nsew
    grid $w.pane.main.vb -row 0 -column 1 -sticky ns
    grid $w.pane.main.hb -row 1 -column 0 -sticky ew

    pack $w.pane.main -fill both -expand 1

    button $w.dismiss -text [m b.dismiss] -command "destroy $w"
    pack $w.pane -fill both -expand 1
    pack $w.dismiss -fill x
    
    #
    # Copy contents of script file.
    #
    catch {
      $w.pane.main.text insert end [read $f]
      close $f
    }
    $w.pane.main.text configure -state disabled

    pack $w.pane.main -fill both -expand 1

    #
    # If no errors, don't display error list
    #
    if { [llength $errList($id)] == 0 } {
      return
    }

    #############################################################################
    #
    # Report errors
    #

    set errNone  [gifI err_none.gif]
    set errArrow [gifI err_arrow.gif]

    scan [$w.pane.main.text index end] %d numLines

    #
    # Get list of lines with errors
    #
    set badLines {}
    foreach e $errList($id) { 
      set lineno [lindex $e 1]
      if {[lsearch -integer $badLines $lineno] < 0} {
	lappend badLines $lineno
      }
    }

    #
    # Mark lines with errors
    #
    for {set i 1} {$i <= $numLines} {incr i} {
      if {[lsearch -integer $badLines $i] < 0} {
	$w.pane.main.text image create $i.0 -image $errNone -align center
      } else {
	$w.pane.main.text image create $i.0 -image $errArrow -align center -name line$i
      }
    }

    frame $w.pane.errlist

    frame $w.pane.errlist.box -bd 1 -relief sunken

    label $w.pane.errlist.box.hdr -text [m info.script.error] -bd 1 -relief raised

    listbox $w.pane.errlist.box.lb -width 60 -height 5 -bg white  -relief flat -bd 0 -yscrollcommand "$w.pane.errlist.vb set" 
    scrollbar $w.pane.errlist.vb -command "$w.pane.errlist.box.lb yview"

    pack $w.pane.errlist.box.hdr -fill x
    pack $w.pane.errlist.box.lb  -fill x

    pack $w.pane.errlist.box -padx 5 -pady 5 -fill both -expand 1 -side left
    pack $w.pane.errlist.vb -padx 5 -pady 5 -fill y -side left

    pack $w.pane.errlist -fill both -expand 1

    bind $w.pane.errlist.box.lb <ButtonRelease-1> "ScriptMgr::highlightError $w $id"

    #
    # Copy error messages
    #
    foreach line $errList($id) { 
      if {[scan $line "%s %d %\[^\n\]" file lineno msg] == 3 } {
	$w.pane.errlist.box.lb insert end [format "%s, %d: %s" [file tail $file] $lineno $msg]
      } else {
	$w.pane.errlist.box.lb insert end $line
      }
    }
  }

  #############################################################################
  #
  # Spreadsheet entry manager for script editor.
  #
  proc manager {cmd args} {
    global simOn

    if { [lindex $args 0] == "-args" } {
      set args [lindex $args 1]
    }

    switch $cmd {
      canenter {                	# check to see if we can enter data
	return 0
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
	set data [lindex $args 3]
	return $data
      }
      reorder {				# Some list entries may have been moved
      }
      canappend {			# return 1 if we can append to the table
	return 0
      }
      initentry {			# create data for a new entry in a table
	set id [pickID]
	return [list $id [gifI bp_standby.gif] ""]
      }
      doublePress {
	set w [lindex $args 0]
	set er [lindex $args 1]

	if {$er < [SpreadSheet::size $w] } {
	  set entry [SpreadSheet::get $w $er]
	  viewScriptFile script:[lindex $entry 0]
	} else {
	  Action::doSimScript
	}
      }
    }
  }

  proc deleteSelection {args} {
    variable smlb_w

    set r [SpreadSheet::getselection $smlb_w]
    if { $r == "" } return
    set r [lindex $r 0] 
    set id [lindex [SpreadSheet::get $smlb_w $r] 0]

    Simulator::cmdSend "\$delscript script:$id"
    SpreadSheet::delete $smlb_w $r 
  }

  proc enableSelection {args} {
    variable smlb_w
    variable scriptState

    set r [SpreadSheet::getselection $smlb_w]
    if { $r == "" } return
    set r [lindex $r 0] 
    set id [lindex [SpreadSheet::get $smlb_w $r] 0]

    if { $scriptState($id) == 2 } {
      seeGo $id
      Simulator::cmdSend "\$enablescript script:$id"
    }
  }

  proc disableSelection {args} {
    variable smlb_w
    variable scriptState

    set r [SpreadSheet::getselection $smlb_w]
    if { $r == "" } return
    set r [lindex $r 0] 
    set id [lindex [SpreadSheet::get $smlb_w $r] 0]

    if { $scriptState($id) == 0 } {
      seeDisabled $id
      Simulator::cmdSend "\$disablescript script:$id"
    }
  }

  proc create {w} {
    variable sm_w tbr
    variable smlb_w

    set sm_w $w

    frame $w

    frame $w.actions
    button $w.actions.add -text [m mm.add] -command Action::doSimScript -width [m @mm.width]
    button $w.actions.delete -text [m mm.delete] -command ScriptMgr::deleteSelection -width [m @mm.width]
    button $w.actions.enable -text [m mm.enable] -command ScriptMgr::enableSelection
    button $w.actions.disable -text [m mm.disable] -command ScriptMgr::disableSelection
    set i 0
    foreach {b1 b2} {add enable delete disable} {
      if { $b1 != "." } {
	grid $w.actions.$b1 -row $i -column 0 -padx 1 -pady 1 -sticky ew
      }
      if { $b2 != "." } {
	grid $w.actions.$b2 -row $i -column 1 -padx 1 -pady 1 -sticky ew
      }
      incr i
    }

# ENABLE THIS IN A FUTURE VERSION
#    
#    frame $w.actions.vcr -relief raised -bd 1
#    grid $w.actions.vcr -row $i -column 0 -columnspan 2 -pady 1 -sticky ew 
#    label $w.actions.vcr.label -text "Script Control:"
#    pack $w.actions.vcr.label -padx 2 -pady 2
#    button $w.actions.vcr.go -image [gifI "sim_go.gif"]  -takefocus 0 -relief flat
#    button $w.actions.vcr.pause -image [gifI "sim_pause.gif"] -takefocus 0 -relief flat
#    button $w.actions.vcr.stop -image [gifI "sim_stop.gif"] -takefocus 0 -relief flat
#    pack  $w.actions.vcr.go  $w.actions.vcr.pause  $w.actions.vcr.stop -side left -padx 4 -pady 1

    pack $w.actions -side right -fill x -anchor nw -pady 3 

    frame $w.t
    pack $w.t -fill both

    set smlb_w $w.t.smlist.lb
    frame $w.t.smlist
    SpreadSheet::create $w.t.smlist.lb  -height 13 -bd 2 -relief sunken -yscrollcommand "$w.t.smlist.vb set" \
	  -entrycommand ScriptMgr::manager -expandcol 2 -resize 1
    SpreadSheet::addcolumn $w.t.smlist.lb -width 4 -header ID
    SpreadSheet::addcolumn $w.t.smlist.lb -width 3 -header S -type image
    SpreadSheet::addcolumn $w.t.smlist.lb -width 50 -header Script
    pack $w.t.smlist.lb -side left -fill both -expand 1

    scrollbar $w.t.smlist.vb -orient vertical -command "SpreadSheet::yview $w.t.smlist.lb"
    pack $w.t.smlist.vb -side left -fill y

    pack $w.t.smlist -fill both -expand 1

    gat_breakpoint -load
  }

  #############################################################################
  #
  # Pick an ID for a new script
  #
  proc pickID {} {
    variable smlb_w
    global simOn

    set max_id 0

    set n [SpreadSheet::size $smlb_w]
    for {set i 0} {$i < $n} {incr i} {
      catch {
	set id [lindex [SpreadSheet::get $smlb_w $i] 0]
	if {$id > $max_id} {set max_id $id}
      }
    }

    return [expr $max_id + 1]
  }

  proc load {fileName} {
    variable smlb_w
    variable stateImage
    variable errList
    variable scriptName
    variable scriptState

    set id [ScriptMgr::pickID]
    Simulator::cmdSend "\$script script:$id $fileName"
    SpreadSheet::insert $smlb_w end [list $id [gifI $stateImage(0)] $fileName]
    set errList(script:$id) {}
    set scriptName(script:$id) $fileName

    set scriptState($id) 0

    InfoPanel::notify Scripts
  }

  proc flush {} {
    variable smlb_w
    SpreadSheet::flush $smlb_w
  }

  proc find {id} {
    variable smlb_w

    set n [SpreadSheet::size $smlb_w]
    for {set i 0} {$i < $n} {incr i} {
      set e [SpreadSheet::get $smlb_w $i]
      if { [lindex $e 0] == $id} {
	return $i
      }
    }
    return -1
  }

  proc seeError {id} { setState $id -1 }
  proc seeGo {id} { setState $id 0 }
  proc seeDone {id} { setState $id 1 }
  proc seeDisabled {id} { setState $id 2 }
  proc seeStandby {id} { setState $id 3 }

  proc setState {id state} {
    variable stateImage
    variable smlb_w
    variable scriptState

    catch {
      set er [find $id]

      set scriptState($id) $state


      if {$er >= 0 } {
	set gif $stateImage($state)
	SpreadSheet::putcell $smlb_w $er 1 [gifI $gif]
      }
    }
    InfoPanel::notify Scripts
  }

  #############################################################################
  #
  # If the DVM dialog box is active, makes the script manager the active page
  #
  proc post {args} {
  }
}
