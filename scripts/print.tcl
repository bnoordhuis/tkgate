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
# Last edit by hansen on Wed Jan 14 22:24:59 2009
#

set pd_isfile 1
set pd_file ""
set pd_traceFile ""
set pd_select all
set pd_oldSelect all
set pd_ok 0
set pd_selectList {}
set pd_epsf 0

set pd_inclib 0
set pd_incunused 0

set pd_filter ".ps"
set pd_filetypes {
    {{PostScript}	{.ps}}
    {{All Files}	*}
}

#############################################################################
#Some other common paper sizes not in the table below (in mm):
#Chu ban			220 x 288
#Ko bosho	    		330 x 470
#Mino, Hosokawa, Hodomura	330 x 850
#Mino ban, Hodomura ban 	610 x 860
#Naka bosho 			364 x 500
#O bosho 			394 x 530
#O biro bosho 			440 x 576
#Takenaga bosho 		515 x 757
#Mutsugiri 			203 x 254
#Yatsugiri (hele plaat) 	165 x 216
#Yotsugiri 			254 x 305
#Briefkaart (officieel) 	100 x 148
#Kiku 4 (boeken) 		227 x 306
#Kiku 5 (boeken) 		151 x 227
#Meishi (visitekaartje) 	55 x 91
#############################################################################



#############################################################################
#
# Desription of paper sizes that we support.
#
# The following variables are used for each of the columns respectively.
#    pd_printPgSize
#    pd_printPgSizeName
#
set paperSizeTable {
  {Letter	"Letter (8 1/2 x 11 in.)"	215	279}
  {Legal	"Legal (8 1/2 x 14 in.)"	215	355}
  {Tabloid	"Tabloid (11 x 17 in.)"		279	431}
  {Executive	"Executive (7 1/2 x 10 in.)"	190	254}
  {A4		"A4 (210 x 297 mm)"		210	297}
  {A3		"A3 (297 x 420 mm)"		297	420}
  {A2		"A2 (420 x 594 mm)"		420	594}
  {A1		"A1 (594 x 841 mm)"		594	841}
  {B5		"B5 (176 x 250 mm)"		176	250}
  {B4		"B4 (250 x 353 mm)"		250	353}
  {B3		"B3 (353 x 500 mm)"		353	500}
  {B2		"B2 (500 x 707 mm)"		500	707}
  {SRA2		"SRA2 (450 x 640 mm)"		450	640}
  {RA2		"RA2 (430 x 610 mm)"		430	610}
  {yatsugiri	"yatsugiri (6.5 x 8.5 in)"	165	215}
  {mutsugiri	"mutsugiri (8 x 10 in)"		203	254}
  {yotsugiri	"yotsugiri (10 x 12 in)"	254	304}
}

#############################################################################
#
# Get the command for printing on the specified printer
#
proc getPrintCommand {args} {
  global tkg_printDefCmdTmp tkg_printCmdTmp
  global opts_printer opts_printCommands

  #
  # Get name of requested printer
  #
  if { [llength $args] == 0 } {
    set name $opts_printer
  } else {
    set name [lindex $args 0]
  }

  set cmd [assoc $name $opts_printCommands]

  if { $cmd == "" || [llength $cmd] == 0 } {
    if { $opts_printer == "default" } {
      set PRINTER $name
      set printer $name
      set cmd [subst $tkg_printDefCmdTmp]
    } else {
      set PRINTER $name
      set printer $name
      set cmd [subst $tkg_printCmdTmp]
    }
  }

  return $cmd
}

#############################################################################
#
# Print a file with default values
#
proc tkg_printWithDefaults {isFile name} {
  global pd_isfile pd_file pd_ok
  global tkg_currentFile tkg_displayFile pd_printPgSize
  global pd_file pd_paper pd_printPgOrient pd_printStyle pd_select pd_selectList
  global pd_epsf pd_printIndex pd_printMergeSmall pd_printDuplex pd_inclib pd_printGraph 

  Option::dialogLoad -optclass {/Print/Output /Print/Content}
  Option::dialogSave -optclass {/Print/Output /Print/Content} -prefix pd

  set p [string last "/" $pd_file]
  if { $p >= 0 } { set pd_file [string range $pd_file [expr $p + 1] end]}


  if {$isFile} {
    gat_print -file $name -paper $pd_printPgSize -orient $pd_printPgOrient -style $pd_printStyle \
	-select $pd_select -modlist $pd_selectList -epsf $pd_epsf -index $pd_printIndex \
	-merge $pd_printMergeSmall -duplex $pd_printDuplex -graph $pd_printGraph \
	-inclib $pd_inclib
  } { 
    set printer [getPrintCommand $name]

    gat_print -printer $printer -paper $pd_printPgSize -orient $pd_printPgOrient -style $pd_printStyle \
	-select $pd_select -modlist $pd_selectList -epsf $pd_epsf -index $pd_printIndex \
	-merge $pd_printMergeSmall -duplex $pd_printDuplex -graph $pd_printGraph \
	-inclib $pd_inclib
  }
}

#############################################################################
#
# Dialog box for configuration of the printer list
#
namespace eval PrinterListDlg {
  variable printCommandEdit

  proc confirm {} {
    variable printCommandEdit
    global opts_printCommands
    global opts_printer

    set opts_printCommands [assocsetc $opts_printer $opts_printCommands $printCommandEdit]

    destroy .printerList
  }

  #############################################################################
  #
  # Post the dialog box for configuring the list of printers
  #
  proc post {pw} {
    global opts_printer
    variable printCommandEdit

    set w .printerList

    if { [catch {toplevel $w}] } {
      return
    }

    wm transient $w [gettoplevel $pw]
    wm resizable $w 0 0
    wm title $w [m print.config.title]
    wm geometry $w [offsetgeometry . 50 50 ]

    okcancel $w.okc -okcommand PrinterListDlg::confirm -cancelcommand { destroy .printerList }
    pack $w.okc -fill both -side bottom

    frame $w.main -bd 2 -relief raised

#    dialogImage $w.main.image -image [gifI printer.gif] -caption [m print.printcirc.cap] -width 175
#    pack $w.main.image -fill both -side left -pady 10

    set printCommandEdit [getPrintCommand $opts_printer]

    frame $w.main.pad1
    frame $w.main.pad2
    frame $w.main.conf
    label $w.main.conf.pname_l -text [m print.printer]
    label $w.main.conf.pname_e -text $PrintDlg::shortToLong($opts_printer)
    label $w.main.conf.pcmd_l -text [m print.prcmd]
    entry $w.main.conf.pcmd_e -textvariable PrinterListDlg::printCommandEdit -width 40 -bg white

    grid $w.main.conf.pname_l -row 0 -column 0 -padx 3 -pady 3 -sticky e
    grid $w.main.conf.pname_e -row 0 -column 1 -padx 3 -pady 3 -sticky w
    grid $w.main.conf.pcmd_l  -row 1 -column 0 -padx 3 -pady 3 -sticky e
    grid $w.main.conf.pcmd_e  -row 1 -column 1 -padx 3 -pady 3 -sticky w

    pack $w.main.pad1 -fill both -expand 1
    pack $w.main.conf
    pack $w.main.pad2 -fill both -expand 1

    pack $w.main -fill both -ipadx 15 -ipady 15

    dialogWait $w
  }
}

