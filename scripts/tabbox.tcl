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
# Last edit by hansen on Sun Jan 18 15:25:10 2009
#############################################################################
#
# Tabboxes are used to select from a group of windows
#
#	new w args		Create a new tabbox
#	select w i args		Select the tab with index i
#	getactive w		Get the active tab name

namespace eval TabBox {
  variable active
  variable disabledTabs 
  variable width
  variable height
  variable tabwidth
  variable selectedforeground
  variable selectedbackground
  variable unselectedforeground
  variable unselectedbackground
  variable activeforeground
  variable activebackground
  variable disabledforeground
  variable disabledbackground
  variable bd
  variable command
  variable tabs
  variable tablabels
  variable tabimages
  variable passive
  variable expand
  variable relief
  variable tabside
  variable tabheight
  variable mouseover
  variable passivewin
  variable takefocus
  variable cur_w


  #############################################################################
  #
  # Select the i'th tab and do any actions associated with it.
  #
  proc select {w i} {
    variable disabledTabs
    variable active
    variable bd
    variable tabheight
    variable tabwidth
    variable selectedbackground
    variable unselectedbackground
    variable command
    variable tabs
    variable passive

    set tab [lindex $tabs($w) $i]

    if {[lsearch -exact $disabledTabs($w) $i] >= 0} return
    if { $active($w) == $i } return

    if { ! $passive($w) } {
      placePane $w $tab
    } else {
      $command($w) $w $tab
    }

    setactive $w $tab
  }

  #############################################################################
  #
  # Return the tab name of the active tab
  #
  proc getactive {w} {
    variable tabs
    variable active

    set i $active($w)

    return [lindex $tabs($w) $i]
  }

  #############################################################################
  #
  # Return the index of the active tab
  #
  proc getactiveidx {w} {
    variable active
    return $active($w)
  }

  #############################################################################
  #
  # Respond to reconfigure of a tab (used to detect when it is first mapped)
  #
  proc tabreconfigure {w i} {
    variable active
    placehider $w $active($w)
  }
  
  #############################################################################
  #
  # Make the named table the active tab
  #
  proc setactive {w name} {
    variable tabs
    set i [lsearch -exact $tabs($w) $name]
    if {$i < 0} return

    setactiveidx $w $i
  }

  #############################################################################
  #
  # Make the tab with the specified index the active tab
  #
  proc setactiveidx {w i} {
    variable details
    variable active
    variable bd
    variable tabheight
    variable tabwidth
    variable width
    variable selectedbackground
    variable unselectedbackground
    variable tabs
    variable tabside
    variable takefocus

    set active($w) $i
    set active_tab [lindex $tabs($w) $i]

    foreach tab $tabs($w) {
      colorTab $w $tab
    }

    placehider $w $i
    if {$takefocus($w)} {
      set tab_w $w.b[lindex $tabs($w) $i]
      focus $tab_w.iw
    }
  }

  #############################################################################
  #
  # Position the "hider" used to make the selected tab look like it is connected
  # to the main window.
  #
  proc placehider {w i} {
    variable tabside
    variable tabwidth
    variable tabheight
    variable bd
    variable tabs

    set tab_w $w.b[lindex $tabs($w) $i]

    set bw [$tab_w cget -bd]

    set coords [$w coords tab$i] 
    set x [lindex $coords 0]
    set y [lindex $coords 1]

    set tw [winfo reqwidth $tab_w] 
    set th [winfo reqheight $tab_w]

#    puts "placehider $w  at $x, $y  ${tw}x${th}"

    switch $tabside($w) {
      top {
	$w coords hider [expr $x + $bw] $y
	$w itemconfigure hider -width [expr $tw - 2*$bw]
      }
      left {
	$w coords hider [expr $x - $bw] [expr $y + $th - $bw]
	$w itemconfigure hider -height [expr $th - 2*$bw]
      }
    }
  }

