#   Copyright (C) 1987-2005 by Jeffery P. Hansen
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
# Last edit by hansen on Mon Apr 25 19:00:49 2005
#


namespace eval Undo {
  variable undo_type undo

  #
  # Post box for undo/redo selection
  #
  proc postURBox {w} {
    cancel_help

    toplevel .undo -cursor arrow

    scan [winfo rootx $w] "%d" X
    scan [winfo pointery .] "%d" Y
    set Y [expr $Y - 10]
    wm geometry .undo +$X+$Y
    wm transient .undo .
    wm overrideredirect .undo 1

    listbox .undo.lb -bd 2 -relief sunken -yscrollcommand ".undo.vb set" -bg white -width 10 -height 5 -selectmode multiple
    scrollbar .undo.vb -command ".undo.lb yview"
    pack .undo.lb .undo.vb -side left -fill both
  }

  #############################################################################
  #
  # Called as a result of a mouse button release while the undo/redo listbox
  # is active.  Causes the listbox to be deleted and the selected undo/redo
  # actions to be executed.
  #
  proc finishURBox {} {
    global undo_type

    set n -1

    if { [.undo.lb size] > 0 } {
      set S [.undo.lb curselection]
      foreach j $S {
	if { $j > $n } {set n $j }
      }
    }

    destroy .undo
    if { $n >= 0 } {
      HdlEditor::startCommand -
      gat_$undo_type [expr $n + 1]
    }
  }

  #############################################################################
  #
  # Handles pointer motion within the undo/redo listbox.
  #
  proc motionURBox {y} {
    set n [.undo.lb size]
    set i [.undo.lb nearest $y]
    .undo.lb selection set 0 $i
    if { [expr $i + 1] < $n } {
      .undo.lb selection clear [expr $i + 1] end
    }
  }

  #############################################################################
  #
  # Waits for the undo/redo listbox to be deleted.
  #
  proc waitURBox {} {
    bind .undo <ButtonRelease-1> Undo::finishURBox
    bind .undo.lb <B1-Motion> { Undo::motionURBox %y }
    bind .undo.lb <Leave> {.undo.lb selection clear 0 end }

    tkwait visibility .undo
    .undo.lb selection clear 0 end
    .undo.lb selection set 0
    grab set .undo
    tkwait window .undo
    grab release .undo
  }

  #############################################################################
  #
  # Called as a result of hitting the "undo" button.  Causes the list of undo
  # actions to be displayed in the listbox.
  #
  proc undoSelect {} {
    global undo_type undo_w

    set i 0
    set undo_type undo
    postURBox $undo_w.back
    foreach u [gat_getUndoList] {
      .undo.lb insert end $u
    }
    waitURBox
  }

  #############################################################################
  #
  # Called as a result of hitting the "redo" button.  Causes the list of redo
  # actions to be displayed in the listbox.
  #
  proc redoSelect {} {
    global undo_type undo_w

    set i 0
    set undo_type redo
    postURBox $undo_w.forward
    foreach u [gat_getRedoList] {
      .undo.lb insert end $u
    }
    waitURBox
  }
}