#############################################################################
#
# Dialog box for printing circuits
#
namespace eval PrintDlg {
  variable longToShort
  variable shortToLong
  variable printerList

  variable availableModules {}
  variable selectedModules {}

  proc init {} {
    trace variable ::longPrinterName w PrintDlg::mapLPNChange
    trace variable ::opts_printer w PrintDlg::optsPrinterChange
    trace variable ::opts_printPgSize w PrintDlg::updateExample
    trace variable ::opts_printPgOrient w PrintDlg::updateExample
  }
  proc uninit {} {
    trace vdelete ::longPrinterName w PrintDlg::mapLPNChange
    trace vdelete ::opts_printer w PrintDlg::optsPrinterChange
    trace vdelete ::opts_printPgSize w PrintDlg::updateExample
    trace vdelete ::opts_printPgOrient w PrintDlg::updateExample
  }

  #############################################################################
  #
  # Map a change in the long printer name to the short printer name
  #
  proc mapLPNChange args {
    global longPrinterName opts_printer   
    variable longToShort
    variable printerList

    if {[info exists longToShort($longPrinterName)]} {
      if { $opts_printer != $longToShort($longPrinterName) } {
	set opts_printer $longToShort($longPrinterName)
      }
    }
  }

  #############################################################################
  #
  # Called when the opts_printer variable changes so we can sync it to the long
  # printer name.
  #
  proc optsPrinterChange args {
    variable shortToLong
    variable printerList
    global opts_printer
    global longPrinterName

    catch {
      #
      # If opts_printer was set to an invalid printer name, reset it
      # to a valid name.
      #
      if {[assoc $opts_printer $printerList] == "" } {
	puts "fixing opts_printer"
	set opts_printer [lindex $printerList 0]
      } 

      if { $longPrinterName != $shortToLong($opts_printer) } {
	set longPrinterName $shortToLong($opts_printer)
      }
    }
  }

  proc fileChangeNotify args {
    global pd_file pd_traceFile
    global tkg_currentFile

    set pd_file       [replaceExtension $tkg_currentFile ".ps"]
    set pd_traceFile  [replaceExtension $tkg_currentFile "_trace.ps"]
  }

  #############################################################################
  #
  # Convert a long name to a basic name. (e.g. "Letter (8 1/2 x 11 in.)" to "Letter").
  # We do this without the paper size table
  #
  proc paperNameToSize {name} {
    if { [scan $name %s size] == 1 } {
      return $size
    } else {
      return Letter
    }
  }

  #############################################################################
  #
  # Convert a basic name (e.g., "A4") to the long name
  #
  proc paperSizeToName {name} {
    global paperSizeTable

    foreach s $paperSizeTable {
      if { $name == [lindex $s 0] } {
	return [lindex $s 1]
      }
    }
    return [lindex [lindex $paperSizeTable 0] 1]
  }

  #############################################################################
  #
  # Lookup a paper size from a basic name (e.g., "A4")
  #
  proc paperSizeLookup {name} {
    global paperSizeTable

    foreach s $paperSizeTable {
      if { $name == [lindex $s 0] } {
	return $s
      }
    }
    return {}
  }

  #############################################################################
  #
  # Update page example based on current settings
  #
  proc updateExample args {
    global opts_printPgSize opts_printPgOrient opts_prntDuplex
    global pageex_w

    catch {
      $pageex_w delete all

      set size [paperSizeLookup $opts_printPgSize]

      set W [expr [lindex $size 2]/10]
      set H [expr [lindex $size 3]/10]

      if { $opts_printPgOrient == "landscape" } {
	set t $W
	set W $H
	set H $t
      }

      $pageex_w create rectangle [expr 50 - $W/2]  [expr 50 - $H/2] [expr 50 + $W/2]  [expr 50 + $H/2] \
	  -outline black -fill white
    }
  }

  proc selectEPSF {} {
    global pd_epsf pd_isfile pd_select pd_oldSelect

    set w .prntdlg

    catch {
      set w1 .prntdlg.right.mod
      set w2 .prntdlg.right.style

      if { $pd_epsf && $pd_isfile } {
	TabBox::settabstate $w.main.tab Modules disabled
	TabBox::settabstate $w.main.tab Advanced disabled

	$w1.sel.all configure -foreground dimgray -state disabled
	$w1.sel.cur configure -foreground dimgray -state disabled
	$w1.sel.use configure -foreground dimgray -state disabled
	$w1.sel.sel configure -foreground dimgray -state disabled
	$w1.mods.lb configure -foreground dimgray -selectforeground dimgray
	$w1.lib     configure -foreground dimgray -state disabled
	$w2.idx     configure -foreground dimgray -state disabled
	$w2.graph   configure -foreground dimgray -state disabled
	$w2.mrgsml  configure -foreground dimgray -state disabled
	$w2.part.scale   configure -foreground dimgray -state disabled
	$w2.part.multi   configure -foreground dimgray -state disabled
	$w2.l   configure -foreground dimgray
	#	$w1.l   configure -foreground dimgray

	set pd_oldSelect $pd_select
	set pd_select cur

      } {
	TabBox::settabstate $w.main.tab Modules normal
	TabBox::settabstate $w.main.tab Advanced normal

	$w1.sel.all configure -foreground black -state normal
	$w1.sel.cur configure -foreground black -state normal
	$w1.sel.use configure -foreground black -state normal
	$w1.sel.sel configure -foreground black -state normal
	$w1.mods.lb configure -foreground black -selectforeground black
	$w1.lib     configure -foreground black -state normal
	$w2.idx     configure -foreground black -state normal
	$w2.graph   configure -foreground black -state normal
	$w2.mrgsml  configure -foreground black -state normal
	$w2.part.scale   configure -foreground black -state normal
	$w2.part.multi   configure -foreground black -state normal
	$w2.l   configure -foreground black
	#	$w1.l   configure -foreground black

	set pd_select $pd_oldSelect

      }
    }
  }



