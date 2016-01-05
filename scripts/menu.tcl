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
# Last edit by hansen on Sat Jan 31 08:14:59 2009

#############################################################################
#
# This file contains tcl scripts for creating and managing menus (except
# popup menus). 
#
# Menu states:
#       edit			Normal edit mode is active
#	interface-all		Top-level interface screen
#       interface-block		Interface editor on a normal block is active
#       interface-symbol	Interface editor on a symbol block is active
#	simulate		Simulator is active
#
# Special Flags:
#	D	Create only if debug mode is active
#	X	If mode guard failed, do not create entry
#
# Base Flags:
#	C	Active only if cutable object is selected
#	P	Active only if pasteable object in cut buffer
#	U	Active only if undoable actions exist
#	R	Active only if redoable actions exist
#	b	Symbol bitmap operations are active
#	p	Symbol port operations are active
#
#
namespace eval Menu {
  variable accelTable
  variable state edit
  variable menuId
  variable flags
  array set flags {D 0 C 0 P 0 U 0 R 0 X 0}

  variable baseFlags {C P R U b p}
  variable baseFlagEntries

  variable editorMenus {file edit tool simulate format module make help}
  variable scopeMenus {scope_file scope_sim scope_trace scope_help}

  #  Name	func.			Flags	Mode
  variable menuButtonData {
    {file	-			-	*}
    {edit	-			-	*}
    {tool	-			-	{edit interface-*}}
    {simulate	-			-	{simulate}}
    {module	-			-	*}
    {format	-			-	edit}
    {make	gat_makeMakeMenu	-	edit}
    {help	-			-	*}
    {scope_file	-			-	*}
    {scope_sim	-			-	*}
    {scope_trace -			-	*}
    {scope_help	-			-	*}
  }


