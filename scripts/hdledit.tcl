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
# Last edit by hansen on Mon Feb 23 20:33:08 2009

#proc dbgputs {s} { puts $s }
proc dbgputs {s} { }

#############################################################################
#
# This set of functions implement the HDL text editor.  It includes the code
# for colorizing verilog code, auto indentation and basic editing features.
#
# Member Function Summary:
#	askRename		Ask user if module should be renamed.
#	askRenameClose		Close the askRename dialog box
#	askSaveOption		Ask user what to do about multiple modules
#	askSaveOptionClose	Close the SaveOption dialog box
#	disable			Disable editing
#	enable			Enable editing
#	isactive		Is the editor active?
#	isselection		Is there an active selection?
#	isselection2		Is there an active selection? (create from mark/pointer if possible)
#	ispointerinselection	Test coordinates to see if it is in the selection
#	hide			Hide the HDL editor
#	show			Show the HDL editor
#	startCommand		Start an edit on the HDL text.
#	configureTag		Configure text tag
#	clear			Clear the contents of the HDL buffer.
#	endText			End adding lines to buffer
#	addLine			Add a line to the buffer
#	insert			Insert a block of text into the buffer
#	insertFile		Insert the contents of a file into the buffer
#	loadText		Load the HDL buffer with the specified text
#	dumpText		Dump the contents of the HDL buffer
#	dumpSelection		Dump the selection region
#	undoSync		Synchronize HDL buffer after undo/redo
#	highlightLine		Highlight a specified line
#	openModule		Begin editing the module named in the selection
#	toggleProbe		Add/delete a probe at the current word
#	undoSelection		Undo the selected region
#
#	seeB3Press		Respond to press of mouse button 3
#	seeB1Press		Respond to press of mouse button 1
#	seeB1DoublePress	Respond to double press of mouse button 1
#	seeB1TripplePress	Respond to tripple press of mouse button 1
#	seeB1Motion		Respond to motion while mouse button 1 is pressed
#	seeB1Release		Respond to release of mouse button 1
#	seeKey			Respond to a key press
#
#	leadingSpaces		Return number of leading spaces on a line.
# 	isStop			Test a token to see if it is a "stop" token.
#	isStart			Test a token to see if it is a "start" token.
#	colorizeToken		Colorize a token
#	tokenize		Parse a line into tokens
#	clearTargetChar		Clear the target character 
#	getWord			Get the next word
#	removeTags		Remove tags from a region
#	finalState		Get the tag state at the end of a line
#	getLineTags		Get the tag state at the beginning of a line
#	recolorize		Recolorize a region
#
#	doIndent		Do indenting of current line or selected region
#	doSetPoint		Set the point for a selection
#	doPointSwap		Swap the point and cursor locations
#	doHMove			Move cursor horizontally
#	doCut			Cut the selection to the cut buffer
#	doCopy			Copy the selection to the cut buffer
#	doYank			Yank the cut buffer to the current location
#	doCancel		Cancel any command sequence in progress
#	doOpenLine		Open a blank line below the current line
#	doDeleteWord		Delete the word (forward or back) from cursor position
#	doPageMove		Move up or down a page
#	doRSearch		Begin a reverse search (or search again)
#	doSearch		Begin a normal search (or search again)
#	doTransposeChar		Transpose characters around the cursor
#	doKillLine		Kill the current line and append it to the cut buffer
#	doVMove			Move the cursor vertically (up or down)
#	doMoveTo		Move cursor to specific location
#	doSetArgument		Set the command argument status line
#	doLowercaseWord		Move cursor forward one word and make all chars lowercase 
#	doLowercaseWord		Move cursor forward one word and make all chars uppercase 
#	doCasefyWord		Move cursor forward one word and make all chars lowercase except first
#	doWordMove		Move cursor forward or backward one word
#	doDelete		Delete forward or backward one character
#
#	updateSearchStatus	Update information about the current search
#	searchDelete		Remove a character from search string
#	searchContinue		Add a character to the search string
#	searchCancel		Cancel the current search
#	argCancel		Cancel the entry of the argument value
#	argContinue		Add a charcter to the argument value
#
#	buildMainEdit		Build the main edit window (helper used by "new")
#	new			Create the edit window
#
#
#

namespace eval HdlEditor {} {
  variable isFocused 0
  variable isActive 0

  # Value of hdl_w when it is used in the "main" window
  variable main_w

  # Top-level window for the HDL editor (including scroll bars~
  variable hdl_w ""

  # Window name for the actual text widget.
  variable text_w ""

  # Non-zero if we have clicked on a selection and are trying to drag it.
  variable isMoveText 0

  # Name of the module in the HDL editor
  variable active_module ""

  # Line number that is highlighted (to show error locations, etc.~
  variable highlight_line_no -1

  # ID of the last kill (emacs ^K) command.
  variable last_kill -1

  # ID of the last search (emacs ^S/^R) command.
  variable last_search -1

  # Direction of the current search (1=forwards, -1= backwards)
  variable search_dir 1

  # Target string for the search.
  variable search_string ""

  # Status message for search (can be "" or "Failing ")
  variable search_status ""

  # String for accumulating argument digits (emacs ^U)
  variable argument_string ""

  # Value of the argument (set by ^U in emacs~
  variable argument 1

  # Non-zero if the argument is active
  variable argument_active 0

  # Result of the module disposition dialog box (can be split, ignore or cancel)
  variable asoResult

  # Result of the module rename dialog box (can be autoedit, ignore, cancel~
  variable arResult

  # Target character position for the line up/down command.
  variable targetChar -1

  # Non-zero if we are currently processing a double or tripple mouse click.
  variable didMultiPress 0

  # Undo name of the current command being executed
  variable commandName ""

  # List of verilog keywords that should be highlighted in keyword color.
  variable keywordList {
    always assign attribute begin case casex casez cmos deassign
    default defparam disable edge else end endattribute endcase endfunction endmodule
    endprimitive endspecify endtable endtask event for force forever fork function
    highz0 highz1 if initial inout input integer join large medium module
    macromodule negedge output parameter posedge primitive pull0 pull1 pulldown pullup
    real realtime reg release repeat scalared signed small specify specparam
    strength strong0 strong1 supply0 supply1 table task time
    tri tri0 tri1 triand trior trireg unsigned vectored wait wand weak0 weak1
    while wire wor 
  }

  # List of verilog gate names that should be highlighted in gate color.
  variable gatewordList {
    and xnor xor buf bufif0 bufif1 pmos tran tranif0 tranif1  rnmos rpmos rtran
    rtranif0 rtranif1 nand nmos nor not notif0 notif1 or rcmos
  }

  #
  # Tokens that start a block 
  #
  variable beginTokens {module primitive case casex casez fork begin task function}

  #
  # Tokens that start a module (or similar structure)
  #
  variable moduleTokens {module primitive case}

  #
  # Tokens that end a block 
  #
  variable endTokens {endmodule endprimitive endcase endtask endfunction endspecify endtable end join}

  #
  # Tokens that signal the end of a statement
  #
  variable stopTokens [concat $endTokens ";"]

  #
  # Tokens that start an 'if' statement
  #
  variable ifTokens {if}

  #
  # Tokens that start an 'else' statement
  #
  variable elseTokens {else}

  #############################################################################
  #
  # Test for a stop token
  #
  proc isStopToken {token} {
    variable stopTokens

    return [expr [lsearch -exact $stopTokens $token] >= 0]
  }

  #############################################################################
  #
  # Test for an end token
  #
  proc isEndToken {token} {
    variable endTokens

    return [expr [lsearch -exact $endTokens $token] >= 0]
  }

  #############################################################################
  #
  # Test for a begin token
  #
  proc isBeginToken {token} {
    variable beginTokens

    return [expr [lsearch -exact $beginTokens $token] >= 0]
  }

  #############################################################################
  #
  # Test for a module token
  #
  proc isModuleToken {token} {
    variable moduleTokens

    return [expr [lsearch -exact $moduleTokens $token] >= 0]
  }
  
  #############################################################################
  #
  # Test for an if token
  #
  proc isIfToken {token} {
    variable ifTokens

    return [expr [lsearch -exact $ifTokens $token] >= 0]
  }

  #############################################################################
  #
  # Test for an else token
  #
  proc isElseToken {token} {
    variable elseTokens

    return [expr [lsearch -exact $elseTokens $token] >= 0]
  }

