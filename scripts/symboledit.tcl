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
# Last edit by hansen on Fri Jan  9 08:58:18 2009
#


#############################################################################
#
# Bitmap editor
#
# Member functions are:
#     isactive				Returns non-zero if symbol editor is active.
#     hide				Shut down and hide the symbol editor
#     show				Make the symbol editor visible
#     seeDrag w er			Respond to a drag event in the port list
#     seeDragDrop port w x y		Respond to a drop of a dragged port name
#     closeEditIcon			Close the bitmap for an icon (normal or select)
#     openEditIcon			Open a icon marked visible for editing
#     openModule name			Open the named module in the symbol editor.
#     loadData				Load data about symbol from the tkgate C side.
#     update_bit x y			Update the color of a bit
#     setbit x y			Mark a bit as set and flag that there are bit value changes.
#     clrbit x y			Mark a bit as unset and flag that there are bit value changes.
#     cutbit x y			Copy a bit to the cut buffer and clear it.
#     copybit x y			Copy a bit to the cut buffer.
#     pastebit x y			Copy a bit from the paste buffer.
#     overlaybit x y			Overlay a bit from the paste buffer.
#     windowToCell w x y		Map window coordinates to cell coordinates.
#     cellToWindow w x y		Map cell coordinates to window coordinates.
#     zoom d				Set the zoom level.
#     reqPaint				Request that the fatbit window be repainted.
#     paint				Repaint the fatbit window be repainted.
#     cutRegion				Cut the selected region
#     copyRegion			Copy the selected region
#     pasteRegion			Paste the selected region
#     overlayRegion			Overlay the selected region
#     makeRubber x y 			Make a pixel appear as a "rubber" pixel.
#     doLine x1 y1 x2 y2 cmd		Apply a command to pixels on a line.
#     doRect x1 y1 x2 y2 cmd		Apply a command to pixels around a rectangle.
#     doFillRect x1 y1 x2 y2 cmd	Apply a command to pixels in a rectangle.
#     clearRubber			Clear rubber pixels
#     setPortSelect name		Set the selected port.
#     doPortHit w x y			Test for a port at the specified screen coordinates.
#     doPortMove w x y			Move selected port to a position.
#     seeB1Press w x y			Respond to a B1 mouse button press.
#     seeB3Press w x y			Respond to a B3 mouse button press.
#     seeB1Motion w x y			Respond to motion with mouse button B1 down.
#     seeB3Motion w x y			Respond to motion with mouse button B3 down.
#     seeB1Release w x y		Respond to a release of the B1 mouse button.
#     seeB3Release w x y		Respond to a release of the B3 mouse button.
#     decode w bmdata minx miny		Decode an X11 bitmap into the SymbolEdit internal data structures.
#     mkempty				Make the bitmap editor empty
#     encode xoff yoff			Encode the bitmap bmdata into a standard X11 bitmap form 
#     saveData args...			Save all data about symbol to the tkgate C side.
#     getOprRegion x1 y1 x2 y2 w h ...	Get the region over which to perform an action.
#     boldBits				Make the selected region bold.
#     rotateBits r			Rotate the selected regon.
#     shiftBits dx dy			Shift bits in the selected region.
#     rotatePort d			Rotate the selected port.
#     removePort name			Remove the selected port.
#     makePort name ...			Create a new port (or move/modify if existing)
#     createEditWin			Create the editor window
#     linkstatus w ...			Spreadsheet callback to create link normal/select checkbutton.
#     dotabs w tab			Respond to a press of the normal/select tab box.
#     create w ...			Create the symbol editor.
#
namespace eval SymbolEdit {
  #############################################################################
  # The "data" variable is used for passing information between the tcl script
  # and the binary side of tkgate.  The elements supported are:
  #
  #    name		Name of current module
  #    loadedName	Name of loaded module
  #    numports		Number of ports in module
  #    port:<n>		Spreadsheet data for port <n>
  #    pplace:<n>	Placement data for port <n>
  #    normalIcon	Normal icon data
  #    selectIcon	Select icon data
  #    normalOffset     x/y offset for normal icon
  #    selectOffset     x/y offset for select icon
  #    ok		ok indicator
  #
  # A port:<n> entry has the form:
  #     
  #     {name type nbits}
  #
  # The port:<n> entries are "suggested" ports generated from the module description.
  #
  # A pplace:<n> entry has the form:
  #
  #     {name x y type dir nbits}
  # 
  # pplace entires are the actual placed ports for a module.
  #
  variable data


  #############################################################################
  #
  # These varaibles are used by the symbol bitmap editor.
  #
  #   lbitem(x,y)       The large canvas item id for pixel (x,y)
  #   sbitem(x,y)       The small canvas item id for pixel (x,y)
  #   bits(x,y)		Array of pixel values
  #   cut(x,y)          The cut buffer
  #   width, height	Size of the bitmap in the editor
  #   scale		Current scale value
  #   emode		The current editing mode
  #
  variable bitem
  variable bits
  variable cut
  variable cut_width 0
  variable cut_height 0
  variable width 
  variable height 
  variable scale 7
  variable emode point
  variable paint_pending 
  variable x_base 0
  variable y_base 0
  variable linkSelected 1

  #
  # These variables set built-in limits
  # 
  variable minWidth 64
  variable minHeight 64
  variable maxWidth 256
  variable maxHeight 256

  # These variables are related to port graphical placement
  #
  # ports		This is an array mapping port name to a definition.  Each definition has the form:
  #   		     		 {name x y type orient nbits}
  #
  # port_select		Name of port that is selected.
  #
  variable ports
  variable port_select ""
  variable port_oldname ""

  #
  # x_set, y_set	Mouse down point for rubber band items and select regions
  # x1_sel, y1_sel	Lowest coordinate values of select region
  # x2_sel, y2_sel	Highest coordinate values of select region
  #
  variable x_set ""
  variable y_set ""
  variable x1_sel ""
  variable y1_sel ""
  variable x2_sel ""
  variable y2_sel ""

  #
  # Other variables used by the symbol editor
  #
  variable edit_w
  variable canv_w
  variable changed
  variable confirm
  variable visibleIcon


  #############################################################################
  #
  # Is the symbol editor active
  #
  proc isactive {} {
    global tabframe_w

    return [expr [llength [pack slaves $tabframe_w.smg]] == 0]

  }

  #############################################################################
  #
  # Hide the symbol editor window.
  #
  proc hide {} {
    global tabframe_w

    # If already hidden, do nothing.
    if { ![isactive] } return

#    puts SymbolEdit::hide
    SpreadSheet::configure $IPanel::portlist_w -dograb 1 -selectmode multiple -dragcommand ""

    pane forget $tabframe_w.smg
    pack $tabframe_w.smg.maing -fill both -expand 1
  } 

