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
# Last edit by hansen on Sat Feb 21 22:26:14 2009
#

namespace eval EditCircDB {
  #
  # The 'props' variable contains the following elements:
  #
  #   file		File name
  #   filev		File version
  #   title		Circuit title
  #   encoding		The character encoding of the file.
  #   locale		The locale of the file.
  #   discard		Discard changes on quiting (used for tutorial circuits)
  #   extbar		Extender bar style
  #   scripts		List of simulator scripts
  #   ok		Inicator that the values were confirmed
  #
  variable props

  proc nameFrame {w} {
    variable props
    global tkg_standardEncodings

    labelframe $w [m circ.ident]
    frame $w.pad
    pack $w.pad -pady 5

    frame $w.g

    #
    # File name
    #
    label $w.g.file_l -text [m circ.file]:
    entry $w.g.file_e -textvariable EditCircDB::props(file) -state disabled -relief sunken
    catch { $w.g.file_e configure  -disabledforeground black }
    helpon $w.g.file_e $EditCircDB::props(file)

    #
    # File version
    #
    label $w.g.ver_l -text [m circ.filev]:
    entry $w.g.ver_e -textvariable EditCircDB::props(filev) -state disabled -relief sunken -width 8
    catch { $w.g.ver_e configure -disabledforeground black }

    #
    # File encoding
    #
    label $w.g.encoding_l -text [m circ.encoding]:
    Dropbox::new $w.g.encoding_e -variable EditCircDB::props(encoding) -width 15 -bd 1 
    set didMine 0
    foreach encoding $tkg_standardEncodings {
      Dropbox::itemadd $w.g.encoding_e $encoding
      if { $encoding == $EditCircDB::props(encoding) } { set didMine 1 }
    }
    if { !$didMine } { Dropbox::itemadd $w.g.encoding_e $EditCircDB::props(encoding) }


    #
    # File language
    #
    label $w.g.lang_l -text [m circ.lang]:
    Dropbox::new $w.g.lang_e -variable EditCircDB::props(locale) -width 15 -bd 1 
    set didMine 0
    foreach lang [lsort -dictionary [gat_getLocales]]  {
      set lname [lindex $lang 0]
      Dropbox::itemadd $w.g.lang_e $lname
      if { $lname == $EditCircDB::props(locale) } { set didMine 1 }
    }
    if { !$didMine } { Dropbox::itemadd $w.g.lang_e $EditCircDB::props(locale) }
    


    label $w.g.title_l -text [m circ.title]:
    entry $w.g.title_e -textvariable EditCircDB::props(title) -bg white
    $w.g.file_e xview moveto 1

    grid columnconfigure $w.g 1 -weight 1
    grid $w.g.file_l -row 0 -column 0 -sticky e -padx 5 -pady 3
    grid $w.g.file_e -row 0 -column 1 -sticky ew -padx 5 -pady 3
    grid $w.g.ver_l -row 1 -column 0 -sticky e -padx 5 -pady 3
    grid $w.g.ver_e -row 1 -column 1 -sticky w -padx 5 -pady 3
    grid $w.g.encoding_l -row 2 -column 0 -sticky e -padx 5 -pady 3
    grid $w.g.encoding_e -row 2 -column 1 -sticky w -padx 5 -pady 3
    grid $w.g.lang_l -row 3 -column 0 -sticky e -padx 5 -pady 3
    grid $w.g.lang_e -row 3 -column 1 -sticky w -padx 5 -pady 3
    grid $w.g.title_l -row 4 -column 0 -sticky e -padx 5 -pady 3
    grid $w.g.title_e -row 4 -column 1 -sticky ew -padx 5 -pady 3 



    helpon $w.g.file_l [m ho.circuit.file]
    helpon $w.g.ver_l [m ho.circuit.ver]
    helpon $w.g.encoding_l [m ho.circuit.encoding]
    helpon $w.g.lang_l [m ho.circuit.language]
    helpon $w.g.title_l [m ho.circuit.title]

    pack $w.g -pady 3 -fill x -expand 1
  }

