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
# Last edit by hansen on Tue Feb  3 15:01:14 2009

#
# The top-level window layout is:
#
#     +------------------------------------+
#     |   .tbar                            |
#     +------------------------------------+
#     |   .pad                             |
#     +---------------------+--------------+
#     | .pane.sbar.strut    |              |
#     +---------------------+              |
#     |                     |              |
#     | .pane.sbar.blklst   |  .pane.frame |
#     |                     |              |
#     +---------------------+              |
#     |                     |              |
#     | .pane.sbar.nptab    |              |
#     |                     |              |
#     +---------------------+--------------+
#     |   .status                          |
#     +------------------------------------+
#
# The main part of the window is built in .pane.frame.  Within this is the major mode tabbox with
# the contents in $tabframe_w (.pane.frame.cf.tab.f.f).  The grid layout for tabframe_w is:
#
#            0             1
#     +------------+---------------+
#     |  ..ipanel  | ..smg.symed   |
#     |            +---------------+
#     |            | ..smg.maing   |
#     +------------+---------------+
#
# .pane.frame.cf.tab.f.f.smg.maing ($main_w) is a grid with the layout:
#
#         0          1
#     +---------+----------+
#  0  | ..main  | ..vert   |
#     +---------+----------+
#  1  | ..horz  |          |
#     +---------+----------+
#
#


#############################################################################
#
# Set up constants for major mode tabs
#
set main_images [list [gifI editmode.gif] [gifI editintr.gif] [gifI simstart.gif]]
set main_tabnames {Edit EditInterfaces Simulate}
set main_tablabels [list [m tab.edit] [m tab.editintr] [m tab.simulate]]
set maintabs_inswitch 0

#############################################################################
#
# Add critical path options
#
if { $tkg_enableCPath } {
  lappend main_images [gifI analyze.gif]
  lappend main_tabnames CriticalPath
  lappend main_tablabels [m tab.cpath]
}

#############################################################################
#
# Main tab handling function
#
# This function is called when the user selects a main mode tab.
#
proc main_tabs {w tab} {
  global maintabs_inswitch
  global tkgate_isInitialized

  if { ! $tkgate_isInitialized } return


#  puts main_tabs

  set maintabs_inswitch 1
  switch $tab {
    Edit {
      global simOn
      action EditMode {
	gat_setMajorMode +edit
      }
    }
    EditInterfaces {
      global simOn

      set result [gat_setMajorMode +edit]
      if { $result != "cancel" } {
	action EditInterf {
	  gat_editBlockDesc
	}
      }
    }
    Simulate {
      global simOn

      action EditMode {
	gat_setMajorMode +edit
      }
      tkg_simRun
    }
    CriticalPath {
      action EditMode {
	gat_setMajorMode +edit
      }
      tkg_cpathAnal
    }
  }
  set maintabs_inswitch 0
}

#############################################################################
#
# Synchronize main tab setting with actual state.
#
# This function is called as part of the idle event handler.
#
proc syncMainTabs {which} {
  global main_tabnames
  global maintabs_inswitch
  global syncProps

  if { $maintabs_inswitch } { return }

#  puts "syncMainTabs $which"

  switch $which {
    EditInterfaces {
#      Menu::setState interface   (State is set elsewhere)
      NetList::configureall -probe 0
      HdlEditor::hide
      HdlEditor::disable
      ToolBar::setMode EditInterfaces
      InfoPanel::setMode Edit
      ScriptMgr::flush
    }
    Edit {
      global tkg_currentModule

      NetList::configureall -probe 0
      Menu::setState edit
      IPanel::hide
      HdlEditor::enable

      set syncProps(name) $tkg_currentModule
      gat_editModProps load syncProps $tkg_currentModule
      if { $syncProps(type) == "hdl" } {
	HdlEditor::show $tkg_currentModule
	ToolBar::setMode HdlEdit
      } else {
	HdlEditor::hide
	ToolBar::setMode Edit
      }
      InfoPanel::setMode Edit
      ScriptMgr::flush
    }
    Simulate {
      global tkg_currentModule
      IPanel::hide

      Menu::setState simulate
      set syncProps(name) $tkg_currentModule
      gat_editModProps load syncProps $tkg_currentModule
      if { $syncProps(type) == "hdl" } {
	HdlEditor::show $tkg_currentModule
	HdlEditor::disable
      } else {
	HdlEditor::hide
      }
      ToolBar::setMode Simulate
      InfoPanel::setMode Simulate
    }
    CriticalPath {
      NetList::configureall -probe 0
      Menu::setState cpath
      HdlEditor::hide
      HdlEditor::disable
      IPanel::hide
      ToolBar::setMode CriticalPath
      InfoPanel::setMode Edit
      ScriptMgr::flush
    }
  }

  catch { set i [lsearch $main_tabnames $which] }

  TabBox::setactive .pane.frame.cf.tab $which
}

