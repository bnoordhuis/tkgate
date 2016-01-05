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
# Last edit by hansen on Mon Feb 23 20:30:42 2009
#


#############################################################################
#
# Use this for a basic action.  "name" is the name of the action that will
# appear in the undo/redo listboxes, and "body" is the undoable code to 
# execute.
#
proc action {name body} {
  set ok [gat_obBeginFrame $name]
  uplevel $body
  if {$ok} { gat_obEndFrame }
}

#############################################################################
#
# Use this for a action which will be appended to the most recent action.
#
proc continueAction {name body} {
  gat_obAppendFrame $name
#  namespace eval :: $body
  uplevel $body
  gat_obEndFrame
}

#############################################################################
#
# Use this for "transparent" actions.  Transparent actions are those
# for which a frame record is kept and placed in the stack, but which
# are not directly visible to the user.
#
proc transAction {name body} {
  gat_obBeginFrame $name 1
#  namespace eval :: $body
  uplevel $body
  gat_obEndFrame
}

#############################################################################
#
# Use this for "background" actions.  Background actions are actions taken
# internally by tkgate and not directly by the user.
#
proc bgAction {name body} {
  gat_obBeginFrame $name 5
#  namespace eval :: $body
  uplevel $body
  gat_obEndFrame
}

#############################################################################
#
# Use this action for operations for which undo processing will
# be suspended.
#
proc suspendAction {body} {
  set m [gat_obMode]
  gat_obMode 0
#  namespace eval :: $body
  uplevel $body
  gat_obMode $m
}

#############################################################################
#
# Use this action for operations for which undo processing will
# be suspended, and all undo data cleared.
#
proc clearAction {body} {
  set m [gat_obMode]
  gat_obMode 0
  gat_obFlush
#  namespace eval :: $body
  uplevel $body
  gat_obMode $m
}

#############################################################################
#############################################################################
##
## Tkgate actions are defined below here.  All calls from the interface
## go through an action function.
##
#############################################################################
#############################################################################
namespace eval Action  {
  proc null {} { }

  proc cancel {} {
    transAction Cancel { tkg_cancel; gat_unselectGates }
  }

  #############################################################################
  #
  # Show debugging info about wires.
  #
  proc dumpWires {} {
    gat_dumpWires
  }

  #############################################################################
  #
  # Run the internal datat structure verifier.
  #
  proc verify {} {
    gat_verify
  }

  #############################################################################
  #
  # Add an input to the selected gate.
  #
  proc addPort {} {
    action AddPort { gat_addPort }
  }

  #############################################################################
  #
  # Change the type of a port
  #
  proc editPort {} {
    action EditPort { gat_changePinDir }
  }


  #############################################################################
  #
  # Set debugging mode
  #
  proc debugMode {} {
    action Mode { ToolBar::selectTool 12 }
  }

  #############################################################################
  #
  # Put tkgate in standard edit mode
  #
  proc editMode {} {
    action Mode { ToolBar::selectTool 1 }
  }

  #############################################################################
  #
  # Put tkgate in delete gate mode (obsolete, code '3' now used for scrolling)
  #
  proc deleteMode {} {
    action Mode {ToolBar::selectTool 3 }
  }

  #############################################################################
  #
  # Put tkgate in delete gate mode
  #
  proc scrollMode {} {
    action Mode {ToolBar::selectTool 3 }
  }

  #############################################################################
  #
  # Put tkgate in cut wire mode
  #
  proc cutMode {} { 
    action Mode {ToolBar::selectTool 0 }
  }

  #############################################################################
  #
  # Put tkgate in invert in/out mode
  #
  proc invertMode {} { 
    action Mode {ToolBar::selectTool 2 }
  }

  #############################################################################
  #
  # Put tkgate in set wire size mode
  #
  proc sizeMode {} { 
    action Mode {ToolBar::selectTool 9 }
  }

  #############################################################################
  #
  # Set gate rotation to 0 degrees
  #
  proc rot0 {} { 
    action Rotation { gat_setrot 0 }
  }

  #############################################################################
  #
  # Set gate rotation to 90 degrees
  #
  proc rot90 {} { 
    action Rotation { gat_setrot 1 }
  }

  #############################################################################
  #
  # Set gate rotation to 180 degrees
  #
  proc rot180 {} { 
    action Rotation { gat_setrot 2 }
  }

  #############################################################################
  #
  # Set gate rotation to 270 degrees
  #
  proc rot270 {} { 
    action Rotation { gat_setrot 3 }
  }