  #############################################################################
  #
  # Make the symbol editor window visible.
  #
  proc show {} {
    global tabframe_w

    # If already shown, do nothing.
    if { [isactive] } return

#    puts SymbolEdit::show
    SpreadSheet::configure $IPanel::portlist_w -dograb 0 -selectmode single -dragcommand SymbolEdit::seeDrag

    #
    # The "after 0" causes pop up of the symbol editor to be delayed.  For some reason
    # which I have not quite been able to figure out, this prevents an erronous window
    # layout condition.  This should be fixed and done properly when I get a chance.
    #
    after 0 {
      update
      update idletasks
      pack forget $tabframe_w.smg.maing
      pane $tabframe_w.smg.symed  $tabframe_w.smg.maing  -orient vertical -handlelook {-relief flat -height 10} \
	  -decoration paneDecoration -fractions { 0.7 0.3 }
    }
  }

  #############################################################################
  #
  # Respond to a drag of an element in the 
  #
  proc seeDrag {w er} {
    set port [SpreadSheet::get $w $er]
    if {[llength $port] == 0} return

    set dw [Dragger::make -command "SymbolEdit::seeDragDrop [list $port]" -grabpoint {-5 12 }]
    if {$dw != "" } {
      set label [lindex $port 0]
      set io [lindex $port 1]
      set bits [lindex $port 2]
      if {$bits != 1 } { set bits 2 }

      set image port_$io$bits.gif

      label $dw.icon -image [gifI $image]
      label $dw.label -text $label
      pack $dw.icon $dw.label -padx 2 -pady 2 -side left
    }
  }

  #############################################################################
  #
  # A port has been dropped.  Make sure it is on the canvas, then create/move
  # it to the specified position.
  #
  proc seeDragDrop {port w x y} {
    variable canv_w
    variable changed
    variable emode

#    puts "seeDragDrop port=$port w=$w x=$x y=$y"
    if { $w != $canv_w } return

    windowToCell $w x y

    set name [lindex $port 0]
    makePort $name -x $x -y $y
    setPortSelect $name
    set emode port
    set changed 1
  }

  #############################################################################
  #
  # Close the icon that is now visible in the editor
  #
  proc closeEditIcon {} {
    variable visibleIcon
    variable data

    #
    # Save the exisiting bitmap
    #
    switch $visibleIcon {
      normal {
	set data(normalIcon) [encode xoff yoff]
	set data(normalOffset) [list $xoff $yoff]
      }
      select {
	set data(selectIcon) [encode xoff yoff]
	set data(selectOffset) [list $xoff $yoff]
      }
      none {
      }
    }
  }

  #############################################################################
  #
  # Make the icon data for the 
  #
  proc openEditIcon {} {
    variable visibleIcon
    variable data
    variable edit_w

    set w $edit_w
    set idata ""
    set xoff 0
    set yoff 0

    switch ${visibleIcon} {
      normal {
	set idata $data(normalIcon)
	scan $data(normalOffset) "%d %d" xoff yoff
      }
      select {
	set idata $data(selectIcon)
	scan $data(selectOffset) "%d %d" xoff yoff
      }
      none {
      }
    }

    decode $w $idata $xoff $yoff
  }

  #############################################################################
  #
  # Open a module
  #
  proc openModule {name} {
    variable data

    clearPortSelect
#    puts "SymbolEdit::openModule $name"
    set data(name) $name
    loadData
  }

  #############################################################################
  #
  # Update the display of a bit
  #
  proc update_bit {x y} {
    variable bits
    variable lbitem
    variable sbitem
    global tkg_moduleColor
    variable canv_w

    set e ${x},$y

    if {$bits($e)} {
      $canv_w itemconfigure $lbitem($e) -fill $tkg_moduleColor
    } else {
      $canv_w itemconfigure $lbitem($e) -fill white
    }
  }

  #############################################################################
  #
  # Set the bit a pixel location (x, y)
  #
  proc setbit {x y} {
    variable bits
    variable changed
    variable width
    variable height

    if { $x >= 0 && $x < $width && $y >= 0 && $y < $height } {
      set bits(${x},$y) 1
      update_bit $x $y
      set changed 1
    }
  }

  #############################################################################
  #
  # Clear the bit a pixel location (x, y)
  #
  proc clrbit {x y} {
    variable bits
    variable changed
    variable width
    variable height

    if { $x >= 0 && $x < $width && $y >= 0 && $y < $height } {
      set bits(${x},$y) 0
      update_bit $x $y
      set changed 1
    }
  }

  #############################################################################
  #
  # Copy the bit at (x,y) to the cut buffer and clear it.
  #
  proc cutbit {x y} {
    variable bits
    variable cut
    variable changed
    variable width
    variable height
    variable x1_sel
    variable y1_sel

    if { $x >= 0 && $x < $width && $y >= 0 && $y < $height } {
      set cut([expr $x - $x1_sel],[expr $y - $y1_sel]) $bits(${x},$y)
      set bits(${x},$y) 0
      update_bit $x $y
      set changed 1
    }
  }

  #############################################################################
  #
  # Copy the bit at (x,y) to the cut buffer 
  #
  proc copybit {x y} {
    variable bits
    variable cut
    variable changed
    variable width
    variable height
    variable x1_sel
    variable y1_sel

    if { $x >= 0 && $x < $width && $y >= 0 && $y < $height } {
      set cut([expr $x - $x1_sel],[expr $y - $y1_sel]) $bits(${x},$y)
    }
  }

  #############################################################################
  #
  # Paste the bit at (x,y) from the cut buffer to the edit buffer.
  #
  proc pastebit {x y} {
    variable bits
    variable cut
    variable changed
    variable width
    variable height
    variable x1_sel
    variable y1_sel

    if { $x >= 0 && $x < $width && $y >= 0 && $y < $height } {
      set bits(${x},$y) $cut([expr $x - $x1_sel],[expr $y - $y1_sel])
      update_bit $x $y
      set changed 1
    }
  }

  #############################################################################
  #
  # Do an overlay paste of the bit at (x,y) from the cut buffer to the edit buffer.
  #
  proc overlaybit {x y} {
    variable bits
    variable cut
    variable changed
    variable width
    variable height
    variable x1_sel
    variable y1_sel

    if { $x >= 0 && $x < $width && $y >= 0 && $y < $height } {
      if { $cut([expr $x - $x1_sel],[expr $y - $y1_sel]) } {
	set bits(${x},$y) 1
	update_bit $x $y
	set changed 1
      }
    }
  }

