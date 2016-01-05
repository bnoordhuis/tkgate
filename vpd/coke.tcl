#   Copyright (C) 1987-2005 by Jeffery P. Hansen
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
# Last edit by hansen on Tue Jul 24 13:55:55 2007

font create title_f -family helvetica -weight bold -size 66
font create button_f -family helvetica -weight bold -size 10
font create empty_f -family helvetica -size 8
font create coin_f -family helvetica -size 8

#############################################################################
#
# Allow all proceedures in the coke machine to be executed from verilog.
#
if {[info exists tkgate_isInitialized]} {
  VPD::register CokeMachine
  VPD::allow CokeMachine::*
}


#############################################################################
#
# TkGate Interactive Module for Coke machine
#
namespace eval CokeMachine {
  # size of the coke machine (do not change)
  variable mWidth  300
  variable mHeight 520

  # options
  variable useDollar

  # colors of cans in slots
  variable canColors {red red gray50 orange blue darkred}

  # Array of windows for coke machines
  variable coke_w

  # number of each type of coin available for spending
  variable numQuarters
  variable numDimes
  variable numNickles
  variable numDollars
  variable numBogus

  # number of cans that have been bought
  variable numBought

  # number of each type of drink
  variable numCans

  # number of coins each type available for change
  variable numChange
  variable changeQuarters
  variable changeDimes
  variable changeNickles

  # number of coins each type held in machine
  variable holdQuarters
  variable holdDimes
  variable holdNickles
  variable holdDollars
  variable holdBogus

  # number of coins each type inserted into machine
  variable insertQuarters
  variable insertDimess
  variable insertNickles

  variable displayCost
  variable ENABLED

  #############################################################################
  #
  # Input signals
  #

  # lights
  variable isEmpty
  variable noChange
  variable billLight

  # input to bill reader and bill in scanner
  variable readyBill
  variable scannerBill


  #############################################################################
  #
  # Output signals
  #
  variable osigPRESS
  variable osigEMPTY
  variable osigINSQ
  variable osigINSD
  variable osigINSN
  variable osigNUMQ
  variable osigNUMD
  variable osigNUMN
  variable osigBILLSNS
  variable osigBILLOK
  variable osigBILLNG
  variable osigBILLACK
  variable osigCNRET

  #############################################################################
  #
  # Pending insertions
  #
  variable pendingCoins


  #############################################################################
  #
  # Initialize the coke machine.
  #
  proc initState {n} {
    variable coke_w
    variable numBought
    variable numQuarters
    variable numDimes
    variable numNickles
    variable changeQuarters
    variable changeDimes
    variable changeNickles
    variable holdQuarters
    variable holdDimes
    variable holdNickles
    variable holdDollars
    variable holdBogus
    variable insertQuarters
    variable insertDimes
    variable insertNickles
    variable numDollars
    variable numBogus
    variable isEmpty
    variable numCans
    variable numChange
    variable noChange
    variable billLight
    variable readyBill
    variable scannerBill
    variable osigPRESS
    variable osigEMPTY
    variable osigNUMQ
    variable osigNUMD
    variable osigNUMN
    variable osigINSQ
    variable osigINSD
    variable osigINSN
    variable osigBILLSNS
    variable osigBILLOK
    variable osigBILLNG
    variable osigBILLACK
    variable osigCNRET
    variable pendingCoins
    variable canColors

#    puts "CokeMachine::initState-1"

    set w "."
    catch { set w $coke_w($n) }

    set numQuarters($n) 20

#    puts "CokeMachine::initState-1a"

    set numDimes($n) 20
    set numNickles($n) 20
    set numDollars($n) 20
    set numBogus($n) 20
    set isEmpty($n) {0 0 0 0 0 0}
    set numCans($n) {10 12 8 7 15 11}
    set numChange($n) {10 8 10}

#    puts "CokeMachine::initState-1b"

    set changeQuarters($n) 10
    set changeDimes($n) 10
    set changeNickles($n) 10

    set holdQuarters($n) 0
    set holdDimes($n) 0
    set holdNickles($n) 0
    set holdDollars($n) 0
    set holdBogus($n) 0

#    puts "CokeMachine::initState-1c"

    set insertQuarters($n) 0
    set insertDimes($n) 0
    set insertNickles($n) 0

    set pendingCoins($n) {}

#    puts "CokeMachine::initState-1d"

    set noChange($n) 0
#    puts "CokeMachine::initState-1e"
    set billLight($n) 0
#    puts "CokeMachine::initState-1f"

    if {[catch { set readyBill($n) "" } xxx]} {
      puts "CokeMachine::xxx=$xxx"
    }
#    puts "CokeMachine::initState-1g"
    set scannerBill($n) ""

#    puts "CokeMachine::initState-2"

    set numBought($n) 0
    catch { $w.c delete boughtCans }
    for {set i 0 } { $i < 6 } { incr i } {
      catch { reloadHopper $w.c.int.h$i $n -color [lindex $canColors $i] -column $i  }
    }

    set osigPRESS($n) {0 0 0 0 0 0}
    set osigEMPTY($n) {0 0 0 0 0 0}
    set osigNUMQ($n) 7
    set osigNUMD($n) 7
    set osigNUMN($n) 7
    set osigINSQ($n) 0
    set osigINSD($n) 0
    set osigINSN($n) 0
    set osigBILLSNS($n) 0
    set osigBILLOK($n) 0
    set osigBILLNG($n) 0
    set osigBILLACK($n) 0
    set osigCNRET($n) 0

#    puts "CokeMachine::initState-3"
  }

  #############################################################################
  #
  # Respond to a change in the bill that is in the scanner.
  #
  proc seeScannerBill {n args} {
    variable coke_w
    variable scannerBill
    variable osigBILLOK
    variable osigBILLNG

    $coke_w($n).c.int delete scanBill
    switch $scannerBill($n) {
      D { makeDollar $coke_w($n).c.int 254 135 -tag scanBill }
      B { makeBogus  $coke_w($n).c.int 254 135 -tag scanBill }
    }

    switch $scannerBill($n) {
      D { set osigBILLOK($n) 1; set osigBILLNG($n) 0  }
      B { set osigBILLOK($n) 0; set osigBILLNG($n) 1  }
      default { set osigBILLOK($n) 0; set osigBILLNG($n) 0 }
    }

  }