  #############################################################################
  #
  # Respond to change in printer page size dialog box.
  #
  proc setPaperSize {w psize} {
    global opts_printPgSize opts_printPgSizeName pageex_w

    set opts_printPgSize [paperNameToSize $opts_printPgSizeName]
    ScopePrintDlg::updateLPPEstimate
  }

  proc fpsInit {} {
    global opts_printPgOrient opts_printPgSize

    trace variable opts_printPgOrient w ScopePrintDlg::updateLPPEstimate
    trace variable opts_printPgSize w ScopePrintDlg::updateLPPEstimate
  } 

  proc fpsUninit {} {
    global opts_printPgOrient opts_printPgSize

    trace vdelete opts_printPgOrient w ScopePrintDlg::updateLPPEstimate
    trace vdelete opts_printPgSize w ScopePrintDlg::updateLPPEstimate
  } 

  #############################################################################
  #
  # Paper selection dialog.  This is shared by both the "options" dialog print
  # page and the print dialog box.
  #
  proc framePaperSelect {w args} {
    global opts_paper paperSizeTable opts_printPgSize paperSizeTable
    global pageex_w

    labelframe $w [m print.pageset]
    frame $w.pad
    pack $w.pad -pady 3

    canvas $w.example -width 100 -height 100 -bd 2 -relief sunken -bg gray
    set pageex_w $w.example
    pack $w.example -side right -padx 5 -pady 5
    updateExample

    frame $w.size

    label $w.size.l -text "[m print.papsz]:"

#    set curSize [paperSizeToName $opts_printPgSize]
#    Dropbox::new $w.size.mb -value $curSize -variable opts_printPgSizeName -width 23 -command PrintDlg::setPaperSize
    Dropbox::new $w.size.mb -variable opts_printPgSizeName -width 23
    foreach ps $paperSizeTable { 
      Dropbox::itemadd $w.size.mb [list [lindex $ps 1]]
    }

    pack $w.size.l $w.size.mb -side left -padx 5 -pady 5 -anchor w

    frame $w.o
    label $w.o.l -text "[m print.orient]: "
    radiobutton $w.o.p -text [m print.portrait] -variable opts_printPgOrient -value portrait
    radiobutton $w.o.ls -text [m print.landscape] -variable opts_printPgOrient -value landscape
    pack $w.o.l $w.o.p $w.o.ls  -side left -padx 5 -pady 5 -anchor w

    frame $w.dup
    checkbutton $w.dup.b -text [m print.2side] -variable opts_printDuplex
    pack  $w.dup.b -padx 5 -pady 5

    pack $w.size -anchor w -ipady 10
    pack $w.o -anchor w
    pack $w.dup -anchor w

    fpsInit
    bind $w <Destroy> PrintDlg::fpsUninit
    ScopePrintDlg::updateLPPEstimate
  }

  #############################################################################
  #
  # Style selection dialog.  This is shared by both the "options" dialog print
  # page and the print dialog box.
  #
  proc frameStyleSelect {w r} {
    labelframe $w [m print.opts]
    packPad $w

    frame $w.part
    radiobutton $w.part.scale -text [m print.scale] -variable opts_printStyle -value fit
    radiobutton $w.part.multi -text [m print.partition] -variable opts_printStyle -value part
    pack $w.part.scale $w.part.multi -anchor w

    checkbutton $w.idx -text [m print.incidx] -variable opts_printIndex
    checkbutton $w.graph -text [m print.incgraph] -variable opts_printGraph
    checkbutton $w.mrgsml -text [m print.4up] -variable opts_printMergeSmall

    frame $w.sp1
    frame $w.sp2

    pack $w.sp1 -anchor w -padx 5 -pady 5
    pack $w.part -anchor w -padx 5 
    pack $w.sp2 -anchor w -padx 5  -pady 10
    pack $w.idx -anchor w -padx 5 
    pack $w.graph -anchor w -padx 5 
    pack $w.mrgsml -anchor w -padx 5 
  }

  proc outputSelectNotify {lpr file} {
    global pd_isfile
    if { $pd_isfile } {
     #    ${lpr}_label configure -foreground dimgray
      $lpr.opts.l1 configure -foreground dimgray
      Dropbox::configure $lpr.opts.e1 -state disabled
      $lpr.opts.advanced configure -state disabled

      #    $lpr.opts.l2 configure -foreground dimgray
      #    $lpr.opts.e2 configure -foreground dimgray

      #    ${file}_label configure -foreground black
      $file.l configure -foreground black
      $file.e configure -foreground black -state normal
      $file.b configure -foreground black -state normal
      $file.epsf configure -foreground black -state normal
      focus $file.e
    } else {
      #    ${lpr}_label configure -foreground black
      $lpr.opts.l1 configure -foreground black
      Dropbox::configure $lpr.opts.e1 -state normal
      $lpr.opts.advanced configure -state normal
      #    $lpr.opts.l2 configure -foreground black
      #    $lpr.opts.e2 configure -foreground black



      #    ${file}_label configure -foreground dimgray
      $file.l configure -foreground dimgray
      $file.e configure -foreground dimgray -state disabled
      $file.b configure -foreground dimgray -state disabled
      $file.epsf configure -foreground dimgray -state disabled
      #    focus $lpr.e
    }
    selectEPSF
  }


  #############################################################################
  #
  # Helping function for frameModuleSelect.  Responds to changes in variables
  # that affect the modules available for printing.
  #
  proc fmsUpdateList {args} {
    variable availableModules
    variable selectedModules
    global pd_inclib pd_incunused
    
    set exclude {}
    if {!$pd_inclib} {
      set exclude {part}
    }

    if {!$pd_incunused} {
      set L {}
      foreach m [BlockList::getallmodules -exclude $exclude] {
	if {[BlockTree::isUsed $m]} {
	  lappend L $m
	}
      }
    } else {
      set L [BlockList::getallmodules -exclude $exclude]
    }

    set availableModules $L
  }

  #############################################################################
  #
  # Helping function for frameModuleSelect.  Sets up traces on variables.
  #
  proc fmsInit {w} {
    global pd_inclib
    global pd_incunused
    trace variable pd_inclib w "PrintDlg::fmsUpdateList"
    trace variable pd_incunused w "PrintDlg::fmsUpdateList"
  }

