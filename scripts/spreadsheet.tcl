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
# Last edit by hansen on Sun Jan 18 15:10:19 2009
#

#
# If we run this file standalone, enter test mode.
#
if {![info exists tkg_progName]} {
  set spreadsheet_test 1
  source "misc.tcl"
  source "dropbox.tcl"
} else {
  set spreadsheet_test 0
}

#############################################################################
#
# Public methods for SpreadSheet
#     size w			Get number of rows
#     getselection w		Get list of selected rows
#     setselection w idx	Set selected item
#     see w idx			Make sure the item idx is selected
#     clearselection w		Clear the selection
#     get w p			Get row at the specified position 
#     put w p item		Put a row at a specified position
#     getcell w p c		Get cell at the specified position 
#     putcell w p c item	Put cell at the specified position
#     insert w p item		Insert a row at a specified position
#     delete w items		Delete an item
#     create w options		Create a new spreadsheet
#     addcolumn w options	Add a column
#     flush w                   Deletes all items
#     entryValue		Value of cell that is being entered
#	
namespace eval SpreadSheet {
  variable parms
  variable focusSave
  variable cell
  variable lastclick 0
  variable dragspot ""
  variable asActive 0
  variable asEvent ""
  variable asSpeed 250
  variable entryValue

  proc shift {v d} {
    set L {}

    foreach e $v {
      lappend L [expr $e + $d]
    }

    return $L
  }


  #
  # Map a physical row to the effective data row
  #
  proc getEffectiveRow {w r} {
    variable parms
    set P $parms($w:position)
    return [expr $r + $P - 1]
  }

  #
  # Map an effective data row to a physical row
  #
  proc getPhysicalRow {w r} {
    variable parms
    set P $parms($w:position)
    return [expr $r - $P + 1]
  }

  #
  # Show the current selection assuming all cells are painted the unselected color
  #
  proc show_selection {w} {
    variable parms
    variable cell

    set P $parms($w:position)
    set Rmax $parms($w:height)
    set Cmax [llength $parms($w:colwidth)]

    if { $parms($w:grab) } {
       set color $parms($w:grabcolor)
    } else {
      set color $parms($w:selectcolor)
    }



    foreach er $parms($w:selection) {
      set r [expr $er - $P + 1]
      if { $r > 0 && $r < $Rmax } {
	for { set c 0 } { $c < $Cmax } { incr c } {
	  $w.c${r}_$c configure -bg $color
	}
      }
    }
  }

  #
  # Add a new row to the selection.
  #
  proc addto_selection {w er} {
    variable parms

    #
    # If row is already selected, then do nothing
    #
    if { [lsearch $parms($w:selection) $er] >= 0 } {
    }

    set P $parms($w:position)
    set Rmax $parms($w:height)
    set Cmax [llength $parms($w:colwidth)]

    #
    # If requested row is not in current data range, then do nothing
    #
    if { $er < 0 || $er >= $parms($w:numrows) } {
      return
    }

    if { $parms($w:selectmode) == "single" } {
      clearselection $w
      set parms($w:selection) $er
    } else {
      lappend parms($w:selection) $er
    }


    set r [expr $er - $P + 1]
    if { $r > 0 && $r < $Rmax } {
      for { set c 0 } { $c < $Cmax } { incr c } {
	$w.c${r}_$c configure -bg $parms($w:selectcolor)
      }
    }
  }

  proc advanceEntry {w K r c} {
    variable parms

    clearEntrySelect $w

    set Rmax $parms($w:height)
    set er [getEffectiveRow $w $r]

    switch $K {
      Tab {
	while {1} { 
	  incr c
	  if { $c >= [llength $parms($w:colwidth)] } {
	    set c 0
	    if {[expr $r + 1] >= $Rmax } {
	      SpreadSheet::yview $w scroll 1 unit
	      update
	    } else {
	      incr r
	    }
	  }
	  if {![entrySelect $w $r $c] } { break }
	}
      }
      Up {
	if { $er == 0 } {
	  entrySelect $w $r $c
	} else {
	  if { $r > 1 } {
	    incr r -1
	    entrySelect $w $r $c
	  } else {
	    clearEntrySelect $w
	    incr parms($w:position) -1
	    repaint $w
	    entrySelect $w $r $c
	  }
	}
      }
      Down {
	if { $er >= $parms($w:numrows) } {
	  entrySelect $w $r $c
	} else {
	  if { [expr $r + 1] < $parms($w:height) } {
	    incr r 
	    entrySelect $w $r $c
	  } else {
	    clearEntrySelect $w
	    incr parms($w:position)
	    repaint $w
	    entrySelect $w $r $c
	  }
	}
      }
    }
  }