  #############################################################################
  #
  # Pickup a bill that is in the "ready" slot.
  #
  proc pickupReady {n args} {
    set dw [Dragger::make -command "CokeMachine::dropCoin $n $CokeMachine::readyBill($n)" -bd 0]
    makeDragCoin $dw $n $CokeMachine::readyBill($n)
    set CokeMachine::readyBill($n) ""
  }

  #############################################################################
  #
  # Respond to a change in the "ready" bill.  The ready bill is the one that
  # is ready to be sucked into the scanner.
  #
  proc seeReadyBill {n args} {
    variable coke_w
    variable readyBill
    variable osigBILLSNS

    puts "CokeMachine::seeReadyBill"

    set w $coke_w($n).c.ext.dslot

    $w delete readyBill
    switch $readyBill($n) {
      D { makeDollar $w 25 22 -tag readyBill }
      B { makeBogus  $w 25 22 -tag readyBill }
    }

    if { $readyBill($n) != "" } {
      $w bind readyBill <1> "CokeMachine::pickupReady $n"
      set osigBILLSNS($n) 1
    } else {
      puts "CokeMachine::seeReadyBill-1"
      set osigBILLSNS($n) 0
      puts "CokeMachine::seeReadyBill-2"
    }

  }

  #############################################################################
  #
  # Make a coin/bill that can be dragged around.
  #
  proc makeDragCoin {dw n coin} {

    if { $dw != "" } {
      switch $coin {
	25 {
	  canvas $dw.c -width 25 -height 23
	  makeCoin $dw.c 12 12 -value 25
	}
	10 {
	  canvas $dw.c -width 19 -height 18
	  makeCoin $dw.c 9 10 -value 10
	}
	5 {
	  canvas $dw.c -width 22 -height 20
	  makeCoin $dw.c 10 11 -value 5
	}
	D {
	  canvas $dw.c -width 27 -height 17
	  makeDollar $dw.c 14 9
	}
	B {
	  canvas $dw.c -width 27 -height 17
	  makeBogus $dw.c 14 9
	}
	A {
	  canvas $dw.c -width 35 -height 35
	  makeCoin $dw.c 20 12 -value 25
	  makeCoin $dw.c 10 25 -value 10
	  makeCoin $dw.c 25 12 -value 5
	}
      }
      pack $dw.c
    }
  }

  #############################################################################
  #
  # Select a coin which has been selected with the mouse button
  #
  proc selectCoin {n coin x y} {
    variable numQuarters
    variable numDimes
    variable numNickles
    variable numDollars
    variable numBogus
    variable ENABLED

    if {!$ENABLED($n)} return

    set ok 0

    #
    # check for availability of funds
    #
    switch $coin {
      25 { if {$numQuarters($n) > 0} { set ok 1}}
      10 { if {$numDimes($n) > 0} { set ok 1}}
      5 { if {$numNickles($n) > 0} { set ok 1}}
      D { if {$numDollars($n) > 0} { set ok 1}}
      B { if {$numBogus($n) > 0} { set ok 1}}
    }

    if {!$ok} return


    set dw [Dragger::make -command "CokeMachine::dropCoin $n $coin" -bd 0]
    makeDragCoin $dw $n $coin
    switch $coin {
      25 { incr numQuarters($n) -1 }
      10 { incr numDimes($n) -1 }
      5 { incr numNickles($n) -1 }
      D { incr numDollars($n) -1 }
      B { incr numBogus($n) -1 }
    }
  }

  #############################################################################
  #   
  # Return a coin (bill) that was not put into the slot (bill reader)
  #   
  proc returnCoin {n coin w x y} {
    variable numQuarters
    variable numDimes
    variable numNickles
    variable numDollars
    variable numBogus
    switch $coin {
      25 { incr numQuarters($n) }
      10 { incr numDimes($n) }
      5 { incr numNickles($n) }
      D { incr numDollars($n) }
      B { incr numBogus($n) }
    }
  }

  #############################################################################
  #
  # Respond to a change in the acknowledge line
  #
  proc insertAck {n args} {
    variable osigINSQ
    variable osigINSD
    variable osigINSN
    variable pendingCoins
    variable ENABLED

    if {!$ENABLED($n)} return


    set value 1
    parseargs $args {-value -tag}

    if { $value } {
      set osigINSQ($n) 0
      set osigINSD($n) 0
      set osigINSN($n) 0
    } else {
      set pendingCoins($n) [lrange $pendingCoins($n) 1 end]

      if {[llength $pendingCoins($n)] > 0} {
	switch [lindex $pendingCoins($n) 0] {
	  25 { set osigINSQ($n) 1 }
	  10 { set osigINSD($n) 1 }
	  5 { set osigINSN($n) 1 }
	}
      }
    }
  }

  #############################################################################
  #
  # Respond to a coin (or bill) dropped in the coin slot (or bill reader)
  #
  proc dropCoin {n coin w x y} {
    variable insertQuarters
    variable insertDimes
    variable insertNickles
    variable readyBill
    variable coke_w
    variable osigINSQ
    variable osigINSD
    variable osigINSN
    variable osigBILLSNS
    variable pendingCoins

    set ok 0

    if { $w == "$coke_w($n).c.ext.cslot" ||  $w == "$coke_w($n).c.ext.cslot.e"} {
      switch $coin {
	25 { 
	  incr insertQuarters($n)
	  if {[llength $pendingCoins($n)] == 0} { set osigINSQ($n) 1 }
	  lappend pendingCoins($n) 25
	  set ok 1
	}
	10 {
	  incr insertDimes($n)
	  if {[llength $pendingCoins($n)] == 0} { set osigINSD($n) 1 }
	  lappend pendingCoins($n) 10
	  set ok 1
	}
	5 {
	  incr insertNickles($n)
	  if {[llength $pendingCoins($n)] == 0} { set osigINSN($n) 1 }
	  lappend pendingCoins($n) 5
	  set ok 1
	}
      }
    }

    if { $w == "$coke_w($n).c.ext.dslot" && $readyBill($n) == ""} {
      if { $coin == "D" } { set readyBill($n) "D"; set ok 1 }
      if { $coin == "B" } { set readyBill($n) "B"; set ok 1 }
      if { $readyBill($n) != ""} {
	set osigBILLSNS($n) 1 
      }
    }

    if {!$ok} {
      set x [expr $x + [winfo rootx $w]]
      set y [expr $y + [winfo rooty $w]]

      set X 20
      switch $coin {
	25 { set Y 30 } 
	10 { set Y 60 } 
	5  { set Y 90 } 
	D  { set Y 120 } 
	B  { set Y 150 } 
      }

      set X [expr $X + [winfo rootx $coke_w($n)]]
      set Y [expr $Y + [winfo rooty $coke_w($n)]]

      set dw [Dragger::make -command "CokeMachine::returnCoin $n $coin" -bd 0 -interactive 0]
      makeDragCoin $dw $n $coin
      Dragger::animate -speed 10
      Dragger::animate -step 30
      Dragger::animate $x $y $X $Y
    }
  }