  #############################################################################
  #
  # Helping function for frameModuleSelect.  Removes traces on variables.
  #
  proc fmsUninit {w} {
    global pd_inclib
    global pd_incunused
    trace vdelete pd_inclib w "PrintDlg::fmsUpdateList"
    trace vdelete pd_incunused w "PrintDlg::fmsUpdateList"
  }
  
  #############################################################################
  #
  # Frame including selector to choose a set of modules to be printed. 
  #
  proc frameModuleSelect {w} {
    labelframe $w [m print.modulesel]
    packPad $w

    SetSelector::new $w.ss -availablelabel Available \
	-selectedlabel Selected \
	-availablevariable PrintDlg::availableModules \
	-selectedvariable PrintDlg::selectedModules

    checkbutton $w.lib -text [m print.inclib] -variable pd_inclib
    checkbutton $w.unused -text [m print.incunused] -variable pd_incunused

    fmsInit $w
    bind $w <Destroy> "PrintDlg::fmsUninit $w"

    pack $w.ss -fill both -expand 1
    pack $w.lib  -padx 5 -anchor w
    pack $w.unused  -padx 5 -anchor w
  }


  #############################################################################
  #
  # Printer selection dialog.  This is shared by both the "options" dialog print
  # page and the print dialog box.
  #
  proc frameLprSelect {w args} {
    global printers opts_printer
    variable longToShort
    variable shortToLong
    variable printerList

    #
    # Get the list of printers
    #
    set printerList [gat_getPrinterList]

    #
    # Set up arrays for mapping between short and long printer names
    #
    array unset longToShort
    array unset shortToLong
    foreach p $printerList {
      set longToShort([lindex $p 1]) [lindex $p 0]
      set shortToLong([lindex $p 0]) [lindex $p 1]
    }

    #
    # If current default printer is not valid, choose the first printer 
    # in the list as the default printer.
    #
    if {[assoc $opts_printer $printerList] == ""} {
      set newDefault [lindex [lindex $printerList 0] 0]
      set opts_printer $newDefault
    } 

    #
    # Set the long name of the printer
    #
    set longDefaultPrinter $shortToLong($opts_printer)



    set radioframe 1
    set relief groove
    set fromoptions 0
    parseargs $args {-relief -radioframe -fromoptions}

    if {$radioframe} {
      radioframe $w [m print.printersel] -relief $relief -bd 2 -variable pd_isfile -value 0
      ${w}_label configure -command "PrintDlg::outputSelectNotify $w [winfo parent $w].file"
    } else {
      labelframe $w [m print.defaultprinter]
    }

    frame $w.opts
    label $w.opts.l1  -text "[m print.prname]:"
    Dropbox::new $w.opts.e1 -value $longDefaultPrinter -variable longPrinterName

    foreach p $printerList {
      Dropbox::itemadd $w.opts.e1 \"[lindex $p 1]\"
    }

    #  label $w.opts.l2  -text "[m print.prcmd]"
    #  entry $w.opts.e2 -textvariable opts_printCommand -bg white

    pack $w.opts.l1 -pady 5 -padx 3 -side left -anchor w
    pack $w.opts.e1 -pady 5 -padx 3 -side left -anchor w -fill x

#    if { $fromoptions } {
      frame $w.opts.pad
      pack $w.opts.pad -fill x -expand 1
      button $w.opts.advanced -text [m print.advanced]  -command "PrinterListDlg::post $w"
      pack $w.opts.advanced -pady 5 -padx 3 -side right -anchor e
#    }


    #  grid $w.opts.l1 -row 0 -column 0 -pady 5 -padx 3 -sticky e
    #  grid $w.opts.e1 -row 0 -column 1 -pady 5 -padx 3 -sticky ew
    #  grid $w.opts.l2 -row 1 -column 0 -pady 5 -padx 3 -sticky e
    #  grid $w.opts.e2 -row 1 -column 1 -pady 5 -padx 3 -sticky ew

    pack $w.opts -side left -pady 10 -padx 10 -fill x -expand 1

    init
    bind $w <Destroy> PrintDlg::uninit
  }

  proc browseFile {which} {
    global pd_filter pd_filetypes pd_file pd_traceFile

    if { $which == "circuit" } {
      set fileNameVar pd_file
    } else {
      set fileNameVar pd_traceFile
    }

    set file [tk_getSaveFile -defaultextension $pd_filter -filetypes $pd_filetypes -parent .prntdlg]
    if { $file != "" } { 
      set $fileNameVar $file
      .prntdlg.left.file.e icursor end
      .prntdlg.left.file.e xview end
    }
  }

  #############################################################################
  #
  # File name selection
  #
  proc frameFileSelect {which w} {
    
    if { $which == "circuit" } {
      set fileNameVar pd_file
    } else {
      set fileNameVar pd_traceFile
    }

    radioframe $w [m print.filesel] -relief groove -bd 2 -variable pd_isfile -value 1
    ${w}_label configure -command "PrintDlg::outputSelectNotify [winfo parent $w].lpr $w"
    frame $w.pad
    pack $w.pad -pady 5
    label $w.l -text "[m print.filename]:"
    entry $w.e -textvariable $fileNameVar -width 30 -bg white
    button $w.b -text [m b.browse] -command "PrintDlg::browseFile $which"

    checkbutton $w.epsf -text [m print.epssave] -variable pd_epsf -command PrintDlg::selectEPSF

    pack $w.epsf -side bottom -pady 5 -padx 5 -anchor w
    pack $w.l -pady 3 -padx 5 -anchor w
    pack $w.e -side left -padx 5 -fill x
    pack $w.b -side left -padx 5
  }


  #############################################################################
  #
  # Handler for tabs on the print dialog box.
  #
  proc tabs {w tab} {
    frame $w
    frame $w.main
    pack $w.main -side right -fill both -expand 1
    frame $w.main.pad -height 5
    pack $w.main.pad

    switch $tab {
      Output {
	#      tkg_prntOutSel $w.main.out $w.main.lpr $w.main.file
	PrintDlg::frameLprSelect $w.main.lpr -relief groove
	PrintDlg::frameFileSelect circuit $w.main.file
	PrintDlg::outputSelectNotify $w.main.lpr $w.main.file
	PrintDlg::framePaperSelect $w.main.paper groove

	#      pack $w.main.out -padx 5 -pady 5 -fill both
	pack $w.main.lpr -padx 5 -pady 5 -fill both
	pack $w.main.file -padx 5 -pady 5 -fill both
	pack $w.main.paper -padx 5 -pady 5 -fill both

	set explaination [m print.printcirc.out.exp]
      }
      Modules {
	PrintDlg::frameModuleSelect $w.main.mod

	pack $w.main.mod -padx 5 -pady 5 -fill both -expand 1

	set explaination [m print.printcirc.mod.exp]
      }
      Advanced {
	PrintDlg::frameStyleSelect $w.main.style groove

	pack $w.main.style -padx 5 -pady 5 -fill both

	set explaination [m print.printcirc.mod.exp]
      }
    }

    dialogImage $w.image -image [gifI printer.gif] -caption [m print.printcirc.cap] -explaination $explaination
    pack $w.image -fill both -side left -pady 10

  }

