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
# Last edit by hansen on Mon Jan 19 19:52:13 2009
#

#############################################################################
#
# Get the selected block (from list or tree)
#
proc tkg_getSelectedBlock {} {

  set name [BlockList::getselection]
  if { $name != "" } { return $name }
  set name [BlockTree::getselection]
  if { $name != "" } { return $name }
  set name [gat_getSelected module]

  return $name
}

proc tkg_moduleSelector {w args} {
  set width 10
  set height 10
  set command ""
  set value ""
  set variable ""
  set entry ""
  set libraryonly 0
  set nomain 0

  parseargs $args {-width -height -command -value -variable -entry -libraryonly -nomain}

  set block_list {}
  foreach m [BlockList::getallmodules] {
    set add_ok 1

    if { $libraryonly && ![BlockList::isLibrary $m]} {
      set add_ok 0
    }
    if { $nomain && [BlockList::getType $m] == "root" } {
      set add_ok 0
    }
    if { $add_ok } {
      lappend block_list $m
    }
  }

  if { $value == "" } {
    set value [lindex $block_list 0]
  }

  Dropbox::new $w -width $width -height $height -command $command -value $value -variable $variable -entry $entry

  foreach m $block_list {
    Dropbox::itemadd $w $m
  }

  return [llength $block_list]
}

#############################################################################
#
# This is the dialog box to define new module types
#
namespace eval BlockOp {
  variable props
  variable srcModule ""
  variable dstModule ""


  proc delete {} {
    baseDlg -title [m blklst.delete.title] -srcprompt "[m blklst.name]:" -command gat_deleteBlock \
      -image [gifI module_del.gif] -caption [m blklst.del.cap] -explaination [m blklst.del.exp]
  }

  proc copy {} {
    baseDlg -title [m blklst.copy.title] -srcprompt "[m blklst.from]:" -dstprompt "[m blklst.to]:" -command gat_copyBlock \
      -image [gifI module_copy.gif] -caption [m blklst.copy.cap] -explaination [m blklst.copy.exp]
  }

  proc rename {} {
    baseDlg -title [m blklst.rename.title] -srcprompt "[m blklst.from]:" -dstprompt "[m blklst.to]:" \
	-command gat_renameBlock \
	-image [gifI module_rename.gif] -caption [m blklst.rename.cap] -explaination [m blklst.rename.exp]
  }

  proc claim {} {
    baseDlg -title [m blklst.claim.title] -srcprompt "[m blklst.name]:" -command gat_claimBlock \
      -image [gifI module_claim.gif] -caption [m blklst.claim.cap] -libraryonly 1 -explaination [m blklst.claim.exp]
  }

  proc setroot {} {
    baseDlg -title [m blklst.setroot.title] -srcprompt "[m blklst.name]:" -command gat_reroot \
      -image [gifI module_root.gif] -caption [m blklst.setroot.cap] -libraryonly 0 -explaination [m blklst.setroot.exp]
  }

