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
# Last edit by hansen on Sat Feb 21 22:53:41 2009
#

######################################################################
#
# Option Definitions
#
# This file defines all of options and their default values.  Note that
# the default values here, can be overriddeng in the site-preferences
# file.
#

######################################################################
#
# General settings
#
Option::new siteName		Earth	-optclass /General
Option::new userName    	""	-optclass /General
Option::new doSafeSave		1	-optclass /General
Option::new saveWithCells	0	-optclass /General
Option::new checkpointEnabled	0	-optclass /General
Option::new checkpointFreq	300	-optclass /General -minvalue 1 -maxvalue 9999


######################################################################
#
# Special settings
#
Option::new enableBeakModes	0	-optclass Special
Option::new baderMode		0	-optclass Special
Option::new trekMode		0	-optclass Special
Option::new batMode		0	-optclass Special
Option::new allowDebug		0	-optclass Special
Option::new batRate		200	-optclass Special
Option::new fakeDoubleClicks	1	-optclass Special
Option::new fakeDCInterval	300	-optclass Special
Option::new checkVersion	0	-optclass Special


######################################################################
#
# Do continuous verification
#
Option::new simDebugInterface	0	-optclass /Debug
Option::new contVerify		0	-optclass /Debug

######################################################################
#
# Obsolete settings 
#
Option::new simInitScript	""	-optclass Obsolete
Option::new simCustomDelay	0	-optclass Obsolete
Option::new simLogFile          ""      -optclass Obsolete

######################################################################
#
# Interface settings
#
Option::new keyBindings		emacs	-optclass /Interface
Option::new smoothScroll	1	-optclass /Interface
Option::new regionUpdate	1	-optclass /Interface
Option::new showDialogImage	1	-optclass /Interface
Option::new fontSet		normal	-optclass /Interface
Option::new blockDropConnect	0	-optclass /Interface
Option::new undoLength		50	-optclass /Interface -minvalue 1 -maxvalue 999
Option::new helpBalloons	1	-optclass /Interface
Option::new tearawayMenus	0	-optclass /Interface


######################################################################
#
# Circuit default options
#
Option::new extendBars		1	-optclass /Circuit
Option::new showSwitchNets	1	-optclass /Circuit

######################################################################
#
# Toolbar options
#
Option::new filetb		1		-optclass /Toolbars/Edit
Option::new edittb		1		-optclass /Toolbars/Edit
Option::new gateoptb		1		-optclass /Toolbars/Edit
Option::new gateprop		1		-optclass /Toolbars/Edit
Option::new undotb		1		-optclass /Toolbars/Edit
Option::new zoomtb		1		-optclass /Toolbars/Edit
Option::new modetb		1		-optclass /Toolbars/Edit
Option::new moduletb		1		-optclass /Toolbars/Edit
Option::new techtb		1		-optclass /Toolbars/Edit
Option::new zoomtb		1		-optclass /Toolbars/Edit

Option::new simctltb		1		-optclass /Toolbars/Simulation
Option::new simauxtb		1		-optclass /Toolbars/Simulation
Option::new simmodestb		1		-optclass /Toolbars/Simulation

Option::new symporttb		1		-optclass /Toolbars/Symboledit
Option::new symshifttb		1		-optclass /Toolbars/Symboledit
Option::new symoprtb		1		-optclass /Toolbars/Symboledit
Option::new symmodestb		1		-optclass /Toolbars/Symboledit
Option::new symedittb		1		-optclass /Toolbars/Symboledit

######################################################################
#
# HDL text colors/fontification
#
Option::new hdlDragSel		1		-optclass /HDL
Option::new hdlIndenting	1		-optclass /HDL
Option::new hdlIndentLevel	2		-optclass /HDL -minvalue 0 -maxvalue 16
Option::new hdlIndentNoBegin	0		-optclass /HDL
Option::new hdlIndentReturn	1		-optclass /HDL

Option::new hdlColorize		1		-optclass /Color/HDL
Option::new hdlCommentColor	"firebrick"	-optclass /Color/HDL
Option::new hdlCommentBold	0		-optclass /Color/HDL
Option::new hdlCommentItalic	1		-optclass /Color/HDL
Option::new hdlStringColor	"RosyBrown"	-optclass /Color/HDL
Option::new hdlStringBold	0		-optclass /Color/HDL
Option::new hdlStringItalic	0		-optclass /Color/HDL
Option::new hdlReservedColor	"magenta4"	-optclass /Color/HDL
Option::new hdlReservedBold	1		-optclass /Color/HDL
Option::new hdlReservedItalic	0		-optclass /Color/HDL
Option::new hdlGateWordColor	"red"		-optclass /Color/HDL
Option::new hdlGateWordBold	1		-optclass /Color/HDL
Option::new hdlGateWordItalic	0		-optclass /Color/HDL
Option::new hdlInstWordColor	"black"		-optclass /Color/HDL
Option::new hdlInstWordBold	1		-optclass /Color/HDL
Option::new hdlInstWordItalic	0		-optclass /Color/HDL
Option::new hdlTaskWordColor	"green4"	-optclass /Color/HDL
Option::new hdlTaskWordBold	1		-optclass /Color/HDL
Option::new hdlTaskWordItalic	0		-optclass /Color/HDL