  #############################################################################
  #
  # We have confirmed that we want to print.
  #
  proc confirm {} {
    global pd_ok pd_selectList
    variable availableModules
    variable selectedModules

    set pd_ok 1

    Option::dialogSave -optclass {/Print/Output /Print/Content} -prefix pd


    set pd_selectList {}
    foreach m $selectedModules {
      if {[lsearch $availableModules $m] >= 0} {
	lappend pd_selectList $m
      }
    }

    destroy .prntdlg
  }



  #############################################################################
  #
  # Post the dialog box for printing a circuit
  #
  proc post {} {
    global pd_isfile pd_file pd_ok
    global tkg_currentFile tkg_displayFile
    variable availableModules
    variable selectedModules

    set pd_file       [replaceExtension $tkg_currentFile ".ps"]


    fmsUpdateList
    set selectedModules $availableModules

    set w .prntdlg

    if { [catch {toplevel $w}] } {
      return
    }

    wm transient $w .
    wm withdraw $w

    set p [string last "/" $pd_file]
    if { $p >= 0 } { set pd_file [string range $pd_file [expr $p + 1] end]}

    set pd_ok 0

    wm resizable $w 0 0
    wm title $w [m print.title]
    wm geometry $w [offsetgeometry . 50 50 ]

    Option::dialogLoad -optclass {/Print/Output /Print/Content}

    set labels [list [m print.tab.output] [m print.tab.modules] [m print.tab.advanced]]
    TabBox::new $w.tab -tabs {Output Modules Advanced} -command PrintDlg::tabs -tablabels $labels

    okcancel $w.okc -okcommand PrintDlg::confirm -cancelcommand { destroy .prntdlg } -oktext [m print.ok]
    pack $w.okc -fill both -side bottom
    pack $w.tab -fill both

    ScopePrintDlg::updateLPPEstimate

    wm deiconify $w
    dialogWait $w

    if {$pd_ok} {
      global pd_file pd_paper pd_printPgOrient pd_printStyle pd_select pd_selectList
      global pd_epsf pd_printIndex pd_printMergeSmall pd_printDuplex pd_inclib pd_printGraph
      global pd_printPgSize

      if {$pd_isfile} {
	gat_print -file $pd_file -paper $pd_printPgSize -orient $pd_printPgOrient \
	    -style $pd_printStyle -select sel -modlist $pd_selectList -epsf $pd_epsf \
	    -index $pd_printIndex -merge $pd_printMergeSmall -duplex $pd_printDuplex \
	    -graph $pd_printGraph -inclib 0
      } { 
	set printer [getPrintCommand]
	gat_print -printer $printer -paper $pd_printPgSize -orient $pd_printPgOrient \
	    -style $pd_printStyle -select sel -modlist $pd_selectList -epsf $pd_epsf \
	    -index $pd_printIndex -merge $pd_printMergeSmall -duplex $pd_printDuplex \
	    -graph $pd_printGraph -inclib 0
      }
    }
  }
}

#############################################################################

namespace eval ScopePrintDlg {
  variable timeCorrection 
  variable startUnits ns
  variable rangeUnits ns
  variable startUnits_saved ns
  variable rangeUnits_saved ns
  variable pageLenUnits_saved ns
  variable start 0
  variable range 0
  variable pageLen
  variable pageLenUnits
  variable units
  variable traceBegin
  variable traceEnd
  variable isTest 0
  variable graphBegin
  variable graphEnd
  variable leftMargin
  variable rightMargin
  variable graphLength 
  variable graphHeight
  variable lppEstimate
  variable tg_w
  variable ppv_w
  variable pageEstimate
  variable linesEstimate
  variable ppvWidth 200
  variable ppvHeight 150
  
  #############################################################################
  #
  # Variable traces to keep the display region timeline updated.
  #
  proc init {} {
    trace variable ScopePrintDlg::start		w ScopePrintDlg::updateRange
    trace variable ScopePrintDlg::startUnits	w ScopePrintDlg::updateRange
    trace variable ScopePrintDlg::range		w ScopePrintDlg::updateRange
    trace variable ScopePrintDlg::rangeUnits	w ScopePrintDlg::updateRange
    trace variable ScopePrintDlg::pageLen	w ScopePrintDlg::updateRange
    trace variable ScopePrintDlg::pageLenUnits	w ScopePrintDlg::updateRange
    trace variable ScopePrintDlg::traceBegin	w ScopePrintDlg::updateRange
    trace variable ScopePrintDlg::traceEnd	w ScopePrintDlg::updateRange
  }
  proc uninit {} {
    trace vdelete ScopePrintDlg::start		w ScopePrintDlg::updateRange
    trace vdelete ScopePrintDlg::startUnits	w ScopePrintDlg::updateRange
    trace vdelete ScopePrintDlg::range		w ScopePrintDlg::updateRange
    trace vdelete ScopePrintDlg::rangeUnits	w ScopePrintDlg::updateRange
    trace vdelete ScopePrintDlg::pageLen	w ScopePrintDlg::updateRange
    trace vdelete ScopePrintDlg::pageLenUnits	w ScopePrintDlg::updateRange
    trace vdelete ScopePrintDlg::traceBegin	w ScopePrintDlg::updateRange
    trace vdelete ScopePrintDlg::traceEnd	w ScopePrintDlg::updateRange
  }

  #############################################################################
  #
  # Convert a units name to the units number
  #
  proc unitsNum {u} {
    switch $u {
      s  { return 1000000000000000.0 }
      ms { return 1000000000000.0 }
      us { return 1000000000.0 }
      ns { return 1000000.0 }
      ps { return 1000.0 }
      fs { return 1.0 }
    }
  }

  #############################################################################
  #
  # Convert a units number to the units name
  #
  proc numUnits {u} {
    if { $u >= 1000000000000000.0 }  { return "s"  }
    if { $u >= 1000000000000.0 } { return "ms" }
    if { $u >= 1000000000.0 } { return "us" }
    if { $u >= 1000000.0 } { return "ns" }
    if { $u >= 1000.0 } { return "ps" }
    if { $u >= 1.0 } { return "fs" }
  }