  #############################################################################
  #
  # Select a cell for entry.  Returns 1 if this is an entry that should be 
  # skipped by the tab key
  #
  proc entrySelect {w r c} {
    variable parms
    variable cell
    variable focusSave

    set focusSave($w) [focus]
#    puts "entrySelect focus=$focusSave($w) takefocus=[$focusSave($w) cget -takefocus]"
    

    clearselection $w

    if { $r >= $parms($w:height) } { return 0 }

    set er [getEffectiveRow $w $r]

    if { $parms($w:entrycommand) != ""} {
      if { $er == $parms($w:numrows) } {
#       focus is not working correctly
#	set er $parms($w:numrows)
#	set r [getPhysicalRow $w $er]

	if {[$parms($w:entrycommand) canappend $w $er $c ""]} {
	  incr parms($w:numrows)
	  set xx [$parms($w:entrycommand) initentry $w $er $c ""]
	  put $w $er $xx
	  repaint $w

	  if {![$parms($w:entrycommand) canenter $w $er $c $cell($w:$er:$c)]} {
	    set c 0
	    while {![$parms($w:entrycommand) canenter $w $er $c $cell($w:$er:$c)]} { incr c }
	  }
	}
      } elseif { $er < $parms($w:numrows) } {
	if {![$parms($w:entrycommand) canenter $w $er $c $cell($w:$er:$c)]} {
	  return 1
	}
      }
    }

    if { $er >= $parms($w:numrows) } { return 0 }

    set width [lindex $parms($w:colwidth) $c]
    addto_selection $w $er

    destroy $w.c${r}_$c

    set SpreadSheet::entryValue $SpreadSheet::cell($w:$er:$c)

    set widget_ok 0
    if { $parms($w:entrycommand) != ""} {
      if { [$parms($w:entrycommand) entrywidget $w $r $c $w.c${r}_$c $width SpreadSheet::entryValue] != 0 } {
	set widget_ok 1
      }
    }
    if { !$widget_ok } {
      entry $w.c${r}_$c  -bd 1 -relief sunken -width $width 
      update
      $w.c${r}_$c configure -bg $parms($w:selectcolor) -textvariable SpreadSheet::entryValue -highlightthickness 1
      update

      focus $w.c${r}_$c
      $w.c${r}_$c icursor end
      $w.c${r}_$c selection range 0 end
    }

    bind $w.c${r}_$c <Destroy> "SpreadSheet::clearEntrySelect $w"

    tabBindings $w.c${r}_$c $w $r $c

    grid $w.c${r}_$c -row $r -column $c -ipadx 0 -ipady 0 -sticky ew
    update
    set parms($w:entryselection)    [list $r $c]

    return 0
  }

  #############################################################################
  #
  # End entering data into a cell
  #
  proc clearEntrySelect {w} {
    variable parms
    variable cell
    variable focusSave

    if { $parms($w:entryselection) == "" } { return }

    set r [lindex $parms($w:entryselection) 0]
    set c [lindex $parms($w:entryselection) 1]
    set er [getEffectiveRow $w $r]

    set width [lindex $parms($w:colwidth) $c]

    if { $parms($w:entrycommand) != ""} {
      set cell($w:$er:$c) [$parms($w:entrycommand) close $w $r $c $SpreadSheet::entryValue]
    } else {
      set cell($w:$er:$c) $SpreadSheet::entryValue
    }
    $parms($w:entrycommand) closenotify $w

    #
    # Unbind the deletion call to this function.
    #
#    puts "clearEntrySelect lastfor=[focus -lastfor .]"
    bind $w.c${r}_$c <Destroy> ""
    destroy $w.c${r}_$c
#    puts "clearEntrySelect focus=[focus]"

    #
    # Try to restore the focus
    #
    catch {
      focus $focusSave($w)
#      puts "clearEntrySelect refocus=[focus]"
    }



    #
    # We may have called this function on a destroy event in which case the following
    # code will fail.
    #
    catch {
      label $w.c${r}_$c  -bd 1 -relief raised -width $width \
	  -bg $parms($w:entrycolor) -anchor w -text $SpreadSheet::cell($w:$er:$c)
      grid $w.c${r}_$c -row $r -column $c -ipadx 1 -ipady 1 -sticky ew
      setBindings $w $r $c
    }

    set parms($w:entryselection)    ""
  }


