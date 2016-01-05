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
# Last edit by hansen on Sat Jan 10 18:35:23 2009
#

#set modtabImageMap(NList) [gifI modlist.gif]
#set modtabImageMap(List) [gifI modlist.gif]
#set modtabImageMap(Tree) [gifI modtree.gif]

#############################################################################
#
# New-style block lists goes here
#
namespace eval BlockList {
  variable bl_w ""
  variable typeimages
  variable typeimagehelp
  variable moduletypes
  variable mouseset_x
  variable mouseset_y
  variable drag_obj

  #
  # Respond to motion of the mouse with button 1 depressed
  #
  proc seeMotion {x y} {
    variable bl_w
    variable drag_obj
    variable mouseset_x
    variable mouseset_y

    if { $drag_obj != "" } {
      set dx [expr $mouseset_x - $x]
      set dy [expr $mouseset_y - $y]
      if { [expr $dx*$dx + $dy*$dy] > 25 } {
	set i [string last / $drag_obj]
	if {$i >= 0 } {
	  set lbl [string range $drag_obj [expr $i + 1] end]
	} else {
	  set lbl $drag_obj
	}
	set icon [Tree::geticon $bl_w.t $drag_obj]

	set dw [Dragger::make -command "seeModuleDrop $lbl"]
	if { $dw != "" } {
	  label $dw.icon -image $icon
	  label $dw.label -text $lbl
	  pack $dw.icon $dw.label -padx 2 -pady 2 -side left
	}
      }
    }
  }

  proc seeB1Press {w x y X Y} {
    action -Unselect { tkg_undoSelections blocks }
    set lbl [Tree::labelat $w $x $y]
    Tree::setselection $w $lbl
    set BlockList::mouseset_x $X
    set BlockList::mouseset_y $Y
    set BlockList::drag_obj $lbl
  }

  proc seeB3Press {w x y X Y} {
    action -Unselect { tkg_undoSelections blocks }
    set lbl [Tree::labelat $w $x $y]
    Tree::setselection $w $lbl
    if { $lbl != ""} {
      tk_popup .pop_modlist $X $Y
    }
  }

  proc seeDoubleB1 {w x y X Y} {
    set lbl [Tree::labelat $w $x $y]
    if { $lbl != "" } {
      continueAction GotoBlock "gat_openBox $lbl"
    }
  }



  proc create {w} {
    variable bl_w
    frame $w
    set bl_w $w

    setupImages

    Tree::create $w.t -width 50 -height 50 -bd 2 -relief sunken \
	-yscrollcommand "$w.vb set" -xscrollcommand "$w.hb set" -nolines 1
    scrollbar $w.vb -orient vertical -command "$w.t yview" -takefocus 0
    scrollbar $w.hb -orient horizontal -command "$w.t xview" -takefocus 0

    grid rowconfigure $w 0 -weight 1
    grid columnconfigure $w 0 -weight 1
    grid $w.t  -row 0 -column 0 -sticky nsew -padx 1 -pady 1
    grid $w.vb -row 0 -column 1 -sticky ns -padx 1 -pady 1
    grid $w.hb -row 1 -column 0 -sticky ew -padx 1 -pady 1

    bind $w.t <1> { BlockList::seeB1Press %W %x %y %X %Y }
    bind $w.t <3> { BlockList::seeB3Press %W %x %y %X %Y }
    bind $w.t <B1-Motion> { BlockList::seeMotion %X %Y }
    bind $w.t <Double-ButtonPress-1> { BlockList::seeDoubleB1 %W %x %y %X %Y }
  }

  proc clearselection {} {
    variable bl_w
    Tree::setselection $bl_w.t ""
  }

  proc getselection {args} {
    variable bl_w

    set path [Tree::getselection $bl_w.t]
    return [string trimleft $path  "/" ]
  }

