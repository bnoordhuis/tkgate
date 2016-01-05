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
# Last edit by hansen on Sat Jan 17 19:53:38 2009
#
#

#############################################################################
#
# This procedure is called when a module from the BlockTree or BlockList
# is dropped in a window.
#
proc seeModuleDrop {m w x y} {
  global main_w

  if {$w == "$main_w.main"} {
    action Make "gat_make MODULE -x $x -y $y -func $m -doinit 1"
  }
}

#############################################################################
#
# These are methods related to the hierarchical module tree.
#
namespace eval BlockTree {
  variable root "main"
  variable children
  variable mtype
  variable typeimages
  variable typeimagehelp
  variable used
  variable modified 0
  variable wantOpenRoot 0
  variable activeMod ""
  variable modtree_w ""
  variable mouseset_x
  variable mouseset_y
  variable drag_obj
  variable Libraries [m module.box.libraries]

  #############################################################################
  #
  # Returns non-zero if p is a validly formed module path name
  #
  proc isModulePath {p} {
    set i [string last "/" $p]
    if {$i < 0 } {
      set i 0
    } else {
      set i [expr $i + 1]
    }
    if { [string index $p $i] == "|" } {
      return 0
    } else {
      return 1
    }
  }

  #############################################################################
  #
  # Flush all modules
  #
  proc flush {} {
    variable children
    variable mtype
    variable typeimages
    variable used
    variable root
    variable modified

    set modified 1

#    puts "BlockTree::flush"
    array unset children "*"
    set root ""
  }

  #############################################################################
  #
  # Make 'mod' the root module
  #
  proc setRoot {mod} {
    variable root
    variable mtype
    variable modified

    set modified 1

#    puts "BlockTree::setRoot $mod"

    catch { set mtype($root) module }
    set mtype($mod) root
    set root $mod
  }


  #############################################################################
  #
  # Delete a module from the module tree.
  #
  # Parameters:
  #      mod		Module to be deleted.
  #
  proc del {mod} {
    variable children
    variable modified

    set modified 1

    array unset children $mod
  }

  #############################################################################
  #
  # Make the list of children of a module empty.
  #
  # Parameters:
  #      mod		Module from which to disown children. 
  #
  proc disownChildren {mod} {
    variable children
    variable modified

    #    puts "BlockTree::disownChildren $mod"
    set modified 1
    set children($mod) {}
  }

  #############################################################################
  #
  # Usage:  add <mod> [switches]
  #
  # Switches:
  #     -locks <ll>		Locks to add to a module
  #     -type <t>		Type of the module.
  #
  # Add module <mod> to the module tree.  The -locks switch specifies a set of
  # lock types that apply to the module.  Currently only the "D" lock, representing
  # a lock on the module definition is supported.  The -type switch specifies the
  # type of the module.
  #
  proc add {mod args} {
    variable children
    variable mtype
    variable modified

#    puts "BlockTree::add $mod $args"

    set type netlist
    set locks ""
    parseargs $args {-locks -type}

    if {[catch {set children($mod)}]} {
      set children($mod) {}

      if { [string first "D" $locks] >= 0 } {
	set mtype($mod) mod_${type}_lock
      } else {
	set mtype($mod) mod_${type}
      }

      set modified 1
    }
  }

  #############################################################################
  #
  # Usage: addLibrary <lib>
  #
  # Parameters:
  #     <lib>		Library that the module is a member of.
  #
  # Add a library to the module tree
  #
  proc addLibrary {lib} {
    variable mtype
    variable modified
    variable children
    variable Libraries

    set lib ||$lib

    set modified 1
    set mtype($lib) partlib

    if {[catch {set children(|$Libraries)}]} {
      set mtype(|$Libraries) allparts
      set children(|$Libraries) {}
    }
    if {[catch {set children($lib)}]} {
      set children($lib) {}
    }
    if {[lsearch $children(|$Libraries) $lib] < 0 } {
      lappend children(|$Libraries) $lib
    }
  }


  #############################################################################
  #
  # Usage: addPart <mod> <lib>
  #
  # Parameters:
  #     <mod>		Name of module to add as a part.
  #     <lib>		Library that the module is a member of.
  #
  # Add a module as a part from a libarary. 
  #
  proc addPart {mod lib} {
    variable mtype
    variable modified
    variable children
    variable Libraries

#    puts "BlockTree::addPart $mod $lib"

    set lib ||$lib

    set modified 1
    add $mod
    set mtype($mod) part
    set mtype($lib) partlib

    if {[catch {set children(|$Libraries)}]} {
      set mtype(|$Libraries) allparts
      set children(|$Libraries) {}
    }
    if {[catch {set children($lib)}]} {
      set children($lib) {}
    }
    if {[lsearch $children($lib) $mod] < 0 } {
      lappend children($lib) $mod
    }
    if {[lsearch $children(|$Libraries) $lib] < 0 } {
      lappend children(|$Libraries) $lib
    }
  }

  #############################################################################
  #
  # Request that the root module be opened
  #
  proc openRoot {} {
    variable wantOpenRoot

    set wantOpenRoot 1
  }