#############################################################################
#
# Build the main editing window.
#
proc tkg_makeMain {w} {
  global main_w tabframe_w tkg_initialWidth tkg_initialHeight main_tabnames main_tablabels main_images
  global tkg_enableCPath tkg_mmSplitEdit tkg_borderSize

  frame $w
  frame $w.cf -relief sunken -bd 1

  TabBox::new $w.cf.tab -passive 1 -tabs $main_tabnames -command main_tabs -tablabels $main_tablabels -tabimages $main_images -expand 1
  pack $w.cf.tab -fill both -expand 1

  set tabframe_w $w.cf.tab.f

  frame $tabframe_w -padx $tkg_borderSize -pady $tkg_borderSize

  pack $tabframe_w -side top -fill both -expand 1
  IPanel::create $tabframe_w.ipanel

  frame $tabframe_w.smg
  grid columnconfigure $tabframe_w 1 -weight 1
  grid rowconfigure $tabframe_w 0 -weight 1
  grid $tabframe_w.smg -row 0 -column 1 -sticky nsew

  frame $tabframe_w.smg.maing
  pack $tabframe_w.smg.maing -fill both -expand 1

  #
  # main_w will be the grid we build everything in, but we won't actually insert the
  # edit window until later.
  #
  set main_w $tabframe_w.smg.maing
  scrollbar $main_w.vert -command "$main_w.main yview" -takefocus 0 
  scrollbar $main_w.horz -orient horizontal -command "$main_w.main xview" -takefocus 0
  HdlEditor::new $main_w.hdl -ismain 1
  SymbolEdit::create $tabframe_w.smg.symed

  grid columnconfigure $main_w 0 -weight 1
  grid rowconfigure $main_w 0 -weight 1

  grid $main_w.vert -column 1 -row 0 -sticky ns
  grid $main_w.horz -column 0 -row 1 -sticky ew

  bind $main_w.vert <ButtonPress> { $main_w.main yview press %b }
  bind $main_w.horz <ButtonPress> { $main_w.main xview press %b }
  bind $main_w.vert <ButtonRelease> { gat_obUnstick }
  bind $main_w.horz <ButtonRelease> { gat_obUnstick }

  TabBox::addpassive $w.cf.tab $tabframe_w
  
  helpon $w.cf.tab.bEdit [m ho.tab.edit]
  helpon $w.cf.tab.bEditInterfaces [m ho.tab.interface]
  helpon $w.cf.tab.bSimulate [m ho.tab.simulate]
  if { $tkg_enableCPath } {
    helpon $w.cf.tab.b3 [m ho.tab.cpath]
  }

#  frame $w.l -width 5
#  pack $w.l -side right
#  pack $w.cf -fill both -expand 1 -padx 0 -pady 0

  InfoPanel::make $w.cf $w.mmgr

  pane $w.cf $w.mmgr -orient vertical -handlelook {-relief flat -height 10} \
      -decoration paneDecoration -fractions [list [expr 1.0-$tkg_mmSplitEdit] $tkg_mmSplitEdit]

#  frame $w.pad -bd 2 -width 5 -height 5 -relief raised
#  pack $w.pad -side right -fill both -expand 1
}

proc setupIconWindow {} {
  global bd

#  toplevel .gateicon
#  label .gateicon.img -image [gifI gatelogo.gif]
#  pack .gateicon.img
#  wm iconwindow . .gateicon

  wm iconbitmap . "@$bd/gateicon.xbm"
}

#############################################################################
#
# Set up the basic structure of the main tkgate interface.
#
proc setupMainWindowPanes {} {
  global tkg_initialWidth tkg_initialHeight

#  frame .mbar -takefocus 1
  frame .tbar
  frame .pad
  frame .status
  frame .pane

  frame .pane.sbar
  tkg_makeMain .pane.frame

#  pack .mbar -fill x
  pack .tbar -fill x
  pack .pad  -pady 2
  pack .status -fill x -side bottom
  pack .pane -fill both -expand 1

  #
  # Fraction of space used by the right-hand side panel with the module,
  # net and port lists.  Use the actual width of an "M" character in the
  # current "tree" font to scale the width.
  #
  set barWidth [expr 0.165 * ([font measure Tree:font "M"]*0.111*0.4 + 0.6)]

  .pane configure -width $tkg_initialWidth -height $tkg_initialHeight
  .pane.sbar configure -width 175

  pane .pane.sbar .pane.frame -orient horizontal -handlelook {-relief flat -width 10} \
      -decoration paneDecoration -fractions [list $barWidth [expr 1.0 - $barWidth]]
}

#############################################################################
#
# Undo selections for everything except 'sel' where 'sel'
# is one of 'blocks', 'nets', 'gate'
#
proc tkg_undoSelections {sel} {
   switch $sel {
     gate {
       BlockTree::clearselection
       BlockList::clearselection
       NetList::clearselection
     }

     nets {
       gat_unselectGates
       BlockTree::clearselection
       BlockList::clearselection
     }
     blocks {
       gat_unselectGates
       NetList::clearselection
     }
   }
}