  #############################################################################
  #
  # Ask user what to do about a renamed module.
  #
  #    Rename		Rename the module to match the text in the definition.
  #    Ignore		Ignore the discrepancy and save module data anyway.
  #    Cancel		Cancel the operation and return to the buffer.
  #
  proc askRename {} {
    variable arResult

    set w .arw

    if { [catch { toplevel $w } ] } { return }
    wm title $w [m db.hdl.namechange.title]
    wm geometry $w [offsetgeometry . 50 50 ]
    wm transient $w .

    dialogImage $w.di -image [gifI general.gif] -caption [m db.hdl.autoeditcap]  -force 1
    pack $w.di -side left -fill y -expand 1

    label $w.exp -text [m db.hdl.autoeditexp] -wraplength 300 -justify left
    pack $w.exp -padx 25 -pady 25

    frame $w.b

    button $w.b.autoedit -default active -text [m db.hdl.autoedit] -command { HdlEditor::askRenameClose autoedit } -width 10
    button $w.b.ignore -text [m db.hdl.ignore] -command { HdlEditor::askRenameClose ignore } -width 10
    button $w.b.cancel -text [m db.hdl.cancel] -command { HdlEditor::askRenameClose cancel } -width 10

    pack $w.b.autoedit  $w.b.ignore $w.b.cancel -padx 10 -side left
    pack $w.b -pady 25

    helpon $w.b.autoedit [m ho.hdl.autoeditexp]
    helpon $w.b.cancel [m ho.hdl.cancelexp]
    helpon $w.b.ignore [m ho.hdl.ignoreexp]

    dialogWait $w

    return $arResult
  }

  #############################################################################
  #
  # Helping function for askRename that is called when one of the buttons
  # is pressed.
  #
  proc askRenameClose {option} {
    variable arResult

    set w .arw
    destroy $w
    set arResult $option
  }

  #############################################################################
  #
  # Show dialog box asking what to do about multiple modules in the buffer for
  # a single module.  The presented options are:
  #
  #    Split		Split multiple modules into new modules 
  #    Ignore		Ignore the discrepancy and save module data anyway
  #    Cancel		Cancel the operation and return to the buffer
  #
  proc askSaveOption {} {
    variable asoResult

    set w .asow

    if { [catch { toplevel $w } ] } { return }
    wm title $w [m db.hdl.textdisp.title]
    wm geometry $w [offsetgeometry . 50 50 ]
    wm transient $w .
    
    dialogImage $w.di -image [gifI general.gif] -caption [m db.hdl.savecap]  -force 1
    pack $w.di -side left -fill y -expand 1

    label $w.exp -text [m db.hdl.saveexp] -wraplength 300 -justify left
    pack $w.exp -padx 25 -pady 25

    frame $w.b

    button $w.b.split -default active -text [m db.hdl.split] -command { HdlEditor::askSaveOptionClose split } -width 10
    button $w.b.ignore -text [m db.hdl.ignore] -command { HdlEditor::askSaveOptionClose ignore } -width 10
    button $w.b.cancel -text [m db.hdl.cancel] -command { HdlEditor::askSaveOptionClose cancel } -width 10

    pack $w.b.split  $w.b.ignore $w.b.cancel -padx 10 -side left
    pack $w.b -pady 25

    helpon $w.b.split [m ho.hdl.splitexp]
    helpon $w.b.cancel [m ho.hdl.cancelexp]
    helpon $w.b.ignore [m ho.hdl.ignoreexp]

    dialogWait $w

    return $asoResult
  }

  #############################################################################
  #
  # Helping function for askSaveOption that is called when one of the buttons
  # is pressed.
  #
  proc askSaveOptionClose {option} {
    variable asoResult

    set w .asow
    destroy $w
    set asoResult $option
  }

  #############################################################################
  #
  # Disable editing - display only
  #
  proc disable {} {
    variable text_w

    selectMain

    $text_w configure -state disabled
  }

  #############################################################################
  #
  # Enable editing
  #
  proc enable {} {
    variable text_w

    $text_w configure -state normal
  }

  #############################################################################
  #
  # Return non-zero if the HDL editor is active.
  #
#  proc isactive {} {
#    variable active_module
#    variable isFocused
#
#    if { $isFocused } {
#      return 1
#    }
#
#    if { $active_module == "" } {
#      return 0
#    } else {
#      return 1
#    }
#  }

  #############################################################################
  #
  # Return non-zero if there is an active selection
  #
  proc isselection {} {
    variable active_module
    variable text_w
    variable isActive

    if { !$isActive } {
      return 0
    }

    set range [$text_w tag ranges sel]

    return [expr [llength $range] != 0]
  }

  #############################################################################
  #
  # Return non-zero if there is an active selection.  Creates a selection if
  # necessary from the "point" mark.
  #
  proc isselection2 {} {
    variable active_module
    variable text_w
    variable isActive

    if { !$isActive } {
      return 0
    }

    set range [$text_w tag ranges sel]

    if { [llength $range] != 0} {
      return 1
    }

    if { [lsearch -exact [$text_w mark names] "point"] < 0} {
      return 0
    }

    
    if { [$text_w compare insert < point] } {
      $text_w tag add sel insert point
    } elseif { [$text_w compare insert > point] } {
      $text_w tag add sel point insert
    } else {
      return 0
    }

    return 1
  }

  #############################################################################
  #
  # Returns non-zero if the coordinates (x,y) are over the selection.
  #
  proc ispointerinselection {x y} {
    variable text_w

    # Check to see if there is an active selection and pointer is in that selection
    if {[isselection]} {
      set range [$text_w tag ranges sel]

      if { [$text_w compare @$x,$y >= end-1chars] } {
	return 0
      }

      if { [$text_w compare @$x,$y >= [lindex $range 0]] \
	       && [$text_w compare @$x,$y < [lindex $range 1]] } {
	return 1
      }
    }

    return 0
  }

  #############################################################################
  #
  # Show HDL editor in a dialog box (not connected with a module)
  #
  proc dbShow {w} {
    variable isActive
    set isActive 1
  }

  #############################################################################
  #
  # Hide HDL editor from a dialog box (not connected with a module)
  #
  proc dbHide {} {
    variable isActive
    set isActive 0
  }

  #############################################################################
  #
  # Hide the HDL editor
  #
  proc hide {} {
    variable hdl_w
    variable text_w
    global main_w
    variable active_module
    variable isActive

    if {$active_module == ""} { return } 

    set isActive 0

    selectMain

    startCommand -
#    gat_hdl close -cursor [$text_w index insert]

    set active_module ""
    grid forget $hdl_w
    grid $main_w.vert -column 1 -row 0 -sticky ns
    grid $main_w.horz -column 0 -row 1 -sticky ew
    grid $main_w.main -row 0 -column 0 -sticky nsew

    ToolBar::takeFocus
  }

  #############################################################################
  # 
  # Make the HDL editor visible with module $name
  #  
  # 
  proc show {name} {
    variable hdl_w
    variable text_w
    global main_w
    variable active_module
    variable isActive

    set isActive 1
    selectMain

#    puts "HdlEditor::show $name"

    if { $name == $active_module } return 

    configureTag linecomment hdlComment
    configureTag blockcomment hdlComment
    configureTag keyword hdlReserved
    configureTag gateword hdlGateWord
    configureTag instword hdlInstWord
    configureTag taskword hdlTaskWord
    configureTag string hdlString


    if { $active_module != ""} {
      startCommand -
    } else {
      grid forget $main_w.main
      grid forget $main_w.vert
      grid forget $main_w.horz
      grid $hdl_w -row 0 -column 0 -sticky nsew -rowspan 3 -columnspan 3
    }

    gat_hdl load
    set active_module $name
    set idx [gat_hdl getindex]
    if {$idx != ""} { $text_w mark set insert $idx }
    focus $text_w
  }

  #############################################################################
  #
  # This function is called before starting any editing action, or to force
  # a synchronization with tkgate.  Two forms of the command may be used:
  #
  # startCommand Name			Called before a command that modifies the buffer
  # startCommand -			Called before a command that does not modify buffer. 
  #
  #
  # last	new		action
  # -------------------------------------------------------------------------------------
  #  ""		Name		Do an empty action with Name and set last equal to Name. 
  # Other	Name		Synchronize and set last equal to Name.
  #  ""         -		Do nothing.
  # Other	-		Synchronize and set last equal to ""
  # Name	Name		Do nothing.
  #
  proc startCommand {name} {
    variable commandName
    variable active_module
    variable text_w

    if {$commandName == $name } return
    if {$active_module == ""} return

    if {$commandName != "" } {
#      puts "startCommand continueAction $commandName"
      continueAction $commandName "gat_hdl save $active_module -cursor [$HdlEditor::text_w index insert]"
    }

    if { $name == "-" } {
      set commandName ""
    } else {
      set commandName $name
#      puts "startCommand action $name"
      action $name { gat_hdl touch  -cursor [$HdlEditor::text_w index insert] }
    }
  }

