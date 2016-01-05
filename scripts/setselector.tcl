#############################################################################
#
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
# Last edit by hansen on Mon Dec 29 14:36:03 2008
#

#
# Set selectors consists of a pair of listboxes and buttons to move items
# between the "selected" list and the "available" list.
#
namespace eval SetSelector {
  variable ss_available
  variable ss_selected

  #############################################################################
  #
  # Respond to changes in selection list variables
  #
  proc updateset {w args}  {
    variable ss_available
    variable ss_selected

    upvar \#0 $ss_available($w) availableList
    upvar \#0 $ss_selected($w) selectedList

    $w.left.lb delete 0 end
    $w.right.lb delete 0 end

    foreach l $availableList {
      if {[lsearch $selectedList $l] >= 0} {
	$w.right.lb insert end $l
      } else {
	$w.left.lb insert end $l
      }
    } 
  }

  #############################################################################
  #
  # Initialized variable watches
  #
  proc init {w} {
    variable ss_available
    variable ss_selected
    upvar \#0 $ss_available($w) availableList
    upvar \#0 $ss_selected($w) selectedList


    trace variable availableList w "SetSelector::updateset $w"
    trace variable selectedList w "SetSelector::updateset $w"
  }

  #############################################################################
  #
  # Undo variable watches
  #
  proc uninit {w} {
    variable ss_available
    variable ss_selected
    upvar \#0 $ss_available($w) availableList
    upvar \#0 $ss_selected($w) selectedList

    trace vdelete availableList w "SetSelector::updateset $w"
    trace vdelete selectedList w "SetSelector::updateset $w"
  }

  #############################################################################
  #
  # Move all items in unselected list to selected list
  #
  proc allSelected {w} {
    variable ss_available
    variable ss_selected
    upvar \#0 $ss_available($w) availableList
    upvar \#0 $ss_selected($w) selectedList

    set selectedList $availableList
  }

  #############################################################################
  #
  # Move all items in selected list to unselected list.
  #
  proc allUnselected {w} {
    variable ss_selected
    upvar \#0 $ss_selected($w) selectedList

    set selectedList {}
  }

  #############################################################################
  #
  # Move highlighted items to selected list
  #
  proc makeSelected {w} {
    variable ss_selected
    upvar \#0 $ss_selected($w) selectedList

    set indexSet [$w.left.lb curselection]
    if { $indexSet == "" } return

    set L $selectedList
    foreach i $indexSet {
      lappend L [$w.left.lb get $i]
    }
    set selectedList $L
  }

  #############################################################################
  #
  # Move highlighted items to unselected list
  #
  proc makeUnselected {w} {
    variable ss_selected
    upvar \#0 $ss_selected($w) selectedList

    set indexSet [$w.right.lb curselection]
    if { $indexSet == "" } return

    set L $selectedList
    foreach i $indexSet {
      set lib [$w.right.lb get $i]

      set j [lsearch $L $lib]
      if {$j < 0} return

      set L [lreplace $L $j $j]
    }
    set selectedList $L
  }

  proc getselectlist {w} {
    variable ss_available
    variable ss_selected
    upvar \#0 $ss_available($w) availableList
    upvar \#0 $ss_selected($w) selectedList

    set L {}
    foreach item $ss_selected {
      if {[lsearch $availableList $item] >= 0} {
	lappend L $item
      }
    }

    return $L
  }

  #############################################################################
  #
  # Create a set selector
  #
  proc new {w args} {
    variable ss_available
    variable ss_selected

    set availablevariable ""
    set selectedvariable ""
    set availablelabel ""
    set selectedlabel ""
    set selectlistboxvariable ""
    parseargs $args {-availablelabel -selectedlabel -availablevariable -selectedvariable -selectlistboxvariable}

    set ss_available($w) $availablevariable
    set ss_selected($w) $selectedvariable

    frame $w

    #
    # Left side box
    #
    frame $w.left
    label $w.left.l -text $availablelabel
    listbox $w.left.lb -yscrollcommand "$w.left.vb set" -width 1 -selectmode extended
    scrollbar $w.left.vb -orient vertical -command "$w.left.lb yview"
    pack $w.left.l
    pack $w.left.lb -side left -fill both -expand 1
    pack $w.left.vb -side left -fill y

    #
    # Right side box
    #
    frame $w.right
    label $w.right.l -text $selectedlabel
    listbox $w.right.lb -yscrollcommand "$w.right.vb set" -width 1 -selectmode extended \
	-listvariable $selectlistboxvariable
    scrollbar $w.right.vb -orient vertical -command "$w.right.lb yview"
    pack $w.right.l
    pack $w.right.lb -side left -fill both -expand 1
    pack $w.right.vb -side left -fill y

    frame $w.middle
    label $w.middle.pad -text " "
    button $w.middle.allr -image [gifI allrightarrow.gif] -command "SetSelector::allSelected $w"
    button $w.middle.ltor -image [gifI bigrightarrow.gif] -command "SetSelector::makeSelected $w"
    button $w.middle.rtol -image [gifI bigleftarrow.gif] -command "SetSelector::makeUnselected $w"
    button $w.middle.alll -image [gifI allleftarrow.gif] -command "SetSelector::allUnselected $w"

    pack $w.middle.pad  -anchor n -side top
    pack $w.middle.allr -anchor n -side top -expand 1
    pack $w.middle.alll -anchor s -side bottom -expand 1
    pack $w.middle.ltor $w.middle.rtol -pady 5

    pack $w.left   -padx 5 -pady 5 -side left -fill both -expand 1
    pack $w.middle -padx 5 -pady 5 -side left -fill y
    pack $w.right  -padx 5 -pady 5 -side left -fill both -expand 1

    init $w
    bind $w <Destroy> "SetSelector::uninit $w"

    helpon $w.middle.allr [m ho.setsel.allr]
    helpon $w.middle.alll [m ho.setsel.alll]
    helpon $w.middle.ltor [m ho.setsel.ltor]
    helpon $w.middle.rtol [m ho.setsel.rtol]

    updateset $w
  }
}
