#   Copyright (C) 1987-2005 by Jeffery P. Hansen
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
# Last edit by hansen on Sun Dec  7 20:44:06 2008

namespace eval VPD {
  variable shutdown_cmd
  variable outSignals {}
  variable allowed_procs {}
  variable disllowed_procs {}
  variable loaded {}
  variable insignalCommand
  variable insignalVariable
  variable insignalFormat

  #############################################################################
  #
  # Register a VPD
  #
  proc register {name} {
    variable loaded

    lappend loaded $name
  }

  #############################################################################
  #
  # Register tcl proceedures patterns that are allowed to be executed from verilog.
  #
  proc allow {args} {
    variable allowed_procs

    foreach p $args { lappend allowed_procs $p }
  }

  #############################################################################
  #
  # Register tcl proceedures patterns that are disallowed from being executed
  # from verilog.
  #
  proc disallow {args} {
    variable disllowed_procs

    foreach p $args { lappend disallowed_procs $p }
  }

  #############################################################################
  #
  # Post a VPD
  #
  # Parameters:
  #      name		Type name of the VPD
  #      inst		Instance name of the VPD	
  #
  proc post {name inst} {
    variable loaded

#    puts "VPD::post $name $inst"

    #
    # Only allow post of registered VPDs
    #
    if {[lsearch -exact $loaded $name] < 0} {
      return
    }

#    puts "exec VPD::post $name $inst"

    ${name}::post $inst

#    puts "done VPD::post $name $inst"
  }

  #############################################################################
  #
  # Respond to data received on a named pipe. 
  #
  proc qdata {name value} {
    variable insignalCommand
    variable insignalVariable
    variable insignalFormat

#    puts "VPD::qdata $name $value"

    if {[info exists insignalCommand($name)]} {
      eval "$insignalCommand($name) $value"
    }
    if {[info exists insignalVariable($name)]} {
      uplevel \#0 set $insignalVariable($name) $value
    }
  }

  #############################################################################
  #
  # Check a command to see if it is allowed
  #
  proc isallowed {cmd} {
    variable allowed_procs
    variable disllowed_procs

    #
    # Check for disallowed proceedures
    #
    set ok 1
    foreach p $disllowed_procs {
      if {[string match $p $cmd]} {
	set ok 0
	break
      }
    }
    if {!$ok} { return 0 }

    set ok 0
    foreach p $allowed_procs {
      if {[string match $p $cmd]} {
	set ok 1
	break
      }
    }
    if {!$ok} { return 0 }


    return 1
  }

  #############################################################################
  #
  # Respond to a shutdown of the simulator.
  #
  proc execShutdown {w args} {
    variable shutdown_cmd
    variable outSignals
    global simOn

    if { !$simOn } { 
      trace vdelete ::simOn w "VPD::execShutdown $w"
      eval $shutdown_cmd($w)
      catch { destroy $w }
    }
  }

  #############################################################################
  #
  # Close a channel
  #
  proc closeChannel {chan var args} {
    global simOn
    variable insignalCommand
    variable insignalVariable
 
    if { !$simOn } { 
      trace vdelete $var w "VPD::dataOut $chan $var"
      trace vdelete ::simOn w "VPD::closeChannel $chan $var"

      catch { array unset insignalCommand }
      catch { array unset insignalVariable }
      catch { array unset insignalFormat }
    }
  }

  #############################################################################
  #
  # Respond to a change in data and convey it to the verilog description.
  #
  proc dataOut {chan var args} {
    set data [uplevel 0 { set $var}]
    set n [llength $data]

    if {$n > 1} {
      set data [encodeBits 1 $data]
    }

#    puts "VPD::dataOut $chan $data"
    Simulator::cmdSend "\$write $chan $data"
  }

  #############################################################################
  #
  # Register a variable as an output signal
  #
  proc signal {chan var args} {
    variable outSignals

    trace variable $var w "VPD::dataOut $chan $var"
    trace variable ::simOn w "VPD::closeChannel $chan $var"
  }

  #############################################################################
  #
  # Register a variable as an output signal
  #
  proc outsignal {chan var args} {
    variable outSignals

    trace variable $var w "VPD::dataOut $chan $var"
    trace variable ::simOn w "VPD::closeChannel $chan $var"
  }

  #############################################################################
  #
  # Register a variable as an input signal
  #
  # insignal $name.EMPTY -command "UserVPD::seeEmpty $name" -format decimal
  # insignal $name.EMPTY -variable "UserVPD::empty($name)" -format verilog
  #
  proc insignal {chan args} {
    variable insignalCommand
    variable insignalVariable
    variable insignalFormat

    set command ""
    set variable ""
    set format "%h"
    parseargs $args {-command -variable -format}

    Simulator::cmdSend "\$qwatch $chan -format $format"
    set insignalFormat($chan) $format
    if { $command != "" } { set insignalCommand($chan) $command }
    if { $variable != "" } { set insignalVariable($chan) $variable }
  }

  #############################################################################
  #
  # Create a top-level window for a user device that will automatically be
  # deleted when the simulator is closed
  #
  proc createWindow {title args} {
    variable shutdown_cmd

    set i 0

    set shutdowncommand ""
    parseargs $args {-shutdowncommand}

    while { [catch { set w [toplevel .udev_$i] }] } {
      incr i
    }

    wm resizable $w 0 0
    wm title $w "TkGate: $title"
    wm geometry $w [offsetgeometry . [expr 50 * ( $i + 1 )] [expr 50 * ( $i + 1 )]]

    set shutdown_cmd($w) $shutdowncommand
    trace variable ::simOn w "VPD::execShutdown $w"

    return $w
  }

  proc loadScripts {} {
    global tkg_simVPDPath
    variable loaded

#    set loaded {}
    foreach directory $tkg_simVPDPath {
      set directory [namespace eval :: "eval concat $directory"]
      foreach file [glob -nocomplain $directory/*.tcl] {
#	lappend loaded [file rootname [file tail $file]]
	namespace eval :: "source $file"
      }
    }

    if {[llength $loaded] > 0} {
      global tkg_statusMessage
      InfoPanel::log "\[Loaded VPDs: $loaded\]" -noshow 1
    }
  }
}
