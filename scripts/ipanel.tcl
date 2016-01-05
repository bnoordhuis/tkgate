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
# Last edit by hansen on Wed Mar 18 05:31:52 2009
#



#############################################################################
#
# This class is used in conjunction with the IPanel class for managing the
# list of module ports and interfacing it with the interface generator.
#
namespace eval PortEntry {
  variable pss_w
  variable popup_w
  variable lc_map {a b  b c  c d  d e  e f  f g  g h  h i  i j  j k  k l  l m  m n  n o  o p \
		       p q  q r  r s  s t  t u  u v  v w  w x  x y  y z  z .}
  variable uc_map {A B  B C  C D  D E  E F  F G  G H  H I  I J  J K  K L  L M  M N  N O  O P \
		       P Q  Q R  R S  S T  T U  U V  V W  W X  X Y  Y Z  Z .}

  #############################################################################
  #
  # Using $name as a start, generate a new name for a port
  #
  proc generateNextNameFrom {name} {
    variable lc_map
    variable uc_map
    set pname "A"

    if {[regexp {^([_A-Za-z]+)([0-9]+)$} $name X B N]} {
      set pname $B[expr $N + 1]
    } elseif {[regexp {^(_?)([A-Za-z])(.*)$} $name X I B T]} {
      set C [string map $lc_map $B]
      set C [string map $uc_map $C]
      if { $C == "." } {
	set pname $I$B${T}0
      } else {
	set pname $I$C$T
      }
    }

    return $pname
  }

  #############################################################################
  #
  # Set the window to be used for the right click popup.
  #
  proc setPopup {w} {
    variable popup_w
    set popup_w $w
  }

  #############################################################################
  # Usage: entryManager <cmd> <args>
  #
  # The entry manager is a callback function registered with a SpreadSheet object.  It is called
  # then certain events occur and to tell the SpreadSheet what operations can be done on data
  # in the spreadsheet.
  #
  # Examples:
  #
  # manager canenter w r c data			see if we can enter data at (r,c)
  # manager entrywidget w r c W width var	create special widget at (r,c) or return 0 for default
  # manager close w r c data			close the entry widget on a cell and do a
  #						transormation on the data.
  # manager canappend w r c data		can we append a row at (r,c)
  # manager initentry w r c			if appending, get initial row values
  #
  proc manager {cmd args} {
    variable pss_w
    variable popup_w

    if { [lindex $args 0] == "-args" } {
      set args [lindex $args 1]
    }

    switch $cmd {
      canenter {                	# check to see if we can enter data
	return 1
      }
      entrywidget {			# use a special widget for some cells (return 0 for normal entry)
	set w [lindex $args 0]
	set r [lindex $args 1]
	set c [lindex $args 2]
	set W [lindex $args 3]
	set width [lindex $args 4]
	set variable [lindex $args 5]
	if { $c > 0 } {
	  switch $c {
	    1 {
	      Dropbox::new $W -variable $variable -width [expr $width - 3] -bd 1 -highlightthickness 0
	      SpreadSheet::tabBindings $W.e $w $r $c
	      Dropbox::itemadd $W "in"
	      Dropbox::itemadd $W "out"
	      Dropbox::itemadd $W "inout"
	    }
	    2 {
	      bitsizeselector $W -variable $variable -width 5
	      SpreadSheet::tabBindings $W.e $w $r $c
	    }
	    3 {
	      Dropbox::new $W -variable $variable -width [expr $width - 3] -bd 1 -highlightthickness 0
	      SpreadSheet::tabBindings $W.e $w $r $c
	      Dropbox::itemadd $W "top"
	      Dropbox::itemadd $W "bottom"
	      Dropbox::itemadd $W "left"
	      Dropbox::itemadd $W "right"
	    }
	  } 
	  update
	  focus $W.e

	  return 1
	}
	return 0
      }
      rightclick {			# respond to right mouse click
	set w [lindex $args 0]
	set items [SpreadSheet::getselection $w]
	if {[llength $items] > 0 } {
	  set X [winfo pointerx .]
	  set Y [winfo pointery .]
	  set pss_w $w
	  tk_popup $popup_w $X $Y
	}
      }
      deletenotify {			# notification of a deletion
	IPanel::syncPortList
      }
      delete {				# respond to the delete key
	set w [lindex $args 0]
	set items [SpreadSheet::getselection $w]
	SpreadSheet::delete $w $items
      }
      close {				# called when a cell editing is finish to do any post processing
	set w [lindex $args 0]
	set r [SpreadSheet::getEffectiveRow $w [lindex $args 1]]
	set c [lindex $args 2]
	set data [lindex $args 3]
	if {$c == 0} {
	  set row [SpreadSheet::get $w $r]
	  set old [lindex $row 0]
	  set dir [lindex $row 1]
	  set bits [lindex $row 2]
	  set orient [lindex $row 3]
	  set data [gat_validatePortName $IPanel::data(name) $data -old $old -dir $dir -bits $bits -orient $orient]
	}
	return $data
      }
      closenotify {
	IPanel::syncPortList
      }
      reorder {			# Some list entries may have been moved
	IPanel::syncPortList
      }
      canappend {			# return 1 if we can append to the table
	return 1
      }
      initentry {			# create data for a new entry in a table
	set w [lindex $args 0]
	set r [lindex $args 1]
	if {$r == 0 } {
	  return {"A" in 1 left}
	} else {
	  set p [SpreadSheet::get $w [expr $r-1]]

	  set pname [generateNextNameFrom [lindex $p 0]]

	  set p [lreplace $p 0 0 $pname]
	  return $p
	}
	IPanel::syncPortList
      }
    }
  }