  #############################################################################
  #
  # Place and size the content pane for the tabbox
  #
  proc placePane {w name} {
    variable tabheight 
    variable tabwidth
    variable tabside
    variable active
    variable passive
    variable bd
    variable tabs
    variable passivewin
    variable cur_w

    set height [winfo height $w]
    set width [winfo width $w]

    if { !$passive($w) } {
      set pw $w.t$name
    } else {
      if {![info exists passivewin($w)]} return
      set pw $passivewin($w)
    }

    switch $tabside($w) {
      top {
	set x 0
	set y [expr $tabheight($w) - $bd($w)]
	set height [expr $height - $tabheight($w) + $bd($w)]
      }
      left {
	set x $tabwidth($w)
	set y 0 
	set width [expr $width - $tabwidth($w)]
      }
    }

    if { $cur_w($w) != $pw} {
      $w delete tabWin
      $w create window $x $y -anchor nw -window $pw -tags tabWin
      set cur_w($w) $pw
    }
    $pw configure -width $width  -height $height
    raise $w.hider
 }

  #############################################################################
  #
  # Place all tabs on the tab box
  #
  proc placeTabs {w} {
    variable tabside 
    variable active

    placeTabs_$tabside($w) $w
  }

  #############################################################################
  #
  # Place tabs on left of tabbox.  The tab slider is not implemented for left
  # tab tab boxes.
  #
  proc placeTabs_left {w} {
    variable width
    variable tabheight
    variable tabwidth
    variable tabs
    variable bd

    set ystart 0

    set x $tabwidth($w)
    set y $ystart
    set i 0
    set vi 0
    foreach tab $tabs($w) {
      set tab_w $w.b$tab

      $w create window [expr $x + $bd($w)] $y -anchor ne -window $tab_w -tags [list tab$i tabs]
      set thisheight [winfo reqheight $tab_w]
      set y [expr $y + $thisheight]
      incr i
      incr vi
    }
  }

  #############################################################################
  #
  # Standard tab boxes with tabs on the top.
  #
  proc placeTabs_top {w} {
    variable width
    variable tabheight
    variable tabwidth
    variable tabs
    variable bd

    set x 0
    set y $tabheight($w)
    set i 0
    set vi 0
    foreach tab $tabs($w) {
      set tab_w $w.b$tab

      $w create window $x $y -anchor sw -window $tab_w -tags [list tab$i tabs]
      set thiswidth [winfo reqwidth $tab_w]
      set x [expr $x + $thiswidth]

      incr i
      incr vi
    }
  }

  #############################################################################
  #
  # Respond to a reconfiguration (e.g., resize) of the main window
  #
  proc reconfigure {w} {
    variable tabheight 
    variable tabwidth
    variable tabside
    variable active
    variable passive
    variable bd
    variable tabs

    setactiveidx $w $active($w)

    placePane $w [lindex $tabs($w) $active($w)]
    placehider $w $active($w)
  }

  #############################################################################
  #
  # Set correct coloring for the specified tab.
  #
  proc colorTab {w tab} {
    variable activeforeground
    variable activebackground
    variable unselectedforeground
    variable unselectedbackground
    variable selectedforeground
    variable selectedbackground
    variable disabledforeground
    variable disabledbackground
    variable active
    variable disabledTabs
    variable mouseover
    variable tabs
    variable takefocus

    set tab_w $w.b$tab

    set i [lsearch $tabs($w) $tab]

    if {[lsearch -exact $disabledTabs($w) $i] >= 0} {
      set fg $disabledforeground($w)
      set bg $disabledbackground($w) 
      $tab_w.iw configure -takefocus 0
    } elseif { $mouseover($w) == $tab } {
      set fg $activeforeground($w)
      set bg $activebackground($w)
      $tab_w.iw configure -takefocus 0
    } elseif { $active($w) == $i } {
      set fg $selectedforeground($w)
      set bg $selectedbackground($w)
      $tab_w.iw configure -takefocus $takefocus($w)
    } else {
      set fg $unselectedforeground($w)
      set bg $unselectedbackground($w)
      $tab_w.iw configure -takefocus 0
    }

    colortree $tab_w -foreground $fg -background $bg -highlightbackground $bg  
  }

