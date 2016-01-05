#!/usr/bin/wish
#
# I am D. Richard Hipp, the author of this code.  I hereby
# disavow all claims to copyright on this program and release
# it into the public domain. 
#
#                     D. Richard Hipp
#                     January 31, 2001
#
# As an historical record, the original copyright notice is
# reproduced below:
#
# Copyright (C) 1997,1998 D. Richard Hipp
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
# 
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA  02111-1307, USA.
#
# Author contact information:
#   drh@acm.org
#   http://www.hwaci.com/drh/
#
# $Revision: 1.7 $
#
#############################################################################
#
# This file contains modifications by Jeffery P. Hansen.  Modifications to
# this file only can be considered public domain.
#

image create photo idir -data {
    R0lGODdhEAAQAPIAAAAAAHh4eLi4uPj4APj4+P///wAAAAAAACwAAAAAEAAQAAADPVi63P4w
    LkKCtTTnUsXwQqBtAfh910UU4ugGAEucpgnLNY3Gop7folwNOBOeiEYQ0acDpp6pGAFArVqt
    hQQAO///
}
image create photo ifile -data {
    R0lGODdhEAAQAPIAAAAAAHh4eLi4uPj4+P///wAAAAAAAAAAACwAAAAAEAAQAAADPkixzPOD
    yADrWE8qC8WN0+BZAmBq1GMOqwigXFXCrGk/cxjjr27fLtout6n9eMIYMTXsFZsogXRKJf6u
    P0kCADv/
}


namespace eval Tree {
  #
  # treeData - array containing all of the tree state information.
  #
  # The types of entries in this array are listed below where $w is the
  # window identifying the tree widget, $v is the path name for an entry
  # in the tree (e.g., "/foo/bar/beltch") and $k is the item number of
  # an image associated with an entry.
  #
  #   $w:nolines		Flag indicating lines should not be drawn.
  #   $w:selection		Name of the tree entry that is selected.
  #   $w:selidx			Identifier for the selection rectangle
  #   $w:width			Width of window that tree is in.
  #   $w:height			Height of window that tree is in.
  #   $w:x			x-coordinate used for building tree 
  #   $w:y			y-coordinate used for building tree 
  #   $w:iarray			Name of array with supplemental text for entries.
  #   $w:buildpending		Non-zero if we need to rebuild tree when idle
  #   $w:$v:children		Children of a tree entry.
  #   $w:$v:open		Non-zero if an entry is open.
  #   $w:$v:visible		Non-zero if an entry is visible.
  #   $w:$v:wasopen		Non-zero if an entry was open.
  #   $w:$v:pos			Position of entry
  #   $w:$v:active		Active level of an entry (0=black, 1=red, 2=red with underline).
  #   $w:$v:icon		Icon to display with entry
  #   $w:$v:tags		Tags attached to an entry
  #   $w:tag:$k			Entry associated with a tag.
  #
  variable treeData

  #
  # Height of each item in tree
  #
  variable itemHeight


  #
  # Create a new tree widget.  $args become the configuration arguments to
  # the canvas widget from which the tree is constructed.
  #
  proc create {w args} {
    variable treeData
    variable itemHeight
    global treeItemHeight

    set nolines 0
    set sorted 1
    set xargs [parseargs $args {-nolines -sorted} -partial]

    set treeData($w:sorted) $sorted
    set treeData($w:nolines) $nolines
    set itemHeight($w) $treeItemHeight

    eval canvas $w -bg white $xargs
    bind $w <Destroy> "Tree::tdestroy $w"
    dfltconfig $w /
    buildwhenidle $w
    set treeData($w:selection) {}
    set treeData($w:selidx) {}
  }

  # Initialize a element of the tree.
  # Internal use only
  #
  proc dfltconfig {w v} {
    variable treeData
    set treeData($w:$v:children) {}
    set treeData($w:$v:open) 0
    set treeData($w:$v:pos) {0 0}
    set treeData($w:$v:active) 0
    catch { set treeData($w:$v:open) $treeData($w:$v:wasopen) }
    set treeData($w:$v:icon) {}
    set treeData($w:$v:tags) {}
  }

