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
# Last edit by hansen on Sat Jan 17 07:51:39 2009
#
#

######################################################################
#
# Basic constants
#
set tkg_progName 		TKGate 
set tkg_prefFile		"~/.tkgate2-preferences"
set tkg_errorLogFile		"tkgate-error.log"
set tkg_splashWait 		1500
set tkg_pastelHighlight		"\#bbbbff"
set tkg_tabHighlight		"\#ffaaaa"
set tkg_enableCPath		0
set tkg_doBackupOnSave		1
set tkg_wantCheckpoint		0
set tkg_simDefaultDelayFile	"${tkg_gateHome}/gdf/default.gdf"
set treeItemHeight		17
set tkg_standardEncodings	{iso8859-1 iso8859-2 euc-jp utf-8}
set lastKeyPressTime		0

######################################################################
#
# Set the current directory
#
set cwd 			[pwd]/

######################################################################
#
# Initial size of main canvas
#
set tkg_initialWidth 		1024
set tkg_initialHeight 		650
set tkg_minWidth 		830
set tkg_minHeight 		585
set tkg_borderSize		2

######################################################################
#
# Executable for simulator
#
set simExec "${tkg_gateHome}/libexec/verga"

######################################################################
#
# InfoPanel position information
#

# current size for InfoPanel in edit mode
set tkg_mmSplitEdit		0

# current size for InfoPanel in simulate mode
set tkg_mmSplitSimulate		0.15

# Minimum size for InfoPanel when switching to edit mode 
set tkg_mmSplitEditMin		0

# Minimum size for InfoPanel when switching to simulate mode 
set tkg_mmSplitEditLog		0.15

# Minimum size for InfoPanel when logging a message
set tkg_mmSplitSimulateMin	0.15


######################################################################
#
# Basic global variables and initial values
#

# Name of file being edited with user specified (possibly relative) path.
set tkg_currentFile	""

# Full path name of file being edited.
set tkg_currentPath	"/"

# Base name of file to be displayed on status line.
set tkg_displayFile	""

# Non-zero if circuit data has been modified since the last save.
set tkg_modifiedFlag	0

# Name of the module currently being edited.
set tkg_currentModule	"main"

# Text currently displayed on the status line.
set tkg_statusMessage	""

# List of possible numbers of critical paths to search for.
set tkg_cpathListLengths	{1 5 10 25 50 100}

# Name of the current default technology
set tkg_defaultTech	"default"

# Name of the current default wire type
set tkg_defaultWType	"wire"

# List of built-in wire types
set tkg_wtypeList	{wire tri wand triand wor trior tri0 tri1 trireg}

# Current and previous edit tool mode
set mode 1
set old_mode 0

# Currently selected bit width
set tkg_bitWidth 1

# Current default rotation for new gates.
set rot 0

# Non-zero if it is OK to discard edits in the current circuit.
set tkg_discardChanges 0

# Non-zero if extender bars are being used on gates in the current circuit.
set tkg_useExtBars 1

# Indicates the state of the selection buffer
set tkg_cutBufferActive	0
set tkg_selectionActive	0

# Indicates if we are doing a replicate
set tkg_replicateOn 0

# Alternate name for the TkGate home directory.
set TKGATE_HOME	$tkg_gateHome

######################################################################
#
# Constants for file name selectors.
#
set gf_file				""
set gf_result				""
set gf_filter				".v"
set gf_filetypes {
    {{Verilog Netlist}			{.v}	}
    {{All Files}			*	}
}
set gf_vfiletypes {
    {{Verilog Netlist}			{.v}	}
    {{All Files}			*	}
}
set gf_imageFiletypes {
    {{X11 Bitmap}			{.xbm}	}
    {{All Files}			*	}
}

set simScript_filter			".vs"
set simScript_filetypes {
    {{Verilog Script Files}		{.vs}	}
    {{All Files}			*	}
}

set mem_filter				".mem"
set mem_filetypes {
    {{Gate Memory Image Files}		{.mem}	}
    {{All Files}			*	}
}

set gdf_filter				".gdf"
set gdf_filetypes {
    {{Gate Delay File}			{.gdf}	}
    {{All Files}			*	}
}