  #############################################################################
  #
  # Configure a tag
  #
  proc configureTag {tagname vname} {
    global tkg_${vname}Color tkg_${vname}Bold tkg_${vname}Italic
    variable text_w

    set color [set tkg_${vname}Color]
    set bold ""
    set italic ""
    if {[set tkg_${vname}Bold]} { set bold Bold }
    if {[set tkg_${vname}Italic]} { set italic Italic }

    $text_w tag configure $tagname -foreground $color -font hdl${bold}${italic}Font
  }


  #############################################################################
  #
  # Clear the contents of the HDL buffer.
  #
  proc clear {} {
    variable text_w

    dbgputs "HdlEditor::clear"

    $text_w delete 1.0 end
  }

  #############################################################################
  #
  # Called to indicate that tkgate is done adding lines.
  #
  proc endText {} {
    variable highlight_line_no
    variable text_w

    if { $highlight_line_no >= 0 } {
      $text_w tag add highlight $highlight_line_no.0 $highlight_line_no.end
      $text_w see $highlight_line_no.0
      $text_w mark set insert $highlight_line_no.0
    }
    set highlight_line_no -1

    recolorize 1.0 end

    set idx [gat_hdl getindex]
    if {$idx != ""} { $text_w mark set insert $idx }
  }

  #############################################################################
  #
  # obsolete?
  #
  proc addLine {text} {
    variable text_w

    $text_w insert end "$text\n"
  }

  #############################################################################
  #
  # Insert a block of text at the insertion cursor position.
  #
  # Parameters:
  #      text		Block of text to insert.
  #
  proc insert {text} {
    variable text_w

    set p [$text_w index insert]
    $text_w insert $p $text
  }

  #############################################################################
  #
  # Insert contents of a file at the insertion cursor position.
  #
  # Parameters:
  #      fileName	Name of file to insert
  #
  proc insertFile {fileName} {
    variable text_w

    set p [$text_w index insert]
    set f [open $fileName r]
    if {$f == ""} return
    $text_w insert $p [read $f]
    close $f
    endText
  }

  #############################################################################
  #
  # Load the HDL buffer
  #
  # Parameters:
  #      text		Text to load into buffer.
  #
  proc loadText {text} {
    variable text_w

    $text_w delete 1.0 end
    $text_w insert end $text
    endText
  }

  #############################################################################
  #
  # Dump the contents of the HDL buffer
  #
  # Returns:		HDL buffer contents as a single string.
  #
  proc dumpText {} {
    variable text_w
    return [$text_w get 1.0 end-1chars]
  }

  #############################################################################
  #
  # Dump selection region of the HDL buffer
  #
  # Returns:		String with contents of selection.
  #
  proc dumpSelection {} {
    variable text_w

    set range [$text_w tag ranges sel]
    if {[llength $range] > 0 } {
      set textDump [eval $text_w get $range]
    } else {
      set textDump ""
    }

    return $textDump
  }

  #############################################################################
  #
  # Synchronize the HDL buffer with internal state after an undo or redo
  # operation.
  #
  proc undoSync {} {
    variable active_module

    if {$active_module != "" } {
      gat_hdl load
    }
  }

  #############################################################################
  #
  # Highlight a line of HDL text
  #
  proc highlightLine {lno} {
    variable text_w
    variable highlight_line_no

    dbgputs "highlightLine $lno"
    set highlight_line_no $lno

    catch {
      $text_w tag remove highlight 1.0 end
      if { $highlight_line_no >= 0 } {
	$text_w tag add highlight $highlight_line_no.0 $highlight_line_no.end
	$text_w see $highlight_line_no.0
	$text_w mark set insert $highlight_line_no.0
      }
    }

  }

  #############################################################################
  #
  # Try to open the module indicated by the word around the current 
  # insertion cursor.
  #
  proc openModule {} {
    set word [getWord insert start stop]
    if { $word != "" } {
      action Open "gat_openBox -hierarchical \{$word\}"
    }
  }

  #############################################################################
  #
  # Add/Delete a probe based on the specified text.
  #
  proc toggleProbe {} {
    set word [getWord insert start stop]
    if { $word != "" } {
      action ToggleProbe "gat_toggleProbe \{$word\}"
    }
  }


  #############################################################################
  #
  # Undo the selection region
  #
  proc undoSelection {} {
    variable text_w

    set range [$text_w tag ranges sel]
    if {[llength $range] > 0 } {
      eval "$text_w tag remove sel $range"
    }
  }

  #############################################################################
  #
  # Called when the third button is pressed.  Set the insertion cursor to the
  # selected position and bring up the popup menu.
  #
  proc seeB3Press {x y X Y} {
    variable text_w
    global tkg_cutBufferActive
    global simOn

    searchCancel

    if {$simOn} {
      $text_w mark set insert @$x,$y
      $text_w see insert
      getWord insert start stop
      undoSelection
      if { $start != $stop } {
	$text_w tag add sel $start $stop
      }
      tk_popup .pop_hdlsimu $X $Y
    } elseif {[ispointerinselection $x $y]} {
      tk_popup .pop_hdleditsel $X $Y
      gat_updateMenuState
    } else {
      $text_w mark set insert @$x,$y
      $text_w see insert
      if {$tkg_cutBufferActive} {
	tk_popup .pop_hdleditp $X $Y
      } else {
	tk_popup .pop_hdledit $X $Y
      }
      gat_updateMenuState
    }
  }

  #############################################################################
  #
  # Called when the first button is pressed.  Set the insertion cursor to the
  # selected position.
  #
  proc seeB1Press {x y} {
    variable text_w
    variable isMoveText
    global bd

    searchCancel

    if {[ispointerinselection $x $y]} {
#      $text_w configure -cursor "@$bd/movtext_curs.xcur $bd/movtext_mask.xcur black white"
      $text_w configure -cursor fleur
      set isMoveText 1
    } else {
      undoSelection
      $text_w mark set insert @$x,$y
      $text_w see insert
    }
  }

  #############################################################################
  #
  # Called when the first button is double clicked.  Selects the current word
  #
  proc seeB1DoublePress {x y} {
    variable text_w
    variable didMultiPress

    getWord insert start stop
    undoSelection
    if { $start != $stop } {
      $text_w tag add sel $start $stop
      set didMultiPress 1
    }
  }

  #############################################################################
  #
  # Called when the first button is triple clicked.  Selects the current line.
  #
  proc seeB1TriplePress {x y} {
    variable text_w
    variable didMultiPress

    scan [$text_w index insert] %d lineno
    undoSelection
    $text_w tag add sel $lineno.0 $lineno.end
    set didMultiPress 1
  }

  #############################################################################
  #
  # Called when the mouse is move with the first button depressed.
  #
  proc seeB1Motion {x y} {
    variable text_w
    variable isMoveText
    variable didMultiPress

    if {$didMultiPress} return

    searchCancel

    if {$isMoveText} {
    } else {
      undoSelection

      set p1 [$text_w index insert]
      set p2 [$text_w index @$x,$y]
      $text_w see $p2

      if { [$text_w compare $p1 < $p2] } {
	$text_w tag add sel $p1 $p2
      } else {
	$text_w tag add sel $p2 $p1
      }
    }
  }

  #############################################################################
  #
  # Called when the first mouse button is released.
  #
  proc seeB1Release {x y} {
    variable text_w
    variable isMoveText
    variable didMultiPress

    if {$didMultiPress} {
      set didMultiPress 0
      return
    }

    searchCancel

    if { $isMoveText } {
      set range [$text_w tag ranges sel]
      if {[llength $range] > 0 && ![ispointerinselection $x $y]} {
	set ipos [$text_w index @$x,$y]
	set data [eval $text_w get $range]
	eval "$text_w delete $range"
	$text_w insert $ipos $data
	recolorize 1.0 end
      }
    } else {
      seeB1Motion $x $y
    }
    gat_updateMenuState
    $text_w configure -cursor xterm
    set isMoveText 0
  }