  #
  # Pass configuration options to the tree widget
  #
  proc config {w args} {
    eval $w config $args
  }


  #
  # Insert a new element $v into the tree $w.
  #
  proc newitem {w v args} {
    variable treeData

    set dir [file dirname $v]
    set n [file tail $v]
    if {![info exists treeData($w:$dir:open)]} {
      error "parent item \"$dir\" is missing"
    }
    set i [lsearch -exact $treeData($w:$dir:children) $n]
    if {$i>=0} {
      error "item \"$v\" already exists"
    }
    lappend treeData($w:$dir:children) $n
    if { $treeData($w:sorted) } {
      set treeData($w:$dir:children) [lsort -dictionary $treeData($w:$dir:children) ]
    }
    dfltconfig $w $v

    set image ""
    set image2 ""
    set imagehelp ""
    set tags ""
    parseargs $args {-image -image2 -imagehelp -tags -text}

    #
    # Alternate text exists
    #
    if {[info exists text]} {
      set treeData($w:$v:text) $text
    }

    set treeData($w:$v:icon) $image
    set treeData($w:$v:icon2) $image2
    set treeData($w:$v:iconhelp) $imagehelp
    set treeData($w:$v:tags) $tags

    buildwhenidle $w
  }

  proc updateItem {w v item _value _changedp} {
    variable treeData

    upvar $_value value
    upvar $_changedp changedp

    if {[info exists value]} {
      if { $treeData($w:$v:$item) != $value } {
	set treeData($w:$v:$item) $value
	set changedp 1
      }
    }
  }

  proc configitem {w v args} {
    parseargs $args  {-image -image2 -imagehelp -tags}

    set changedp 0
    updateItem $w $v icon image changedp
    updateItem $w $v icon2 image2 changedp
    updateItem $w $v iconhelp imagehelp changedp
    updateItem $w $v tags tags changedp

    buildwhenidle $w
  }

  proc tdestroy {w} {
    catch {destroy $w}
    foreach t [array names treeData $w:*] {
      if {[regexp $w:(.*):open $t all v]} {
	set treeData($w:$v:wasopen) $treeData($w:$v:open)
	unset treeData($t)
      } elseif {![regexp $w:(.*):wasopen $t all v]} {
	unset treeData($t)
      }
    }
  }

  proc exists {w v} {
    return [info exists treeData($w:$v:open)]
  }

  #
  # Delete element $v from the tree $w.  If $v is /, then the widget is
  # deleted.
  #
  proc delitem {w v} {
    variable treeData

    if {![info exists treeData($w:$v:open)]} {
      return
    }
    if {[string compare $v /]==0} {
      # delete all items
      foreach t [array names treeData $w:*] {
	if {[string match "$w:/:*" $t] == 0 && [string match "$w:*:*" $t] != 0} {
	  if {[regexp $w:(.*):open $t all v]} {
	    set treeData($w:$v:wasopen) $treeData($w:$v:open)
	    unset treeData($t)
	  } elseif {![regexp $w:(.*):wasopen $t all v]} {
	    unset treeData($t)
	  }
	}
      }
      dfltconfig $w /
      set treeData($w:selection) {}
      set treeData($w:selidx) {}
      buildwhenidle $w
      return
    }

    foreach c $treeData($w:$v:children) {
      catch {delitem $w $v/$c}
    }

    set treeData($w:$v:wasopen) $treeData($w:$v:open)
    unset treeData($w:$v:open)
    catch { unset treeData($w:$v:text) }
    unset treeData($w:$v:pos)
    unset treeData($w:$v:active)
    unset treeData($w:$v:children)
    unset treeData($w:$v:icon)
    unset treeData($w:$v:icon2)
    unset treeData($w:$v:iconhelp)
    set dir [file dirname $v]
    set n [file tail $v]
    set i [lsearch -exact $treeData($w:$dir:children) $n]
    if {$i>=0} {
      set treeData($w:$dir:children) [lreplace $treeData($w:$dir:children) $i $i]
    }
    buildwhenidle $w
  }