  #############################################################################
  #
  # Map window coordinates to bit cell coordinates
  #
  proc windowToCell {w x y} {
    variable width 
    variable height
    variable scale

    set xstart [lindex [$w xview] 0]
    set ystart [lindex [$w yview] 0]
    set xoffset [expr ($width*$scale+2)*$xstart - 2]
    set yoffset [expr ($height*$scale+2)*$ystart - 2]

    upvar $x X
    upvar $y Y

    set wX $X
    set wY $Y

    set X [expr int(($wX + $xoffset) / $scale) - 1]
    set Y [expr int(($wY + $yoffset) / $scale) - 1]

    if { $X < 0 || $Y < 0 } { return 0 }
    if { $X >= $width || $Y >= $height } { return 0 }
    return 1
  }

  #############################################################################
  #
  # Map cell coordinates to window coordinates
  #
  proc cellToWindow {x y} {
    variable width 
    variable height
    variable scale

    upvar $x X
    upvar $y Y

    set X [expr ($X + 1)*$scale]
    set Y [expr ($Y + 1)*$scale]

    return 1
  }

  #############################################################################
  #
  # Zoom in or out
  #
  proc zoom {d} {
    variable scale

    set zoom_list { 5 7 10 13 15 20 25 }
    set N [llength $zoom_list]

    set i [lsearch $zoom_list $scale]
    if { $i < 0 } {
      set scale 15
    } else {
      incr i $d
      if { $i < 0 } { set i 0 }
      if { $i >= $N } { set i [expr $N-1] }

      set scale [lindex $zoom_list $i]
    }
    reqPaint
  }

  #############################################################################
  #
  # Request a repaint of the bitmap data at the next convenient time.
  #
  proc reqPaint {} {
    variable paint_pending

    if { !$paint_pending } {
      set paint_pending 1
      after idle SymbolEdit::paint
    }
  }

  #############################################################################
  #
  # Display the bitmap array on the screen.
  #
  proc paint {} {
    variable width 
    variable height
    variable scale
    variable x_base 
    variable y_base 
    variable bits
    variable paint_pending
    variable lbitem
    variable sbitem
    variable ports
    variable canv_w
    global tkg_moduleColor

    set paint_pending 0

#    puts SymbolEdit::paint

#    TimeCheck::begin paint

    set colorTable(0) white
    set colorTable(1) $tkg_moduleColor

    set act_width [expr $width*$scale]
    set act_height [expr $height*$scale]

    $canv_w delete all

    set X $scale
    set X1 [expr 2*$scale]
    set lx $x_base
    for { set x 0 } { $x < $width } { incr x } { 
      set Y $scale
      set Y1 [expr $Y + $scale]
      set ly $x_base
      for { set y 0 } { $y < $height } { incr y } { 
	set color $colorTable($bits($x,$y))

	set lbitem($x,$y) [$canv_w create rectangle $X $Y $X1 $Y1 -fill $color -outline gray -width 1]

	incr Y $scale
	incr Y1 $scale
	incr ly
      }
      incr X $scale
      incr X1 $scale
      incr lx
    }
    $canv_w configure -scrollregion [list 0 0 [expr $act_width + 2*$scale] [expr $act_height + 2*$scale]]

    foreach name [array names ports] {
      makePort $name
    }

#    TimeCheck::end paint
  }

  #############################################################################
  #
  # Import image from file
  #
  proc importImage {} {
    global gf_imageFiletypes tkg_currentModule
    variable visibleIcon
    variable edit_w

    if { $visibleIcon == "select" } {
      set initialName ${tkg_currentModule}-bold.xbm
    } else {
      set initialName $tkg_currentModule.xbm
    }

    set load [tk_getOpenFile -defaultextension ".xbm" -filetypes $gf_imageFiletypes  -initialfile $initialName]
    if { $load != "" } {
      set f [open $load r]
      if { $f == "" } {
	tk_messageBox -type error -icon warning -message [format [m err.badopen] $load]
	return
      }
      set data [read $f]

      decode $edit_w $data 0 0 -useoffset 1




      reqPaint
      saveData -action ImportImg

      close $f
    }
  }

  #############################################################################
  #
  # Export image to file
  #
  proc exportImage {} {
    global gf_imageFiletypes tkg_currentModule
    variable visibleIcon

    if { $visibleIcon == "select" } {
      set initialName ${tkg_currentModule}-bold.xbm
    } else {
      set initialName $tkg_currentModule.xbm
    }

    set save [tk_getSaveFile -defaultextension ".xbm" \
		  -filetypes $gf_imageFiletypes -initialfile $initialName]
    if { $save != "" } {
      set f [open $save w]
      if { $f == "" } {
	tk_messageBox -type error -icon warning -message [format [m err.badopen] $save]
	return
      }
      puts $f [encode xoff yoff -includehot 1]
      close $f
    }
  }

  #############################################################################
  #
  # Respond to the resize button/command
  #
  proc resizeImage {} {
  }


  #############################################################################
  #
  # Cut the selected region
  #
  proc cutRegion {} {
    variable x1_sel
    variable y1_sel
    variable x2_sel
    variable y2_sel
    variable cut_width
    variable cut_height

    if { $x1_sel != "" } {
      set cut_width  [expr $x2_sel - $x1_sel + 1]
      set cut_height [expr $y2_sel - $y1_sel + 1]

      doFillRect $x1_sel $y1_sel $x2_sel $y2_sel cutbit
      clearRubber
    }
  }

  #############################################################################
  #
  # copy the selected region
  #
  proc copyRegion {} {
    variable x1_sel
    variable y1_sel
    variable x2_sel
    variable y2_sel
    variable cut_width
    variable cut_height

    if { $x1_sel != "" } {
      set cut_width  [expr $x2_sel - $x1_sel + 1]
      set cut_height [expr $y2_sel - $y1_sel + 1]

      doFillRect $x1_sel $y1_sel $x2_sel $y2_sel copybit
      clearRubber
    }
  }

  #############################################################################
  #
  # Paste to the selected spot
  #
  proc pasteRegion {} {
    variable x1_sel
    variable y1_sel
    variable x2_sel
    variable y2_sel
    variable cut_width
    variable cut_height

    if { $x1_sel != ""  && $cut_width > 0 && $cut_height > 0 } {
      doFillRect $x1_sel $y1_sel [expr $x1_sel + $cut_width -1] [expr $y1_sel + $cut_height - 1] pastebit
      clearRubber
    }
  }

  #############################################################################
  #
  # Overlay paste to the selected spot
  #
  proc overlayRegion {} {
    variable x1_sel
    variable y1_sel
    variable x2_sel
    variable y2_sel
    variable cut_width
    variable cut_height

    if { $x1_sel != ""  && $cut_width > 0 && $cut_height > 0 } {
      doFillRect $x1_sel $y1_sel [expr $x1_sel + $cut_width -1] [expr $y1_sel + $cut_height - 1] overlaybit
      clearRubber
    }
  }

