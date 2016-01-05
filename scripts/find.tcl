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
# Last edit by hansen on Fri Jan  9 09:16:28 2009
#

namespace eval Find {
  variable lb_w		""
  variable what
  variable target	""
  variable reqListEv
  variable qualifier	""
  variable data
  variable goto

  #
  # If this is the first time we have done a find, set inital settings.
  #
  proc init {} {
    variable what
    variable qualifier

    if { $qualifier == "" } {
      set qualifier [m find.qual.contains]
      set what(ignorecase) 0
      set what(gates) 1
      set what(nets) 1
      set what(text) 1
    }
  }

  #
  # Get the type code for a search
  #
  proc getType {} {
    set type 0

    if { $Find::what(nets) } { incr type 1 }
    if { $Find::what(gates) } { incr type 2 }
    if { $Find::what(text) } { incr type 4 }
    if { $Find::what(ignorecase) } { incr type 256 }

    return $type
  }

  #
  # Get the qualifier code for a search
  #
  proc getQual {} {
    variable qualifier

    if { $qualifier == [m find.qual.contains] } {
      return 0
    }

    if { $qualifier == [m find.qual.begins] } {
      return 1
    }

    if { $qualifier == [m find.qual.ends] } {
      return 2
    }

    if { $qualifier == [m find.qual.matches] } {
      return 2
    }

    return 0
  }


  #############################################################################
  #
  # Get the list of matching strings and display it in the spreadsheet.
  #
  proc getFindList args {
    variable target
    variable data
    variable lb_w

    set w .finder

    set data(count) 0

    action -Find {
      gat_find list -type [Find::getType] -qualifier [Find::getQual] -target $Find::target -variable Find::data
    }

    set n $data(count)

    SpreadSheet::flush $lb_w
    for { set i 0 } { $i < $n } {incr i } {
      set mod [lindex $data($i) 0]
      set type [m @find.type.[lindex $data($i) 1]]
      set name [lindex $data($i) 2]

      SpreadSheet::insert $lb_w end [list $mod $type $name]
    }
    SpreadSheet::see $lb_w 0
  }

  proc reqFindList {} {
    variable reqListEv

    catch { after cancel $reqListEv }
    set reqListEv [after 500 Find::getFindList]
  }

  proc findNext {} {
    variable lb_w

    set n [SpreadSheet::size $lb_w]
    set idx [SpreadSheet::getselection $lb_w]
    if { $idx == "" } {
      set idx 0
    } else {
      incr idx
    }
    if { $idx >= $n } {
      set idx 0
    }
    if {$n > 0 } {
      SpreadSheet::clearselection $lb_w
      SpreadSheet::setselection $lb_w $idx
      SpreadSheet::see $lb_w $idx

      set Find::goto [SpreadSheet::get $lb_w $idx]

      action Find { 
	gat_find goto $Find::goto
      }
    }
  }

  proc gotoSelected {} {
    variable lb_w

    set idx [SpreadSheet::getselection $lb_w]
#    puts "goto idx=$idx"
    
    if { $idx != "" } {
      set Find::goto [SpreadSheet::get $lb_w $idx]

      action Find { 
	gat_find goto $Find::goto
      }
    }
  }

  proc buildActions {w} {
    frame $w -relief raised -bd 2
    button $w.find -text [m b.find] -command { Find::findNext }
    button $w.clear -text [m b.clear] -command { set Find::target ""; Find::getFindList }
    button $w.close -text [m b.close] -command { destroy .finder  }

    pack $w.close -side right -pady 5 -padx 5 -fill x
    pack $w.clear -side right -pady 5 -padx 5 -fill x
    pack $w.find -side right -pady 5 -padx 5 -fill x
  }

