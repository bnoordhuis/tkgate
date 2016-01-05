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
# Last edit by hansen on Fri Feb 13 20:46:38 2009
#

set runlogo_state -1
set runlogo_speed 50
set runlogo_startstate 27
set runlogo_maxstate 50

proc updateStatus args {
  global tkg_displayFile tkg_currentModule tkg_modifiedFlag tkg_statusMessage
  global tkg_progName tkg_progVer

  if { $tkg_modifiedFlag } {
     set mod "*"
  } else {
     set mod ""
  }
  
  .status.file configure -text "[m ifile]: $tkg_displayFile$mod"
  .status.block configure -text "[m imodule]: $tkg_currentModule"
  .status.msg configure -text "$tkg_statusMessage" -anchor w

   wm title . "$tkg_displayFile$mod - $tkg_progName $tkg_progVer"

}

proc tkg_resetLogo {} {
  global bd runlogo_state runlogo_startstate logo_w

  catch {
    $logo_w configure -image "" -image [gifI [format "run%02d.gif" $runlogo_startstate]]
    set runlogo_state $runlogo_startstate
  }
}

proc tkg_pauseLogo {} {
  global bd runlogo_ev

  catch { after cancel $runlogo_ev }
}

proc tkg_stepLogo {} {
  global bd runlogo_state runlogo_speed runlogo_ev runlogo_maxstate logo_w

  catch {
    set filename [format "run%02d.gif" ${runlogo_state}]
    $logo_w configure -image "" -image [gifI "$filename"]
    set runlogo_state [expr $runlogo_state + 1 ]
    if { $runlogo_state > $runlogo_maxstate } {
      set runlogo_state 1
    }
  }
}

proc tkg_stepRunLogo {} {
  global bd runlogo_state runlogo_speed runlogo_ev runlogo_maxstate logo_w

  catch {
    set filename [format "run%02d.gif" ${runlogo_state}]
    $logo_w configure -image "" -image [gifI "$filename"]
    set runlogo_state [expr $runlogo_state + 1 ]
    if { $runlogo_state > $runlogo_maxstate } {
      set runlogo_state 1
    }
    set runlogo_ev [after $runlogo_speed tkg_stepRunLogo]
  }
}

proc tkg_runLogo {} {
  global bd runlogo_speed runlogo_ev


  catch {
    catch { after cancel $runlogo_ev }
    set runlogo_ev [after $runlogo_speed tkg_stepRunLogo]
  }
}

proc tkg_editLogo {} {
  global bd runlogo_ev logo_w

  catch {
    catch { after cancel $runlogo_ev }
    $logo_w configure -image [gifI "gatelogo.gif"]
  }
}

proc tkg_analysisLogo {} {
  global bd runlogo_ev logo_w

  catch {
    catch { after cancel $runlogo_ev }
    $logo_w configure -image [gifI "anallogo.gif"]
  }
}

proc tkg_checkpointReq {} {
  global tkg_wantCheckpoint tkg_checkpointEnabled tkg_checkpointFreq

  if { $tkg_checkpointEnabled } {
    set tkg_wantCheckpoint 1
  }
    after [expr $tkg_checkpointFreq * 1000] tkg_checkpointReq
}

#############################################################################
#
# Add or remove the simulation time window
#
proc updateSimTimeWin {w args} {
  global simOn

  if {$simOn} {
    pack $w.simtime -side right -padx 3 -pady 3 -ipadx 2 -ipady 2
  } else {
    pack forget $w.simtime
  }
}

proc makeStatus {w} {
  global tkg_displayFile tkg_currentModule tkg_modifiedFlag tkg_statusMessage
  global simOn currentSimTime

  label $w.file  -relief sunken -bg "\#ffbbbb" -width 30 -height 1 -justify left -anchor w
  label $w.block -relief sunken -bg "\#ffbbbb" -width 25 -height 1 -justify left -anchor w
  label $w.msg -relief sunken -bg "\#ffbbbb" -height 1
  label $w.simtime -relief sunken -bg "\#ffbbbb" -height 1 -width 15 -textvariable ::currentSimTime -anchor w

  pack $w.file -side left -padx 3 -pady 3 -ipadx 2  -ipady 2 -anchor w
  pack $w.block -side left -padx 3 -pady 3 -ipadx 2 -ipady 2 -anchor w
  pack $w.msg -side left -padx 3 -pady 3 -ipadx 2 -ipady 2 -fill x -expand 1

  updateStatus

  trace variable tkg_displayFile w updateStatus
  trace variable tkg_currentModule w updateStatus
  trace variable tkg_modifiedFlag w updateStatus
  trace variable tkg_statusMessage w updateStatus
  trace variable simOn w "updateSimTimeWin $w"

  helpon $w.msg [m ho.status.msg]
  helpon $w.block [m ho.status.block]
  helpon $w.file [m ho.status.file]
}

proc tkg_cancel {} {
  global tkg_statusMessage
  set tkg_statusMessage ""
  updateStatus
}