  proc size {w} {
    variable parms
    return $parms($w:numrows)
  }

  proc setselection {w sel} {
    variable parms

    set parms($w:selection) $sel
    reqRepaint $w setselction
  }

  #
  # Make sure that entry idx is in the visible range
  #
  proc see {w idx} {
    variable parms

    set Rmax [expr $parms($w:height) - 1]
    set P $parms($w:position)

    if {$idx < $P} {
      # Index is above top of visible range
      set parms($w:position) $idx
      reqRepaint $w see-1
    } elseif { [expr $idx - $P + 1] >= $Rmax } {
      # Index is below bottom of visible range

      set pos [expr $idx - $Rmax + 1]

      if {[expr [winfo y $w.c${Rmax}_0] + [winfo height $w.c${Rmax}_0]] > [winfo height $w] } {
	if {$pos < $Rmax} {
	  incr pos
	}
      }

      set parms($w:position) $pos
      reqRepaint $w see-2
    }

  }

  proc getselection {w} {
    variable parms
    return $parms($w:selection)
  }

  proc clearselection {w} {
    variable parms
    variable cell

    set P $parms($w:position)
    set Rmax $parms($w:height)
    set Cmax [llength $parms($w:colwidth)]

    clearEntrySelect $w

    foreach er $parms($w:selection) {
      set r [expr $er - $P + 1]
      if { $r > 0 && $r < $Rmax } {
	for { set c 0 } { $c < $Cmax } { incr c } {
	  $w.c${r}_$c configure -bg $parms($w:entrycolor)
	}
      }
    }
    set parms($w:selection) ""
  }


  proc reqRepaint {w args} {
    variable parms

#    puts "reqRepaint $w $args"

    if { !$parms($w:updatepending) } {
      set parms($w:updatepending) 1
      after idle "SpreadSheet::repaint $w"
    }
  }


  #############################################################################
  #
  # Repaint the display from the stored cell data.
  #
 proc repaint {w} {
    variable parms
    variable cell


    set parms($w:updatepending) 0

    set P $parms($w:position)
    set Rmax $parms($w:height)
    set N $parms($w:numrows)
    set Cmax [llength $parms($w:colwidth)]
    set types $parms($w:types)

#    puts "repaint $w  [winfo width $w]x[winfo height $w]  Rmax=$Rmax"

    for { set r 1 } { $r < $Rmax } { incr r } {
      for { set c 0 } { $c < $Cmax } { incr c } {
	set er [expr $r + $P - 1]

	if {$er < $N} {
	  set text $cell($w:$er:$c)
	} else {
	  set text ""
	}
	catch {
	  set type [lindex $types $c]

	  if {$type == "image" } {
	    $w.c${r}_$c configure -image $text -bg $parms($w:entrycolor) -anchor center
	  } else {
	    $w.c${r}_$c configure -text $text -bg $parms($w:entrycolor)
	  }

	}
      }
    }
    show_selection $w

    if { $parms($w:yscrollcommand) != "" } {
      set N $parms($w:numrows)

      set start [expr ($P+0.0)/($N+1.0)]
      set stop [expr ($P + $Rmax+0.0)/($N+1.0)]

      eval "$parms($w:yscrollcommand) $start $stop"
    }
  }

  proc yview {w args} {
    variable parms

    set cmd [lindex $args 0]
    set n [lindex $args 1]

    set P $parms($w:position)
    set Rmax $parms($w:height)
    set N $parms($w:numrows)

    clearEntrySelect $w

    if {$cmd == "moveto" } {
      set P [expr int($N*$n)]
    } elseif {$cmd == "scroll" } {
      set P [expr int($P + $n)]
    }

    if { $P < 0 } { set P 0 }
    if { $P >= $N } { set P $N }
    set parms($w:position) $P
    reqRepaint $w yview
  }