  #############################################################################
  #
  # Change the units of the start or range value
  #
  proc changeUnits {uvName vvName w args} {
    set old [set ${uvName}_saved]
    set newValue [set $uvName]

    set $vvName [expr [set $vvName] * ([unitsNum $old] / [unitsNum $newValue]) ]

    set ${uvName}_saved $newValue
  }

  #############################################################################
  #
  # Convert time with units to raw units
  #
  proc getRawTime {v u} {
    variable units

    return [expr $v * ([unitsNum $u]/[unitsNum $units])]
  } 

  #############################################################################
  #
  # Convert a time to an x value
  #
  proc timeToX {t} {
    variable graphBegin
    variable graphEnd
    variable leftMargin
    variable graphLength

    return [expr ($t-$graphBegin)/($graphEnd-$graphBegin)*$graphLength+$leftMargin]
  }

  #############################################################################
  #
  # Clip a raw time to remain in the range
  #
  proc clipTime {t} {
    variable traceBegin
    variable traceEnd

    if { $t < $traceBegin } {
      return $traceBegin
    }
    if { $t > $traceEnd } {
      return $traceEnd
    }

    return $t
  }

  #############################################################################
  #
  # Update page count if doing a trace print.
  #
  proc updateLPPEstimate args {
    global opts_printPgOrient opts_printPgSize
    variable lppEstimate

    set lppEstimate [gat_traceLPPEst  -orient $opts_printPgOrient -paper $opts_printPgSize]
    updatePageEstimate
  }

  #############################################################################
  #
  # Update display of the range of values selected for printing.
  #
  proc updateRange args {
    variable graphBegin
    variable graphEnd
    variable leftMargin
    variable rightMargin
    variable graphLength 
    variable graphHeight 
    variable units
    variable start
    variable startUnits
    variable range
    variable rangeUnits
    variable tg_w

    catch {
      set v1 [clipTime [getRawTime $start $startUnits]]
      set v2 [clipTime [expr $v1 + [getRawTime $range $rangeUnits]]]

      set x1 [timeToX $v1]
      set x2 [timeToX $v2]

      if { [expr $x2-$x1] < 1 } { set x2 [expr $x1 + 1]}

      set baseline [expr $graphHeight - 15]

      $tg_w.c delete theRange
      $tg_w.c create rectangle $x1 $baseline $x2 [expr $baseline-20] -fill gray50 -tags theRange -outline ""
      $tg_w.c lower theRange
      $tg_w.c lower theTrace
    }

    updatePageEstimate
  }

  proc updatePageEstimate args {
    variable lppEstimate
    variable pageEstimate
    variable linesEstimate
    variable start
    variable range
    variable units
    variable startUnits
    variable rangeUnits
    variable pageLen
    variable pageLenUnits

    catch {
      set rawPageLen [getRawTime $pageLen $pageLenUnits]
      set rawStart [clipTime [getRawTime $start $startUnits]]
      set rawEnd   [clipTime [expr $rawStart + [getRawTime $range $rangeUnits]]]
      set rawRange [expr $rawEnd - $rawStart]

      set linesEstimate	[expr $rawRange/$rawPageLen]
      set pageEstimate [expr int([ceil [expr [ceil $linesEstimate]/$lppEstimate]])]

      updatePagePreview
    }
  }

  proc loadRange {} {
    variable start
    variable range
    variable pageLen
    variable units
    variable startUnits
    variable rangeUnits
    variable pageLenUnits
    variable startUnits_saved
    variable rangeUnits_saved
    variable pageLenUnits_saved
    variable traceBegin
    variable traceEnd
    variable timeCorrection

    set data [gat_getTraceData]

    set start	   [lindex $data 0]
    set range	   [expr [lindex $data 1] - $start]
    set pageLen	   [lindex $data 2]
    set timeCorrection  [lindex $data 3]
    set units	   [lindex $data 4]
    set traceBegin [lindex $data 5]
    set traceEnd   [lindex $data 6]
    set startUnits $units
    set rangeUnits $units
    set pageLenUnits $units

    set start [expr $start * $timeCorrection]
    set range [expr $range * $timeCorrection]
    set pageLen [expr $pageLen * $timeCorrection]
    set traceBegin [expr $traceBegin * $timeCorrection]
    set traceEnd [expr $traceEnd * $timeCorrection]

    #
    # Set units for start value
    #
    set su [unitsNum $startUnits]
    while { $start > 1000 && $su < 1000000000000000.0 } {
      set start [expr $start / 1000.0 ] 
      set su [expr $su * 1000.0 ]
    }
    set startUnits [numUnits $su]
    set startUnits_saved $startUnits

    #
    # Set units for range value
    #
    set su [unitsNum $rangeUnits]
    while { $range > 1000 && $su < 1000000000000000.0 } {
      set range [expr $range / 1000.0 ] 
      set su [expr $su * 1000.0 ]
    }
    set rangeUnits [numUnits $su]
    set rangeUnits_saved $rangeUnits

    #
    # Set units for pageLen value
    #
    set su [unitsNum $pageLenUnits]
    while { $pageLen > 1000 && $su < 1000000000000000.0 } {
      set pageLen [expr $pageLen / 1000.0 ] 
      set su [expr $su * 1000.0 ]
    }
    set pageLenUnits [numUnits $su]
    set pageLenUnits_saved $pageLenUnits
  }


