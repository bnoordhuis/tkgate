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
# Last edit by hansen on Thu May  9 16:58:53 2002
#

set debug_nameframe 0

proc nameframe {w name} {
  canvas $w -width 100 -height 100
  frame $w.f -bd 2 -relief groove
  label $w.l -text $name
  frame $w.f.pad -height 7
  pack $w.f.pad

  $w create window 7 7 -window $w.l -anchor w

  $w create window 2 7 -window $w.f -anchor nw
  bind $w.f <Configure> "update; $w configure -width \[expr \[winfo reqwidth $w.f\] + 2\] -height \[expr \[winfo reqheight $w.f\] + 7\]"
}

if {$debug_nameframe} {
  nameframe .w "Floop"
  pack .w -padx 5 -pady 5

  button .w.f.b -text splat -command { 
    button .w.f.b2 -text boom -command {
      puts "config [winfo reqwidth .w.f] [winfo reqheight .w.f]"
    }
    pack .w.f.b2
  }

  pack .w.f.b -padx 10 -pady 10
}