  proc geticon {w v} {
    variable treeData
    return $treeData($w:$v:icon)
  }

  proc geticon2 {w v} {
    variable treeData
    return $treeData($w:$v:icon2)
  }

  #
  # Change the selection to the indicated item
  #
  proc setselection {w v} {
    variable treeData

    set treeData($w:selection) $v
    drawselection $w
  }

  # 
  # Retrieve the current selection
  #
  proc getselection w {
    variable treeData
    return $treeData($w:selection)
  }

  #
  # Bitmaps used to show which parts of the tree can be opened.
  #
  set maskdata "#define solid_width 9\n#define solid_height 9"
  append maskdata {
    static unsigned char solid_bits[] = {
      0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xff, 0x01,
      0xff, 0x01, 0xff, 0x01, 0xff, 0x01
    };
  }
  set data "#define open_width 9\n#define open_height 9"
  append data {
    static unsigned char open_bits[] = {
      0xff, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x7d, 0x01, 0x01, 0x01,
      0x01, 0x01, 0x01, 0x01, 0xff, 0x01
    };
  }
  image create bitmap Tree:openbm -data $data -maskdata $maskdata \
      -foreground black -background white
  set data "#define closed_width 9\n#define closed_height 9"
  append data {
    static unsigned char closed_bits[] = {
      0xff, 0x01, 0x01, 0x01, 0x11, 0x01, 0x11, 0x01, 0x7d, 0x01, 0x11, 0x01,
      0x11, 0x01, 0x01, 0x01, 0xff, 0x01
    };
  }
  image create bitmap Tree:closedbm -data $data -maskdata $maskdata \
      -foreground black -background white

  proc updateSize {w} {
    variable treeData

    set W [winfo width $w]
    set H [winfo height $w]
    set treeData($w:width) $W
    set treeData($w:height) $H
  }

  proc updateScrollRegion {w} {
    variable treeData

    updateSize $w

    set BB [$w bbox all]
    if { $BB == "" } return

    set W [expr $treeData($w:width) - 4]
    set H [expr $treeData($w:height) - 4]

    set x1 [lindex $BB 0]
    set y1 [lindex $BB 1]
    set x2 [lindex $BB 2]
    set y2 [lindex $BB 3]
    if {$x1 > 0} { set x1 0 }
    if {$y1 > 0} { set y1 0 }
    if {$x2 < $W} { set x2 $W }
    if {$y2 < $H} { set y2 $H }

    $w config -scrollregion [list $x1 $y1 $x2 $y2]
  }

  # Internal use only.
  # Draw the tree on the canvas
  proc build {w} {
    variable treeData

    foreach item [$w find withtag helpon] {
      helponitemcancel $w $item
    }

    $w delete all
    catch {unset treeData($w:buildpending)}
    set treeData($w:y) 15

    if { $treeData($w:nolines) } { 
      buildlayer $w / -10
    } else {
      buildlayer $w / 6
    }

    updateScrollRegion $w

    drawselection $w
  }

  proc getchildren {w v } {
    variable treeData
    return $treeData($w:$v:children)
  }

  #
  # Mark a the children and decendents of $v as invisible
  #
  proc markInvisible {w v} {
    variable treeData

    if {[string length $treeData($w:$v:children)]} {
      foreach c $treeData($w:$v:children) {
	set treeData($w:$v/$c:visible) 0
	markInvisible $w $v/$c
      }
    }
  }