  #
  # This table contains information about all menu data.  The entry names should have
  # the form "name.ent" where "name" defines which menu it should belong to.  The text
  # for the menu is taken from the message "menu.name.ent" (e.g., for "file.new", the
  # message tag will be "menu.file.new")
  #
  #  Entry Name		Action		Image		Sel. Image	Var.	Flags	Mode
  variable menuEntryData {
    {file.new		newFile		file_new	-		-	-	*}
    {file.open		loadFile	file_open	-		-	-	*}
    {file.save		saveFile	file_save	-		-	-	*}
    {file.saveas	saveAsFile	file_saveas	-		-	-	*}
    {file.separator	-		-		-		-	-	*}
    {file.print		print		file_print	-		-	-	*}
    {file.separator	-		-		-		-	-	*}
    {file.openlib	loadLibrary	file_lib	-		-	-	*}
    {file.cprop		editCProps	i_circprops	-		-	-	*}
    {file.separator	-		-		-		-	X	interface-symbol}
    {file.import	importImage	symed_import	-		-	X	interface-symbol}
    {file.export	exportImage	symed_export	-		-	X	interface-symbol}
    {file.separator	-		-		-		-	-	*}
    {file.quit		exit		emptytool	-		-	-	*}

    {edit.undo		undo		back		-		-	U	{edit interface-*}}
    {edit.redo		redo		forward		-		-	R	{edit interface-*}}
    {edit.separator	-		-		-		-	X	{edit interface-symbol}}
    {edit.cut		cutToBuf	edit_cut	-		-	XC	{edit}}
    {edit.copy		copyToBuf	edit_copy	-		-	XC	{edit}}
    {edit.paste		yankFromBuf	edit_paste	-		-	XP	{edit}}
    {edit.cut		seCutToBuf	edit_cut	-		-	XC	{interface-symbol}}
    {edit.copy		seCopyToBuf	edit_copy	-		-	XC	{interface-symbol}}
    {edit.paste		seYankFromBuf	edit_paste	-		-	XP	{interface-symbol}}
    {edit.overlay	seOverlayFromBuf edit_overlay	-		-	XP	{interface-symbol}}
    {edit.selall	selectAll	emptytool	-		-	X	edit}
    {edit.find		findObject	edit_find	-		-	X	{edit simulate}}

    {edit.separator	-		-		-		-	X	{edit simulate}}
    {edit.zoomin 	zoomIn		zoom_in		-		-	X	{edit simulate}}
    {edit.zoomout	zoomOut		zoom_out	-		-	X	{edit simulate}}
    {edit.separator	-		-		-		-	X	interface-symbol}
    {edit.zoomin 	seZoomIn	zoom_in		-		-	X	interface-symbol}
    {edit.zoomout	seZoomOut	zoom_out	-		-	X	interface-symbol}

    {edit.separator	-		-		-		-	X	interface-symbol}
    {edit.rshift	seRShiftBits	arrow0		-		-	X	interface-symbol}
    {edit.lshift	seLShiftBits	arrow180	-		-	X	interface-symbol}
    {edit.ushift	seUShiftBits	arrow90		-		-	X	interface-symbol}
    {edit.dshift	seDShiftBits	arrow270	-		-	X	interface-symbol}
    {edit.cwrotate	seCWRotate	symed_cwrotate	-		-	X	interface-symbol}
    {edit.ccwrotate	seCCWRotate	symed_ccwrotate	-		-	X	interface-symbol}

    {edit.separator	-		-		-		-	X	interface-symbol}
    {edit.pcwrotate	seCWRotPort	symed_pcrot	-		-	X	interface-symbol}
    {edit.pccwrotate	seCCWRotPort	symed_pccrot	-		-	X	interface-symbol}

    {edit.separator	-		-		-		-	X	interface-symbol}
    {edit.autobold	seBoldBits	symed_autobold	-		-	X	interface-symbol}

    {tool.move		editMode	mov_curs	mov_curs_S	mode=1	X	{edit interface-all interface-block}}
    {tool.cutw		cutMode		cut_curs	cut_curs_S	mode=0	X	{edit interface-all interface-block}}
    {tool.inv		invertMode	inv_curs	inv_curs_S	mode=2	X	edit}
    {tool.bitw		sizeMode	size_curs	size_curs_S	mode=9	X	{edit interface-all interface-block}}
    {tool.debug		debugMode	bug_curs	bug_curs_S	mode=12	XD	edit}

    {tool.point		null		symed_point	symed_point_S	SymbolEdit::emode=point	 X	interface-symbol}
    {tool.line		null		symed_line	symed_line_S	SymbolEdit::emode=line	 X	interface-symbol}
    {tool.rect		null		symed_rect	symed_rect_S	SymbolEdit::emode=rect	 X	interface-symbol}
    {tool.fillrect	null		symed_fillrect	symed_fillrect_S SymbolEdit::emode=fillrect X	interface-symbol}
    {tool.port		null		symed_port	symed_port_S	SymbolEdit::emode=port	 X	interface-symbol}
    {tool.select	null		symed_select	symed_select_S	SymbolEdit::emode=select X	interface-symbol}


    {tool.separator	-		-		-		-	X	edit}
    {tool.rep		replicate	replicate	-		-	-	*}
    
    {tool.separator	-		-		-		-	-	*}
    {tool.options	editOptions	emptytool	-		-	-	*}
    {tool.tclshell	shellWindow	emptytool	-		-	D	*}

    {simulate.begin	startSimulator	simstart	-		-	X	{edit interface-*}}
    {simulate.end	endSimulator	simstart	-		-	X	simulate}
    {simulate.separator	-		-		-		-	X	simulate}
    {simulate.run	simRun		sim_go		-		-	X	simulate}
    {simulate.pause	simStop		sim_stop	-		-	X	simulate}
    {simulate.step	simStep		sim_step	-		-	X	simulate}
    {simulate.cycle	simCycleOrRotate sim_clock	-		-	X	simulate}
    {simulate.break	editBreakpoints	sim_break	-		-	X	simulate}
    {simulate.script	doSimScript	sim_script	-		-	X	simulate}
    {simulate.separator	-		-		-		-	X	simulate}
    {simulate.load	simLoadMem	sim_load	-		-	X	simulate}
    {simulate.dump	simDumpMem	sim_dump	-		-	X	simulate}
    {simulate.view	simViewMem	sim_view	-		-	X	simulate}
    {simulate.separator	-		-		-		-	X	simulate}
    {simulate.addprobe	toggleProbe	emptytool	-		-	X	simulate}

    {module.open	openMod		blk_open	-		-	-	*}
    {module.close	closeMod	blk_close	-		-	-	*}
    {module.separator	-		-		-		-	X	edit}
    {module.interface	-cascade	editintr	-		-	X	edit}
    {module.interface.edit editBlockDesc -		-		-	-	*}
    {module.interface.set setBlockDesc	-		-		-	-	*}
    {module.interface.update updateInterface	-	-		-	-	*}
    {module.interface.updateall updateAllInterface -	-		-	-	*}
    {module.interface.auto autoGenerateCanvas	-	-		-	-	*}

    {module.separator	-		-		-		-	X	{edit interface-*}}
    {module.new		blockNew	blk_new		-		-	X	{edit interface-*}}
    {module.del		blockDelete	blk_delete	-		-	X	{edit interface-*}}
    {module.copy	blockCopy	blk_copy	-		-	X	{edit interface-*}}
    {module.rename	blockRename	blk_rename	-		-	X	{edit interface-*}}
    {module.claim	blockClaim	blk_claim	-		-	X	{edit interface-*}}
    {module.setroot	blockSetRoot	blk_root	-		-	X	{edit interface-*}}
    {module.prop	blockProp	i_modprops	-		-	X	{edit interface-*}}

    {format.addport	addPort		addport		-		-	-	*}
    {format.separator	-		-		-		-	-	*}
    {format.anchor	anchor		anchor		-		-	-	*}
    {format.unanchor	unAnchor	unanchor	-		-	-	*}
    {format.separator	-		-		-		-	X	edit}
    {format.algnv	vAlign		edit_valgn	-		-	X	edit}
    {format.algnh 	hAlign		edit_halgn	-		-	X	edit}
    {format.separator	-		-		-		-	-	*}
    {format.settech	-cascade	emptytool	-		-	-	*}
    {format.rot		-cascade	rotation0	-		-	X	edit}
    {format.rot.0	rot0		rotation0	rotation0_S	rot=0	X	edit}
    {format.rot.90	rot90		rotation90	rotation90_S	rot=1	X	edit}
    {format.rot.180	rot180		rotation180	rotation180_S	rot=2	X	edit}
    {format.rot.270	rot270		rotation270	rotation270_S	rot=3	X	edit}
    {format.rot.separator -separator	-		-		-	-	edit}
    {format.rot.rotate	simCycleOrRotate edit_rotate	-		-	X	edit}
    {format.rot.brotate	backRotate	 edit_brotate	-		-	X	edit}
    {format.prop	editProps	i_gateprops	-		-	-	*}

    {help.about		showAbout	about		-		-	-	*}
    {help.license	showLicense	gnuhead		-		-	-	*}
    {help.doc		showDocumentation helpdoc	-		-	-	*}
    {help.separator	-		-		-		-	-	*}
    {help.home		loadWelcome	helpdoc		-		-	-	edit}
    {help.tut		loadTutorial	helpdoc		-		-	-	edit}
    {help.example	loadExample	helpdoc		-		-	-	edit}

    {scope_file.ptrace	printTrace	file_print	-		-	-	*}
    {scope_file.close	endSimulator	simstart	-		-	-	*}

    {scope_sim.run	simRun		sim_go		-		-	-	*}
    {scope_sim.pause	simStop		sim_stop	-		-	-	*}
    {scope_sim.step	simStep		sim_step	-		-	-	*}
    {scope_sim.cycle	simCycleOrRotate sim_clock	-		-	-	*}
    {scope_sim.break	editBreakpoints	sim_break	-		-	-	*}
    {scope_sim.script	doSimScript	sim_script	-		-	-	*}
    {scope_sim.separator -		-		-		-	-	*}
    {scope_sim.load	simLoadMem	sim_load	-		-	-	*}
    {scope_sim.dump	simDumpMem	sim_dump	-		-	-	*}
    {scope_sim.separator -		-		-		-	-	*}
    {scope_sim.addprobe	toggleProbe	emptytool	-		-	-	*}

    {scope_trace.zoomin	scopeZoomIn	zoom_in		-		-	-	*}
    {scope_trace.zoomout scopeZoomOut	zoom_out	-		-	-	*}

    {scope_help.about	showAbout	about		-		-	-	*}
    {scope_help.license	showLicense	gnuhead		-		-	-	*}
    {scope_help.doc	showDocumentation helpdoc	-		-	-	*}
  }

