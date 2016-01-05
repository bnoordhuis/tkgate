#############################################################################
#
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
# Last edit by hansen on Sat Sep 26 16:26:04 2009
#

namespace eval MemView {
  #
  # Colors for highlighting read/write accesses
  #
  variable readColor \#88ee88
  variable writeColor \#ee8888

  #
  # Last access display variables
  #
  variable lastAddr
  variable lastData
  variable lastTime

  #
  # Window to use
  #
  variable mv_w

  #
  # Base address for current page
  #
  variable baseAddr
  variable reqBaseAddr

  #
  # Number of chars. in a word
  #
  variable wordLen

  #
  # Number of chars. in an address
  #
  variable addrLen

  #
  # Starting address of memory
  #
  variable startAddr

  #
  # Ending address of memory
  #
  variable stopAddr

  #
  # Number of words shown per page
  #
  variable pageWords

  #
  # Number of words shown on each line
  #
  variable lineWords

  #
  # Starting positions of words on a line
  #
  variable wordStartChar

  #
  # Overwrite with "text" at the specified position in a text widget 
  #
  proc overWrite {w pos text} {
    set l [string length $text]

    set cpos [$w index insert]

    $w delete $pos "$pos + $l chars"
    $w insert $pos $text

    if { $cpos != "" } { $w mark set insert $cpos }
  }

  proc dismiss {name} {
    variable mv_w

    destroy $mv_w($name)
  }

  proc seeKey {name s K A} {
    variable mv_w
    variable wordLen
    variable baseAddr
    variable pageWords
    variable lineWords

#    puts "seeKey $name s=$s k=$K a=$A"

    #
    # Get the window
    #
    set w $mv_w($name).text

    #
    # Get cursor postion. If no position, ignore the key
    #
    set cpos [$w index insert]
    if {$cpos == ""} return


    #
    # Get address for current postion (ignore if none)
    #
    set addr [getAddress $name $cpos]
    if {$addr == ""} return

    #
    # Figure out position within word
    #
    set apos [getPos $name $addr]
    if {[scan $cpos "%*d.%d" cpos_c] != 1} return
    if {[scan $apos "%*d.%d" apos_c] != 1} return
    set pww [expr $cpos_c - $apos_c]
    if {$pww < 0} return

    #
    # See if this was an arrow key
    #
    if { [lsearch {Up Down Left Right} $K] >= 0} {
      switch $K {
	Up { incr addr [expr -$lineWords($name)] }
	Down { incr addr $lineWords($name) }
	Left { incr pww -1 }
	Right { incr pww 1 }
      }

      if { $pww < 0 } {
	incr addr -1
	set pww $wordLen($name)
      }
      if { $pww > $wordLen($name) } {
	incr addr
	set pww 0
      }

      set cpos [getPos $name $addr]
      if {$cpos == "" } {
	$w mark unset insert
      } else {
	$w mark set insert "$cpos + $pww chars"
      }

      return
    }

    #
    # Not a character (i.e., key was a shift, control key, etc.) or not a hex digit 
    #
    if {$A == "" } return
    set A [string tolower $A]
    if {![string is xdigit $A] && $A != "x"} return

    #
    # Advance to next word if we are at end of another word
    #
    if {$pww >= $wordLen($name) } {
      incr addr
      set cpos [getPos $name $addr]
      if {$cpos == ""} return
      set apos $cpos
      set pww 0
    }

    #
    # Write the character.  Note that we do not actually need to change it in the text
    # widget.  Instead, we send the new data to the simulator which will then respond with
    # the updated data for this word.
    #
    set data [$w get $apos "$apos + $wordLen($name) chars"]
    set data [string replace $data $pww $pww $A]
    Simulator::cmdSend -ignoreerrors "\$memput $name $addr 'h$data"
    $w mark set insert "$cpos + 1 char"
  }

  proc seeButton {name x y} {
    variable mv_w

    set w $mv_w($name).text

    set pos [$w index @$x,$y]

    set addr [getAddress $name $pos]

#    puts "seeButton @$x,$y -- pos=$pos  addr=$addr"
    if { $addr == "" } {
      $w mark unset insert
    } else {
      $w mark set insert $pos
      $w see insert
    }
    focus $w
  }

  proc showPage {name addr} {
  }

