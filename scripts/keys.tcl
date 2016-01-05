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
# Last edit by hansen on Thu Jan 29 22:35:38 2009
#
#     endPrefix                 End a keyboard shortcut prefix
#     startPrefix               Start a keyboard shortcut prefix
#     tkg_setBindings           Set bindings for a window
#     newPrefix			Declare a prefix character
#     keyBinding                Define a key binding (redefine if existing)
#     newBinding		Define a key binding (error if existing)
#     getCount			Get cummulative count of key presses
#

#
# Note on bindtags usage - In tcl/tk bindings for multi key sequences
# are executed regardless of overlap with other bindings.  For example,
# if "Ctl-X Ctl-S" and "Ctl-S g" are commands, then the key sequence
# "Ctl-X Ctl-S g" will cause both commands to be executed.  To avoid this
# confusing behavior, a special nop command key is used.  After the
# end of a two-key sequence, a special key not used in any commands is
# forced into the event stream to break up any unexpected commands. 
#
# Another problem is when "g" is a command, but "Ctl-X g" is not a
# command.  Since "Ctl-X" is a command prefix, if the next character
# does not form a valid mult-charcter command, it should be ignored.
# We implement this using bindtags on windows.  Single charcter commands
# are bound to the tag "keywin", and multi-character commands are bound
# to the tag "key2win".  These tags are attached to all windows where
# which process commands.  When a prefix character is entered, the
# "keywin" tag is temporarily removed from all windows, and restored after
# a second character has been entered. 
#

array set keysymTable {
  space              "Space"
  exclam             "!"
  quotedbl           "\""
  numbersign         "\#"
  dollar             "\$"
  percent            "%"
  ampersand          "&"
  apostrophe         "'"
  parenleft          "("
  parenright         ")"
  asterisk           "*"
  plus               "+"
  comma              ","
  minus              "-"
  period             "."
  slash              "/"
  colon              ":"
  semicolon          ";"
  less               "<"
  equal              "="
  greater            ">"
  question           "?"
  at                 "@"
  bracketleft        "\["
  backslash          "\\"
  bracketright       "\]"
  asciicircum        "^"
  underscore         "_"
  grave              "`"
  braceleft          "{"
  bar                "|"
  braceright         "}"
  asciitilde         "~"
  Delete             "Del"
  Tab                "Tab"
  ISO_Left_Tab       "BackTab"
}




namespace eval KeyBinding {
  variable wgroups
  variable unmatchedHandlers
  variable lastKey ""
  variable cmdTable
  variable permTable
  variable shiftKeys { Shift_L Shift_R Control_L Control_R Alt_L Alt_R }
  variable prefixes

  #
  # See a key press
  #
  proc seeKey {w state key chr} {
    variable keyCount
    variable shiftKeys
    variable lastKey
    variable cmdTable
    variable wgroups
    variable unmatchedHandlers
    variable prefixes
    variable keyCount

    #
    # Ignore Alt key combinations because the built-in tcl/tk menu stuff deals with that.
    #
    if { [expr $state & 8 ] != 0 } { return }

    if { [lsearch -exact $shiftKeys $key] >= 0 } { return }

    set modifiers ""
    if { [expr $state & 8 ] != 0 } { set modifiers "Alt-$modifiers" }
    if { [expr $state & 4 ] != 0 } { set modifiers "Control-$modifiers" }

    set key "<$modifiers$key>"

    catch { incr keyCount($w) }

    if { $key == "<Control-g>" } {
      set lastKey ""
      bell	
    }

    set keySeq $lastKey$key

    set g $wgroups($w)
    if {![catch {set cmd $cmdTable(${g}:$keySeq)}]} {
      eval $cmd
      set lastKey ""
    } else {
      if { $lastKey == "" } {
	if {[lsearch $prefixes($g) $key] >= 0} {
	  set lastKey $key
	} else {
	  if { $unmatchedHandlers($w) != "" } {
	    $unmatchedHandlers($w) $chr
	  }
	}
      } else {
	if { $unmatchedHandlers($w) != "" } {
	  $unmatchedHandlers($w) $chr
	}
	set lastKey ""
      }
    }
  }

