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
# Last edit by hansen on Mon Feb 23 19:27:02 2009
#
#

# Toolbar y padding
set tbypad 0

set logo_w ""
set tbfile_w ""
set tech_w ""
set undo_w ""
set gateop_w ""
set simtools_w ""

namespace eval ToolBar {
  variable currentMode
  variable newMode
  variable enabledTools
  variable grabX 3
  variable grabY 12
  variable tbMiddle 30
  variable modeTools
  variable mbar_w

  #
  # These variable show the natural position of the tools.  One or more
  # may be hidden and thus not be actually be displayed, but when the tool
  # is unhidden it will be displayed according to the order shown here.
  #
  variable topTools {File Undo Edit SymEdit GateOp Zoom SymZoom BlockOp}
  variable bottomTools {Modes SimModes SymModes SymPort SymShift SymOpr LimModes SimControl SimAux GateProp Tech}
  variable windowTools {}

  array set modeTools {
    None {}
    All {File Undo Edit GateOp Zoom SymZoom Modes SymModes SimModes SymEdit SymPort SymShift SymOpr \
	     LimModes BlockOp GateProp Tech SimControl SimAux}
    Edit {File Undo Edit GateOp Zoom Modes BlockOp GateProp Tech}
    HdlEdit {File Undo Edit BlockOp}
    EditInterfaces {File Undo Edit Zoom LimModes BlockOp}
    Simulate {File Zoom SimModes SimControl SimAux}
    EditInterfaces.sym {File Undo SymEdit SymZoom SymModes SymPort SymShift SymOpr BlockOp}
  }


  #
  # This shows the actual state of a tool.  It is either the window the tool is in,
  # or "hidden" if the tool is not visible.
  #
  variable toolState
  variable toolWindow
  variable toolPos

  proc tbGrabber {w tool argv} {
    frame $w

    set dobind 1
    parseargs $argv {-dobind}

    frame $w.p1 -width 3
    frame $w.v1 -bd 1 -relief raised -width 2 -height 13
    frame $w.v2 -bd 1 -relief raised -width 2 -height 13
    frame $w.p2 -width 5

    pack $w.p1 -side left
    pack $w.v1 -side left -padx 0
    pack $w.v2 -side left -padx 0
    pack $w.p2 -side left

    if { $dobind } {
      foreach W [list $w $w.p1 $w.v1 $w.v2 $w.p2] {
	bind $W <B1-Motion> "ToolBar::setGrab $tool"
      }
    }
  }

  proc resetTechList {} {
    global gatesetech_w tech_w

    set L [gat_getTechList]

    Dropbox::flush $tech_w
    Dropbox::itemadd $tech_w $L

    #
    # Build menus for plain entry technology lists
    #
    set menu_list [list $gatesetech_w]
    foreach m $menu_list {
      catch {
	destroy $m
	menu $m  -tearoff 0 
	foreach mi $L {
	  $m add command -label $mi -command "gat_setTech $mi"
	}
      }
    }

    #
    # Build menus for radio button technology lists
    #
    set menu_list [list .pop_MIgate.settech .pop_gate.settech .pop_multi.settech]
    foreach m $menu_list {
      catch {
	destroy $m
	menu $m  -tearoff 0 
	foreach mi $L {
	  $m add radiobutton -variable pop_tech -value $mi -label $mi -command "gat_setTech $mi"
	}
      }
    }
  }