  #############################################################################
  #
  # Decode a verilog value into an 'n' digit hex number
  #
  proc decodeData {data n} {
    #
    # Replace any digits with unknow bits as fully unknown characters
    #
#    while {[set p [string match "(....)" $data]] >= 0} {
#      set data [string replace $data $p [expr $p+5] "x"]
#    }

    #
    # Replace any z with x (probably should not be any)
    #
#    while {[set p [string match "z" $data]] >= 0} {
#      set data [string replace $data $p $p "x"]
#    }

    scan $data "%*d'%*c%s" baseData

    set length [string length $baseData]

    if { $length < $n} {
      if {[string index $baseData 0] == "x"} {
	set x "x"
      } else {
	set x "0"
      }
      set baseData [string repeat $x [expr $n-$length]]$baseData
    }

    return $baseData
  }

  #############################################################################
  #
  # Get the address for a screen position (or empty if not on a memory value) 
  #
  proc getAddress {name pos} {
    variable lineWords
    variable wordLen
    variable addrLen
    variable baseAddr
    variable pageWords
    variable lineWords
    variable wordStartChar
    variable baseAddr


    #
    # Get line and character position
    #
    if {[scan $pos "%d.%d" line cpos] != 2} { return "" }

    #
    # Clicked on address portion
    #
    if { $cpos < $addrLen($name)+3 }  { return "" }


    set found 0
    set wol 0
    foreach wsc $wordStartChar($name) {
      if {$cpos >= $wsc && $cpos <= [expr $wsc+$wordLen($name)]} {
	set found 1
	break
      }
      incr wol
    }

    if {!$found} { return "" }

    set addr [expr $baseAddr($name) + $lineWords($name)*($line-1) + $wol]

    if { $addr < $baseAddr($name) } { return "" }
    if { $addr > [expr $baseAddr($name) + $pageWords($name)] } { return "" }

    return $addr
  }

  #############################################################################
  #
  # Get the screen position for an address
  #
  proc getPos {name addr} {
    variable lineWords
    variable wordLen
    variable addrLen
    variable baseAddr
    variable pageWords
    variable lineWords
    variable wordStartChar
    
    #
    # If value is not on current page, return an empty string
    #
    if { $addr < $baseAddr($name) } { return "" }
    if { $addr >= [expr $baseAddr($name) + $pageWords($name)] } { return "" }

    #
    # Get the "address-on-page"
    #
    set aop [expr $addr - $baseAddr($name)]

    set line [expr $aop / $lineWords($name) + 1]
    set wol  [expr $aop % $lineWords($name)]

    set char [lindex $wordStartChar($name) $wol]

    return $line.$char
  }

  #############################################################################
  #
  # Receive data from memory
  #
  proc setData {name addr args} {
    variable mv_w
    variable wordLen

    #
    # Window could have closed while waiting for message
    #
    if {![info exists mv_w($name)]} { return }

    #puts "setData $name $addr $args"

    set w $mv_w($name).text

    foreach d $args {
      set dd  [decodeData $d $wordLen($name)]
      set pos [getPos $name $addr]

      if { $pos != "" } {
	overWrite $w $pos $dd
      }

      incr addr
    }
  }

  #############################################################################
  #
  # Set the last accessed address
  #
  proc setAccessAddr {name addr data type time}  {
    variable readColor
    variable writeColor
    variable lastAddr
    variable lastData
    variable lastTime
    variable addrLen
    variable wordLen
    variable baseAddr
    variable pageWords
    variable mv_w

#    puts "MemView::setAccessAddr <$name> <$addr> <$data> <$type> <$time>"

    set w $mv_w($name)

    set addrFormat %0$addrLen($name)x
    set lastAddr($name:$type) [format $addrFormat $addr]
    set lastData($name:$type) $data
    set lastTime($name:$type) [gat_formatTime $time]

    set pos [getPos $name $addr]
    $w.text tag delete $type
    if { $pos != "" } {
      $w.text tag add $type $pos "$pos + $wordLen($name) chars"
      $w.text tag configure $type -background [set ${type}Color]
    }
  }

