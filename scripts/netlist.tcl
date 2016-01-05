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
# Last edit by hansen on Fri Jan  9 20:06:34 2009
#
#
#

set ednet_oldName ""
set ednet_name ""
set ednet_hide 0
set ednet_nbits 1
set ednet_io 0
set ednet_vtype wire

proc tkg_editNetUpdate {} {
  global ednet_oldName ednet_name ednet_hide ednet_nbits ednet_io ednet_vtype

  gat_changeNet $ednet_oldName $ednet_name $ednet_hide $ednet_nbits $ednet_io $ednet_vtype

  destroy .ednet
}

proc tkg_editNet {x y net io wtype} {
  global ednet_oldName ednet_name ednet_hide ednet_nbits ednet_io
  global ednet_result edgat_newPort ednet_vtype
  global tkg_wtypeList

  set ednet_result 1

  set ednet_vtype $wtype

  if {[scan $net {%[^@]%s} net ednet_hide] == 2} {
    set ednet_hide 1
  } else {
    set ednet_hide 0
  }

  set geom [offsetgeometry . $x $y]

  if {[scan $net {%[^\[][%d:%d]} net msb lsb] == 3} {
    set ednet_name $net
    set ednet_nbits [expr $msb - $lsb + 1 ]
  } else {
    set ednet_name $net
    set ednet_nbits 1
  }

  set ednet_oldName $ednet_name

  set w .ednet
  toplevel $w
  wm resizable $w 0 0
  wm title $w [m db.net.title]
  wm geometry $w $geom
  wm transient $w .

  okcancel $w.ok -okcommand { tkg_editNetUpdate } -cancelcommand { set ednet_result 0; destroy .ednet }
  pack $w.ok -fill x -side bottom

  frame $w.main -bd 2 -relief raised
  pack $w.main -fill both

  dialogImage $w.main.image -image [gifI netprops.gif] -caption  [m db.net.caption]
  pack $w.main.image -fill both -side left

  frame $w.main.box
  pack $w.main.box -padx 10 -pady 10 -fill both

  label $w.main.box.ln -text "[m db.net.name]: "
  entry $w.main.box.en -textvariable ednet_name -bg white

  label $w.main.box.lh -text "[m db.net.hide]: "
  checkbutton $w.main.box.eh -variable ednet_hide

  label $w.main.box.ls -text "[m db.net.bits]: "
  bitsizeselector $w.main.box.es -variable ednet_nbits -width 3 

  label $w.main.box.lt -text "[m db.net.type]: "
  Dropbox::new $w.main.box.et -variable ednet_vtype -width 6
  Dropbox::itemadd $w.main.box.et $tkg_wtypeList
  if {[lsearch $tkg_wtypeList $ednet_vtype] < 0 } {
    #
    # If not a standard wire type, assume the type is fixed (e.g., supply0, switch, port)
    #
    Dropbox::configure $w.main.box.et -state disabled 
  }


  grid $w.main.box.ln -row 0 -column 0 -sticky e -padx 3 -pady 3
  grid $w.main.box.en -row 0 -column 1 -sticky w -padx 3 -pady 3
  grid $w.main.box.lh -row 1 -column 0 -sticky e -padx 3 -pady 3
  grid $w.main.box.eh -row 1 -column 1 -sticky w -padx 3 -pady 3
  grid $w.main.box.ls -row 2 -column 0 -sticky e -padx 3 -pady 3
  grid $w.main.box.es -row 2 -column 1 -sticky w -padx 3 -pady 3
  grid $w.main.box.lt -row 3 -column 0 -sticky e -padx 3 -pady 3
  grid $w.main.box.et -row 3 -column 1 -sticky w -padx 3 -pady 3

  if { $io != 0 } {
    label $w.main.box.lio -text "[m db.net.port]: "
    frame $w.main.box.fio
    set ednet_io $io
    #
    # The values here must match the definitins for LOGICIN,
    # LOGICOUT, LOGICTRI in the main program.
    # 
    radiobutton $w.main.box.fio.in -text [m db.net.in] -variable ednet_io -value 4102
    radiobutton $w.main.box.fio.out -text [m db.net.out] -variable ednet_io -value 4103
    radiobutton $w.main.box.fio.tri -text [m db.net.inout] -variable ednet_io -value 4104

    pack $w.main.box.fio.in $w.main.box.fio.out $w.main.box.fio.tri -side left

    grid $w.main.box.lio -row 4 -column 0 -sticky e -padx 3 -pady 3
    grid $w.main.box.fio -row 4 -column 1 -sticky w -padx 3 -pady 3
  }

  bind $w.main.box.en <Return> { tkg_editNetUpdate }
  bind $w.main.box.es <Return> { tkg_editNetUpdate }

  if { $edgat_newPort } {
      $w.main.box.en selection range 0 end
      focus $w.main.box.en
  }


  dialogWait $w

  return $ednet_result
}

#
# Implementation of netlist appearing in the sidebar
#
namespace eval NetList {
  variable netl_w ""
  variable typeimages
  variable typeimagehelp
  variable netbits

  proc setupImages {} {
    variable typeimages
    variable typeimagehelp

    set typeimages(wire) [gifI net_wire.gif]
    set typeimages(wire2) [gifI net_wire2.gif]
    set typeimages(h-wire) [gifI net_hwire.gif]
    set typeimages(h-wire2) [gifI net_hwire2.gif]

    set typeimages(reg) [gifI net_reg.gif]
    set typeimages(reg2) [gifI net_reg2.gif]
    set typeimages(h-reg) [gifI net_reg.gif]
    set typeimages(h-reg2) [gifI net_reg2.gif]

    set typeimages(input) [gifI port_in1.gif]
    set typeimages(h-input) [gifI port_in1.gif]
    set typeimages(input2) [gifI port_in2.gif]
    set typeimages(h-input2) [gifI port_in2.gif]

    set typeimages(output) [gifI port_out1.gif]
    set typeimages(h-output) [gifI port_out1.gif]
    set typeimages(output2) [gifI port_out2.gif]
    set typeimages(h-output2) [gifI port_out2.gif]

    set typeimages(inout) [gifI port_inout1.gif]
    set typeimages(h-inout) [gifI port_inout1.gif]
    set typeimages(inout2) [gifI port_inout2.gif]
    set typeimages(h-inout2) [gifI port_inout2.gif]


    #
    # Set up help messages...
    #
    set typeimagehelp(wire)		[m ho.net.wire]
    set typeimagehelp(wire2)		[m ho.net.wire2]
    set typeimagehelp(h-wire)		[m ho.net.hwire]
    set typeimagehelp(h-wire2)		[m ho.net.hwire2]

    set typeimagehelp(reg) 		[m ho.net.reg]
    set typeimagehelp(reg2)		[m ho.net.reg2]
    set typeimagehelp(h-reg)		[m ho.net.reg]
    set typeimagehelp(h-reg2) 		[m ho.net.reg2]

    set typeimagehelp(input) 		[m ho.net.in]
    set typeimagehelp(h-input) 		[m ho.net.in]
    set typeimagehelp(input2) 		[m ho.net.in2]
    set typeimagehelp(h-input2)		[m ho.net.in2]

    set typeimagehelp(output) 		[m ho.net.out]
    set typeimagehelp(h-output)		[m ho.net.out]
    set typeimagehelp(output2) 		[m ho.net.out2]
    set typeimagehelp(h-output2)	[m ho.net.out2]

    set typeimagehelp(inout)		[m ho.net.inout]
    set typeimagehelp(h-inout)		[m ho.net.inout]
    set typeimagehelp(inout2) 		[m ho.net.inout2]
    set typeimagehelp(h-inout2)		[m ho.net.inout2]
  }

  proc flush {} {
    variable netl_w
    Tree::delitem $netl_w.t /
  }

  proc replace {net args} {
#    if {[gat_getMajorMode] == "interface" } return
    del $net
    eval "add $net $args"
  }

  proc configureall {args} {
    variable netl_w
    variable netbits

    set probe 0
    parseargs $args {-probe}

    foreach net [Tree::getchildren $netl_w.t /] {
      if {$probe} {
	if { $netbits($net) == "" } {
	  Tree::configitem $netl_w.t /$net -image2 [gifI net_probe.gif]
	} else {
	  Tree::configitem $netl_w.t /$net -image2 [gifI net_probe2.gif]
	}
      } else {
	Tree::configitem $netl_w.t /$net -image2 ""
      }
    }
  }
  
  proc configureitem {net args} {
    variable netl_w
    variable netbits

    set probe 0
    parseargs $args {-probe}

    if {$probe} {
      if { $netbits($net) == "" } {
	Tree::configitem $netl_w.t /$net -image2 [gifI net_probe.gif]
      } else {
	Tree::configitem $netl_w.t /$net -image2 [gifI net_probe2.gif]
      }
    } else {
      Tree::configitem $netl_w.t /$net -image2 ""
    }
  }

  proc del {net} {
    variable netl_w
    Tree::delitem $netl_w.t /$net
  }

  proc add {net args} {
    variable netl_w
    variable typeimages
    variable typeimagehelp
    variable netbits

    set bits 1
    set hidden 0
    set type "wire"
    set probe 0
    parseargs $args {-hidden -bits -type -probe}

    if {$bits == 1} {
      set netbits($net) ""
    } else {
      set type "${type}2"
      set netbits($net) "\[[expr $bits-1]:0\]"
    }
    if {$hidden} {
      set type "h-$type"
    }

    Tree::newitem $netl_w.t /$net -image $typeimages($type) -imagehelp $typeimagehelp($type)
  }

  proc clearselection {} {
    variable netl_w
    Tree::setselection $netl_w.t ""
  }

  proc getselection {args} {
    variable netl_w

    set path [Tree::getselection $netl_w.t]
    return [string trimleft $path  "/" ]
  }

  proc setselection {name} {
    variable netl_w

    Tree::setselection $netl_w.t $name
    action -Select { gat_selectNet [string range $name 1 end] }
  }

  proc shadowselection {name} {
    variable netl_w

    catch {
      Tree::setselection $netl_w.t /$name
    }
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
    global simOn

    action -Unselect { tkg_undoSelections nets }
    set lbl [Tree::labelat $w $x $y]
    setselection $lbl

    if { $simOn } {
      continueAction ToggleProbe { gat_toggleProbe [NetList::getselection] }
    } else {
      if { $lbl != "" } {
	continueAction EditNet { gat_editNet [NetList::getselection] }
      }
    }
  }

  proc create {w} {
    variable netl_w

    setupImages

    frame $w
    set netl_w $w

    Tree::create $w.t -width 50 -height 50 -bd 2 -relief sunken \
	-yscrollcommand "$w.vb set" -xscrollcommand "$w.hb set" -nolines 1
    scrollbar $w.vb -orient vertical -command "$w.t yview" -takefocus 0
    scrollbar $w.hb -orient horizontal -command "$w.t xview" -takefocus 0

    grid rowconfigure $w 0 -weight 1
    grid columnconfigure $w 0 -weight 1
    grid $w.t  -row 0 -column 0 -sticky nsew -padx 1 -pady 1
    grid $w.vb -row 0 -column 1 -sticky ns -padx 1 -pady 1
    grid $w.hb -row 1 -column 0 -sticky ew -padx 1 -pady 1

    bind $w.t <1> { NetList::seeB1Press %W %x %y %X %Y }
    bind $w.t <3> { NetList::seeB3Press %W %x %y %X %Y }
    bind $w.t <Double-ButtonPress-1> { NetList::seeDoubleB1 %W %x %y %X %Y }

    Tree::setsuparray $w.t NetList::netbits
  }
}