  # Internal use only.
  # Build a single layer of the tree on the canvas.  Indent by $in pixels
  proc buildlayer {w v in} {
    variable treeData
    variable itemHeight

    if {$v=="/"} {
      set vx {}
    } else {
      set vx $v
    }

    set start [expr $treeData($w:y)-10]
    foreach c $treeData($w:$v:children) {
      set y $treeData($w:y)
      incr treeData($w:y) $itemHeight($w)
      if { !$treeData($w:nolines) } {
	$w create line $in $y [expr $in+10] $y -fill gray50 
      }
      set treeData($w:$vx/$c:visible) 1
      set icon $treeData($w:$vx/$c:icon)
      set icon2 $treeData($w:$vx/$c:icon2)
      set iconhelp $treeData($w:$vx/$c:iconhelp)
      set taglist x

      if {[info exists treeData($w:$vx/$c:text)]} {
	set text $treeData($w:$vx/$c:text)
      } else {
	set text $c
      }


      foreach tag $treeData($w:$vx/$c:tags) {
	lappend taglist $tag
      }
      set x [expr $in+12]

      set treeData($w:$vx/$c:pos) [list $x $y]

      if { $icon != "" && $icon2 != "" } {
	if { $iconhelp != "" } {
	  set ho_taglist $taglist
	  lappend ho_taglist helpon
	  set k [$w create image $x $y -image $icon -anchor w -tags $ho_taglist]
	  helponitem $w $k $iconhelp
	  incr x 16
	  set k2 [$w create image $x $y -image $icon2 -anchor w -tags $taglist]
	  incr x 20
	  set treeData($w:tag:$k) $vx/$c
	  set treeData($w:tag:$k2) $vx/$c
	} else {
	  set k [$w create image $x $y -image $icon -anchor w -tags $taglist]
	  incr x 16
	  set k2 [$w create image $x $y -image $icon2 -anchor w -tags $taglist]
	  incr x 20
	  set treeData($w:tag:$k) $vx/$c
	  set treeData($w:tag:$k2) $vx/$c
	}
      } elseif { $icon != "" } {
	if { $iconhelp != "" } {
	  set ho_taglist $taglist
	  lappend ho_taglist helpon
	  set k [$w create image $x $y -image $icon -anchor w -tags $ho_taglist]
	  helponitem $w $k $iconhelp
	} else {
	  set k [$w create image $x $y -image $icon -anchor w -tags $taglist]
	}
	incr x 20
	set treeData($w:tag:$k) $vx/$c
      }


      # JPH remove leading "|" in displayed text
      set display_text [string trimleft $text "|"]

      # JPH set dislay color and underlining effects
      switch $treeData($w:$vx/$c:active) {
	1 { set useFont Tree:actfont; set color red }
	2 { set useFont Tree:act2font; set color red }
	default { set useFont Tree:font; set color black }
      }

      # JPH - if the is auxilary text, append it.
      catch {
	global $treeData($w:iarray)
	set inst [lindex [array get $treeData($w:iarray) $c] 1]
	if { $inst != "" } {
	  set display_text "$display_text$inst"
	}
      }

      set j [$w create text $x $y -text $display_text -font $useFont \
		 -anchor w -tags $taglist -fill $color]

      set treeData($w:tag:$j) $vx/$c
      set treeData($w:$vx/$c:tag) $j
      if {[string length $treeData($w:$vx/$c:children)]} {
	if {$treeData($w:$vx/$c:open)} {
	  set j [$w create image $in $y -image Tree:openbm]
	  $w bind $j <1> "set Tree::treeData($w:$vx/$c:open) 0; Tree::build $w"
	  buildlayer $w $vx/$c [expr $in+18]
	} else {
	  set j [$w create image $in $y -image Tree:closedbm]
	  $w bind $j <1> "set Tree::treeData($w:$vx/$c:open) 1; Tree::build $w"
	  markInvisible $w $vx/$c
	}
      }
    }
    catch {
      if { !$treeData($w:nolines) } {
	set j [$w create line $in $start $in [expr $y+1] -fill gray50 ]
      }
      $w lower $j
    }
  }

  # Open a branch of a tree
  #
  proc open {w v} {
    variable treeData

#    setselection $w ""
    if {[info exists treeData($w:$v:open)] && $treeData($w:$v:open)==0
	&& [info exists treeData($w:$v:children)] 
	&& [string length $treeData($w:$v:children)]>0} {
      set treeData($w:$v:open) 1
      build $w
    }
  }