  #############################################################################
  #
  # Make a coin of a particular demonination.
  #
  proc makeCoin {w x y args} {
    set size 10

    set tag ""
    set value 25
    parseargs $args {-value -tag}

    if {$value == 25 } { set size 10 }
    if {$value == 10 } { set size 8 }
    if {$value == 5 } { set size 9 }

    set x1 [expr $x - $size]
    set y1 [expr $y - $size]
    set x2 [expr $x + $size]
    set y2 [expr $y + $size]
    $w create oval $x1 $y1 $x2 $y2 -fill gray20 -tags $tag
    incr x1 2
    incr y1 -1
    incr x2 2
    incr y2 -1
    $w create oval $x1 $y1 $x2 $y2 -fill gray50 -tags $tag
    $w create text [expr $x + 2] $y -text $value -font coin_f -tags $tag
  }

  #############################################################################
  #
  # Make a dollar bill
  #
  proc makeDollar {w x y args} {
    set tag ""
    parseargs $args {-tag}

    set x1 [expr $x - 13]
    set y1 [expr $y - 8]
    set x2 [expr $x + 13]
    set y2 [expr $y + 8]

    $w create rectangle $x1 $y1 $x2 $y2 -fill "\#008800" -tags $tag
    $w create text $x $y -text "\$1" -tags $tag
  }

  #############################################################################
  #
  # Make a bogus bill
  #
  proc makeBogus {w x y args} {
    set tag ""
    parseargs $args {-tag}

    set x1 [expr $x - 13]
    set y1 [expr $y - 8]
    set x2 [expr $x + 13]
    set y2 [expr $y + 8]

    $w create rectangle $x1 $y1 $x2 $y2 -fill white -tags $tag
    $w create text $x $y -text "bogus" -font empty_f -tags $tag
  }

  #############################################################################
  #
  # Make the list of coins that can be dragged and inserted into the machine.
  #
  proc makeCoinList {w n} {
    variable useDollar

    makeCoin $w 20 30 -value 25 -tag usrQuarter
    makeCoin $w 20 60 -value 10 -tag usrDime
    makeCoin $w 20 90 -value 5  -tag usrNickle

    $w bind usrQuarter <1> "CokeMachine::selectCoin $n 25 %x %y"
    $w bind usrDime <1> "CokeMachine::selectCoin $n 10 %x %y"
    $w bind usrNickle <1> "CokeMachine::selectCoin $n 5 %x %y"

    $w create text 40 30 -text "X" 
    $w create text 40 60 -text "X" 
    $w create text 40 90 -text "X" 

    label $w.qcount -textvariable CokeMachine::numQuarters($n)
    $w create window 55 30 -window $w.qcount

    label $w.dcount -textvariable CokeMachine::numDimes($n)
    $w create window 55 60 -window $w.dcount

    label $w.ncount -textvariable CokeMachine::numNickles($n)
    $w create window 55 90 -window $w.ncount

    if {$useDollar($n)} {
      makeDollar $w 20 120 -tag usrDollar
      makeBogus $w 20 150 -tag usrBogus
      $w bind usrDollar <1> "CokeMachine::selectCoin $n D %x %y"
      $w bind usrBogus <1> "CokeMachine::selectCoin $n B %x %y"
      $w create text 40 120 -text "X" 
      $w create text 40 150 -text "X" 
      label $w.lcount -textvariable CokeMachine::numDollars($n)
      $w create window 55 120 -window $w.lcount

      label $w.bcount -textvariable CokeMachine::numBogus($n)
      $w create window 55 150 -window $w.bcount
    }
  }

  #############################################################################
  #
  # See a change in cost
  #
  proc setCost {n value args} {
    variable displayCost

    set displayCost($n) [format "\$%0.2f" [expr $value * 0.05]]
  }

  #############################################################################
  #
  # Respond to a change in the reset line.  A falling edge resets the machine,
  # and a non-zero value allows the machine to operate.
  #
  proc resetMachine {n state args} {
    variable coke_w
    variable canColors
    variable ENABLED

    set w $coke_w($n)

    set ENABLED($n) $state
    if {$state == 0} {
      initState $n
    }
  }


  #############################################################################
  #
  # See a change in any lights
  #
  proc seeLight {n args} {
    variable coke_w
    variable isEmpty
    variable noChange
    variable billLight
    variable useDollar

    for {set i 0 } { $i < 6 } { incr i } {
      if {[lindex $isEmpty($n) $i]} {
	$coke_w($n).c.ext.bbar.b$i.e configure -bg "\#ffcc40" -fg "\#904000"
      } else {
	$coke_w($n).c.ext.bbar.b$i.e configure -bg "\#a05010" -fg "\#904000"
      }
    }

    if {$noChange($n)} {
      $coke_w($n).c.ext.cslot.e configure -bg "\#ffcc40" -fg "\#904000"
    } else {
      $coke_w($n).c.ext.cslot.e configure -bg "\#a05010" -fg "\#904000"
    }

    if {$useDollar($n)} {
      if {$billLight($n)} {
	$coke_w($n).c.ext.dslot itemconfigure billok -fill green
      } else {
	$coke_w($n).c.ext.dslot itemconfigure billok -fill darkgreen
      }
    }
  }


