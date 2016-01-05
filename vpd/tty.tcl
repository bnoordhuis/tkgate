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
# Last edit by hansen on Sun Dec  7 20:30:36 2008
#

#cut and paste into tty
#bind $w <Shift-KeyPress-Insert> " if { \[catch {selection get} sel\]==0 } { foreach c \[split \$sel \"\"\] { scan \$c %%c c; Simulator::cmdSend \"command $tkg_ttygat($w) key \$c\" } } "

#
# List of ttys
#

image create bitmap txtcurs -file "$bd/txtcurs.b"

#Security::allow TTY::post
#Security::allow TTY::data

#############################################################################
#
# Register the "safe" tty procedures
#
VPD::register TTY
VPD::allow TTY::post
VPD::allow TTY::data

namespace eval TTY {
  variable tty_w
  variable TD

  proc post {n} {
    variable tty_w
    global simOn

    set w [VPD::createWindow "TTY $n" -shutdowncommand "TTY::unpost $n"]
    set tty_w($n) $w

    text $w.txt -state disabled
    pack $w.txt
    $w.txt image create end -image txtcurs

    bind $w <KeyPress> "TTY::sendChar $n \"%A\""

    if {[info exists ::tkgate_isInitialized]} {
      VPD::outsignal $n.TD TTY::TD($n)
      VPD::insignal $n.RD -command "TTY::data $n" -format %d
      VPD::insignal $n.FORCE -command "TTY::force $n" -format %s
    }
  }

  proc sendChar {n key} {
    variable TD
    if { [string length $key ] == 1 } {
      binary scan $key c c
      set TTY::TD($n) $c
    }
  }

  #
  # Send character c as if it were typed in the keyboard.
  #
  proc force {n c} {
    set L [string length $c] 

    for { set i 0 } { $i < $L } { incr i } {
      TTY::sendChar $n [string range $c $i $i]
    }
  }


  proc unpost {n} {
    variable tty_w

    destroy $tty_w($n)
    unset tty_w($n)
  }

  proc data {n c} {
    variable tty_w

    set w $tty_w($n)

    catch {
      if { $c == 7 } {
	bell
	return
      } elseif { $c == 127 } {
	$w.txt configure -state normal
	$w.txt delete "end - 3 chars"
	$w.txt see end
	$w.txt configure -state disabled
	return
      }

      set x [format %c $c]

      $w.txt configure -state normal
      $w.txt insert "end - 2 chars" $x
      $w.txt see end
      $w.txt configure -state disabled
    }
  }
}