  #
  # Build the search target selector
  #
  proc buildTarget {w} {
    variable qualifier
    variable what

    labelframe $w [m find.target]
    frame $w.pad -height 8
    pack $w.pad

    #
    # Entry for the search string and qualifier selection
    #
    frame $w.str
    label $w.str.l -text [m find.search]
    Dropbox::new $w.str.db -variable Find::qualifier -width 14 -command Find::getFindList
    Dropbox::itemadd $w.str.db [m find.qual.contains] -islist 0
    Dropbox::itemadd $w.str.db [m find.qual.begins] -islist 0
    Dropbox::itemadd $w.str.db [m find.qual.ends] -islist 0
    Dropbox::itemadd $w.str.db [m find.qual.matches] -islist 0

    entry $w.str.e -textvariable Find::target

    pack $w.str.l -side left -padx 5 -pady 5
    if {[m @find.qual.pos] != "after" } {
      pack $w.str.db -side left -padx 5 -pady 5
      pack $w.str.e -side left -padx 5 -pady 5 -fill x -expand 1
    } else {
      pack $w.str.e -side left -padx 5 -pady 5 -fill x -expand 1
      pack $w.str.db -side left -padx 5 -pady 5
    }
    $w.str.e selection range 0 end
    focus $w.str.e
    pack $w.str -fill x -expand 1


    frame $w.ignorecase
    checkbutton $w.ignorecase.b -text [m find.ignorecase] -variable Find::what(ignorecase) -command Find::getFindList
    pack $w.ignorecase.b
    pack $w.ignorecase -pady 2 -padx 3 -anchor w

    #
    #
    #
    frame $w.dom
    label $w.dom.l -text [m find.dom.label]
    pack $w.dom.l -anchor w -padx 5 -pady 5
    frame $w.dom.pad -width 15
    checkbutton $w.dom.gates -text [m find.dom.gates] -variable Find::what(gates) -command Find::getFindList
    checkbutton $w.dom.nets -text [m find.dom.nets] -variable Find::what(nets) -command Find::getFindList
    checkbutton $w.dom.text -text [m find.dom.text] -variable Find::what(text) -command Find::getFindList
    pack $w.dom.pad $w.dom.gates $w.dom.nets $w.dom.text -pady 2 -padx 3 -anchor w -side left
    pack $w.dom -fill x
  }

  proc buildResultsBox {w} {
    variable lb_w

    set lb_w $w.lb

    labelframe $w [m find.results]

    SpreadSheet::create $w.lb -height 10 -bd 2 -relief sunken -yscrollcommand "$w.vb set" \
	-entrycommand Find::manager -selectmode single
    SpreadSheet::addcolumn $w.lb -width 12 -header [m find.result.module]
    SpreadSheet::addcolumn $w.lb -width 10 -header [m find.result.type]
    SpreadSheet::addcolumn $w.lb -width 30 -header [m find.result.name]

    scrollbar $w.vb -orient vertical -command "SpreadSheet::yview $w.lb"
    bind $w.lb <Double-ButtonPress-1> Find::gotoSelected

    grid $w.lb -row 1 -column 1 -sticky nsew -padx 1 -pady 1
    grid $w.vb -row 1 -column 2 -sticky nsew -padx 1 -pady 1

    grid columnconfigure $w 0 -minsize 5
    grid columnconfigure $w 3 -minsize 5
#    grid columnconfigure $w 1 -weight 1

    grid rowconfigure $w 0 -minsize 10
    grid rowconfigure $w 3 -minsize 5
  }

  proc manager {cmd args} {
    # Instead of doing entry when we get a double click, goto that selected item
    switch $cmd {
      doublePress {
	Find::gotoSelected	
	return 0
      }
      canenter {
	return 0
      }
    }
  }

  proc post {} {
    variable target
    global simOn

    if { $simOn } {
      errmsg "'Find' not supported in simulation mode."
      return
    }

    set w .finder

    if { [catch { toplevel $w }] } {
      raise $w
      Find::findNext
      return
    }

    init
 
    wm resizable $w 0 0
    wm title $w [m find.title]
    wm geometry $w [offsetgeometry . 50 50 ]
#    wm transient $w .


    #
    # Action buttons on the bottom
    #
    buildActions $w.ok
    pack $w.ok -fill both -ipadx 5 -ipady 5 -side bottom

    #
    # Build top frame area
    #
    frame $w.top -bd 2 -relief raised
    pack $w.top -fill both

    #
    # Build the dialog image
    #
    dialogImage $w.top.di -image [gifI search.gif] -caption [m find.label] -explaination [m find.exp]
    pack $w.top.di -side left -fill both

    frame $w.top.pad -height 8
    pack $w.top.pad

    buildTarget $w.top.target
    pack $w.top.target  -fill both -padx 5 -pady 5

    #
    # Build the search results box 
    #
    buildResultsBox $w.top.list
    pack $w.top.list -fill both -padx 5 -pady 5

    bind $w <KeyPress> Find::reqFindList
    #  pack $w.l  -fill both -ipadx 5 -ipady 5


    bind $w <KeyPress-Return> { .finder.ok.find invoke }


    getFindList
  }
}