  proc tbSymModes {w args} {
    global bd tbr tbypad

    frame $w
    tbGrabber $w.grab SymModes $args
    pack $w.grab -side left -fill both

    foreach e {point line rect fillrect port select} {
      flatradiobutton $w.$e -image [gifI "symed_${e}.gif"]  -variable SymbolEdit::emode -value $e
      pack $w.$e -side left -pady $tbypad
      helpon $w.$e [m ho.symed.$e]
    }


    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbSymEdit {w args} {
    global bd tbr tbypad 

    frame $w
    tbGrabber $w.grab SymEdit $args
    pack $w.grab -side left -fill both

    toolbutton $w.cut		"edit_cut.gif"		SymbolEdit::cutRegion		""
    toolbutton $w.copy		"edit_copy.gif"		SymbolEdit::copyRegion		""
    toolbutton $w.paste 	"edit_paste.gif"	SymbolEdit::pasteRegion		""
    toolbutton $w.overlay	"edit_overlay.gif"	SymbolEdit::overlayRegion	""

    pack $w.cut $w.copy $w.paste $w.overlay -side left -pady $tbypad

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbSymPort {w args} {
    global bd tbr tbypad 

    frame $w
    tbGrabber $w.grab SymPort $args
    pack $w.grab -side left -fill both

    toolbutton $w.clk		"symed_pcrot.gif"	Action::seCWRotPort		""
    toolbutton $w.cntr		"symed_pccrot.gif"	Action::seCCWRotPort		""
    pack $w.clk $w.cntr -side left -pady $tbypad

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbSymShift {w args} {
    global bd tbr tbypad 

    frame $w
    tbGrabber $w.grab SymShift $args
    pack $w.grab -side left -fill both

    toolbutton $w.rshift	"arrow0.gif"		{ SymbolEdit::shiftBits 1 0 }	ho.symed.rshift
    toolbutton $w.lshift	"arrow180.gif"		{ SymbolEdit::shiftBits -1 0 }	ho.symed.lshift
    toolbutton $w.ushift	"arrow90.gif"		{ SymbolEdit::shiftBits 0 -1 }	ho.symed.ushift
    toolbutton $w.dshift	"arrow270.gif"		{ SymbolEdit::shiftBits 0 1  }	ho.symed.dshift
    toolbutton $w.cwrotate	"symed_cwrotate.gif"	{ SymbolEdit::rotateBits 1  }	ho.symed.cwrotate
    toolbutton $w.ccwrotate	"symed_ccwrotate.gif"	{ SymbolEdit::rotateBits -1  } 	ho.symed.ccwrotate

    pack $w.rshift $w.lshift $w.ushift $w.dshift $w.cwrotate $w.ccwrotate -side left -pady $tbypad

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbSymOpr {w args} {
    global bd tbr tbypad 

    frame $w
    tbGrabber $w.grab SymOpr $args
    pack $w.grab -side left -fill both

    toolbutton $w.import	"symed_import.gif"	{ Action::importImage }		ho.symed.import
    toolbutton $w.export	"symed_export.gif"	{ Action::exportImage }		ho.symed.export
    toolbutton $w.autobold	"symed_autobold.gif"	{ SymbolEdit::boldBits }	ho.symed.autobold
#    toolbutton $w.resize	"symed_resize.gif"	{}				ho.symed.resize

    pack $w.import $w.export $w.autobold -side left -pady $tbypad

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbSymZoom {w args} {
    global bd tbr tbypad 

    frame $w
    tbGrabber $w.grab SymEdit $args
    pack $w.grab -side left -fill both

    toolbutton $w.in		"zoom_in.gif"		{ SymbolEdit::zoom 1 }		""
    toolbutton $w.out 		"zoom_out.gif"		{ SymbolEdit::zoom -1 }		""
    pack $w.in $w.out -side left  -pady $tbypad

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbFile {w args} {
    global bd tbr tbypad tbfile_w

    set tbfile_w $w
    frame $w
    tbGrabber $w.grab File $args
    pack $w.grab -side left -fill both

    toolbutton $w.new		"file_new.gif"		{clearAction File::new}		ho.new
    toolbutton $w.open		"file_open.gif"		{clearAction File::load}	ho.open
    toolbutton $w.lib		"file_lib.gif"		Action::loadLibrary		ho.library
    toolbutton $w.save		"file_save.gif"		Action::saveFile		ho.save
    toolbutton $w.saveas	"file_saveas.gif"	Action::saveAsFile		ho.saveas
    toolbutton $w.print		"file_print.gif"	{suspendAction PrintDlg::post}	ho.print
    frame $w.vbar -bd 1 -relief sunken -width 2

    pack $w.new $w.open $w.lib $w.save $w.saveas $w.print -side left -pady $tbypad
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbEdit {w args} {
    global bd tbr tbypad

    frame $w
    tbGrabber $w.grab Edit $args
    pack $w.grab -side left -fill y

    toolbutton $w.cut	"edit_cut.gif"		Action::cutToBuf	ho.tool.cut
    toolbutton $w.copy	"edit_copy.gif"		Action::copyToBuf	ho.tool.copy
    toolbutton $w.paste "edit_paste.gif"	Action::yankFromBuf	ho.tool.paste
    toolbutton $w.find  "edit_find.gif"		Action::findObject	ho.tool.find

    frame $w.vbar -bd 1 -relief sunken -width 2

    pack $w.cut $w.copy $w.paste $w.find -side left -pady $tbypad
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbGateOp {w args} {
    global bd tbr tbypad gateop_w


    set gateop_w $w
    frame $w
    tbGrabber $w.grab GateOp $args
    pack $w.grab -side left -fill y

    toolbutton $w.curRot	"rotation0.gif"		ToolBar::advanceRotation	ho.tool.currot
    updateRotation

    toolbutton $w.valgn		"edit_valgn.gif"	Action::vAlign			ho.tool.valgn
    toolbutton $w.halgn		"edit_halgn.gif"	Action::hAlign			ho.tool.halgn
    toolbutton $w.rotate	"edit_rotate.gif"	Action::rotate			ho.tool.rotate
    toolbutton $w.brotate	"edit_brotate.gif"	Action::backRotate		ho.tool.brotate

    frame $w.vbar -bd 1 -relief sunken -width 2

    pack $w.curRot $w.rotate $w.brotate $w.valgn $w.halgn -side left -pady $tbypad
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbGateProp {w args} {
    global bd tbr tbypad gateprop_w

    set gateprop_w $w
    frame $w
    tbGrabber $w.grab GateProp $args
    pack $w.grab -side left -fill y

    toolbutton $w.addport	"addport.gif"		Action::addPort		ho.tool.addport
    toolbutton $w.anchor	"anchor.gif"		Action::anchor		ho.tool.anchor
    toolbutton $w.unanchor	"unanchor.gif"		Action::unAnchor	ho.tool.unanchor
    flatradiobutton $w.replicate -image [gifI "replicate.gif"] -variable tkg_replicateOn -value 1 -command Action::replicate
    toolbutton $w.delete	"delgate.gif"		Action::delete		ho.tool.delete

    pack $w.addport $w.anchor $w.unanchor $w.replicate $w.delete -anchor w -side left

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y

    helpon $w.replicate		[m ho.tool.replicate]
  }


  proc updateRotation args {
    global gateop_w rot

    catch {
      switch $rot {
	0 { $gateop_w.curRot configure -image [gifI rotation0.gif] }
	1 { $gateop_w.curRot configure -image [gifI rotation90.gif] }
	2 { $gateop_w.curRot configure -image [gifI rotation180.gif] }
	3 { $gateop_w.curRot configure -image [gifI rotation270.gif] }
      }
    }
  }

  proc advanceRotation args {
    global gateop_w rot
    switch $rot {
      0 { Action::rot90 }
      1 { Action::rot180 }
      2 { Action::rot270 }
      3 { Action::rot0 }
    }
  }

  proc tbSimModes {w args} {
    global bd tbr tbypad
    global tkg_defaultWType tkg_wtypeList

    frame $w
    tbGrabber $w.grab Modes $args
    pack $w.grab -side left -fill y

    #
    # Flat relief icon radio button are not supported until tck/tk 8.4, so use our own
    #
    flatradiobutton $w.mov -image [gifI "mov_curs.gif"]  -variable mode -value 1 -command Action::editMode
    flatradiobutton $w.scroll -image [gifI "scroll_curs.gif"]  -variable mode -value 3 -command Action::scrollMode

    pack $w.mov $w.scroll  -side left -pady  $tbypad 

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y

    helpon $w.mov [m ho.simmove]
    helpon $w.scroll [m ho.scroll]
 }

  proc tbModes {w args} {
    global bd tbr tbypad
    global tkg_defaultWType tkg_wtypeList

    frame $w
    tbGrabber $w.grab Modes $args
    pack $w.grab -side left -fill y

    #
    # Flat relief icon radio button are not supported until tck/tk 8.4, so use our own
    #
    flatradiobutton $w.mov -image [gifI "mov_curs.gif"]  -variable mode -value 1 -command Action::editMode
    flatradiobutton $w.cut -image [gifI "cut_curs.gif"]  -variable mode -value 0 -command Action::cutMode
    flatradiobutton $w.inv -image [gifI "inv_curs.gif"]  -variable mode -value 2 -command Action::invertMode
    flatradiobutton $w.scroll -image [gifI "scroll_curs.gif"]  -variable mode -value 3 -command Action::scrollMode
    flatradiobutton $w.siz -image [gifI "size_curs.gif"] -variable mode -value 9 -command Action::sizeMode

#    pack $w.mov $w.del $w.cut $w.inv $w.siz -side left -pady $tbypad -ipadx 2 -ipady 2
    pack $w.mov $w.cut $w.inv $w.scroll $w.siz -side left -pady $tbypad

    label $w.nbitsl -text "[m ibits] "
    bitsizeselector $w.nbitsv -variable tkg_bitWidth -width 3 -takefocus 0
    pack $w.nbitsl $w.nbitsv -side left -pady $tbypad

    label $w.typel -text "[m itype] "
    Dropbox::new $w.typev -variable tkg_defaultWType -value $tkg_defaultWType -width 7 -takefocus 0
    pack $w.typel $w.typev -side left -pady $tbypad
    Dropbox::itemadd $w.typev $tkg_wtypeList


    frame $w.pad
    pack $w.pad -side left -padx 2 -pady 2 

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y

    helpon $w.mov [m ho.move]
#    helpon $w.del [m ho.delgat]
    helpon $w.cut [m ho.cutw]
    helpon $w.inv [m ho.inv]
    helpon $w.siz [m ho.bitw]
    helpon $w.scroll [m ho.scroll]

    helpon $w.nbitsl [m ho.cmd.bitw]
    helpon $w.nbitsv [m ho.cmd.bitw]
  }

  proc tbLimModes {w args} {
    global bd tbr tbypad

    frame $w
    tbGrabber $w.grab Modes $args
    pack $w.grab -side left -fill y

    #
    # Flat relief icon radio button are not supported until tck/tk 8.4, so use our own
    #
    flatradiobutton $w.mov -image [gifI "mov_curs.gif"]  -variable mode -value 1 -command Action::editMode
    flatradiobutton $w.cut -image [gifI "cut_curs.gif"]  -variable mode -value 0 -command Action::cutMode
    flatradiobutton $w.siz -image [gifI "size_curs.gif"] -variable mode -value 9 -command Action::sizeMode

    pack $w.mov $w.cut $w.siz -side left -pady $tbypad

    label $w.nbitsl -text "[m ibits] "
    bitsizeselector $w.nbitsv -variable tkg_bitWidth -width 3 -takefocus 0
    pack $w.nbitsl $w.nbitsv -side left -pady $tbypad

    frame $w.pad
    pack $w.pad -side left -padx 2 -pady 2 

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y

    helpon $w.mov [m ho.move]
    helpon $w.cut [m ho.cutw]
    helpon $w.siz [m ho.bitw]

    helpon $w.nbitsl [m ho.cmd.bitw]
    helpon $w.nbitsv [m ho.cmd.bitw]
  }

  proc tbUndo {w args} {
    global bd tbr tbypad tbfile_w undo_w

    set undo_w $w
    frame $w
    tbGrabber $w.grab Undo $args
    pack $w.grab -side left -fill y

    label $w.back -image [gifI "back.gif"] -bd 1 -relief flat -takefocus 0
    label $w.forward -image [gifI "forward.gif"] -bd 1 -relief flat -takefocus 0
#    button $w.back -image [gifI "back.gif"] -bd 1 -relief flat -takefocus 0
#    button $w.forward -image [gifI "forward.gif"] -bd 1 -relief flat -takefocus 0

    pack $w.back $w.forward  -side left -pady  $tbypad -ipadx 1 -ipady 1

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y

    bind $w.back <Button-1> { %W configure  -relief sunken ;Undo::undoSelect; %W configure  -relief flat }
    bind $w.forward <Button-1>  { %W configure  -relief sunken ;Undo::redoSelect; %W configure  -relief flat }

    bind $w.back <Enter> "%W configure -relief raised"
    bind $w.back <Leave> "%W configure -relief flat"
    bind $w.forward <Enter> "%W configure -relief raised"
    bind $w.forward <Leave> "%W configure -relief flat"

    helpon $w.back [m ho.undo]
    helpon $w.forward [m ho.redo]
    
    set activebackground [option get . activeBackground UndoRedo]
    set normalbackground [$w.back cget -bg]

    bind $w.back <Enter> "+ %W configure -bg $activebackground"
    bind $w.back <Leave> "+ %W configure -bg $normalbackground"
    bind $w.forward <Enter> "+ %W configure -bg $activebackground"
    bind $w.forward <Leave> "+ %W configure -bg $normalbackground"
  }

  proc tbBlockOp {w args} {
    global bd tbr tbypad

    frame $w
    tbGrabber $w.grab BlockOp $args
    pack $w.grab -side left -fill y

    toolbutton $w.new		"blk_new.gif"		Action::blockNew	ho.mod.new
    toolbutton $w.del		"blk_delete.gif"	Action::blockDelete	ho.mod.del
    toolbutton $w.copy		"blk_copy.gif"		Action::blockCopy 	ho.mod.copy
    toolbutton $w.rename	"blk_rename.gif"	Action::blockRename	ho.mod.rename
    toolbutton $w.claim		"blk_claim.gif"		Action::blockClaim	ho.mod.claim
#    toolbutton $w.root		"blk_root.gif"		Action::blockSetRoot	ho.mod.setroot
    toolbutton $w.open		"blk_open.gif"		Action::openMod		ho.mod.open
    toolbutton $w.close		"blk_close.gif"		Action::closeMod	ho.mod.close

    pack $w.new $w.del $w.copy $w.rename $w.claim $w.open $w.close  -side left -pady  $tbypad

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbZoom {w args} {
    global bd tbr tbypad

    frame $w
    tbGrabber $w.grab Zoom $args
    pack $w.grab -side left -fill y

    toolbutton $w.zoomin	"zoom_in.gif"		{ action ZoomIn { gat_zoom 1 } }	ho.tool.zoomin
    toolbutton $w.zoomout	"zoom_out.gif"		{ action ZoomOut { gat_zoom -1 } }	ho.tool.zoomout -state disabled
    pack $w.zoomin $w.zoomout  -side left -pady  $tbypad


    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbSimControl {w args} {
    global bd tbr tbypad simtools_w

    set simtools_w $w

    frame $w
    tbGrabber $w.grab SimControl $args
    pack $w.grab -side left -fill y

    toolbutton $w.simgo		"sim_go.gif"		tkg_simRun				ho.simgo
    toolbutton $w.simpause	"sim_pause.gif"		tkg_simStop				ho.simpause
    toolbutton $w.simstep	"sim_step.gif"		Simulator::stepEpoch			ho.simstep
    toolbutton $w.simclock	"sim_clock.gif"		tkg_simCycle				ho.simclock
    toolbutton $w.simstop	"sim_stop.gif"		{ gat_setMajorMode edit }		ho.simstop

    pack $w.simgo $w.simpause $w.simstep $w.simclock $w.simstop -side left -pady  $tbypad

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbSimAux {w args} {
    global bd tbr tbypad

    frame $w
    tbGrabber $w.grab SimAux $args
    pack $w.grab -side left -fill y

    toolbutton $w.simbreak	"sim_break.gif"		Action::editBreakpoints			ho.simbreak
    toolbutton $w.simscript	"sim_script.gif"	Action::doSimScript			ho.simexec
    toolbutton $w.simload	"sim_load.gif"		Action::simLoadMem			ho.simload
    toolbutton $w.simdump	"sim_dump.gif"		Action::simDumpMem			ho.simdump
    toolbutton $w.simview	"sim_view.gif"		Action::simViewMem			ho.simview

    pack $w.simbreak $w.simscript $w.simload $w.simdump $w.simview -side left -pady  $tbypad
#    pack $w.simload $w.simdump -side left -pady  $tbypad

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y
  }

  proc tbLogo {w} {
    global bd tbr tbypad logo_w

    set logo_w $w
    label $logo_w -image [gifI "gatelogo.gif"] 
    helpon $logo_w [m ho.status.logo]

  }

  proc tbTech {w args} {
    global tkg_defaultTech tech_w tbr bd

    set tech_w $w.v
    frame $w
    tbGrabber $w.grab Tech $args
    pack $w.grab -side left -fill y

    label $w.l -text "[m b.tech]:" 
    Dropbox::new $w.v -variable tkg_defaultTech -value $tkg_defaultTech -width 14 -takefocus 0
    Dropbox::itemadd $w.v [gat_getTechList]

    pack $w.l $w.v -anchor w -side left

    frame $w.vbar -bd 1 -relief sunken -width 2
    pack $w.vbar -side left -padx 1 -pady 2 -fill y

    helpon $w.l [m ho.deftech]
  }

  #############################################################################
  #
  # Methods from here are used for controlling toobars
  #
  #############################################################################

  proc clearLocation {t} {
    variable windowTools
    variable topTools
    variable bottomTools

    set i [lsearch $topTools $t]
    if {$i >= 0 } { set topTools [lreplace $topTools $i $i] }
    set i [lsearch $bottomTools $t]
    if {$i >= 0 } { set bottomTools [lreplace $bottomTools $i $i] }
    set i [lsearch $windowTools $t]
    if {$i >= 0 } { set windowTools [lreplace $windowTools $i $i] }
  }

  proc seeToolDrop {t w x y} {
    variable toolWindow
    variable windowTools
    variable topTools
    variable bottomTools
    variable toolPos
    variable grabX
    variable grabY
    variable tbMiddle
    variable toolState

    #
    # If we drop the tool on the toolbar, then go figure out where to insert it,
    # otherwise make a new window for it.
    #
    if { [string range $w 0 4] == ".tbar" } {
      #
      # If the drop is on .tbar itself, use the y position to decide top or bottom
      # and add it to the end.  Otherwise figure out which window we dropped on
      # and where to insert it.
      #
      scan $w ".tbar.%\[^.\]" where
      set tbroot_x [winfo rootx .tbar.top]
      set tbroot_y [winfo rooty .tbar.top]

      if { [expr $y - $tbroot_y] > $tbMiddle } {
	set i 0
	foreach q $bottomTools {
	  if {!$toolState($q) } { 
	    incr i
	    continue
	  }
	  set mw .tbar.bottom.w$q
	  set mx [winfo rootx $mw]
	  set mwidth [winfo width $mw]
	  if { $x < [expr $mx + $mwidth/2] } {
	    break
	  }
	  incr i
	}
	set bottomTools [linsert $bottomTools $i $t]
      } else {
	set i 0
	foreach q $topTools {
	  if {!$toolState($q) } { 
	    incr i
	    continue
	  }
	  set mw .tbar.top.w$q
	  set mx [winfo rootx $mw]
	  set mwidth [winfo width $mw]
	  if { $x < [expr $mx + $mwidth/2] } {
	    break
	  }
	  incr i
	}
	set topTools [linsert $topTools $i $t]
      }
      show $t
    } else {
      clearLocation $t
      lappend windowTools $t
      set toolPos($t) +[expr $x - $grabX]+[expr $y - $grabY]
      show $t
    }
  }

  proc setGrab {t} {
    variable toolWindow
    variable toolState
    variable topTools
    variable bottomTools
    variable windowTools
    variable grabX
    variable grabY

    clearLocation $t

    set dw [Dragger::make -command "ToolBar::seeToolDrop $t" -absolute 1 -grabpoint [list $grabX $grabY] ]
    if { $dw != "" } {
      makeW $t $dw.w$t -dobind 0
      pack $dw.w$t
      set toolWindow($t) ""
      set toolState($t) 0
    }
  }

  proc hideEdit {} {
    pack forget .tbar.bottom.modes
    pack forget .tbar.bottom.tech
    pack forget .tbar.top.edit
    pack forget .tbar.top.undo
  }

  proc showEdit {} {
    pack .tbar.bottom.modes -side left -before .tbar.bottom.blockop
    pack .tbar.bottom.tech -side left -after .tbar.bottom.blockop
    pack .tbar.top.undo -side left -after .tbar.top.file
    pack .tbar.top.edit -side left -after .tbar.top.file
  }

  proc showSim {} {
    pack .tbar.bottom.simctl -side left
    pack .tbar.bottom.simaux -side left
  }

  proc hideSim {} {
    pack forget .tbar.bottom.simctl .tbar.bottom.simaux
  }


  #
  # make tool t be in window w
  #
  proc makeW {t w args} {
    variable toolWindow

    if { $t != $toolWindow($t) } {
      if { $toolWindow($t) != "" } {
	if { [string range  $toolWindow($t) 0 4] == ".tbar" } {
	  destroy $toolWindow($t)
	} else {
	  destroy .tb$t
	}
      }
      if { $w != "" } {
	eval "tb$t $w $args"
      }
      set toolWindow($t) $w
    }
  }

  #
  # swallow toolbar t
  #
  proc show {t} {
    variable topTools
    variable bottomTools
    variable windowTools
    variable toolWindow
    variable toolState
    variable toolPos

    #
    # If tool is already visible, don't do anything
    #
    if { $toolState($t) == 1 } {
      return
    }

    set after_w .tbar.top.pad
    foreach q $topTools {
      if {$t == $q } {
	set w .tbar.top.w$t
	makeW $t $w
	pack $w -after $after_w -side left
	set toolState($t) 1
	return
      }
      if { $toolState($q) == 1 } {
	set after_w .tbar.top.w$q
      }
    }

    set after_w .tbar.bottom.pad
    foreach q $bottomTools {
      if {$t == $q } {
	set w .tbar.bottom.w$t
	makeW $t $w
	pack $w -after $after_w -side left
	set toolState($t) 1
	return
      }
      if { $toolState($q) == 1 } {
	set after_w .tbar.bottom.w$q
      }
    }

    foreach q $windowTools {
      if {$t == $q } {
	set tw .tb$t
	toplevel $tw -cursor arrow -bd 2 -relief solid
	wm resizable $tw 0 0
	wm transient $tw .
	wm overrideredirect $tw 1
	wm geometry $tw $toolPos($t)


	set w $tw.w$t
	makeW $t $w

	pack $w -padx 1 -pady 1
	set toolState($t) 1
	return
      }
    }
  }

  #
  # remove a toolbar
  #
  proc hide {t} {
    variable toolWindow
    variable toolState
    #
    # If tool is already hidden, don't do anything
    #
    if { $toolState($t) == 0 } {
      return
    }

    set w $toolWindow($t)
    if { [string range $w 0 4] == ".tbar" } {
      pack forget $w
    } else {
      set rootw [winfo parent $w]
      destroy $rootw
      set toolWindow($t) ""
    }
    set toolState($t) 0
  }

  proc toolConfigure {t subw args} {
    variable toolWindow

    if { $toolWindow($t) == "" } {
      return
    }
    catch {
	eval "$toolWindow($t).$subw configure $args"
    }
  }

  #############################################################################
  #
  # Set the current toolbar mode.  Tools relevent to the mode will be displayed
  # and irrelevent tools will be hidden.
  #
  proc setMode {m} {
    variable modeTools
    variable newMode
    variable currentMode
    variable enabledTools

    if { $m == "EditInterfaces" && [info exists IPanel::data(itype)] &&  $IPanel::data(itype) == "symbol" } {
      set m EditInterfaces.sym
    }

    #
    # Hide tool groups that are in the current set but not in the new set
    #
    foreach item $modeTools($currentMode) {
      if {[lsearch $modeTools($m) $item] < 0 } {
	hide $item
      }
    }

    #
    # Show tool groups that are in the new set but not in the current set
    #
    set newMode $m
    foreach item $ToolBar::modeTools($ToolBar::newMode) {
      if {[lsearch $ToolBar::modeTools($ToolBar::currentMode) $item] < 0 \
	      && [lsearch $ToolBar::enabledTools $item] >= 0 } {
	ToolBar::show $item
      }
    }

    set currentMode $m
  }

  #############################################################################
  #
  # Update the list of enabled tools
  #
  proc updateEnabled {} {
    variable modeTools
    variable currentMode
    variable enabledTools
    global tkg_filetb tkg_edittb tkg_gateoptb tkg_undotb tkg_zoomtb tkg_modetb 
    global tkg_moduletb tkg_techtb tkg_simctltb tkg_simauxtb tkg_gateprop
    global tkg_symmodestb tkg_simmodestb tkg_symedittb tkg_zoomtb tkg_symporttb tkg_symshifttb tkg_symoprtb


    set newSet {}
    if {$tkg_filetb} { lappend newSet File }
    if {$tkg_edittb} { lappend newSet Edit }
    if {$tkg_gateoptb} { lappend newSet GateOp }
    if {$tkg_gateprop} { lappend newSet GateProp }
    if {$tkg_undotb} { lappend newSet Undo }
    if {$tkg_zoomtb} { lappend newSet Zoom }
    if {$tkg_modetb} { lappend newSet Modes LimModes }
    if {$tkg_moduletb} { lappend newSet BlockOp }
    if {$tkg_techtb} { lappend newSet Tech }
    if {$tkg_simctltb} { lappend newSet SimControl }
    if {$tkg_simauxtb} { lappend newSet SimAux } 

    if {$tkg_symmodestb} { lappend newSet SymModes } 
    if {$tkg_simmodestb} { lappend newSet SimModes } 
    if {$tkg_symedittb} { lappend newSet SymEdit } 
    if {$tkg_zoomtb} { lappend newSet SymZoom } 
    if {$tkg_symporttb} { lappend newSet SymPort } 
    if {$tkg_symshifttb} { lappend newSet SymShift } 
    if {$tkg_symoprtb} { lappend newSet SymOpr } 

    if { $enabledTools != $newSet } {
      foreach item $modeTools($currentMode) {
	if  {[lsearch $newSet $item] < 0 && [lsearch $enabledTools $item] >= 0} {
	  hide $item
	}
	if  {[lsearch $newSet $item] >= 0 && [lsearch $enabledTools $item] < 0} {
	  show $item
	}
      }

      set enabledTools $newSet
    }
  }

  proc init {} {
    variable topTools
    variable bottomTools
    variable toolWindow
    variable toolState
    variable modeTools
    variable currentMode
    variable enabledTools
    variable mbar_w
    global logo_w rot
    global gatesetech_w

    set currentMode None

    set enabledTools $modeTools(All)
    updateEnabled

    foreach t $modeTools(All) {
      set toolWindow($t) ""
      set toolState($t) 0
    }


    tbLogo .tbar.logo
    pack $logo_w -side right -ipadx 2 -ipady 2 -padx 5 -pady 5 -anchor n


    #
    # Menu setup
    #
    Menu::makeBar .tbar.m editor
    set mbar_w .tbar.m
    set gatesetech_w	$mbar_w.format.menu.settech
    KeyBinding::listener .tbar.m main -unmatchedcommand dobell
    focus $mbar_w

    frame .tbar.bar1 -bd 1 -relief sunken -height 2
    frame .tbar.top
    frame .tbar.bar2 -bd 1 -relief sunken -height 2
    frame .tbar.bottom

    pack .tbar.m -fill x
    pack .tbar.bar1 -fill x -padx 3 -pady 1
    pack .tbar.top -fill y -anchor w
    pack .tbar.bar2 -fill x -padx 3 -pady 1
    pack .tbar.bottom -fill y -anchor w
    frame .tbar.top.pad -height 25
    frame .tbar.bottom.pad -height 25
    pack .tbar.top.pad -padx 3 -side left
    pack .tbar.bottom.pad -padx 3 -side left

    trace variable rot w ToolBar::updateRotation
  }

  #############################################################################
  #
  # 
  #
  proc takeFocus {} {
    variable mbar_w

    focus $mbar_w
  }

  #############################################################################
  #
  # Set the editing tool (mode) to use.
  #
  proc selectTool {tool} {
    global mode simOn

    if { ! $simOn || $tool == 1 || $tool == 3} {
      set mode $tool
      gat_mode $tool
    }
  }

  #############################################################################
  #
  # Respond to a change in the mode.
  #
  proc modeChanged args {
    global mode tkg_batMode tkg_batRate tkg_bitWidth old_mode

    if { $mode == 1 && $tkg_batMode } {
      after $tkg_batRate gat_batInc
    }

    set old_mode $mode
  }

  proc simStateChange args {
    global simulatorState simtools_w

    set baseBg [$simtools_w cget -bg]

      if { $simulatorState == "stop" } {
	$simtools_w.simgo configure -relief flat -bg  $baseBg
	$simtools_w.simpause configure -relief sunken -bg "\#aaaacc"
      } else {
	$simtools_w.simgo configure -relief sunken -bg "\#aaaacc"
	$simtools_w.simpause configure -relief flat -bg $baseBg
      }
    }
}

trace variable mode w ToolBar::modeChanged
trace variable ::simulatorState w ToolBar::simStateChange