  #############################################################################
  #
  # Get the text for token
  #
  # Parameters:
  #     token		Token in the form  {type start stop}
  #
  proc getTokenText {token} {
    variable text_w

    return [$text_w get [lindex $token 1] [lindex $token 2]]
  }

  #############################################################################
  #
  # Get the first token on the same line as token (or one the specified line number)
  #
  proc getFirstToken {token} {
    if {[llength $token] == 3 && [scan [lindex $token 1] %d line] == 0 } {
    } elseif {[scan $token %d line] == 0} {
    } else {
      return ""
    }

    set token_list [tokenize $line]
    set N [llength $token_list]

    for { set i 0} { $i < $N} {incr i 3 } {
      set token [lrange $token_list $i [expr $i+2]]

      if { [lsearch -exact {linecomment blockcomment space} [lindex $token 0]] < 0} {
	return $token
      }
    }
    return ""
  }

  #############################################################################
  #
  # Loop through tokens back from the specified line
  #
  # Parameters:
  #     var		Variable to store current token at each iteration
  #     line		Line number to start at.
  #     body		Script to execute at each iteration.
  #
  # Scans backwards from tokens starting at the end of line 'line'.  At each
  # iteration, the token data is stored into 'var' in the form {type start stop}
  # and the body is executed in the context of the caller.  
  #
  proc fortokensbackfrom {var line body} {
    upvar $var _var

    while { $line > 0 } {
      set token_list [tokenize $line]
      set N [llength $token_list]

      for { set i [expr $N-3]} { $i >= 0} {incr i -3 } {
	set _var [lrange $token_list $i [expr $i+2]]

	#
	# Ignore comment and space tokens
	#
	if { [lsearch -exact {linecomment blockcomment space} [lindex $_var 0]] >= 0} {
	  continue
	}

	uplevel $body
      }
      incr line -1
    }
  }

  #############################################################################
  # 
  # Determine matching 'if' when indenting an 'else' statement.  This function
  # similar to getPrevTokens and is used as a replacement in a special case.
  # it is possible that these two functions can be merged at some time.
  #
  proc getIfMatch {line args} {
    variable text_w

    parseargs $args {-begin -ifmatch -prev}

    upvar $begin t_begin
    upvar $ifmatch t_ifmatch
    upvar $prev t_prev

    set t_begin ""
    set t_ifmatch ""
    set t_prev ""

    set level 0
    set if_level 0

    fortokensbackfrom token [expr $line - 1] {
      set token_text [getTokenText $token]

      if { $t_prev == ""} { 
	set t_prev $token
      }

      if {[isBeginToken $token_text]} {
	incr level -1
	if { $level < 0 } {
	  set t_begin $token
	  return 1
	}
      }
      if {[isEndToken $token_text]} {
	incr level
      }

      if { $level == 0 } {
	if {[isElseToken $token_text]} {
	  incr if_level
	}
	if {[isIfToken $token_text]} {
	  incr if_level -1
	}
	if { $if_level < 0 } {
	  set t_ifmatch [getFirstToken $token]
	  return 1
	}
      }

    }

    return 1
  }


  #############################################################################
  #
  # Determine various key tokens that determine the indentation of a line.  The
  # tokens returned are:
  #
  #    prev		Last token on previous line 
  #    begin		Enclosing begin token if found
  #    laststart	Last token that started a statement
  #
  # Options passed in are:
  #
  #    level		Level (begin..end) at which to start
  #
  proc getPrevTokens {line args} {
    variable text_w


    set did_ifstop 0
    set level 0
    set if_level 0
    parseargs $args {-prev -begin -laststart -level -isend}


    upvar $prev t_prev
    upvar $begin t_begin
    upvar $laststart t_laststart

    set t_prev ""
    set t_begin ""
    set t_laststart ""

    fortokensbackfrom token [expr $line - 1] {
      set token_text [getTokenText $token]

      #	if { $level == 0 && $t_prev != "" && [isStopToken $token_text]} {}
      if { $level == 0 && $t_prev != "" } {
	if { [isBeginToken $token_text] } { 
	  set t_begin $token
	  set t_laststart ""
	  return -code return 1
	}
	if { [isIfToken $token_text]  && !$isend} {
	  incr if_level -1
	  if {!$did_ifstop || ($if_level == 0 )} { 
	    set t_laststart [getFirstToken $token]
	    return -code return 1
	  }
	}
	if { [isStopToken $token_text] && !$isend} { 
	  if { [isElseToken [getTokenText $t_laststart]] && [isStopToken [getTokenText $t_prev]]} {
	    # this is the statement after and 'else' statement.  in this case we mimic
	    # and block end and do indentation based on the enclosing 'begin' token.
	    set isend 1
	  } else {
	    return -code return 1
	  }
	}
	if { [isElseToken $token_text] } { 
	  incr if_level
	}
      }

      if { $level == 0 && !$isend && ([isBeginToken $token_text] || [isStopToken $token_text]) && $if_level == 0} {
	set did_ifstop 1
      }

      if {[isBeginToken $token_text]} {
	incr level -1
	#	  set t_begin $token
      }
      if {[isEndToken $token_text]} {
	incr level
      }

      #
      # The first token we see is the "prev" token.
      #
      if { $t_prev == ""} { 
	set t_prev $token
	if {[isBeginToken $token_text]} { 
	  set t_begin $token
	  return -code return 1
	}
      }

      #
      # This token is now a candidate for the last start token
      #
      set t_laststart $token
    }
    return 1
  }

  ##############################################################################
  #
  # Debugging function to format a token for display
  #
  proc formattoken {token} {
    variable text_w

    if {[llength $token] == 0} {
      return "null"
    } else {
      return "$token \"[getTokenText $token]\""
    }
  }

  #############################################################################
  #
  # Compute the amount by which to indent the specified line
  #
  # Indentation Overview ---
  #
  #   Define "begin" tokens as those that begin a block (e.g., "begin", "module", "case", etc.)
  #   Define "end" tokens as those that end a block (e.g., "end", "endmodule", "endcase", etc.)
  #   Define "stop" tokens as those that are end tokens or a ";" token.
  #   Define "start" tokens as any token the follows a stop token
  #   Define "target line" as the line being indented
  #
  # Indentation is then performed according to the rule:
  #
  # if (target line begins with comment and previous line ends with "stop" token)
  #    find previous start token at the same level, and indent to position of that token.
  # else if (target line begins with comment and previous line does not end with "stop" token)
  #    find previous start token at the same level, and indent to position of that token plus one level.
  # else if (target line begins with an "end" token)
  #    find matching begin token and indent to the start of the line containing that token.
  # else if (target line begins with a "start" token)
  #    find previous start token at the same level, and indent to position of that token.
  # else
  #    find previous start token at the same level, and indent to position of that token plus one level.
  #
  proc getIndentLevel {line} {
    global tkg_hdlIndentLevel    
    variable text_w

    #
    # Get first token type on current line
    #
    if {[scan [tokenize $line] "%s %s %s" ttype start stop] != 3} {
      return 0
    }
    set token [$text_w get $start $stop]


    #
    # Mark flags based on type of first token on line
    #
    set target_is_else [isElseToken $token]
    set target_is_end [isEndToken $token]
    set target_is_comment [expr [lsearch -exact {linecomment blockcomment} $ttype] >= 0]



    #
    # Get the final token before this line (not counting comments).
    #
    if { $target_is_else } {
      if {[getIfMatch $line -prev prev -begin begin -ifmatch laststart] < 0} {
	return 0
      }
    } else {
      if {[getPrevTokens $line -isend $target_is_end -prev prev -begin begin -laststart laststart] < 0} {
	return 0
      }
    }

    if { $prev == "" } {
      return
    }
    set prev_token [getTokenText $prev]
    set prev_is_stop [isStopToken $prev_token]
    set prev_is_begin [isBeginToken $prev_token]



#    catch {
#      puts "getIndentLevel $line ---"
#      puts "   prev=[formattoken $prev]"
#      puts "   laststart=[formattoken $laststart]"
#      puts "   begin=[formattoken $begin]"
#      puts "   prev_is_stop=$prev_is_stop"
#      puts "   target_is_comment=$target_is_comment"
#      puts "   target_is_end=$target_is_end"
#    }

    if {[llength $begin] != 0 } {
      scan [lindex $begin 1] %*d.%d n

      if { $target_is_end } {
	return $n
      } else {
	return [expr $n + $tkg_hdlIndentLevel]
      }
    }

    if {[llength $laststart] != 0 } {
      scan [lindex $laststart 1] %*d.%d n

      if { $prev_is_stop } {
	return $n
      } else {
	return [expr $n + $tkg_hdlIndentLevel]
      }
    }

    return 0
  }

