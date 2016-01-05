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
# Last edit by hansen on Sat Feb 21 16:28:03 2009
#
#
# Popup flag meanings:
#      r		Use label directly without a messages file lookup
#      R		Do not prepend "Action::" to command name
#      I		Prepend a "PopupMenu::inpop " to command.
#

namespace eval PopupMenu {
  #
  #
  #  Tag	Label Name		Action			Image		Sel. Image	Var./Cascade	Flags
  variable menuEntryData {
    {libraries	library.mgr		loadLibrary		file_lib	-		-		- }
    {libname	library.mgr		loadLibrary		file_lib	-		-		- }
    {libname	library.close		unloadSelectedLibrary	file_unlib	-		-		- }

    {wire	wire.prop		popupWireProps		i_wireprops	-		-		- }
    {wire	wire.addseg		popupWireAddStub	emptytool	-		-		- }

    {wirelabel	wire.showlabel		popupWireShowLabel	emptytool	-		-		- }
    {wirelabel	wire.hidealllabel	popupWireHideAllLabel	emptytool	-		-		- }
    {wirelabel	wire.clearalllabel	popupWireClearAllLabel	emptytool	-		-		- }

    {wireunlabel wire.hidelabel		popupWireHideLabel	emptytool	-		-		- }
    {wireunlabel wire.hidealllabel	popupWireHideAllLabel	emptytool	-		-		- }
    {wireunlabel wire.clearalllabel	popupWireClearAllLabel	emptytool	-		-		- }

    {wiresize	wire.showsize		popupWireShowSize	emptytool	-		-		- }

    {wireunsize	wire.hidesize		popupWireHideSize	emptytool	-		-		- }

    {gateport	gate.addport		addPort			addport		-		-		- }

    {gateprop	gate.prop		editProps		i_gateprops	-		-		- }
    {portprop	gate.prop		editProps		i_portprops	-		-		- }

    {gate	gate.anchor		popupAnchor		anchor		unanchor	pop_anchor	- }
    {gate	gate.settech		-cascade		emptytool	-		settech		- }
    {gate	gate.delete		delete			delgate		-		-		- }

    {gatebase	gate.anchor		popupAnchor		anchor		unanchor	pop_anchor	- }
    {gatebase	gate.delete		delete			delgate		-		-		- }

    {joint3	gate.anchor		popupAnchor		anchor		unanchor	pop_anchor	- }
    {joint3	wire.addseg		popupWireAddStub	emptytool	-		-		- }

    {joint4	gate.anchor		popupAnchor		anchor		unanchor	pop_anchor	- }

    {block	gate.open		openMod			blk_open	-		-		- }
    {block	interface		-cascade		editintr	-		int=block.int	- }
    {block	module.prop		blockPropCanvas		i_modprops	-		-		- }
    {block.int	interface.edit		openInterface		-		-		-		- }
    {block.int	interface.set		setBlockDesc		-		-		-		- }
    {block.int	interface.update	updateInterface		-		-		-		- }
    {block.int	interface.updateall	updateAllInterfaces	-		-		-		- }
    {block.int	interface.auto		autoGenerateCanvas	-		-		-		- }

    {blockedge	gate.addin		popupBlockAddIn		emptytool	-		-		- }
    {blockedge	gate.addout		popupBlockAddOut	emptytool	-		-		- }
    {blockedge	gate.addinout		popupBlockAddInOut	emptytool	-		-		- }
      
    {blockport	igen.type		-cascade		emptytool	-		ioset=blockport.type - }
    {blockport	igen.size		-cascade		emptytool	-		iosize=blockport.size - }
    {blockport	igen.delete		popupDeletePort		emptytool	-		-		- }

    {blockport.type	port.in		popupPortMakeIn		emptytool	-		-		- }
    {blockport.type	port.out	popupPortMakeOut	emptytool	-		-		- }
    {blockport.type	port.inout	popupPortMakeInOut	emptytool	-		-		- }

    {blockport.size 1			"popupPortSize 1" 	-		-		-		r }
    {blockport.size 2			"popupPortSize 2" 	-		-		-		r }
    {blockport.size 4			"popupPortSize 4" 	-		-		-		r }
    {blockport.size 8			"popupPortSize 8" 	-		-		-		r }
    {blockport.size 16			"popupPortSize 16"	-		-		-		r }
    {blockport.size 32			"popupPortSize 32"	-		-		-		r }


    {canv	make			-cascade		emptytool	-		make=@make	- }
    {canv	tool			-cascade		emptytool	-		tool=@tool	- }
    {canv	-			-separator		-		-		-		- }
    {canv	gate.close		closeMod		blk_close	-		-		- }

    {notmove	tool			-cascade		-		-		tool=@tool	- }

    {simu	gate.close		closeMod		blk_close	-		-		- }
    {simu	-			-separator		-		-		-		- }
    {simu	simulate.end		endSimulator		sim_stop	-		-		- }
    {simu	-			-separator		-		-		-		- }
    {simu	simulate.run		simRun			sim_go		-		-		- }
    {simu	simulate.pause		simPause		sim_pause	-		-		- }
    {simu	simulate.step		simStep			sim_step	-		-		- }
    {simu	simulate.cycle		simCycle		sim_clock	-		-		- }
    {simu	simulate.break		editBreakpoints		sim_break	-		-		- }
    {simu	simulate.script		doSimScript		sim_script	-		-		- }
    {simu	-			-separator		-		-		-		- }
    {simu	simulate.load		simLoadMem		sim_load	-		-		- }
    {simu	simulate.dump		simDumpMem		sim_dump	-		-		- }
    {simu	simulate.view		simViewMem		sim_view	-		-		- }

    {wiresimu	simulate.addprobe	toggleProbe		net_probe	-		-		I }

    {scopetrace	simulate.delprobe	delScopeProbe		net_probe	-		-		I }

    {close	gate.close		closeMod		blk_close	-		-		- }

    {open	gate.open		openMod			blk_open	-		-		- }

    {modlist	module.new		blockNew		blk_new		-		-		- }
    {modlist	module.del		blockDelete		blk_delete	-		-		- }
    {modlist	module.copy		blockCopy		blk_copy	-		-		- }
    {modlist	module.rename		blockRename		blk_rename	-		-		- }
    {modlist	module.claim		blockClaim		blk_claim	-		-		- }
    {modlist	module.setroot		blockSetRoot		blk_root	-		-		- }
    {modlist	module.prop		blockProp		i_modprops	-		-		- }
    {modlist	interface		-cascade		editintr	-		int=modlist.int	- }

    {modlist.int interface.edit		openInterface		-		-		-		- }
    {modlist.int interface.updateall	updateAllInterfaces	-		-		-		- }
    {modlist.int interface.auto		autoGenerateSelected	-		-		-		- }


    {ipanports	igen.type		-cascade		-		-		type=ipanports.type	- }
    {ipanports	igen.size		-cascade		-		-		size=ipanports.size	- }
    {ipanports	igen.side		-cascade		-		-		side=ipanports.side	- }
    {ipanports	igen.delete		"PortEntry::inpopup delete"	-	-		-		R }

    {ipanports.type igen.in		"PortEntry::inpopup setType in" -	-		-		R }
    {ipanports.type igen.out		"PortEntry::inpopup setType out" -	-		-		R }
    {ipanports.type igen.inout		"PortEntry::inpopup setType inout" -	-		-		R }

    {ipanports.size 1			"PortEntry::inpopup setSize 1" -	-		-		Rr }
    {ipanports.size 2			"PortEntry::inpopup setSize 2" -	-		-		Rr }
    {ipanports.size 4			"PortEntry::inpopup setSize 4" -	-		-		Rr }
    {ipanports.size 8			"PortEntry::inpopup setSize 8" -	-		-		Rr }
    {ipanports.size 16			"PortEntry::inpopup setSize 16" -	-		-		Rr }
    {ipanports.size 32			"PortEntry::inpopup setSize 32" -	-		-		Rr }

    {ipanports.side igen.left		"PortEntry::inpopup setSide left" -	-		-		R }
    {ipanports.side igen.right		"PortEntry::inpopup setSide right" -	-		-		R }
    {ipanports.side igen.top		"PortEntry::inpopup setSide top" -	-		-		R }
    {ipanports.side igen.bottom		"PortEntry::inpopup setSide bottom" -	-		-		R }

    {paste	edit.paste		yankFromBuf		edit_paste	-		-		- }

    {hdledit	gate.close		closeMod		blk_close	-		-		- }
    {hdledit	gate.open		HdlEditor::openModule	blk_open	-		-		R }

    {cut	edit.copy		copyToBuf		edit_copy	-		-		- }
    {cut	edit.cut		cutToBuf		edit_cut	-		-		- }

    {hdlsimu	simulate.addprobe	HdlEditor::toggleProbe	net_probe	-		-		R }
  }

