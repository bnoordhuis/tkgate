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
# Last edit by hansen on Thu May 12 10:58:38 2005
#

#############################################################################
#
# Drag-and-drop functions
#
namespace eval Dragger {
  variable drag_w
  variable W
  variable H
  variable command
  variable absolute
  variable grabpoint
  variable anim_speed 50
  variable anim_step 20

  proc setGeometry {X Y} {
    variable drag_w
    variable W
    variable H
    variable grabpoint

    if { [llength $grabpoint] < 2 } {
      wm geometry $drag_w +[expr ${X} - $W/2]+[expr ${Y} - 3*$H/4]
    } else {
      set dx [lindex $grabpoint 0]
      set dy [lindex $grabpoint 1]
      wm geometry $drag_w +[expr ${X} - $dx]+[expr ${Y} - $dy]
    }
  }


  proc windowConfigure {} {
    variable drag_w
    variable W
    variable H

    scan [winfo pointerxy .] "%d %d" X Y

    set W [winfo width $drag_w]
    set H [winfo height $drag_w]

    updatePosition $X $Y
  }

  proc updatePosition {X Y} {
    setGeometry $X $Y
    update
  }

  proc drop {x y} {
    variable drag_w
    variable command
    variable absolute

    set w $drag_w

    destroy $w
    if { $command != "" } {
      update
      set w [winfo containing $x $y]

      if { $w == "" } {
	eval "$command \"\" $x $y"
      } else {
	if { ! $absolute } {
	  set x [expr $x - [winfo rootx $w]]
	  set y [expr $y - [winfo rooty $w]]
	}
	eval "$command $w $x $y"
      }
    }
  }

  #############################################################################
  #
  # Animate a drop window along a path.  There are two forms of this command:
  #
  # animate x1 y1 x2 y2 ...				We are on a vertix
  # animate -step x y cos sin   x1 y1 x2 y2 ...		We are between verticies
  # animate -offset ox oy    x1 y1 x2 y2 ...            Apply offset to all elements
  #
  proc animate {args} {
    variable drag_w
    variable anim_step
    variable anim_speed

    set step 20
    set speed $anim_speed

    if {[lindex $args 0] == "-speed"} {
      set anim_speed [lindex $args 1]
    } elseif {[lindex $args 0] == "-step"} {
      set anim_step [lindex $args 1]
    } elseif {[lindex $args 0] == "-offset"} {
      set ox    [lindex $args 1]
      set oy    [lindex $args 2]
      set args [lrange $args 3 end]
      set L {}
      foreach {x y} $args {
	lappend L [expr $ox+$x] [expr $oy+$y]
      }
      set args $L
      eval "Dragger::animate $L"
    } elseif {[lindex $args 0] == "-next"} {
      set x    [lindex $args 1]
      set y    [lindex $args 2]
      set cosr [lindex $args 3]
      set sinr [lindex $args 4]
      set args [lrange $args 5 end]
      set x2   [lindex $args 0]
      set y2   [lindex $args 1]

      set x [expr $x + $cosr*$step]
      set y [expr $y + $sinr*$step]

      updatePosition [expr int($x)] [expr int($y)]
      set r    [expr sqrt(($x-$x2)*($x-$x2) + ($y-$y2)*($y-$y2))]

      if {$r < [expr $step + 1] } {
	after $speed "Dragger::animate $args"
      } else {
	after $speed "Dragger::animate -next $x $y $cosr $sinr $args"
      }
    } else {
      set x [lindex $args 0]
      set y [lindex $args 1]
      set args [lrange $args 2 end]
      updatePosition [expr int($x)] [expr int($y)]

      if {[lindex $args 0] == "-jump"} {
	set args [lrange $args 1 end]
	after $speed "Dragger::animate $args"
      } elseif {[llength $args] > 1 } {
	set x2 [lindex $args 0]
	set y2 [lindex $args 1]

	set r    [expr sqrt(($x-$x2)*($x-$x2) + ($y-$y2)*($y-$y2))]
	if { $r < 6} {
	  after $speed "Dragger::animate $args"
	} else {
	  set cosr [expr ($x2-$x)/$r]
	  set sinr [expr ($y2-$y)/$r]
	  after $speed "Dragger::animate -next $x $y $cosr $sinr $args"
	}
      } else {
	Dragger::drop $x $y
      }
    }
  }

  proc make {args} {
    variable drag_w
    variable W
    variable H
    variable command
    variable absolute
    variable grabpoint

    set command ""
    set absolute 0
    set grabpoint 0
    set bd 2
    set relief solid
    set interactive 1
    parseargs $args {-command -absolute -grabpoint -bd -relief -interactive}

    set W 10
    set H 10

    set drag_w .dragger
    set w $drag_w
    if {[catch { toplevel $w -cursor arrow -bd $bd -relief $relief } xx]} {
      return ""
    }

    scan [winfo pointerxy .] "%d %d" X Y

    setGeometry $X $Y
    wm overrideredirect $w 1

    if {$interactive} {
      bind $w <ButtonRelease-1> "Dragger::drop %X %Y"
      bind $w <B1-Motion> { Dragger::updatePosition %X %Y}
      bind $w <Configure> { Dragger::windowConfigure }
    }

    update

    if {$interactive} {
      if {[catch { grab -global $w } xx]} {
	destroy $w
	return ""
      }
    }

    return $w
  }
}