  #############################################################################
  #
  # Set the state of the empty light
  #
  proc setEmpty {n elist} {
    variable isEmpty 

    set isEmpty($n) $elist
  }


  #############################################################################
  #
  # Dispense product
  #
  proc dispense {n i} {
    variable coke_w
    variable canColors
    variable numBought
    variable mHeight
    variable osigEMPTY
    variable ENABLED

    if {!$ENABLED($n)} return

    set w $coke_w($n).c.int.h$i

    set L [winfo children $w]
    if {[llength $L] == 0 } return

    if {[llength $L] == 1 } {
      set osigEMPTY($n) [lreplace $osigEMPTY($n) $i $i 1]
    }


    set d [lindex $L [expr [llength $L]-1]]

    set w [winfo width  $coke_w($n).c.int.h$i]
    set h [winfo height  $coke_w($n).c.int.h$i]
    set x0 [expr [winfo rootx $coke_w($n).c.int.h$i] + $w/2]
    set y0 [expr [winfo rooty $coke_w($n).c.int.h$i] + $h]

    destroy $d


    set rX [winfo rootx $coke_w($n)]
    set rY [winfo rooty $coke_w($n)]

    set x1 [expr $rX + 500]
    set y1 [expr $rY + 460]

    set x2 [expr $rX + 200]
    set y2 [expr $rY + 460]

    set x3 [expr $rX + 25 + ($numBought($n) % 50)*14]
    set y3 [expr $rY + $mHeight + 45 + ($numBought($n) / 50)*30]


    set dw [Dragger::make -command "CokeMachine::drinkCan $n $i" -bd 0 -interactive 0]
    if {$dw != ""} {
      frame $dw.f -width 20 -height 15 -bg [lindex $canColors $i]
      pack $dw.f
      Dragger::animate -speed 10
      Dragger::animate -step 10
      Dragger::animate $x0 $y0 $x1 $y1 -jump $x2 $y2 $x3 $y3
    } else {
      drinkCan $n $i
    }
  }

  #############################################################################
  #
  # Add an empty can to the empties list 
  #
  proc drinkCan {n i args} {
    variable coke_w
    variable numBought
    variable mHeight
    variable canColors

    set x [expr 25 + ($numBought($n) % 50)*14]
    set y [expr $mHeight + 45 + ($numBought($n) / 50)*30]
    $coke_w($n).c create rectangle $x $y [expr $x + 10] [expr $y + 18] -fill [lindex $canColors $i] -outline "" -tags boughtCans

    incr numBought($n)
  }

  #############################################################################
  #
  # Dump coins into the coin hold
  #
  proc dumpInHold {n nQ nD nN args} {
    variable holdQuarters
    variable holdDimes
    variable holdNickles

    incr holdQuarters($n) $nQ
    incr holdDimes($n) $nD
    incr holdNickles($n) $nN
  }

  #############################################################################
  #
  # Dump coins into the coin hold
  #
  proc dumpBack {n nQ nD nN args} {
    variable numQuarters
    variable numDimes
    variable numNickles

    incr numQuarters($n) $nQ
    incr numDimes($n) $nD
    incr numNickles($n) $nN
  }

  #############################################################################
  #
  # Return inserted coins
  #
  proc returnInserted {n} {
    variable coke_w
    variable insertQuarters
    variable insertDimes
    variable insertNickles
    variable numQuarters
    variable numDimes
    variable numNickles
    variable ENABLED

    if {!$ENABLED($n)} return

    set L [list $insertQuarters($n) $insertDimes($n) $insertNickles($n)]

    if { $L == {0 0 0} } return

    set insertQuarters($n) 0
    set insertDimes($n) 0
    set insertNickles($n) 0

    set X0 [winfo rootx $coke_w($n).c]
    set Y0 [winfo rooty $coke_w($n).c]

    set X1 [winfo rootx $coke_w($n).c.int]
    set Y1 [winfo rooty $coke_w($n).c.int]

    set dw [Dragger::make -command "CokeMachine::dumpBack $n $L" -bd 0 -interactive 0]
    if {$dw != ""} {
      makeDragCoin $dw $n A
      Dragger::animate -speed 10
      Dragger::animate -step 40
      Dragger::animate [expr $X1 + 250] [expr $Y1 + 200] [expr $X0 + 50] [expr $Y0 + 50]
    } else {
      set numQuarters($n) [lindex $L 0]
      set numDimes($n) [lindex $L 1]
      set numNickles($n) [lindex $L 2]
    }
  }

  #############################################################################
  #
  # Commit inserted coins to buying a product
  #
  proc commitInserted {n} {
    variable insertQuarters
    variable insertDimes
    variable insertNickles
    variable holdQuarters
    variable holdDimes
    variable holdNickles
    variable coke_w
    variable ENABLED

    if {!$ENABLED($n)} return

    set L [list $insertQuarters($n) $insertDimes($n) $insertNickles($n)]

    if { $L == {0 0 0} } return

    set insertQuarters($n) 0
    set insertDimes($n) 0
    set insertNickles($n) 0

    set X [winfo rootx $coke_w($n).c.int]
    set Y [winfo rooty $coke_w($n).c.int]

    set dw [Dragger::make -command "CokeMachine::dumpInHold $n $L" -bd 0 -interactive 0]
    if {$dw != ""} {
      makeDragCoin $dw $n A
      Dragger::animate -speed 20
      Dragger::animate -step 20
      Dragger::animate -offset $X $Y 250 200 200 250 200 350 250 450
    } else {
      set holdQuarters($n) [lindex $L 0]
      set holdDimes($n) [lindex $L 1]
      set holdNickles($n) [lindex $L 2]
    }
  }