  #############################################################################
  #
  # Open a Tcl shell window
  #
  proc shellWindow {} { 
    ::shellWindow
  }

  #############################################################################
  #
  # Open the selected module.
  #
  proc openMod {} {
    action Open { if { "%W" != ".scope"} { gat_openBox } else { .scope.main.frame.canvas zoom 1 } }
  }

  #############################################################################
  #
  # Close the current module.
  #
  proc closeMod {} {
    action Close { if { "%W" != ".scope"} { gat_closeBox } else { .scope.main.frame.canvas zoom -1 } }
  }

  ######################################################################
  #
  # Replicate a gate.
  #
  proc replicate {} {
    action Replicate { gat_replicate } 
  }

  ######################################################################
  #
  # Delete the selected object.
  #
  proc delete {} {
    action Delete { gat_deleteSelected }
  }

  ######################################################################
  #
  # Edit the properties of the selected object.
  #
  proc editProps {} {
    action EditProps { gat_editProps }
  }

  ######################################################################
  #
  # Set/unset a probe on the selected wire.
  #
  proc toggleProbe {} {
    gat_toggleProbe
  }

  ######################################################################
  #
  # Set/unset a probe on the selected wire.
  #
  proc delScopeProbe {} {
    gat_delScopeProbe
  }

  ######################################################################
  #
  # Move to the previous error in the error box.
  #
  proc errBoxUp {} {
    action ErrUp { ErrBox::selectUp } 
  }

  ######################################################################
  #
  # Move to the next error in the error box.
  #
  proc errBoxDown {} { 
    action ErrDown { ErrBox::selectDown }
  }

  ######################################################################
  #
  # Align gates horizontally
  #
  proc hAlign {} {
    action HAlign { gat_align 1 }
  }

  ######################################################################
  #
  # Counter clockwise rotation.
  #
  # The type of rotation depends on the current edit mode.  If en regular
  # edit mode, selected gates are rotated.  If the symbol editor is active
  # and we are in port mode, then the selected port is edited.  If the symbol
  # editor is active and we are not in port mode, then the bits are rotated. 
  #
  proc rotate {} {
    action Rotate { 
      set mm [gat_getMajorMode]

      switch $mm {
	edit {
	  gat_rotate
	}
	interface {
	  if {[SymbolEdit::isactive]} {
	    if { $SymbolEdit::emode == "port" } {
	      SymbolEdit::rotatePort 1
	    } else {
	      SymbolEdit::rotateBits 1
	    }
	  }
	}
      }
    }
  }

  ######################################################################
  #
  # Back rotate gates
  #
  proc backRotate {} {
    action Rotate { 
      set mm [gat_getMajorMode]

      switch $mm {
	edit {
	  gat_rotate -1
	}
	interface {
	  if {[SymbolEdit::isactive]} {
	    if { $SymbolEdit::emode == "port" } {
	      SymbolEdit::rotatePort -1
	    } else {
	      SymbolEdit::rotateBits -1
	    }
	  }
	}
      }
    }
  }

  ######################################################################
  #
  # Align gates vertically
  #
  proc vAlign {} {
    action VAlign { gat_align 0 }
  }

  ######################################################################
  #
  # Clear the current circuit and start editing a new file.
  #
  proc newFile {} {
    clearAction File::new
  }

  ######################################################################
  #
  # Load the specified file
  #
  proc loadFile {} {
    clearAction File::load
  }

  ######################################################################
  #
  # Load the specified file
  #
  proc loadNamedFile {name} {
    clearAction "gat_load $name"
  }

  ######################################################################
  #
  # Load modules from a library
  #
  proc loadLibrary {} {
    clearAction File::loadLibrary
  }

  ######################################################################
  #
  # Unload the library selected in the module tree view
  #
  proc unloadSelectedLibrary {} {
    set lib [BlockTree::getselection]
    if {[string first "||" $lib] != 0} return
    set lib [string range $lib 2 end]
    clearAction { gat_unloadLibrary $lib } 
  }

  ######################################################################
  #
  # Save the current circuit to the current file
  #
  proc saveFile {} {
    transAction -Save File::saveCurrent
  }

  ######################################################################
  #
  # Save the current circuit to a new file.
  #
  proc saveAsFile {} {
    clearAction File::saveAs
  }

  ######################################################################
  #
  # Import image
  #
  proc importImage {} {
    SymbolEdit::importImage
  }