  proc add {name args} {
    global tkg_currentModule
    variable bl_w
    variable typeimages
    variable typeimagehelp
    variable moduletypes

    set locks ""
    parseargs $args {-locks -type}

    set moduletypes($name) module

    if {$locks == "D" || $locks == "ID" } {
      set mtype mod_${type}_lock
    } else {
      set mtype mod_${type}
    }
    Tree::newitem $bl_w.t /$name -image $typeimages($mtype) -imagehelp $typeimagehelp($mtype)

    if { $name == $tkg_currentModule } {
      Tree::setactive $bl_w.t /$name
    }
  }
  proc addPart {name} {
    global tkg_currentModule
    variable bl_w
    variable typeimages
    variable typeimagehelp
    variable moduletypes

    set moduletypes($name) part
    Tree::newitem $bl_w.t /$name -image $typeimages(part) -imagehelp $typeimagehelp(part)

    if { $name == $tkg_currentModule } {
      Tree::setactive $bl_w.t /$name
    }
  }
  proc setRoot {name} {
    global tkg_currentModule
    variable bl_w
    variable typeimages
    variable typeimagehelp
    variable moduletypes

    set moduletypes($name) root
    Tree::delitem $bl_w.t /$name
    Tree::newitem $bl_w.t /$name -image $typeimages(root) -imagehelp $typeimagehelp(root)

    if { $name == $tkg_currentModule } {
      Tree::setactive $bl_w.t /$name
    }
  }
  proc flush {} {
    variable bl_w
    variable moduletypes

    array unset moduletypes "*"

    Tree::delitem $bl_w.t /
  }

  proc setupImages {} {
    variable typeimages
    variable typeimagehelp

    set typeimages(part)		[gifI chip.gif]
    set typeimages(root)		[gifI document.gif]
    set typeimages(mod_netlist)		[gifI mod_net.gif]
    set typeimages(mod_hdl) 		[gifI mod_hdl.gif]
    set typeimages(mod_netlist_lock) 	[gifI mod_netL.gif]
    set typeimages(mod_hdl_lock) 	[gifI mod_hdlL.gif]


    #
    # Set up help messages
    #
    set typeimagehelp(part)		[m ho.mod.type.part]
    set typeimagehelp(root)		[m ho.mod.type.root]
    set typeimagehelp(mod_netlist)	[m ho.mod.type.netlist]
    set typeimagehelp(mod_hdl)		[m ho.mod.type.hdl]
    set typeimagehelp(mod_netlist_lock)	[m ho.mod.type.Lnetlist]
    set typeimagehelp(mod_hdl_lock)	[m ho.mod.type.Lhdl]

  }

  proc getallmodules args {
    variable bl_w

    # -types is a list of one or more of: module, root, part
    set exclude {}
    set types {}
    parseargs $args {-types -exclude}

    set L [Tree::getchildren $bl_w.t /]

    set L2 $L
    set L {}
    foreach m $L2 {
      if {[lsearch $exclude [getType $m]] >= 0} {
	continue
      }
	
      if {[llength $types] == 0 || [lsearch $types [getType $m]] >= 0} {
	lappend L $m
      }
    }

    return $L
  }

  proc getType {name} {
    variable moduletypes
    return $moduletypes($name)
  }

  proc isLibrary {name} {
    variable bl_w
    variable moduletypes

    if { $moduletypes($name) == "part" } {
      return 1
    } else {
      return 0
    }
  }
}


#############################################################################
#
# This is called when a tab is pressed to switch the visible item in the
# block-list/block-tree tab box interface.
#
proc blocklist_tabs {w tab} {
  switch $tab {
    List {
      catch { pack forget $w.tree }
      catch { pack forget $w.nlist }
#      catch { pack $w.list -fill both -expand 1 -padx 2 -pady 2  }
    }
    Tree {
#      catch { pack forget $w.list }
      catch { pack forget $w.nlist }
      catch { pack $w.tree -fill both -expand 1 -padx 2 -pady 2  }
    }
    NList {
#      catch { pack forget $w.list }
      catch { pack forget $w.tree }
      catch { pack $w.nlist -fill both -expand 1 -padx 2 -pady 2  }
    }
  }
}

proc tkg_blockListTest {} {
  button .open -text Open -command tkg_blockListTestSetup
  pack .open
}