  #
  # Delete items
  #
  proc delete {w args} {
    variable parms
    variable cell

    set dlist {}
    foreach ilist $args {
      foreach item $ilist {
	lappend dlist $item
      }
    }
    set dlist [lsort -integer $dlist]
    set first [lindex $dlist 0]

    if { [llength $dlist] == 0 } { return }

    set Cmax [llength $parms($w:colwidth)]

    set dst_r [lindex $dlist 0]
    set src_r [expr $dst_r + 1]
    set N $parms($w:numrows)

    set Dcount 1

    while { $src_r < $N } {
      if { [lsearch $dlist $src_r] >= 0 } {
	incr src_r
	incr Dcount
	continue
      }

      for { set c 0 } { $c < $Cmax } { incr c } {
	set cell($w:$dst_r:$c) $cell($w:$src_r:$c)
      }
      incr dst_r
      incr src_r
    }

    incr parms($w:numrows) [expr -$Dcount]
    clearselection $w
    if { $first < [size $w] } {
      setselection $w $first
    }
    reqRepaint $w delete

    catch { $parms($w:entrycommand) deletenotify $w }
  }

  proc flush {w} {
    variable parms

    clearselection $w
    set parms($w:numrows) 0
    reqRepaint $w flush
  }


  #
  # Get item at a postion
  #
  proc get {w er} {
    variable parms
    variable cell

    set Cmax [llength $parms($w:colwidth)]

    set L {} 
    for { set c 0 } { $c < $Cmax } { incr c } {
      lappend L $cell($w:$er:$c)
    }
    return $L
  }

  #
  # Get a single cell
  #
  proc getcell {w er c} {
    variable cell

    return $cell($w:$er:$c)
  }


  #
  # Put item at a postion
  #
  proc put {w er item} {
    variable parms
    variable cell

    set Cmax [llength $parms($w:colwidth)]

    for { set c 0 } { $c < $Cmax } { incr c } {
      set cell($w:$er:$c) [lindex $item $c]
    }
    reqRepaint $w put
  }

  #
  # Put a single cell
  #
  proc putcell {w pos c item} {
    variable cell
    variable parms

    if {$pos=="end"} {
      set pos $parms($w:numrows)
    }

    set cell($w:$pos:$c) $item
    reqRepaint $w putcell
  }

  #
  # Add an item to the spreadsheet
  #
  proc insert {w pos value} {
    variable parms
    variable cell

    if {$pos=="end"} {
      set pos $parms($w:numrows)
    }

    set Cmax [llength $parms($w:colwidth)]

    for { set r $parms($w:numrows) } { $r > $pos } { incr r -1 } {
      for { set c 0 } { $c < $Cmax } { incr c } {
	set cell($w:$r:$c) $cell($w:[expr $r - 1]:$c)
      }
    }

    for { set c 0 } { $c < $Cmax } { incr c } {
      set cell($w:$pos:$c) [lindex $value $c]
    }
    incr parms($w:numrows)

    reqRepaint $w insert
  }

  proc grabMotion {w r c} {
    variable parms

    set er_delta [expr $r - [lindex $parms($w:mousedown) 0]]


    if { $er_delta == 0 } {
      return
    }


    set selection [getselection $w]
#    puts "grabMotion $w : $parms($w:mousedown) : $r $c sel=$selection"

    set N $parms($w:numrows)
    set min_er [vmin $selection]
    set max_er [vmax $selection]


    if { [expr $min_er + $er_delta] < 0 } { set er_delta [expr -$min_er]}
    if { [expr $max_er + $er_delta+1] >= $N } { set er_delta [expr $N-$max_er-1]}

    # puts "sel=[list $selection] min=$min_er max=$max_er  delta=$er_delta"

    set newselection [shift $selection $er_delta]


    set min_er [min [expr $min_er + $er_delta] $min_er]
    set max_er [max [expr $max_er + $er_delta] $max_er]

    set SI {}
    set NSI {}
    for {set xr $min_er } { $xr <= $max_er } { incr xr } {
      if {[lsearch $selection $xr] < 0} {
	lappend NSI [get $w $xr]
      } else {
	lappend SI [get $w $xr]
      }
    }

    for {set xr $min_er } { $xr <= $max_er } { incr xr } {
      if {[lsearch $newselection $xr] < 0} {
	put $w $xr [lindex $NSI 0]
	set NSI [lrange $NSI 1 end]
      } else {
	put $w $xr [lindex $SI 0]
	set SI [lrange $SI 1 end]
      }
    }

    set parms($w:selection) $newselection
    set parms($w:mousedown) [list $r $c]

    catch { $parms($w:entrycommand) reorder $w }

    reqRepaint $w grabMotion
  }