  #############################################################################
  #
  # Configure the memory display.  This is normally called once after the
  # initial request for the memory dimensions returns.
  #
  proc configMemory {name dbits mstart mstop} {
    variable mv_w
    variable wordLen
    variable addrLen
    variable startAddr
    variable stopAddr
    variable baseAddr
    variable lineWords
    variable pageWords
    variable wordStartChar

    #puts "MemView::configMem $name $dbits $mstart $mstop"

    set w $mv_w($name).text

    #
    # Number of hex digits in an address
    #
    set addrLen($name) [string length [format %x $mstop]]

    #
    # Number of hex digits in word
    #
    set wordLen($name) [expr ($dbits+3)/4]

    #
    # Choose number of words per line and per page
    #
    if {$wordLen($name) <= 2} {
      set pageWords($name) 256
      set lineWords($name) 16
    } elseif {$wordLen($name) <= 8} {
      set pageWords($name) 128
      set lineWords($name) 8
    } elseif {$wordLen($name) <= 16} {
      set pageWords($name) 64
      set lineWords($name) 4
    } elseif {$wordLen($name) <= 32} {
      set pageWords($name) 32
      set lineWords($name) 2
    } else {
      set pageWords($name) 16
      set lineWords($name) 1
    }

    if { $pageWords($name) > [expr $mstop + 1] } {
      set pageWords($name) [expr $mstop+1]
    }
    if { $lineWords($name) > [expr $mstop + 1] } {
      set lineWords($name) [expr $mstop+1]
    }
    
    #puts "pageWord=$pageWords($name)"
    #puts "lineWord=$lineWords($name)"

    #
    # Range of addresses in memory
    #
    set startAddr($name) $mstart
    set stopAddr($name) $mstop

    #
    # Erase what is in the text buffer now
    #
    $w delete 1.0 end

    #
    # Format for addresses
    #
    set addrFormat %0$addrLen($name)x
    set dataFormat [string repeat "x" $wordLen($name)]

    #
    # Format a fake line to find positioning of words and width of text area
    #
    set wordStartChar($name) {}
    set line "[format $addrFormat 0] : "
    for {set i 0} {$i < $lineWords($name)} {incr i} {
      lappend wordStartChar($name) [string length $line]
      if {$i == 7} {
	set line "${line}$dataFormat  "
      } else {
	set line "${line}$dataFormat "
      }
    }

    #
    # Set up initial data pattern
    #
    set dstop $mstop
    if { $dstop > 256 } { set dstop 256 }
    for { set a $mstart } { $a <= $dstop } { incr a $lineWords($name)} {
      set line "[format $addrFormat $a] : "
      for {set i 0} {$i < $lineWords($name)} {incr i} {
	if {$i == 7} {
	  set line "${line}$dataFormat  "
	} else {
	  set line "${line}$dataFormat "
	}
      }
      $w insert end "$line\n"
    }

    #
    # Set the page increment/decrement
    #
    set w $mv_w($name)
    set lastPage [expr $mstop - ($pageWords($name)-1) ]
    incdecEntry_configure $w.top.base -delta $pageWords($name) -min $mstart -max $lastPage
    $w configure -width [string length $line]
#    $w.bottom.read.addr_v configure -width $addrLen($name)
#    $w.bottom.read.data_v configure -width [expr $wordLen($name) + 5]
#    $w.bottom.read.time_v configure -width 15
#    $w.bottom.write.addr_v configure -width $addrLen($name)
#    $w.bottom.write.data_v configure -width [expr $wordLen($name) + 5]
#    $w.bottom.write.time_v configure -width 15

    #
    # Request page to be filled with the actual data
    #
    loadPage $name $startAddr($name)
  }

  proc loadPage {name addr} {
    variable baseAddr
    variable pageWords

    set baseAddr($name) $addr
    set endAddr [expr $addr + $pageWords($name) - 1]

    #puts "loadPage $baseAddr($name) $endAddr"

    Simulator::cmdSend -ignoreerrors "\$memget $name $addr $endAddr"
    Simulator::cmdSend -ignoreerrors "\$memwatch $name $addr $endAddr"
  }

  proc windowConfigure {name} {
    variable mv_w

    set w $mv_w($name).text
    
    set width [winfo width $w]
    set height [winfo height $w]

    set idx [$w index @[expr ${width}-1],[expr ${height}-1]]
#    puts "windowConfigure $name ${width}x${height} idx=$idx"
  }


  #############################################################################
  #
  # Respond to change in requested base address
  #
  proc changeBase {name args} {
    variable mv_w
    variable addrLen
    variable pageWords
    variable lineWords
    variable baseAddr
    variable reqBaseAddr
    variable startAddr
    variable stopAddr

    #puts "changeBase $name $args :: reqBaseAddr=$reqBaseAddr($name)"

    if {[scan $reqBaseAddr($name) %x addr] == 0} return

    set addr [expr ($addr / $pageWords($name)) * $pageWords($name)]

    if { $addr < $startAddr($name) } { set addr $startAddr($name) }

    set lastPage [expr $stopAddr($name) - ($pageWords($name)-1) ]
    if { $addr > $lastPage } { set addr $lastPage }

    set baseAddr($name) $addr
    set endAddr [expr $addr + $pageWords($name)]

    Simulator::cmdSend -ignoreerrors "\$memget $name $addr $endAddr"
    Simulator::cmdSend -ignoreerrors "\$memwatch $name $addr $endAddr"

    set addrFormat %0$addrLen($name)x

    set w $mv_w($name).text
    set line 1
    for {set a $baseAddr($name) } {$a < $endAddr} { incr a $lineWords($name)} {
      overWrite $w $line.0 [format $addrFormat $a]
      incr line
    }

  }