  #############################################################################
  #
  # Eject change
  #
  proc ejectChange {n coin} {
    variable numQuarters
    variable numDimes
    variable numNickles
    variable changeQuarters
    variable changeDimes
    variable changeNickles
    variable osigNUMQ
    variable osigNUMD
    variable osigNUMN
    variable coke_w
    variable ENABLED

    if {!$ENABLED($n)} return

    set ok 0

    switch $coin { 
      25 {
	if { $changeQuarters($n) > 0 } {
	  incr changeQuarters($n) -1
	  incr numQuarters($n)
	  set osigNUMQ($n) [min $changeQuarters($n) 7]
	  set ok 1
	}
      }
      10 {
	if { $changeDimes($n) > 0 } {
	  incr changeDimes($n) -1
	  incr numDimes($n)
	  set osigNUMD($n) [min $changeDimes($n) 7]
	  set ok 1
	}
      }
      5 {
	if { $changeNickles($n) > 0 } {
	  incr changeNickles($n) -1
	  incr numNickles($n)
	  set osigNUMN($n) [min $changeNickles($n) 7]
	  set ok 1
	}
      }
    }

    if {$ok} {
      set X [winfo rootx $coke_w($n)]
      set Y [winfo rooty $coke_w($n)]

      set dw [Dragger::make -bd 0 -interactive 0]
      if { $dw != "" } {
	makeDragCoin $dw $n $coin
	Dragger::animate -speed 20
	Dragger::animate -step 20
	Dragger::animate -offset $X $Y 300 400 20 50
      }
    }
  }

  #############################################################################
  #
  # Move bill into reader (and bill in reader into inserted coin bucket)
  #
  proc billIn {n args} {
    variable readyBill
    variable scannerBill
    variable holdDollars
    variable holdBogus
    variable osigBILLSNS
    variable osigBILLACK
    variable ENABLED

    if {!$ENABLED($n)} return

    set value 1
    parseargs $args {-value}

    if {$value } {
      # Bill in signal is asserted
      switch $scannerBill($n) {
	D { incr holdDollars($n) }
	B { incr holdBogus($n) }
      }
      set scannerBill($n) $readyBill($n)
      set readyBill($n) ""

      set osigBILLSNS($n) 0 
      set osigBILLACK($n) 1
    } else {
      # Bill in signal is unasserted
      set osigBILLACK($n) 0
    }
  }

  #############################################################################
  #
  # Reject bill in reader
  #
  proc billOut {n args} {
    variable readyBill
    variable scannerBill
    variable numDollars
    variable numBogus
    variable osigBILLSNS
    variable osigBILLACK
    variable ENABLED

    if {!$ENABLED($n)} return

    set value 1
    parseargs $args {-value}

    if {$value} {
      switch $scannerBill($n) {
	D { incr numDollars($n) }
	B { incr numBogus($n) }
      }
      set scannerBill($n) ""
      switch $readyBill($n) {
	D { incr numDollars($n) }
	B { incr numBogus($n) }
      }
      set readyBill($n) ""

      set osigBILLSNS($n) 0 
      set osigBILLACK($n) 1
    } else {
      set osigBILLACK($n) 0 
    }
  }

  #############################################################################
  #
  # But a drink button into the "down" state.
  #
  proc buttonDown {w n id} {
    variable osigPRESS
    $w configure -relief sunken

    set osigPRESS($n) [lreplace $osigPRESS($n) $id $id 1]
  }

  #############################################################################
  #
  # But a drink button into the "up" state.
  #
  proc buttonUp {w n id} {
    variable coke_w
    variable osigPRESS
    $w configure -relief raised

    set osigPRESS($n) [lreplace $osigPRESS($n) $id $id 0]
  }

  #############################################################################
  #
  # Create a drink selection button
  #
  proc evButton {w n args} {

    set bcolor \#ffeedd

    set title ""
    set id 0
    parseargs $args {-title -id}

    frame $w -bd 2 -relief raised -bg $bcolor
    label $w.e -text empty -font empty_f -bg "\#a05010" -fg "\#904000" -borderwidth 0
    label $w.l -text $title -width 12 -font button_f -bg $bcolor
    pack $w.l -side bottom
    pack $w.e -side top -anchor w

    bind $w <ButtonPress> "CokeMachine::buttonDown $w $n $id"
    bind $w <ButtonRelease> "CokeMachine::buttonUp $w $n $id"
    bind $w.e <ButtonPress> "CokeMachine::buttonDown $w $n $id"
    bind $w.e <ButtonRelease> "CokeMachine::buttonUp $w $n $id"
    bind $w.l <ButtonPress> "CokeMachine::buttonDown $w $n $id"
    bind $w.l <ButtonRelease> "CokeMachine::buttonUp $w $n $id"
  }

  #############################################################################
  #
  # Create the button box
  #
  proc evButtonBox {w n args} {
    frame $w
    evButton $w.b0 $n -title Coke -id 0
    evButton $w.b1 $n -title Coke -id 1
    evButton $w.b2 $n -title "Diet Coke" -id 2
    evButton $w.b3 $n -title "Root Beer" -id 3
    evButton $w.b4 $n -title "Pocari Sweat" -id 4
    evButton $w.b5 $n -title "Dr. Pepper" -id 5
    pack $w.b0
    pack $w.b1
    pack $w.b2
    pack $w.b3
    pack $w.b4
    pack $w.b5
  }

  #############################################################################
  #
  # Create the dollar bill reader external view
  #
  proc evDollarSlot {w args} {
    canvas $w -bg black -width 70 -height 30 -bg grey -bd 2 -relief raised

    $w create rectangle 10 10 40 25 -fill gray60
    $w create rectangle 10 10 40 13 -fill gray40

    $w create polygon 12 23   15 18   18 23   -fill green -tags billok
    $w create polygon 22 23   25 18   28 23   -fill green -tags billok
    $w create polygon 32 23   35 18   38 23   -fill green -tags billok

    $w create text 60 18 -text "\$1"
  }

  #############################################################################
  #
  # Create the coin slot external view
  #
  proc evCoinSlot {w n args} {
    canvas $w -bg black -width 70 -height 60 -bg grey -bd 2 -relief raised 

    # coin slot
    $w create line 65 10 65 32 -width 4

    # 
    label $w.e -text "no\nchange" -font empty_f -bg "\#a05010" -fg "\#904000" -borderwidth 0
    $w create window 10 10 -window $w.e -anchor nw

    # coin return lever
    $w create polygon 10 40   35 40   35 45    20 45     15 50   10 45   -fill gray20 -tags coinReturnLev

    bind $w <1> "CokeMachine::setCRLever $n 1"
    bind $w <ButtonRelease-1> "CokeMachine::setCRLever $n 0"

#    pack $w.slot -padx 10 -pady 20
  }