  proc speedMap {d} {
    if { $d > 40} {
      return 50
    } else {
      return  [expr int(250-(40-$d)*200.0/40.0-50)]
    }
  }

  proc doAutoScroll {w} {
    variable asEvent
    variable asActive
    variable asSpeed
    variable asX
    variable asY
    variable parms

    set H $parms($w:numrows)
    set P $parms($w:position)

    if { $asActive < 0} {
      if {$P < [expr $H-1]} {
	incr parms($w:position)
	reqRepaint $w autoScroll
	seeB1Motion $w $asX $asY -autoscroll
      }
    } else {
      if {$P > 0} {
	incr parms($w:position) -1
	reqRepaint $w autoScroll
	seeB1Motion $w $asX $asY -autoscroll
      }
    }

    set asEvent [after $asSpeed "SpreadSheet::doAutoScroll $w"]
  }

  proc checkAutoScroll {w X Y args} {
    variable asEvent
    variable asActive
    variable asX
    variable asY
    variable asSpeed

    set asX $X
    set asY $Y

    set cancel 0
    if {[lsearch $args -cancel] >= 0} { set cancel 1 }


    set asc 0
    set rootY [winfo rooty $w]
    set height [winfo height $w]
    if { $Y < [expr $rootY + 40]} {
      set asY [max $Y 1]
      set asc 1
      
      set asSpeed [speedMap [expr $rootY + 40 - $Y]]
    } elseif { $Y > [expr $rootY + $height - 20]} {
      set asY [min $Y [expr $rootY + $height - 1]]
      set asc -1

      set asSpeed [speedMap [expr $Y - ($rootY + $height - 20)]]
    }

    #
    # Check for event cancelation
    #
    if {$cancel || $asc == 0 || ($asActive != 0 && $asc != $asActive)} {
      if {$asEvent != ""} { 
	after cancel $asEvent  
      }
      set asEvent ""
      set asActive 0
      return
    }

    set asActive $asc
    if { $asEvent == "" } {
      set asEvent [after 500 "SpreadSheet::doAutoScroll $w"]
    }
  }

  #
  # See press of B1
  #
  proc seeB1Press {w r c s X Y} {
    variable parms
    variable lastclick
    variable dragspot

    set dragspot ""

    checkAutoScroll $w $X $Y

    if { $s != 0 } {
      clearselection $w
      set parms($w:mousedown) {}
    }

    if {![catch { set clicktime [clock clicks -milliseconds] }]} {
      if { $lastclick != 0 && [expr $clicktime - $lastclick] < 350} {
	seeB1Double $w $r $c
	set lastclick 0
	return
      } else {
	set lastclick $clicktime
      }
    }

    set er [getEffectiveRow $w $r]

    if { [lsearch [getselection $w] $er] >= 0 } {
      set parms($w:mousedown) [list $er $c]
      set parms($w:grab) 1
      clearEntrySelect $w
      show_selection $w
      return
    }

    clearselection $w

    set P $parms($w:position)
    set Rmax $parms($w:height)
    set Cmax [llength $parms($w:colwidth)]

    set direct_move 0
    if { $parms($w:selectmode) == "single" 
	 || ($parms($w:selectmode) == "shift-multiple" && $s == 0) } {
      set direct_move 1
    }

    if { $er >= 0 && $er < $parms($w:numrows) } {
      set parms($w:mousedown) [list $er $c]
      addto_selection $w $er

      if {!$parms($w:grab) && $direct_move} {
	set parms($w:mousedown) [list $er $c]
	set parms($w:grab) 1
	clearEntrySelect $w
	show_selection $w
      }

      return
    }

    set parms($w:mousedown) {}
  }