  #############################################################################
  #
  # Called in response to pressing the 'Add' button on the interface editor.
  #
  proc add {w} {
    set r [SpreadSheet::size $w]
    set r [SpreadSheet::getPhysicalRow $w $r]
    SpreadSheet::entrySelect $w $r 0
  }

  #############################################################################
  #
  # Called in response to pressing the 'Edit' button on the interface editor.
  #
  proc edit {w} {
    set items [SpreadSheet::getselection $w]

    if { [llength $items] < 1 } { return }

    set r [lindex $items 0]
    set r [SpreadSheet::getPhysicalRow $w $r]
    SpreadSheet::entrySelect $w $r 0
  }

  #############################################################################
  #
  # Called in response to pressing the 'Delete' button on the interface editor.
  #
  proc delete {w} {
    set items [SpreadSheet::getselection $w]
    SpreadSheet::delete $w $items
  }

  #############################################################################
  #
  # Set the size of all selected ports in the interface editor.
  # 
  proc setSize {w bits} {
    set items [SpreadSheet::getselection $w]

    foreach i $items {
      set p [SpreadSheet::get $w $i]
      set p [lreplace $p 2 2 $bits]
      SpreadSheet::put $w $i $p
    }
    IPanel::syncPortList
  }

  #############################################################################
  #
  # Set the type of all selected ports in the interface editor.
  # 
  proc setType {w type} {
    set items [SpreadSheet::getselection $w]

    foreach i $items {
      set p [SpreadSheet::get $w $i]
      set p [lreplace $p 1 1 $type]
      SpreadSheet::put $w $i $p
    }
    IPanel::syncPortList
  }

  #############################################################################
  #
  # Set the side of all selected ports in the interface editor.
  # 
  proc setSide {w side} {
    set items [SpreadSheet::getselection $w]

    foreach i $items {
      set p [SpreadSheet::get $w $i]
      set p [lreplace $p 3 3 $side]
      SpreadSheet::put $w $i $p
    }
    IPanel::syncPortList
  }

  #############################################################################
  #
  # Handle a popup menu command in the interface editor.
  #
  proc inpopup {cmd args} {
    variable pss_w
    eval "$cmd $pss_w $args"
  }
}


#############################################################################
#
# Interface Panel - This panel is used on the interface edit page.
#
#############################################################################
namespace eval IPanel {
  variable data
  variable props
  variable ipanel_w
  variable dpyModType
  variable portlist_w
  variable itype_w
  variable selectedColor "\#c1d2ee"
  variable mouseoverColor "\#e0e8f6"
  variable canvasWidth 175
  variable baseWidth 370