  #############################################################################
  #
  # Light up a "rubber" pixel
  #
  proc makeRubber {x y} {
    variable scale
    variable width
    variable height
    variable canv_w

    if { $x >= 0 && $x < $width && $y >= 0 && $y < $height } {
      set wx1 [expr ($x+1)*$scale]
      set wy1 [expr ($y+1)*$scale]
      set wx2 [expr ($x+2)*$scale]
      set wy2 [expr ($y+2)*$scale]
      $canv_w create rectangle $wx1 $wy1 $wx2 $wy2 -fill "\#8080ff" -outline gray -tags rubber
    }
  }

  #############################################################################
  #
  # Draw a line calling the procedure cmd for each pixel
  #
  proc doLine {x1 y1 x2 y2 cmd} {
    if { $x1 == $x2 && $y1 == $y2 } {
      $cmd $x1 $y1
    } elseif { [expr abs($x1-$x2) > abs($y1-$y2)] } {
      #
      # X-axis dominates
      #
      if { $x2 < $x1 } {
	set tx $x1
	set ty $y1
	set x1 $x2
	set y1 $y2
	set x2 $tx
	set y2 $ty
      }

      if { $y2 >= $y1 } {
	set Q [expr ($y2 - $y1 + 0.5)/($x2 - $x1)]
      } else {
	set Q [expr ($y2 - $y1 - 0.5)/($x2 - $x1)]
      }
      for { set x $x1 } { $x <= $x2 } { incr x } {
	set y [expr int(($x-$x1)*$Q) + $y1]

	$cmd $x $y
      }
    } else {
      #
      # Y-axis dominates
      #
      if { $y2 < $y1 } {
	set tx $x1
	set ty $y1
	set x1 $x2
	set y1 $y2
	set x2 $tx
	set y2 $ty
      }

      if { $x2 >= $x1 } {
	set Q [expr ($x2 - $x1 + 0.5)/($y2 - $y1)]
      } else {
	set Q [expr ($x2 - $x1 - 0.5)/($y2 - $y1)]
      }
      for { set y $y1 } { $y <= $y2 } { incr y } {
	set x [expr int(($y-$y1)*$Q) + $x1]

	$cmd $x $y
      }
    }
  }

  #############################################################################
  #
  # Draw a rectangle calling the procedure cmd for each pixel
  #
  proc doRect {x1 y1 x2 y2 cmd} {
    if { $x2 < $x1 } {
      set t $x1
      set x1 $x2
      set x2 $t
    }
    if { $y2 < $y1 } {
      set t $y1
      set y1 $y2
      set y2 $t
    }

    for { set x $x1 } { $x <= $x2 } { incr x } {
      $cmd $x $y1
      if { $y1 != $y2 } {
	$cmd $x $y2
      }
    }

    incr y1
    incr y2 -1
    for { set y $y1 } { $y <= $y2 } { incr y } {
      $cmd $x1 $y
      if { $x1 != $x2 } {
	$cmd $x2 $y
      }
    }
  }


  #############################################################################
  #
  # Draw a filled rectangle calling the procedure cmd for each pixel
  #
  proc doFillRect {x1 y1 x2 y2 cmd} {
    if { $x2 < $x1 } {
      set t $x1
      set x1 $x2
      set x2 $t
    }
    if { $y2 < $y1 } {
      set t $y1
      set y1 $y2
      set y2 $t
    }

    for { set x $x1 } { $x <= $x2 } { incr x } {
      for { set y $y1 } { $y <= $y2 } { incr y } {
	$cmd $x $y
      }
    }
  }


  #############################################################################
  #
  # Erase an "rubber" items.
  #
  proc clearRubber {} {
    variable x1_sel
    variable y1_sel
    variable x2_sel
    variable y2_sel
    variable canv_w

    #
    # Undo any selection
    #
    set x1_sel ""
    set y1_sel ""
    set x2_sel ""
    set y2_sel ""

    $canv_w delete rubber
  }

  #############################################################################
  #
  # Clear the port selection
  #
  proc clearPortSelect {} {
    variable port_select
    set port_select ""
  }

  #############################################################################
  #
  # Set the port selection
  #
  proc setPortSelect {name} {
    variable port_select

    set old_select $port_select
    set port_select $name
    if { $old_select != "" } { makePort $old_select }
    if { $name != ""} { makePort $name }
  }
  
  #############################################################################
  #
  # Select a port.  (x,y) are raw screen coordinates
  #
  proc doPortHit {w x y} {
    variable width
    variable height
    variable scale
    set name ""

    set xstart [lindex [$w xview] 0]
    set ystart [lindex [$w yview] 0]
    set xoffset [expr int(($width+2)*$scale*$xstart)]
    set yoffset [expr int(($height+2)*$scale*$ystart)]

    incr x $xoffset
    incr y $yoffset

    set i [$w find closest $x $y 4]
    if { $i != "" } {
      set name ""
      foreach tag [$w gettags $i] {
	scan $tag "port:%s" name
      }
    }
#    puts "hit i=$i name=$name"

    setPortSelect $name
  }

  #############################################################################
  #
  # Move a port.  (x,y) are cell coordinates
  #
  proc doPortMove {w x y} {
    variable port_select
    variable ports
    variable changed

    if { $port_select != "" } {
      makePort $port_select -x $x -y $y
    }
    set changed 1
  }

  #############################################################################
  #
  # Respond to press of mouse button 1
  #
  proc seeB1Press {w x y} {
    variable emode
    variable x_set
    variable y_set

    #
    # Save actual x/y position
    #
    set X $x
    set Y $y
    windowToCell $w x y

    clearRubber

    switch $emode {
      point {
	setbit $x $y
      }
      line {
	set x_set $x
	set y_set $y
	doLine $x $y $x $y makeRubber
      }
      rect {
	set x_set $x
	set y_set $y
	doRect $x $y $x $y makeRubber
      }
      fillrect {
	set x_set $x
	set y_set $y
	doRect $x $y $x $y makeRubber
      }
      select {
	set x_set $x
	set y_set $y
	doRect $x $y $x $y makeRubber
      }
      port {
	doPortHit $w $X $Y
      }
    }
  }

  #############################################################################
  #
  # Respond to press of mouse button 3
  #
  proc seeB3Press {w x y} {
    variable emode
    variable x_set
    variable y_set

    windowToCell $w x y

    clearRubber

    switch $emode {
      point {
	clrbit $x $y
      }
      line {
	set x_set $x
	set y_set $y
	doLine $x $y $x $y makeRubber
      }
      rect {
	set x_set $x
	set y_set $y
	doRect $x $y $x $y makeRubber
      }
      fillrect {
	set x_set $x
	set y_set $y
	doRect $x $y $x $y makeRubber
      }
    }
  }

