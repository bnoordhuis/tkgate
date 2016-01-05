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
# Last edit by hansen on Sun Feb 22 10:28:29 2009
#
#

set emptyImageData {#define aa_width 10
#define aa_height 10
static unsigned char aa_bits[] = {
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};}

image create bitmap emptyImage -data $emptyImageData
font create SSValFont -family Helvetica -size 8

#############################################################################
#
# Procedures in this namespace are used to handle the color selector.
#
namespace eval Color {
  variable colors {
     red red3 firebrick orange brown coral1 sienna tan4 RosyBrown MistyRose3 seashell 
     green green2 green4 chartreuse4 YellowGreen OliveDrab turquoise aquamarine3 aquamarine DarkSeaGreen3 honeydew 
     blue MediumBlue navy RoyalBlue purple SlateBlue CornflowerBlue MediumOrchid4 SkyBlue lavender GhostWhite 
     magenta magenta2 magenta3 violet magenta4 DarkMagenta thistle2 plum3 plum3 LavenderBlush2 orchid4 
     yellow yellow2 yellow3 khaki1 LemonChiffon khaki3 cornsilk yellow4 beige seashell2 ivory4 
     cyan cyan2 LightSeaGreen PaleTurquoise cyan4 PowderBlue azure AliceBlue LightCyan3 CadetBlue DarkSlateGray 
     black gray10 gray20 gray30 gray40 gray50 gray60 gray70 gray80 gray90 white 
  }

  proc setReidaiColor {tag c} {
    global colorreidai1_w colorreidai2_w

    if {[string match ".*backgroundc" $tag]} {
      catch { $colorreidai1_w configure -bg $c }
      catch { $colorreidai2_w configure -bg $c }
      return
    }

    set wlist {}
    catch { lappend wlist $colorreidai1_w }
    catch { lappend wlist $colorreidai2_w }

    foreach w $wlist {
      set taglist {}
      catch { set taglist [$w find withtag $tag] }
      foreach t $taglist {
	foreach option {-outline -fill -foreground} {
	  if {![catch { $w itemconfigure $t $option $c }]} break
	}
      }
    }
  }

  proc setColor {w c vname} {
    catch { global $vname }

    set $vname $c
    $w configure  -bg $c -activebackground $c
    setReidaiColor $w $c
  }

  #
  # Dialog box for selecting color
  #
  proc postSelector {pw vname} {
    variable colors
    catch { global $vname }

    set x [expr [winfo rootx $pw] - 75]
    set y [expr [winfo rooty $pw] - 75]

    set n [llength $colors]
    set width [expr $n/7]

    set w .colorbox
    if { [catch {toplevel $w -cursor arrow }] } { return }
    wm resizable $w 0 0
    wm title $w [m opt.color.title]
    wm geometry $w +${x}+$y
    wm transient $w $pw
    wm overrideredirect $w 1

    frame $w.pallet -bg gray90 -bd 2 -relief raised

    set bgcolor [$w.pallet cget -bg]


    set dialog_color [set $vname]
    set x 0
    set y 0
    foreach c $colors {
      if { $c == $dialog_color } { set relief sunken } else { set relief flat }


      button $w.pallet.c${x}_$y -image emptyImage -bg $c \
	  -activebackground $c -command "destroy $w" -relief $relief \
	  -highlightbackground $bgcolor -highlightthickness 2 -command "Color::setColor $pw $c $vname; destroy $w"
      grid $w.pallet.c${x}_$y -row $y -column $x -padx 1 -pady 1

      bind $w.pallet.c${x}_$y <Enter> "$w.pallet.c${x}_$y configure -highlightbackground cyan3 -relief raised"
      bind $w.pallet.c${x}_$y <Leave> "$w.pallet.c${x}_$y configure -highlightbackground $bgcolor -relief $relief"


      helpon $w.pallet.c${x}_$y $c

      incr y
      if { $y >= $width } {
	set y 0
	incr x
      }
    }

    pack $w.pallet

    button $w.dismiss -bg gray90 -text [m b.dismiss] -command "destroy $w"
    pack $w.dismiss -fill x


    dialogWait $w
  }

  proc setDefaults {w} {
    global tkgdef_moduleColor tkgdef_modulePortColor tkgdef_frameColor tkgdef_commentColor
    global tkgdef_hyperlinkColor tkgdef_wireColor tkgdef_busColor tkgdef_zeroColor
    global tkgdef_toolColor tkgdef_cpathColor tkgdef_gridColor tkgdef_oneColor tkgdef_instColor
    global tkgdef_floatColor tkgdef_unknownColor tkgdef_offLedColor tkgdef_onLedColor tkgdef_zLedColor tkgdef_backgroundColor

    setColor $w.edit.instc    $tkgdef_instColor			opts_instColor		
    setColor $w.edit.modc     $tkgdef_moduleColor		opts_moduleColor		
    setColor $w.edit.modpc    $tkgdef_modulePortColor		opts_modulePortColor	
    setColor $w.edit.framec   $tkgdef_frameColor		opts_frameColor		
    setColor $w.edit.commc    $tkgdef_commentColor		opts_commentColor	
    setColor $w.edit.hlinkc   $tkgdef_hyperlinkColor		opts_hyperlinkColor	
    setColor $w.edit.wirec    $tkgdef_wireColor			opts_wireColor		
    setColor $w.edit.busc     $tkgdef_busColor			opts_busColor		
    setColor $w.edit.toolc    $tkgdef_toolColor			opts_toolColor		
    setColor $w.edit.cpathc   $tkgdef_cpathColor		opts_cpathColor		
    setColor $w.edit.offledc  $tkgdef_offLedColor		opts_offLedColor		
    setColor $w.edit.onledc   $tkgdef_onLedColor		opts_onLedColor		
    setColor $w.edit.zledc    $tkgdef_zLedColor           	opts_zLedColor           
    setColor $w.scope.backgroundc $tkgdef_backgroundColor	opts_backgroundColor	

    setColor $w.scope.gridc    $tkgdef_gridColor		opts_gridColor		
    setColor $w.scope.onec     $tkgdef_oneColor			opts_oneColor		
    setColor $w.scope.zeroc    $tkgdef_zeroColor		opts_zeroColor		
    setColor $w.scope.floatc   $tkgdef_floatColor		opts_floatColor		
    setColor $w.scope.unknownc $tkgdef_unknownColor		opts_unknownColor	
  }

  proc selectorVarChanged {w vname args} {
    global $vname
    Color::setColor $w [set $vname] $vname
  }

  proc selectorbutton {w vname} {
    variable colors
    global $vname

    set c [set $vname]

    setReidaiColor $w $c

    button $w -image emptyImage -bg $c -activebackground $c -command "Color::postSelector $w $vname"

    trace variable $vname w "Color::selectorVarChanged $w $vname"
    bind $w <Destroy> "trace vdelete $vname w \"Color::selectorVarChanged $w $vname\""
  }
}

#############################################################################
#
# Create a dialog box group for editing a list of directories or files.
#
namespace eval PathSelector {
  variable ps_filter
  variable ps_filetypes
  variable ps_variable
  variable ps_command

  proc add {w variable query} {
    variable ps_filter
    variable ps_filetypes

    if { $ps_filter($w) != "none" } {
      set dirname [$query -parent $w -defaultextension $ps_filter($w) -filetypes $ps_filetypes($w) ]
    } else {
      set dirname [$query -parent $w]
    }

    if { $dirname == "" } return
    
    lappend ::$variable $dirname
  }

  proc del {w  variable} {
    set idx [$w.dirs.lb curselection]
    if { $idx != "" } {
      set name [$w.dirs.lb get $idx]
      set ::$variable [ldelete [set ::$variable] $name]
    }
  }

  proc edit {w  variable edit} {
    set idx [$w.dirs.lb curselection]
    if { $idx == "" } return

    set name [$w.dirs.lb get $idx]
    set new_name [$edit -parent $w -value $name]
    if { $new_name == "" } return

    set vidx [lsearch [set ::$variable] $name]
    if { $vidx < 0 } return

    set ::$variable [lreplace [set ::$variable] $vidx $vidx $new_name]
  }

  proc reload {w args} {
    variable ps_variable
    variable ps_command

    $w.dirs.lb delete 0 end
    foreach f [set ::$ps_variable($w)] {
      $w.dirs.lb insert end $f
    }

    if { $ps_command($w) != "" } {
      uplevel \#0 $ps_command($w)
    }
  }