  proc settabstate {w tab state} {
    variable disabledTabs
    variable tabs

    set i [lsearch -exact $tabs($w) $tab]
    if { $i < 0 } return
    
    switch $state {
      disabled {
	lappend disabledTabs($w) $i
      }
      normal {
	set j [lsearch -exact $disabledTabs($w) $i]
	if { $j >= 0 } {
	  set disabledTabs($w) [lreplace $disabledTabs($w) $j $j]
	}
      }
    }
    colorTab $w $tab
  }


  #############################################################################
  #
  # Respond to mouse entering a tab.
  #
  proc mouseEnter {w tab} {
    variable mouseover

    set mouseover($w) $tab
    colorTab $w $tab
  }

  #############################################################################
  #
  # Respond to mouse leaving a tab.
  #
  proc mouseLeave {w tab} {
    variable mouseover

    set mouseover($w) ""
    colorTab $w $tab
  }

  #############################################################################
  #
  # Called by "new" to create the tabs
  #
  proc makeTabs {w} {
    variable tabs
    variable tablabels
    variable tabimages
    variable bd
    variable relief
    variable tabheight
    variable tabwidth
    variable takefocus

    set i 0
    foreach tab $tabs($w) {
      set label [lindex $tablabels($w) $i]
      set img [lindex $tabimages($w) $i]
      set tab_w $w.b$tab

      frame $tab_w -bd $bd($w) -relief $relief($w)

      frame $tab_w.iw
      pack $tab_w.iw -padx 3 -pady 1

      if { $img != "" } {
	label $tab_w.iw.img -image $img
	pack $tab_w.iw.img -side left
      }
      if { $label != "" } {
	label $tab_w.iw.l -text $label
	pack $tab_w.iw.l -side left
      }

      if { $takefocus($w) } {
	$tab_w.iw configure -highlightthickness 1
	set n [llength $tabs($w)]
	set next [expr ($i+1) % $n]
	set prev [expr ($i+$n-1) % $n]
	bind $tab_w.iw <KeyPress-Right> "TabBox::select $w $next"
	bind $tab_w.iw <KeyPress-Left> "TabBox::select $w $prev"
      }

      set bgcolor [$tab_w cget -bg]
      bind $tab_w <Enter> "TabBox::mouseEnter $w $tab"
      bind $tab_w <Leave> "TabBox::mouseLeave $w $tab"

      bind $tab_w <Button-1> "TabBox::select $w $i"
      catch { bind $tab_w.iw.l <Button-1> "TabBox::select $w $i" }
      catch { bind $tab_w.iw.img <Button-1> "TabBox::select $w $i" }

      bind $tab_w <Configure> "TabBox::tabreconfigure $w $i"

      incr i
    }

    #
    # Go back through tabs and find the largest one 
    #
    update
    set i 0
    foreach tab $tabs($w) {
      set th [winfo reqheight $tab_w]
      set tw [winfo reqwidth $tab_w]
      if {$th > $tabheight($w)} { set tabheight($w) $th }
      if {$tw > $tabwidth($w)} { set tabwidth($w) $tw }

      incr i
    }

    placeTabs $w
  }

  #############################################################################
  #
  # Make all of the panes for this tab box
  #
  proc makePanes {w} {
    variable tabs
    variable command
    variable bd
    variable relief
    variable width
    variable height
    variable tabwidth
    variable tabheight
    variable expand
    variable tabside

    foreach tab $tabs($w) {
      set tw $w.t$tab

      $command($w) $tw $tab
      $tw configure -bd $bd($w) -relief $relief($w)
    }

    update

    foreach tab $tabs($w) {
      set tw $w.t$tab

      set myWidth [winfo reqwidth $tw]
      set myHeight [winfo reqheight $tw]

      if { $myWidth > $width($w) } { set width($w) $myWidth }
      if { $myHeight > $height($w) } { set height($w) $myHeight }

      catch {grid propagate $tw 0}
      catch {pack propagate $tw 0}
    }

    if { ! $expand($w) } {
      foreach tab $tabs($w) {
	set tw $w.t$tab
	$tw configure -width $width($w) -height $height($w)
      }


      switch $tabside($w) {
	top {
	  $w configure -width $width($w) -height [expr $tabheight($w) + $height($w) - $bd($w)]
	}
	left {
	  $w configure -width [expr $width($w) + $tabwidth($w) - $bd($w)] -height $height($w)
	}
      }
    }
  }