  #############################################################################
  #
  # Create the graph showing the range of time values in the full trace, and
  # the range selected for printing.
  #
  proc timegraph {w args} {
    variable traceBegin
    variable traceEnd
    variable units
    variable graphBegin
    variable graphEnd
    variable leftMargin
    variable rightMargin
    variable graphLength 
    variable graphHeight
    variable tg_w

    set tg_w $w

    set height 50
    set length 200
    parseargs $args {-length -height}
    set graphHeight $height
    set graphLength $length

    set leftMargin 15
    set rightMargin 15
    set graphLength [expr $graphLength - $leftMargin - $rightMargin]


    frame $w
    canvas $w.c -width [expr $graphLength + $leftMargin + $rightMargin] \
	-height $graphHeight -bg white -bd 2 -relief sunken
    pack $w.c -anchor w -padx 5 -pady 5

    set range [expr $traceEnd - $traceBegin]

    if { $range > 0 } {
      set interval [chooseInterval $range]

      set firstTick [floor [expr $traceBegin/$interval]]
      set lastTick [ceil [expr $traceEnd/$interval]]
      set N [expr $lastTick - $firstTick]

      set graphBegin [expr $firstTick * $interval]
      set graphEnd [expr $lastTick * $interval]

      set intervalUnits [unitsNum $units]
      while { $interval > 100 } {
	set interval [expr $interval / 1000]
	set intervalUnits [expr $intervalUnits * 1000.0]
      }
      set intervalUnits [numUnits $intervalUnits]

      set baseline [expr $graphHeight - 15]
      $w.c create line $leftMargin $baseline [expr $graphLength + $leftMargin] $baseline

      for { set t 0 } { $t  <= $N } { incr t } {
	set x [expr $graphLength * $t / ($N+0.0) + $leftMargin]
	$w.c create line $x $baseline $x [expr $baseline - 5]

	set text [format %g [expr ($t+$firstTick)*$interval]]
	if { $t >= [expr $N-1] } { set text $text$intervalUnits }

	if { [expr $t % 2] == 0} {
	  $w.c create text $x [expr $graphHeight - 5] -text $text
	}
      }

      set x1 [timeToX $traceBegin]
      set x2 [timeToX $traceEnd]
      $tg_w.c create rectangle $x1 $baseline $x2 [expr $baseline-20] -fill aquamarine -outline "" -tags theTrace
      $tg_w.c lower theTrace
    }

    helpon $w [m ho.print.timegraph]
  }

  proc timeselector {w args} {
    variable units

    frame $w

    set min 0
    set max 1000
    set unitvariable ""
    set valuevariable ""
    parseargs $args {-min -max -unitvariable -valuevariable}

    incdecEntry $w.ne -width 10 -variable $valuevariable -min 0 -class FloatEntry
    Dropbox::new $w.units -width 2 -variable $unitvariable -command "ScopePrintDlg::changeUnits $unitvariable $valuevariable"

    foreach u {s ms us ns ps fs} {
      Dropbox::itemadd $w.units $u
    }

    pack $w.ne $w.units -side left
  }

  proc updatePagePreview {} {
    global opts_printPgSize opts_printPgOrient opts_prntDuplex
    variable ppv_w
    variable lppEstimate
    variable ppvWidth
    variable ppvHeight
    variable linesEstimate

    set w $ppv_w.c

    $w delete all
#    $w create text 10 170 -text "[m print.estpg]: $ScopePrintDlg::pageEstimate" -tags pageCountStr -anchor sw

    set size [PrintDlg::paperSizeLookup $opts_printPgSize]

    set W [expr [lindex $size 2]/10]
    set H [expr [lindex $size 3]/10]
    if { $opts_printPgOrient == "landscape" } {
      set t $W
      set W $H
      set H $t
    }
#    puts "paper $W x $H"
    set W [expr $W * 2.8]
    set H [expr $H * 2.8]

    set N $ScopePrintDlg::pageEstimate

    if { $N > 10 } {
      set N 7
      set figW [expr $W + $N*5 + 3*8] 
      set figH [expr $H + $N*5 + 3*8] 
    } else {
      set figW [expr $W + $N*5] 
      set figH [expr $H + $N*5] 
    }

    set x0 [expr int($ppvWidth/2.0 - $figW/2.0) ]
    set y0 [expr int($ppvHeight/2.0 + $figH/2.0 - $H) ]
    set x $x0
    set y $y0

    for {set i 0} {$i < $N} {incr i} {
      set id [$w create rectangle $x $y [expr $x + $W] [expr $y + $H] -outline black -fill white]
      $w lower $id
      incr x 5
      incr y -5
    }

    #
    # Draw the dots if more than 10 pages
    #
    if { $N != $ScopePrintDlg::pageEstimate } {
      set x [expr int($x + $W)]
      incr x 3
      incr y -3
      $w create oval $x $y [expr $x + 3] [expr $y + 3] -outline black -fill black
      incr x 8
      incr y -8
      $w create oval $x $y [expr $x + 3] [expr $y + 3] -outline black -fill black
      incr x 8
      incr y -8
      $w create oval $x $y [expr $x + 3] [expr $y + 3] -outline black -fill black
    }

    #
    # Draw the "traces" on the first page
    #
    set x $x0
    set y $y0
    set traceSpace [expr ($H-10)/$lppEstimate]
    set traceGap [min 3 [expr $traceSpace/2.0]]
    set traceHeight [expr $traceSpace-$traceGap]
    if { $N > 1 } {
      if { $lppEstimate <= 1 } {
	$w create rectangle [expr $x+5] [expr $y+5] [expr $x+$W-5] [expr $y+5+$traceHeight] \
		-outline "" -fill gray60
      } else {
	set M [expr int($lppEstimate)]

	for {set i 0} { $i < $M } {incr i} {
	  $w create rectangle [expr $x+5] [expr $y+5+$i*$traceSpace] [expr $x+$W-5] \
		  [expr $y+5+($i+1)*$traceSpace-$traceGap] -outline "" -fill gray60
	  
	}
      }
    } else {
      set M [expr int($linesEstimate)]

      for {set i 0} { $i < $M } {incr i} {
	  $w create rectangle [expr $x+5] [expr $y+5+$i*$traceSpace] [expr $x+$W-5] \
		  [expr $y+5+($i+1)*$traceSpace-$traceGap] -outline "" -fill gray60
	  
      }

      set P [expr $linesEstimate-int($linesEstimate)]
      if { $P > 0} {
        set i $M
        $w create rectangle [expr $x+5] [expr $y+5+$i*$traceSpace] [expr $x+5+($W-10)*$P] \
	    [expr $y+5+($i+1)*$traceSpace-$traceGap] -outline "" -fill gray60
      }
    }

  }

  proc pagePreview {w} {
    variable ppvWidth
    variable ppvHeight
    variable ppv_w

    set ppv_w $w
    frame $w

    canvas $w.c -width $ppvWidth -height $ppvHeight -bg gray -bd 2 -relief sunken

    frame $w.estpg -bg gray -bd 2 -relief sunken
    label $w.estpg.l  -text "[m print.estpg]: " -bg gray
    label $w.estpg.pc -textvariable ScopePrintDlg::pageEstimate -bg gray

    pack $w.estpg.l -side left -anchor w
    pack $w.estpg.pc -side left -anchor w -fill both

    pack $w.c $w.estpg -fill both
  }