  #############################################################################
  #
  # Respond to motion of mouse button 1
  #
  proc seeB1Motion {w x y} {
    variable emode
    variable x_set
    variable y_set

    windowToCell $w x y

    clearRubber

    switch $emode {
      point {
	setbit $x $y
      }
      line {
	doLine $x_set $y_set $x $y makeRubber
      }
      rect {
	doRect $x_set $y_set $x $y makeRubber
      }
      fillrect {
	doRect $x_set $y_set $x $y makeRubber
      }
      select {
	doRect $x_set $y_set $x $y makeRubber
      }
      port {
	doPortMove $w $x $y
      }
    }
  }

  #############################################################################
  #
  # Respond to motion of mouse button 3
  #
  proc seeB3Motion {w x y} {
    variable emode
    variable x_set
    variable y_set

    windowToCell $w x y

    clearRubber

    switch $emode {
      point {
	clrbit $x $y
      }
      line {
	doLine $x_set $y_set $x $y makeRubber
      }
      rect {
	doRect $x_set $y_set $x $y makeRubber
      }
      fillrect {
	doRect $x_set $y_set $x $y makeRubber
      }
    }
  }

  #############################################################################
  #
  # Respond to release of mouse button 1
  #
  proc seeB1Release {w x y} {
    variable emode
    variable x_set
    variable y_set
    variable x1_sel
    variable y1_sel
    variable x2_sel
    variable y2_sel

    windowToCell $w x y

    switch $emode {
      point {
	clearRubber
	setbit $x $y
	saveData -action DrawPoint
      }
      line {
	clearRubber
	doLine $x $y $x_set $y_set setbit
	saveData -action DrawLine
      }
      rect {
	clearRubber
	doRect $x $y $x_set $y_set setbit
	saveData -action DrawRect
      }
      fillrect {
	clearRubber
	doFillRect $x $y $x_set $y_set setbit
	saveData -action FillRect
      }
      select {
	set x1_sel [min $x $x_set]
	set y1_sel [min $y $y_set]
	set x2_sel [max $x $x_set]
	set y2_sel [max $y $y_set]
      }
      port {
	saveData -action PortMove
      }
    }
  }

  #############################################################################
  #
  # Respond to release of mouse button 3
  #
  proc seeB3Release {w x y} {
    variable emode
    variable x_set
    variable y_set

    windowToCell $w x y

    switch $emode {
      point {
	clearRubber
	clrbit $x $y
	saveData -action ClrPoint
      }
      line {
	clearRubber
	doLine $x $y $x_set $y_set clrbit
	saveData -action ClrLine
      }
      rect {
	clearRubber
	doRect $x $y $x_set $y_set clrbit
	saveData -action ClrRect
      }
      fillrect {
	clearRubber
	doFillRect $x $y $x_set $y_set clrbit
	saveData -action ClrArea
      }
    }
  }

  #############################################################################
  #
  # Decode an X11 bitmap into the SymbolEdit internal data structures
  #
  proc decode {w bmdata minx miny args} {
    variable width 
    variable height
    variable x_base 
    variable y_base 
    variable bits
    variable minWidth
    variable minHeight

    set useoffset 0
    parseargs $args {-useoffset}

    if { $bmdata == "" } {
      mkempty $w
      return
    } 

    catch { unset bits }

    set bmdata [string map {";" "" "," "" "[" "" "]" "" "=" "" "\#" "" "static unsigned char " ""} $bmdata]
    set iwidth [lindex $bmdata 2]
    set iheight [lindex $bmdata 5]

    if {[lindex $bmdata 6] == "define"} {
      set hotx [lindex $bmdata 8]
      set hoty [lindex $bmdata 11]
      if { $useoffset } {
	set minx [expr - $hotx]
	set miny [expr - $hoty]
      }

      set bmdata [lindex $bmdata 13]
    } else {
      set bmdata [lindex $bmdata 7]
    }

    set width [max $minWidth $iwidth]
    set height [max $minHeight $iheight]

    set maxx [expr $minx + $iwidth - 1]
    set maxy [expr $miny + $iheight - 1]

    set x_base 50
    set y_base 50


    for { set x 0 } { $x < $width } { incr x } {
      for { set y 0 } { $y < $height } { incr y } {
	set bits($x,$y) 0
      }
    }

    set x $minx
    set y $miny
    foreach v $bmdata {
      for { set i 0 } { $i < 8 } { incr i } {
	if {[expr $v & (1 << $i)] != 0} {
	  set bits($x,$y) 1
	} else {
	  set bits($x,$y) 0
	}
	incr x
	if { $x > $maxx } {
	  incr y
	  set x $minx
	  break
	}
      }
    }
  }

  proc increaseHeight {newHeight} {
    variable width
    variable height
    variable maxHeight
    variable bits

    if { $newHeight > $maxHeight || $newHeight <= $height } {
      return
    }

    for { set x 0 } { $x < $width } { incr x } {
      for { set y $height } { $y < $newHeight } { incr y } {
	set bits($x,$y) 0
      }
    }

    set height $newHeight

    reqPaint
  }

  proc increaseWidth {newWidth} {
    variable width
    variable height
    variable maxWidth
    variable bits

    if { $newWidth > $maxWidth || $newWidth <= $width } {
      return
    }

    for { set x $width } { $x < $newWidth } { incr x } {
      for { set y 0 } { $y < $height } { incr y } {
	set bits($x,$y) 0
      }
    }

    set width $newWidth

    reqPaint
  }

  #############################################################################
  #
  # Make an empty bit map.
  #
  proc mkempty {w} {
    variable width 
    variable height
    variable bits
    variable ports
    variable minWidth
    variable minHeight

    catch { unset ports }
    catch { unset bits }

    set width $minWidth
    set height $minHeight

    set x 0
    set y 0
    for { set x 0 } { $x < $width } { incr x } {
      for { set y 0 } { $y < $height } { incr y } {
	set bits($x,$y) 0
      }
    }
  }

  #############################################################################
  #
  # Encode the bitmap bmdata into a standard X11 bitmap form 
  #
  proc encode {xoff yoff args} {
    variable width 
    variable height
    variable bits

    upvar $xoff minx
    upvar $yoff miny

    set includehot 0
    parseargs $args {-includehot}

    set minx $width
    set miny $height
    set maxx -1
    set maxy -1

    for {set x 0} {$x < $width} {incr x } {
      for {set y 0} {$y < $height} {incr y } {
	if { $bits($x,$y) } {
	  if { $x < $minx } { set minx $x }
	  if { $y < $miny } { set miny $y }
	  if { $x > $maxx } { set maxx $x }
	  if { $y > $maxy } { set maxy $y }
	}
      }
    }

    if { $maxx < 0 } {
      set minx 0
      set miny 0
      set maxx 0
      set maxy 0
    }

    set iwidth  [expr $maxx - $minx  + 1]
    set iheight [expr $maxy - $miny  + 1]


    set l1 "\#define bitmap_width $iwidth\n"
    set l2 "\#define bitmap_height $iheight\n"
    set header "$l1$l2"
    if { $includehot } {
      set l2b "\#define bitmap_x_hot -$minx\n"
      set l2c "\#define bitmap_y_hot -$miny\n"
      set header "$header$l2b$l2c"
    }
    set l3 "static unsigned char bitmap_bits\[\] = \{\n"
    set header "$header$l3"

    set L {}

    for {set y $miny} {$y <= $maxy} {incr y } {
      set v 0
      set b 0
      for {set x $minx} {$x <= $maxx} {incr x } {
	set v [expr $v | ($bits(${x},$y) << $b)]
	incr b
	if {$b == 8} {
	  lappend L [format "0x%02x" $v]
	  set b 0
	  set v 0
	}
      }
      if {$b != 0} {
	lappend L [format "0x%02x" $v]
      }
    }

    set S ""
    while {[llength $L] > 0} {
      set R [string map {" " ", "} [lrange $L 0 11]]
      set L [lrange $L 12 end]

      if {[llength $L] > 0} {
	set S "$S   $R,\n"
      } else {
	set S "$S   $R\};\n"
      }
    }

    set rval "$header$S"

    return $rval
  }

