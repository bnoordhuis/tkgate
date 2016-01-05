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
# Last edit by hansen on Fri Mar 26 13:26:09 2004

#
# Current setting for number of paths
#
set cur_numCPath $tkg_numCPath

#
# Get critical paths.
#
proc tkg_cpathAnal {} {
  proc tkg_altStartup {} { tkg_cpathStart }
  gat_setMajorMode analyze
}

proc tkg_cpathClear {} {
  global cpb_w

  catch {
    $cpb_w.f.path delete 0 end
    $cpb_w.f.delay delete 0 end
  }
  gat_clearPath
}

proc tkg_cpathStart {} {
  global cur_numCPath cpb_count

  set cpb_count 0

  tkg_cpathClear
  Simulator::cmdSend "cpath $cur_numCPath"
}


proc tkg_cpathAdd {n p} {
  global cpb_w cpb_count cpb_paths

  set cpb_paths($cpb_count) $p
  incr cpb_count

  set P {}

  foreach cp $p {
    scan $cp {%[^:]:%[^:]:%[^:]} p1 name p2
    lappend P $name
  }

  tkg_cpathBox
  $cpb_w.f.path insert end $P
  $cpb_w.f.delay insert end $n

} 

proc tkg_cpathEnd {} {
  global cpb_w

#  gat_end*Simulate
  tkg_showPath 0

  catch {
    $cpb_w.f.path selection set 0
    $cpb_w.f.path see 0
  }
}

proc cpathSetView {w args} {
  eval "$w.path yview $args"
  eval "$w.delay yview $args"
}

proc tkg_showPath {idx} {
  global cpb_w cpb_paths

  catch {
    set d 0
    set p {}

    set w $cpb_w.f
    catch { set p $cpb_paths($idx) }
    catch { set d [$w.delay get $idx] }

    if { $p == {} } return

    $cpb_w.sel.g.nets delete 0 end
    foreach n $p {
      catch {
	scan $n {%[^:]:%[^:]:%[^:]} p1 name p2
	$cpb_w.sel.g.nets insert end $name
      }
    }

    $cpb_w.sel.g.delay.v configure -text $d

    eval "gat_showPath $p"
  }
}

proc tkg_showSelectedPath {w} {
  set idx [$w curselection]
  tkg_showPath $idx
}

proc tkg_cpathDestroyed {w} {
  global simId

  if {$w == ".cpbox" } {
    tkg_cpathClear
    gat_setMajorMode edit

#    puts "simId: $simId"
    catch { set analyOn 0 }
    catch { close $simId }
  }
}

#
# Build the path details portion of the critical path analysis box.
#
proc tkg_cpathDetails {w} {
  global cur_numCPath tkg_cpathListLengths

  frame $w

  frame $w.g -bd 2 -relief raised
  frame $w.g.delay -bd 2 -relief sunken
  label $w.g.delay.l -text [m cp.delay]
  label $w.g.delay.v
  pack $w.g.delay.l $w.g.delay.v -padx 5 -pady 3 -side left

  listbox $w.g.nets -width 20 -height 6 -xscrollcommand "$w.g.hb set" -yscrollcommand "$w.g.vb set"
  scrollbar $w.g.vb -orient vertical -command "$w.g.nets yview"
  scrollbar $w.g.hb -orient horizontal -command "$w.g.nets xview"

  bind $w.g.nets <ButtonRelease-1> { catch { gat_gotoCPathNet [%W get [%W curselection]] }}

  grid $w.g.delay		-row 0 -column 0 -columnspan 2 -sticky ew -padx 3 -pady 3
  grid $w.g.nets		-row 1 -column 0 -sticky nsew -padx 3 -pady 3
  grid $w.g.vb			-row 1 -column 1 -sticky ns -padx 3 -pady 3
  grid $w.g.hb			-row 2 -column 0 -sticky ew -padx 3 -pady 3

  frame $w.num -bd 2 -relief raised
  label $w.num.l -text [m cp.numpath]
  menubutton $w.num.v -text $cur_numCPath -indicatoron 1 -relief raised -bd 2 -menu $w.num.v.m -width 3
  menu $w.num.v.m -tearoff 0
  set path_maxes $tkg_cpathListLengths
  foreach n $path_maxes {
    $w.num.v.m add command -label $n -command "$w.num.v configure -text $n; set cur_numCPath $n"
  }

  button $w.num.reco -text [m cp.recompute] -command tkg_cpathStart

  pack $w.num.reco -padx 5 -pady 5 -side bottom -anchor se
  pack $w.num.l  $w.num.v -padx 5 -pady 5 -fill x -side left -anchor nw 

  pack $w.g   -ipadx 5 -ipady 5 -side left -anchor nw
  pack $w.num -ipadx 5 -ipady 5 -side left -anchor nw -fill both -expand 1

}