  #############################################################################
  #
  # Base dialog for block operations
  #
  proc baseDlg args {
    variable srcModule
    variable dstModule
    global tkg_currentModule

    set title ""
    set srcprompt ""
    set dstprompt ""
    set command ""
    set image ""
    set caption ""
    set explaination ""
    set libraryonly 0

    set w .blk

    parseargs $args {-title -srcprompt -dstprompt -command -image -caption -libraryonly -explaination}

    if { [catch { toplevel $w } ] } { return }
    wm title $w $title
    wm geometry $w [offsetgeometry . 50 50 ]
    wm transient $w .

    set srcModule [tkg_getSelectedBlock]
    set dstModule ""

    frame $w.top -bd 2 -relief raised
    frame $w.top.t

    #
    # If no selected module, use the current module.
    #
    if { $srcModule == "" } {
      set srcModule $tkg_currentModule
    }

    if { $srcprompt != "" } {
      label $w.top.t.tsrc -text $srcprompt

      if { $libraryonly } {
	set count [tkg_moduleSelector $w.top.t.esrc -width 20 -variable BlockOp::srcModule -entry 0 -libraryonly 1]
      } else {
	set count [tkg_moduleSelector $w.top.t.esrc -width 20 -variable BlockOp::srcModule -value $srcModule -entry 0 -libraryonly 0]
      }

      if { $count == 0 } {
	destroy $w
	errmsg [m err.nomodop]
	return
      }

      append command { $BlockOp::srcModule }
      grid $w.top.t.tsrc -row 0 -column 0 -sticky e -padx 3 -pady 3
      grid $w.top.t.esrc -row 0 -column 1 -sticky w -padx 3 -pady 3
      bind $w.top.t.esrc <Return> $command
      bind $w.top.t.esrc <Return> "+ destroy $w"

      set mod_list [Dropbox::getcontents $w.top.t.esrc]
      if {[lsearch $mod_list $srcModule] < 0 } {
	set srcModule [lindex $mod_list 0]
      }
    }


    if { $dstprompt != "" } {
      label $w.top.t.tdst -text $dstprompt
      entry $w.top.t.edst -textvariable BlockOp::dstModule -bg white
      grid $w.top.t.tdst -row 1 -column 0 -sticky e -padx 3 -pady 3
      grid $w.top.t.edst -row 1 -column 1 -sticky ew -padx 3 -pady 3 

      append command { $BlockOp::dstModule }
      bind $w.top.t.edst <Return> $command
      bind $w.top.t.edst <Return> "+ destroy $w"
    }
    pack $w.top.t -padx 10 -pady 10 -fill both -expand 1 -side right


    if { $explaination != "" } {
      label $w.top.t.exp -text $explaination -justify left -wraplength 200 -font dialogExpFont
      grid $w.top.t.exp -padx 10 -pady 10 -row 2 -column 0 -columnspan 2
    }


    okcancel $w.b -okcommand "$command; destroy $w" -cancelcommand "destroy $w"

    pack $w.b -fill x -side bottom

    if { $image != "" } {
      dialogImage $w.top.icpad -image $image -caption $caption
      pack $w.top.icpad -fill both -side left
    }

    pack $w.top -fill both -expand 1

    if { $srcprompt != "" } {
      focus $w.top.t.esrc
    } else {
      focus $w.top.t.edst
    }

    dialogWait $w
    #  BlockTree::rebuild
  }

  #############################################################################
  #
  # Confirm property settings for a new module.
  #
  proc newConfirm {w} {
    variable props

    if { $props(name) != "" } {
      set props(ok) 1
      gat_newBlock BlockOp::props
    }
    destroy $w
  }

  #############################################################################
  #
  # Frame with property flags
  #
  proc flagFrame {w args} {
    labelframe $w [m db.mod.flags]

    frame $w.pad -height 5
    pack $w.pad

    set array BlockOp::props
    parseargs $args {-array}

    checkbutton $w.prot -variable ${array}(prot)  -text [m db.mod.dataprot]
    pack $w.prot -anchor w -pady 3 -padx 5

    checkbutton $w.protint -variable ${array}(protint)  -text [m db.mod.intfprot]
    pack $w.protint -anchor w -pady 3 -padx 5

    checkbutton $w.proted -variable ${array}(proted)  -text [m db.mod.edprot]
    pack $w.proted -anchor w -pady 3 -padx 5

    label $w.ppos -text [m ipanel.ppos.label]
    pack $w.ppos -anchor w -pady 3 -padx 5
    
    

    set IPanel::props(ppos) scale
    frame $w.grid
    radiobutton $w.grid.grid -text [m ipanel.ppos.scale] -variable ${array}(ppos) -value scale
    frame $w.grid.pad -width 5
    radiobutton $w.grid.free -text [m ipanel.ppos.fix] -variable ${array}(ppos) -value fix
    pack $w.grid.grid $w.grid.pad $w.grid.free -side left

    pack $w.grid -anchor w -pady 3 -padx 5
  }

  #############################################################################
  #
  # Respond to a change in the type by updating image and label.
  #
  proc seeTypeChanged {w args} {
    if { $BlockOp::props(type) == "netlist" } {
      $w.image.i configure -image [gifI type_netlist.gif] 
      $w.image.l configure -text [m db.mod.netexpl]
    } else {
      $w.image.i configure -image [gifI type_hdl.gif] 
      $w.image.l configure -text [m db.mod.hdlexpl]
    }
  }