  #############################################################################
  #
  # Hide the IPanel windows
  #
  proc hide {} {
    global main_w tabframe_w

    grid forget $tabframe_w.ipanel
    SymbolEdit::hide
  }

  #############################################################################
  #
  # Show the IPanel windows
  #
  proc show {} {
    global tabframe_w tkg_currentModule

#    puts "IPanel::show $IPanel::data(itype)"

    grid $tabframe_w.ipanel -row 0 -column 0 -sticky nsew -rowspan 2
    
    if { $IPanel::data(itype) == "symbol" } {
      SymbolEdit::openModule  $tkg_currentModule
      SymbolEdit::show
    } else {
      SymbolEdit::hide
    }

#    update
#    update idletasks
#    gat_modintCenter

  }

  #############################################################################
  #
  # transfer data from IPanel 'data' to spreadsheet
  #
  proc loadSpreadsheet {lbw} {
    variable data

    SpreadSheet::flush $lbw
    for { set i 0 } { $i < $data(numports) } { incr i } {
      SpreadSheet::insert $lbw end $data(port:$i)
    }
  }

  #############################################################################
  #
  # Synchronize the graphical version of a module interface with the data
  # in the tcl/tk portlist which has been updated through an editing command.
  #
  proc syncPortList {} {
    variable data
    variable ipanel_w
    global  tkg_currentModule

    set lbw $ipanel_w.lbg.lb


    set data(numports) [SpreadSheet::size $lbw]
    for { set i 0 } { $i < $data(numports) } { incr i } {
      set data(port:$i) [SpreadSheet::get $lbw $i]
    }

#    puts "gat_interface generate $IPanel::data(name) -data IPanel::data -which int -pos keep"
    action +PortListEd {
      gat_interface generate $IPanel::data(name) -data IPanel::data -which int -pos keep
    }
  }

  #############################################################################
  #
  # Synchronize interface after an undo or redo
  #
  proc undoSync {} {
    loadCurrent
  }

  #############################################################################
  #
  # Load interface data for the current module
  #
  proc loadCurrent {} {
    variable data
    variable dpyModType
    variable ipanel_w
    global  tkg_currentModule

#    puts "loadCurrent $tkg_currentModule"

    set data(name) $tkg_currentModule

    if { $tkg_currentModule != "<interfaces>" } {
      gat_interface load  $data(name) -data IPanel::data
      gat_editModProps load IPanel::props $data(name)
      set dpyModType $data(itype)
      show
      loadSpreadsheet $ipanel_w.lbg.lb

      if {$data(itype) == "symbol"} {
	Menu::setState interface-symbol
      } else {
	Menu::setState interface-block
      }
    } else {
      Menu::setState interface-all
      hide
    }
  }



  #############################################################################
  #
  # Create the port list
  #
  proc portList {w} {
    variable portlist_w

    labelframe $w [m ipanel.plist]
    packPad $w -toppad 12

    frame $w.buttons
    pack $w.buttons -side bottom -pady 5 -fill x -expand 1

    frame $w.left_pad -width 5
    pack $w.left_pad -side left
    frame $w.right_pad -width 5
    pack $w.right_pad -side right

    set portlist_w $w.lb
    SpreadSheet::create $w.lb  -height 20 -bd 2 -relief sunken \
	-yscrollcommand "$w.vb set"  -entrycommand PortEntry::manager -resize 1
    SpreadSheet::addcolumn $w.lb -width 12 -header [m igen.port]
    SpreadSheet::addcolumn $w.lb -width 7 -header [m igen.type]
    SpreadSheet::addcolumn $w.lb -width 7 -header [m igen.bits]
    SpreadSheet::addcolumn $w.lb -width 9 -header [m igen.side]
  
    scrollbar $w.vb -orient vertical -command "SpreadSheet::yview $w.lb"
    pack $w.lb -side left
    pack $w.vb -fill y -side left -padx 2

    #
    # Set up the buttons
    #
    button $w.buttons.edit -text [m igen.edit] -command "PortEntry::edit $w.lb"
    button $w.buttons.add -text [m igen.add] -command "PortEntry::add $w.lb"
    button $w.buttons.delete -text [m igen.delete] -command "PortEntry::delete $w.lb"
    frame $w.buttons.pad

    pack $w.buttons.pad -fill x -side left -expand 1
    pack $w.buttons.edit $w.buttons.add $w.buttons.delete -side left -padx 3

    helpon $w.buttons.edit [m ho.ipanel.editport]
    helpon $w.buttons.add [m ho.ipanel.addport]
    helpon $w.buttons.delete [m ho.ipanel.deleteport]
  }