proc tkg_cpathAddLoop {n} {
  global loopNets

  catch {
    set cpb_w .cpbox
    set w $cpb_w

    toplevel $w
    wm resizable $w 0 0
    wm title $w "TKGate: Critical Paths (EXPERIMENTAL)"
    wm geometry $w [offsetgeometry . 50 50]
    wm transient $w .

    bind $w <Destroy>	{ tkg_cpathDestroyed %W}

    frame $w.msg -bd 2 -relief raised
    label $w.msg.l -text [m cpath.cloops] -justify left
    pack $w.msg.l -padx 5 -pady 5

    frame $w.data -bd 2 -relief raised
    listbox $w.data.lb -yscrollcommand "$w.data.vb set" -xscrollcommand "$w.data.hb set" -width 40 -height 6
    scrollbar $w.data.vb -orient vertical -command "$w.data.lb yview"
    scrollbar $w.data.hb -orient horizontal -command "$w.data.lb xview"
    grid $w.data.lb -row 0 -column 0 -sticky nsew -padx 5 -pady 5
    grid $w.data.vb -row 0 -column 1 -sticky nsew -padx 5 -pady 5
    grid $w.data.hb -row 1 -column 0 -sticky nsew -padx 5 -pady 5

    bind $w.data.lb <ButtonRelease-1> { catch { gat_gotoCPathNet [%W get [%W curselection]] }}

    frame $w.d -bd 2 -relief raised
    button $w.d.d -text [m b.dismiss] -command "destroy $w"
    pack $w.d.d -pady 5
    pack $w.msg $w.data $w.d -fill both

    catch { unset loopNets }
  }

  if { [catch {set loopNets($n) } xxx ] } {
    $w.data.lb insert end $n
    set loopNets($n) 1
  }
}

proc tkg_cpathBox {} {
  global cpb_w

  catch {
    set cpb_w .cpbox
    set w $cpb_w

    toplevel $w
    wm resizable $w 0 0
    wm title $w "TKGate: Critical Paths (EXPERIMENTAL)"
    wm geometry $w [offsetgeometry . 50 50]
    wm transient $w .

    bind $w <Destroy>	{ tkg_cpathDestroyed %W}

    frame $w.f -bd 2 -relief raised 
    label $w.f.path_l -text [m cp.pathl]
    label $w.f.delay_l -text [m cp.delay]
    listbox $w.f.path -yscrollcommand "$w.f.vb set" -xscrollcommand "$w.f.hb set" -width 40 -height 6
    listbox $w.f.delay -yscrollcommand "$w.f.vb set" -width 5 -height 6
    scrollbar $w.f.vb -orient vertical -command "cpathSetView $w.f"
    scrollbar $w.f.hb -orient horizontal -command "$w.f.path xview"

    bind $w.f.path <ButtonRelease-1> "tkg_showSelectedPath $w.f.path"
    bind $w.f.delay <ButtonRelease-1> "tkg_showSelectedPath $w.f.delay"

    grid $w.f.path_l	-row 0 -column 0 -sticky w
    grid $w.f.delay_l	-row 0 -column 1 -sticky w
    grid $w.f.path	-row 1 -column 0 -sticky ns -padx 3 -pady 3
    grid $w.f.delay	-row 1 -column 1 -sticky ns -padx 3 -pady 3
    grid $w.f.vb	-row 1 -column 2 -sticky ns -pady 3
    grid $w.f.hb	-row 2 -column 0 -sticky ew -padx 3

    tkg_cpathDetails $w.sel

    frame $w.d -bd 2 -relief raised
    button $w.d.d -text [m b.dismiss] -command "destroy $w"
    pack $w.d.d -pady 5

    pack $w.f -fill both -ipadx 5 -ipady 5
    pack $w.sel -fill both -anchor w -fill both
    pack $w.d -fill both
    tkg_showSelectedPath $w.f.path
  }
}

proc tkg_postNetDelay {n fd bd} {
  global tkg_netDelayValue

  catch {
    scan [winfo pointerxy .] "%d %d" x y
    toplevel .netdel -bg bisque

    wm geometry .netdel +[expr $x + 5]+[expr $y - 30]
    wm transient .netdel .
    wm overrideredirect .netdel 1

    label .netdel.l -textvariable tkg_netDelayValue -bg bisque -justify left
    pack .netdel.l -padx 4 -pady 4
  }

  catch {
    set tkg_netDelayValue "$fd -> $n -> $bd"
  }
}

proc tkg_unpostNetDelay {} {
  catch { destroy .netdel } 
}

#
# Respond to command from analyzer
#
proc tkg_readAnalyzer {} {
  global simId analyOn

  if { ! $analyOn } return

  if { [catch { set command [gets $simId] } ] } {
    catch { set analyOn 0 }
    catch { close $simId }
    gat_setMajorMode edit
    errmsg "Simulator has died (read error)."
    return
  }
  if { $command == "" && [eof $simId] } {
    catch { set analyOn 0 }
    catch { close $simId }
    gat_setMajorMode edit
    errmsg "Simulator has died (eof in read)."
    return
  }

  gat_analyzerCommand $command
  return 0
}

#
# Start up the simulator in analyzer mode
#
proc tkg_startAnalyzer {fname} {
  global analyOn simId simExec tkg_simDelayFile tkg_simDefaultDelayFile tkg_simCustomDelay tkg_currentFile tkg_simDebugInterface
  global cur_numCPath tkg_numCPath

  set basename ""

  set p [string last "/" $tkg_currentFile]
  if { $p > 0 } {
    set basename [string range $tkg_currentFile 0 $p]
    set basename "-B $basename"
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

  if { $tkg_simDebugInterface } {
    puts "Exec: $simExec -a -D $delayFile $basename $fname"
  }

  set cur_numCPath $tkg_numCPath

  if { [catch { set simId [open "|$simExec -a -D $delayFile $basename $fname" r+ ] } ] } {
    errmsg "Failed to start analyzer '${simExec}'"
    return
  }

  fileevent $simId readable { tkg_readAnalyzer; update idletasks }
  set analyOn 1

  return $simId
}

proc tkg_flashCPath {} {
  gat_cpathFlash
  after 500 tkg_flashCPath
}