  proc seeB1Motion {w X Y args} {
    variable parms
    variable dragspot

    #
    # -autoscroll flag means we are being called from within autoscroll and should avoid
    # a recursive call.
    #
    if {[lsearch $args -autoscroll] < 0 } {
      checkAutoScroll $w $X $Y
    }

    set W [winfo containing $X $Y]
    if { [scan $W $w.c%d_%d r c] != 2} { 
      catch { $parms($w:entrycommand) dragout zoom }
      return
    }

    if {[llength $parms($w:mousedown)] == 0} {return }

    set er [getEffectiveRow $w $r]

    if { $parms($w:dragcommand) != "" } {
      if { $dragspot == "" } {
	set dragspot [list $X $Y]
      } else {
	set dsX [lindex $dragspot 0]
	set dsY [lindex $dragspot 1]

	set delta [expr ($X-$dsX)*($X-$dsX) + ($Y-$dsY)*($Y-$dsY)]

	if { $delta > 7 } {
	  set er [getselection $w]
	  if {[llength $er] == 1 } {
	    $parms($w:dragcommand) $w $er
	  }
	  set dragspot ""
	}
      }
      return
    }

    
    if { $parms($w:grab) } {
      if { $parms($w:dograb) } {
	grabMotion $w $er $c
      }
      return
    }


    set P $parms($w:position)
    set Rmax $parms($w:height)
    set Cmax [llength $parms($w:colwidth)]

    set br [lindex $parms($w:mousedown) 0]

    if { $er < $br } {
      set x $er
      set er $br
      set br $x
    }

    clearselection $w
    for { set xr $br } { $xr <= $er } { incr xr } {
      addto_selection $w $xr
    }
  }

  proc seeB1Release {w X Y} {
    variable parms
    variable dragspot

    set dragspot ""

#    focus $w

    if { $parms($w:grab) } {
      set parms($w:mousedown) ""
      set parms($w:grab) 0
      show_selection $w
      return
    }
    seeB1Motion $w $X $Y
    checkAutoScroll $w 0 0 -cancel
  }

  proc seeB1Double {w r c } {
    variable parms
    variable dragspot

    set dragspot ""

    checkAutoScroll $w 0 0 -cancel


    set er [getEffectiveRow $w $r]
    catch { $parms($w:entrycommand) doublePress $w $er $c }
    entrySelect $w $r $c
  }

  proc seeB3Press {w r c s X Y} {
    variable parms
    variable lastclick
    variable dragspot

    set dragspot ""

    #
    # The double calls to seeB1Press cause the selected item to be shown as grabbed.
    # we clear lastclick so that it will not be treated as a double click.
    #
    seeB1Press $w $r $c $s $X $Y
    set lastclick 0
    seeB1Press $w $r $c $s $X $Y
    checkAutoScroll $w 0 0 -cancel

    catch { $parms($w:entrycommand) rightclick $w }
    set parms($w:mousedown) ""
    set parms($w:grab) 0
  }

  #############################################################################
  #
  # Send a request to delete an entry to the entry manager.
  #
  proc requestDelete {w args} {
    variable parms

    catch { $parms($w:entrycommand) delete $w }
  }

  #############################################################################
  #
  # Set bindings on cells that are not active for entry
  #
  proc setBindings {w r c} {
    bind $w.c${r}_$c <1> "SpreadSheet::seeB1Press $w $r $c %s %X %Y"
    bind $w.c${r}_$c <3> "SpreadSheet::seeB3Press $w $r $c %s %X %Y"
    bind $w.c${r}_$c <B1-Motion> "SpreadSheet::seeB1Motion $w %X %Y"
    bind $w.c${r}_$c <ButtonRelease-1> "SpreadSheet::seeB1Release $w %X %Y"
#    bind $w.c${r}_$c <Double-ButtonPress-1> "SpreadSheet::seeB1Double $w $r $c"
  }

  #############################################################################
  #
  # Set bindings on cells that are active for entry
  #
  proc tabBindings {W w r c} {
    bindtags $W [ldelete [bindtags $W] all]
    bind $W <Tab> "SpreadSheet::advanceEntry $w %K $r $c"
    bind $W <Escape> "SpreadSheet::clearEntrySelect $w; SpreadSheet::reqRepaint $w tab-escape"
    bind $W <Return> "SpreadSheet::clearEntrySelect $w; SpreadSheet::reqRepaint $w tab-return"
#    bind $W <Up> "SpreadSheet::advanceEntry $w %K $r $c"
#    bind $W <Down> "SpreadSheet::advanceEntry $w %K $r $c"
#    bind $W <KeyPress> { puts "KeyPress %K" }
  }


