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
# Last edit by hansen on Thu Jan 29 22:49:36 2009
#

namespace eval Scope {
  proc makeToolbar {w} {
    global bd tkg_showXHair tbr

    frame $w

    frame $w.f
    toolbutton $w.f.print	"file_print.gif"	ScopePrintDlg::post			ho.traceprint
    pack $w.f.print -side left -pady 5

    frame $w.z
    toolbutton $w.z.zoomin	"zoom_in.gif"		{ .scope.main.frame.canvas zoom -1 }	ho.tool.zoomin
    toolbutton $w.z.zoomout	"zoom_out.gif"		{ .scope.main.frame.canvas zoom 1 }	ho.tool.zoomout
    pack $w.z.zoomin $w.z.zoomout  -side left -pady 5

    frame $w.s
    toolbutton $w.s.simgo	"sim_go.gif"		tkg_simRun				ho.simgo
    toolbutton $w.s.simpause	"sim_pause.gif"		tkg_simStop				ho.simpause
    toolbutton $w.s.simstep	"sim_step.gif"		Simulator::stepEpoch			ho.simstep
    toolbutton $w.s.simclock	"sim_clock.gif"		tkg_simCycle				ho.simclock
    toolbutton $w.s.simstop	"sim_stop.gif"		{ gat_setMajorMode edit }		ho.simstop
    pack $w.s.simgo $w.s.simpause $w.s.simstep $w.s.simclock $w.s.simstop -side left -pady 5

    frame $w.scope
    toolbutton $w.scope.simbreak	"sim_break.gif"	 Action::editBreakpoints		ho.simbreak
    toolbutton $w.scope.simscript	"sim_script.gif" Action::doSimScript			ho.simexec
    toolbutton $w.scope.simload		"sim_load.gif"	 Action::simLoadMem			ho.simload
    toolbutton $w.scope.simdump		"sim_dump.gif"	 Action::simDumpMem			ho.simdump
    pack $w.scope.simbreak $w.scope.simscript $w.scope.simload $w.scope.simdump -side left -pady 5
    pack $w.scope.simload $w.scope.simdump -side left -pady 5

    frame $w.extra
    set tkg_showXHair 1
    checkbutton $w.extra.showxhair -indicatoron 0 -image [gifI "show_xhair.gif"] -variable tkg_showXHair -takefocus 0 -command { gat_setShowXHairState $tkg_showXHair } -selectcolor "\#b9b9b9" -relief flat

    pack $w.extra.showxhair -side left -pady 5

    pack $w.f $w.z $w.s $w.scope $w.extra -side left -padx 5
    #  pack $w.f $w.z $w.s $w.sc  -side left -padx 5

    helpon $w.extra.showxhair [m ho.showxhair]
  }

  proc makeHidden {} { wm withdraw .scope }
  proc makeVisible {} { wm deiconify .scope }
#  proc makeHidden {} {  }
#  proc makeVisible {} { }

  proc post {} {
    toplevel .scope -takefocus 0

    wm geometry .scope +70+0
    wm minsize .scope 500 300
    wm title .scope [m scope.title]
    makeHidden

    #  tkg_scopeMenu .scope.mbar
    Menu::makeBar .scope.mbar scope
    frame .scope.bar1 -bd 1 -relief sunken -height 2
    makeToolbar .scope.tbar

    frame .scope.main
    frame .scope.main.frame -relief sunken -bd 2


    bind .scope <Destroy> { gat_setMajorMode edit }
    bind .scope <Leave> Scope::hideTraceName

    gat_scope .scope.main.frame.canvas -width 600 -height 400 -bg white
    scrollbar .scope.main.vert -command ".scope.main.frame.canvas yview" -takefocus 0
    scrollbar .scope.main.horz -orient horizontal  -command ".scope.main.frame.canvas xview" -takefocus 0

    #  pack .scope.main.vert -side right -fill y
    #  pack .scope.main.horz -side bottom -fill x
    #  pack .scope.main.frame -side top -fill both -expand 1 -padx 5 -pady 5
    pack .scope.main.frame.canvas -fill both -expand 1

    grid .scope.main.frame -row 0 -column 0 -sticky nsew
    grid .scope.main.horz -row 1 -column 0 -sticky ew
    grid .scope.main.vert -row 0 -column 1 -sticky ns
    grid rowconfigure .scope.main 0 -weight 1
    grid columnconfigure .scope.main 0 -weight 1

    pack .scope.mbar -fill x
    pack .scope.bar1 -fill x -padx 3 -pady 1
    pack .scope.tbar -fill x
    pack .scope.main -fill both -expand 1 -padx 5 -pady 5

    KeyBinding::listener .scope.mbar main -unmatchedcommand dobell
    focus .scope.mbar

    bind .scope.main.frame.canvas <Button-1> 	{ gat_scopeButton %x %y %X %Y 1 %s }
    bind .scope.main.frame.canvas <Button-3> 	{ gat_scopeButton %x %y %X %Y 3 %s }
    bind .scope.main.frame.canvas <Motion> 	{ gat_scopeMotion %x %y %X %Y 0 %s }
    bind .scope.main.frame.canvas <ButtonRelease-1> { gat_scopeRelease %x %y %X %Y 1 %s }
    bind .scope.main.frame.canvas <ButtonRelease-3> { gat_scopeRelease %x %y %X %Y 3 %s }
  }

  proc unpost {} {
    catch { destroy .scope }
  }

  variable pvEvent ""

  #
  # Post the full name of a net near the mouse pointer
  #
  proc showTraceName {n} {
    variable pvEvent

    catch { after cancel $pvEvent }
    scan [winfo pointerxy .] "%d %d" x y
    set pvEvent [after 50 Scope::postName $x $y $n]
  }

  proc hideTraceName {} {
    catch { after cancel $pvEvent }
    catch { destroy .stName }
  }

  proc postName {x y name} {
    catch { destroy .stName }
    toplevel .stName -bg bisque

    wm geometry .stName +[expr $x + 5]+[expr $y - 30]
    wm transient .stName .
    wm overrideredirect .stName 1
    bind .stName <Enter> { destroy .stName }

    label .stName.l -text $name -bg bisque
    pack .stName.l -padx 4 -pady 4
  }
}

proc scope_test {} {
  button .make -text Scope -command makeScope
  button .print -text Print -command printDlg
  pack .make .print -fill x
  wm geometry . +0+0
}