  proc menuEntry {m cLabel args} {
    global menuCommandTable
    variable accelTable

    set command ""
    set radio 0
    set state normal
    set variable ""
    set value ""
    set image ""
    set selectimage ""
    set menu ""
    parseargs $args {-command -state -variable -value -radio -image -selectimage -menu}

    parseName $cLabel label underline

    set accelerator ""
    if {[info exists accelTable($command)]} {
      set accelerator $accelTable($command)
    }

    set index ""

    #
    # The -image, -selectimage and -compound options are not supported before Tk 8.4, so
    # we must check for an error and be ready to generate menu entries without symbols.
    #
    if {$menu != ""} {
      safeeval $::menu_forbidden $m add cascade -label $label -underline $underline -menu $menu \
				      -state $state -image $image -compound left
      set index [$m index end]
    } elseif {$radio} {
      if { $image != "" } {
	safeeval $::menu_forbidden $m add radiobutton -label $label -underline $underline -command $command \
			-state $state -variable $variable -value $value \
			-image $image -selectimage $selectimage -compound left -indicatoron 0 -accelerator $accelerator
	set index [$m index end]
      } else {
	$m add radiobutton -label $label -underline $underline -command $command \
	    -state $state -variable $variable -value $value -accelerator $accelerator 
	set index [$m index end]
      }
    } else {
      safeeval $::menu_forbidden $m add command -label $label -underline $underline -command $command \
		      -state $state -image $image -compound left -accelerator $accelerator
      set index [$m index end]
    }

    #
    # This is a kludge to exclude popup menus.
    #
    if {! [string match ".pop_*" $m ] } {
      set idx [$m index end]
      lappend menuCommandTable($command) ${idx}:$m
    }

    return $index
  }

