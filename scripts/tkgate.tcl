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
# Last edit by hansen on Sat Feb 21 22:32:47 2009
#

#############################################################################
#
# This file is the entry point into the tcl/tk code executed by tkgate.  Before
# this file is executed, the following variables are set in the C portion of
# tkgate:
#
#    sd				The directory with tcl script files
#    bd				The directory with bitmaps/images
#    lang			Language code (e.g. "en", "ja", "fr", ...)
#    territory			Territory code (e.g., "US", "GB", "JP", ...)
#    simOn			Flag indicating if the simulator is active.
#    tkg_gateHome		The home directory for tkgate.
#    tkg_doSplash		Indicates if a splash window should be displayed.
#    tkg_currentFile		Name of current file being edited.
#    tkg_tutotialDir		Directory with tutorial files.
#    tkg_exampleDir		Directory with example files.
#    tkg_progVer		TkGate version number
#    tkg_description		Text descrition of tkgate
#    tkg_copyright		Copyright message
#    tkg_mailContact		E-mail address of tkgate maintainer
#    tkg_homepage		Homepage of  tkgate maintainer
#    tkg_localdoc		Location of local html format documentation


#############################################################################
#
# Minimm scripts we need to begin tkgate initialization
#
set startScriptSet { misc options optdefaults parms}

#############################################################################
#
# The rest of tkgate's scripts
#
set baseScriptSet {
  keys ipanel dragger tabbox notebook pane dropbox helpon tree
  spreadsheet wizard frame info blocktree blockops blocklist netlist
  portlist sidebar menu memview popups setselector status print scope toolbar
  message errbox editgate find circuit simulator simopts breakpoint
  scriptmgr vpd file optionsdialog cpath license undo action symboledit
  hdledit library debug init
}


proc mainWinButton12Press {w x y s b} {
  global tkg_fakeDoubleClicks
  global tkg_fakeDCInterval
  global lastKeyPressTime

  action -Press { tkg_buttonPress $w $x $y $s $b }

  if {$tkg_fakeDoubleClicks} {
    set t [clock clicks -milliseconds]
    set interval [expr $t-$lastKeyPressTime]
    if {$interval < $tkg_fakeDCInterval} {
      mainWinButton12DoublePress $w $x $y $s
    }

    set lastKeyPressTime $t
  }
}

proc mainWinButton12DoublePress {w x y s} {
  continueAction -DblPress { tkg_buttonDoublePress $w $x $y $s } 
}

proc mainWinButton12Release {w x y s} {
  continueAction - { tkg_buttonRelease $w $x $y $s } 
}

proc mainWinButtonMotion {w x y s} {
  continueAction - { tkg_buttonMotion $w $x $y $s}
}

proc mainWinButton3Press {w x y s X Y} {
  action -Select { PopupMenu::post $w $x $y $s $X $Y }
}