  proc cleanup {name} {
    variable mv_w

    Simulator::cmdSend -ignoreerrors "\$memwatch $name delete"
    trace vdelete MemView::reqBaseAddr($name) w "MemView::changeBase $name"

    unset mv_w($name)
  }

  proc accessInfo {w args} {
    variable lastAddr
    variable lastData
    variable lastTime

    set label ""
    set name ""
    set bg "red"
    set type "read"
    parseargs $args {-name -label -bg -type}

    set lastAddr($name:$type) -
    set lastData($name:$type) -
    set lastTime($name:$type) -

    frame $w -bd 1 -relief solid
    label $w.l -text $label -bg $bg
    label $w.addr_l -text "Address:"
    label $w.data_l -text "Data:"
    label $w.time_l -text "Time:"
    label $w.addr_v -textvariable MemView::lastAddr($name:$type) -width 15 -anchor w
    label $w.data_v -textvariable MemView::lastData($name:$type) -width 15 -anchor w
    label $w.time_v -textvariable MemView::lastTime($name:$type) -width 15 -anchor w

    grid $w.l -row 0 -column 0 -columnspan 2 -sticky ew 
    grid $w.addr_l -row 1 -column 0 -sticky e -padx 3
    grid $w.addr_v -row 1 -column 1 -sticky w -padx 3
    grid $w.data_l -row 2 -column 0 -sticky e -padx 3
    grid $w.data_v -row 2 -column 1 -sticky w -padx 3
    grid $w.time_l -row 3 -column 0 -sticky e -padx 3
    grid $w.time_v -row 3 -column 1 -sticky w -padx 3

  }

  #
  # Create a memory viewer for the specified memory
  #
  proc new {name args} {
    variable mv_w
    variable reqBaseAddr
    variable readColor
    variable writeColor

    #
    # Window already exists
    #
    if {[info exists mv_w($name)]} {
      raise $mv_w($name)
      return
    }

    #
    # Set the requested base address
    #
    set reqBaseAddr($name) 0

    set title $name
    parseargs $args {-title}

    #
    # Choose a top-level window for the dip setter
    #
    for {set i 0} { [catch { set w [toplevel .memview$i] }] } { incr i } { }
    set mv_w($name) $w

    wm title $w "TkGate: Memory $title"
    wm geometry $w [offsetgeometry . 50 50]
    wm transient $w .

    dialogImage $w.image -image [gifI memory_view.gif] -caption [m memview.title] -explaination [m memview.exp]
    pack $w.image -side left -fill y


    frame $w.bottom
    button $w.bottom.dismiss -text [m b.dismiss] -command "MemView::dismiss $name"

    accessInfo $w.bottom.read -label "Last Read Access" -bg $readColor -type read -name $name
    accessInfo $w.bottom.write -label "Last Write Access" -bg $writeColor -type write -name $name
    pack $w.bottom.read $w.bottom.write -side left -padx 5 -pady 5
    pack $w.bottom.dismiss -side right -padx 5 -pady 5 -anchor se
    pack $w.bottom -fill both -side bottom

    frame $w.top
    label $w.top.base_l -text "Page: "
    incdecEntry $w.top.base -width 12 -variable MemView::reqBaseAddr($name) -format %x -validatecommand hexValidate

    
    
    pack $w.top.base_l $w.top.base -side left -padx 5 -pady 5
    pack $w.top -fill both -side top



    text $w.text -wrap none -font hdlFont -height 16 -width 70
    pack $w.text -padx 5 -pady 5

    bindtags $w.text [list $w.text]
    bind $w.text <KeyPress> "MemView::seeKey $name %s %K %A"
    bind $w.text <Button-1> "MemView::seeButton $name %x %y"
    bind $w.text <Configure> "MemView::windowConfigure $name"
    bind $w.text <Destroy> "MemView::cleanup $name"
    bind .scope <Destroy> "+ destroy $w"

    trace variable MemView::reqBaseAddr($name) w "MemView::changeBase $name"

    Simulator::cmdSend "\$netinfo $name"
 }
}