  #############################################################################
  #
  # Indent line by n spaces.
  #
  proc indentLine {line n} {
    variable text_w

    while { ([$text_w index $line.end] != "$line.0") && [string is space [$text_w get $line.0]] } {
      $text_w delete $line.0 
    }
    for {set i 0 } { $i < $n } { incr i } {
      $text_w insert $line.0 " "
    }
  }

  #############################################################################
  #
  # Indent the current line or the selected region.
  #
  proc doIndent {} {
    variable text_w

    if { [$text_w cget -state] != "normal" } return

    #
    # Get range of lines to indent.
    #
    if {[isselection]} {
      set ranges [$text_w tag ranges sel]
      if { [scan $ranges "%d.%*d %d.%*d" start_line stop_line] != 2 } return
      set selection_indent 1
    } else {
      if { [scan [$text_w index insert] %d lnum] < 1 } { return }
      set start_line $lnum
      set stop_line $lnum
      set selection_indent 0
    }

    for { set line $start_line} { $line <= $stop_line } { incr line } {
      set n [getIndentLevel $line]
      indentLine $line $n
    }
  }

  #############################################################################
  #
  # Apply a state (tag) to a range of characters.
  #
  proc colorizeToken {start stop args} {
    variable keywordList
    variable gatewordList
    variable text_w

#    dbgputs "  colorizeToken [$text_w get $start $stop] $args"

    set state keyword
    set canbeinst 0
    parseargs $args {-state -canbeinst}

    if {$state == "keyword"} {
      if { [$text_w get $start] == "\$" } {
	$text_w tag add taskword $start $stop
      } elseif { [lsearch -exact $keywordList [$text_w get $start $stop]] >= 0 } {
	$text_w tag add keyword $start $stop
      } elseif { [lsearch -exact $gatewordList [$text_w get $start $stop]] >= 0 } {
	$text_w tag add gateword $start $stop
      } elseif { $canbeinst } {
	$text_w tag add instword $start $stop
      }
    } elseif {$state != "normal"} {
      $text_w tag add $state $start $stop
    }
  }

  #############################################################################
  #
  # Get the words in a tokenized line
  #
  proc getTokenWords {line} {
    variable text_w

    set state [getLineTags $line]
    if {[lsearch -exact $state blockcomment] >= 0} {
      set state blockcomment
    } else {
      set state space
    }
    set tokens [tokenize $line -state  $state] 


    set L {}

    foreach {state start stop} $tokens {
      switch $state {
	blockcomment {}
	linecomment {}
	keyword { lappend L [$text_w get $start $stop] }
	string { lappend L [$text_w get $start $stop] }
	symbol { 
	  set s [$text_w get $start $stop]
	  set n [string length $s]
	  for { set i 0 } { $i < $n } { incr i } {
	    lappend L [string index $s $i]
	  }
	}
	space {}
      }
    }

    return $L
  }

  #############################################################################
  #
  # Helping function used by recolorize to find the tokens in a line.
  #
  proc tokenize {line args} {
    variable text_w

    set state ""
    parseargs $args {-state}

    #
    # If state is null or unspecified, infer state from first colored characters on line.
    #
    if { $state == "" } {
      set state [getLineTags $line]
      if {[lsearch -exact $state blockcomment] >= 0} {
	set state blockcomment
      } else {
	set state space
      }
    }

    set L {}

    set text [$text_w get $line.0 $line.end]
    set length [string length $text]

    if {$state == "blockcomment" } {
      lappend L blockcomment $line.0
    }

    set pc ""
    for {set p 0} {$p < $length} {incr p} {
      set c [string index $text $p]
      set c2 [string index $text [expr $p+1]]

      switch $state {
	space {
	  if {[string is space $c]} {
	  } elseif { $c == "\""} {
	    set state string
	    lappend L $state $line.$p
	  } elseif { "$c$c2" == "//" } {
	    set state linecomment
	    lappend L $state $line.$p
	    set p $length
	  } elseif { "$c$c2" == "/*" } {
	    set state blockcomment
	    lappend L $state $line.$p
	  } elseif {[iswordchar $c] || $c == "\$"} {
	    set state keyword
	    lappend L $state $line.$p
	  } else {
	    set state symbol
	    lappend L $state $line.$p
	  }
	}
	keyword {
	  if {[string is space $c]} {
	    lappend L $line.$p
	    set state space
	  } elseif { $c == "\""} {
	    lappend L $line.$p
	    set state string
	    lappend L $state $line.$p
	  } elseif { "$c$c2" == "//" } {
	    lappend L $line.$p
	    set state linecomment
	    lappend L $state $line.$p
	    set p $length
	  } elseif { "$c$c2" == "/*" } {
	    lappend L $line.$p
	    set state blockcomment
	    lappend L $state $line.$p
	  } elseif {![iswordchar $c] && $c != "\$"} {
	    lappend L $line.$p
	    set state symbol
	    lappend L $state $line.$p
	  }
	}
	symbol {
	  if {[string is space $c]} {
	    lappend L $line.$p
	    set state space
	  } elseif { $c == "\""} {
	    lappend L $line.$p
	    set state string
	    lappend L $state $line.$p
	  } elseif { "$c$c2" == "//" } {
	    lappend L $line.$p
	    set state linecomment
	    lappend L $state $line.$p
	    set p $length
	  } elseif { "$c$c2" == "/*" } {
	    lappend L $line.$p
	    set state blockcomment
	    lappend L $state $line.$p
	  } elseif {[iswordchar $c] || $c == "\$"} {
	    lappend L $line.$p
	    set state keyword
	    lappend L $state $line.$p
	  } else {
	    lappend L $line.$p
	    set state symbol
	    lappend L $state $line.$p
	  }
	}
	string {
	  if {$c == "\"" && $pc != "\\"} {
	    lappend L $line.[expr $p + 1]
	    set state space
	  }
	}
	blockcomment {
	  if { "$pc$c" == "*/" } {
	    lappend L $line.[expr $p + 1]
	    set state space
	  }
	}
      }

      set pc $c
    }

    if { [expr [llength $L] % 3] != 0 } {
      lappend L $line.end
    }

    if { $state != "blockcomment" } {
      lappend L space $line.end $line.end
    }

    return $L
  }

  #############################################################################
  #
  # Remove all colorization tags
  #
  proc removeTags {start stop} {
    variable text_w
    $text_w tag remove blockcomment $start $stop
    $text_w tag remove linecomment $start $stop
    $text_w tag remove keyword $start $stop
    $text_w tag remove taskword $start $stop
    $text_w tag remove gateword $start $stop
    $text_w tag remove instword $start $stop
    $text_w tag remove string $start $stop
  }

  #############################################################################
  #
  # Test for a blank line.
  #
  proc isBlank {line} {
    variable text_w

    set s [$text_w get $line.0 $line.end]
    set l [string length $s]
    for {set i 0} { $i < $l } { incr i } {
      if {![string is space [string index $s $i]]} {
	return 0
      }
    }
    return 1
  }

  #############################################################################
  #
  # Return state at end of line.
  #
  proc finalState {line} {
    variable text_w


    while { $line > 0 && [isBlank $line]}  { incr line -1 }

    #
    # If before first line, we are starting in space mode
    #
    if { $line < 1 } { return space }

    #
    # If line ends in */, then we must be in space mode
    #
    if { [$text_w get $line.end-2chars  $line.end] == "*/" } {
      return space
    }

    set tags [$text_w tag names $line.end-1chars]
    if {[lsearch -exact $tags blockcomment] >= 0 } {
      return blockcomment
    }

    return space
  }

  #############################################################################
  #
  # Get the current tag state on the line
  #
  proc getLineTags {line} {
    variable text_w

    scan [$text_w index $line.end] %*d.%d endc

    set tags {}
    for { set i 0 } { $i < $endc } { incr i } { 
      set tags [$text_w tag names $line.$i]
      if {[llength $tags] > 0} break
      
    }

    return $tags
  }