#############################################################################
#
# This is the function that is called to start up tkgate.
#
proc startTkGate {argv} {
  global tkgate_isInitialized mode tkg_allowDebug
  global tkg_progName tkg_progVer tkg_gateHome tkg_prefFile
  global main_w tkg_checkpointFreq tkg_keyBindings
  global tkg_minWidth tkg_minHeight
  global tkg_fontSet
  global bd sd
  global tkg_fakeDoubleClicks
  global startScriptSet baseScriptSet

  #############################################################################
  #
  # Set flag to indicate that initialization is not complete. 
  #
  set tkgate_isInitialized 0

  #############################################################################
  #
  # If argument list starts with "--", remove it. 
  #
  if { [lindex $argv 0] == "--" } {
    set argv [lrange $argv 1 end]
  }

  #############################################################################
  #
  # If the tcl/tk version is less than 8.4, then there are certain
  # unsupported options for menus that we must remember.
  #
  global menu_forbidden
  if {[info tclversion] < 8.4} {
    set menu_forbidden {-compound -image -selectimage}
  } else {
    set menu_forbidden {}
  }

  #############################################################################
  #
  # Set up pointers to important directories.
  #
  set sd [file dirname [info script]]
  set bd [file dirname $sd]/images

  #############################################################################
  #
  # Run tkgate's main
  #
  eval tkgate_main $argv
  catch { package require Tk }
  tkgate_posttk

  #############################################################################
  #
  # Load in the minimum tcl files we need to start the splash screen.
  #
  foreach f $startScriptSet {
    uplevel "source $sd/${f}.tcl"
  }

  #############################################################################
  #
  # Read the site default preferences (global to all users)
  #
  if {[file exists $tkg_gateHome/site-preferences]} {
    uplevel "source $tkg_gateHome/site-preferences"
  }

  #############################################################################
  #
  # Read the user specific preferences (in user's home directory)
  #
  if {[file exists $tkg_prefFile]} {
    uplevel "source $tkg_prefFile"
  }


  #############################################################################
  #
  # Begin display of the splash window (if it has not been disabled)
  #
  start_splash


  #############################################################################
  #
  # Load the rest of the tcl/tk code for tkgate.
  #
  foreach f $baseScriptSet {
    uplevel "source $sd/${f}.tcl"
  }


  #############################################################################
  #
  # Define fonts to be used.
  #
  setFontSet ${tkg_fontSet}

  #############################################################################
  #
  # Initialize the options database 
  #
  setupOptions


  #############################################################################
  #
  # Printer dialog box initializations
  #
  printDlgInit
  
  #############################################################################
  #
  # Make all of the popup menus
  #
  makePopups

  #############################################################################
  #
  # If debugging is enabled, set up debugging key bindings.
  #
  if { $tkg_allowDebug } {
    KeyBinding::new <F7>		Action::dumpWires
    KeyBinding::new <Alt-v>		Action::verify
    KeyBinding::new <F6>		Action::debugMode
  }

  #############################################################################
  #
  # Set up hex and number entry tags
  #
  hexEntrySetup
  numEntrySetup
  floatEntrySetup

  #############################################################################
  #
  # Set up main window panes.
  #
  setupMainWindowPanes
  wm title . "$tkg_progName $tkg_progVer"
  wm minsize . $tkg_minWidth $tkg_minHeight
  setupIconWindow


  #############################################################################
  #
  # Side bar setup
  #
  SideBar::new .pane.sbar

  #############################################################################
  #
  # Status bar setup
  #
  makeStatus .status
  after [expr $tkg_checkpointFreq * 1000] tkg_checkpointReq

  #############################################################################
  #
  # Set key bindings
  #
  KeyBinding::loadBindings $tkg_gateHome/bindings/$tkg_keyBindings

  #############################################################################
  #
  # Toolbar initialization
  #
  ToolBar::init
  if { $tkg_allowDebug } {
    Menu::setFlags D
  }


  #############################################################################
  #
  # Create the main canvas and define bindings for mouse activity actions.
  #
  tkg_gatewin $main_w.main   -width 100 -height 100 -takefocus 0 \
      -xscrollcommand "$main_w.horz set" -yscrollcommand "$main_w.vert set" -bg seashell
  grid $main_w.main -row 0 -column 0 -sticky nsew

  bind $main_w.main <Button-1> 		{ mainWinButton12Press %W %x %y %s %b }
  bind $main_w.main <Button-2> 		{ mainWinButton12Press %W %x %y %s %b }
  if {!$tkg_fakeDoubleClicks} {
    bind $main_w.main <Double-Button-1>	{ mainWinButton12DoublePress %W %x %y %s }
    bind $main_w.main <Double-Button-2>	{ mainWinButton12DoublePress %W %x %y %s }
  }
  bind $main_w.main <ButtonRelease-1>	{ mainWinButton12Release %W %x %y %s }
  bind $main_w.main <ButtonRelease-2>	{ mainWinButton12Release %W %x %y %s }
  bind $main_w.main <Motion>		{ mainWinButtonMotion %W %x %y %s }
  bind $main_w.main <Button-3>		{ mainWinButton3Press %W %x %y %s %X %Y }
  bind $main_w.main <MouseWheel>	{ puts mouseWheel } 

  #############################################################################
  #
  # The focus should stay here automatically, but in case it doesn't make sure
  # a click restores focus to the menu bar to key commands can be processed
  # properly.
  #
  bind $main_w.main <Button-1>	{+ focus .tbar.m }
  bind $main_w.main <Button-2>	{+ focus .tbar.m }

  #############################################################################
  #
  # This forces the forces the focus to be returned to the menu bar (which
  # processes keyboard commands) any time the mouse is pressed in the main
  # window.  In most cases, the focus should stay there automatically.  This
  # handler is a contingency.
  #
  bind $main_w <Button> ToolBar::takeFocus


  #############################################################################
  #
  # Start the cpath flasher (currently disabled)
  #
  #tkg_flashCPath

  #############################################################################
  #
  # Set up initial mode.
  #
  ToolBar::modeChanged $mode
  Menu::setState edit

  #############################################################################
  #
  # Do additional initialization required on the C interface side.
  #
  gat_init

  #############################################################################
  #
  # Create the keyboard shortcuts for creating gates.
  #
  gat_makeMakeShortcuts

  #############################################################################
  #
  # Load any VPD scripts in the current VPD path.
  #
  VPD::loadScripts

  #############################################################################
  #
  # We are done with the splash window so we can take it down now.
  #
  end_splash

  #############################################################################
  #
  # Set the flag to indicate that tkgate is initialized, then do an update to
  # force tcl/tk to execute any idle commands.
  #
  set tkgate_isInitialized 1
  update
}

#############################################################################
#
# Start the tkgate interface.
#
startTkGate $argv