  #############################################################################
  #
  # Translate a key specifier to a user friendly key specifier that can be 
  # displayed in a menu.
  #
  proc beautifyKey {key} {
    global keysymTable

    set prefix ""

    if {[regexp "<KeyPress-(.+)>" $key X KP] > 0} { 
      set key $KP
    } elseif {[regexp "<Control-(.+)>" $key X KP] > 0} { 
      set prefix "Ctl-"
      set key $KP
    } elseif {[regexp "<Alt-(.+)>" $key X KP] > 0} { 
      set prefix "Alt-"
      set key $KP
    } elseif {[regexp "<(.+)>" $key X KP] > 0} { 
      set key $KP
    }

    if {[info exists keysymTable($key)]} {
      set key $keysymTable($key)
    }

    return $prefix$key
  }

  #############################################################################
  #
  # Beautify a key sequence for display in user menus as shortcuts.
  #
  proc beautify {keyseq} {

    if {[regexp "(.|<\[^>\]+>)(.|<\[^>\]+>)?" $keyseq X key1 key2] <= 0 } {
      error "Illegal key definition."
    }

    if { $key2 == "" } {
      return [beautifyKey $key1]
    } else {
      return "[beautifyKey $key1] [beautifyKey $key2]"
    }
  }

  #############################################################################
  #
  # Make window w a key command listener.
  #
  # Parameters:
  #      w		Window that should listen for key presses.
  #      group		Group that this listener belongs to.
  #
  # Switches:
  #      -unmatchedcommand {}		command to execute when an unhandled key is pressed.
  #     
  #
  proc listener {w group args} {
    variable wgroups
    variable unmatchedHandlers
    variable keyCount

#    puts "listener w=\"$w\" group=\"$group\" args=\"$args\""

    set keyCount($w) 0

    set unmatchedcommand ""
    parseargs $args {-unmatchedcommand}

    set unmatchedHandlers($w) $unmatchedcommand
    set wgroups($w) $group


    bind $w <KeyPress> "KeyBinding::seeKey %W %s %K %A"
  }

  #############################################################################
  #
  # Get total number of key presses in a window registered as a listener.
  #
  # Parameters:
  #      w		Window to check for key count.
  #
  proc getKeyCount {w} {
    variable keyCount

    set n 0
    catch { set n $keyCount($w) }
    return $n
  }

  #############################################################################
  #
  # Clear all key command tables.
  #
  proc clearAll {} {
    variable cmdTable
    variable permTable
    variable prefixes

    array unset cmdTable
    array set cmdTable [array get permTable]
    array unset prefixes
  }

  #############################################################################
  #
  # Create a new key binding
  #
  proc new {keyseq cmd args} {
    variable cmdTable
    variable permTable
    variable prefixes
    global menuCommandTable

    set perm 0
    set groups {main}
    parseargs $args {-groups -perm}

    if {[regexp "(.|<\[^>\]+>)(.|<\[^>\]+>)?" $keyseq X key1 key2] <= 0 } {
      error "Illegal key definition."
    }

    if {[regexp "<KeyPress-(.)>" $key1 X KP] > 0} { set key1 $KP }
    if {[regexp "<KeyPress-(.)>" $key2 X KP] > 0} { set key2 $KP }

    if {[string length $key1] == 1 } { set key1 "<$key1>" }
    if {[string length $key2] == 1 } { set key2 "<$key2>" }

    
    set keyseq $key1$key2

    foreach g $groups {
      if {![info exists prefixes($g)]} { set prefixes($g) {}}
      if {$key2 != "" && [lsearch $prefixes($g) $key1] < 0} {
	lappend prefixes($g) $key1
      }


      set cmdTable($g:$keyseq) $cmd
      set permTable($g:$keyseq) $cmd
    }

    Menu::setAccelerator $cmd [beautify $keyseq]

#    if { [info exists menuCommandTable($cmd)] } {
#      foreach ment $menuCommandTable($cmd) {
#	scan $ment %d:%s idx m
#	$m entryconfigure $idx -accelerator [beautify $keyseq]
#      }
#    }

  }

  #############################################################################
  #
  # Load key bindings from a file.
  #
  proc loadBindings {fileName} {
    set f [open $fileName]
    while {![eof $f]} {
      set line [gets $f]

      set n [string length $line]
      for {set i 0 } { $i < $n } { incr i } {
	if {![string is space [string index $line $i]]} break
      }
      # If line is blank or first non-space is a #, then ignore the line
      if {$i == $n || [string index $line $i] == "\#" } continue

      set keyseq [lindex $line 0]
      set cmd Action::[lindex $line 1]
      set flags [lrange $line 2 end]


      eval "KeyBinding::new $keyseq $cmd $flags"
    }
    close $f
  }

}