  ######################################################################
  #
  # Export image
  #
  proc exportImage {} {
    SymbolEdit::exportImage
  }

  ######################################################################
  #
  # Print the current circuit
  #
  proc print {} {
    suspendAction PrintDlg::post
  }

  ######################################################################
  #
  # Exit tkgate
  #
  proc exit {} {
    File::closeApplication
  }

  ######################################################################
  #
  # Edit tkgate options
  #
  proc editOptions {} {
    suspendAction OptionDlg::post
  }

  ######################################################################
  #
  # Select all gates in the current module
  #
  proc selectAll {} { 
    if {$HdlEditor::isActive} { 
      action SelectAll { HdlEditor::selectAll }
    } else {
      action SelectAll { gat_selectAll }
    }
  }

  ######################################################################
  #
  # Edit circuit properties
  #
  proc editCProps {} {
    action EditCProps { EditCircDB::post }
  }

  ######################################################################
  #
  # Set an anchor on the selected gate
  #
  proc anchor {} { 
    action Anchor { gat_anchor 1 }
  }

  ######################################################################
  #
  # Remove an anchor from the current gate.
  #
  proc unAnchor {} {
    action UnAnchor { gat_anchor 0 }
  }

  ######################################################################
  #
  # Begin critical path analysis mode
  #
  proc cpathAnal {} {
    tkg_cpathAnal
  }

  ######################################################################
  #
  # Copy selected region to cut buffer
  #
  proc copyToBuf {} { 
    if {$HdlEditor::isActive} { 
      HdlEditor::doCopy
    } else {
      action Copy { gat_copyToBuf }
    }
  }

  ######################################################################
  #
  # Cut the selected objects and place them in the cut buffer
  #
  proc cutToBuf {} {
    if {$HdlEditor::isActive} { 
      HdlEditor::doCut
    } else {
      action Cut gat_cutToBuf
    }
  }

  ######################################################################
  #
  # Cut the selected objects and append them to the cut buffer
  #
  proc cutToBufAppend {} {
    if {$HdlEditor::isActive} { 
      HdlEditor::startCommand - 
    }
    action CutAppend gat_cutToBufAppend
  }

  ######################################################################
  #
  # Yank a copy of the cut buffer to the current module
  #
  proc yankFromBuf {} {
    if {$HdlEditor::isActive} { 
      HdlEditor::doYank
    } else {
      action Yank gat_yankFromBuf
    }
  }

  ######################################################################
  #
  # Duplicate
  #
  proc duplicate {} { 
    action Duplicate {
      gat_copyToBuf
      gat_yankFromBuf 30 30
    }
  }


  ######################################################################
  #
  # Find an object
  #
  proc findObject {} {
    transAction Find { Find::post }
  }

  ######################################################################
  #
  # Start the simulator
  #
  proc startSimulator {} {
    gat_setMajorMode simulate
  }

  ######################################################################
  #
  # End the simulator
  #
  proc endSimulator {} {
    gat_setMajorMode edit
  }

  ######################################################################
  #
  # Put simulator in "run" mode
  #
  proc simRun {} {
    tkg_simRun
  }

  ######################################################################
  #
  # Put simulator in "stop" mode.
  #
  proc simStop {} {
    tkg_simStop
  }

  ######################################################################
  #
  # Edit simulator breakpoints
  #
  proc editBreakpoints {} {
    InfoPanel::notify Breakpoints -force 1
    update
    Breakpoint::add
  }

  ######################################################################
  #
  # Execute a simulation script
  #
  proc doSimScript {} {
    tkg_doSimScript
  }

  ######################################################################
  #
  # Load a memory file
  #
  proc simLoadMem {} {
    tkg_simLoadMem
  }

  ######################################################################
  #
  # Dump a memory file.
  #
  proc simDumpMem {} {
    tkg_simDumpMem
  }

  ######################################################################
  #
  # View a memory file.
  #
  proc simViewMem {} {
    set g [gat_simSelected ram rom]

    if {$g != "" } {
      MemView::new $g.m -title $g
    } else {
      errmsg [m err.nomemselect]
    }
  }

  ######################################################################
  #
  # Print a scope trace
  #
  proc scopePrint {} {
    ScopePrintDlg::post
  }

  ######################################################################
  #
  # Step the simulator one epoch
  #
  proc simStep {} {
    Simulator::stepEpoch
  }