  #############################################################################
  #
  # Recolorize a region assuming that color state up to start is current.
  # Colorization is always performed a full line at a time so colorization will
  # begin at the beginning of the line that $start is on.
  #
  # Token types:
  #   keyword		Keywords such as "module", "while", "case", etc.
  #   taskword		System task names such as "$display".
  #   gateword		Words for built-in gates such as "and", "or", "xor", etc.
  #   instword		Words that are instance names.
  #   normal		Literals that do not fall into one of the above categories. 
  #   string		Strings delineated by '"' character.
  #   lineComment	Comments between "//" and the end of the line.
  #   blockComment	Comments between "/*" and "*/".
  #
  proc recolorize {start stop args} {
    variable text_w
    global tkg_hdlColorize
    global tkg_hdlIndentLevel

    if {[$text_w compare $start >= end]} return

    set start [$text_w index $start]
    set stop [$text_w index $stop]
    scan $start %d start_line
    scan $stop %d stop_line
    scan [$text_w index end] %d end_line

    if {! $tkg_hdlColorize } {
      removeTags $start.0 $stop.end
      return
    }

    #
    # Check previous line to detect starting state.
    #
    set state [finalState [expr $start_line - 1]]

    incr stop_line
    for {set line $start_line } { $line <= $stop_line } { incr line } {
      if { $line > $end_line} break

      set token_list [tokenize $line -state  $state]

      set before [getLineTags $line]

      removeTags $line.0 $line.end

      foreach {state startc stopc} $token_list {
	if { $state != "space" } {

	  set canbeinst 0
	  catch {
	    scan $startc %*d.%d x
	    if { [expr $x/$tkg_hdlIndentLevel] < 2 } {
	      set canbeinst 1
	    }
	  }

	  colorizeToken $startc $stopc -state $state -canbeinst $canbeinst
	}
      }
      set after [getLineTags $line]
      if { $before != $after || [llength $before] == 0 } { incr stop_line }
    }
  }

  #############################################################################
  #
  # Clear the target character 
  #
  proc clearTargetChar {} {
    variable targetChar
    set targetChar -1
  }

  #############################################################################
  #
  # Get the range of characters forming a word starting at a position.
  #
  proc getWord {index _start _stop} {
    variable text_w

    set index [$text_w index $index]

    upvar $_start start
    upvar $_stop stop

    if {![istaskwordchar [$text_w get $index]] && [istaskwordchar [$text_w get "$index + 1 char"]]} {
      set index [$text_w index "$index + 1char"]
    }

    set start $index
    set stop $index
    while {[$text_w compare $start > 1.0] && [istaskwordchar [$text_w get "$start - 1char"]]} {
      set start [$text_w index "$start - 1char"]
    }
    while {[$text_w compare $stop < end] && [istaskwordchar [$text_w get $stop]]} {
      set stop [$text_w index "$stop + 1char"]
    }
    return [$text_w get $start $stop]
  }

  #############################################################################
  #
  # See a keypress
  #
  proc seeKey {c} {
    variable text_w
    variable argument
    global tkg_hdlIndentReturn

    # We are still part of a search
    if {[searchContinue $c]} return

    # We are still part of an argument number 
    if {[argContinue $c]} return

    undoSelection
    if { ![string is print $c] && $c != "\r" && $c != "\n"} return
    if { $c == "\r" } { set c "\n" }

    set c [string repeat $c $argument]

    startCommand Typing

    set autoIndent 0
    if { $tkg_hdlIndentReturn && $c == "\n" } {
      if {[scan [$text_w index insert] %*d.%d cpos] == 1 && $cpos > 0 } {
	set autoIndent 1
      }
    }

    if { $autoIndent } {
      doIndent
    }

    set start [$text_w index insert]
    $text_w insert insert $c
    $text_w see insert

    recolorize $start $start

    if { $autoIndent} {
      doIndent
    }

    argCancel
  }


  #############################################################################
  #
  # Set the point for a selection
  #
  proc doSetPoint {} {
    variable text_w

    $text_w mark set point insert
    $text_w mark gravity point left
  }

  #############################################################################
  #
  # Swap the "point" and the cursor.
  #
  proc doPointSwap args {
    variable text_w
    variable argument

    set count $argument
    searchCancel
    if { ($count % 2) == 0 } return

    set nosearch 0
    parseargs $args {-nosearch}
    if { ! $nosearch } { searchCancel }

    if { [lsearch -exact [$text_w mark names] "point"] < 0} {
      return
    }

    set point  [$text_w index point]
    set insert [$text_w index insert]

    $text_w mark set point $insert
    $text_w mark set insert $point
    $text_w see insert
  }

  #############################################################################
  #
  # Move horizontally and return non-zero on success
  #
  proc doHMove {d} {
    variable text_w
    variable argument

    set d [expr $d * $argument]

    searchCancel

    set oldPos [$text_w index insert]
    $text_w mark set insert insert+${d}chars
    set newPos [$text_w index insert]
    $text_w see insert

    return [expr $oldPos != $newPos]
  }

  #############################################################################
  #
  # Cut the selection and copy it to the cut buffer
  #
  proc doCut {} {
    variable text_w

    startCommand -
    searchCancel
    action Cut gat_cutToBuf
    $text_w see insert
    recolorize insert insert
  }

  #############################################################################
  #
  # Copy the selection to the cut buffer
  #
  proc doCopy {} {
    startCommand -
    searchCancel
    action Copy { gat_copyToBuf }
  }

  #############################################################################
  #
  # Paste the contents of the cut buffer at the current location.
  #
  proc doYank {} {
    variable text_w

    startCommand -
    searchCancel

    set startIndex [$text_w index insert]

    HdlEditor::doSetPoint 
    action Yank gat_yankFromBuf
    $text_w see insert

    set endIndex [$text_w index insert]

    recolorize $startIndex $endIndex
  }

  #############################################################################
  #
  # Cancel a command in progress (i.e., respond to ^G).
  #
  proc doCancel {} {
    bell
    searchCancel -reset 1
  }

  #############################################################################
  #
  # Open a blank line below the current line.
  #
  proc doOpenLine {} {
    variable text_w
    variable argument

    set start [$text_w index insert]
    set s [string repeat "\n" $argument]

    searchCancel


    $text_w insert insert $s
    $text_w mark set insert $start
    $text_w see insert
    recolorize insert insert
  }

  #############################################################################
  #
  # Delete from the current position to the end of the line.
  #
  proc doDeleteWord {d} {
    variable text_w

    if {$d < 0} { set d -1 } else { set d 1}

    set oldinsert [$text_w index insert]
    doWordMove $d
    if { $d > 0 } {
      $text_w delete $oldinsert insert
    } else {
      $text_w delete insert $oldinsert 
    }
    recolorize insert insert
  }

  #############################################################################
  #
  # Move up or down a page
  #
  proc doPageMove {count} {
    variable text_w
    variable argument
    variable argument_active

    if {$argument_active} { 
      set count [expr $argument*$count]
    } else {
      set count [expr 22*$count]
    }

    searchCancel

    set bbox  [$text_w bbox insert]
    $text_w yview scroll $count units

    set last @[winfo width $text_w],[winfo height $text_w]
    if { $count > 0 } {
      if {[$text_w compare insert < @0,0] || [$text_w compare insert > $last]} {
	$text_w mark set insert @0,0
	$text_w see insert
      }
    } else {
      if {[$text_w compare insert < @0,0] || [$text_w compare insert > $last]} {
	$text_w mark set insert $last
	$text_w see insert
      }
    }
  }

  #############################################################################
  #
  # Update status message with current search state.
  #
  proc updateSearchStatus {} {
    variable search_string
    variable search_dir
    variable search_status
    global tkg_statusMessage

    if {$search_dir > 0 } {
      set tkg_statusMessage "${search_status}I-search: $search_string"
    } else {
      set tkg_statusMessage "${search_status}I-search backwards: $search_string"
    }
  }

  #############################################################################
  #
  # Initiate a reverse search, or search backwards for the next occurance.
  #
  proc doRSearch {} {
    variable text_w
    variable last_search
    variable search_string
    variable search_dir
    variable search_status

    set search_dir -1
    set id [KeyBinding::getKeyCount $text_w]
    if { $id != [expr $last_search + 1] } {
      startCommand -

      set last_search $id
      doSetPoint
      set search_string ""
      set search_status ""
      updateSearchStatus
    } else {
      set last_search $id
      set pos [$text_w search -backwards $search_string insert]
      if {$pos < 0 } {
	if { $search_status == "" } bell
	set search_status "Failing "
	updateSearchStatus
	return
      }
      $text_w mark set insert ${pos}
      undoSelection
      $text_w tag add sel insert insert+[string length $search_string]chars
    }
    set last_search $id
  }