  proc clockFrame {w} {
    variable props

    labelframe $w [m opt.simclock]
    frame $w.pad
    pack $w.pad -pady 3

    radiobutton $w.all -text [m opt.posedge] -variable EditCircDB::props(clockMode) -value 0 \
	-command "$w.actNam configure -state disabled -foreground gray"
    radiobutton $w.act -text "[m opt.setclock]:" -variable EditCircDB::props(clockMode) -value 1 \
	-command "$w.actNam configure -state normal -foreground black"
    entry $w.actNam -width 15 -textvariable EditCircDB::props(clockName) -bg white

    if { $EditCircDB::props(clockMode) == 0 } {
      $w.actNam configure -state disabled
    }

    helpon $w.all [m ho.opt.ckall]
    helpon $w.act [m ho.opt.ckspec]

    pack $w.all -padx 5 -pady 5 -anchor w
    pack $w.act -side left -padx 5 -pady 5 -anchor w 
    pack $w.actNam -side left -padx 5 -pady 5 -anchor w -fill x -expand 1

  }

  proc timescaleFrame {w} {
    labelframe $w [m circ.timescale]
    frame $w.pad
    pack $w.pad -pady 3

    frame $w.g
    pack $w.g

    label $w.g.units_l -text [m circ.timescale.unit]
    label $w.g.precision_l -text [m circ.timescale.prec]

    Dropbox::new $w.g.units_n -variable EditCircDB::props(unitsN) -width 5 -bd 1 -highlightthickness 0
    Dropbox::new $w.g.units_u -variable EditCircDB::props(unitsU) -width 5 -bd 1 -highlightthickness 0
    Dropbox::new $w.g.precision_n -variable EditCircDB::props(precisionN) -width 5 -bd 1 -highlightthickness 0
    Dropbox::new $w.g.precision_u -variable EditCircDB::props(precisionU) -width 5 -bd 1 -highlightthickness 0

    grid $w.g.units_l -row 0 -column 0 -stick e -pady 5 -padx 1
    grid $w.g.units_n -row 0 -column 1 -stick w -pady 5 -padx 1
    grid $w.g.units_u -row 0 -column 2 -stick w -pady 5 -padx 1

    grid $w.g.precision_l -row 1 -column 0 -stick e -pady 5 -padx 1
    grid $w.g.precision_n -row 1 -column 1 -stick w -pady 5 -padx 1
    grid $w.g.precision_u -row 1 -column 2 -stick w -pady 5 -padx 1

    foreach n {1 10 100 } {
      Dropbox::itemadd $w.g.units_n $n
      Dropbox::itemadd $w.g.precision_n $n
    }
    foreach u {s ms us ns ps fs } {
      Dropbox::itemadd $w.g.units_u $u
      Dropbox::itemadd $w.g.precision_u $u
    }
    grid columnconfigure $w.g 3 -minsize 3
  }

  proc simOtherFrame {w} {
    labelframe $w [m circ.sim.features]
    packPad $w
    checkbutton $w.auto -text [m circ.autostart] -anchor w -variable EditCircDB::props(autostart)

    helpon $w.auto [m ho.circuit.autostart]
    pack $w.auto -fill both -anchor w -padx 5 -pady 2
  }

  proc simTVFrame {w} {
    labelframe $w [m circ.sim.tv.title]
    packPad $w

    radiobutton $w.tvall -text [m circ.sim.tv.all] -variable EditCircDB::props(initMode) -value all
    frame $w.tvafter
    radiobutton $w.tvafter.l1 -text [m circ.sim.tv.after.1] -variable EditCircDB::props(initMode) -value after
    entry $w.tvafter.e -textvariable EditCircDB::props(initTime) -width 7 -justify right
    Dropbox::new $w.tvafter.u -variable EditCircDB::props(initUnits) -width 5 -bd 1 -highlightthickness 0
    label $w.tvafter.l2 -text [m circ.sim.tv.after.2]
    pack $w.tvafter.l1 -side left
    pack $w.tvafter.e $w.tvafter.u -side left -padx 1
    pack $w.tvafter.l2 -side left

    radiobutton $w.tvnever -text [m circ.sim.tv.never] -variable EditCircDB::props(initMode) -value never

    foreach u {s ms us ns ps fs } {
      Dropbox::itemadd $w.tvafter.u $u
    }

    pack $w.tvall -anchor w -padx 5
    pack $w.tvafter -anchor w -padx 5
    pack $w.tvnever -anchor w -padx 5
  }

  proc flagsFrame {w} {
    variable props

    labelframe $w [m circ.flags]
    frame $w.pad
    pack $w.pad -pady 5

    checkbutton $w.disc -text [m circ.discchg] -anchor w -variable EditCircDB::props(discard)

    pack $w.disc -fill both -anchor w -padx 5 -pady 2

    helpon $w.disc [m ho.circuit.discchg]
  }