  #
  # Add a spreadsheet column
  #
  proc addcolumn {w args} {
    variable parms
    variable cell

    set width 5
    set header ""
    set type text
    parseargs $args {-width -header -type}

    set c [llength $parms($w:colwidth)]

    lappend parms($w:colwidth) $width
    lappend parms($w:headers) $header
    lappend parms($w:types) $type


    if {$parms($w:expandcol) == $c} {
      grid columnconfigure $w $c -weight 1
    }

    # column header
    label $w.h${c} -bd 1 -relief raised -width $width -bg $parms($w:headercolor) -text $header -font dialogBigExpFont -takefocus 0
    grid $w.h${c} -row 0 -column $c -ipadx 1 -ipady 1 -sticky ew

    # column rows
    for {set r 1 } {$r < $parms($w:height) } {incr r } {
      label $w.c${r}_$c -bd 1 -relief raised -width $width -bg $parms($w:entrycolor) -anchor w -takefocus 0
      grid $w.c${r}_$c -row $r -column $c  -ipadx 1 -ipady 1 -sticky ew
      set cell($w:$r:$c) ""
      setBindings $w $r $c
    }
  }

  proc reduceSize {w new_height} {
    variable parms

#    puts "reduceSize $w $new_height"

    set Ncol [llength $parms($w:colwidth)]
    set height $parms($w:height)
    for  { set r $new_height } { $r < $height } { incr r } {
#      puts "    delrow - $r"
      for { set c 0} { $c < $Ncol } { incr c } { 
	destroy $w.c${r}_$c
      }
    }
    set parms($w:height) $new_height
  }

  proc expandSize {w H} {
    variable parms

    set Ncol [llength $parms($w:colwidth)]

    set H [winfo reqheight [winfo parent $w]]
#    puts "expandSize $w $H"

    set rowHeight [winfo height $w.h0]
    
    for {set r $parms($w:height) } {[expr $r*$rowHeight] < $H } {incr r } {
#      puts "    addrow - $r"
      for { set c 0} { $c < $Ncol } { incr c } { 
	set width [lindex $parms($w:colwidth) $c]
	label $w.c${r}_$c -bd 1 -relief raised -width $width -bg $parms($w:entrycolor) -anchor w
	grid $w.c${r}_$c -row $r -column $c  -ipadx 1 -ipady 1 -sticky ew
	set cell($w:$r:$c) ""
	setBindings $w $r $c
      }
    }
#    puts "set parms($w:height) [expr $r + 1]"
    set parms($w:height) $r
  }

  proc updateSize {w} {
    variable parms

    set W [winfo width $w]
    set H [winfo height $w]

#    puts "SpreadSheet::updateSize $w $W $H"

    if { $parms($w:resize) } {
      set new_height 0

      set height $parms($w:height)

      set new_height 10000000
      for { set r 1 } { $r < $height } { incr r } {
	if { [winfo y $w.c${r}_0] > $H } {
	  set new_height $r
	  break
	}
      }

      if {$new_height < $height } {
	reduceSize $w $new_height
      } else {
	expandSize $w $H
      }

    }


    SpreadSheet::reqRepaint $w updateSize
  }

  #############################################################################
  #
  # Configure a SpreadSheet widget (arguments given in list)
  #
  proc configurev {w argv} {
    variable parms

#    puts "SpreadSheet::configurev $w $argv"

    set optlist {-height -headercolor -entrycolor -selectcolor -command -statecommand 
      -bd -relief -yscrollcommand -grabcolor -entrycommand -expandcol -selectmode
      -dograb -dragcommand -resize}

    parseargs $argv $optlist

    foreach o $optlist {
      scan $o "-%s" var

      if {[info exists $var]} {
	set parms($w:$var) [set $var]
      }
    }
  }

  #############################################################################
  #
  # Configure a SpreadSheet widget (arguments on command line)
  #
  proc configure {w args} {
    configurev $w $args
  }


  #############################################################################
  #
  # SpreadSheet::init $w $argv
  #
  # Initialize the options of a spreadsheet
  #
  proc init {w} {
    variable parms
    variable cell


    set parms($w:mousedown) {}
    set parms($w:colwidth) {}
    set parms($w:position) 0
    set parms($w:numrows) 0
    set parms($w:grab) 0
    set parms($w:selection) ""
    set parms($w:entryselection) ""
    set parms($w:updatepending) 0
    set parms($w:headers) {}
    set parms($w:types) {}
    set parms($w:repaintpending) ""
    set parms($w:dograb) 1
    set parms($w:height) 1
    set parms($w:headercolor) [option get $w SpreadSheet.headerColor {}]
    set parms($w:entrycolor)  [option get $w SpreadSheet.entryColor {}]
    set parms($w:selectcolor) [option get $w SpreadSheet.selectColor {}]
    set parms($w:grabcolor)   [option get $w SpreadSheet.grabColor {}]
    set parms($w:entrycommand) ""
    set parms($w:command) ""
    set parms($w:statecommand) ""
    set parms($w:bd) 2
    set parms($w:relief) sunken
    set parms($w:yscrollcommand) ""
    set parms($w:expandcol) -1
    set parms($w:selectmode)  [option get $w SpreadSheet.selectmode {}]
    set parms($w:dragcommand) ""
    set parms($w:resize) 0

    set cell($w:0:0) ""
  }