  #############################################################################
  #
  # Load data from tkgate
  #
  proc loadData {} {
    variable data
    variable ports
    variable changed
    variable visibleIcon
    variable edit_w

    set w $edit_w

    if { $data(name) != "" } {
      gat_interface load  $data(name) -data SymbolEdit::data
      gat_interface geticon $data(name) -data SymbolEdit::data

      openEditIcon

      reqPaint
      set data(loadedName) $data(name)
    }

    catch { unset ports }
    set n $data(numPPlace)
    for { set i 0 } { $i < $n } { incr i } {
      set name [lindex $data(pplace:$i) 0]
      set ports($name) $data(pplace:$i)
    }

    set changed 0
  }

  #############################################################################
  #
  # Save all data about this symbol to the C-side
  #
  proc saveData {args} {
    variable data
    variable ports
    variable changed
    variable visibleIcon

    set name $data(name)
    set action SymEdit
    parseargs $args {-action -name}

    closeEditIcon

    set n 0
    foreach portName [array names ports] {
      set data(pplace:$n) $ports($portName)
      incr n
    }
    set data(numPPlace) $n

    action $action "gat_interface puticon $name -icon $visibleIcon -data SymbolEdit::data"
    set changed 0
  }

  #############################################################################
  #
  # Get the region on which to perform an operation.  If there is an active
  # selection, then get the selection.  If there is no selection, get the
  # whole image.
  #
  proc getOprRegion {x1 y1 x2 y2 w h args} {
    variable x1_sel
    variable y1_sel
    variable x2_sel
    variable y2_sel
    variable width
    variable height

    upvar $x1 sb_x1
    upvar $x2 sb_x2
    upvar $y1 sb_y1
    upvar $y2 sb_y2
    upvar $w sb_width
    upvar $h sb_height

    set square 0
    parseargs $args {-square}

    #
    # Set the region to do the shift operation on.  If there is a selection,
    # use the selection.  If not, use the whole bitmap buffer.
    #
    if { $x1_sel != "" } {
      set sb_x1 $x1_sel
      set sb_y1 $y1_sel
      set sb_x2 $x2_sel
      set sb_y2 $y2_sel
    } else {
      set sb_x1 0
      set sb_y1 0
      set sb_x2 [expr $width - 1]
      set sb_y2 [expr $height - 1]
    }

    set sb_width [expr $sb_x2 - $sb_x1 + 1]
    set sb_height [expr $sb_y2 - $sb_y1 + 1]

    if {$square} {
      #
      # Make sure we have a square area
      #
      if { $sb_height < $sb_width } {
	incr sb_x1 [expr ($sb_width-$sb_height)/2]
	set sb_width $sb_height
	set sb_x2 [expr $sb_x1 + $sb_width - 1]
      } elseif { $sb_height > $sb_width } { 
	incr sb_y1 [expr ($sb_height-$sb_width)/2]
	set sb_height $sb_width
	set sb_y2 [expr $sb_y1 + $sb_height - 1]
      }
    }
  }

  #############################################################################
  #
  # Make the symbol bold
  #
  # When boldifying, bits values are expanded into the follwing bit mask:
  #
  #   0x1		Originally set bit
  #   0x2		Bit is leftmost
  #   0x4		Bit is rightmost
  #   0x8		Bit is topmost
  #   0x10		Bit is bottommost
  #   0x20		Bit is a boldified bit
  #
  proc boldBits {} {
    variable bits

    getOprRegion sb_x1 sb_y1 sb_x2 sb_y2 sb_width sb_height -square 1


    for { set x 0 } { $x < $sb_width } { incr x } {
      for { set y 0 } { $y < $sb_height } { incr y } {
	set sx [expr $x + $sb_x1 ]
	set sy [expr $y + $sb_y1 ]
	set bb(${x},$y) $bits(${sx},$sy)
      }
    }

    #
    # Mark bits on the parimeter with a 2
    #
    for { set x 0 } { $x < $sb_width } { incr x } {
      for { set y 0 } { $y < $sb_height } { incr y } {
	if { $bb(${x},$y) } {
	  set bb(${x},$y) [expr $bb(${x},$y) | 8]
	  break
	}
      }
      for { set y [expr $sb_height-1] } { $y >= 0 } { incr y -1 } {
	if { $bb(${x},$y) } {
	  set bb(${x},$y) [expr $bb(${x},$y) | 16]
	  break
	}
      }
    }
    for { set y 0 } { $y < $sb_height } { incr y } {
      for { set x 0 } { $x < $sb_width } { incr x } {
	if { $bb(${x},$y) } {
	  set bb(${x},$y) [expr $bb(${x},$y) | 2]
	  break
	}
      }
      for { set x [expr $sb_width-1] } { $x >= 0 } { incr x -1 } {
	if { $bb(${x},$y) } {
	  set bb(${x},$y) [expr $bb(${x},$y) | 4]
	  break
	}
      }
    }

    #
    # Do boldification.
    #
    for { set y 0 } { $y < $sb_height } { incr y } {
      for { set x 0 } { $x < $sb_width } { incr x } {
	#
	# If a bit is set, figure out which adjacent bits to set
	#
	if { ($bb(${x},$y) & 1) } {
	  if { ($bb(${x},$y) & 4) } {
	    #boldify left
	    if { $x > 0 && $bb([expr $x - 1],$y) == 0 } {
	      set bb([expr $x - 1],$y) 32
	    }
	  } else {
	    #boldify right
	    if { $x < [expr $sb_width-1] && $bb([expr $x + 1],$y) == 0 } {
	      set bb([expr $x + 1],$y) 32
	    }
	  }

	  if { ($bb(${x},$y) & 16) } {
	    #boldify left
	    if { $y > 0 && $bb($x,[expr $y - 1]) == 0 } {
	      set bb($x,[expr $y - 1]) 32
	    }
	  } else {
	    #boldify right
	    if { $y < [expr $sb_height-1] && $bb($x,[expr $y + 1]) == 0 } {
	      set bb($x,[expr $y + 1]) 32
	    }
	  }
	}
      }
    }
    #
    # Copy boldified region back to bits()
    #
    for { set x 0 } { $x < $sb_width } { incr x } {
      for { set y 0 } { $y < $sb_height } { incr y } {
	set sx [expr $x + $sb_x1 ]
	set sy [expr $y + $sb_y1 ]
	set bits(${sx},$sy) [expr $bb(${x},$y) != 0 ]
      }
    }

    reqPaint
    saveData -action Boldify
    set changed 1
  }