  #############################################################################
  #
  # This procedure is called when the user clicks on the button to confirm a
  # change in the interface type from one to another.  "to_itype" is the new
  # interface type and can be either "symbol" or "block".
  # i
  proc itypeChangeRequest {to_itype args} {
    variable data
    variable itype_w
    variable dpyModType

    set dpyModType $to_itype

    if { $data(itype) == $to_itype } {
      return
    }

    action ChangeType {
      set IPanel::data(itype) $to_itype
      gat_interface changetype $IPanel::data(name) -data IPanel::data -which mod
      IPanel::loadCurrent
    }
  }

  #############################################################################
  #
  # Initialize all of the interface/module properties
  #
  proc mpropInit {} {
    variable props

    set props(shortfile) ""
    set props(file) ""
    set props(type) ""
    set props(prot) "0"
    set props(protint) "0"
    set props(proted) "0"
    set props(inuse) "0"
    set props(modload) "0"
    set props(modsave) "0"
    set props(libname) ""
    set props(wantclaim) "0"
    set props(grid) "grid"
    set props(ppos) fix
  }

  #############################################################################
  #
  # Create the framed list of module property options
  #
  proc moduleFlags {w args} {
    set array IPanel::props
    parseargs $args {-array}

    frame $w

    checkbutton $w.prot -variable ${array}(prot)  -text [m db.mod.dataprot] \
	-command IPanel::mpropUpdate
    pack $w.prot -anchor w -pady 3

    checkbutton $w.protint -variable ${array}(protint)  -text [m db.mod.intfprot] \
	-command IPanel::mpropUpdate
    pack $w.protint -anchor w -pady 3

    checkbutton $w.proted -variable ${array}(proted)  -text [m db.mod.edprot] \
	-command IPanel::mpropUpdate
    pack $w.proted -anchor w -pady 3

    frame $w.ppos
    pack $w.ppos -anchor w -pady 3

    label $w.ppos.l -text [m ipanel.ppos.label]
    radiobutton $w.ppos.scale -text [m ipanel.ppos.scale] -variable ${array}(ppos) -value scale \
	-command IPanel::mpropUpdate
    radiobutton $w.ppos.fix -text [m ipanel.ppos.fix] -variable ${array}(ppos) -value fix \
	-command IPanel::mpropUpdate


    pack $w.ppos.l -anchor w
    pack $w.ppos.scale -side left -padx 10
    pack $w.ppos.fix -side left

    helpon $w.prot	[m ho.ipanel.prot]
    helpon $w.protint	[m ho.ipanel.protint]
    helpon $w.proted	[m ho.ipanel.proted]
    helpon $w.ppos.scale [m ho.ipanel.scale]
    helpon $w.ppos.fix	[m ho.ipanel.fix]
  }


  #############################################################################
  #
  # Respond to a requested change in a module property
  # 
  proc mpropUpdate {} {
    action ModProp {
      gat_editModProps save IPanel::props $IPanel::data(name)
    }
  }

  #############################################################################
  #
  # The module type has changed.  Update the interface accordingly.
  #
  proc itypeChangeNotify args {
    variable selectedColor
    variable mouseoverColor
    variable data
    variable ipanel_w
    variable itype_w

    set w $ipanel_w.itype

    if { $data(itype) == "symbol" } {
      $itype_w.c itemconfig blockrect -fill "white"
      $itype_w.c itemconfig symbolrect -fill $selectedColor

      $w.flags.ppos.scale configure -state disabled
      $w.flags.ppos.fix configure -state disabled
      $w.flags.ppos.l configure -fg [$w.flags.ppos.fix cget -disabledforeground]
    } else {
      $itype_w.c itemconfig blockrect -fill $selectedColor
      $itype_w.c itemconfig symbolrect -fill "white"

      $w.flags.ppos.l configure -fg black
      $w.flags.ppos.scale configure -state normal
      $w.flags.ppos.fix configure -state normal
    }
  }