  #############################################################################
  #
  # Select the type (netlist vs. HDL) for a module
  #
  proc typeSelector {w} {
    global tkg_pastelHighlight

    labelframe $w [m db.mod.type]

    frame $w.pad -height 5
    pack $w.pad

    frame $w.button
    radiobutton $w.button.netlist -variable BlockOp::props(type) -value netlist -text [m db.mod.netlist] 
    radiobutton $w.button.hdl -variable BlockOp::props(type) -value hdl -text [m db.mod.hdl] 
    pack $w.button.netlist $w.button.hdl -padx 3 -pady 3 -anchor nw

    frame $w.image
    label $w.image.i -image [gifI type_netlist.gif] 
    label $w.image.l -text [m db.mod.netexpl] -justify left -anchor n
    pack $w.image.i $w.image.l -padx 5 -pady 5 -side left -anchor nw


    pack $w.button -side left -anchor nw
    pack $w.image -side left -anchor center -fill x -expand 1

    trace variable BlockOp::props(type) w "BlockOp::seeTypeChanged $w"
    bind $w <Destroy> "trace vdelete BlockOp::props(type) w \"BlockOp::seeTypeChanged $w\""
    seeTypeChanged $w

    #	-image [gifI type_Uhdl.gif] -selectimage [gifI type_hdl.gif]
  }

  #############################################################################
  #
  # Post dialog box for defining a new module.
  #
  proc new args {
    variable props
    global tkg_currentModule
    set parent .

    parseargs $args {-parent}

    set w .blk

    if { [catch { toplevel $w } ] } { return }
    wm title $w [m blklst.new.title]
    wm geometry $w [offsetgeometry $parent 50 50 ]
    wm transient $w $parent

    set props(ok) 0
    set props(name) ""
    set props(type) netlist
    set props(prot) 0
    set props(protint) 0
    set props(proted) 1
    set props(ppos) scale

    okcancel $w.ok -okcommand "BlockOp::newConfirm $w" -cancelcommand "destroy $w"
    pack $w.ok -fill x -side bottom

    frame $w.top -bd 2 -relief raised
    pack $w.top -fill both -expand 1

    dialogImage $w.top.icpad -image [gifI module_new.gif] -caption [m blklst.new.cap] -explaination [m blklst.new.exp]
    pack $w.top.icpad -fill both -side left
    frame $w.top.strut -height 300
    pack $w.top.strut -side left

    set pw $w.top.props

    frame $pw
    pack $pw -padx 10 -pady 10 -anchor n -side right

    frame $pw.name
    label $pw.name.l -text [m blklst.name]:
    entry $pw.name.e -textvariable BlockOp::props(name) -bg white -width 40
    pack $pw.name.l -padx 5 -pady 10 -side left
    pack $pw.name.e -padx 5 -pady 10 -side left -fill x -expand 1

    typeSelector $pw.type
    flagFrame $pw.flags

    pack $pw.name  $pw.type  $pw.flags -pady 5 -fill x -anchor w

    bind $pw.name.e <Return> "BlockOp::newConfirm $w"

    update
    focus -force $pw.name.e
    dialogWait $w
  }
}

