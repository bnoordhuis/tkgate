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
# Last edit by hansen on Sat Sep 26 18:18:20 2009
#

set simId 0
set simOn 0
set currentSimTime 0ns

set tkg_simDisplayedVal 0

namespace eval SimWatcher {
  variable w ""

  proc addText {s} {
    variable w
    if {$w == ""} return

    if {[string range $s 0 2] == "<--" } {
      $w.text insert end "$s\n" red
    } else {
      $w.text insert end "$s\n" blue
    }
    $w.text see end
  }

  proc flushText {} {
    variable w
    if {$w == ""} return

    $w.text delete 1.0 end
  }

  proc destroyNotify {} {
    variable w

    set w ""
  }

  proc post {} {
    variable w

    set w .simwatcher
    if { [catch { toplevel $w }] } {
      flushText
      return
    }

    wm resizable $w 0 0
    wm title $w "TkGate: Simulation Watch"
    wm geometry $w [offsetgeometry . 50 50]

    bind $w <Destroy> SimWatcher::destroyNotify

    button $w.dismiss -text [m b.dismiss] -command "destroy $w"
    text $w.text -yscrollcommand "$w.vb set"
    scrollbar $w.vb -command "$w.text yview"

    pack $w.dismiss -side bottom -fill x
    pack $w.text $w.vb -side left -fill y
    $w.text tag configure blue -foreground blue
    $w.text tag configure red -foreground red
  }
}

#############################################################################
#
# Verifiy that simulator is running
#
proc tkg_simCheck {} {
  global simOn

  if { $simOn == 0 } {
    errmsg [m err.simonly]
    return 0
  }
  return 1
}

########################################
#
# start up the simulator
#
proc tkg_startSim {fname initTime} {
  global simId simOn mode simExec tkg_simDelayFile tkg_simDefaultDelayFile tkg_simCustomDelay 
  global tkg_currentFile tkg_simDebugInterface tkg_warningMode

  set basename ""

  if { $simOn } { return }

  set p [string last "/" $tkg_currentFile]
  if { $p > 0 } {
    set basename [string range $tkg_currentFile 0 $p]
  } else {
    set basename "/"
  }

  if { $tkg_simCustomDelay } {
    set delayFile $tkg_simDelayFile
  } {
    set delayFile $tkg_simDefaultDelayFile
  }

  set delayFile $tkg_simDefaultDelayFile
  foreach f $tkg_simDelayFile {
    set delayFile "${delayFile}:$f"
  }

  #
  # Construct the commmand line to use for starting the simulator.
  #
  set simCmd "$simExec -i $fname -B $basename -D [gat_computestrhash $fname] -W $tkg_warningMode -I $initTime"

  #
  # Start up the simulator
  #
  if { [catch { set simId [open "|$simCmd" r+ ] } ] } {
    errmsg "Failed to start simulator '${simExec}'"
    return
  }

  if { $tkg_simDebugInterface } {
    SimWatcher::post
  }

  fileevent $simId readable { tkg_readSim; update idletasks }
  set simOn 1
  set mode 1

  tkg_resetLogo

  return $simId
}

########################################
#
# read a command from simulator
#
proc tkg_readSim {} {
  global simId simOn tkg_simDebugInterface

  if { !$simOn } { return }

  if { [catch { set command [gets $simId] } ] } {
    gat_setMajorMode edit
    errmsg "Simulator has died (read error)."
    return
  }
  if { $command == "" && [eof $simId] } {
    gat_setMajorMode edit
    errmsg "Simulator has died (eof in read)."
    return
  }

  if { $tkg_simDebugInterface } {
    SimWatcher::addText "--> $command"
  }

  gat_scopeCommand $command
  return 0
}

proc tkg_endSim {} {
  global simId simOn

  catch { fileevent $simId readable }
  catch { close $simId }
  set simOn 0
  gat_breakpoint -clear
  gat_setMajorMode edit
  tkg_editLogo
}

#############################################################################
#
# Simulation script handling
#

proc tkg_doSimScript args {
  global simScript_filter simScript_filetypes

  if { ![tkg_simCheck] } return

  set parent "."
  parseargs $args {-parent}

  set fileName [tk_getOpenFile -defaultextension $simScript_filter -parent $parent -filetypes $simScript_filetypes ]
  if { $fileName != "" } {
    ScriptMgr::load $fileName
  }
}

