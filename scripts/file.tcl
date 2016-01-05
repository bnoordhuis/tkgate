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
# Last edit by hansen on Wed Jan 14 23:18:28 2009
#
#

namespace eval File {
  variable newFileName ""
  variable newModuleName

  #############################################################################
  #
  # Respond to change in value of tkg_currentFile
  #
  proc curFileChanged args {
    global tkg_currentFile
    global tkg_displayFile
    global tkg_doBackupOnSave

    set name $tkg_currentFile
    set p [string last "/" $name]
    if { $p >= 0 } {
      set name [string range $name [expr $p + 1] end]
    }
    set tkg_displayFile $name
    set tkg_doBackupOnSave 1
  }

  proc openNewFile {} {
    destroy .tkgnew
    if {[string index $File::newFileName 0] != "/"} {
      gat_new $DirSelector::currentDir/$File::newFileName $File::newModuleName
    } else {
      gat_new $File::newFileName $File::newModuleName
    }
  }

  #############################################################################
  #
  # Create a new circuit
  #
  proc new {} {
    variable newFileName
    variable newModuleName
    global  tkg_modifiedFlag

    if { $tkg_modifiedFlag } {
      if { [confDelMods] != "yes" } {
	return;
      }
    }

    set w .tkgnew

    toplevel $w
    
    wm resizable $w 0 0
    wm title $w [m db.newcirc.title]
    wm geometry $w [offsetgeometry . 50 50 ]
    wm transient $w .

    set newFileName "new.v"
    set newModuleName "main"

    frame $w.top -bd 1 -relief raised
    dialogImage $w.top.di -image [gifI newcircuit.gif] -caption [m db.newcirc.cap] -explaination [m db.newcirc.exp]
    pack $w.top.di -fill both -side left

    frame $w.top.form

    DirSelector::new $w.top.form.ds -extensions .v -filevariable File::newFileName

    label $w.top.form.fnl -text "[m db.newcirc.file]: "
    entry $w.top.form.fne -textvariable File::newFileName -bg white

    label $w.top.form.tml -text "[m db.newcirc.topname]: "
    entry $w.top.form.tme -textvariable File::newModuleName -bg white


    grid columnconfigure $w.top.form 1 -weight 1
    grid $w.top.form.ds -row 0 -column 0 -columnspan 2 -sticky nsew
    grid $w.top.form.fnl -row 1 -column 0 -sticky e -padx 3 -pady 3
    grid $w.top.form.fne -row 1 -column 1 -sticky ew -padx 3 -pady 3
    grid $w.top.form.tml -row 2 -column 0 -sticky e -padx 3 -pady 3
    grid $w.top.form.tme -row 2 -column 1 -sticky ew -padx 3 -pady 3
    pack $w.top.form -padx 5 -pady 5 -fill both -expand 1

    okcancel $w.b -okcommand File::openNewFile -cancelcommand "destroy .tkgnew"

    pack $w.top
    pack $w.b -fill x

    dialogWait $w
  }

  #############################################################################
  #
  # Open a file after prompting for a file name.
  #
  proc load {} {
    global gf_filetypes gf_filter


    set load [tk_getOpenFile -defaultextension $gf_filter -filetypes $gf_filetypes ]
    if { $load != "" } {
      gat_load $load
    }
  }

  #############################################################################
  #
  # Open a libray 
  #
  proc loadLibrary {} {
#    global gf_filetypes gf_filter
#
#    set load [tk_getOpenFile -defaultextension $gf_filter -filetypes $gf_filetypes ]
#    if { $load != "" } {
#      gat_loadLibrary $load
#    }
    EditCircDB::post -starttab 1
  }

  #############################################################################
  #
  # Get a file name and save the current circuit data in it.
  #
  proc saveAs {} {
    global gf_filetypes gf_filter tkg_currentFile

    set save [tk_getSaveFile -defaultextension $gf_filter \
		  -filetypes $gf_filetypes -initialfile $tkg_currentFile]
    if { $save != "" } {
      gat_saveCircuit $save
    }
  }