  proc itypeHighlight {event itype} {
    variable mouseoverColor
    variable dpyModType
    variable itype_w

    if { $itype == $dpyModType } return

    if { $event == "enter" } {
      $itype_w.c itemconfig ${itype}rect -fill $mouseoverColor
    } else {
      $itype_w.c itemconfig ${itype}rect -fill white
    }
  }

  proc itypeSelector {w} {
    variable itype_w
    variable dpyModType
    variable selectedColor
    variable canvasWidth

    set dpyModType none

    set x1 55
    set x2 121
    set b 32

    set itype_w $w
    frame $w

    canvas $w.c -bd 1 -relief sunken -bg white -width $canvasWidth -height 90

    $w.c create rectangle [expr $x1-$b] 0 [expr $x1+$b] 90 -outline "" -tags {block blockrect}
    $w.c create rectangle [expr $x2-$b] 0 [expr $x2+$b] 90 -outline "" -tags {symbol symbolrect}

    $w.c create image 55 40 -image [gifI ipanel_block.gif] -tags {block}
    $w.c create image 120 40 -image [gifI ipanel_symbol.gif] -tags {symbol}
    $w.c create text 55 80 -text [m ipanel.block] -tags {block}
    $w.c create text 120 80 -text [m ipanel.symbol] -tags {symbol}

    pack $w.c -fill both
    label $w.l -text [m ipanel.itype.select] -bd 1 -relief sunken -bg $selectedColor

    $w.c bind block <1> { IPanel::itypeChangeRequest block }
    $w.c bind symbol <1> { IPanel::itypeChangeRequest symbol }

    $w.c bind block <Enter> { IPanel::itypeHighlight enter block }
    $w.c bind block <Leave> { IPanel::itypeHighlight leave block }
    $w.c bind symbol <Enter> { IPanel::itypeHighlight enter symbol }
    $w.c bind symbol <Leave> { IPanel::itypeHighlight leave symbol }


    pack $w.c $w.l -fill both

    helpon $w [m ho.ipanel.select]

    trace variable IPanel::dpyModType w IPanel::itypeChangeNotify
  }

  #############################################################################
  #
  # Create the panel for selecting the type of module interface
  #
  proc interfaceProperties {w} {
    global tkg_pastelHighlight
    variable dpyModType

    labelframe $w [m ipanel.props]

    frame $w.pad
    pack $w.pad -pady 6

    moduleFlags $w.flags
    pack $w.flags -side bottom -anchor w -padx 5 -pady 3

    frame $w.sep -bd 2 -relief sunken -height 2
    pack $w.sep -fill x -padx 15 -pady 5 -side bottom -expand 1

    itypeSelector $w.type
    pack $w.type -anchor w -padx 5 -pady 2 -side left

    button $w.autogen -text [m ipanel.autogen] -command Action::autoGenerate
    button $w.update -text [m ipanel.updateall] -command Action::updateOpenInterface

    pack $w.autogen $w.update -padx 5 -pady 5 -fill x

    helpon $w.autogen [m ho.ipanel.generate]
    helpon $w.update [m ho.ipanel.updateall]
    

#    frame $w.buttons
#    button $w.buttons.edit -text [m ipanel.itype.generate]
#    pack $w.buttons.edit -anchor e -padx 5 -pady 3 -fill x
#    pack $w.buttons -padx 5 -pady 3 -anchor e
  }

  #############################################################################
  #
  # Create the main side panel for module interface editing.
  #
  proc create {w} {
    variable data
    variable ipanel_w
    variable baseWidth
    variable canvasWidth

    mpropInit

    PortEntry::setPopup .pop_ipanports

    #
    # Compute width of interface panel
    #
    set fontScale [getFontScale labelFont 13]

    set w1 [expr $baseWidth*$fontScale]
    set w2 [expr $canvasWidth+($baseWidth-$canvasWidth)*$fontScale]
    set width [max $w1 $w2]
    
    #
    # Create left side panel
    #
    set ipanel_w $w
    frame $w -width $width
    pack propagate $w 0

    frame $w.pad1 -height 5
    pack $w.pad1

    interfaceProperties $w.itype
    pack $w.itype -fill x -padx 5 -pady 5

    portList $w.lbg
    pack $w.lbg -padx 5 -pady 5 -fill x
  }
}