  proc inpop {cmd} {
    gat_popupSetState 1
    eval $cmd
    gat_popupSetState 0
  }

  #############################################################################
  #
  # Make the menu for $tag in the menu $w
  #
  proc make {m args} {
    variable menuEntryData

    menu $m -tearoff 0

    set isfirst 1
    foreach context $args {

      if { $context == "-nosep" } {
	set isfirst 1
	continue
      }
      #
      # Add separators between blocks
      #
      if {!$isfirst} {
	$m add separator
      }
      set isfirst 0

      #
      # Create the menu entries
      #
      foreach entry $menuEntryData {
	set tag ""
	set name ""
	set action ""
	set image ""
	set simage ""
	set varval ""
	set gflags ""
	lscan $entry tag name action image simage varval gflags

	if { $tag != $context } continue

	if {$image != "" && $image != "-"} { set image [gifI $image.gif]} { set image "" }
	if {$simage != "" && $simage != "-"} { set simage [gifI $simage.gif]} { set simage "" }

	set index ""

	if { [string first r $gflags] < 0 } {
	  set name [m pop.$name]
	}

	if { $action == "-separator" } {
	  # This is a spearator entry
	  $m add separator
	  set index [$m index last]
	} elseif { $action == "-cascade" } {


	  if { [scan $varval "%\[^=\]=%s" var val] == 2 }  {
	    safeeval $::menu_forbidden $m add cascade -image $image -compound left -label $name -menu $m.$var
	    if { [string index $val 0] == "@" } {
	      Menu::makeMenu $m.$var [string range $val 1 end]
	      $m.$var configure -tearoff 0
	    } else { 
	      PopupMenu::make $m.$var $val
	    }
	  } else {
	    safeeval $::menu_forbidden $m add cascade -image $image -compound left -label $name -menu $m.$varval
	  }

	} else {

	  if { [string first R $gflags] < 0 } {
	    set action "Action::$action"
	  }

	  if { [string first I $gflags] >= 0 } {
	    set action "PopupMenu::inpop $action"
	  }

	  if { $varval != "-"} {
	    set var ""
	    set val ""
    
	    if { [scan $varval "%\[^=\]=%s" var val] == 2 }  {
	      safeeval $::menu_forbidden $m add command -command $action -radio 1 -variable $var -value $val -image $image -selectimage $simage -compound left -indicatoron 0 -label $name
	    } else {
	      safeeval $::menu_forbidden $m add checkbutton -command $action -variable $var -image $image -selectimage $simage -compound left -indicatoron 0 -label $name
	    }
	  } else {
	    # This is a normal command entry
	    safeeval $::menu_forbidden $m add command -command $action -image $image -compound left -label $name
	  }
	}
      }
    }
  }


