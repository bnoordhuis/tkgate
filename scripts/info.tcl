#   Copyright (C) 1987-2006 by Jeffery P. Hansen
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
# Last edit by hansen on Fri Jan  9 08:57:44 2009


#############################################################################
#
# The info panel (InfoPanel) is used to 
#  * display informational messages in a scrollable window 
#  * display current breakpoint status and edit breakpoints
#  * display active scripts.
#
namespace eval InfoPanel {
  variable log_w
  variable mgr_w
  variable edit_w
  variable active_mode Edit

  #############################################################################
  #
  # Change the InfoPanel according to the specified action
  #
  # 	Edit -		We are entering edit mode
  #	Simulate -	We are entering simulate mode
  #	Log -		We have been requested to log a message
  #
  proc setMode {action} {
    variable edit_w
    variable mgr_w
    variable active_mode
    global tkg_mmSplitEdit tkg_mmSplitSimulate tkg_mmSplitEditMin
    global tkg_mmSplitEditLog tkg_mmSplitSimulateMin
    global mode

    if {$action == $active_mode } return

    set split 0
    set cur_split [lindex [pane cget [winfo parent $mgr_w] -fractions] 1]

    if {$active_mode == "Edit" } {
      set tkg_mmSplitEdit $cur_split
    } elseif {$active_mode == "Simulate" } {
      set tkg_mmSplitSimulate $cur_split
    }

    switch $action {
      Edit { 
	set split [max $tkg_mmSplitEdit $tkg_mmSplitEditMin ]
	set active_mode Edit
      }
      Simulate {
	set split [max $tkg_mmSplitSimulate $tkg_mmSplitSimulateMin ]
	set active_mode Simulate
      }
      Log {
	if { [gat_getMajorMode] == "simulate" } {
	  set split [max $tkg_mmSplitSimulate $tkg_mmSplitSimulateMin ]
	} else {
	  set split [max $tkg_mmSplitEdit $tkg_mmSplitEditLog ]
	}
      }
    }

    if {$split != $cur_split } {
      pane configure $edit_w $mgr_w -orient vertical -handlelook {-relief flat -height 10} \
	  -decoration paneDecoration -fractions [list [expr 1.0 - $split] $split]
    }
  }

  proc log {msg args} {
    variable log_w

    set noshow 0
    set color ""
    parseargs $args {-noshow -color} 

    if { !$noshow } {
      setMode Log
    }

    $log_w configure -state normal
    if {$color == ""} {
      $log_w insert end "$msg\n"
    } else {

      $log_w tag configure $color -foreground $color
      $log_w insert end "$msg\n" $color
    }


    $log_w configure -state disabled
    $log_w see end
    InfoPanel::notify Log
  }

  proc msgmake {w} {
    variable log_w

    frame $w -padx 2 -pady 2
    set log_w $w.text
    text $w.text -background white -yscrollcommand "$w.vb set" -state disabled -takefocus 0  -height 0 -relief sunken
    scrollbar $w.vb -command "$w.text yview" -takefocus 0 
    pack $w.text -side left -expand 1 -fill both
    pack $w.vb -side left -fill y
  }

  proc notify {tab args} {
    variable mgr_w
    global tkg_tabHighlight

    if { [TabBox::getactive $mgr_w.tab] == $tab } return

    set force 0
    parseargs $args {-force}

    if { $force } {
      TabBox::select $mgr_w.tab 1
      return
    }

    set w ""
    switch $tab {
      Log {
	set w $mgr_w.tab.b0
      }
      Breakpoints {
	set w $mgr_w.tab.b1
      }
      Scripts {
	set w $mgr_w.tab.b2
      }
    }

    catch "$w configure -bg $tkg_tabHighlight"
    catch "$w.l configure -bg $tkg_tabHighlight"
    catch "$w.img configure -bg $tkg_tabHighlight"
  }

  proc msg_tabs {w tab} {
    variable mgr_w

    switch $tab {
      Log {
	msgmake $w
      }
      Breakpoints {
	Breakpoint::create $w
      }
      Scripts {
	ScriptMgr::create $w
      }
      SimOptions {
	SimOptions::create $w
      }
    }
  }

  proc make {ew w} {
    variable mgr_w
    variable edit_w

    set mgr_w $w
    set edit_w $ew

    set images [list [gifI log.gif] [gifI sim_break.gif] [gifI sim_script.gif] [gifI simoptions.gif]]

    frame $w
    TabBox::new $w.tab -expand 1 -tabs {Log Breakpoints Scripts SimOptions} -tablabels {"" "" "" ""} -command InfoPanel::msg_tabs -tabside left -tabimages $images

    pack $w.tab -expand 1 -fill both

    helpon $w.tab.bLog [m ho.log]
    helpon $w.tab.bBreakpoints [m ho.simbreak]
    helpon $w.tab.bScripts [m ho.simexec]
    helpon $w.tab.bSimOptions [m ho.simoptions]
  }

}