namespace eval ModuleProps {
  variable props

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
  }

  proc postSelected {args} {

    set which modlist
    parseargs $args {-which}
    if { $which == "canvas" } {
      set name [gat_getSelected module]
      if { $name == "" } {
	set name [tkg_getSelectedBlock]
      }
    } else {
      set name [tkg_getSelectedBlock]
      if { $name == "" } {
	set name [gat_getSelected module]
      }
    }

    if {$name != ""} {
      post $name
    } else {
      errmsg [m err.nomodule]
    }
  }

  proc update {w name} {
    variable props

    gat_editModProps save ModuleProps::props $name
    destroy $w

    catch {
      set IPanel::props(prot) $props(prot)
      set IPanel::props(protint) $props(protint)
      set IPanel::props(proted) $props(proted)
      set IPanel::props(ppos) $props(ppos)
    }
  }

  #############################################################################
  #
  # Create the framed list of module property options
  #
  proc moduleFlags {w args} {
    variable props

    set array ModuleProps::props
    parseargs $args {-array}

    frame $w

    checkbutton $w.prot -variable ${array}(prot)  -text [m db.mod.dataprot]
    pack $w.prot -anchor w -pady 3

    checkbutton $w.protint -variable ${array}(protint)  -text [m db.mod.intfprot]
    pack $w.protint -anchor w -pady 3

    checkbutton $w.proted -variable ${array}(proted)  -text [m db.mod.edprot]
    pack $w.proted -anchor w -pady 3

    frame $w.ppos
    pack $w.ppos -anchor w -pady 3

    label $w.ppos.l -text [m ipanel.ppos.label]
    radiobutton $w.ppos.scale -text [m ipanel.ppos.scale] -variable ${array}(ppos) -value scale
    radiobutton $w.ppos.fix -text [m ipanel.ppos.fix] -variable ${array}(ppos) -value fix

    pack $w.ppos.l -anchor w
    pack $w.ppos.scale -side left -padx 10
    pack $w.ppos.fix -side left

    if { $props(itype) == "symbol" } {
      $w.ppos.scale configure -state disabled
      $w.ppos.fix configure -state disabled
      $w.ppos.l configure -fg [$w.ppos.fix cget -disabledforeground]
    } else {
      $w.ppos.l configure -fg black
      $w.ppos.scale configure -state normal
      $w.ppos.fix configure -state normal
    }


    helpon $w.prot	[m ho.ipanel.prot]
    helpon $w.protint	[m ho.ipanel.protint]
    helpon $w.proted	[m ho.ipanel.proted]
    helpon $w.ppos.scale [m ho.ipanel.scale]
    helpon $w.ppos.fix	[m ho.ipanel.fix]
  }


  proc post {name} {
    variable props

    set w .mprops

    if { [catch { toplevel $w } ] } { return }
    wm title $w [m module.props.title]
    wm geometry $w [offsetgeometry . 50 50 ]
    wm transient $w .

    #
    # Get properties about this module
    #
    gat_editModProps load ModuleProps::props $name


    okcancel $w.okc -okcommand "ModuleProps::update $w $name" -cancelcommand "destroy $w"
    pack $w.okc -side bottom -fill x 

    frame $w.main -bd 2 -relief raised

    dialogImage $w.main.typeimg -image [gifI type_$props(type).gif] -imgbd 2 -imgrelief raised -caption [m module.props]
    grid $w.main.typeimg -row 0 -column 0 -rowspan 4 -padx 5 -pady 5

    grid rowconfigure $w.main 3 -weight 1

    label $w.main.lname -text [m db.mod.name]:
    label $w.main.name -text $name
    grid $w.main.lname -row 0 -column 1 -padx 5 -pady 3 -sticky e
    grid $w.main.name  -row 0 -column 2 -padx 5 -pady 3 -sticky w

    label $w.main.lfile -text [m db.mod.file]:
    entry $w.main.file -textvariable ModuleProps::props(shortfile) -state disabled -width 25 -relief flat
    catch { $w.main.file configure -disabledforeground black }
    grid $w.main.lfile -row 1 -column 1 -padx 5 -pady 3 -sticky e
    grid $w.main.file  -row 1 -column 2 -padx 5 -pady 3 -sticky w

    label $w.main.ltype -text [m db.mod.type]:
    label $w.main.type -text [m db.mod.$props(type)]
    grid $w.main.ltype -row 2 -column 1 -padx 5 -pady 3 -sticky e
    grid $w.main.type  -row 2 -column 2 -padx 5 -pady 3 -sticky w

#    frame $w.main.pad
#    grid $w.main.pad -row

    moduleFlags $w.main.flags -array ModuleProps::props
    grid $w.main.flags -row 3 -column 1 -columnspan 2 -sticky nw

    pack $w.main

    dialogWait $w
  }
}