#############################################################################
#
# Load a memory from a file.
#
proc tkg_simLoadMem args {
  global mem_filetypes mem_filter

  if { ![tkg_simCheck] } return

  set parent "."
  parseargs $args {-parent}

  set g [gat_simSelected ram rom]

  set load [tk_getOpenFile -defaultextension $mem_filter -parent $parent -filetypes $mem_filetypes ]
  if { $load != "" } {
#    tkg_simGateCmd $g load $load
    Simulator::cmdSend "\$memload $load $g"
  }
}

#############################################################################
#
# Dump a memory to a file.
#
proc tkg_simDumpMem args {
  global mem_filetypes mem_filter

  if { ![tkg_simCheck] } return

  set parent "."
  parseargs $args {-parent}

  set g [gat_simSelected ram rom]

  set dump [tk_getSaveFile -defaultextension $mem_filter -parent $parent -filetypes $mem_filetypes ]
  if { $dump != "" } {
    if { $g == "" } {
      Simulator::cmdSend "\$memdump $dump"
    } else {
      Simulator::cmdSend "\$memdump $dump $g"
    }
  }
}

#############################################################################
#
# basic commands
#
proc tkg_simNetSet {net val} {
  if { ![tkg_simCheck] } return

  Simulator::cmdSend "\$set $net $val"
}

#############################################################################
#
# Put simulator in continuous run mode.
#
proc tkg_simRun {} {
  global simOn

  if { $simOn == 0  } {
    gat_setMajorMode simulate
  } {
    gat_breakpoint -clear
    Simulator::cmdSend "\$go"
  }
}

#############################################################################
#
# Stop the simulator (put it in pause mode)
#
proc tkg_simStop {} {
  global simOn

  if { $simOn == 0  } {
  } {
    Simulator::cmdSend "\$stop"
  }
}

#############################################################################
#
# args: +/- # after [clock]
#
proc tkg_simCycle args {
  global tkg_simClockOverStep tkg_simActClock tkg_simUseActClock tkg_simClockStepSize
  global simOn

  if { $simOn == 0  } return

  gat_editCircProps load simProps

  set tkg_simClockStepSize [validate_posint $tkg_simClockStepSize]
  set tkg_simClockOverStep [validate_nonnegint $tkg_simClockOverStep]

  gat_breakpoint -clear

  if { $args == "" } {
    if { $::simProps(clockMode) == 1 } {
      Simulator::cmdSend "\$clock + $tkg_simClockStepSize $tkg_simClockOverStep $::simProps(clockName).Z"
    } else {
      Simulator::cmdSend "\$clock + $tkg_simClockStepSize $tkg_simClockOverStep"
    }
  } {
    Simulator::cmdSend "\$clock $args"
  }
}

#############################################################################
#
# Simulator interface
#
namespace eval Simulator {
  variable dipValue
  variable dipWindow
  variable timeUpdate_ev

  proc doTimeUpdate {} {
    variable timeUpdate_ev

    if { [cmdSend -ignoreerrors "\$time"] } {
      set timeUpdate_ev [after 500 Simulator::doTimeUpdate]
    }
  }

  proc startTimeUpdates {} {
    variable timeUpdate_ev
    catch {
      catch { after cancel $timeUpdate_ev }
      set timeUpdate_ev [after 500 Simulator::doTimeUpdate]
    }
  }

  proc stopTimeUpdates {} {
    variable timeUpdate_ev
    catch { after cancel $timeUpdate_ev }
  }

  #############################################################################
  #
  # Step simulator by a single epoch.
  #
  proc stepEpoch args {
    global tkg_simStepSize
    global simOn

    if { $simOn == 0  } return

    gat_breakpoint -clear

    set tkg_simStepSize [validate_posint $tkg_simStepSize]

    if { $args == "" } {
      Simulator::cmdSend "\$step $tkg_simStepSize"
    } {
      Simulator::cmdSend "\$step $args"
    }
  }

  proc showValue {n} {
    global tkg_simDisplayedVal tkg_showValueEv tkg_valuePopUpDelay

    set tkg_simDisplayedVal "-"

    Simulator::cmdSend "\$show $n"

    scan [winfo pointerxy .] "%d %d" x y

    set tkg_showValueEv  [after $tkg_valuePopUpDelay "Simulator::postShowValue $x $y"]
  }

  proc postShowValue {x y} {
    global tkg_simDisplayedVal

    catch { destroy .showv }
    toplevel .showv -bg bisque

    wm geometry .showv +[expr $x + 5]+[expr $y - 30]
    wm transient .showv .
    wm overrideredirect .showv 1

    label .showv.l -textvariable tkg_simDisplayedVal -bg bisque
    pack .showv.l -padx 4 -pady 4
  }
  proc hideValue {} {
    global tkg_showValueEv

    catch { after cancel $tkg_showValueEv }
    catch { destroy .showv }
  }

