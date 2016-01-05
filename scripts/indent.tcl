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
# Last edit by hansen on Mon Mar 21 10:31:49 2005


#############################################################################
#
# This class is used to perform code sensitive indenting on verilog text
# in the HDL editor.
#
# The following pairs increase the indent level between then:
#
#     module ... endmodule
#     begin ... end
#     case? ... endcase
#     fork ... join
#     ( ... )
#     
# "else" will be set 1 level less than previous line.
#
# optional indent following:
#     initial 
#     always 
#
# optional indent of "begin ... end" when following partial statement.
#
namespace eval Indent {
  #############################################################################
  #
  # Return the indentation level for the specified line.
  #
  proc indentLevel {lnum} {
    if {$lnum <= 1  } { return 0 }
    
  }

  #############################################################################
  #
  # Run the indenter on $w from line $start to line $stop.
  #
  proc run {w start stop} {
#    if {$lnum <= 1  } { return 0 }
  }
}
