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
# Last edit by hansen on Fri Jan 16 08:15:18 2009
#

set dropbox_debug 0

if { $dropbox_debug } {
  source "misc.tcl"
}

image create bitmap dropbox_arrow -data {
#define dbarrow_width 5
#define dbarrow_height 3
static unsigned char dbarrow_bits[] = {
    0x1f, 0x0e, 0x04};
}


#############################################################################
#
# 
#
namespace eval Dropbox {
  variable contents
  variable did_leave
  variable p_entryVariable
  variable p_width
  variable p_height
  variable p_command
  variable p_value
  variable p_variable
  variable p_entry
  variable p_didbind
  variable p_validatecommand

  proc init {w} {
    variable p_width
    variable p_height
    variable p_command
    variable p_value
    variable p_variable
    variable p_entry
    variable p_didbind
    variable contents
    variable p_bd
    variable p_highlightthickness
    variable p_state
    variable p_validatecommand

    set p_bd($w) 1
    set p_highlightthickness($w) 1
    set p_width($w) 15
    set p_height($w) 5
    set p_command($w) ""
    set p_value($w) ""
    set p_variable($w) ""
    set p_entry($w) 0
    set p_didbind($w) 0
    set p_state($w) normal
    set p_validatecommand($w) ""
  }

  proc getcontents {w} {
    variable contents
    return $contents($w)
  }

  proc prevItem {w} {
    catch {
      variable contents
      variable p_variable
      catch { global $p_variable($w) }
      set value [set $p_variable($w)]

      set i [lsearch $contents($w) $value]
      if {$i > 0} {
	set $p_variable($w) [lindex $contents($w) [expr $i-1]]
      }
    }
  }

  proc nextItem {w} {
    catch {
      variable contents
      variable p_variable
      catch { global $p_variable($w) }
      set value [set $p_variable($w)]

      set i [lsearch $contents($w) $value]
      set N [llength $contents($w)]
      if {$i >= 0 && $i < [expr $N-1]} {
	set $p_variable($w) [lindex $contents($w) [expr $i+1]]
      }
    }
  }

  proc getKey {w c} {
    variable p_state

    if {$p_state($w) == "disabled" } return

    if { $c != "" } {
      catch {
	variable contents
	variable p_variable
	catch { global $p_variable($w) }
	set best ""

	foreach item $contents($w) {
	  set cmp [string compare -nocase $c [string index $item 0]]
	  if {$cmp == 0 } {
	    set best $item
	    break
	  }
	  if {$cmp < 0 && $best == ""} {
	    set best $item
	  }
	}
	#
	# No exact match, so use best match
	#
	if {$best == "" } { set best [lindex $contents($w) end] }

	if { $p_variable($w) != $best } {
	  set $p_variable($w) $best
	  docommand $w
	}
      }
    }
  }

  proc new {w args} {
    variable contents
    variable p_entryVariable

    set p_entryVariable($w) 0

    frame $w -bd 1 -relief sunken -highlightthickness 1 -takefocus 0
    entry $w.e -bd 0 -relief flat -bg white -state disabled -highlightthickness 0 -textvariable p_entryVariable($w)

    #
    # This is supported from tcl/tk 8.4
    #
    catch { $w.e configure -disabledbackground white -disabledforeground black }

    label $w.b -image dropbox_arrow -width 10 -takefocus 0
    pack $w.e $w.b -side left -fill y
    set contents($w) {}

    init $w
    configurel $w $args

    bind $w <Enter> "Dropbox::enter $w"
    bind $w <Leave> "Dropbox::leave $w"
    bind $w.b <Button-1> "Dropbox::press $w"
    bind $w.e <KeyPress-Return> "Dropbox::returnKey $w"

    bind $w.e <Up>		"Dropbox::prevItem $w"
    bind $w.e <Down>		"Dropbox::nextItem $w"
  }
  proc enter {w} {
    $w.b configure -relief raised
  }
  proc leave {w} {
    $w.b configure -relief flat
  }
  proc press {w} {
    variable did_enter
    variable p_state

    if {$p_state($w) == "disabled" } return

    $w.b configure -relief sunken
    grab release $w.b
    set did_enter 0
    Dropbox::showlist $w
  }
  proc returnKey {w} {
    variable p_state

    if {$p_state($w) == "disabled" } return

    catch { destroy .dblist }
    configure $w -value [$w.e get]
    docommand $w
  }
  proc itemadd {w l args} {
    variable contents

    set islist 1
    parseargs $args {-islist}

    if {$islist} {
      foreach item $l {
	lappend contents($w) $item
      }
    } else {
      lappend contents($w) $l
    }
  }
  proc flush {w} {
    variable contents
    set contents($w) {}
  }

  proc doBindings {w} {
    variable p_didbind

    if { ! $p_didbind($w) } {
      set p_didbind($w) 1
      bind $w.e <Button-1>	"focus $w.e"
      bind $w.e <KeyPress>	"Dropbox::getKey $w %A"
    }
  }