  #############################################################################
  #
  # Select entire text
  #
  proc selectAll {} {
    variable text_w

    $text_w tag add sel 1.0 end
  }

  #############################################################################
  #
  # Initiate a search, or search for the next occurance.
  #
  proc doSearch {} {
    variable text_w
    variable last_search
    variable search_string
    variable search_dir
    variable search_status

    dbgputs doSearch

    set search_dir 1
    set id [KeyBinding::getKeyCount $text_w]
    if { $id != [expr $last_search + 1] } {
      startCommand -

      set last_search $id
      doSetPoint
      set search_string ""
      set search_status ""
      updateSearchStatus
    } else {
      set last_search $id
      set pos [$text_w search $search_string insert]
      if {$pos < 0 } {
	if { $search_status == "" } bell
	set search_status "Failing "
	updateSearchStatus
	return
      }
      $text_w mark set insert ${pos}+[string length $search_string]chars
      undoSelection
      $text_w tag add sel insert-[string length $search_string]chars insert
      $text_w see insert
    }
  }

  #############################################################################
  #
  # Delete the last character in the incremental search buffer.
  #
  proc searchDelete {} {
    variable text_w
    variable last_search
    variable search_string
    variable search_dir
    variable search_status

    set id [KeyBinding::getKeyCount $text_w]
    if { $id == [expr $last_search + 1] } {
      set last_search $id

      if {$search_string == ""} { return 1 }
      set search_string [string range $search_string 0 [expr [string length $search_string] - 2]]

      if {$search_dir > 0} {
	set stext [$text_w get insert-[string length $search_string]chars insert]
      } else {
	set stext [$text_w get insert insert+[string length $search_string]chars]
      }

      if { $stext == $search_string } {
	set search_status ""
      }

      updateSearchStatus
      return 1
    }

    return 0
  }

  #############################################################################
  #
  # Add the character $c to the incremental search string if there is an active
  # search in progress.  Returns 0 if there was no active search and 1 if there
  # was an active search.  This procedure is normally called by seeKey which
  # will check here first to see if there is a search in progress before doing
  # normal character handling.
  #
  proc searchContinue {c} {
    variable text_w
    variable last_search
    variable search_string
    variable search_dir
    variable search_status

    set id [KeyBinding::getKeyCount $text_w]
    if { $id == [expr $last_search + 1] } {
      set last_search $id

      if {![string is print $c]} {
	bell
	searchCancel -reset 1 -noargcancel 1
	return 1
      }

      set search_string $search_string$c

      if { $search_dir > 0 } {
	updateSearchStatus
	set pos [$text_w search $search_string insert-[expr [string length $search_string] - 1]chars]
	if {$pos < 0 } {
	  if { $search_status == "" } bell
	  set search_status "Failing "
	  updateSearchStatus
	  return 1
	}
	$text_w mark set insert ${pos}+[string length $search_string]chars
	undoSelection
	$text_w tag add sel insert-[string length $search_string]chars insert
	$text_w see insert
      } else {
	updateSearchStatus
	set pos [$text_w search -backwards $search_string insert+[string length $search_string]chars]
	if {$pos < 0 } {
	  if { $search_status == "" } bell
	  set search_status "Failing "
	  updateSearchStatus
	  return 1
	}
	$text_w mark set insert ${pos}
	undoSelection
	$text_w tag add sel insert insert+[string length $search_string]chars
	$text_w see insert
      }
      return 1
    } else {
      searchCancel -noargcancel 1
      return 0
    }
  }

  #############################################################################
  #
  # End a search (and argument count)
  #
  proc searchCancel args {
    variable last_search
    variable search_string
    variable text_w
    global tkg_statusMessage

    set noargcancel 0
    set reset 0
    parseargs $args {-reset -noargcancel}

    clearTargetChar

    if { !$noargcancel} {argCancel }

    set tkg_statusMessage ""
    set last_search -1

    if { $reset } { 
      undoSelection
      doPointSwap -nosearch 1
    }
#    $text_w tag remove sel 1.0 end
  }

  proc argCancel args {
    variable argument_active
    variable argument_string
    variable argument

    set argument_active 0
    set argument_string ""
    set argument 1
  }


  #############################################################################
  #
  # Returns non-zero if we are still entering a count/argument
  #
  proc argContinue {c} {
    variable argument_active
    variable argument_string
    variable argument
    global tkg_statusMessage

    if {!$argument_active} { return 0 }

    if {[string is digit $c]} {
      set argument_string "$argument_string$c"
      if {[scan $argument_string %d argument] != 1 } { set argument 1 }
      if { $argument > 500 } { set argument 500 }
      set tkg_statusMessage "Argument: $argument_string"
      return 1
    } else {
      set argument_active 0
      set argument_string ""
      return 0
    }
  }


  #############################################################################
  #
  # Transpose the two characters around the cursor and advance the cursor
  #
  proc doTransposeChar {} {
    variable text_w
    variable argument

    set count $argument
    searchCancel

    set start [$text_w index insert]

    while { $count > 0 } {
      #
      # Can't do this operation if cursor is a front or end of buffer.
      #
      if { [$text_w compare insert == "1.0"] || [$text_w compare "insert + 1chars" == end]} break

      $text_w mark set insert "insert-1chars"

      set c1 [$text_w get insert]
      set c2 [$text_w get "insert+1chars"]

      $text_w delete insert "insert+2chars"
      $text_w insert insert $c2$c1

      incr count -1
    }

    recolorize $start $start
  }

  #############################################################################
  #
  # Impelements the kill line command.
  #
  proc doKillLine {} {
    variable text_w
    variable last_kill
    variable argument


    set count $argument
    searchCancel

    set ins   [$text_w index insert]
    set pos   [$text_w index insert]
    set end   [$text_w index end]
    while { $count > 0 } {

      if { [$text_w index "$ins + 1 chars"] == $end } break

      if { [$text_w get $pos] == "\n" } {
	#
	# If insertion cursor is at a newline, advance end pos by one char
	#
	set pos [$text_w index "$pos + 1 chars"]
      } else {
	#
	# Advance pos to next newline
	#
	while { $pos != $end } {
	  set c   [$text_w get $pos]
	  if { $c == "\n" } break
	  set pos [$text_w index "$pos + 1 chars"]
	}
      }
      incr count -1
    }

    #
    # Do the actual kill here
    #
    undoSelection
    set id [KeyBinding::getKeyCount $text_w]
    if { $id != [expr $last_kill + 1] } {
      $text_w tag add sel $ins $pos
      action Cut gat_cutToBuf
    } else {
      $text_w tag add sel $ins $pos
      action Cut gat_cutToBufAppend
    }
    $text_w see insert
    set last_kill $id
    recolorize insert insert
  }


  #############################################################################
  #
  # Move veritically and return non-zero on success
  #
  proc doVMove {d} {
    variable text_w
    variable argument
    variable targetChar

    set d [expr $d * $argument]
    set target $targetChar

    searchCancel

    # Save old position
    set oldPos [$text_w index insert]

    # If no target column, then set target column to our column.
    if { $target < 0 } {
      scan $oldPos %*d.%d target
    }

    # Move the cursor and try to keep it in the target column
    $text_w mark set insert insert-${d}lines
    scan [$text_w index insert] %d lineno
    $text_w mark set insert $lineno.$target
    set targetChar $target


    # Make sure cursor is visible
    $text_w see insert

    # Get new position
    set newPos [$text_w index insert]

    # Return non-zero if position changed.
    return [expr $oldPos != $newPos]
  }

  #############################################################################
  #
  # Move to a specified position
  #
  proc doMoveTo {pos args} {
    variable text_w

    searchCancel

    set setpoint 0
    parseargs $args {-setpoint}
    if { $setpoint } { doSetPoint }

    set oldPos [$text_w index insert]
    $text_w mark set insert "$pos"
    set newPos [$text_w index insert]
    $text_w see insert

    return [expr $oldPos != $newPos]
  }

  proc doSetArgument {} {
    variable argument_active
    variable argument_string
    variable argument
    global tkg_statusMessage

    set argument_active 1
    set argument_string ""
    set argument 4
    set tkg_statusMessage "Argument: "
  }