  proc post {w x y s Px Py} {
    global pop_anchor pop_tech main_w

    tkg_buttonPress $main_w $x $y $s 3
    set ptype [gat_setpop $x $y]
    set pop_anchor [gat_anchor]
    set pop_tech [gat_getTech]
    tkg_buttonRelease $main_w $x $y $s

    catch { tk_popup .pop_$ptype $Px $Py }
  }

  proc scopePost {Px Py} {
    catch { tk_popup .pop_scopetrace $Px $Py }
  }
}

#############################################################################
#############################################################################
#
# Create the popup menus
#
proc makePopups {} {
  PopupMenu::make 	.pop_wire 		wire wirelabel
  PopupMenu::make 	.pop_wireL 		wire wireunlabel
  PopupMenu::make 	.pop_mbwire 		wire wiresize wirelabel
  PopupMenu::make 	.pop_mbwireL 		wire wiresize wireunlabel
  PopupMenu::make 	.pop_mbwireS 		wire wireunsize wirelabel
  PopupMenu::make 	.pop_mbwireLS 		wire wireunsize wireunlabel
  PopupMenu::make	.pop_MIgate		gateport gateprop gate cut
  PopupMenu::make	.pop_gate		gateprop gate cut
  PopupMenu::make	.pop_joint3		joint3
  PopupMenu::make	.pop_joint4		joint4
  PopupMenu::make	.pop_multi		gate cut
  PopupMenu::make	.pop_block		block gateprop gatebase
  PopupMenu::make	.pop_blockedge		blockedge block gateprop gatebase
  PopupMenu::make	.pop_blockport		blockport block portprop gatebase
  PopupMenu::make 	.pop_canv		canv paste
  PopupMenu::make 	.pop_simu		simu 
  PopupMenu::make 	.pop_blksimu		open -nosep simu 
  PopupMenu::make 	.pop_wiresimu		wiresimu simu
  PopupMenu::make	.pop_anal		close
  PopupMenu::make	.pop_blkanal		open -nosep close
  PopupMenu::make 	.pop_intf		close
  PopupMenu::make 	.pop_intfblock		open -nosep close 
  PopupMenu::make 	.pop_intfblockport	blockport open -nosep close 
  PopupMenu::make 	.pop_intfblockedge	blockedge open -nosep
  PopupMenu::make	.pop_notmove		notmove close
  PopupMenu::make	.pop_modlist		modlist
  PopupMenu::make	.pop_ipanports		ipanports
  PopupMenu::make	.pop_hdleditp		paste hdledit
  PopupMenu::make	.pop_hdledit		hdledit
  PopupMenu::make 	.pop_hdleditsel		cut hdledit
  PopupMenu::make 	.pop_hdlsimu		hdlsimu hdledit
  PopupMenu::make 	.pop_scopetrace		scopetrace
  PopupMenu::make	.pop_libraries		libraries
  PopupMenu::make	.pop_libname		libname
}