  proc setCRLever {n state} {
    variable coke_w
    variable ENABLED
    variable osigCNRET

    if {!$ENABLED($n)} return

    set w $coke_w($n).c.ext.cslot

    set L {10 40   35 40   35 45    20 45     15 50   10 45}

    if { $state } {
      set R {}
      foreach {x y} $L {
	lappend R [expr -$y + 60] [expr $x + 25]
      }

      set osigCNRET($n) 1
      eval "$w coords coinReturnLev  $R"
    } else {
      set osigCNRET($n) 0
      eval "$w coords coinReturnLev  $L"
    }
  }

  #############################################################################
  #
  # Create the coin return external view
  #
  proc evCoinReturn {w args} {
    canvas $w -bg black -width 35 -height 35 -bg grey -bd 2 -relief raised 

    # coin return
    $w create rectangle 10 10 30 30 -fill gray20
  }

  #############################################################################
  #
  # Create the can out slot external view
  #
  proc evCanOutSlot {w args} {
    frame $w -bg black -width 175 -height 30
  }

  #############################################################################
  #
  # Create the drink cost window external view
  #
  proc evCostDisplay {w n} {
    variable displayCost

    frame $w -bd 2 -relief raised
    label $w.l -textvariable CokeMachine::displayCost($n) -width 5 -height 2 -bg white -fg red3
    pack $w.l -padx 3 -pady 3
  }


  proc reloadHopper {w n args} {
    variable numCans

    set width 25
    set color red
    set column 0
    parseargs $args {-width -height -color -column}

    set L [winfo children $w]

    set count [lindex $numCans($n) $column]
    for { set i [llength $L]} { $i < $count } { incr i } {
      frame $w.can$i -width [expr $width-4] -height [expr ($width-4)/2] -bd 0 -bg $color 
      pack $w.can$i -padx 1 -pady 1 -side bottom
    }
  }

  #############################################################################
  #
  # Create the can hopper internal view
  #
  proc ivCanHopper {w n args} {
    variable numCans

    set width 20
    set height 250
    set color red
    set column 0
    parseargs $args {-width -height -color -column}

    frame $w -width $width -height $height -bd 2 -relief solid -bg white

    pack propagate $w 0
    set count [lindex $numCans($n) $column]
    for { set i 0 } { $i < $count } { incr i } {
      frame $w.can$i -width [expr $width-4] -height [expr ($width-4)/2] -bd 0 -bg $color 
      pack $w.can$i -padx 1 -pady 1 -side bottom
    }
  }

  #############################################################################
  #
  # Create the bill reader internal view.
  #
  proc ivBillReader {w n} {
    variable mWidth
    variable mHeight

    #
    # copied from ivChangeHopper
    #
    set ch_w  50
    set cs_x1 [expr $mWidth - 55]
    set cs_x2 [expr $mWidth - 35]
    set x_cent [expr ($cs_x1+$cs_x2)/2]

    set x1 [expr $x_cent - $ch_w/2]
    set x2 [expr $x_cent + $ch_w/2]
    set y1 120
    set y2 150

    $w create text $x_cent $y1 -text "Bill Scanner" -anchor s
    $w create rectangle $x1 $y1 $x2 $y2 -fill white -outline black -width 2
  }

  #############################################################################
  #
  # Create the money box internal view.
  #
  proc ivMoneyBox {w n} {
    variable mWidth
    variable mHeight
    variable useDollar

    #
    # copied from ivChangeHopper
    #
    set ch_w  75
    set cs_x1 [expr $mWidth - 55]
    set cs_x2 [expr $mWidth - 35]
    set x_cent [expr ($cs_x1+$cs_x2)/2]

    set x3 [expr $x_cent - $ch_w/2]
    set x5 [expr $x_cent + $ch_w/2]

    set x1 [expr $x3 - 25]
    set x2 [expr $x3 - 5]
    set x4 [expr $x3 + 20]

    set y1 [expr $mHeight - 340]
    set y2 [expr $y1 + 58]
    set y3 [expr $y2 + 20]
    set y4 [expr $y3 + 25]
    set y5 [expr $mHeight - 142]
    set y6 [expr $mHeight - 117]
    set y7 [expr $mHeight - 5]

    $w create polygon $x3 $y1 $x3 $y3 $x1 $y4 $x1 $y5 $x3 $y6 $x3 $y7 $x5 $y7 $x5 $y6 \
	$x4 $y6 $x2 $y5 $x2 $y4 $x4 $y3 $x5 $y3 $x5 $y1 -width 2 -fill white -outline black

    $w create text [expr $x_cent + 7] $y6 -text Collected -anchor s
    $w create text $x_cent $y1 -text Inserted -anchor s
    $w create text $x_cent 279 -text Change -anchor s

    #
    # Repository for money that has been used to purchase a product
    #
    set x [expr $x3 + 15]
    set y [expr $y6 + 15]

    if {$useDollar($n)} {
      set entries {D holdDollars B holdBogus 25 holdQuarters 10 holdDimes 5 holdNickles}
    } else {
      set entries {25 holdQuarters 10 holdDimes 5 holdNickles}
    }

    foreach {coin aname} $entries {
      if {$coin == "D"} {
	makeDollar $w $x $y
      } elseif {$coin == "B"} {
	makeBogus $w $x $y
      } else {
	makeCoin $w $x $y -value $coin
      }
      $w create text [expr $x + 20] $y -text "X"

      label $w.hld$coin -textvariable CokeMachine::${aname}($n) -bg white
      $w create window [expr $x + 35] $y -window $w.hld$coin

      incr y 21
    }

    #
    # Repository for money that has been inserted but not committed
    #
    set x [expr $x3 + 15]
    set y [expr $y1 + 15]
    foreach {coin aname} {25 insertQuarters 10 insertDimes 5 insertNickles} {
      if {$coin == "D"} {
	makeDollar $w $x $y
      } elseif {$coin == "B"} {
	makeBogus $w $x $y
      } else {
	makeCoin $w $x $y -value $coin
      }
      $w create text [expr $x + 20] $y -text "X"

      label $w.ins$coin -textvariable CokeMachine::${aname}($n) -bg white
      $w create window [expr $x + 35] $y -window $w.ins$coin

      incr y 21
    }
  }

