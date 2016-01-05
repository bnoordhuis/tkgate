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
# Last edit by hansen on Tue Mar 17 05:44:35 2009
#

namespace eval LibraryMgr {
  variable libTitle
  variable libStatus
  variable libReqStatus
  variable leftMargin 8
  variable topMargin 8
  variable boldFont checklistBoldFont
  variable normalFont checklistFont
  variable lineHeight
  variable lineAscent
  variable lineDescent
  variable boxSize
  variable libraryList
  variable yPos

  #############################################################################
  #
  # Called at start-up to load the base libraries
  #
  proc loadBaseLibraries {} {
    global tkg_simVLibNames

    foreach name $tkg_simVLibNames {
      catch { gat_loadLibrary $name }
    }
  }

  #############################################################################
  #
  # Get the title of a library file.
  #
  proc gettitle {name} {
    set f [open $name]
    if { $f == "" } { return "" }

    while {![eof $f]} {
      set s [gets $f]

      if {[regexp "//: *property *title *= *(.*)" $s all title]} {
	close $f
	return $title
      }
    }
    close $f
    return ""
  }

  #############################################################################
  #
  # Scan the library path for the list of available libraries.
  #
  proc scanlibs args {
    global tkg_simVLibPath
    variable libTitle
    variable libStatus
    variable libReqStatus

    set path $tkg_simVLibPath
    if {[llength $args] > 0} {
      set path [lindex $args 0] 
    }

    set L {}

    foreach directory $path {
      set directory [namespace eval :: "eval concat $directory"]
      foreach file [glob -nocomplain $directory/*.v] {
	set lib [file rootname [file tail $file]]
	set title [gettitle $file]
	lappend L $lib
	set libTitle($lib) $title
	set libStatus($lib) [gat_libIsLoaded $lib]
	set libReqStatus($lib) $libStatus($lib)
      }
    }
    return [lsort -dictionary $L]
  }

  proc drawBox {w name} {
    variable libStatus
    variable libReqStatus
    variable boxSize
    variable leftMargin
    variable yPos

    set x $leftMargin
    set y $yPos($name)

    $w delete $name.box

    set x2 [expr $x + $boxSize]
    set y2 [expr $y - $boxSize]

    set tags [list box $name.box]
    $w create rectangle $x $y $x2 $y2 -tags $tags -fill white

    if { $libReqStatus($name) != 0} {
      $w create image [expr $x+1] $y -anchor sw -image [gifI checkon.gif] -tags $tags
    }
  }

  #############################################################################
  #
  #
  #
  proc makeLibLine {w name} {
    variable leftMargin
    variable libTitle
    variable libStatus
    variable libReqStatus
    variable lineDescent
    variable boldFont
    variable normalFont
    variable boxSize
    variable yPos

    set x $leftMargin
    set y $yPos($name)

    $w delete $name

    #
    # Draw the box
    #
    set boxTags [list $name box $name.box]
    set x2 [expr $x + $boxSize]
    set y2 [expr $y - $boxSize]
    $w create rectangle $x $y $x2 $y2 -tags $boxTags -fill white
    if { $libReqStatus($name) != 0} {
      $w create image [expr $x+1] $y -anchor sw -image [gifI checkon.gif] -tags $boxTags
    }
    set x [expr $x + $boxSize + [rescale *8]]
    set y [expr $y + $lineDescent]

    #
    # Draw the library name
    #
    $w create text $x $y -text $name -anchor sw -font $boldFont -tags $name
    set x [expr $x + [font measure $boldFont $name]]

    #
    # Draw load/unload status
    #
    if { $libStatus($name) != $libReqStatus($name) } {
      if { $libReqStatus($name) } {
	set status " \[load\]"
      } else {
	set status " \[unload\]"
      }
      $w create text $x $y -text $status -anchor sw -font $boldFont -fill red -tags $name
      set x [expr $x + [font measure $boldFont $status]]
    }

    #
    # Draw the library description
    #
    $w create text $x $y -text " - $libTitle($name)" -anchor sw -font $normalFont -tags $name
  }

  proc toggleBox {w y args} {
    variable lineHeight
    variable libraryList
    variable libReqStatus
    
    set index [expr int($y/$lineHeight-1)]

   if {$index >= [llength $libraryList]} return

    set name [lindex $libraryList $index]

    set libReqStatus($name) [expr !$libReqStatus($name)]

    makeLibLine $w $name
  }

  #############################################################################
  #
  # Commit library changes
  #
  proc commit {} {
    variable libraryList
    variable libReqStatus
    variable libStatus

    #
    # If variable does not exist, then we have not visited library page
    # and thus there is no reason to update library status
    # 
    if {![info exists libraryList]} return

    foreach name $libraryList {
      if {$libReqStatus($name) != $libStatus($name)} {
	if  { $libReqStatus($name) } {
	  gat_loadLibrary $name
	} else {
	  gat_unloadLibrary $name
	}
      }
    } 

  }

  #############################################################################
  #
  # Dialog for loading and unloading libraries.
  #
  proc create {w} {
    variable topMargin
    variable boldFont
    variable normalFont
    variable lineHeight
    variable lineAscent
    variable lineDescent
    variable boxSize
    variable libraryList
    variable yPos

    set lineAscent [font metrics $boldFont -ascent]
    set lineDescent [font metrics $boldFont -descent]
    set lineHeight [expr $lineAscent + $lineDescent]
    set boxSize 9

    frame $w

    set width [rescale [m @circ.library.width]]
    set height [rescale [m @circ.library.height]]
    canvas $w.c -bg white -bd 1 -relief sunken -yscrollcommand "$w.vb set" -width $width -height $height
    scrollbar $w.vb -command "$w.c yview"
    pack $w.c -fill both -expand 1 -side left
    pack $w.vb -side left -fill y

    $w.c bind box <Button-1> "LibraryMgr::toggleBox %W %y"

    set y [expr $topMargin + $lineHeight]

    set libraryList [scanlibs]

    foreach name $libraryList {
      set yPos($name) $y
      makeLibLine $w.c $name
      incr y $lineHeight
    }

    $w.c configure -scrollregion [list 0 0 0 $y]
  }
}