  ######################################################################
  #
  # Step the simulator one "clock cycle".
  #
  proc simCycle {} {
    tkg_simCycle
  }

  ######################################################################
  #
  # If in simulator mode, step the simulator one "clock cycle".  If in
  # edit mode, rotate the selected gates.
  #
  proc simCycleOrRotate {} {
    set mm [gat_getMajorMode]

    if { $mm == "simulate" } {
      tkg_simCycle
    } else {
      rotate
    }
  }

  ######################################################################
  #
  # Refresh the screen.
  #
  proc refreshScreen {} {
    gat_refreshScreen
  }

  ######################################################################
  #
  # Create a new module
  #
  proc blockNew {} {
    action NewMod BlockOp::new
  }

  ######################################################################
  #
  # Delete a module
  #
  proc blockDelete {} {
    action DeleteMod BlockOp::delete
  }

  ######################################################################
  # 
  # Copy a module
  #
  proc blockCopy {} {
    action CopyMod BlockOp::copy
  }

  ######################################################################
  # 
  # Set root module
  #
  proc blockSetRoot {} {
    action SetRoot BlockOp::setroot
  }

  ######################################################################
  #
  # Rename a module
  #
  proc blockRename {} {
    action RenameMod BlockOp::rename
  }

  ######################################################################
  #
  # Set the interface of the selected module
  #
  proc setBlockDesc {} {
    action SetInterf { gat_setBlockDesc }
  }

  ######################################################################
  #
  # Edit the module interfaces
  #
  proc editBlockDesc {} {
    action EditInterf gat_editBlockDesc
  }

  ######################################################################
  #
  # Autogenerate a module interface
  #
  proc autoGenerate {} {
    IGen::post -which ipanel
  }

  ######################################################################
  #
  # Autogenerate a module interface from module list
  #
  proc autoGenerateSelected {} {
    IGen::post -which selected
  }

  ######################################################################
  #
  # Autogenerate a module interface from direct canvas selection
  #
  proc autoGenerateCanvas {} {
    IGen::post -which canvas
  }

  ######################################################################
  #
  # "Claim" a block (make it a non-library block)
  #
  proc blockClaim {} {
    action Claim BlockOp::claim
  }

  ######################################################################
  #
  # Shows/edits properties of a module
  #
  proc blockProp {} {
    action Props ModuleProps::postSelected
  }

  ######################################################################
  #
  # Shows/edits properties of a module
  #
  proc blockPropCanvas {} {
    action Props { ModuleProps::postSelected -which canvas }
  }

  ######################################################################
  #
  # Zoom in
  #
  proc zoomIn {} {
    action ZoomIn { gat_zoom 1 }
  }

  ######################################################################
  #
  # Zoom out
  #
  proc zoomOut {} {
    action ZoomOut { gat_zoom -1 }
  }

  ######################################################################
  #
  # SymboleEdit Zoom in
  #
  proc seZoomIn {} {
    SymbolEdit::zoom 1
  }

  ######################################################################
  #
  # SymboleEdit Zoom out
  #
  proc seZoomOut {} {
    SymbolEdit::zoom -1
  }

  ######################################################################
  #
  # Nudge the selected objects left
  #
  proc nudgeLeft {} {
    action NudgeL { gat_moveGate -1 0 }
  }

  ######################################################################
  #
  # Nudge the selected objects right
  #
  proc nudgeRight {} {
    action NudgeR { gat_moveGate 1 0 }
  }

  ######################################################################
  #
  # Nudge the selected objects up
  #
  proc nudgeUp {} {
    action NudgeU { gat_moveGate 0 -1 }
  }

  ######################################################################
  #
  # Nudge the selected objects down
  #
  proc nudgeDown {} {
    action NudgeD { gat_moveGate 0 1 }
  }

  ######################################################################
  #
  # Show the about message
  #
  proc showAbout {} {
    ::showAbout
  }

  ######################################################################
  #
  # Show the license message
  #
  proc showLicense {} {
    ::showLicense
  }

  ######################################################################
  #
  # Show the documentation message
  #
  proc showDocumentation {} {
    ::showDocumentation
  }

  ######################################################################
  #
  # Load example circuit 1
  #
  proc loadExample {} { global tkgate_exampledir; gat_load $tkgate_exampledir/index.v }


  ######################################################################
  #
  # Load home page
  #
  proc loadWelcome {} { global tkgate_tutorialdir; gat_load $tkgate_tutorialdir/welcome.v }