  proc close {w v} {
    variable treeData

#    setselection $w ""
    if {[info exists treeData($w:$v:open)] && $treeData($w:$v:open)==1} {
      set treeData($w:$v:open) 0
      build $w
    }
  }


  proc clearactive {w} {
    variable treeData

    foreach q [array names treeData "$w:*:active"] {
      set treeData($q) 0
    }
    buildwhenidle $w
  }

  #
  # Define an array with supplemental text for some nodes
  #
  proc setsuparray {w a} {
    variable treeData
    set treeData($w:iarray) $a
  }

  proc setactive {w v} {
    variable treeData

    set idx [string last "/" $v]
    set subV $v
    while { $idx > 0 } {
      set subV [string range $subV 0 [expr $idx-1]] 

      foreach q [array names treeData "$w:*$subV:active"] {
	set treeData($q) 1
      }
      set idx [string last "/" $subV]
    }

    set idx [string last "/" $v]
    foreach q [array names treeData "$w:*[string range $v [expr $idx+1] end]:active"] {
      set treeData($q) 2
    }
    buildwhenidle $w
  }

  proc entryBBox {w v} {
    variable treeData
    variable itemHeight

    set ascent [expr int($itemHeight($w)*0.5)]
    set descent [expr $itemHeight($w) - $ascent]

    set bbox [$w bbox $treeData($w:$v:tag)]
    catch {
      updateSize $w
      set pos $treeData($w:$v:pos)
      set x [lindex $pos 0]
      set y [lindex $pos 1]
      set bbox [list $x [expr $y - $descent] [expr $x + $treeData($w:width)+5] [expr $y + $ascent]]
    }
    return $bbox
  }

  # Internal use only.
  # Draw the selection highlight
  proc drawselection w {
    variable treeData

    if {[string length $treeData($w:selidx)]} {
      $w delete $treeData($w:selidx)
    }
    set v $treeData($w:selection)
    if {[string length $v]==0} return
    if { $treeData($w:$v:visible) == 0 } return
    if {![info exists treeData($w:$v:tag)]} return

    set bbox [entryBBox $w $v]

    if {[llength $bbox]==4} {
      set i [eval $w create rectangle $bbox -fill skyblue -outline {{}}]
      set treeData($w:selidx) $i
      $w lower $i
    } else {
      set treeData($w:selidx) {}
    }
  }

  # Internal use only
  # Call build then next time we're idle
  proc buildwhenidle w {
    variable treeData
    if {![info exists treeData($w:buildpending)]} {
      set treeData($w:buildpending) 1
      after idle "Tree::build $w"
    }
  }

  #
  # Return the full pathname of the label for widget $w that is located
  # at real coordinates $x, $y
  #
  proc labelat {w x y} {
    set x [$w canvasx $x]
    set y [$w canvasy $y]
    variable treeData

    set best_v ""
    set best_dy 0
    foreach m [$w find overlapping 0 [expr $y - 5] 500 [expr $y + 5]] {

      if {[info exists treeData($w:tag:$m)]} {
	set v $treeData($w:tag:$m)

	set bbox [entryBBox $w $v]

	if { $x >= [lindex $bbox 0] && $x <= [lindex $bbox 2] && \
		 $y >= [lindex $bbox 1] && $y <= [lindex $bbox 3] } {

	  set best_v $v
	}

#	set item_x [lindex $treeData($w:$v:pos) 0]
#	set item_y [lindex $treeData($w:$v:pos) 1]
#	set dy [expr $y-$item_y]
#	if { $dy < 0 } { set dy [expr -$dy] }
#	if { $best_v == "" || $dy < $best_dy } {
#	  set best_dy $dy
#	  set best_v $v
#	}


      }
    }
    return $best_v
  }
}

#toplevel .x
#Tree::create .x.t -nolines 1
#Tree::newitem .x.t /foo -image ifile
#Tree::newitem .x.t /bar -image ifile
#Tree::newitem .x.t /blat -image ifile
#pack .x.t