  proc rangeSelect {w} {
    labelframe $w [m print.rangeselection]
    packPad $w

    frame $w.bb
    label $w.bb.start_l -text [m print.start]:
    timeselector $w.bb.start -unitvariable ScopePrintDlg::startUnits -valuevariable ScopePrintDlg::start

    label $w.bb.range_l -text [m print.range]:
    timeselector $w.bb.range -unitvariable ScopePrintDlg::rangeUnits -valuevariable ScopePrintDlg::range

    timegraph $w.g

    pack $w.g -padx 3 -pady 3 -side right -anchor ne 
    pack $w.bb -padx 3 -pady 3 -side right -anchor ne 

    grid $w.bb.start_l -row 0 -column 0 -sticky e
    grid $w.bb.start   -row 0 -column 1
    grid $w.bb.range_l -row 1 -column 0 -sticky e
    grid $w.bb.range   -row 1 -column 1

    init
    bind $w <Destroy> ScopePrintDlg::uninit
  }

  proc scaleSelect {w} {
    set sc_max 100
    set sc_min 1

    labelframe $w [m print.scaleselect]
    frame $w.pad
    pack $w.pad -pady 5

    #
    # Old style display
    #
#    frame $w.b 
#    label $w.b.l -text "[m print.estpg]: "
#    label $w.b.pc -textvariable ScopePrintDlg::pageEstimate
#    pack $w.b.l $w.b.pc -side left -padx 5 -pady 5
#    pack $w.b -side bottom -anchor w

    
    frame $w.pageLen
    label $w.pageLen.l -text "[m print.linelen]: "
    timeselector $w.pageLen.ts -unitvariable ScopePrintDlg::pageLenUnits -valuevariable ScopePrintDlg::pageLen
    pack $w.pageLen.l $w.pageLen.ts -side left

    pagePreview $w.ppv

    pack $w.ppv -padx 5 -pady 5 -side right -anchor ne
    pack $w.pageLen -padx 5 -pady 5 -side right -anchor ne
  }

  proc confirm {} {
    global pd_ok

    set pd_ok 1
    destroy .prntdlg
  }

  proc tabs {w tab} {
    frame $w
    frame $w.main
    pack $w.main -side right -fill both -expand 1
    frame $w.main.pad -height 5
    pack $w.main.pad

    switch $tab {
      Output {
	PrintDlg::frameLprSelect $w.main.lpr -relief groove
	PrintDlg::frameFileSelect trace $w.main.file
	PrintDlg::outputSelectNotify $w.main.lpr $w.main.file
	PrintDlg::framePaperSelect $w.main.paper groove
	
	pack $w.main.lpr -padx 5 -pady 5 -fill both
	pack $w.main.file -padx 5 -pady 5 -fill both
	pack $w.main.paper -padx 5 -pady 5 -fill both

	set explaination [m print.printcirc.out.exp]
      }
      Content {
	ScopePrintDlg::rangeSelect $w.main.range
	ScopePrintDlg::scaleSelect $w.main.scale

	updateRange

	pack $w.main.range -padx 5 -pady 5 -fill x -expand 1 -anchor n
	pack $w.main.scale -padx 5 -pady 5 -fill x -expand 1 -anchor n

	set explaination [m print.printtrace.trace.exp]
      }
    }

    dialogImage $w.image -image [gifI printer.gif] -caption [m print.printtrace.cap] -explaination $explaination
    pack $w.image -fill both -side left -pady 10
  }

  #
  # Dialog box for scope traces
  #
  proc post {} {
    global pd_isfile pd_traceFile pd_ok
    global tkg_currentFile tkg_displayFile
    variable isTest
    variable start
    variable range
    variable pageLen
    variable startUnits
    variable rangeUnits
    variable pageLenUnits
    variable timeCorrection
    variable pageEstimate

    set pd_traceFile  [replaceExtension $tkg_currentFile "_trace.ps"]

    if { ![tkg_simCheck] && !$isTest } return

    loadRange

    set w .prntdlg

    if { [catch {toplevel $w}] } {
      return
    }

    catch { wm transient $w .scope }
    wm withdraw $w

    set pd_traceFile  [replaceExtension $tkg_currentFile "_trace.ps"]
    set p [string last "/" $pd_traceFile]
    if { $p >= 0 } { set pd_traceFile [string range $pd_traceFile [expr $p + 1] end]}

    set pd_ok 0

    wm resizable $w 0 0
    wm title $w [m print.scope.title]
    wm geometry $w [offsetgeometry . 50 50 ]

    Option::dialogLoad -optclass {/Print/Output /Print/Content}

    set labels [list [m print.tab.output] [m print.tab.content]]
    TabBox::new $w.tab -tabs {Output Content} -command ScopePrintDlg::tabs -tablabels $labels

    okcancel $w.okc -okcommand ScopePrintDlg::confirm -cancelcommand { destroy .prntdlg } -oktext [m print.ok]
    pack $w.okc -fill both -side bottom
    pack $w.tab -fill both

    wm deiconify $w
    dialogWait $w

    Option::dialogSave -optclass {/Print/Output /Print/Content} -prefix pd

    if {$pd_ok} {
      global pd_traceFile pd_paper pd_printPgOrient pd_epsf pd_printDuplex pd_printPgSize

      if { $pageEstimate > 40 } {
	if {[tk_messageBox -type yesno -default no -message [m err.manypages]] != "yes"} {
	  return
	}
      }
      
      set rawStart   [expr int([getRawTime $start $startUnits] / $timeCorrection + 0.5) ]
      set rawRange   [expr int([getRawTime $range $rangeUnits] / $timeCorrection + 0.5) ]
      set rawPageLen [expr int([getRawTime $pageLen $pageLenUnits] / $timeCorrection + 0.5) ]

      if {$pd_isfile} {
	gat_tracePrint -file $pd_traceFile -paper $pd_printPgSize -orient $pd_printPgOrient -epsf $pd_epsf \
	    -duplex $pd_printDuplex -scalelength $rawPageLen -start $rawStart -end [expr $rawStart + $rawRange]
      } { 
	set printer [getPrintCommand]
	gat_tracePrint -printer $printer -paper $pd_printPgSize -orient $pd_printPgOrient -epsf $pd_epsf \
	    -duplex $pd_printDuplex -scalelength $rawPageLen -start $rawStart -end [expr $rawStart + $rawRange]
      }
    }
  }

}

#############################################################################
#
# Initialization of print dialog boxes when we are running in interactive mode.
#
#
proc printDlgInit {} {
  global tkgate_isInitialized
  global opts_printPgSize opts_printPgSizeName
  global paperSizeTable

  set opts_printPgSizeName ""
  set opts_printPgSize ""

  linkVars opts_printPgSize opts_printPgSizeName $paperSizeTable
}