  #############################################################################
  #
  # Save circuit data in the current file.
  #
  proc saveCurrent {} {
    global tkg_currentFile

    if { $tkg_currentFile != "" } {
      gat_saveCircuit $tkg_currentFile
    } else {
      File::saveAs
    }
  }

  #############################################################################
  #
  # Load the "novice" (welcome page) circuit.
  #
  proc loadWelcomeCircuit {} {
    global tkg_gateHome tkgate_tutorialdir

    gat_load $tkgate_tutorialdir/welcome.v
  }

  #############################################################################
  #
  # Confirm that user want to discard changes and start a new circuit.
  #
  proc confDelMods {} {
    global tkg_discardChanges

    if { $tkg_discardChanges } { set tkg_discardChanges 0; return "yes"; }

    set result [tk_messageBox -default no -type yesno -icon warning \
		    -message [m msg.reallynew]]
    return $result;
  }

  #############################################################################
  #
  # Called to shut down tkgate.  If the modified flag is set, make sure the
  # user really wants to quit first.
  #
  proc closeApplication {} {
    global tkg_modifiedFlag tkg_discardChanges
    
    if { $tkg_modifiedFlag && ! $tkg_discardChanges} {
      set result [tk_messageBox -default no -type yesno -icon warning \
		      -message [m msg.reallyquit]]
      if { "$result" != "yes" } { 
	return
      }
    }
    gat_exit
  }
}

namespace eval DirSelector {
  variable dirListWidth 400
  variable dirListHeight 150
  variable temp
  variable currentDir
  variable extensions
  variable filevariable

  proc setCurrentDir {m w d} {
    variable currentDir

    set currentDir $d
    updateDirMenu $m $w
    updateDirContents $m $w
  }

  proc dirHighlight {w id iid} {
    $w delete selbox
    set sbid [$w create rectangle [$w bbox $id $iid] -tags selbox -fill skyblue -outline ""]
    $w lower $sbid
  }

  proc upDir {m w} {
    variable currentDir

    setCurrentDir $m $w [file dirname $currentDir]
  }

  proc chdir {m w d} {
    variable currentDir

    if { [string range $d end-1 end] == "/"} {
      set d "$currentDir$d"
    } else {
      set d "$currentDir/$d"
    }

    setCurrentDir $m $w $d
  }

  proc setfile {m w d} {
    variable filevariable
    upvar \#0 $filevariable myfile

    set myfile $d
  }

  proc updateDirContents {m w} {
    variable dirListWidth
    variable dirListHeight
    variable currentDir
    variable extensions

    $w delete all

    set dirs [glob -directory $currentDir -types d -nocomplain "*"]
    set files [glob -directory $currentDir -types f -nocomplain "*$extensions"]

    set dirs [lsort $dirs]
    set files [lsort $files]

    set x 5
    set y 20
    set maxWidth 0

    foreach d $dirs {
      if {$y > [expr $dirListHeight - 5] } {
	incr x [expr $maxWidth + 40]
	set y 20
	set maxWidth 0
      }

      set d [file tail $d]
      set iid [$w create image $x $y -image [gifI directory.gif] -anchor sw]
      set id [$w create text [expr $x + 18] $y -text $d -anchor sw]
      scan [$w bbox $id] "%d %*d %d %*d" x1 x2
      set width [expr $x2 - $x1]
      if {$width > $maxWidth } { set maxWidth $width }

      $w bind $id <Button-1> "DirSelector::dirHighlight $w $id $iid"
      $w bind $iid <Button-1> "DirSelector::dirHighlight $w $id $iid"
      $w bind $id <Double-Button-1> "DirSelector::chdir $m $w $d"
      $w bind $iid <Double-Button-1> "DirSelector::chdir $m $w $d"

      incr y 15
    }
    foreach d $files {
      if {$y > [expr $dirListHeight - 5] } {
	incr x [expr $maxWidth + 40]
	set y 20
	set maxWidth 0
      }

      set d [file tail $d]
      set iid [$w create image $x $y -image [gifI file_new.gif] -anchor sw]
      set id [$w create text [expr $x + 18] $y -text $d -anchor sw]
      scan [$w bbox $id] "%d %*d %d %*d" x1 x2
      set width [expr $x2 - $x1]
      if {$width > $maxWidth } { set maxWidth $width }

      $w bind $id <Button-1> "DirSelector::dirHighlight $w $id $iid; DirSelector::setfile $m $w $d"
      $w bind $iid <Button-1> "DirSelector::dirHighlight $w $id $iid; DirSelector::setfile $m $w $d"
      $w bind $id <Double-Button-1> "File::openNewFile"
      $w bind $iid <Double-Button-1> "File::openNewFile"

      incr y 15
    }

    set width [expr $x + $maxWidth + 20]
    if { $width < $dirListWidth } { set width $dirListWidth }

    $w configure -scrollregion [list 0 0 $width $dirListHeight]
  }