######################################################################
#
# Simulator settings
#
Option::new simStepSize		1	-optclass /Simulate/Basic -minvalue 1 -maxvalue 1000000
Option::new valuePopUpDelay	333	-optclass /Simulate/Basic -minvalue 1 -maxvalue 10000
Option::new simClockStepSize	1	-optclass /Simulate/Basic -minvalue 1 -maxvalue 1000000
Option::new simClockOverStep	21	-optclass /Simulate/Basic -minvalue 0 -maxvalue 1000000
Option::new simSortTraces	1	-optclass /Simulate/Basic
Option::new saveTraces		1	-optclass /Simulate/Basic
Option::new glitchSuppress	1	-optclass /Simulate/Basic

Option::new simDelayFile	""	-optclass /Simulate/Technology
Option::new warningMode		3	-optclass /Simulate/Error -minvalue 1 -maxvalue 4

######################################################################
#
# Library Options
#
Option::new simVPDPath		"\${TKGATE_HOME}/vpd"		-optclass /Library/VPD
Option::new simVLibPath		"\${TKGATE_HOME}/vlib"		-optclass /Library/Path
Option::new simVLibNames	""				-optclass /Library/Load

######################################################################
#
# Security Options
#
Option::new securityHandling	1	-optclass /Security
Option::new securityExec	1	-optclass /Security
Option::new securityWriteMem	0	-optclass /Security
Option::new securityOpen	0	-optclass /Security
Option::new securitySend	0	-optclass /Security
Option::new securityEnqueue	1	-optclass /Security


######################################################################
#
# HTML Options
#
Option::new browserCommand	"firefox %s"			-optclass /HTML
Option::new emailCommand	"thunderbird %s"		-optclass /HTML

######################################################################
#
# Critical Path Options
#
Option::new flashCPath		1
Option::new numCPath		10

######################################################################
#
# Default colors
#
Option::new instColor          "blue"		-optclass /Color/Edit
Option::new moduleColor        "magenta4"	-optclass /Color/Edit
Option::new modulePortColor    "cyan4"		-optclass /Color/Edit
Option::new frameColor         "black"		-optclass /Color/Edit
Option::new commentColor       "black"		-optclass /Color/Edit
Option::new hyperlinkColor     "blue3"		-optclass /Color/Edit
Option::new wireColor          "green4"		-optclass /Color/Edit
Option::new busColor           "red"		-optclass /Color/Edit
Option::new toolColor          "black"		-optclass /Color/Edit
Option::new cpathColor         "red"		-optclass /Color/Edit
Option::new offLedColor	       "lavender"	-optclass /Color/Edit
Option::new onLedColor         "red"		-optclass /Color/Edit
Option::new zLedColor          "yellow"		-optclass /Color/Edit
Option::new backgroundColor    "white"		-optclass /Color/Edit

Option::new gridColor          "black"		-optclass /Color/Scope
Option::new oneColor           "green4"		-optclass /Color/Scope
Option::new zeroColor          "magenta"	-optclass /Color/Scope
Option::new floatColor         "blue"		-optclass /Color/Scope
Option::new unknownColor       "red"		-optclass /Color/Scope

######################################################################
#
# Print options
#
Option::new printer		default			-optclass /Print/Output
Option::new printDefCmdTmp	"lpr"			-optclass /Print/Output
Option::new printCmdTmp		"lpr -P\${PRINTER}"	-optclass /Print/Output
Option::new printCommands	{}			-optclass /Print/Output
Option::new printPgSize(US)	Letter			-optclass /Print/Output
Option::new printPgSize(*)	A4			-optclass /Print/Output
Option::new printPgOrient	landscape		-optclass /Print/Output
Option::new printDuplex		0			-optclass /Print/Output
Option::new printStyle		fit			-optclass /Print/Content
Option::new printIndex		0			-optclass /Print/Content
Option::new printGraph		0			-optclass /Print/Content
Option::new printMergeSmall	1			-optclass /Print/Content