  proc parseName {cName _name _ul} {
    upvar $_name name
    upvar $_ul ul

    set name 	""
    set ul	-1
    if { [scan $cName "%d:%\[^\n\]" ul name] != 2 } {
      set name $cName
    }
  }

  #############################################################################
  #
  # Rebuild the menus for $tag.
  #
  proc rebuildMenu {tag} {
    variable menuId

    foreach m $menuId($tag) {
      if {$tag == "make"} continue
      $m delete 0 end
      foreach sm [winfo children $m] {
	$sm delete 0 end
      }
      makeMenu $m $tag
    }
  }

  proc rebuildAll {} {
    variable menuButtonData
    variable menuId
    variable state
    variable baseFlags
    variable baseFlagEntries

    #
    # Clear the base flags 
    #
    foreach f $baseFlags {
      set baseFlagEntries($f) {}
    }

    foreach b $menuButtonData {
      set name [lindex $b 0]
      set func [lindex $b 1]
      set flags [lindex $b 2]
      set gstate [lindex $b 3]

      set itemstate disabled
      foreach x $gstate {
	if {[string match $x $state]} {
	  set itemstate normal
	}
      }

      foreach m $menuId($name) {
	catch { [winfo parent $m] configure -state $itemstate }
      }

      if {$itemstate == "normal" } {
	catch { rebuildMenu $name }
      }
    }
  }



  #############################################################################
  #
  # Make the menu for $tag in the menu $w
  #
  proc makeMenu {m tag} {
    variable menuEntryData
    variable menuButtonData
    variable menuId
    variable flags
    variable state
    variable baseFlags
    variable baseFlagEntries

    #
    # Register the menu.
    #
    if {![info exists menuId($tag)]} {set menuId($tag) {}}
    if {[lsearch $menuId($tag) $m] < 0} {
      lappend menuId($tag) $m
    }

    #
    # If a custom creation function is specified, use that function and return.
    #
    set button [assocg $tag  $menuButtonData]
    if {[lindex $button 1] != "-"} {
      [lindex $button 1] $m
      return
    }

    #
    # Create the menu entries
    #
    foreach entry $menuEntryData {
      set name ""
      set action ""
      set image ""
      set simage ""
      set varval ""
      lscan $entry name action image simage varval gflags gstate

      #
      # Seprate name into chars before last "." and chars after last "."
      #
      set lname [string map {. " "} $name]
      set mname [lindex $lname 0]
      set ename [lindex $lname 1]

      if {$mname != $tag } continue

      #
      # Check to see if we are disabled due to a guarded state
      #
      set itemstate disabled
      foreach x $gstate {
	if {[string match $x $state]} {
	  set itemstate normal
	}
      }

      #
      # If we are disabled due to a guarded state and the X flag is set, then ignore this entry.
      #
      if {[string first "X" $gflags] >= 0 && $itemstate != "normal"} {
	continue
      }

      #
      # If debug flag is required, but not set, then ignore this entry
      #
      if {[string first "D" $gflags] >= 0 && !$flags(D)} {
	continue
      }

      #
      # Test other regular flags.  Set item to disabled if the test fails.
      #
      foreach f $baseFlags {
	if {[info exists flags($f)] && !$flags($f) } {
	  if {[string first $f $gflags] >= 0 } {
	    set itemstate disabled
	  }
	}
      }

      if {$image != "" && $image != "-"} { set image [gifI $image.gif]} { set image "" }
      if {$simage != "" && $simage != "-"} { set simage [gifI $simage.gif]} { set simage "" }

      set index ""

      if {[llength $lname] > 2} {
	set subm [string map {" " .} [lrange $lname 1 [expr [llength $lname]-2]]]
	set em $m.$subm
	set lname {}
      } else {
	set em $m
      }

      catch { menu $em }
      if { $ename == "separator" || $action == "-separator"} {
	# This is a spearator entry
	$em add separator
	set index [$em index last]
      } elseif { $action == "-cascade" } {
	# This is an cascade parent
	set submenu $em.$ename
	catch { menu $em.$ename -tearoff 0}
	set index [menuEntry $em [m menu.$name] -menu $submenu -image $image -state $itemstate]
      } elseif { $varval != "-"} {
	set var ""
	set val ""
	scan $varval "%\[^=\]=%s" var val
	set index [menuEntry $em [m menu.$name] -command Action::$action -radio 1 \
		       -variable $var -value $val -image $image -selectimage $simage -state $itemstate]
      } else {
	# This is a normal command entry
	set index [menuEntry $em [m menu.$name] -command Action::$action -image $image -state $itemstate]
      }

      #
      # Record menu entries that use 
      #
      for {set i 0} {$i < [string length $gflags] } { incr i } {
	set f [string index $gflags $i]
	if {[string is alnum $f] && [lsearch $baseFlags $f] >= 0 } {
	  if {![info exists baseFlagEntries($f)] || [lsearch $baseFlagEntries($f) [list $em $index] ] < 0 } {
	    lappend baseFlagEntries($f) [list $em $index]
	  }
	}
      }

    }
  }