  #############################################################################
  #
  # Create the change hopper internal view
  #
  proc ivChangeHopper {w n} {
    variable mWidth
    variable mHeight

    # coin return slot
    set cs_x1 [expr $mWidth - 55]
    set cs_y1 [expr $mHeight - 155]
    set cs_x2 [expr $mWidth - 35]
    set cs_y2 [expr $mHeight - 135]

    # change box
    set ch_w  75
    set ch_x1 [expr ($cs_x1+$cs_x2)/2 - $ch_w/2]
    set ch_x2 [expr ($cs_x1+$cs_x2)/2 + $ch_w/2]
    set ch_y1 281
    set ch_y2 345

    $w create polygon $cs_x1 $cs_y1 $ch_x1 $ch_y2 $ch_x1 $ch_y1 $ch_x2 $ch_y1 \
	$ch_x2 $ch_y2 $cs_x2 $cs_y1 -width 2 -fill white -outline black

    set x [expr $ch_x1 + 15]
    set y [expr $ch_y1 + 15]

    foreach {coin aname} {25 changeQuarters 10 changeDimes 5 changeNickles} {
      makeCoin $w $x $y -value $coin
      $w create text [expr $x + 20] $y -text "X"

      label $w.chg$coin -textvariable CokeMachine::${aname}($n) -bg white
      $w create window [expr $x + 35] $y -window $w.chg$coin

      incr y 21
    }

    $w create rectangle $cs_x1 $cs_y1 $cs_x2 $cs_y2 -fill gray20
  }

  #############################################################################
  #
  # External view of coke machine
  #
  proc externalView {w n} {
    variable mWidth
    variable mHeight
    variable useDollar

    set color "\#dd0000"

    frame $w -background $color -width $mWidth -height $mHeight

    #
    # "Soda" title on top of machine
    #
    label $w.title -text Soda -font title_f -background $color -foreground white
    place $w.title -x [expr $mWidth/2] -y 60 -anchor center

    #
    # Can out slot
    #
    evCanOutSlot $w.canout
    place $w.canout -x 25 -y [expr $mHeight - 80] -anchor w

    #
    # buttons
    #
    evButtonBox $w.bbar $n
    place $w.bbar -x 20 -y 120 -anchor nw

    #
    # Cost display
    #
    evCostDisplay $w.costwin $n
    place $w.costwin -x 120 -y 120 -anchor nw

    #
    # Dollar slot
    #
    if {$useDollar($n)} {
      evDollarSlot $w.dslot
      place $w.dslot -x [expr $mWidth - 20] -y 120 -anchor ne
    }

    #
    # Coin slot
    #
    evCoinSlot $w.cslot $n
    place $w.cslot -x [expr $mWidth - 20] -y 170 -anchor ne


    #
    # Coin return
    #
    evCoinReturn $w.creturn
    place $w.creturn -x [expr $mWidth - 20] -y [expr $mHeight - 120] -anchor se
  }

  #############################################################################
  #
  # Create the internal view of the coke machine
  #
  proc internalView {w n args} {
    variable mWidth
    variable mHeight
    variable canColors
    variable useDollar

    canvas $w -width $mWidth -height $mHeight -bg tan

    set hwidth 25
    for {set i 0 } { $i < 6 } { incr i } {
      ivCanHopper $w.h$i $n -width $hwidth -color [lindex $canColors $i] -column $i
      $w create window [expr 15 + ($hwidth+5) *$i] 70 -window $w.h$i -anchor nw
    }


    # change holder
    ivChangeHopper $w $n

    # change holder
    ivMoneyBox $w $n

    if {$useDollar($n)} {
      # Bill reader
      ivBillReader $w $n
    }

    # can output slot
    evCanOutSlot $w.co
    $w create window  25 [expr $mHeight - 80] -window $w.co -anchor w
    
  }


  #############################################################################
  #
  # Unpost the coke machine and clean everything up
  #
  proc unpost {n} {
    variable coke_w
    variable useDollar

    set w $coke_w($n)
    unset coke_w($n)

    trace vdelete CokeMachine::isEmpty($n)   w "CokeMachine::seeLight $n"
    trace vdelete CokeMachine::noChange($n)  w "CokeMachine::seeLight $n"
    trace vdelete CokeMachine::billLight($n) w "CokeMachine::seeLight $n"
    if { $useDollar($n)} {
      trace vdelete CokeMachine::scannerBill($n) w "CokeMachine::seeScannerBill $n"
      trace vdelete CokeMachine::readyBill($n) w "CokeMachine::seeReadyBill $n"
    }

    destroy $w
  }