  #############################################################################
  #
  # Usage: rebuildAt <path> <m>
  #
  # Parameters:
  #      <path>		Path from node at which to build tree.
  #      <m>            Name of module at which to build.
  #
  proc rebuildAt {path m} {
    variable modtree_w
    variable children
    variable mtype
    variable typeimages
    variable typeimagehelp
    variable used
    variable activeMod

#    puts "BlockTree::rebuildAt $path $m"

    if { [string first "/${m}/" $path ] < 0 } {
      set used($m) 1

      if {[info exists children($m)]} {
	Tree::newitem $modtree_w.t $path -image $typeimages($mtype($m)) -imagehelp $typeimagehelp($mtype($m))

	foreach c $children($m) {
	  rebuildAt ${path}/$c $c
	}
      } else {
	Tree::newitem $modtree_w.t $path -image $typeimages(unknown) -imagehelp $typeimagehelp(unknown)
      }
    } else {
      Tree::newitem $modtree_w.t $path -image $typeimages(conflict) -imagehelp $typeimagehelp(conflict)
    }
  }

  #############################################################################
  #
  # Clear the selection from the tree.
  #
  proc clearselection {} {
    variable modtree_w
    Tree::setselection $modtree_w.t ""
  }

  #############################################################################
  #
  # Usage: getselection [switches]
  #
  # Switches:
  #     -path <n>		If <n> is non-zero, get the selection by path.
  #
  # Get the currently selected module, or module path.
  #
  proc getselection {args} {
    variable modtree_w

    set path [Tree::getselection $modtree_w.t]

    if {[lsearch $args "-path"] >= 0} {
      return $path
    } else {
      set i [string last "/" $path]
      if { $i >= 0 } {
	return [string range $path [expr $i+1] end]
      } else {
	return $path
      }
    }
  }

  #############################################################################
  #
  # Set the current path (obsolete?)
  #
  proc setCurrent {} {
  }

  #############################################################################
  #
  # Rebuild the tree from scratch
  #
  proc rebuild {} {
    variable modtree_w
    variable root
    variable children
    variable typeimages
    variable typeimagehelp
    variable mtype
    variable used
    variable modified
    variable wantOpenRoot
    variable activeMod
    variable Libraries
    global tkg_currentPath
    global tkg_pathInstance


    if { !$modified } return
    set modified 0

    setCurrent

    catch { array unset BlockTree::used "*" }

    Tree::delitem $modtree_w.t /

    if { $root != "" } {
      rebuildAt /$root $root
    }

    if {![info exists children(|$Libraries)]} {
      set mtype(|$Libraries) allparts
      set children(|$Libraries) {}
    }
      rebuildAt /|$Libraries |$Libraries

    set did_unused 0

    #     Tree::newitem $modtree_w.t /|Libraries -image $typeimages(allparts)
    foreach m [array names children "*"] {
      if {[catch { set BlockTree::used($m) } ] && [isModule $m]} {

	if {!$did_unused} {
	  Tree::newitem $modtree_w.t /|Unused -image $typeimages(unused) -imagehelp $typeimagehelp(unused)
	  set did_unused 1
	}
	Tree::newitem $modtree_w.t /|Unused/$m -image $typeimages($mtype($m)) -imagehelp $typeimagehelp($mtype($m))
	foreach c $children($m) {
	  rebuildAt /|Unused/$m/$c $c
	}
      }
    }

    if { $wantOpenRoot } {
      Tree::open $modtree_w.t /$root
      set wantOpenRoot 0
    }

    Tree::setactive $modtree_w.t $tkg_currentPath
#    Tree::setsuparray $modtree_w.t $tkg_pathInstance
  }

  proc setActive {path} {
    variable modtree_w
    catch {
      Tree::clearactive $modtree_w.t
      Tree::setactive $modtree_w.t $path
    }
  }

  proc isUsed {m} {
    variable used
    if { [info exists used($m)] && $used($m) } {
      return 1
    } else {
      return 0
    }
  }

  #############################################################################
  #
  # Return true if m is some kind of module type.
  #
  proc isModule {m} {
    variable mtype
    if { $mtype($m) == "module"} { return 1 }
    if { $mtype($m) == "mod_netlist"} { return 1 }
    if { $mtype($m) == "mod_netlist_lock"} { return 1 }
    if { $mtype($m) == "mod_hdl"} { return 1 }
    if { $mtype($m) == "mod_hdl_lock"} { return 1 }
    if { $mtype($m) == "lockImod"} { return 1 }
    if { $mtype($m) == "lockDmod"} { return 1 }
    if { $mtype($m) == "lockIDmod"} { return 1 }
    return 0
  }

  #############################################################################
  #
  # Mark cmod as a child of pmod
  #
  proc makeChild {cmod pmod} {
    variable children
    variable modified

    set modified 1

    #    puts "BlockTree::makeChild $cmod $pmod"
    if {[lsearch $children($pmod) $cmod] < 0 } {
      lappend children($pmod) $cmod
    }
  }