  proc defaultsFrame {w} {
    variable props

    labelframe $w [m circ.defaults]
    packPad $w

    checkbutton $w.extbar -text [m circ.extbar] -anchor w -variable EditCircDB::props(extbar)
    checkbutton $w.shownet -text [m circ.shownet] -anchor w -variable EditCircDB::props(shownet)

    pack $w.extbar -fill both -anchor w -padx 5 -pady 2
    pack $w.shownet -fill both -anchor w -padx 5 -pady 2

    helpon $w.extbar [m ho.circuit.extbar]
    helpon $w.shownet [m ho.circuit.shownet]
  }

  #
  # Send dialog box contents back to gate 
  #
  proc save {w} {
    variable props

    set props(ok) 1

    LibraryMgr::commit
    gat_editCircProps save EditCircDB::props
  }

  proc frameSimulate {w} {
    frame $w

    simOtherFrame $w.other
    clockFrame $w.clock
    timescaleFrame $w.timescale
    simTVFrame $w.tv

    pack $w.other -fill both -padx 5 -pady 5 
    pack $w.clock -fill both -padx 5 -pady 5
    pack $w.timescale -fill both -padx 5 -pady 5
    pack $w.tv -fill both -padx 5 -pady 5 
  }

  proc frameScript {w} {
    global simScript_filter simScript_filetypes

    frame $w

    PathSelector::create $w.script -label [m circ.iniscript] \
	-query tk_getOpenFile  -variable EditCircDB::props(scripts) \
	-filter $simScript_filter -filetypes $simScript_filetypes
    pack $w.script -fill both -padx 5 -pady 5 -expand 1
  }


  proc frameGeneral {w} {
    frame $w

    nameFrame $w.name
    flagsFrame $w.flags
    defaultsFrame $w.defaults

    pack $w.name -fill both -padx 5 -pady 5
    pack $w.flags -fill both -padx 5 -pady 5
    pack $w.defaults -fill both -padx 5 -pady 5
  }

  proc dotab {w tab} {
    frame $w

    switch $tab {
      General {
	frameGeneral $w.body
	set explaination [m circ.exp.general]
      }
      Library {
	LibraryMgr::create $w.body
	set explaination [m circ.exp.library]
      }
      Script {
	frameScript $w.body
	set explaination [m circ.exp.script]
      }
      Simulator {
	frameSimulate $w.body
	set explaination [m circ.exp.simulator]
      }
    }

    dialogImage $w.image -image [gifI circuit_prop.gif] -caption [m circ.label] -explaination $explaination

    pack $w.image -side left -fill y
    pack $w.body -padx 5 -pady 5 -fill both -side left -anchor w -expand 1
  }

  proc post {args} {
    variable props
    global tkg_statusMessage

    #
    # Do not allow while simulator is active
    #
    if {[gat_getMajorMode] == "simulate" } {
      set tkg_statusMessage [m err.notinsim]
      bell
      return
    }

    set starttab ""
    parseargs $args {-starttab}

    set w .edcirc

    if { [catch { toplevel $w }] } { return }

    set props(file) ""
    set props(filev) ""
    set props(title) ""
    set props(discard) 0
    set props(extbar) 1
    set props(scripts) {}
    set props(vlibs) {}
    set props(clockMode) 0
    set props(clockName) ""
    set props(scripts) {}
    set props(ok) 0
    gat_editCircProps load EditCircDB::props

    wm resizable $w 0 0
    wm title $w [m circ.wintitle]
    wm geometry $w [offsetgeometry . 50 50]
    wm transient $w .
    wm withdraw $w
    


    set tabs {General Library Script Simulator}
    set tablabels [list [m circ.general] [m circ.library] [m circ.script] [m circ.simulator]]
    TabBox::new $w.tbox -tabs $tabs -tablabels $tablabels -command EditCircDB::dotab -takefocus 1

    if {$starttab != ""} {
      TabBox::select $w.tbox $starttab
    }

    okcancel $w.ok -okcommand "EditCircDB::save $w; destroy $w" -cancelcommand "destroy $w"
    pack $w.ok -fill both -side bottom
    pack $w.tbox


    wm deiconify $w
    dialogWait $w
  }
}