#############################################################################
#
# Interface generator wizard
#
#############################################################################
namespace eval IGen {
  variable igen_w
  variable whichports
  variable portpos
  variable moduleName

  proc ok {} {
    variable igen_w
    variable whichports
    variable portpos
    variable moduleName

    action GenInterf {
      gat_interface generate $moduleName -data IPanel::data -which $IGen::whichports -pos $IGen::portpos
      IPanel::loadCurrent
    }

    destroy $igen_w
  } 

  #
  # Cancel interface generation.
  #
  proc cancel {} {
    variable igen_w

    destroy $igen_w
  } 

  #
  # Post the interface generator wizard
  #
  proc post {args} {
    variable igen_w
    variable whichports
    variable portpos
    variable moduleName

    set which ipanel
    parseargs $args {-which}

    switch $which {
      "selected"  {
	set moduleName [tkg_getSelectedBlock]
      }
      "canvas" {
	set moduleName [gat_getSelected module]
      }
      default {
	set moduleName $IPanel::data(name)
      }
    }

    if { $moduleName == "" } {
      errmsg [m err.nomodule]
      return
    }

    if {[BlockList::getType $moduleName] == "root" } {
      errmsg [m err.rootinterface]
      return
    }


    #
    # Create the interface dialog box
    #
    set w .igen
    set igen_w $w
    if { [catch { toplevel $w }] } { return }
    wm title $w [m igen.title]
    wm geometry $w [offsetgeometry . 50 50 ]


    #
    # Set initial values of 
    #
    set whichports modint
    set portpos keep

    #
    # Create the OK/Cancel buttons
    #
    okcancel $w.ok -okcommand IGen::ok -cancelcommand IGen::cancel
    pack $w.ok -side bottom -fill x

    frame $w.main -bd 2 -relief raised
    pack $w.main

    dialogImage $w.main.image -image [gifI general.gif] -caption [m igen.caption] -explaination [m igen.description]
    pack $w.main.image -side left -fill y

    frame $w.main.name
    pack $w.main.name -anchor w -padx 5 -pady 5
    label $w.main.name.l -text [m igen.modname]
    tkg_moduleSelector $w.main.name.e -width 20 -variable IGen::moduleName -entry 0 -libraryonly 0 -value $moduleName -nomain 1
    pack  $w.main.name.l  $w.main.name.e -side left -anchor w


    #
    # Port selection group
    #
    labelframe $w.main.ports [m igen.portsel]
    frame $w.main.ports.pad
    pack $w.main.ports.pad -pady 7
    radiobutton $w.main.ports.mod    -text [m igen.mod] -variable IGen::whichports -value mod
    radiobutton $w.main.ports.int    -text [m igen.int] -variable IGen::whichports -value int
    radiobutton $w.main.ports.modint -text [m igen.modint] -variable IGen::whichports -value modint
    pack $w.main.ports.mod $w.main.ports.int $w.main.ports.modint -padx 5 -pady 3 -anchor w

    #
    # Port spacing group
    #
    labelframe $w.main.pspace [m igen.portpos]
    frame $w.main.pspace.pad
    pack $w.main.pspace.pad -pady 7
    radiobutton $w.main.pspace.keep -text [m igen.portpos.keep] -variable IGen::portpos -value keep
    radiobutton $w.main.pspace.renew -text  [m igen.portpos.renew] -variable IGen::portpos -value renew
    pack $w.main.pspace.keep $w.main.pspace.renew -padx 5 -pady 3 -anchor w

    frame $w.main.pad

    #
    # Pack the main groups
    #
    pack $w.main.ports -pady 10 -fill x -padx 5
    pack $w.main.pspace -pady 5 -fill x -padx 5
    pack $w.main.pad -pady 50


    #
    # Go into dialog box mode.
    #
    dialogWait $w
  }
}