  #############################################################################
  #
  #
  #
  proc setupImages {} {
    global bd
    variable typeimages
    variable typeimagehelp

    set typeimages(part) [gifI chip.gif]
    set typeimages(allparts) [gifI parts.gif]
    set typeimages(partlib) [gifI ichipdir.gif]
    set typeimages(unknown) [gifI unknown.gif]
    set typeimages(unused) [gifI unused.gif]
    set typeimages(conflict) [gifI conflict.gif]
    set typeimages(root) [gifI document.gif]

    set typeimages(mod_netlist)		[gifI mod_net.gif]
    set typeimages(mod_hdl) 		[gifI mod_hdl.gif]
    set typeimages(mod_netlist_lock) 	[gifI mod_netL.gif]
    set typeimages(mod_hdl_lock) 	[gifI mod_hdlL.gif]
    set typeimages(mod_part)		[gifI chip.gif]

    #
    # Set up help messages
    #
    set typeimagehelp(part)		[m ho.mod.type.part]
    set typeimagehelp(allparts)		[m ho.mod.type.allparts]
    set typeimagehelp(partlib)		[m ho.mod.type.partlib]
    set typeimagehelp(unknown)		[m ho.mod.type.unknown]
    set typeimagehelp(unused)		[m ho.mod.type.unused]
    set typeimagehelp(conflict)		[m ho.mod.type.conflict]
    set typeimagehelp(root)		[m ho.mod.type.root]

    set typeimagehelp(mod_netlist)	[m ho.mod.type.netlist]
    set typeimagehelp(mod_hdl)		[m ho.mod.type.hdl]
    set typeimagehelp(mod_netlist_lock)	[m ho.mod.type.Lnetlist]
    set typeimagehelp(mod_hdl_lock)	[m ho.mod.type.Lhdl]
    set typeimagehelp(mod_part)		[m ho.mod.type.part]
  }

  proc getPathBasename {path} {
    set i [string last / $path]
    if {$i >= 0 } {
      return [string range $path [expr $i + 1] end]
    } else {
      return $path
    }
  }

  #
  # Respond to motion of the mouse with button 1 depressed
  #
  proc seeMotion {x y} {
    variable modtree_w
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
	set icon [Tree::geticon $modtree_w.t $drag_obj]

	set dw [Dragger::make -command "seeModuleDrop $lbl"]
	if {$dw != "" } {
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

    if { $lbl != ""} {
      Tree::setselection $w $lbl
      set BlockTree::mouseset_x $X
      set BlockTree::mouseset_y $Y
      if { [BlockTree::isModulePath $lbl] } {
	set BlockTree::drag_obj $lbl
      } else {
	set BlockTree::drag_obj ""
      }
    } else {
      set BlockTree::drag_obj ""
    }
  }

  proc seeB3Press {w x y X Y} {
    variable Libraries

    action -Unselect { tkg_undoSelections blocks }
    set lbl [Tree::labelat $w $x $y]
    if { $lbl == ""} return

    Tree::setselection $w $lbl

    if { [BlockTree::isModulePath $lbl] } {
      tk_popup .pop_modlist $X $Y
    } else {
      set target [getPathBasename $lbl]
      if { $target == "|$Libraries" } {
	tk_popup  .pop_libraries $X $Y
      } else {
	tk_popup  .pop_libname $X $Y
      }
    }
  }

  proc seeDoubleB1 {w x y X Y} {
    set lbl [Tree::labelat $w $x $y]
    if { $lbl != "" && [BlockTree::isModulePath $lbl] } {
      Tree::setselection $w $lbl
      continueAction GotoBlock "gat_openBox $lbl"
    }
  }

  proc create {w} {
    variable modtree_w

    setupImages

    frame $w
    set modtree_w $w
    Tree::create $w.t -width 50 -height 50 -bd 2 -relief sunken \
	-yscrollcommand "$w.vb set" -xscrollcommand "$w.hb set"
    scrollbar $w.vb -orient vertical -command "$w.t yview" -takefocus 0
    scrollbar $w.hb -orient horizontal -command "$w.t xview" -takefocus 0

    grid rowconfigure $w 0 -weight 1
    grid columnconfigure $w 0 -weight 1
    grid $w.t  -row 0 -column 0 -sticky nsew -padx 1 -pady 1
    grid $w.vb -row 0 -column 1 -sticky ns -padx 1 -pady 1
    grid $w.hb -row 1 -column 0 -sticky ew -padx 1 -pady 1

#    $w.t bind x <1> { BlockTree::seeB1Press %W %x %y %X %Y }
#    $w.t bind x <3> { BlockTree::seeB3Press %W %x %y %X %Y }
#    $w.t bind x <B1-Motion> { BlockTree::seeMotion %X %Y }
#    $w.t bind x <Double-ButtonPress-1> { BlockTree::seeDoubleB1 %W %x %y %X %Y }

    bind $w.t <1> { BlockTree::seeB1Press %W %x %y %X %Y }
    bind $w.t <3> { BlockTree::seeB3Press %W %x %y %X %Y }
    bind $w.t <B1-Motion> { BlockTree::seeMotion %X %Y }
    bind $w.t <Double-ButtonPress-1> { BlockTree::seeDoubleB1 %W %x %y %X %Y }
  }
}