  ######################################################################
  #
  # Load tutorial index
  #
  proc loadTutorial {} { global tkgate_tutorialdir; gat_load $tkgate_tutorialdir/index.v }

  #############################################################################
  #
  # Undo last action
  #
  proc undo {} {
    HdlEditor::startCommand -
    gat_undo
  }

  #############################################################################
  #
  # Redo last undone action
  #
  proc redo {} {
    HdlEditor::startCommand -
    gat_redo
  }

  ######################################################################
  #
  # Update the interface of a module instance  with the current interface 
  #
  proc updateInterface {} {
    action UpdIntf { gat_updateInterface }
  }

  ######################################################################
  #
  # Update the interface of all module instances with the current interface 
  #
  proc updateAllInterfaces {} {
    action UpdAllIntf { gat_updateInterface -all }
  }

  ######################################################################
  #
  # Update the interface of all module instances with the current interface 
  #
  proc updateOpenInterface {} {
    action UpdAllIntf { gat_updateInterface -open }
  }

  ######################################################################
  #
  # Edit wire properties through popup menu
  #
  proc popupWireProps {} { continueAction WireProp gat_popupWireProps }

  ######################################################################
  #
  # Add a stub to a wire
  #
  proc popupWireAddStub {} { action AddStub gat_popupWireAddStub }

  ######################################################################
  #
  # Hide/Show the bit size mark on a multi-bit wire
  #
  proc popupWireHideSize {} { action SizeDisp { gat_popupWireSize -hide }}
  proc popupWireShowSize {} { action SizeDisp { gat_popupWireSize -show }}

  ######################################################################
  #
  # Hide/Show the label on a wire
  #
  proc popupWireHideLabel {} { action SizeDisp { gat_popupWireLabel -hide } }
  proc popupWireHideAllLabel {} { action SizeDisp { gat_popupWireLabel -hideall } }
  proc popupWireClearAllLabel {} { action SizeDisp { gat_popupWireLabel -clearall } }
  proc popupWireShowLabel {} { action SizeDisp { gat_popupWireLabel -show } }

  ######################################################################
  #
  # Change the anchor status of an object
  #
  proc popupAnchor {} { action Anchor { global pop_anchor; gat_anchor $pop_anchor } }

  ######################################################################
  #
  # Change the port type of a module port
  #
  proc popupPortMakeIn {} { action ChgPort { gat_popupPortCmd change-in } }
  proc popupPortMakeOut {} { action ChgPort { gat_popupPortCmd change-out } }
  proc popupPortMakeInOut {} { action ChgPort { gat_popupPortCmd change-inout } }


  ######################################################################
  #
  # Delete a port on a module through right-click
  #
  proc popupDeletePort {} { action DelPort { gat_popupPortCmd delete } }

  ######################################################################
  #
  # Delete a port on a module through right-click
  #
  proc popupPortSize {n} { action SizePort { gat_popupPortCmd size $n } }

  ######################################################################
  #
  # Add an input port to a module
  #
  proc popupBlockAddIn {} { action AddInPort { gat_popupPortCmd add-in } }

  ######################################################################
  #
  # Add an output port to a module
  #
  proc popupBlockAddOut {} { action AddOutPort { gat_popupPortCmd add-out } }

  ######################################################################
  #
  # Add an in/out port to a module
  #
  proc popupBlockAddInOut {} { action AddTriPort { gat_popupPortCmd add-inout } }

  #############################################################################
  #
  # Edit the interface of the selected block.
  #
  #############################################################################
  proc openInterface {args} {
    action EditInterf {
      set name [gat_getSelected module]
      if { $name == "" } {
	set name [tkg_getSelectedBlock]
      }
      if { $name != "" } {
	gat_editBlockDesc $name
      }
    }
  }