  #############################################################################
  #
  # SpreadSheet::create $w [args...]
  #
  # Create a new spreadsheet.
  #
  proc create {w args} {
    variable parms

    set p_repaintPending($w) 0

    frame $w
    init $w
    configurev $w $args
    $w configure -bd $parms($w:bd) -relief $parms($w:relief) -takefocus 0 -width 10 -height 10

    bind $w <Delete> "SpreadSheet::requestDelete $w"
    bind $w <Configure> "SpreadSheet::updateSize $w"
  }

  proc tester {} {
    SpreadSheet::create .lb -bd 2 -relief sunken -yscrollcommand ".vb set" -height 15  -entrycommand entryManager
    scrollbar .vb -orient vertical -command "SpreadSheet::yview .lb"
    grid .lb -row 0 -column 0 -padx 20 -pady 20
    grid .vb -row 0 -column 1 -sticky ns
    SpreadSheet::addcolumn .lb -width 10 -header Fee
    SpreadSheet::addcolumn .lb -width 10 -header Fei
    SpreadSheet::addcolumn .lb -width 10 -header Foe
    SpreadSheet::addcolumn .lb -width 10 -header Fum

    SpreadSheet::insert .lb end {1 5 6 7}
    SpreadSheet::insert .lb end {2 12 18 99}
    SpreadSheet::insert .lb end {3 8 77 120}
    SpreadSheet::insert .lb end {4 12 18 35}
    SpreadSheet::insert .lb end {5 87 423 72}
    SpreadSheet::insert .lb end {6 786 72 281}
    SpreadSheet::insert .lb end {7 76 7823 76}
    SpreadSheet::insert .lb end {8 76 1289 89}
    SpreadSheet::insert .lb end {9 5 2013 1283}
    SpreadSheet::insert .lb end {10 12 12 123123}
    SpreadSheet::insert .lb end {11 123 87 28}
    SpreadSheet::insert .lb end {12 12783 765 123}
    SpreadSheet::insert .lb end {13 783 65 223}
    SpreadSheet::insert .lb end {14 183 76 129}
    SpreadSheet::insert .lb end {15 273 75 121}
    SpreadSheet::insert .lb end {16 1273 865 103}
    SpreadSheet::insert .lb end {17 1783 965 183}
  }
}

if { $spreadsheet_test } {
  SpreadSheet::tester
}


#
# entryManager canenter w r c data		see if we can enter data at (r,c)
# entryManager entrywidget w r c W width var	create special widget at (r,c) or return 0 for default
# entryManager close w r c data			close the entry widget on a cell and do a trandormation on the data
# entryManager canappend w r c data		can we append a row at (r,c)
# entryManager initentry w r c			if appending, get initial row values
#
proc entryManager {cmd args} {
  switch $cmd {
    canenter {
      set c [lindex $args 2]
      return [expr $c != 2 ]
    }
    entrywidget {
      set w [lindex $args 0]
      set r [lindex $args 1]
      set c [lindex $args 2]
      set W [lindex $args 3]
      set width [lindex $args 4]
      set variable [lindex $args 5]
      if { $c == 3 } {
	Dropbox::new $W -variable $variable -width [expr $width - 3] -bd 1 -highlightthickness 0
	Dropbox::itemadd $W "one"
	Dropbox::itemadd $W "two"
	Dropbox::itemadd $W "three"
	Dropbox::itemadd $W "four"

	return 1
      }
    }
    close {
      set data [lindex $args 3]
      return [string tolower $data]
    }
    canappend {
      set c [lindex $args 2]
      return [expr $c != 2 ]
    }
    initentry {
      return {0 0 0 0}
    }
  }
  return 0
}