  proc create {w args} {
    variable ps_filter
    variable ps_filetypes
    variable ps_variable
    variable ps_command

    set variable opts_simTIMPath
    set query tk_chooseDirectory
    set label [m opt.devfilepath]
    set filter "none"
    set filetypes "none"
    set height 3
    set edit ""
    set command ""
    parseargs $args {-variable -query -label -filter -filetypes -height -edit -command}

    set ps_filter($w) $filter
    set ps_filetypes($w) $filetypes
    set ps_variable($w) $variable
    set ps_command($w) $command

    labelframe $w $label
    packPad $w -pad 12

    frame $w.dirs
    listbox $w.dirs.lb -height $height -xscrollcommand "$w.dirs.hb set" -yscrollcommand "$w.dirs.vb set" -bg white
    scrollbar $w.dirs.vb -orient vertical -command "$w.dirs.lb yview"
    scrollbar $w.dirs.hb -orient horizontal  -command "$w.dirs.lb xview"

    grid rowconfigure $w.dirs 0 -weight 1
    grid columnconfigure $w.dirs 0 -weight 1
    grid  $w.dirs.lb -row 0 -column 0 -sticky nsew
    grid  $w.dirs.vb -row 0 -column 1 -sticky ns
    grid  $w.dirs.hb -row 1 -column 0 -sticky ew
    pack  $w.dirs -padx 5 -pady 3 -side left -expand 1 -fill both

    button $w.add -text [m b.add] -command "PathSelector::add $w $variable $query"
    pack $w.add -padx 5 -pady 3 -fill x

    if {$height > 3  && $edit != ""} {
      button $w.edit -text [m b.edit] -command "PathSelector::edit $w $variable $edit"
      pack $w.edit -padx 5 -pady 3 -fill x
    }

    button $w.del -text [m b.remove] -command "PathSelector::del $w  $variable"
    pack $w.del -padx 5 -pady 3 -fill x

    foreach f [set ::$variable] {
      $w.dirs.lb insert end $f
    }

    trace variable ::$variable w "PathSelector::reload $w"
    bind $w <Destroy> "trace vdelete ::$variable w \"PathSelector::reload $w\""
  }
}


namespace eval OptionDlg {
  variable edsamp_w
  variable currentPage /General
  variable libraryList


  proc frameGUI {w} {
    labelframe $w [m opt.interaction]
    packPad $w -toppad 12

    checkbutton $w.bdc -variable opts_blockDropConnect -text [m opt.blockDropConnect]

    frame $w.ud
    label $w.ud.l -text "[m opt.undolen]:"
#    entry $w.ud.e -textvariable opts_undoLength -width 4 -bg white
    incdecEntry $w.ud.e -variable opts_undoLength -width 4 -format %d
    pack  $w.ud.l  $w.ud.e -side left

    checkbutton $w.hb -variable opts_helpBalloons -text [m opt.balloon]

    checkbutton $w.tm -variable opts_tearawayMenus -text [m opt.tearawaymenus]

    pack $w.bdc -anchor w -pady 2
    pack $w.ud -anchor nw -pady 2
    pack $w.hb -anchor nw -pady 2
    pack $w.tm -anchor nw -pady 2

    helpon $w.bdc [m ho.opt.blockDropConnect]
    helpon $w.ud [m ho.opt.undolen]
    helpon $w.hb [m ho.opt.bhelp]
    helpon $w.tm [m ho.opt.tearawaymenus]
  }

  proc frameDisplay {w} {
    labelframe $w [m opt.display]
    packPad $w -toppad 12

    checkbutton $w.ss -variable opts_smoothScroll -text [m opt.smoothscroll]
    checkbutton $w.ru -variable opts_regionUpdate -text [m opt.regudate]
    checkbutton $w.db -variable opts_showDialogImage -text [m opt.showimage]
    fontSetSelect $w.fss

    pack $w.ss -anchor w
    pack $w.ru -anchor w
    pack $w.db -anchor w
    pack $w.fss -anchor w

    helpon $w.ss [m ho.opt.smooth]
    helpon $w.ru [m ho.opt.clip]
    helpon $w.db [m ho.opt.showimage]
  }