  #
  # Apply value in dip value selector
  #
  proc dipApply {w g} { 
    variable dipValue
    set q [ gat_setDip $g $dipValue($g) ]
    set dipValue($g) $q
    dipSetIndicator $w 0
  }

  #
  # Destroy a DIP entry window
  #
  proc dipDestroy {w g} {
    variable dipWindow

    set dipWindow($g) ""
    catch { trace vdelete Simulator::dipValue($g) w "Simulator::dipSetIndicator $w 1" }
    catch { destroy $w }
  }

  #
  # Set the "changed" indicator
  #
  proc dipSetIndicator {w changed args} {
    if { $changed } {
      set color red
    } else {
      set color green
    }
    $w.top.indicator itemconfigure indicator -fill $color -outline $color
  }

  #
  # Create a window for entering a dip value.
  #
  proc dipEntry {g v x y} {
    variable dipValue
    variable dipWindow

    #
    # If DIP window is already open, just raise it. 
    #
    if { [catch { set w $dipWindow($g) } ] } { set w "" }
    if { $w != "" } { 
      raise $w
      return
    }

    #
    # Choose a top-level window for the dip setter
    #
    for {set i 0} { [catch { set w [toplevel .dipe$i] }] } { incr i } { }

    set dipWindow($g) $w

    wm resizable $w 0 0
    wm title $w [m db.dip.title]
    wm geometry $w [offsetgeometry . $x $y]
    wm transient $w .

    set dipValue($g) $v

    frame $w.top
    canvas $w.top.indicator -width 12 -height 12 -bd 0 -bg [$w cget -bg] -highlightthickness 0
    $w.top.indicator create oval 2 2 10 10 -fill green -outline green -tags indicator

    label $w.top.label -text "$g:"
    incdecEntry $w.entr -width 10 -font dipFont -variable Simulator::dipValue($g) \
	-format %x -validatecommand hexValidate

    button $w.apply -text [m b.apply] -command "Simulator::dipApply $w $g"
    button $w.close -text [m b.close] -command "Simulator::dipDestroy $w $g"

    bindtags $w.entr [concat [list HexEntry] [bindtags $w.entr]]


    pack $w.top.indicator -side left
    pack $w.top.label -side left -fill both -padx 2
    pack $w.top -side top -fill both
    pack $w.close -side bottom -fill both -pady 3
    pack $w.entr $w.apply -side left -pady 5 -padx 5 

    trace variable Simulator::dipValue($g) w "Simulator::dipSetIndicator $w 1"
    bind $w <Destroy> "Simulator::dipDestroy $w $g"
    bind $w.entr.e <Return> "Simulator::dipApply $w $g"
    bind .scope <Destroy> "+ Simulator::dipDestroy $w $g"
  }

  #tkg_simWrite
  proc cmdSend {args} {
    global simId tkg_simDebugInterface

    set ignoreerrors 0

    if {[llength $args] > 1} {
      if {[lsearch $args -ignoreerrors] >= 0} {
	set ignoreerrors 1
      }
      set msg [lindex $args [expr [llength $args]-1]]
    } else {
      set msg [lindex $args 0]
    }

    if { $tkg_simDebugInterface } {
      SimWatcher::addText "<-- $msg"
    }

    if { [catch { puts $simId $msg }] } {
      gat_setMajorMode edit
      if {!$ignoreerrors} {
	errmsg "Simulator has died (in write)."
      }
      return 0
    }
    cmdFlush
    return 1
  }

  #tkg_simFlush
  proc cmdFlush {} {
    global simId
    if { [catch { flush $simId } ] } {
      gat_setMajorMode edit
      errmsg "Simulator has died (in flush)."
      return
    }
  }

  #############################################################################
  #
  # Inform simulator of security settings
  #
  proc setupSecurity {} {
    global tkg_securityHandling tkg_securityExec tkg_securityWriteMem tkg_securityOpen
    global tkg_securitySend tkg_securityEnqueue

    Simulator::cmdSend "\$option sec.send $tkg_securitySend"
    Simulator::cmdSend "\$option sec.fopen $tkg_securityOpen"
    Simulator::cmdSend "\$option sec.writemem $tkg_securityWriteMem"
    Simulator::cmdSend "\$option sec.queue $tkg_securityEnqueue"
    Simulator::cmdSend "\$option sec.exec $tkg_securityExec"
    Simulator::cmdSend "\$option sec.handling $tkg_securityHandling"
  }
}