  #############################################################################
  #
  # Rotate the image in a region
  #
  proc rotateBits {r} {
    variable bits

    getOprRegion sb_x1 sb_y1 sb_x2 sb_y2 sb_width sb_height -square 1

    for { set x 0 } { $x < $sb_width } { incr x } {
      for { set y 0 } { $y < $sb_height } { incr y } {
	set sx [expr $x + $sb_x1 ]
	set sy [expr $y + $sb_y1 ]
	set rotbuf(${x},$y) $bits(${sx},$sy)
      }
    }
    for { set x 0 } { $x < $sb_width } { incr x } {
      for { set y 0 } { $y < $sb_height } { incr y } {

	if { $r < 0 } {
	  set rx $y
	  set ry [expr $sb_height-$x-1]
	} else {
	  set rx [expr $sb_width-$y-1]
	  set ry $x
	}

	set sx [expr $rx + $sb_x1 ]
	set sy [expr $ry + $sb_y1 ]
	set  bits(${sx},$sy) $rotbuf(${x},$y)
      }
    }

    reqPaint
    saveData -action RotBits
    set changed 1
  }

  #############################################################################
  #
  # Shift bits by the specified amount
  #
  proc shiftBits {dx dy} {
    variable bits

    getOprRegion sb_x1 sb_y1 sb_x2 sb_y2 sb_width sb_height

    for { set x 0 } { $x < $sb_width } { incr x } {
      set sx [expr (($x - $dx + $sb_width) % $sb_width) + $sb_x1 ]
      set sy [expr ((- $dy + $sb_height) % $sb_height) + $sb_y1 ]
      set sy_max [expr $sb_height + $sb_y1 ]
      for { set y 0 } { $y < $sb_height } { incr y } {
	if { $sy >= $sy_max } {
	  set sy [expr (($y- $dy + $sb_height) % $sb_height) + $sb_y1 ]
	}
	set shiftbuf(${x},$y) $bits(${sx},$sy)
	incr sy
      }
    }
    for { set x 0; set sx $sb_x1 } { $x < $sb_width } { incr x; incr sx } {
      for { set y 0; set sy $sb_y1 } { $y < $sb_height } { incr y; incr sy } {
	set  bits(${sx},$sy) $shiftbuf(${x},$y)
      }
    }

    reqPaint
    saveData -action ShiftBits
    set changed 1
  }

  #############################################################################
  #
  # rotate selected port
  #
  proc rotatePort {d} {
    variable port_select
    variable ports
    variable changed
  
    if { $port_select != "" } {
      set q [lindex $ports($port_select) 4]
      set q [expr ($q + $d + 4) % 4]
      makePort $port_select -orient $q
      set changed 1
    }
    saveData -action RotPort
  }

  #############################################################################
  #
  # remove the selected port
  #
  proc removePort {name}  {
    variable ports
    variable canv_w

    $canv_w delete port:$name
    if {[info exists ports($name)]} {
      unset ports($name)
    }
  }

  #############################################################################
  #
  # Make a port
  #
  proc makePort {name args}  {
    variable ports
    variable scale
    variable x_base
    variable y_base
    variable port_select
    variable width
    variable height
    variable canv_w

    if {[catch {set x [lindex $ports($name) 1]}]}      { set x 0}
    if {[catch {set y [lindex $ports($name) 2]}]}      { set y 0}
    if {[catch {set type [lindex $ports($name) 3]}]}   { set type in}
    if {[catch {set orient [lindex $ports($name) 4]}]} { set orient 0}
    if {[catch {set bits [lindex $ports($name) 5]}]}   { set bits 1}
    parseargs $args {-x -y -orient -type -bits}

    if { $x < 0 } { set x 0 }
    if { $y < 0 } { set y 0 }
    if { $x >= $width } { set x [expr $width-1] }
    if { $y >= $height } { set y [expr $height-1] }

    set ports($name) [list $name $x $y $type $orient $bits]

    $canv_w delete port:$name


    #
    # Coordinates for small window.  Do this before converting to large screen coordinates.
    #
    set lx [expr $x + $x_base]
    set ly [expr $y + $y_base]

    cellToWindow x y

    if { $name == $port_select } {
      set x1 $x
      set y1 $y
      set x2 [expr $x + $scale]
      set y2 [expr $y + $scale]
    } else {
      set x1 [expr $x + ($scale+0.0)/5.0]
      set y1 [expr $y + ($scale+0.0)/5.0]
      set x2 [expr $x + (4.0*$scale+0.0)/5.0]
      set y2 [expr $y + (4.0*$scale+0.0)/5.0]
    }

    set xc [expr $x + ($scale+0.0)/2.0]
    set yc [expr $y + ($scale+0.0)/2.0]

    if { $bits == 1 } {
      global tkg_wireColor
      set color $tkg_wireColor
    } else {
      global tkg_busColor
      set color $tkg_busColor
    }


    set lwidth 2
    if { $scale < 10 } { set lwidth 1 }
    set tags [list port:$name port]

    set xwend $xc
    set ywend $yc
    set lx2 $lx
    set ly2 $ly

    set lab_x $xc
    set lab_y [expr $y + $scale]
    set lab_anchor n

    switch $orient {
      0 {
	set xwend [expr $xwend + 2*$scale]
	set lx2 [expr $lx + 10]
      }
      1 {
	set ywend [expr $ywend -  2*$scale]
	set ly2 [expr $ly - 10]

	set lab_x $x
	set lab_y $yc
	set lab_anchor e
      }
      2 {
	set xwend [expr $xwend - 2*$scale]
	set lx2 [expr $lx - 10]
      }
      3 {
	set ywend [expr $ywend + 2*$scale]
	set ly2 [expr $ly + 10]
	set lab_x $x
	set lab_y $yc
	set lab_anchor e
      }
    }

    #
    # Draw basic parts of symbol
    #
    $canv_w create line $xc $yc $xwend $ywend -fill $color -width $lwidth -tags $tags
    $canv_w create oval $x1 $y1 $x2 $y2 -fill $color -outline "" -tags $tags
    $canv_w create text $lab_x $lab_y -text $name -tags $tags -anchor $lab_anchor -fill $color


    #
    # Draw the bit size if more than 1 bit.
    #
    if { $bits > 1 } {
      set num_x [expr int(0.4*$xc + 0.6*$xwend)]
      set num_y [expr int(0.4*$yc + 0.6*$ywend)]
      $canv_w create line [expr $num_x + 5] [expr $num_y - 5] [expr $num_x - 5] [expr $num_y + 5] \
	  -fill $color -width 1 -tags $tags

      switch $orient {
	0 { $canv_w create text $num_x [expr $num_y - 5] -text $bits -fill $color -anchor s -tags $tags }
	1 { $canv_w create text [expr $num_x + 5] $num_y -text $bits -fill $color -anchor w -tags $tags }
	2 { $canv_w create text $num_x [expr $num_y - 5] -text $bits -fill $color -anchor s -tags $tags }
	3 { $canv_w create text [expr $num_x - 5] $num_y -text $bits -fill $color -anchor e -tags $tags }
      }
    }
  }

