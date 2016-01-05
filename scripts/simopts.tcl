#   Copyright (C) 1987-2006 by Jeffery P. Hansen
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
# Last edit by hansen on Thu Jan  8 23:40:23 2009

namespace eval SimOptions {
  proc create {w} {
    frame $w
    packPad $w

    labelframe $w.t [m opt.simbase]
    frame $w.t.pad -height [rescale *10]

    label $w.t.simstepL -text "[m opt.estepsz]:"
    entry $w.t.simstepE -width 5 -textvariable tkg_simStepSize -bg white

    label $w.t.clkstepL -text "[m opt.cstepsz]:"
    entry $w.t.clkstepE -width 5 -textvariable tkg_simClockStepSize -bg white

    label $w.t.clkostepL -text "[m opt.overstep]:"
    entry $w.t.clkostepE -width 5 -textvariable tkg_simClockOverStep -bg white

    grid $w.t.pad       -row 0 -column 0
    grid $w.t.simstepL	-row 1 -column 0 -sticky e -padx 5 -pady 5
    grid $w.t.simstepE	-row 1 -column 1 -sticky w -padx 5 -pady 5
    grid $w.t.clkostepL	-row 1 -column 3 -sticky e -padx 5 -pady 5
    grid $w.t.clkostepE	-row 1 -column 4 -sticky w -padx 5 -pady 5 
    grid $w.t.clkstepL	-row 2 -column 3 -sticky e -padx 5 -pady 5
    grid $w.t.clkstepE	-row 2 -column 4 -sticky w -padx 5 -pady 5 

    pack $w.t -side left -padx 5 -pady 5 -anchor nw

  }
}

