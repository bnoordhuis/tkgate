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
# Last edit by hansen on Tue Feb  3 10:31:35 2009
#
# This file contains procedures for displaying and maniuplating
# the error list.  The error list is a box listing any errors that
# occured when starting the simulator.  The user can click on an
# error messsage and cause the editor to jump to the location
# of the error.
#
#
# ErrBox methods:
#   post			Pop up the error message box
#   unpost			Delete the error box
#   addError "error message"	Add a message to the box
#   report			Call back to report selection
#
# C callbacks used:
#   gat_errBoxReport		Display error position
#

namespace eval ErrBox {
  variable errbox_x 50
  variable errbox_y 50
  variable is_active 0

  #tkg_errBoxReport
  proc report {} {
    action ShowErr {
      gat_errBoxReport [.errbox.lframe.list curselection]
    }
  }

  #tkg_delErrBox
  proc unpost {} {
    variable is_active
    variable errbox_x
    variable errbox_y

    set is_active 0
    transAction - {
      gat_errBoxReport -1
    }
    catch { 
      set errbox_x [expr [winfo rootx .errbox] - [winfo rootx .]]
      set errbox_y [expr [winfo rooty .errbox] - [winfo rooty .]]
      destroy .errbox
    }
  }

  #tkg_errBoxUp
  proc selectUp {} {
    if { [catch {set N [.errbox.lframe.list curselection]} ] } {
      return
    } 


    if { $N == "" } {
      set N 0
    } else {
      set N [expr $N - 1]
    }
    .errbox.lframe.list selection clear 0 end
    .errbox.lframe.list selection set $N
    .errbox.lframe.list see $N
    report
  }

  #tkg_errBoxDown
  proc selectDown {} {
    if { [catch {set N [.errbox.lframe.list curselection]} ] } {
      return
    } 

    if { $N == "" } {
      set N 0
    } else {
      set N [expr $N + 1]
    }
    .errbox.lframe.list selection clear 0 end
    .errbox.lframe.list selection set $N
    .errbox.lframe.list see $N
    report
  }

  #tkg_errBoxAdd
  proc addError args {
    .errbox.lframe.list insert end [join $args ]
  }

  proc hadFatalErrors {} {
    catch { .errbox.image.exp configure -text [m db.err.explain.err] }
  }

  proc post {} {
    variable is_active
    variable errbox_x
    variable errbox_y

    toplevel .errbox
    wm title .errbox [m db.err.title]
    wm geometry .errbox [offsetgeometry . $errbox_x $errbox_y]
    wm transient .errbox .

    dialogImage .errbox.image -image [gifI bug.gif] -caption [m db.err.caption] -explaination [m db.err.explain.warn]
    pack .errbox.image -fill both -side left -pady 10

    frame .errbox.lframe

    listbox .errbox.lframe.list -width 60 -height 15 -yscrollcommand ".errbox.lframe.vert set" -xscrollcommand ".errbox.lframe.horz set" -bg white
    scrollbar .errbox.lframe.vert -command ".errbox.lframe.list yview"
    scrollbar .errbox.lframe.horz -orient horizontal -command ".errbox.lframe.list xview"

    grid rowconfigure .errbox.lframe 0 -weight 1
    grid columnconfigure .errbox.lframe 0 -weight 1
    grid .errbox.lframe.list -row 0 -column 0 -sticky nsew 
    grid .errbox.lframe.vert -row 0 -column 1 -sticky ns 
    grid .errbox.lframe.horz -row 1 -column 0 -sticky ew 

    pack .errbox.lframe -side top  -padx 5 -pady 5 -fill both -expand 1

    bind .errbox.lframe.list <ButtonRelease-1> { catch { ErrBox::report } }

    frame .errbox.dframe
    button .errbox.dframe.dismiss -text [m b.dismiss] -command ErrBox::unpost
    pack .errbox.dframe.dismiss -padx 5 -pady 5 -anchor e
    pack .errbox.dframe -side bottom  -padx 5 -pady 5 -fill both


    bind .errbox <Up>		{ Action::errBoxUp }
    bind .errbox <Down>		{ Action::errBoxDown }
    bind .errbox <p>		{ Action::errBoxUp }
    bind .errbox <n>		{ Action::errBoxDown }
    bind .errbox <Control-p>	{ Action::errBoxUp }
    bind .errbox <Control-n>	{ Action::errBoxDown }

    set is_active 1
  }
}