  proc kbdOptSelect {w} {
    global tkg_gateHome

    frame $w
    label $w.l -text "[m opt.keybind]:"

    Dropbox::new $w.r -variable opts_keyBindings -width 12 -bd 1 -highlightthickness 0

    foreach kbd [glob $tkg_gateHome/bindings/*] {
      set kbd [file tail $kbd]
      if {[string first "~" $kbd] < 0} {
	Dropbox::itemadd $w.r $kbd
      }
    }

    pack $w.l $w.r -side left

    helpon $w.l [m ho.opt.keybind]
    helpon $w.r [m ho.opt.keybind]
  }

  proc fontSetSelect {w} {
    global tkg_gateHome

    frame $w
    label $w.l -text "[m opt.general.fontset]:"

    Dropbox::new $w.r -variable opts_fontSet -width 12 -bd 1 -highlightthickness 0
    Dropbox::itemadd $w.r tiny
    Dropbox::itemadd $w.r small
    Dropbox::itemadd $w.r normal
    Dropbox::itemadd $w.r large
    Dropbox::itemadd $w.r huge

    pack $w.l $w.r -side left

    helpon $w.l [m ho.opt.general.fontset]
    helpon $w.r [m ho.opt.general.fontset]
  }

  proc frameInput {w} {
    labelframe $w [m opt.input]
    packPad $w

    frame $w.f1
    frame $w.pad
    kbdOptSelect $w.f1.kbd
    pack $w.f1.kbd -anchor nw -pady 2

    #
    # top-level frames
    #
    pack $w.f1 -side left -anchor nw
    pack $w.pad -side left -anchor nw -padx 10
  }

  proc frameCircuitDefaults {w} {
    labelframe $w [m opt.circdefaults]
    packPad $w

    checkbutton $w.extbar -text [m circ.extbar] -anchor w -variable opts_extendBars
    checkbutton $w.shownet -text [m circ.shownet] -anchor w -variable opts_showSwitchNets

    pack $w.extbar -fill both -anchor w -padx 5 -pady 2
    pack $w.shownet -fill both -anchor w -padx 5 -pady 2

    helpon $w.extbar [m ho.opt.circuit.extbar]
    helpon $w.shownet [m ho.opt.circuit.shownet]
  }

  proc frameToolbar {w name toolbars} {
    labelframe $w $name

    frame $w.f

    set ncols [m @opt.tool.columns]
    set numInRow [expr ([llength $toolbars]+$ncols-1)/$ncols]

    set r 0
    set c 0
    foreach t $toolbars {
      set name [m [lindex $t 0]]
      set help [m [lindex $t 1]]
      set var  [lindex $t 2]

      checkbutton $w.f.b${r}_$c -variable $var -text $name
      grid  $w.f.b${r}_$c -row $r -column $c -sticky w

      helpon $w.f.b${r}_$c $help

      incr r
      if { $r >= $numInRow } {
	grid columnconfigure  $w.f [expr $c+1] -minsize [m @opt.tool.colwidth]
	incr c 2
	set r 0
      }
    }
    pack $w.f -padx 10 -pady [rescale *10] -anchor w
  }

  proc frameBasic {w} {
    labelframe $w [m opt.features]
    packPad $w -toppad 12

    checkbutton $w.ssave -variable opts_doSafeSave -text [m opt.safeSave]
    checkbutton $w.fsave -variable opts_saveWithCells -text [m opt.saveCells]

    frame $w.cpe
    checkbutton $w.cpe.b -variable opts_checkpointEnabled -text [m opt.ckpoint.1]
    incdecEntry $w.cpe.e -variable opts_checkpointFreq -width 4 -format %d
    label $w.cpe.l -text "[m opt.ckpoint.2]"
    pack $w.cpe.b $w.cpe.e  $w.cpe.l -side left

    pack $w.ssave -anchor w
    pack $w.fsave -anchor w
    pack $w.cpe -anchor nw 

    helpon $w.ssave [m ho.opt.ssave]
    helpon $w.fsave [m ho.opt.fsave]
    helpon $w.cpe.b [m ho.opt.ckpt]
    helpon $w.cpe.l [m ho.opt.ckptfreq]
  }

  proc frameIdentity {w} {
    labelframe $w [m opt.identity]

    label $w.siteL -text "[m opt.site]:" -anchor w
    entry $w.siteE -width 20 -textvariable opts_siteName -bg white

    label $w.nameL -text "[m opt.username]:" -anchor w
    entry $w.nameE -width 20 -textvariable opts_userName -bg white

    grid columnconfigure $w 1 -weight 1
    grid rowconfigure $w 0 -minsize [rescale *10]
    grid rowconfigure $w 3 -minsize [rescale *10]
    grid $w.siteL -row 1 -column 0 -sticky e  -padx 5 -pady 2
    grid $w.siteE -row 1 -column 1 -sticky ew  -padx 5 -pady 2
    grid $w.nameL -row 2 -column 0 -sticky e  -padx 5 -pady 2
    grid $w.nameE -row 2 -column 1 -sticky ew  -padx 5 -pady 2

    helpon $w.siteL [m ho.opt.site]
    helpon $w.nameL [m ho.opt.name]

  }

  proc frameHDLEditor {w} {
    labelframe $w [m opt.editor.features]
    packPad $w

    checkbutton $w.movesel -text [m opt.editor.movesel] -variable opts_hdlDragSel
    pack $w.movesel -anchor w -padx 5 -pady 2

    helpon $w.movesel [m ho.opt.editor.movesel]
  }

  proc frameHDLAutoindent {w} {
    checkframe $w [m opt.editor.format] -relief groove -bd 2 -variable opts_hdlIndenting
    packPad $w

    frame $w.indent
    label $w.indent.l -text [m opt.editor.indentlevel]
    incdecEntry $w.indent.e -width 5 -variable opts_hdlIndentLevel -format %d
    bindtags $w.indent.e [concat [list NumEntry] [bindtags $w.indent.e]]
    checkbutton $w.beginindent -text [m opt.editor.beginindent] -variable opts_hdlIndentNoBegin
    checkbutton $w.rtab -text [m opt.editor.returntab] -variable opts_hdlIndentReturn

    helpon ${w}_label [m ho.opt.editor.format]
    helpon $w.indent.l [m ho.opt.editor.indent]
    helpon $w.rtab [m ho.opt.editor.rtab]
    helpon $w.beginindent [m ho.opt.editor.beginindent]

    pack $w.indent.l $w.indent.e -side left
    pack $w.indent  -anchor w -padx 5 -pady 4
    pack $w.beginindent $w.rtab -anchor w -padx 5 -pady 2
  }

  proc editorSample {w} {
    variable edsamp_w 

    frame $w

    set edsamp_w $w.c
    text $w.c -bg white -width 1 -height 1 -font hdlFont -yscrollcommand "$w.vb set"
    scrollbar $w.vb -orient vertical -command "$w.c yview"

    $w.c insert end "// line comment\n"
    $w.c insert end "module foo(X,Y,Z,A,B);\n"
    $w.c insert end "  output X,Y,Z;\n"
    $w.c insert end "  input A,B;\n"
    $w.c insert end "  reg Z;\n"
    $w.c insert end "   /* block comment */\n"
    $w.c insert end "  initial\n"
    $w.c insert end "    \$display(\"start\");\n"
    $w.c insert end "  and g1(X,A,B);\n"
    $w.c insert end "  bar g2(Y,A,B);\n"
    $w.c insert end "\n"
    $w.c insert end "endmodule\n"

    $w.c tag add hdlComment 1.0 1.end
    $w.c tag add hdlComment 6.3 6.end
    $w.c tag add hdlReserved 2.0 2.6
    $w.c tag add hdlReserved 3.2 3.8
    $w.c tag add hdlReserved 4.2 4.7
    $w.c tag add hdlReserved 5.2 5.5
    $w.c tag add hdlReserved 7.2 7.9
    $w.c tag add hdlReserved 12.0 12.9
    $w.c tag add hdlTaskWord 8.4 8.12
    $w.c tag add hdlString 8.13 8.20
    $w.c tag add hdlGateWord 9.2 9.5
    $w.c tag add hdlInstWord 10.2 10.5
    $w.c configure -state disabled

    pack $w.c -fill both -side left -expand 1
    pack $w.vb -fill y -side left
  }

  proc editorDoColor args {
    variable edsamp_w
    global opts_hdlColorize

    foreach vname {hdlComment hdlReserved hdlGateWord hdlInstWord hdlTaskWord hdlString} {
      global opts_${vname}Color opts_${vname}Bold opts_${vname}Italic
      catch {
	set bold ""
	set italic ""

	if { $opts_hdlColorize } {
	  set color [set opts_${vname}Color]
	  if {[set opts_${vname}Bold]} { set bold Bold }
	  if {[set opts_${vname}Italic]} { set italic Italic }
	} else {
	  set color black
	}


	$edsamp_w tag configure $vname -foreground $color -font hdl${bold}${italic}Font
      }
    }
  }

  proc stylegroup_init {w v} {
    trace variable ::opts_${v}Color w OptionDlg::editorDoColor
    trace variable ::opts_${v}Bold w OptionDlg::editorDoColor
    trace variable ::opts_${v}Italic w OptionDlg::editorDoColor
  }

  proc stylegroup_uninit {w v args} {
    trace vdelete ::opts_${v}Color w OptionDlg::editorDoColor
    trace vdelete ::opts_${v}Bold w OptionDlg::editorDoColor
    trace vdelete ::opts_${v}Italic w OptionDlg::editorDoColor
  }

  proc stylegroup {w l v} {
    frame $w
    label $w.l -width 15 -text $l -justify left -anchor w

    Color::selectorbutton $w.color opts_${v}Color
    label $w.color_l -text [m opt.editor.color]

    frame $w.pad -width 4
    checkbutton $w.bold -text [m opt.editor.bold] -variable opts_${v}Bold
    checkbutton $w.italic -text [m opt.editor.italic] -variable opts_${v}Italic

    pack $w.l 			-padx 5 -side left -anchor w
    pack $w.color		-padx 6 -side left -anchor w
    pack $w.color_l $w.pad	-side left -anchor w
    pack $w.bold $w.italic	-padx 8 -side left -anchor w

    stylegroup_init $w $v
    bind $w <Destroy> "OptionDlg::stylegroup_uninit $w $v"
  }

  proc editorOptions {w} {
    frame $w

    stylegroup $w.comment [m opt.editor.comments] hdlComment
    stylegroup $w.string [m opt.editor.strings] hdlString
    stylegroup $w.reserved [m opt.editor.reserved] hdlReserved
    stylegroup $w.gate [m opt.editor.gateword] hdlGateWord
    stylegroup $w.task [m opt.editor.taskword] hdlTaskWord
    stylegroup $w.inst [m opt.editor.instword] hdlInstWord

    pack $w.comment -anchor w
    pack $w.string -anchor w
    pack $w.reserved -anchor w
    pack $w.gate -anchor w
    pack $w.task -anchor w
    pack $w.inst -anchor w
  }


  #############################################################################
  #
  # Create as sample editing scene in a canvas
  #
  proc setupSampleEdit {c w} {
    global bd

    $c create text 75 105 -text Comment -justify left -anchor w	-tags $w.commc
    $c create text 155 105 -text Hyperlink -justify left -anchor w -tags $w.hlinkc

    $c create line 2 75 20 75				-tags $w.wirec
    $c create line 2 85 20 85				-tags $w.wirec
    $c create line 40 80 100 80				-tags $w.wirec
    $c create bitmap 30 80 -bitmap "@$bd/sampleand.b"	-tags $w.instc
    $c create text 102 80 -text E -justify left -anchor w	-tags $w.modpc

    $c create bitmap 30 20 -bitmap "@$bd/samplemark.b"	-tags $w.toolc


    $c create line 75 80 75 65				-tags $w.wirec
    $c create bitmap 75 65 -bitmap "@$bd/sampleled.b"	-tags $w.instc
    $c create bitmap 75 65 -bitmap "@$bd/sampleledI.b"	-tags $w.offledc
    $c create rectangle 74 79 76 81			-tags $w.instc
    $c create text 75 53 -text off 			-tags $w.commc


    $c create text 173 80 -text Q -justify right -anchor e -tags $w.modpc
    $c create line 175 80 200 80  200 65			-tags $w.wirec
    $c create bitmap 200 65 -bitmap "@$bd/sampleled.b"	-tags $w.instc
    $c create bitmap 200 65 -bitmap "@$bd/sampleledI.b"	-tags $w.onledc
    $c create text 200 53 -text on 			-tags $w.commc

    $c create text 173 40 -text Z -justify right -anchor e -tags $w.modpc
    $c create line 175 40 200 40  200 25			-tags $w.wirec
    $c create bitmap 200 25 -bitmap "@$bd/sampleled.b"	-tags $w.instc
    $c create bitmap 200 25 -bitmap "@$bd/sampleledI.b"	-tags $w.zledc
    $c create text 200 13 -text float 			-tags $w.commc


    $c create line 2 40 100 40 -width 2			-tags $w.busc
    $c create line 40 45 52 33				-tags $w.busc
    $c create text 45 30 -text 8				-tags $w.busc
    $c create text 102 40 -text D -justify left -anchor w	-tags $w.modpc


    $c create rectangle 100 25 175 90			-tags $w.modc
    $c create text 138 55 -text XYOP			-tags $w.modc


    $c create rectangle 10 60 60 100 -dash {3 3}		-tags $w.framec
    $c create rectangle 15 55 47 65 -fill white -outline white
    $c create text 30 60 -text AND 			-tags $w.framec

    $c create line 140 0  140 25				-tags $w.wirec
    $c create line 140 90 140 120				-tags $w.wirec

    $c create text 135 27 -text Cin -anchor n -tags $w.modpc
    $c create text 135 88 -text Cout -anchor s -tags $w.modpc

    for { set q 0 } { $q < 5 } { incr q } {
      set dy [expr $q*5]
      $c create line 135 [expr $dy + 5] 145 [expr $dy - 5]	-tags $w.cpathc
      $c create line 135 [expr 95 + $dy + 5] 145 [expr 95 + $dy - 5]	-tags $w.cpathc
    }
    
  }

  #############################################################################
  #
  # Create as sample scope scene in a canvas
  #
  proc setupSampleScope {c w} {
    global opts_wireColor opts_busColor

    for { set y 10 } { $y < 150 } { incr y 20 } {
      $c create line 5 $y 37 $y				-tags $w.gridc
      $c create line 40 $y 200 $y -width 2		-tags $w.gridc
      for { set x 40 } { $x <= 200 } { incr x 40 } {
	$c create line $x [expr $y-2] $x [expr $y+2] -width 2 -tags $w.gridc
      }
    }

    $c create text 10 20 -text ck	 -anchor w	-fill $opts_wireColor
    $c create text 10 40 -text A	 -anchor w	-fill $opts_wireColor
    $c create text 10 60 -text B	 -anchor w	-fill $opts_wireColor
    $c create text 10 80 -text D1	 -anchor w	-fill $opts_busColor
    $c create text 10 100 -text D2	 -anchor w	-tags $opts_busColor

    #
    # The clock trace
    #
    set y1 15
    set y0 25
    set x 40
    set H 20
    while { $x < 175 } {
      $c create line $x $y0 [expr $x + $H]  $y0 -tags $w.zeroc
      incr x $H
      $c create line $x $y0 $x $y1 -tags $w.onec
      $c create line $x $y1 [expr $x + $H] $y1 -tags $w.onec
      incr x $H
      $c create line $x $y0 $x $y1 -tags $w.zeroc
    }

    #
    # "A" trace
    #
    set y1 35
    set yz 40
    set y0 45
    set x 40
    $c create line $x $y0 [expr $x + 45]  $y0 -tags $w.zeroc
    incr x 45
    $c create line $x $y0 $x $y1 -tags $w.onec
    $c create line $x $y1 [expr $x + 30] $y1 -tags $w.onec
    incr x 30
    $c create line $x $y1 $x $yz -tags $w.floatc
    $c create line $x $yz [expr $x + 40] $yz -tags $w.floatc
    incr x 40
    $c create line $x $yz $x $y1 -tags $w.onec
    $c create line $x $y1 [expr $x + 45] $y1 -tags $w.onec

    #
    # "B" trace
    #
    set y1 55
    set yz 60
    set y0 65
    set x 40
    $c create line $x $y0 [expr $x + 25]  $y0 -tags $w.zeroc
    incr x 25
    $c create line $x $y0 $x $y1 -tags $w.unknownc
    $c create line $x $y1 [expr $x + 30] $y1 -tags $w.unknownc
    $c create line $x $y0 [expr $x + 30] $y0 -tags $w.unknownc
    incr x 30
    $c create line $x $y0 $x $y1 -tags $w.onec
    $c create line $x $y1 [expr $x + 30] $y1 -tags $w.onec
    $c create line $x $y1 $x $y0 -tags $w.zeroc
    incr x 30
    $c create line $x $y1 $x $yz -tags $w.unknownc
    $c create line $x $yz [expr $x + 40] $yz -tags $w.unknownc
    $c create line $x $y1 [expr $x + 40] $y1 -tags $w.unknownc
    incr x 40
    $c create line $x $y1 $x $y0 -tags $w.zeroc
    $c create line $x $y0 [expr $x + 35] $y0 -tags $w.zeroc

    #
    # "D1" trace
    #
    set y1 75
    set yz 80
    set y0 85
    set x 40
    $c create text $x $yz -text 42 -tags $w.onec -font SSValFont -anchor w
    $c create line $x $y0 [expr $x + 35] $y0 -tags $w.onec
    $c create line $x $y1 [expr $x + 35] $y1 -tags $w.onec
    incr x 35
    $c create text $x $yz -text 45 -tags $w.onec -font SSValFont -anchor w
    $c create line $x $y1 $x $y0 -tags $w.onec
    $c create line $x $y0 [expr $x + 25] $y0 -tags $w.onec
    $c create line $x $y1 [expr $x + 25] $y1 -tags $w.onec
    incr x 25
    $c create text $x $yz -text 0 -tags $w.zeroc -font SSValFont -anchor w
    $c create line $x $y1 $x $y0 -tags $w.zeroc
    $c create line $x $y0 [expr $x + 30] $y0 -tags $w.zeroc
    $c create line $x $y1 [expr $x + 30] $y1 -tags $w.zeroc
    incr x 30
    $c create text $x $yz -text 41 -tags $w.onec -font SSValFont -anchor w
    $c create line $x $y1 $x $y0 -tags $w.onec
    $c create line $x $y0 [expr $x + 45] $y0 -tags $w.onec
    $c create line $x $y1 [expr $x + 45] $y1 -tags $w.onec
    incr x 45
    $c create text $x $yz -text 4B -tags $w.onec -font SSValFont -anchor w
    $c create line $x $y1 $x $y0 -tags $w.onec
    $c create line $x $y0 [expr $x + 25] $y0 -tags $w.onec
    $c create line $x $y1 [expr $x + 25] $y1 -tags $w.onec

    #
    # "D2" trace
    #
    set y1 95
    set yz 100
    set y0 105
    set x 40
    $c create text $x $yz -text 49 -tags $w.onec -font SSValFont -anchor w
    $c create line $x $y0 [expr $x + 50] $y0 -tags $w.onec
    $c create line $x $y1 [expr $x + 50] $y1 -tags $w.onec
    incr x 50
    $c create line $x $y1 $x $y0 -tags $w.floatc
    $c create line $x $yz [expr $x + 30] $yz -tags $w.floatc
    incr x 30
    $c create line $x $y1 $x $y0 -tags $w.unknownc
    $c create line $x $y0 [expr $x + 30] $y0 -tags $w.unknownc
    $c create line $x $y1 [expr $x + 30] $y1 -tags $w.unknownc
    incr x 30
    $c create text $x $yz -text 53 -tags $w.onec -font SSValFont -anchor w
    $c create line $x $y1 $x $y0 -tags $w.onec
    $c create line $x $y0 [expr $x + 50] $y0 -tags $w.onec
    $c create line $x $y1 [expr $x + 50] $y1 -tags $w.onec
  }

  proc frameColorEdit {w} {
    global colorreidai1_w

    labelframe $w [m opt.color.editwin]

    frame $w.opts

    set colorreidai1_w $w.reidai
    canvas $w.reidai -bg white -bd 2 -relief sunken -width 250 -height 115
    setupSampleEdit $w.reidai $w.opts

    label $w.opts.inst -text [m opt.inst]
    Color::selectorbutton $w.opts.instc opts_instColor

    label $w.opts.mod -text [m opt.mods]
    Color::selectorbutton $w.opts.modc opts_moduleColor

    label $w.opts.modp -text [m opt.modports]
    Color::selectorbutton $w.opts.modpc opts_modulePortColor

    label $w.opts.frame -text [m opt.frames]
    Color::selectorbutton $w.opts.framec opts_frameColor

    label $w.opts.comm -text [m opt.comment]
    Color::selectorbutton $w.opts.commc opts_commentColor

    label $w.opts.hlink -text [m opt.hlink]
    Color::selectorbutton $w.opts.hlinkc opts_hyperlinkColor

    label $w.opts.wire -text [m opt.wire]
    Color::selectorbutton $w.opts.wirec opts_wireColor

    label $w.opts.bus -text [m opt.bus]
    Color::selectorbutton $w.opts.busc opts_busColor

    label $w.opts.tool -text [m opt.tools]
    Color::selectorbutton $w.opts.toolc opts_toolColor

    label $w.opts.cpath -text [m opt.cpath] 
    Color::selectorbutton $w.opts.cpathc opts_cpathColor

    label $w.opts.background -text [m opt.background]
    Color::selectorbutton $w.opts.backgroundc opts_backgroundColor

    label $w.opts.offled -text [m opt.offled]
    Color::selectorbutton $w.opts.offledc opts_offLedColor

    label $w.opts.onled -text [m opt.onled]
    Color::selectorbutton $w.opts.onledc opts_onLedColor

    label $w.opts.zled -text [m opt.zled]
    Color::selectorbutton $w.opts.zledc opts_zLedColor


    set r 1
    set c 0
    set colorItem { background inst mod modp frame comm hlink wire bus tool cpath offled onled zled }
    foreach ci $colorItem {
      grid $w.opts.${ci}c  -row $r  -column $c -padx 5 -pady 0 -sticky w
      grid $w.opts.$ci     -row $r  -column [expr $c+1] -padx 5 -pady 0 -sticky w

      if { $c == 0 } {
	set c 3
      } else {
	set c 0
	incr r
      }
    }
    grid columnconfigure $w.opts 2 -minsize 25
    grid rowconfigure $w.opts 0 -minsize 10

    pack $w.opts $w.reidai -padx 5 -pady 5
  }

  proc frameColorScope {w} {
    global colorreidai2_w

    labelframe $w [m opt.color.scopewin]

    set colorreidai2_w $w.reidai
    canvas $w.reidai -bg white -bd 2 -relief sunken -width 250 -height 115
    setupSampleScope $w.reidai $w.opts

    frame $w.opts

    label $w.opts.grid -text [m opt.grid]
    Color::selectorbutton $w.opts.gridc opts_gridColor

    label $w.opts.one -text [m opt.logic1]
    Color::selectorbutton $w.opts.onec opts_oneColor

    label $w.opts.zero -text [m opt.logic0]
    Color::selectorbutton $w.opts.zeroc opts_zeroColor

    label $w.opts.float -text [m opt.float]
    Color::selectorbutton $w.opts.floatc opts_floatColor

    label $w.opts.unknown -text [m opt.unknown]
    Color::selectorbutton $w.opts.unknownc opts_unknownColor

    set r 1
    set c 0
    set colorItem { grid one zero float unknown  }
    foreach ci $colorItem {
      grid $w.opts.${ci}c  -row $r  -column $c -padx 5 -pady 0 -sticky w
      grid $w.opts.$ci     -row $r  -column [expr $c+1] -padx 5 -pady 0 -sticky w

      if { $c == 0 } {
	set c 3
      } else {
	set c 0
	incr r
      }
    }
    grid columnconfigure $w.opts 2 -minsize 25
    grid rowconfigure $w.opts 0 -minsize 10

    pack $w.opts $w.reidai -padx 5 -pady 5
  }

  proc frameHDLColor {w} {
    checkframe $w [m opt.editor.colorize] -relief groove -bd 2 -variable opts_hdlColorize -command OptionDlg::editorDoColor
    packPad $w -pad 13

    helpon ${w}_label [m ho.opt.editor.colorize]


    editorOptions $w.opts
    editorSample $w.sample

    pack $w.sample -padx 2 -pady 2 -fill both -expand 1
    pack $w.opts -padx 2 -pady 2 -fill x

    editorDoColor
  }

  #############################################################################
  #
  # simulator options
  #
  proc frameSimulateBasic {w} {
    global simscr_w

    labelframe $w [m opt.simbase]
    packPad $w

    frame $w.t

    label $w.t.simstepL -text "[m opt.estepsz]:"
    incdecEntry $w.t.simstepE -width 5 -variable opts_simStepSize -format %d

    label $w.t.clkstepL -text "[m opt.cstepsz]:"
    incdecEntry $w.t.clkstepE -width 5 -variable opts_simClockStepSize -format %d

    label $w.t.clkostepL -text "[m opt.overstep]:"
    incdecEntry $w.t.clkostepE -width 5 -variable opts_simClockOverStep -format %d

    label $w.t.vpopL -text "[m opt.vpopupdelay]:"
    incdecEntry $w.t.vpopE -width 5 -variable opts_valuePopUpDelay -format %d

    checkbutton $w.dosort -text "[m opt.sorttraces]" -variable opts_simSortTraces
    checkbutton $w.savetraces -text "[m opt.simulator.savetraces]" -variable opts_saveTraces
    checkbutton $w.noglitch -text "[m opt.simulator.noglitch]" -variable opts_glitchSuppress

    grid columnconfigure $w.t 2 -weight 1
    grid $w.t.simstepL	-row 0 -column 0 -sticky e -padx 5 -pady 1
    grid $w.t.simstepE	-row 0 -column 1 -sticky w -padx 5 -pady 1
    grid $w.t.vpopL	-row 1 -column 0 -sticky e -padx 5 -pady 1
    grid $w.t.vpopE	-row 1 -column 1 -sticky w -padx 5 -pady 1 

    grid $w.t.clkostepL	-row 2 -column 0 -sticky e -padx 5 -pady 1
    grid $w.t.clkostepE	-row 2 -column 1 -sticky w -padx 5 -pady 1 
    grid $w.t.clkstepL	-row 3 -column 0 -sticky e -padx 5 -pady 1
    grid $w.t.clkstepE	-row 3 -column 1 -sticky w -padx 5 -pady 1 

    pack $w.t -anchor w -fill x
    pack $w.dosort -padx 5 -pady 0 -anchor w 
    pack $w.savetraces -padx 5 -pady 0 -anchor w 
    pack $w.noglitch -padx 5 -pady 0 -anchor w 

    helpon $w.t.simstepL [m ho.opt.simstep]
    helpon $w.t.clkstepL [m ho.opt.ckstep]
    helpon $w.t.clkostepL [m ho.opt.ckover]
    helpon $w.t.vpopL [m ho.opt.vpopupdelay]
    helpon $w.dosort [m ho.opt.sorttraces]
    helpon $w.noglitch [m ho.opt.sim.noglitch]
    helpon $w.savetraces [m ho.opt.sim.savetraces]
  }

  proc frameSimulateTech {w} {
    global gdf_filter gdf_filetypes

    PathSelector::create $w -label [m opt.delayfilepath] -query tk_getOpenFile -variable opts_simDelayFile \
	    -filter $gdf_filter -filetypes $gdf_filetypes -height 4

    helpon ${w}_label [m ho.opt.sim.tech]
  }

  proc frameSimulateErrors {w} {
    global opts_simDelayFile

    labelframe $w [m opt.simulator.err]
    packPad $w

    radiobutton $w.stopany -text [m opt.simulator.stopany] -value 4 -variable opts_warningMode
    radiobutton $w.showall -text [m opt.simulator.showall] -value 3 -variable opts_warningMode
    radiobutton $w.showiferr -text [m opt.simulator.showiferr] -value 2 -variable opts_warningMode
    radiobutton $w.ignore -text [m opt.simulator.ignore] -value 1 -variable opts_warningMode

    pack $w.stopany -anchor w -padx 5
    pack $w.showall -anchor w -padx 5
    pack $w.showiferr -anchor w -padx 5
    pack $w.ignore -anchor w -padx 5

    frame $w.pad2
    pack $w.pad2 -pady 3

    helpon ${w}_label [m ho.opt.sim.err]
    helpon $w.stopany [m ho.opt.sim.err.stopany]
    helpon $w.showall [m ho.opt.sim.err.showall]
    helpon $w.showiferr [m ho.opt.sim.err.showiferr]
    helpon $w.ignore [m ho.opt.sim.err.ignore]
  }

  proc flpPathUpdated {args} {
    variable libraryList
    global opts_simVLibPath

    set libraryList [LibraryMgr::scanlibs $opts_simVLibPath]
  }

  proc frameLibraryPath {w} {
    PathSelector::create $w -label [m opt.vlibpath] -query tk_chooseDirectory -variable opts_simVLibPath -height 10 -command OptionDlg::flpPathUpdated
    helpon ${w}_label	[m ho.opt.lib.vpath]
  }

  #############################################################################
  #
  # Helper function for frameLibraryLoad to update contents of listboxes when
  # the opts_simVLibNames variable is modified
  #
  proc fllUpdate {w args}  {
    variable libraryList
    global opts_simVLibNames

    $w.left.lb delete 0 end
    $w.right.lb delete 0 end

    foreach l $libraryList {
      if {[lsearch $opts_simVLibNames $l] >= 0} {
	$w.right.lb insert end $l
      } else {
	$w.left.lb insert end $l
      }
    } 
  }

  #############################################################################
  #
  # Helper function for frameLibraryLoad to install variable watches when window
  # is created.
  #
  proc fllInit {w} {
    trace variable ::opts_simVLibNames w "OptionDlg::fllUpdate $w"
    trace variable OptionDlg::libraryList w "OptionDlg::fllUpdate $w"
  }

  #############################################################################
  #
  # Helper function for frameLibraryLoad to uninstall variable watches when
  # window is destroyed.
  #
  proc fllUninit {w} {
    trace vdelete ::opts_simVLibNames w "OptionDlg::fllUpdate $w"
    trace vdelete OptionDlg::libraryList w "OptionDlg::fllUpdate $w"
  }

  #############################################################################
  #
  # Helper function for frameLibraryLoad to make a library selected
  #
  proc fllMakeSelected {w} {
    global opts_simVLibNames

    set index [$w.left.lb curselection]
    if { $index == "" } return

    set lib [$w.left.lb get $index]

    lappend opts_simVLibNames $lib
  }

  #############################################################################
  #
  # Helper function for frameLibraryLoad to make a library selected
  #
  proc fllMakeUnselected {w} {
    global opts_simVLibNames

    set index [$w.right.lb curselection]
    if { $index == "" } return

    set lib [$w.right.lb get $index]

    set index [lsearch $opts_simVLibNames $lib]
    if {$index < 0} return

    set opts_simVLibNames [lreplace $opts_simVLibNames $index $index]

  }


  proc frameLibraryLoad {w} {
    variable libraryList

    labelframe $w [m opt.vlibnames]
    packPad $w
    helpon ${w}_label	[m ho.opt.lib.vlib]

    #
    # Get current set of libraries
    #
    set libraryList [LibraryMgr::scanlibs]
    
    #
    # Left side box
    #
    frame $w.left
    label $w.left.l -text [m opt.lib.load.available]
    listbox $w.left.lb -yscrollcommand "$w.left.vb set" -width 1
    scrollbar $w.left.vb -orient vertical -command "$w.left.lb yview"
    pack $w.left.l
    pack $w.left.lb -side left -fill both -expand 1
    pack $w.left.vb -side left -fill y

    #
    # Right side box
    #
    frame $w.right
    label $w.right.l -text [m opt.lib.load.selected]
    listbox $w.right.lb -yscrollcommand "$w.right.vb set" -width 1
    scrollbar $w.right.vb -orient vertical -command "$w.right.lb yview"
    pack $w.right.l
    pack $w.right.lb -side left -fill both -expand 1
    pack $w.right.vb -side left -fill y

    frame $w.middle
    button $w.middle.ltor -image [gifI bigrightarrow.gif] -command "OptionDlg::fllMakeSelected $w"
    button $w.middle.rtol -image [gifI bigleftarrow.gif] -command "OptionDlg::fllMakeUnselected $w"
    pack $w.middle.ltor $w.middle.rtol -pady 5

    pack $w.left   -padx 5 -pady 5 -side left -fill both -expand 1
    pack $w.middle -padx 5 -pady 5 -side left
    pack $w.right  -padx 5 -pady 5 -side left -fill both -expand 1

    fllInit $w
    bind $w <Destroy> "OptionDlg::fllUninit $w"

    fllUpdate $w
  }

  proc frameLibraryLoad {w} {
    variable libraryList

    labelframe $w [m opt.vlibnames]
    packPad $w
    helpon ${w}_label	[m ho.opt.lib.vlib]

    #
    # Get current set of libraries
    #
    set libraryList [LibraryMgr::scanlibs]


    SetSelector::new $w.ss -availablelabel [m opt.lib.load.available] \
	-selectedlabel [m opt.lib.load.selected] \
	-availablevariable OptionDlg::libraryList \
	-selectedvariable opts_simVLibNames

    pack $w.ss -fill both -expand 1
  }

  proc frameLibraryVPD {w} {
    PathSelector::create $w -label [m opt.devfilepath] -query tk_chooseDirectory -variable opts_simVPDPath -height 10

    helpon ${w}_label	[m ho.opt.lib.vpd]
  }

  proc frameSecurityFeatures {w} {
    labelframe $w [m opt.sec.simfeatures]
    packPad $w

    checkbutton $w.send -text [m opt.sec.send] -variable opts_securitySend
    checkbutton $w.open -text [m opt.sec.open] -variable opts_securityOpen
    checkbutton $w.writemem -text [m opt.sec.writemem] -variable opts_securityWriteMem
    checkbutton $w.enqueue -text [m opt.sec.enqueue] -variable opts_securityEnqueue
    
    pack $w.send -anchor w -padx 5 -pady 2
    pack $w.open -anchor w -padx 5 -pady 2
    pack $w.writemem -anchor w -padx 5 -pady 2
    pack $w.enqueue -anchor w -padx 5 -pady 2

    helpon $w.send [m ho.opt.sec.send]
    helpon $w.open [m ho.opt.sec.open]
    helpon $w.writemem [m ho.opt.sec.writemem]
    helpon $w.enqueue [m ho.opt.sec.enqueue]
  }

  proc frameSecurityExecLevel {w} {
    labelframe $w [m opt.sec.execlevel]
    packPad $w

    radiobutton $w.exec -text [m opt.sec.exec] -variable opts_securityExec -value 2
    radiobutton $w.regexec -text [m opt.sec.regexec] -variable opts_securityExec -value 1
    radiobutton $w.noexec -text [m opt.sec.noexec] -variable opts_securityExec -value 0

    pack $w.exec -anchor w -padx 5 -pady 2
    pack $w.regexec -anchor w -padx 5 -pady 2
    pack $w.noexec -anchor w -padx 5 -pady 2

    helpon $w.exec [m ho.opt.sec.exec]
    helpon $w.regexec [m ho.opt.sec.regexec]
    helpon $w.noexec [m ho.opt.sec.noexec]
  }

  proc frameSecurityHandle {w} {
    labelframe $w [m opt.sec.handle]
    packPad $w

    radiobutton $w.stop -text [m opt.sec.stop] -value 2 -variable opts_securityHandling
    radiobutton $w.warn -text [m opt.sec.warn] -value 1 -variable opts_securityHandling
    radiobutton $w.ignore -text [m opt.sec.ignore] -value 0 -variable opts_securityHandling
    
    pack $w.stop -anchor w -padx 5 -pady 2
    pack $w.warn -anchor w -padx 5 -pady 2
    pack $w.ignore -anchor w -padx 5 -pady 2

    helpon $w.stop	[m ho.opt.sec.stop]
    helpon $w.warn	[m ho.opt.sec.warn]
    helpon $w.ignore	[m ho.opt.sec.ignore]
  }

  proc frameHTML {w} {
    labelframe $w [m opt.html.linkopts]
    frame $w.pad
    grid rowconfigure $w 0 -minsize [rescale *10]

    label $w.html_l -text [m opt.html.browser]:
    entry $w.html_e -textvariable opts_browserCommand

    label $w.email_l -text [m opt.html.email]:
    entry $w.email_e -textvariable opts_emailCommand

    grid columnconfigure $w 1 -weight 1
    grid $w.html_l -row 1 -column 0 -sticky e  -padx 5 -pady 5
    grid $w.html_e -row 1 -column 1 -sticky ew  -padx 5 -pady 5
    grid $w.email_l -row 2 -column 0 -sticky e  -padx 5 -pady 5
    grid $w.email_e -row 2 -column 1 -sticky ew  -padx 5 -pady 5

    helpon $w.html_l [m ho.opt.html.browser]
    helpon $w.email_l [m ho.opt.html.email]
  }

  proc frameDebug {w} {

    labelframe $w [m opt.debug.opt]
    packPad $w

    checkbutton $w.cvfy -variable opts_contVerify -text [m opt.contver] -anchor w
    checkbutton $w.simdbg -variable opts_simDebugInterface -text [m opt.showsimcmd] -anchor w
    pack $w.cvfy  $w.simdbg  -padx 5 -pady 5 -fill x -anchor w

    helpon $w.cvfy [m ho.contver]
    helpon $w.simdbg [m ho.simwatch]
  }


  proc pageToolbarsEdit {w} {
    frame $w

    set base_toolbars {
	  {opt.tool.file ho.opt.tool.file opts_filetb}
	  {opt.tool.edit ho.opt.tool.edit opts_edittb}
	  {opt.tool.gateopt ho.opt.tool.gateopt opts_gateoptb}
	  {opt.tool.gateprop ho.opt.tool.gateprop opts_gateprop}
	  {opt.tool.undo ho.opt.tool.undo opts_undotb}
	  {opt.tool.zoom ho.opt.tool.zoom opts_zoomtb}
	  {opt.tool.mode ho.opt.tool.mode opts_modetb}
	  {opt.tool.module ho.opt.tool.module opts_moduletb}
	  {opt.tool.tech ho.opt.tool.tech opts_techtb}
    }
    OptionDlg::frameToolbar $w.opts [m opt.gen.basetoolbar] $base_toolbars
    pack $w.opts -fill x -padx 5 -pady 5 
  }

  proc pageToolbarsSimulation {w} {
    frame $w

    set base_toolbars {
	  {opt.tool.simctl ho.opt.tool.simctl opts_simctltb}
	  {opt.tool.simaux ho.opt.tool.simaux opts_simauxtb}
	  {opt.tool.simmode ho.opt.tool.simmode opts_simmodetb}
    }
    OptionDlg::frameToolbar $w.opts [m opt.gen.basetoolbar] $base_toolbars
    pack $w.opts -fill x -padx 5 -pady 5 
  }

  proc pageToolbarsSymboledit {w} {
    frame $w

    set base_toolbars {
	  {opt.tool.symmode ho.opt.tool.symmode opts_symmodestb}
	  {opt.tool.symedit ho.opt.tool.symedit opts_symedittb}
	  {opt.tool.symport ho.opt.tool.symport opts_symporttb}
	  {opt.tool.symshift ho.opt.tool.symshift opts_symshifttb}
	  {opt.tool.symopr ho.opt.tool.symopr opts_symoprtb}
    }
    OptionDlg::frameToolbar $w.opts [m opt.gen.basetoolbar] $base_toolbars
    pack $w.opts -fill x -padx 5 -pady 5 
  }


  proc pageGeneralBasic {w} {
    frame $w
    frameIdentity $w.id
    frameBasic $w.basic
    pack $w.id -fill x -padx 5 -pady 5 -anchor n
    pack $w.basic -fill x -padx 5 -pady 5 -anchor n
  }

  proc pageInterface {w} {
    frame $w
    frameInput $w.input
    frameDisplay $w.display
    frameGUI $w.gui
    pack $w.input $w.display $w.gui -fill x -padx 5 -pady 5 -anchor n
  }

  # Disabled for now
  proc pageCircuit {w} {
    frame $w
    frameCircuitDefaults $w.defaults
    pack $w.defaults -fill x -padx 5 -pady 5 -anchor n
  }

  proc pageHDLEditor {w} {
    frame $w
    frameHDLEditor $w.edit
    frameHDLAutoindent $w.autoindent
    pack $w.edit $w.autoindent -fill x -padx 5 -pady 5 -anchor n
  }

  proc pageHDLColor {w} {
    frame $w
    frameHDLColor $w.color
    pack $w.color -fill both  -padx 5 -pady 5 -expand 1
  }

  proc pageColorEdit {w} {
    frame $w
    frameColorEdit $w.color
    pack $w.color -fill x -padx 5 -pady 5 -anchor n
  }

  proc pageColorScope {w} {
    frame $w
    frameColorScope $w.color
    pack $w.color -fill x -padx 5 -pady 5 -anchor n
  }

  proc pageSimulateBasic {w} {
    frame $w
    frameSimulateBasic $w.simbase
    pack $w.simbase -fill x -padx 5 -pady 5 -anchor n
  }

  proc pageSimulateTech {w} {
    frame $w
    frameSimulateTech $w.simtech
    pack $w.simtech -fill x -padx 5 -pady 5 -anchor n
  }

  proc pageSimulateErrors {w} {
    frame $w
    frameSimulateErrors $w.simerr
    pack $w.simerr -fill x -padx 5 -pady 5 -anchor n
  }

  proc pageLibraryPath {w} {
    frame $w
    frameLibraryPath $w.libpath
    pack $w.libpath -fill both -padx 5 -pady 5 -anchor n -expand 1
  }

  proc pageLibraryLoad {w} {
    frame $w
    frameLibraryLoad $w.libload
    pack $w.libload -fill both -padx 5 -pady 5 -anchor n -expand 1
  }

  proc pageLibraryVPD {w} {
    frame $w
    frameLibraryVPD $w.libVPD
    pack $w.libVPD -fill both -padx 5 -pady 5 -anchor n -expand 1
  }

  proc pageSecurity {w} {
    frame $w

    frameSecurityFeatures $w.feat
    frameSecurityExecLevel $w.exec
    frameSecurityHandle $w.handle

    pack $w.feat $w.exec $w.handle -fill x -padx 5 -pady 5
  }


  proc pageHTML {w} {
    frame $w

    frameHTML $w.html

    pack $w.html -fill x -padx 5 -pady 5
  }

  proc pageDebug {w} {
    frame $w

    frameDebug $w.debug

    pack $w.debug -fill x -padx 5 -pady 5
  }

  proc pagePrint {w} {
    frame $w

    PrintDlg::frameLprSelect $w.lpr -relief groove -radioframe 0 -fromoptions 1
    PrintDlg::framePaperSelect $w.pap right
    pack $w.lpr -fill x -pady 5 -padx 5
    pack $w.pap -fill x -pady 5 -padx 5
  }

  proc pagePrintContent {w} {
    frame $w

    PrintDlg::frameStyleSelect $w.sty groove
    pack $w.sty -fill x -pady 5 -padx 5
  }

  proc restorePageDefaults {} {
    variable currentPage
    Option::restoreDefaults $currentPage
  }

  proc restoreAllDefaults {} {
    Option::restoreDefaults
  }

  proc buttonBar {w} {
    frame $w

    button $w.ok -text [m b.ok] -command OptionDlg::ok
    button $w.restoreall -text [m opt.bbar.restoreall] -command OptionDlg::restoreAllDefaults
    button $w.restore -text [m opt.bbar.restore] -command OptionDlg::restorePageDefaults
    button $w.cancel -text [m b.cancel] -command OptionDlg::cancel

    pack $w.ok -side left -padx 5 -pady 5
    pack $w.cancel -side right -padx 5 -pady 5
    pack $w.restore -side right -padx 5 -pady 5
    pack $w.restoreall -side right -padx 5 -pady 5
  }

  proc seeB1Press {w x y X Y panel_w} {
    set label [Tree::labelat $w $x $y]

    if { $label == "" } return

    Tree::setselection $w $label
    selectPage $panel_w $label
  }

  proc selectPage {panel_w label} {
    variable currentPage

    foreach p [winfo children $panel_w] {
      pack forget $p
    }

    set pack_w ""
    switch $label {
      / {
	set pack_w $panel_w.general_basic
      }
      /General {
	set pack_w $panel_w.general_basic
      }
      /Interface {
	set pack_w $panel_w.interface
      }
      /Toolbars {
	set label /Toolbars/Edit
	set pack_w $panel_w.toolbars_edit
      }
      /Toolbars/Edit {
	set pack_w $panel_w.toolbars_edit
      }
      /Toolbars/Simulation {
	set pack_w $panel_w.toolbars_simulation
      }
      /Toolbars/Symboledit {
	set pack_w $panel_w.toolbars_symboledit
      }
      /HDL {
	set pack_w $panel_w.hdl_editor
      }
      /Print {
	set label /Print/Output
	set pack_w $panel_w.print
      }
      /Print/Output {
	set pack_w $panel_w.print
      }
      /Print/Content {
	set pack_w $panel_w.print_content
      }
      /Color {
	set label /Color/Edit
	set pack_w $panel_w.color_edit
      }
      /Color/Edit {
	set pack_w $panel_w.color_edit
      }
      /Color/Scope {
	set pack_w $panel_w.color_scope
      }
      /Color/HDL {
	set pack_w $panel_w.hdl_color
      }
      /Simulate {
	set label /Simulate/Basic
	set pack_w $panel_w.simulate_basic
      }
      /Simulate/Basic {
	set pack_w $panel_w.simulate_basic
      }
      /Simulate/Technology {
	set pack_w $panel_w.simulate_tech
      }
      /Simulate/Error {
	set pack_w $panel_w.simulate_errors
      }
      /Library {
	set label /Library/Path
	set pack_w $panel_w.library_path
      }
      /Library/Path {
	set pack_w $panel_w.library_path
      }
      /Library/Load {
	set pack_w $panel_w.library_load
      }
      /Library/VPD {
	set pack_w $panel_w.library_vpd
      }
      /Security {
	set pack_w $panel_w.security
      }
      /HTML {
	set pack_w $panel_w.html
      }
      /Debug {
	set pack_w $panel_w.debug
      }
    }

#      /Circuit {
#	set pack_w $panel_w.circuit
#      }

    set currentPage $label

    if { $pack_w != ""} {
      pack $pack_w -fill both -expand 1
    }

  }

  #############################################################################
  #
  # User pressed the cancel button on the dialog box
  #
  proc cancel {} {
    destroy .opts
  }

  #############################################################################
  #
  # User pressed the ok button on the dialog box
  #
  proc ok {} {
    store
    destroy .opts
  }

  #############################################################################
  #
  # This function is called after clicking OK on the options dialog box.
  #
  proc store {} {
    global opts_keyBindings tkg_keyBindings tkg_gateHome
    set rereadbindings 0

    if {$opts_keyBindings != $tkg_keyBindings} {
      set rereadbindings 1
    }

    Option::dialogSave

#    gat_reinitDelay
    gat_setcolors
    Option::writePreferences

#    if {$rereadbindings} {
#      KeyBinding::clearAll
#      source $tkg_gateHome/bindings/$tkg_keyBindings
#    }
  }

  proc post {} {
    global tkg_allowDebug tkg_statusMessage

    #
    # Do not allow while simulator is active
    #
    if {[gat_getMajorMode] == "simulate" } {
      set tkg_statusMessage [m err.notinsim]
      bell
      return
    }

    set w .opts

    Option::dialogLoad



    if { [catch {toplevel $w}] } { return }
    wm resizable $w 0 0
    wm title $w [m opt.title]
    wm geometry $w [offsetgeometry . 50 50]
    wm transient $w .

    frame $w.left
    label $w.left.l -text [m opt.treelabel]
    Tree::create $w.left.tree -width [rescale [m @opt.tree.width]] -yscrollcommand "$w.left.vb set" -sorted 0
    Tree::newitem $w.left.tree /General -text [m opt.general]
    Tree::newitem $w.left.tree /Interface -text [m opt.interface]
#    Tree::newitem $w.left.tree /Circuit -text [m opt.circuit]
    Tree::newitem $w.left.tree /Toolbars -text [m opt.toolbars]
    Tree::newitem $w.left.tree /Toolbars/Edit -text [m opt.toolbars.edit]
    Tree::newitem $w.left.tree /Toolbars/Simulation -text [m opt.toolbars.sim]
    Tree::newitem $w.left.tree /Toolbars/Symboledit -text [m opt.toolbars.symed]
    Tree::newitem $w.left.tree /HDL -text [m opt.editor]
    Tree::newitem $w.left.tree /Print -text [m opt.print]
    Tree::newitem $w.left.tree /Print/Output -text [m opt.print.output]
    Tree::newitem $w.left.tree /Print/Content -text [m opt.print.content]
    Tree::newitem $w.left.tree /Simulate -text [m opt.simulate]
    Tree::newitem $w.left.tree /Simulate/Basic -text [m opt.simulate.basic]
    Tree::newitem $w.left.tree /Simulate/Technology -text [m opt.simulate.tech]
    Tree::newitem $w.left.tree /Simulate/Error -text [m opt.simulate.error]
    Tree::newitem $w.left.tree /Library -text [m opt.library]
    Tree::newitem $w.left.tree /Library/Path -text [m opt.library.path]
    Tree::newitem $w.left.tree /Library/Load -text [m opt.library.load]
    Tree::newitem $w.left.tree /Library/VPD -text [m opt.library.vpd]
    Tree::newitem $w.left.tree /Security -text [m opt.security]
    Tree::newitem $w.left.tree /Color -text [m opt.color]
    Tree::newitem $w.left.tree /Color/Edit -text [m opt.color.edit]
    Tree::newitem $w.left.tree /Color/Scope -text [m opt.color.scope]
    Tree::newitem $w.left.tree /Color/HDL -text [m opt.color.hdl]
    Tree::newitem $w.left.tree /HTML -text [m opt.html]

    if { $tkg_allowDebug } {
      Tree::newitem $w.left.tree /Debug -text [m opt.debug]
    }

    scrollbar $w.left.vb -orient vertical -command "$w.left.tree yview" -takefocus 0
    pack $w.left.l -anchor w
    pack $w.left.vb -side right -fill both
    pack $w.left.tree -anchor w -fill y -expand 1

    bind $w.left.tree <1> "OptionDlg::seeB1Press %W %x %y %X %Y $w.right"

    #
    # Initial guess of size
    #
    set width 0
    set height 0

    frame $w.top_strut -width $width -height 10 -bd 0
    frame $w.left_strut -height $height -bd 0

    frame $w.right

    pageGeneralBasic $w.right.general_basic
    pageInterface $w.right.interface
#    pageCircuit $w.right.circuit
    pageToolbarsEdit $w.right.toolbars_edit
    pageToolbarsSimulation $w.right.toolbars_simulation
    pageToolbarsSymboledit $w.right.toolbars_symboledit
    pageHDLEditor $w.right.hdl_editor
    pageHDLColor $w.right.hdl_color
    pageColorEdit $w.right.color_edit
    pageColorScope $w.right.color_scope
    pageSimulateBasic $w.right.simulate_basic
    pageSimulateTech $w.right.simulate_tech
    pageSimulateErrors $w.right.simulate_errors
    pageLibraryPath $w.right.library_path
    pageLibraryLoad $w.right.library_load
    pageLibraryVPD $w.right.library_vpd
    pageSecurity $w.right.security
    pageHTML $w.right.html
    pagePrint $w.right.print
    pagePrintContent $w.right.print_content
    pageDebug $w.right.debug


    buttonBar $w.b
    pack $w.b -fill both -side bottom

    pack $w.left -padx 3 -pady 3 -side left -fill y -expand 1
    pack $w.top_strut -side top
    pack $w.left_strut -side left

    pack $w.right -fill both -expand 1



    #
    # Make sure page layout has been done
    #
    update

    #
    # Look for largest option page (in each dimension) and use it to
    # set the size of the dialog box.
    #
    foreach page {general_basic interface toolbars_edit toolbars_simulation
      toolbars_symboledit hdl_editor hdl_color color_edit color_scope
      simulate_basic simulate_tech simulate_errors library_path
      library_load library_vpd security html print print_content debug} {

      set pw $w.right.$page
      set pgWidth [expr [winfo reqwidth $pw] + 4*[$pw cget -bd]]
      set pgHeight [expr [winfo reqheight $pw] + 4*[$pw cget -bd]]
      
      if {$pgWidth > $width} { set width $pgWidth }
      if {$pgHeight > $height} { set height $pgHeight }
    }


    $w.top_strut configure -width $width -height [rescale *10]
    $w.left_strut configure -height $height

    pack $w.right.general_basic -fill both

    #
    # We need the update because the tree does not actually get built until
    # a tcl/tk idle period occurs.
    #
    update
    Tree::setselection $w.left.tree /General
  }
}