  proc doLowercaseWord {} {
    variable text_w
    variable argument

    set count $argument
    searchCancel

    set start0 [$text_w index insert]

    while {$count > 0} {
      set start [$text_w index insert]
      if {![doWordMove 1]} break
      set stop [$text_w index insert]
      for {set i $start } { [$text_w compare $i < $stop] } { set i [$text_w index "$i +1chars"]} {
	set c [$text_w get $i]
	if { [string is upper $c] } {
	  $text_w delete $i
	  $text_w insert $i [string tolower $c]
	}
      }
      incr count -1
    }

    recolorize $start0 $start0
  }

  proc doUppercaseWord {} {
    variable text_w
    variable argument

    set count $argument
    searchCancel

    set start0 [$text_w index insert]

    while {$count > 0} {
      set start [$text_w index insert]
      if {![doWordMove 1]} break
      set stop [$text_w index insert]
      for {set i $start } { [$text_w compare $i < $stop] } { set i [$text_w index "$i +1chars"]} {
	set c [$text_w get $i]
	if { [string is lower $c] } {
	  $text_w delete $i
	  $text_w insert $i [string toupper $c]
	}
      }
      incr count -1
    }

    recolorize $start0 $start0
  }

  proc doCasefyWord {} {
    variable text_w
    variable argument

    set count $argument
    searchCancel

    set start0 [$text_w index insert]

    while {$count > 0} {
      set start [$text_w index insert]
      if {![doWordMove 1]} break
      set stop [$text_w index insert]

      if {[$text_w compare $start == $stop]} {
	break
      }

      for {set i $start } { [$text_w compare $i < $stop] } { set i [$text_w index "$i +1chars"]} {
	set c [$text_w get $i]
	if { [string is alnum $c] } {
	  if { [string is lower $c] } {
	    $text_w delete $i
	    $text_w insert $i [string toupper $c]
	  }
	  break
	}
      }

      for {set i [$text_w index "$i +1chars"] } { [$text_w compare $i < $stop] } { set i [$text_w index "$i +1chars"]} {
	set c [$text_w get $i]
	if { [string is upper $c] } {
	  $text_w delete $i
	  $text_w insert $i [string tolower $c]
	}
      }
      incr count -1
    }

    recolorize $start0 $start0
  }

  #############################################################################
  #
  # Move forward/backward a word.
  #
  proc doWordMove {d} {
    variable text_w
    variable argument

    set count $argument
    searchCancel

    if {$d < 0} { set d -1 } else { set d 1}

    while { $count > 0 } {
      if {$d < 0 } { doHMove -1 }
      while {1} {
	set c [$text_w get insert]
	if { [string is alnum $c] } break
	if {![doHMove $d]} { return 0 }
      }

      while {1} {
	set c [$text_w get insert]
	if {![string is alnum $c] } break
	if {![doHMove $d]} { return 0 }
      }
      if {$d < 0 } { doHMove 1 }

      incr count -1
    }
    return 1
  }

  #############################################################################
  #
  # See a press of the delete key
  #
  proc doDelete {d} {
    variable text_w
    variable argument

    set count $argument

    #
    # If this is a backspace delete and there is an active search, do the
    # search related command instead of an actual delete.
    #
    if {$d < 0 && [searchDelete]} return

    searchCancel

    set range [$text_w tag ranges sel]
    if { [llength $range] > 0} {
      eval "$text_w delete $range"
      return
    }

    if { $d < 0 } {
      set stop [$text_w index insert]
      if { ! [doHMove [expr $count*$d]]} return
      set start [$text_w index insert]
      $text_w delete $start $stop
      $text_w mark set insert $start 
    } elseif { $d > 0 } {
      set start [$text_w index insert]
      if { ! [doHMove [expr $count*$d]]} return
      set stop [$text_w index insert]
      $text_w delete $start $stop
      $text_w mark set insert $start 
    } else {
      $text_w delete insert "insert + $count chars"
    }
    # This mean delete the character after the insertion cursor.
 
    recolorize insert insert
  }

  #############################################################################
  #
  # got the focus
  #
  proc focusIn {hw tw} {
    variable hdl_w
    variable text_w
    variable isFocused

    set isFocused 1
    set hdl_w $hw
    set text_w $tw
  }

  #############################################################################
  #
  # lost the focus
  #
  proc selectMain {} {
    variable main_w
    variable hdl_w
    variable text_w
    set hdl_w $main_w
    set text_w $main_w.pane.main.t
  }

  proc focusOut {} {
    variable isFocused

    set isFocused 0
    selectMain
  }

  #############################################################################
  #
  # Build the main window in the HDL text editor
  #
  proc buildMainEdit {w args} {
    global tkg_pastelHighlight
    global tkg_hdlCommentColor tkg_hdlCommentBold tkg_hdlCommentItalic
    variable text_w


    set width ""
    set height ""
    parseargs $args {-width -height}

    dbgputs HdlEditor::buildMainEdit

    set text_w $w.t

    frame $w
    text $w.t -yscrollcommand "$w.vb set" -bg white -font hdlFont
    scrollbar $w.vb -command "$w.t yview" -takefocus 0 
    pack $w.t -side left -fill both -expand 1 -pady 5
    pack $w.vb -side right -fill y -pady 5

    if { $width != "" } { $text_w configure -width $width }
    if { $height != "" } { $text_w configure -height $height }

    $text_w tag configure highlight -background $tkg_pastelHighlight
    configureTag blockcomment hdlComment
    configureTag linecomment hdlComment
    configureTag keyword hdlReserved
    configureTag gateword hdlGateWord
    configureTag instword hdlInstWord
    configureTag string hdlString

    $text_w tag lower keyword
    $text_w tag raise highlight
  }

  #############################################################################
  #
  # Create the HDL text editor
  #
  proc new {w args} {
    variable hdl_w
    variable text_w
    variable main_w

    set ismain 0
    set width ""
    set height ""
    set keygroup hdlmodule
    parseargs $args {-ismain -width -height -keygroup}
    if {$ismain} {
      set main_w $w
    }

    set hdl_w $w

    frame $w

    frame $w.pane
    pack $w.pane -fill both -expand 1

    buildMainEdit $w.pane.main -width $width -height $height

    #
    # Do initialization of bindings
    #
    bindtags $text_w [list $text_w]
    bind $text_w <1> { focus %W; HdlEditor::seeB1Press %x %y }
    bind $text_w <Double-1> { HdlEditor::seeB1DoublePress %x %y }
    bind $text_w <Triple-1> { HdlEditor::seeB1TriplePress %x %y }
    bind $text_w <B1-Motion> { HdlEditor::seeB1Motion %x %y }
    bind $text_w <ButtonRelease-1> { HdlEditor::seeB1Release %x %y }
    bind $text_w <3> { HdlEditor::seeB3Press %x %y %X %Y }
    bind $text_w <FocusIn> "HdlEditor::focusIn $hdl_w $text_w"
    bind $text_w <FocusOut> "HdlEditor::focusOut"
    bind $text_w <Destroy> "HdlEditor::focusOut"
    KeyBinding::listener $text_w $keygroup -unmatchedcommand HdlEditor::seeKey

    pack $w.pane.main -expand 1 -fill both
  }
}

#############################################################################
#
# NOT WORKING AND NOT CURRENTLY USED.
# 
# Selection handling - allows cut and paste from external windows.  This is
# mostly implemented to allow cut and paste into the hdl editor, but it will
# also allow cut and paste into netlist windows as well.
#
namespace eval Selection {
  proc handle args {
    dbgputs "Selection::handle $args -- [selection get]"
  }

  proc lost args  {
    dbgputs "Selection::lost $args  -- [selection own]"
  }

  proc take args  {
    return
    dbgputs "Selection::take $args"
    selection own -command Selection::lost .
  }

  proc init {} {
    dbgputs "Selection::init"
    selection handle  . { dbgputs abc1 }
    Selection::showOwner
  }

  proc showOwner {} {
    dbgputs owner=[selection own]
    after 1000 Selection::showOwner
  }
}

#bind $w <Shift-KeyPress-Insert> " if { \[catch {selection get} sel\]==0 } { foreach c \[split \$sel \"\"\] { scan \$c %%c c; Simulator::cmdSend \"command $tkg_ttygat($w) key \$c\" } } "