  proc updateDirMenu {m w} {
    variable currentDir


    $m delete 0 end

    set d $currentDir
    set L {}
    set i 0
    while { $d != "/" } {
      lappend L $d
      set d [file dirname $d]
      incr i

      #
      # Just in case our assumptions about dirname are wrong
      #
      if {$i > 25} break
    }
    lappend L /

    for {set i [expr [llength $L] - 1]} { $i >= 0 } { incr i -1 } {
      set d [lindex $L $i]
      $m add command -label $d -command "DirSelector::setCurrentDir $m $w $d"
    }
  }


  #############################################################################
  #
  # Create a directory selector
  #
  proc new {w args} {
    variable dirListWidth
    variable dirListHeight
    variable temp
    variable extensions
    variable filevariable
    global tkg_gateHome

    set extensions {}
    set filevariable ""
    parseargs $args {-extensions -filevariable}

    frame $w
    frame $w.top
    label $w.top.dir -text [m db.newcirc.directory]
#    Dropbox::new $w.top.dbox -width 10 -height 10
#    Dropbox::itemadd $w "."
#    Dropbox::itemadd $w ".."

    #
    # Use the directory of the currently loaded file unless it is in the tkgate home directory.
    # In that case, use the current working directory.
    #
    gat_editCircProps load DirSelector::temp
    set DirSelector::currentDir  [file dirname $DirSelector::temp(file)]
    if {[string compare -length [string length $tkg_gateHome] $DirSelector::currentDir $tkg_gateHome] == 0} {
      set DirSelector::currentDir [pwd]
    }


    menubutton $w.top.dbox -indicatoron true -menu $w.top.dbox.m -textvariable DirSelector::currentDir -relief raised -direction flush 
    menu $w.top.dbox.m -tearoff 0

    toolbutton $w.top.updir	"updirectory.gif"	"DirSelector::upDir $w.top.dbox.m $w.list"	""

    pack $w.top.dir -padx 5 -pady 5 -side left
    pack $w.top.dbox -padx 5 -pady 5 -side left -fill x -expand 1
    pack $w.top.updir -padx 5 -pady 5 -side left
    pack $w.top -fill x

    canvas $w.list -bg white -width $dirListWidth -height $dirListHeight -takefocus 0  -xscrollcommand "$w.hb set" -bd 1 -relief sunken
    scrollbar $w.hb -command "$w.list xview" -takefocus 0 -orient horizontal
    pack $w.list $w.hb -padx 5 -pady 5 -fill x

    updateDirMenu $w.top.dbox.m $w.list
    updateDirContents $w.top.dbox.m $w.list
  }
}

trace variable tkg_currentFile w File::curFileChanged

