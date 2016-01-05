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
# Last edit by hansen on Sun Jan 18 15:25:08 2009
#

#set imtest 0
#catch {
#  set imtest [tk useinputmethods 1]
#}

set tinyFontSet {
  {dipFont		Couier		6	normal	roman}
  {dialogCapFont	Helvetica	8	bold	roman}
  {dialogExpFont	Helvetica	6	normal	roman}
  {dialogBigExpFont	Helvetica	6	bold	roman}
  {labelFont	 	Helvetica	6	normal	roman}
  {boldLabelFont	Helvetica	6	bold	roman}
  {smallLabelFont	Helvetica	6	normal	roman}
  {buttonFont		Helvetica	6	normal	roman}
  {menuFont		Helvetica	6	normal	roman}
  {entryFont		Helvetica	6	normal	roman}
  {menuButtonFont	Helvetica	6	normal	roman}
  {checklistFont	Helvetica	6	normal	roman}
  {checklistBoldFont	Helvetica	6	bold	roman}
  {hdlFont		Courier		6	normal	roman}
  {hdlItalicFont	Courier		6	normal	italic}
  {hdlBoldFont		Courier		6	bold	roman}
  {hdlBoldItalicFont	Courier		6	bold	italic}
  {Tree:font		Helvetica	6	normal	roman}
  {Tree:actfont		Helvetica	6	bold	roman}
  {Tree:act2font	Helvetica	6	bold	roman 1}
}

set smallFontSet {
  {dipFont		Couier		8	normal	roman}
  {dialogCapFont	Helvetica	10	bold	roman}
  {dialogExpFont	Helvetica	8	normal	roman}
  {dialogBigExpFont	Helvetica	8	bold	roman}
  {labelFont	 	Helvetica	8	normal	roman}
  {boldLabelFont	Helvetica	8	bold	roman}
  {smallLabelFont	Helvetica	8	normal	roman}
  {buttonFont		Helvetica	8	normal	roman}
  {menuFont		Helvetica	8	normal	roman}
  {entryFont		Helvetica	8	normal	roman}
  {menuButtonFont	Helvetica	8	normal	roman}
  {checklistFont	Helvetica	8	normal	roman}
  {checklistBoldFont	Helvetica	8	bold	roman}
  {hdlFont		Courier		8	normal	roman}
  {hdlItalicFont	Courier		8	normal	italic}
  {hdlBoldFont		Courier		8	bold	roman}
  {hdlBoldItalicFont	Courier		8	bold	italic}
  {Tree:font		Helvetica	8	normal	roman}
  {Tree:actfont		Helvetica	8	bold	roman}
  {Tree:act2font	Helvetica	8	bold	roman 1}
}

set normalFontSet {
  {dipFont		Couier		10	normal	roman}
  {dialogCapFont	Helvetica	12	bold	roman}
  {dialogExpFont	Helvetica	10	normal	roman}
  {dialogBigExpFont	Helvetica	10	bold	roman}
  {labelFont	 	Helvetica	10	normal	roman}
  {boldLabelFont	Helvetica	10	bold	roman}
  {smallLabelFont	Helvetica	10	normal	roman}
  {buttonFont		Helvetica	10	normal	roman}
  {menuFont		Helvetica	10	normal	roman}
  {entryFont		Helvetica	10	normal	roman}
  {menuButtonFont	Helvetica	10	normal	roman}
  {checklistFont	Helvetica	10	normal	roman}
  {checklistBoldFont	Helvetica	10	bold	roman}
  {hdlFont		Courier		10	normal	roman}
  {hdlItalicFont	Courier		10	normal	italic}
  {hdlBoldFont		Courier		10	bold	roman}
  {hdlBoldItalicFont	Courier		10	bold	italic}
  {Tree:font		Helvetica	10	normal	roman}
  {Tree:actfont		Helvetica	10	bold	roman}
  {Tree:act2font	Helvetica	10	bold	roman 1}
}

set largeFontSet {
  {dipFont		Couier		14	normal	roman}
  {dialogCapFont	Helvetica	16	bold	roman}
  {dialogExpFont	Helvetica	14	normal	roman}
  {dialogBigExpFont	Helvetica	14	bold	roman}
  {labelFont	 	Helvetica	14	normal	roman}
  {boldLabelFont	Helvetica	14	bold	roman}
  {smallLabelFont	Helvetica	14	normal	roman}
  {buttonFont		Helvetica	14	normal	roman}
  {menuFont		Helvetica	14	normal	roman}
  {entryFont		Helvetica	14	normal	roman}
  {menuButtonFont	Helvetica	14	normal	roman}
  {checklistFont	Helvetica	14	normal	roman}
  {checklistBoldFont	Helvetica	14	bold	roman}
  {hdlFont		Courier		14	normal	roman}
  {hdlItalicFont	Courier		14	normal	italic}
  {hdlBoldFont		Courier		14	bold	roman}
  {hdlBoldItalicFont	Courier		14	bold	italic}
  {Tree:font		Helvetica	14	normal	roman}
  {Tree:actfont		Helvetica	14	bold	roman}
  {Tree:act2font	Helvetica	14	bold	roman 1}
}