  #############################################################################
  #
  # Post the coke machine window.
  #
  proc post {n args} {
    variable coke_w
    variable isEmpty
    variable mWidth
    variable mHeight
    variable useDollar
    variable displayCost
    variable ENABLED

    puts "CokeMachine::post $n $args"

    set displayCost($n) ""
    set ENABLED($n) 0

    set dollar 1
    parseargs $args {-dollar}
    set useDollar($n) $dollar

    # Initialize state to make sure all variables exist
    initState $n

    set i 0
    set w ""

    set w [VPD::createWindow "Coke Machine $n" -shutdowncommand "CokeMachine::unpost $n"]
    set coke_w($n) $w

    canvas $w.c -width [expr 125 + 2*$mWidth] -height [expr 100 + $mHeight]
    pack $w.c

    externalView $w.c.ext $n
    internalView $w.c.int $n

    $w.c create window 75 25 -window $w.c.ext -anchor nw
    $w.c create window [expr 100 + $mWidth] 25 -window $w.c.int -anchor nw

    button $w.c.reset -text Reset -command "CokeMachine::resetMachine $n 0; CokeMachine::resetMachine $n 1"
    $w.c create window 4 [expr 25 + $mHeight] -window $w.c.reset -anchor sw

    makeCoinList $w.c $n

    trace variable CokeMachine::isEmpty($n)   w "CokeMachine::seeLight $n"
    trace variable CokeMachine::noChange($n)  w "CokeMachine::seeLight $n"
    trace variable CokeMachine::billLight($n) w "CokeMachine::seeLight $n"

    if { $useDollar($n)} {
      trace variable CokeMachine::scannerBill($n) w "CokeMachine::seeScannerBill $n"
      trace variable CokeMachine::readyBill($n) w "CokeMachine::seeReadyBill $n"
    }


    puts "CokeMachine::post-2 $n $args"

    if {[info exists ::tkgate_isInitialized]} {
      VPD::signal $n.PRESS CokeMachine::osigPRESS($n)
      VPD::signal $n.EMPTY CokeMachine::osigEMPTY($n)
      VPD::signal $n.INSQ CokeMachine::osigINSQ($n)
      VPD::signal $n.INSD CokeMachine::osigINSD($n)
      VPD::signal $n.INSN CokeMachine::osigINSN($n)
      VPD::signal $n.NUMQ CokeMachine::osigNUMQ($n)
      VPD::signal $n.NUMD CokeMachine::osigNUMD($n)
      VPD::signal $n.NUMN CokeMachine::osigNUMN($n)
      VPD::signal $n.BILLSNS CokeMachine::osigBILLSNS($n)
      VPD::signal $n.CNRET CokeMachine::osigCNRET($n)
      VPD::signal $n.BILLOK CokeMachine::osigBILLOK($n)
      VPD::signal $n.BILLNG CokeMachine::osigBILLNG($n)
      VPD::signal $n.BILLACK CokeMachine::osigBILLACK($n)

      VPD::insignal $n.NOCHG -variable CokeMachine::noChange($n) -format %d
      VPD::insignal $n.BILLLT -variable CokeMachine::billLight($n) -format %d
    } else {
      set ENABLED($n) 1
    }

    puts "CokeMachine::post-3 $n $args"

    # Initialize state again to react to variable settings
    initState $n

    puts "CokeMachine::post-4 $n $args"
  }
}

#############################################################################
#############################################################################
#
# Code from here down is used for debugging and is only invoked if this
# script is run stand-alone.
#
if {![info exists tkgate_isInitialized]} {
  tk scaling 1
  wm state . withdrawn

  source scripts/misc.tcl
  source scripts/simulator.tcl
  source scripts/udev.tcl
  source scripts/dragger.tcl

  proc updateEmpty {} {
    global emptyLight

    set L {}
    for {set i 0} { $i < 6 } { incr i } {
      lappend L $emptyLight($i)
    }    
    set CokeMachine::isEmpty(test) $L
  }

  proc oDisplay {w vname} {
    frame $w
    label $w.l -text ${vname}:
    label $w.v -textvariable CokeMachine::${vname}(test)
    pack $w.l $w.v -side left
    pack $w -side left -padx 5 -pady 5
  }

  CokeMachine::post test

  set w $CokeMachine::coke_w(test)

  frame $w.sep -bd 2 -relief sunken -height 6
  pack $w.sep -padx 20 -fill x

  frame $w.b
  pack $w.b -fill both
  checkbutton $w.b.nochg -text "No Change" -variable CokeMachine::noChange(test)
  pack $w.b.nochg -side left -padx 5 -pady 5

  if {$CokeMachine::useDollar(test)} {
    checkbutton $w.b.insdol -text "Insert Dollar" -variable CokeMachine::billLight(test)
    pack $w.b.insdol -side left -padx 5 -pady 5
  }

  button $w.b.chg25 -text "Chg 25" -command "CokeMachine::ejectChange test 25"
  button $w.b.chg10 -text "Chg 10" -command "CokeMachine::ejectChange test 10"
  button $w.b.chg5 -text "Chg 5" -command "CokeMachine::ejectChange test 5"
  pack $w.b.chg25 $w.b.chg10 $w.b.chg5 -side left -padx 5 -pady 5

  if {$CokeMachine::useDollar(test)} {
    button $w.b.billin -text "Bill In" -command "CokeMachine::billIn test"
    button $w.b.billout -text "Bill Out" -command "CokeMachine::billOut test"
    pack $w.b.billin $w.b.billout -side left -padx 5 -pady 5
  }

  button $w.b.commit -text "Commit" -command "CokeMachine::commitInserted test"
  pack $w.b.commit -side left -padx 5 -pady 5


  frame $w.b2
  pack $w.b2 -fill both

  for {set i 0} { $i < 6 } { incr i } {
    checkbutton $w.b2.e$i -text "E$i" -variable emptyLight($i) -command updateEmpty
    pack $w.b2.e$i -side left -padx 5 -pady 5
  }
  for {set i 0} { $i < 6 } { incr i } {
    button $w.b2.d$i -text "D$i" -command "CokeMachine::dispense test $i"
    pack $w.b2.d$i -side left -padx 5 -pady 5
  }

  button $w.b2.rins -text "RETINS" -command "CokeMachine::returnInserted test"
  pack $w.b2.rins -padx 5 -pady 5 -side left

  #############################################################################
  #
  # Output display
  #

  frame $w.sep2 -bd 2 -relief sunken -height 6
  pack $w.sep2 -padx 20 -fill x

  frame $w.out1
  pack $w.out1 -fill both
  oDisplay $w.out1.buttons osigPRESS
  oDisplay $w.out1.havecans osigEMPTY
  oDisplay $w.out1.numq osigNUMQ
  oDisplay $w.out1.numd osigNUMD
  oDisplay $w.out1.numn osigNUMN
  frame $w.out2
  pack $w.out2 -fill both
  oDisplay $w.out2.insq osigINSQ
  oDisplay $w.out2.insd osigINSD
  oDisplay $w.out2.indn osigINSN
  if {$CokeMachine::useDollar(test)} {
    oDisplay $w.out2.billsns osigBILLSNS
    oDisplay $w.out2.billok osigBILLOK
  }
  oDisplay $w.out2.cnret osigCNRET
}