  proc hdlDeleteForward {} {
    HdlEditor::startCommand DelFwrd
    HdlEditor::doDelete 0
  }
  proc hdlDeleteBackward {} {
    HdlEditor::startCommand DelBack
    HdlEditor::doDelete -1
  }
  proc hdlCut {} {
    HdlEditor::startCommand -Cut
    HdlEditor::doCut
  }
  proc hdlYank {} {
    HdlEditor::startCommand Yank
    HdlEditor::doYank
  }
  proc hdlKillLine {} {
    HdlEditor::startCommand -KillLine
    HdlEditor::doKillLine
  }
  proc hdlTransposeChar {} {
    HdlEditor::startCommand CharSwap
    HdlEditor::doTransposeChar
  }
  proc hdlSearch {} {
    HdlEditor::doSearch
  }
  proc hdlReverseSearch {} {
    HdlEditor::doRSearch
  }
  proc hdlPageDown {} {
    HdlEditor::startCommand -
    HdlEditor::doPageMove 1 
  }
  proc hdlPageUp {} {
    HdlEditor::startCommand -
    HdlEditor::doPageMove -1
  }
  proc hdlDeleteWord {} {
    HdlEditor::startCommand DelWord
    HdlEditor::doDeleteWord 1
  }
  proc hdlOpenLine {} {
    HdlEditor::startCommand OpenLine
    HdlEditor::doOpenLine
  }
  proc hdlGotoLineEnd {} {
    HdlEditor::startCommand -
    HdlEditor::doMoveTo "insert lineend"
  }
  proc hdlGotoLineStart {} {
    HdlEditor::startCommand -
    HdlEditor::doMoveTo "insert linestart"
  }
  proc hdlGotoTop {} {
    HdlEditor::startCommand -
    HdlEditor::doMoveTo "1.0" -setpoint 1
  }
  proc hdlGotoEnd {} {
    HdlEditor::startCommand -
    HdlEditor::doMoveTo "end" -setpoint 1
  }
  proc hdlForwardWord {} {
    HdlEditor::startCommand -
    HdlEditor::doWordMove 1
  }
  proc hdlBackwardWord {} {
    HdlEditor::startCommand -
    HdlEditor::doWordMove -1
  }
  proc hdlBackwardChar {} {
    HdlEditor::startCommand -
    HdlEditor::doHMove -1
  }
  proc hdlForwardChar {} {
    HdlEditor::startCommand -
    HdlEditor::doHMove 1
  }
  proc hdlUpLine {} {
    HdlEditor::startCommand -
    HdlEditor::doVMove 1
  }
  proc hdlDownLine {} {
    HdlEditor::startCommand -
    HdlEditor::doVMove -1
  }
  proc hdlSetMark {} {
    HdlEditor::startCommand -
    HdlEditor::doSetPoint
  }
  proc hdlExchangePointAndMark {} {
    HdlEditor::startCommand -
    HdlEditor::doPointSwap 
  }
  proc hdlCancel {} {
    HdlEditor::startCommand -
    HdlEditor::doCancel
  }
  proc hdlIndent {} {
    HdlEditor::startCommand Indent
    HdlEditor::doIndent
  }
  proc hdlLowercaseWord {} {
    HdlEditor::startCommand ToLower
    HdlEditor::doLowercaseWord
  }
  proc hdlUppercaseWord {} {
    HdlEditor::startCommand ToUpper
    HdlEditor::doUppercaseWord
  }
  proc hdlCasefyWord {} {
    HdlEditor::startCommand Casefy
    HdlEditor::doCasefyWord
  }
  proc hdlSetArgument {} {
    HdlEditor::doSetArgument
  }

  proc printTrace {} {
    ScopePrintDlg::post
  }
  proc scopeZoomIn {} {
    catch { .scope.main.frame.canvas zoom -1 }
  }
  proc scopeZoomOut {} {
    catch { .scope.main.frame.canvas zoom 1 }
  }

  proc seRShiftBits {} {
    SymbolEdit::shiftBits 1 0
  }

  proc seLShiftBits {} {
    SymbolEdit::shiftBits -1 0
  }

  proc seUShiftBits {} {
    SymbolEdit::shiftBits 0 -1
  }

  proc seDShiftBits {} {
    SymbolEdit::shiftBits 0 1
  }

  proc seCWRotate {} {
    SymbolEdit::rotateBits 1
  }

  proc seCCWRotate {} {
    SymbolEdit::rotateBits -1
  }

  proc seCWRotPort {} {
    SymbolEdit::rotatePort -1
  }

  proc seCCWRotPort {} {
    SymbolEdit::rotatePort 1
  }

  proc seBoldBits {} {
    SymbolEdit::boldBits
  }

  proc seResize	{} {
    SymbolEdit::resize
  }

  proc seCutToBuf {} {
    SymbolEdit::cutRegion
  }
  proc seCopyToBuf {} {
    SymbolEdit::copyRegion
  }
  proc sePasteFromBuf {} {
    SymbolEdit::pasteRegion
  }
  proc seOverlayFromBuf {} {
    SymbolEdit::overlayRegion
  }
}