set hugeFontSet {
  {dipFont		Couier		20	normal	roman}
  {dialogCapFont	Helvetica	24	bold	roman}
  {dialogExpFont	Helvetica	20	normal	roman}
  {dialogBigExpFont	Helvetica	20	bold	roman}
  {labelFont	 	Helvetica	20	normal	roman}
  {boldLabelFont	Helvetica	20	bold	roman}
  {smallLabelFont	Helvetica	20	normal	roman}
  {buttonFont		Helvetica	20	normal	roman}
  {menuFont		Helvetica	20	normal	roman}
  {entryFont		Helvetica	20	normal	roman}
  {menuButtonFont	Helvetica	20	normal	roman}
  {checklistFont	Helvetica	20	normal	roman}
  {checklistBoldFont	Helvetica	20	bold	roman}
  {hdlFont		Courier		20	normal	roman}
  {hdlItalicFont	Courier		20	normal	italic}
  {hdlBoldFont		Courier		20	bold	roman}
  {hdlBoldItalicFont	Courier		20	bold	italic}
  {Tree:font		Helvetica	20	normal	roman}
  {Tree:actfont		Helvetica	20	bold	roman}
  {Tree:act2font	Helvetica	20	bold	roman 1}
}

#############################################################################
#
# Create all needed fonts from the font list
#
proc createFontSet {fontList} {
  foreach fontSpec $fontList {
    set name [lindex $fontSpec 0]
    set family [lindex $fontSpec 1]
    set size [lindex $fontSpec 2]
    set weight [lindex $fontSpec 3]
    set slant [lindex $fontSpec 4]
    set overstrike 0
    if { [llength $fontSpec] > 5} {
      set overstrike [lindex $fontSpec 5]
    }
    font create $name -family $family -size $size -weight $weight -slant $slant -overstrike $overstrike
  }
}

#############################################################################
#
# Set the font size for the application. Size is one of "small", "normal",
# "large" or "huge"
#
proc setFontSet {size} {
  global tkg_initialWidth tkg_initialHeight
  global treeItemHeight
  global tinyFontSet smallFontSet normalFontSet largeFontSet hugeFontSet

  switch $size {
    tiny {
      set treeItemHeight 13
      createFontSet $tinyFontSet
    }
    small {
      set treeItemHeight 15
      createFontSet $smallFontSet
    }
    normal {
      set treeItemHeight 17
      createFontSet $normalFontSet
    }
    large {
      set treeItemHeight 20
      createFontSet $largeFontSet
    }
    huge {
      set tkg_initialWidth	1400
      set tkg_initialHeight 	850
      set treeItemHeight 22
      createFontSet $hugeFontSet
    }
  }
}

proc setupOptions {} {
  global tkg_tearawayMenus

  #
  # I'm not sure how to figure out what the default properties for widgets
  # are, so for now we will just create something to get its properites
  # then destroy it later.  We need to use these default properties for
  # our home-grown wigets, but since the actual property values may depend
  # on the window manager and theme options, we want to figure out what
  # these defaults are so we can blend in with the theme.
  #
  menubutton .bogus_mb

  set activeBackground [.bogus_mb cget -activebackground]
  set activeForeground [.bogus_mb cget -activeforeground]
  set unselectedBackground [.bogus_mb cget -background]
  set foreground [.bogus_mb cget -foreground]

  if { $activeBackground == "" } {
    set activeBackground "\#dcdcff"
  }
  if { $activeForeground == "" } {
    set activeForeground black
  }
  if { $unselectedBackground == "" } {
    set unselectedBackground gray70
  }
  if { $foreground == "" } {
    set foreground black
  }

  option add *highlightColor			blue
  option add *font				labelFont
  option add *exportselection			0
  option add *activeBackground			$activeBackground
  option add *activeForeground			$activeForeground
  option add *unselectedBackground		$unselectedBackground
  option add *borderWidth			1
#  option add *background $unselectedBackground
#  option add *foreground $foreground

  option add *FlatRadioButton*activeBackground	$unselectedBackground
  option add *FlatRadioButton*background	$unselectedBackground

  option add *SpreadSheet*headerColor		"\#fff0f0"
  option add *SpreadSheet*entryColor		"\#f0f0ff"
  option add *SpreadSheet*selectColor		"\#d0ffd0"
  option add *SpreadSheet*grabColor		"\#80ff80"
  option add *SpreadSheet*selectMode		multiple

  option add *TabBox*foreground			$foreground
  option add *TabBox*background			$unselectedBackground
  option add *TabBox*unselectedForeground	black
  option add *TabBox*unselectedBackground	gray60
  option add *TabBox*disabledForeground		gray30
  option add *TabBox*disabledBackground		gray60


  option add *TkgDialog*borderWidth 1
  option add *TkgDialog*relief raised
  option add *Entry*background white

  option add *LabelFrame*borderWidth 2
  option add *LabelFrame*relief groove

  option add *Listbox*takeFocus 0
  option add *Scrollbar*takeFocus 0
  option add *Scrollbar*width 10
  option add *Label*font labelFont
  option add *Checkbutton*font labelFont
  option add *Radiobutton*font labelFont
  option add *Button*font buttonFont
  option add *Menu*font menuFont
  option add *Popup*font menuFont
  option add *Menubutton*font menuFont
  option add *Entry*font entryFont
  option add *Listbox*font entryFont
  option add *Text*font hdlFont
  option add *Listbox*background white
  option add *SpreadSheet*selectmode shift-multiple

  option add *Menu*tearOff $tkg_tearawayMenus

  #option add *Checkbutton*image [gifI checkoff.gif]
  #option add *Checkbutton*selectImage [gifI checkon.gif]
  #option add *Checkbutton*compound left
  #option add *Checkbutton*indicatorOn 0
  #option add *Checkbutton*relief flat
  #option add *Checkbutton*boarderwidth 0

  destroy .bogus_mb

  #
  # Disable normal handling of Tab and BackTab
  #
  #bind all <Tab> ""
  #bind all <<PrevWindow>> ""
}
