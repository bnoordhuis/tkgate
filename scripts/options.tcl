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
# Last edit by hansen on Tue Dec 30 00:44:09 2008
#
#
#
# These methods are used for managing options in tkgate.  For each option "name"
# there are three (or fixe) variables that are used:
#
#     tkg_name        The currently active value of the parameter.
#     opts_name       The value to be shown in the options dialog box.
#     tkgdef_name     The default value of the option.
#     loctkg_name(l)  Locale specific versions of tkg_ (used only for some options). 
#
# Each option belongs to a class.  If no class is specified, then it will be added
# to the class "generic".
#
# Examples:
#
# Option::new smoothScroll 1 -optclass display		Defines an option "smoothScroll" belonging
#							to class "display" and with a default value of 1.
#
# Option::dialogLoad			Load all tkg_ variables into the opts_ variables.
# Option::dialogSave			ave all opts_ variables into the tkg_variables
# Option::writePreferences		Write the preferences file.
# Option::dialogLoadDefaults		Load all default values into the opts_ variables.
# Option::dialogLoadDefaults colors	Load all default values from class "colors" into the opts_ variables.
#
# Option::dialogSave -optclass Print -prefix pd
#     Save opts_ values in the Print class to variables with a pd_ prefix.
#
# Option::version ver			Indicate version of preferences file.
# Option::value name value		Set an option to a specified value
#
namespace eval Option {
  variable option_list
  variable minval
  variable maxval

  proc sfix {s} {
    set q ""
    set L [string length $s]

    set need_quotes [expr $L == 0]

    for { set i 0 } { $i < $L } { incr i } {
      set c [string index $s $i]
      if { $c == "\\" || $c == "\$" || $c == "\[" || $c == "\]" } {
	set c "\\$c"
      }
      if { $c == " " || $c == "\t"} {
	set need_quotes 1
      }

      set q $q$c
    }

    if { $need_quotes } {
      return "\"$q\""
    } else {
      return $q
    }
  }

  #
  # Load all options from
  #
  proc dialogLoad {args} {
    variable option_list

    set prefix "tkg"
    parseargs $args {-optclass -prefix}

    if {[info exists optclass]} {
      set names $optclass
    } else {
      set names [array names option_list "*"]
    }

    foreach n $names {
      foreach v $option_list($n) {
	global ${prefix}_$v opts_$v

	set opts_$v [set ${prefix}_$v]
      } 
    }
  }

  proc dialogSave {args} {
    variable option_list

    set prefix "tkg"
    parseargs $args {-optclass -prefix}

    if {[info exists optclass]} {
      set names $optclass
    } else {
      set names [array names option_list "*"]
    }

    foreach n $names {
      foreach v $option_list($n) {
	global ${prefix}_$v opts_$v

	set ${prefix}_$v [set opts_$v]
      } 
    }
  }

  proc writePreferences {args} {
    global tkg_prefFile tkg_progVer
    variable option_list


    set names [lindex $args 0]
    if { $names == "" } {
      set names [array names option_list "*"]
    }

    set fd [open $tkg_prefFile w]
    puts $fd "#"
    puts $fd "# Automatically generated preferences file - DO NOT EDIT"
    puts $fd "#"
    puts $fd ""
    puts $fd "Option::version $tkg_progVer"

    foreach n $names {
      puts $fd ""
      puts $fd "#"
      puts $fd "# $n settings"
      puts $fd "#"
      foreach v $option_list($n) {
	global tkg_$v tkgdef_$v
	global territory

	set value [set tkg_$v]
	set dvalue [set tkgdef_$v]

	if {[array exists ::loctkg_$v]} {
	  set did_my_locale 0
	  foreach {loc locvalue} [array get ::loctkg_$v] {
	    if {$loc == "*" } continue
	    if {$loc == $territory} {
	      set locvalue $value
	      set did_my_locale 1
	    }
	    puts $fd "Option::value ${v}($loc) [sfix $locvalue]"
	  }
	  if {!$did_my_locale} {
	    puts $fd "Option::value ${v}($territory) [sfix $value]"
	  }
	} else {
	  puts $fd "Option::value $v [sfix $value]"
	}
      } 
    }
    close $fd
  }

  proc version {ver} {
    global tkg_optionsVersion

    set tkg_optionsVersion $ver
  }

  #############################################################################
  #
  # Restore defaults from the tkgdef_ to opts_ variables.
  #
  proc restoreDefaults {args} {
    variable option_list

    set names [array names option_list "*"]
    foreach n $names {
      if {[llength $args] == 0 || [lsearch $args $n] >= 0 } {
	foreach v $option_list($n) {
	  set dvar tkgdef_$v
	  set ovar opts_$v
	  global $dvar $ovar

	  set $ovar [set $dvar]
	}
      }
    }
  }

  proc value {name val} {
    variable minval
    variable maxval

    if {[scan $name "%\[^(\](%\[^)\])" baseName locName] == 2} {
      global territory
      global loctkg_$baseName

      set name $baseName
      set loctkg_${name}($locName) $val
      if { $locName != $territory } { return }
    }

    global tkg_$name

    catch { if {$val < $minval($name) } { set val $minval($name) } }
    catch { if {$val > $maxval($name) } { set val $maxval($name) } }

    set tkg_$name $val
  }

  #
  # Set the default value of an option.
  #
  proc setDefault {name val} {
    variable minval
    variable maxval

    if {[scan $name "%\[^(\](%\[^)\])" baseName locName] == 2} {
      global territory
      global loctkg_$baseName

      set name $baseName
      set loctkg_${name}($locName) $val
      if { $locName != $territory } { return }
    }

    global tkg_$name tkgdef_$name

    catch { if {$val < $minval($name) } { set val $minval($name) } }
    catch { if {$val > $maxval($name) } { set val $maxval($name) } }

    set tkg_$name $val
    set tkgdef_$name $val
  }

  #
  # Decare a new option
  #
  proc new {name dval args} {
    variable option_list
    variable minval
    variable maxval

    set optclass "generic"
    parseargs $args {-optclass -minvalue -maxvalue}

    if {[scan $name "%\[^(\](%\[^)\])" baseName locName] == 2} {
      global territory
      global loctkg_$baseName

      set name $baseName
      set loctkg_${name}($locName) $dval

      if { $locName != $territory && ($locName != "*" ||  [info exists tkg_$name])} { return }
    }

    global tkg_$name tkgdef_$name opts_$name

    catch { set minval($name) $minvalue }
    catch { set maxval($name) $maxvalue }

    lappend option_list($optclass) $name

    set tkg_$name $dval
    set tkgdef_$name $dval
    set opts_$name $dval
  }
}