  proc yview {args} {
    variable canv_w
    variable maxWidth
    variable maxHeight
    variable width
    variable height

    # moveto n
    # scroll n pages
    # scroll n units

    set start [lindex [$canv_w yview] 0]
    set end   [lindex [$canv_w yview] 1]
    set b     [lindex $args 1]

    switch [lindex $args 0] {
      moveto {
	set ymax [expr $b+$end-$start]
	if {$ymax > 1} {
	  set oldHeight $height
	  increaseHeight [expr int($ymax*$height)]
	  set p [expr ($height-$oldHeight*($end-$start))/(0.0+$height)]
	  set args [list moveto $p]
	}
      }
      scroll {
	if { $end == 1 && $b > 0} {
	  set oldHeight $height
	  if {[lindex $args 2] == "pages"} {
	    set b [expr $b*10]
	  }
	  increaseHeight [expr ($height+$b)]
	  set p [expr ($height-$oldHeight*($end-$start))/(0.0+$height)]
	  set args [list moveto $p]
	}
      }
    }

    eval "$canv_w yview $args"
  }

  proc xview {args} {
    variable canv_w
    variable maxWidth
    variable maxHeight
    variable width
    variable height

    # moveto n
    # scroll n pages
    # scroll n units

    set start [lindex [$canv_w xview] 0]
    set end   [lindex [$canv_w xview] 1]
    set b     [lindex $args 1]
    set oldWidth $width

    switch [lindex $args 0] {
      moveto {
	set xmax [expr $b+$end-$start]
	if {$xmax > 1} {
	  increaseWidth [expr int($xmax*$width)]
	  set p [expr ($width-$oldWidth*($end-$start))/(0.0+$width)]
	  set args [list moveto $p]
	}
      }
      scroll {
	if { $end == 1 && $b > 0} {
	  if {[lindex $args 2] == "pages"} {
	    set b [expr $b*10]
	  }
	  increaseWidth [expr ($width+$b)]
	  set p [expr ($width-$oldWidth*($end-$start))/(0.0+$width)]
	  set args [list moveto $p]
	}
      }
    }

    eval "$canv_w xview $args"
  }

  #############################################################################
  #
  # Create window for editing symbol
  #
  proc createEditWin {w} {
    variable canv_w

    set canv_w $w.c
    frame $w 
    canvas $w.c -width 50 -height 50 -bd 2 -relief sunken \
	-bg white -yscrollcommand "$w.vb set" -xscrollcommand "$w.hb set" -takefocus 0
    scrollbar $w.vb -orient vertical -command "SymbolEdit::yview" -takefocus 0
    scrollbar $w.hb -orient horizontal -command "SymbolEdit::xview" -takefocus 0
    grid $w.c -row 0 -column 0 -sticky nsew
    grid $w.vb -row 0 -column 1 -sticky ns
    grid $w.hb -row 1 -column 0 -sticky ew
    grid rowconfigure $w 0 -weight 1
    grid columnconfigure $w 0 -weight 1
    bind $w.c <1> "SymbolEdit::seeB1Press %W %x %y"
    bind $w.c <3> "SymbolEdit::seeB3Press %W %x %y"
    bind $w.c <B1-Motion> "SymbolEdit::seeB1Motion %W %x %y"
    bind $w.c <B3-Motion> "SymbolEdit::seeB3Motion %W %x %y"
    bind $w.c <ButtonRelease-1> "SymbolEdit::seeB1Release %W %x %y"
    bind $w.c <ButtonRelease-3> "SymbolEdit::seeB3Release %W %x %y"
  }
  
  #############################################################################
  #
  # Create the link status
  #
  proc linkstatus {w args} {
    variable linkSelected
    checkbutton $w -text [m symed.link] -variable SymbolEdit::linkSelected
  }

  #############################################################################
  #
  # Change the image (normal vs select) that is being edited.
  #
  proc dotabs {w tab} {
    variable data
    variable edit_w
    variable visibleIcon

    TabBox::setactive $edit_w.tab $tab

    gat_interface syncicon $data(name) -icon $tab

    closeEditIcon
    set visibleIcon $tab
    openEditIcon
    reqPaint
  }

  #############################################################################
  #
  # Post the module symbol editor.
  #
  proc create {w args} {
    variable width 
    variable height
    variable data
    variable ports
    variable edit_w
    variable canv_w
    variable changed
    variable visibleIcon
    variable paint_pending
    variable port_select
    global bd

    set bmdata ""
    set port_select ""

    set paint_pending 0

    set width 20
    set height 20
    set parent ""
    set module ""

    set data(ok) 0
    set changed 0

    parseargs $args {-bmdata -parent -module}

    set visibleIcon normal

    set edit_w $w
    frame $w

    set data(loadedName) ""
    set data(name) $module
    set data(normalIcon) ""
    set data(selectIcon) ""
    set data(normalOffset) "0 0"
    set data(selectOffset) "0 0"

    #
    # Make symbol bitmap data empty 
    #
    mkempty $w



    #
    # Create main edit window
    #
    set labels [list [m symed.tab.normal] [m symed.tab.select]]
    set images [list  [gifI symed_normal.gif] [gifI symed_bold.gif]]
    TabBox::new $w.tab -passive 1 -tabs {normal select} -command SymbolEdit::dotabs -tablabels $labels -expand 1 -tabimages $images
    pack $w.tab -fill both -expand 1 -side right

    frame $w.tab.f -padx 2 -pady 2

    createEditWin $w.tab.f.symedit
    pack $w.tab.f.symedit -fill both -expand 1

    TabBox::addpassive $w.tab $w.tab.f

    helpon $w.tab.bnormal [m ho.symed.tab.normal]
    helpon $w.tab.bselect [m ho.symed.tab.select]

    reqPaint
  }
}