  #############################################################################
  #
  # add pw as a passive window to w
  #
  proc addpassive {w pw} {
    variable bd
    variable relief
    variable passivewin

    set passivewin($w) $pw

    $pw configure -bd $bd($w) -relief $relief($w)
    catch {grid propagate $pw 0}
    catch {pack propagate $pw 0}
  }


  #############################################################################
  #
  # Create a new tab box.
  #
  proc new {w args} {
    variable active
    variable disabledTabs 
    variable width
    variable height
    variable selectedforeground
    variable selectedbackground
    variable unselectedforeground
    variable unselectedbackground
    variable activeforeground
    variable activebackground
    variable disabledforeground
    variable disabledbackground
    variable bd
    variable command
    variable tabs
    variable tablabels
    variable passive
    variable tabimages
    variable expand
    variable relief
    variable tabside
    variable tabheight
    variable tabwidth
    variable mouseover
    variable takefocus
    variable cur_w

    canvas $w -highlightthickness 0 -bd 0

    set selectedforeground($w) [option get $w TabBox.foreground {}]
    set selectedbackground($w) [option get $w TabBox.background {}]
    set activeforeground($w) [option get $w TabBox.activeForeground {}]
    set activebackground($w) [option get $w TabBox.activeBackground {}]

    set unselectedforeground($w) [option get $w TabBox.unselectedForeground {}]
    set unselectedbackground($w) [option get $w TabBox.unselectedBackground {}]

    set disabledforeground($w) [option get $w TabBox.disabledForeground {}]
    set disabledbackground($w) [option get $w TabBox.disabledBackground {}]


    set width($w) 300
    set height($w) 300
    set tabheight($w) 0
    set tabwidth($w)  0
    set tabs($w) {}
    set tablabels($w) {}
    set bd($w)  [option get $w TkgDialog.borderWidth {}]
    set relief($w) [option get $w TkgDialog.relief {}]
    set command($w) ""
    set passive($w) 0
    set expand($w) 0
    set tabimages($w) ""
    set noslider($w) 0
    set tabside($w) top
    set active($w) ""
    set disabledTabs($w) {}
    set compound($w) ""
    set startpage($w) ""
    set mouseover($w) ""
    set takefocus($w) 0
    set cur_w($w) ""

    parseargs $args {-width -height -tabheight -tabwidth -bd -relief -tabs \
			 -selectedbackground -selectedforeground \
			 -unselectedbackground -unselectedforeground -command \
			 -tablabels -startpage -tabimages -expand -passive \
			 -tabside -activebackground -activeforeground \
			 -compound -takefocus} -array $w

    set width($w) [rescale $width($w)]
    set height($w) [rescale $height($w)]
    set tabwidth($w) [rescale $tabwidth($w)]
    set tabheight($w) [rescale $tabheight($w)]

    $w configure -width $width($w) -height $height($w)

    set start_idx [lsearch -exact $tabs($w) $startpage($w)]
    if { $start_idx < 0 } { set start_idx 0 }

    if { [llength $tablabels($w)] == 0 } {
      set tablabels($w) $tabs($w)
    }
    if { [llength $tablabels($w)] != [llength $tabs($w)] } {
      error "-tablabels and -tabs must be of equal length"
    }

    switch $tabside($w) {
      top {
	frame $w.hider -width $bd($w) -height $bd($w)
	$w create window 0 0 -anchor sw -window $w.hider -tags hider
      }
      left {
	frame $w.hider -width $bd($w) -height $bd($w)
	$w create window 0 0 -anchor sw -window $w.hider -tags hider
      }
    }

    makeTabs $w

    if { ! $passive($w) } {
      makePanes $w
    }

    bind $w <Configure> "TabBox::reconfigure $w"


    TabBox::select $w $start_idx
  }
}