  ######################################################################
  #
  # This procedure builds the actual menu bar.  It is assumed that the frame
  # for the menu bar has already been created.
  #
  proc makeBar {w type} {
    variable menuButtonData
    variable menuEntryData
    variable menuId
    variable baseFlags
    variable baseFlagEntries
    variable editorMenus
    variable scopeMenus

    switch $type {
      editor { set menuList $editorMenus }
      scope { set menuList $scopeMenus }
    }

    #
    # Clear the base flags 
    #
    foreach f $baseFlags {
      set baseFlagEntries($f) {}
    }

    frame $w -takefocus 1

    #
    # Initialize menuId for all buttons if necessary
    #
    foreach button $menuButtonData {
      set name	[lindex $button 0]

      if {![info exists menuId($name)]} {
	set menuId($name) {}
      }
    }

    #
    # Create the menu buttons
    #
    foreach button $menuList {
      set data [assocg $button $menuButtonData]
      set name	[lindex $data 0]
      set func	[lindex $data 1]

      parseName [m menu.$name] label ul
      menubutton $w.$name -text $label -underline $ul -menu $w.$name.menu
      pack $w.$name -in $w -side left

      makeMenu $w.$name.menu $name
    }

#    bind $w <FocusIn> { puts FocusIn }
#    bind $w <FocusOut> { puts FocusOut }
  }

  #############################################################################
  #
  # Register a key sequence with a command.
  #
  # Parameters:
  #     cmd			Name of command to register.
  #     keyseq			Key sequenc corresponding to a command.
  #
  proc setAccelerator {cmd keyseq} {
    variable accelTable

    set accelTable($cmd) $keyseq
  }

  #############################################################################
  #
  # Set or clear flags that can control menu appearance
  #
  proc setFlags args {
    variable flags
    variable baseFlags
    variable baseFlagEntries

    set state 1
    set estate "normal"
    set rebuild 0

    foreach f $args {
      if {$f == "-clear"} {
	set state 0
	set itemstate disabled
      } elseif {$f == "-set"} {
	set state 1
	set itemstate normal
      } elseif {[lsearch $baseFlags $f] >= 0 } {
	if { $flags($f) == $state } continue
	set flags($f) $state

	foreach p $baseFlagEntries($f) {
	  [lindex $p 0] entryconfigure [lindex $p 1] -state $itemstate
	}
      } else {
	if { $flags($f) == $state } continue
	set flags($f) $state
	set rebuild 1
      }
    }

    if {$rebuild} {
      rebuildAll
    }
  }

  #############################################################################
  #
  # Inform the menu manager of the current tkgate mode.
  #
  proc setState {s} {
    variable state

    if {$s == $state } return

    set state $s
    rebuildAll
  }
}