  proc configure {w args} {
    configurel $w $args
  }
  proc configurel {w argv} {
    variable p_width
    variable p_height
    variable p_command
    variable p_value
    variable p_variable
    variable p_entry
    variable p_bd
    variable p_highlightthickness
    variable p_state
    variable p_validatecommand
    variable p_entryVariable

    parseargs $argv {-width -height -command -value -variable -entry -bd  -highlightthickness -state -validatecommand -takefocus} 

    if {[info exists variable]} {
      catch { global $variable }
      set p_variable($w) $variable
      $w.e configure -textvariable $variable
    }

    if {[info exists bd]}   {set p_bd($w) $bd }
    if {[info exists highlightthickness]}   {set p_highlightthickness($w) $highlightthickness }

    if {[info exists height]}   { set p_height($w) $height }
    if {[info exists command]}  { set p_command($w) $command }
    if {[info exists entry]}  { set p_entry($w) $entry }
    if {[info exists state]}  { set p_state($w) $state }

    if {[info exists validatecommand]}  { 
      $w.e configure -validatecommand "$validatecommand %W %d %s %S %P" -validate key -invalidcommand bell
    }

    if {[info exists takefocus]}  { 
      $w.e configure -takefocus $takefocus
    }

    if {[info exists width]} {
      set p_width($w) $width
      $w.e configure -width $width
    }
    if {[info exists value]} {
      catch { global $p_variable($w) }
      set p_value($w) $value

      $w.e configure -state normal
      if {[info exists p_variable($w)]} {
      }

      set p_entryVariable($w) $value
      if { $p_entry($w) == 0 } { $w.e configure -state disabled -bg white }
      if { $p_variable($w) != "" } {
	set $p_variable($w) $value
      }
    }

    $w configure -bd $p_bd($w) -highlightthickness $p_highlightthickness($w)

    if {[info exists p_entry($w)] && $p_entry($w)} { 
      $w.e configure -state normal
    } else {
      $w.e configure -state disabled
      doBindings $w
    }

    if { $p_state($w) == "normal" } {
      $w.e configure -fg black
      $w.e configure -bg white
    } else {
      $w.e configure -fg dimgray
      $w.e configure -bg gray
    }
  }
  proc docommand {w} {
    variable p_command
    variable p_value
    variable p_state

    if {$p_state($w) == "disabled" } return

    if { $p_command($w) != "" } {
      eval $p_command($w) [list $w $p_value($w)]
    }
  }
  proc listdnaction {w W} {
    variable p_state

    if {$p_state($w) == "disabled" } return

    if { $W == ".dblist" } { destroy .dblist }
  }
  proc listupaction {w W} {
    variable did_enter
    variable p_command
    variable p_state

    if {$p_state($w) == "disabled" } return

    if { $W == ".dblist.lb" } { 
      set i [.dblist.lb curselection]
      if  {$i != ""} {
	set text [.dblist.lb get $i]
	configure $w -value $text
	docommand $w
      }
    }
    if { $did_enter && $W != ".dblist.vb" } { destroy .dblist }
  }
  proc showlist {w} {
    variable contents
    variable p_width
    variable p_height

    catch { destroy .dblist }
    toplevel .dblist -cursor arrow

    set w_top [winfo toplevel $w]

    set X [winfo rootx $w]
    set Y [winfo rooty $w]
    scan [winfo geometry $w] "%dx%d+%d+%d" W H Xw Yw

    wm geometry .dblist +[expr $X]+[expr $Y + $H]
    wm transient .dblist $w
    wm overrideredirect .dblist 1

    listbox .dblist.lb -bg white -yscrollcommand ".dblist.vb set" -height $p_height($w) -width $p_width($w)
    scrollbar .dblist.vb -command ".dblist.lb yview"
    pack .dblist.lb .dblist.vb -side left -fill y

    foreach i $contents($w) {
      .dblist.lb insert end $i
    }

    bind $w_top <Configure> { destroy .dblist }
    bind .dblist <Button-1> "Dropbox::listdnaction $w %W"
    bind .dblist <ButtonRelease-1> "Dropbox::listupaction $w %W"
    bind .dblist <Enter> "Dropbox::didEnter $w"

    dialogWait .dblist -dosync 0
  }

  proc didEnter {w} {
    variable p_state

    if {$p_state($w) == "disabled" } return

    set Dropbox::did_enter 1
  }
}

#
# If in debugging mode, throw up a simple dropbox
# for testing.
#
if { $dropbox_debug } {
  proc box_changed {w v} {
    focus .
  }

  Dropbox::new .db1 -width 10 -command box_changed -value Sand
  Dropbox::new .db2 -width 10 -command box_changed -value Oo-Toro -entry 1
  Dropbox::new .db3 -width 10 -command box_changed -value Diplodocus

  button .dismiss -text Dismiss -command { destroy . }
  pack .db1 .db2 .db3 .dismiss -padx 10 -pady 10

  Dropbox::itemadd .db1 Foo
  Dropbox::itemadd .db1 Bar
  Dropbox::itemadd .db1 Bletch
  Dropbox::itemadd .db1 Vomit
  Dropbox::itemadd .db1 Sand
  Dropbox::itemadd .db1 Bile
  Dropbox::itemadd .db1 Smelly
  Dropbox::itemadd .db1 Socks

  Dropbox::itemadd .db2 Hamachi
  Dropbox::itemadd .db2 Maguro
  Dropbox::itemadd .db2 Suzuki
  Dropbox::itemadd .db2 Ikura
  Dropbox::itemadd .db2 Tobiko
  Dropbox::itemadd .db2 Odori
  Dropbox::itemadd .db2 Ika
  Dropbox::itemadd .db2 Oo-Toro


  Dropbox::itemadd .db3 Diplodocus
  Dropbox::itemadd .db3 Apatasaurus
  Dropbox::itemadd .db3 Stegosaurus
  Dropbox::itemadd .db3 Styracasaurus
  Dropbox::itemadd .db3 Tyrannosaurus
}
