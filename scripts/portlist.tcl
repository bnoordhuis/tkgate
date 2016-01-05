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
# Last edit by hansen on Fri Jan  9 20:06:58 2009
#

namespace eval PortList {
  variable typeimages
  variable typeimagehelp
  variable portbits
  variable port_w

  proc setupImages {} {
    variable typeimages
    variable typeimagehelp

    set typeimages(in) [gifI [m @portlist.in]]
    set typeimages(out) [gifI [m @portlist.out]]
    set typeimages(inout) [gifI [m @portlist.inout]]
    set typeimages(in2) [gifI [m @portlist.in2]]
    set typeimages(out2) [gifI [m @portlist.out2]]
    set typeimages(inout2) [gifI [m @portlist.inout2]]

    set typeimagehelp(in) [m ho.net.in]
    set typeimagehelp(out) [m ho.net.out]
    set typeimagehelp(inout) [m ho.net.inout]
    set typeimagehelp(in2) [m ho.net.in2]
    set typeimagehelp(out2) [m ho.net.out2]
    set typeimagehelp(inout2) [m ho.net.inout2]
  }

  proc flush {} {
    variable port_w
    Tree::delitem $port_w.t /
  }

  proc replace {mod args} {
    del $mod
    eval "add $mod $args"
  }

  proc del {mod} {
    variable port_w
    Tree::delitem $port_w.t /$mod
  }


  proc add {name args} {
    variable netl_w
    variable typeimages
    variable typeimagehelp
    variable portbits
    variable port_w

    set bits 1
    set type "in"
    parseargs $args {-bits -type}

    #
    # Ignore duplicate entries. 
    #
    if {[Tree::exists $port_w.t /$name]} {
      return
    }

    if {$bits == 1} {
      set portbits($name) ""
    } else {
      set portbits($name) "\[[expr $bits-1]:0\]"
      set type "${type}2"
    }

    Tree::newitem $port_w.t /$name -image $typeimages($type) -imagehelp $typeimagehelp($type)
  }

  proc seeB1Press {w x y X Y} {
    action -Unselect { tkg_undoSelections nets }
    set lbl [Tree::labelat $w $x $y]
    setselection $lbl
  }
  proc seeB3Press {w x y X Y} {
    action -Unselect { tkg_undoSelections nets }
    set lbl [Tree::labelat $w $x $y]
    setselection $lbl
  }
  proc seeDoubleB1 {w x y X Y} {
    action -Unselect { tkg_undoSelections nets }
    set lbl [Tree::labelat $w $x $y]
    setselection $lbl
    if { $lbl != "" } {
      continueAction EditNet { gat_editNet [PortList::getselection] }
    }
  }

  proc setselection {name} {
    variable port_w

    Tree::setselection $port_w.t $name
    action -Select { gat_selectNet [string range $name 1 end] }
  }

  proc getselection {args} {
    variable port_w

    set path [Tree::getselection $port_w.t]
    return [string trimleft $path  "/" ]
  }

  proc create {w} {
    variable port_w

    setupImages

    frame $w
    set port_w $w

    Tree::create $w.t -width 50 -height 50 -bd 2 -relief sunken \
	-yscrollcommand "$w.vb set" -xscrollcommand "$w.hb set" -nolines 1
    scrollbar $w.vb -orient vertical -command "$w.t yview" -takefocus 0
    scrollbar $w.hb -orient horizontal -command "$w.t xview" -takefocus 0

    grid rowconfigure $w 0 -weight 1
    grid columnconfigure $w 0 -weight 1
    grid $w.t  -row 0 -column 0 -sticky nsew -padx 1 -pady 1
    grid $w.vb -row 0 -column 1 -sticky ns -padx 1 -pady 1
    grid $w.hb -row 1 -column 0 -sticky ew -padx 1 -pady 1

    bind $w.t <1> { PortList::seeB1Press %W %x %y %X %Y }
    bind $w.t <3> { PortList::seeB3Press %W %x %y %X %Y }
    bind $w.t <Double-ButtonPress-1> { PortList::seeDoubleB1 %W %x %y %X %Y }

    Tree::setsuparray $w.t PortList::portbits
  }
}
