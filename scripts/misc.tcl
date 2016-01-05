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
# Last edit by hansen on Sun Feb  1 17:56:08 2009
#
#
# Function summary:
#     mr			Get a message and apply [rescale] to it.
#     ul			Underline position generator.
#     offsetgeometry		Generate geometry string relative to another window.
#     okcancel                  Create an OK/Cancel bar for dialogs.
#     replaceExtension		Replace the extension of a file name.
#     toolbutton		Button for a toolbar
#     ldelete			Remove a named item from a list.
#     lsubtract			Do a list "set subtraction" option
#     lpop			Pop an item from the end of a list
#     lscan			Partition elements of a list
#     assoc			Find an item from a list of pairs and return value.
#     assocn			Generalized version of assoc
#     assocg			Find an item from a list of lists and return indexed list.
#     assocset			Find an item from a list of pairs and change its value
#     assocsetc			Find an item from a list of pairs and change its value (create if necessary)
#     parseargs			Parse an argument list.
#     viewFile                  View the contents of a file in a separate window.
#     min			Minimum value
#     max			Maximum value
#     ceil			Ceiling function
#     floor			Floor function
#     labelframe		Labled groove frame
#     checkframe		Labled groove frame with checkbutton label
#     radioframe		Labled groove frame with radiobutton label
#     windowframe		Labled groove frame with arbitrary window label
#     gifI			Get a gif image from a file or from cache
#     start_splash		Display the "splash" window
#     end_splash		End the "splash" window.
#     yesno			Generic yes/no dialog box
#     frb_trace			Flat radio button variable change notifier
#     frb_setMouseOverBackground Set the mouseover background for flat radio buttons.
#     frb_seeB1press		See a B1 mouse press on a flat radio button.
#     flatradiobutton		Create a flat radio button.
#     paneDecoration		Create the decoration for a resizable pane slider.
#     dialogImage		Create the image bar for a dialog box.
#     gettoplevel		Get the top level window from a path
#     Option::			Options management class
#        sfix			   Fix any special characters in a string befor saving it.
#        dialogLoad		   Load new option variables from the dialog box
#        dialogSave		   Save option variables to the dialog box
#        writePreferences	   Write current option settings to prefernces file.
#        new			   Declare a new option.
#        restoreDefaults	   Restore defaults of all options or of a class of options
#     TimeCheck			Procedure timing check
#        begin			   Begin a timer
#        end			   End a timer
#     unqiueName		Generate a unique name.
#     hexEntrySetup		Setup for hex only entry widgets.
#     encodeBits		Encode list of numbers into a single number
#     incdecEntry		Numeric entry with inc/dec buttons
#     chooseInterval            Choose an interval to use for a range
#     colortree			Set background color of a window and all its children
#     packPad			Add font-size scaled padding to the top and bottom of a frame.
#     linkVars			Link two variables 
# 

proc mr {tag} {
  return [rescale [m $tag]]
}

proc getFontScale {fontName baseEmSize} {
  return [expr [font measure $fontName "M"] / ($baseEmSize+0.0)]
}

proc colortree {w args} {
  if {[lindex $args 0] == "-list"} {
    set args [lindex $args 1]
  }

  foreach {sw val} $args {
    catch { $w configure $sw $val }
  }

  foreach cw [winfo children $w] {
    colortree $cw -list $args
  }
}

proc dobell args {
  bell
}

proc ul {n} {
  global lang

  if { $lang == "en" } {
      return $n
  } else {
      return -1
  }
}

proc safeeval {bad cmd args} {
  set L {}
  set skip 0
  foreach p $args {
    if {[lsearch -exact $bad $p] >= 0} {
      set skip 1
    } elseif {$skip == 0} {
      lappend L $p
    } else {
      set skip 0
    }
  }    
  eval $cmd $L
}


# offsetgeometry win dx dy
#
# Return a geomtry string with the specified offset from the specified window.
#
#
proc offsetgeometry { win dx dy } {
  set g [wm geometry $win]
  set x [string range $g [expr [string first "+" $g] + 1] end]
  set p [string first "+" $x]
  set y [string range $x [expr $p + 1] end]
  set x [string range $x 0 [expr $p - 1]]
  return +[expr $x + $dx ]+[expr $y + $dy]
}

proc okcancel {w args} {
  frame $w


  set cancelcommand "destroy $w"
  set okcommand "destroy $w"
  set oktext [m b.ok]
  set canceltext [m b.cancel]
  set bd [option get $w TkgDialog.borderWidth {}]
  set relief [option get $w TkgDialog.relief {}]

  parseargs $args {-oktext -canceltext -okcommand -cancelcommand -bd}

  $w configure -relief $relief -bd $bd
  button $w.ok -text $oktext -command $okcommand
  button $w.cancel -text $canceltext -command $cancelcommand

  pack $w.cancel -side right -padx 5 -pady 5
  pack $w.ok -side right -padx 5 -pady 5
}

#
# Replace the extension of name with ext.  "ext" should
# include the '.' character if it is desired.
#
proc replaceExtension {name ext} {
  set p [string last "." $name ]
  if { $p >= 0 } {
    set name [string range $name 0 [expr $p - 1]]
  }
  return "$name$ext"
}

#
# Remove all elements from d from l
#
proc lsubtract {l d} {
  foreach x $d {
    set l [ldelete $l $x]
  }
  return $l
}

#
# remove i from list l
#
proc ldelete {l i} {
  set p [lsearch $l $i]
  if { $p >= 0 } {
    return [lreplace $l $p $p]
  } {
    return $l
  }
}

#
# Given a list of the form {{t1 v1} {t2 v2} ... }
# return the value from the pair having the tag 'tag'.
#
proc assoc {tag l} {
  set value ""
  catch {
    foreach p $l {
      if {[lindex $p 0] == $tag } { 
	set value [lindex $p 1]
	break
      }
    }
  }
  return $value
}

#
# Given a list of the form {{a1 b1 ...} {a2 b2 ...} ... } return 
# the value for which l matches the nth (starting at zero) item
# in a sub-list
#
proc assocn {tag n l} {
  set value {}
  catch {
    foreach p $l {
#      puts "testing string equal [lindex $p $n] $tag"
      if {[string equal [lindex $p $n] $tag]} { 
	set value $p
	break
      }
    }
  }
  return $value
}

proc assocg {tag l} {
  set value ""
  catch {
    foreach p $l {
      if {[lindex $p 0] == $tag } { 
	set value $p
	break
      }
    }
  }
  return $value
}

proc assocset {tag l newV} {
  set value ""

  set L {}

  foreach p $l {
    if {[lindex $p 0] == $tag } { 
      lappend L [list $tag $newV]
    } else {
      lappend L $p
    }
  }
  return $L
}

proc assocsetc {tag l newV} {
  set value ""

  set L {}
  set found 0

  foreach p $l {
    if {[lindex $p 0] == $tag } { 
      lappend L [list $tag $newV]
      set found 1
    } else {
      lappend L $p
    }
  }

  if {!$found} {
    lappend L [list $tag $newV]
  }

  return $L
}

##############################################################################
#
# Parse an argument list
#
proc parseargs {argv nameset args} {
  set R {}
  set is_partial 0
  set index ""

  if {[lsearch $args  "-partial"] >= 0} {
    set is_partial 1
  }
  set array [lsearch $args  "-array"]
  if { $array >=  0} {
    set index [lindex $args [expr $array+1]]
  }

  while { [llength $argv] > 0 } {

    set sw [lindex $argv 0]

    if { [lsearch -exact $nameset $sw] >= 0 } {
      set vname [string range $sw 1 end]
      set val [lindex $argv 1]
      set argv [lrange $argv 2 end]

      if { $index == "" } {
	upvar $vname local_$vname
      } else {
	upvar ${vname}($index) local_$vname
      } 
      set  local_$vname $val
    } elseif { $is_partial } {
      lappend R $sw
      set argv [lrange $argv 1 end]
    } else {
      error "bad option \"$sw\" must be one of: $nameset"
      return
    }

  }

  return $R
}

proc viewFile {label file} {
  if {[catch { set f [open $file]}]} {
    errmsg [format [m err.viewfile] $file]
    return
  }


  set w .vfwin
  set i 0
  while { [catch { toplevel $w$i}] } {
    incr i
  }
  set w $w$i

  wm title $w $label

  frame $w.main
  text $w.main.text -yscrollcommand "$w.main.vb set" -xscrollcommand "$w.main.hb set"
  scrollbar $w.main.vb -command "$w.main.text yview"
  scrollbar $w.main.hb -orient horizontal -command "$w.main.text xview"

  grid rowconfigure $w.main 0 -weight 1
  grid columnconfigure $w.main 0 -weight 1
  grid $w.main.text -row 0 -column 0 -sticky nsew
  grid $w.main.vb -row 0 -column 1 -sticky ns
  grid $w.main.hb -row 1 -column 0 -sticky ew

  button $w.dismiss -text [m b.dismiss] -command "destroy $w"

  pack $w.main -fill both -expand 1
  pack $w.dismiss -fill x

  catch {
    $w.main.text insert end [read $f]
    close $f
  }
  $w.main.text configure -state disabled
}

proc ceil {n} {
  set n [format %f $n]
  set d [string first "." $n]
  if { $d < 0 } {
    return $n
  }
  if { [string trim [string range $n [expr $d+1] end] "0"] == "" } {
    return [expr [string range $n 0 $d] + 0.0]
  } else {
    return [expr [string range $n 0 $d] + 1.0]
  }
}

proc floor {n} {
  set n [format %f $n]
  set d [string first "." $n]
  if { $d < 0 } {
    return $n
  }
  return [expr [string range $n 0 $d] + 0.0]
}

proc min args {
  if {[llength $args] == 0 } { return 0 }

  set m [lindex $args 0]
  foreach v $args {
    if {$v < $m} { set m $v }
  }
  return $m
}

proc max args {
  if {[llength $args] == 0 } { return 0 }

  set m [lindex $args 0]
  foreach v $args {
    if {$v > $m} { set m $v }
  }
  return $m
}

proc vmin {v} {
  if {[llength $v] == 0} {
    return ""
  }

  set M [lindex $v 0]
  foreach e $v {
    if { $e < $M } { set M $e }
  }
  return $M
}

proc vmax {v} {
  if {[llength $v] == 0} {
    return ""
  }

  set M [lindex $v 0]
  foreach e $v {
    if { $e > $M } { set M $e }
  }
  return $M
}

proc imin {args} { return [vmin $args]}
proc imax {args} { return [vmax $args]}


#
# Create a labeled grooved frame.
#
proc labelframe {w lab args} {
  frame $w

  set borderwidth [option get $w LabelFrame.borderWidth {}]
  set relief [option get $w LabelFrame.relief {}]

  parseargs $args {-borderwidth -bd -relief}
  if {[info exists bd]} {set borderwidth $bd}

  $w configure -bd $borderwidth -relief $relief

#  eval "$w configure $args"
#    frame $w.labelframe_pad -height 10
#    pack $w.labelframe_pad
  label ${w}_label -text $lab
  place ${w}_label -in $w -x 10 -y -10
}

#
# Create a labeled grooved frame with a checkbutton
#
proc checkframe {w lab args} {
  
  frame $w

  set borderwidth [option get $w LabelFrame.borderWidth {}]
  set relief [option get $w LabelFrame.relief {}]

  set variable "checkframe$w"
  set command ""
  parseargs $args {-bd -relief -variable -command}
  if {[info exists bd]} {set borderwidth $bd}

  $w configure -borderwidth $borderwidth -relief $relief

  checkbutton ${w}_label -text $lab -variable $variable -command $command
  place ${w}_label -in $w -x 10 -y -10
}

#
# Create a labeled grooved frame with a radiobutton
#
proc radioframe {w lab args} {
  frame $w

  set borderwidth [option get $w LabelFrame.borderWidth {}]
  set relief [option get $w LabelFrame.relief {}]

  set variable "checkframe$w"
  set value "0"
  set command ""
  parseargs $args {-bd -relief -variable -value -command}
  if {[info exists bd]} {set borderwidth $bd}

  $w configure -borderwidth $borderwidth -relief $relief

  radiobutton ${w}_label -text $lab -variable $variable -value $value -command $command
  place ${w}_label -in $w -x 10 -y -10
}

#
# Create a grooved frame with another window as a label
#
proc windowframe {w lw args} {

  frame $w

  set borderwidth [option get $w LabelFrame.borderWidth {}]
  set relief [option get $w LabelFrame.relief {}]
  set dy 0
  parseargs $args {-bd -relief -dy}
  if {[info exists bd]} {set borderwidth $bd}

  $w configure -borderwidth $borderwidth -relief $relief

  place $lw -in $w -x 10 -y [expr -10 + $dy]
  raise $lw
}

#
# Create an image radiobutton with accompaning text and explaination.
#
proc imageRadioButton {w args} {
  set onimage ""
  set offimage ""
  set variable ""
  set label ""
  set description ""
  set value ""
  set wraplength 200

  parseargs $args {-onimage -offimage -variable -value -label -description -wraplength}

  frame $w
  radiobutton $w.button -image $offimage -selectimage $onimage -variable $variable -value $value -indicatoron 0
  label $w.header -text $label -font dialogBigExpFont
  label $w.details -text $description -justify left -wraplength $wraplength -font dialogExpFont
  frame $w.pad -width 4

  pack $w.button  -side left -anchor nw
  pack $w.pad -side left
  pack $w.header  -anchor nw
  pack $w.details -anchor nw -pady 2
}

#
# Create an image object from a .gif file name.
#
proc gifI {f} {
  global bd
  global gifTable

  if { [string index $f 0] != "/" } {
    set f "$bd/$f"
  }

  #
  # Try to get the image from the image table first.  If we don't find it,
  # create the image and save it in the table.
  #
  catch {
    return $gifTable($f)
  }

  if { [ catch { set gifTable($f) [image create photo -file $f] }]} {
    set gifTable($f) [image create photo -file "$bd/broken-img.gif" ]
  }

  return $gifTable($f)
}


#
# Post the splash window.
#
proc start_splash {} {
  global sd bd splash_start_time tkg_doSplash

  if { ! $tkg_doSplash} { return } 

  if {[catch { set splash_start_time [clock clicks -milliseconds] }]} {
    set splash_start_time "rawdelay"
  }

  if {[catch { wm state . withdrawn }]} {
    wm iconify .
  }

  update

  toplevel .splash -class Splash
  label .splash.logo -image [gifI "$bd/biggatelogo.gif"]
  pack .splash.logo
  wm overrideredirect .splash 1
  wm transient .splash ""

  set iwidth [image width [.splash.logo cget -image]]
  set iheight [image height [.splash.logo cget -image]]

  set x [expr ([winfo screenwidth .] - $iwidth)/2 ]
  set y [expr ([winfo screenheight .] - $iheight)/2 ]
  wm geometry .splash +${x}+${y}
  update
}

#
# Remove the splash window
#
proc end_splash {} {
  global bd splash_start_time tkg_doSplash tkg_splashWait

  if { ! $tkg_doSplash} { return } 

  if { $splash_start_time == "rawdelay" } {
    set time_to_go 1000
  } else {
    set time_to_go [expr $tkg_splashWait - ([clock clicks -milliseconds] - $splash_start_time)]
  }

  if { $time_to_go < 1 } { set time_to_go 1 }

  after $time_to_go {
    update
    destroy .splash
    wm deiconify .
    update
  }
}

proc yesno {msg} {
  return [tk_messageBox -default no -type yesno -icon warning -message $msg]
}

#
# Respond to a variable change in a flatradiobutton
#
proc frb_trace {w v args} {
  catch {
    global flatradiobutton_details
    set variable [assoc variable $flatradiobutton_details($w)]
    global $variable

    set v [set $variable]

    set bg [assoc bg $flatradiobutton_details($w)]
    set value [assoc value $flatradiobutton_details($w)]

    if {$v == $value } {
      $w configure -relief sunken
    } else {
      $w configure -relief flat
      $w configure -bg $bg
    }
  }
}

#
# Set brackground of flat radio button based on mouseover state
#
proc frb_setMouseOverBackground {w ismouseover} {
  global flatradiobutton_details

  set bg [assoc bg $flatradiobutton_details($w)]
  set activebackground [assoc activebackground $flatradiobutton_details($w)]
  set selectcolor [assoc selectcolor $flatradiobutton_details($w)]
  set value [assoc value $flatradiobutton_details($w)]
  set variable [assoc variable $flatradiobutton_details($w)]
  global $variable

  set v [set $variable]

  if { $ismouseover } {
    if { $v == $value } {
      $w configure -bg $selectcolor -activebackground  $selectcolor -relief raised
    } else {
      $w configure -bg $activebackground -activebackground $activebackground -relief raised
    }
  } else {
    if { $v == $value } {
      $w configure -bg $selectcolor -relief sunken
    } else {
      $w configure -bg $bg -relief flat
    }
  }
}

proc frb_seeB1press {w} {
  global flatradiobutton_details

  set command  [assoc command $flatradiobutton_details($w)]
  set value [assoc value $flatradiobutton_details($w)]
  set var [assoc variable $flatradiobutton_details($w)]

  global $var
  set $var $value

  frb_setMouseOverBackground $w 1

  eval $command
}

#
# tcl/tk 8.3 and earlier does not support flat radio buttons with "-indicatoron false".
# This is a limited implementation and only supports the features we need for the mode
# selectors.
#
proc flatradiobutton {w args} {
  global flatradiobutton_details

  set image ""
  set variable ""
  set value ""
  set command ""
  set selectcolor "\#aaaacc"
  set activebackground [option get . FlatRadioButton.activeBackground {}]
  set bg [option get . FlatRadioButton.background {}]

  parseargs $args {-image -variable -value -command -selectcolor -bg -activebackground}


  set flatradiobutton_details($w) {}
  lappend flatradiobutton_details($w) [list image $image]
  lappend flatradiobutton_details($w) [list variable $variable]
  lappend flatradiobutton_details($w) [list value $value]
  lappend flatradiobutton_details($w) [list command $command]
  lappend flatradiobutton_details($w) [list selectcolor $selectcolor]
  lappend flatradiobutton_details($w) [list bg $bg]
  lappend flatradiobutton_details($w) [list activebackground $activebackground]

  global $variable

  if {[catch {button $w -takefocus 0 -image $image -bd 1 -relief flat -overrelief raised}]} {
    button $w -takefocus 0 -image $image -bd 1 -relief flat
  }

  set v [set $variable]
  if {$v == $value } {
    $w configure -relief sunken
    $w configure -bg $selectcolor
  } else {
    $w configure -relief flat
  }

  trace variable $variable w "frb_trace $w"
  bind $w <Destroy> "trace vdelete $variable w \"frb_trace $w\""
  bind $w <ButtonPress-1> "frb_seeB1press $w"

  bind $w <Enter> "+ frb_setMouseOverBackground %W 1"
  bind $w <Leave> "+ frb_setMouseOverBackground %W 0"
}

proc paneDecoration {w args} {
  set orient vertical
  parseargs $args {-orient}

  if { $orient == "vertical" } {
    set width 50
    set height 2
  } else {
    set width 2
    set height 50
  }

  frame $w.v1 -bd 1 -relief raised -width $width -height $height
  frame $w.v2 -bd 1 -relief raised -width $width -height $height
  frame $w.v3 -bd 1 -relief raised -width $width -height $height

  if { $orient == "vertical" } {
    pack $w.v1
    pack $w.v2
    pack $w.v3
  } else {
    pack $w.v1 -side left
    pack $w.v2 -side left
    pack $w.v3 -side left
  }
  
}

proc dialogImage {w args} {
  global tkg_showDialogImage
  set fontScale [getFontScale dialogExpFont 10]

  set image ""
  set caption ""
  set font dialogCapFont
  set bd 0
  set width [rescale [m @opt.sidebar.width]]
  set height 0
  set relief flat
  set explaination ""
  set labelheight [rescale 50]
  set imgbd 0
  set force 0
  set imgrelief flat
  set expfont dialogExpFont

  parseargs $args {-image -caption -font -bd -imgbd -imgrelief -width -relief -explaination -labelheight -force -expfont}

  if {!$tkg_showDialogImage && !$force} {
    frame $w
    return
  }

  set iheight [image height $image]
  set iwidth  [image width $image]

  set height [expr $iheight + 100 ]

  frame $w -bd $bd -relief $relief -width $width -height $height

  label $w.cap -text $caption -font $font
  label $w.img -image $image -bd $imgbd -relief $imgrelief
  #  label $w.exp -text $explaination -justify left -wraplength [expr 155 * $fontScale] -font $expfont
  label $w.exp -text $explaination -justify left -wraplength [expr $width - 10] -font $expfont

  set x [expr $width/2]
  set y [expr $labelheight/2]

  place $w.cap -x $x -y $y -anchor center
  set y [expr $y + $iheight/2 + $labelheight]
  place $w.img -x $x -y $y -anchor center
  set y [expr $y + [image height $image]/2 + 30]
  place $w.exp -x $x -y $y -anchor n

  set bottom [expr [winfo reqheight $w.exp] + $y + 15]

  if { $bottom > $height } {
    $w configure -height $bottom
  }

}

#
# Validate function for bit size selector.  All chars must be digits, and there
# is a 3 char maximum.
#
proc bsValidate {w act cur c newv} {
#  puts "bsValidate $w <$act> <$cur> <$c> <$newv>"
#  if {[string length $newv] == 0} { return 0 }
  if {$act == 1} {
    if {[string length $cur] > 2} {
      return 0
    }
    return [string is digit $c]
  }

  return 1
}

proc bitsizeselector {w args} {

  set entry 1

  parseargs $args {-variable -value -width -entry -takefocus}

  set argv {}
  if {[info exists variable]} { lappend argv -variable $variable }
  if {[info exists value]} { lappend argv -value $value }
  if {[info exists width]} { lappend argv -width $width }
  if {[info exists entry]} { lappend argv -entry $entry }
  if {[info exists takefocus]} { lappend argv -takefocus $takefocus }

  eval "Dropbox::new $w $argv -entry $entry -validatecommand bsValidate"
  for { set i 1} { $i <= 32 } { incr i } {
    Dropbox::itemadd $w $i
  }
}

namespace eval TimeCheck {
  variable timer

  proc begin {e} {
    variable timer

    set timer($e) [clock clicks -milliseconds]
    puts "begin $e" 
  }
  proc end {e} {
    variable timer

    set t  [clock clicks -milliseconds]
    set dt [expr ($t - $timer($e) + 0.0)/1000.0]
    puts "end $e $dt"
  }
}

#
# Fix name so as not to have special characters and make sure it does
# not conflict with names in l.
#
proc unqiueName {name l} {
  puts "unqiueName $name"
  return $name
}

#
# Setup for hex only entry widgets
#
proc hexEntrySetup {} {
  bind HexEntry <Delete> { continue }
  bind HexEntry <BackSpace> { continue }
  bind HexEntry <Control-KeyPress> { continue }
  bind HexEntry <KeyPress> {
    set c [string tolower %A]

    if { $c == "" } { continue }

    if { [string first $c "0123456789abcdef"] < 0 } { break }


    if {[%W selection present]} {
      %W delete sel.first sel.last
    } else {
      %W delete insert
    }
    set L [string length [%W get]]

    if { $L >= 8 } { break }

    #
    # Temporarily disable the "HexEntry" event handler and send a regular
    # event to insert the character.
    #
    bindtags %W [lrange [bindtags %W] 1 end]
    event generate %W <KeyPress> -keysym $c
    bindtags %W [concat [list HexEntry] [bindtags %W]]

    break
  }
}

#
# Setup for number only entry widgets
#
proc numEntrySetup {} {
  bind NumEntry <Delete> { continue }
  bind NumEntry <BackSpace> { continue }
  bind NumEntry <Control-KeyPress> { continue }
  bind NumEntry <KeyPress> {
    set c [string tolower %A]

    if { $c == "" } { continue }

    if { [string first $c "0123456789"] < 0 } { break }

    if {[%W selection present]} {
      %W delete sel.first sel.last
    } else {
      %W delete insert
    }
    set L [string length [%W get]]

    if { $L >= 8 } { break }

    #
    # Temporarily disable the "NumEntry" event handler and send a regular
    # event to insert the character.
    #
    bindtags %W [lrange [bindtags %W] 1 end]
    event generate %W <KeyPress> -keysym $c
    bindtags %W [concat [list NumEntry] [bindtags %W]]

    break
  }
}


#
# Setup for number only entry widgets
#
proc floatEntrySetup {} {
  bind FloatEntry <Delete> { continue }
  bind FloatEntry <BackSpace> { continue }
  bind FloatEntry <Control-KeyPress> { continue }
  bind FloatEntry <KeyPress> {
    set c [string tolower %A]

    if { $c == "" } { continue }

    if { [string first $c "0123456789."] < 0 } { break }

    if {[%W selection present]} {
      %W delete sel.first sel.last
    } else {
      %W delete insert
    }
    set L [string length [%W get]]

    if { $L >= 8 } { break }

    #
    # Temporarily disable the "FloatEntry" event handler and send a regular
    # event to insert the character.
    #
    bindtags %W [lrange [bindtags %W] 1 end]
    if { $c == "." } { set c period }
    event generate %W <KeyPress> -keysym $c
    bindtags %W [concat [list FloatEntry] [bindtags %W]]

    break
  }
}

#############################################################################
#
# Helping function for shellWindow.
#
#############################################################################
proc shellExec {args} {
  global shellCommand

  set w .shell_win

  $w.text insert end "${shellCommand}\n" cmd
  $w.text tag configure cmd -foreground blue

  if {[catch { set result [namespace eval :: "$shellCommand"] } err]} {
    $w.text insert end "${err}\n" err
    $w.text tag configure err -foreground red
  } else {
    $w.text insert end "${result}\n" result
    $w.text tag configure result -foreground black
  }

  $w.text see end

  set shellCommand ""
}

#############################################################################
#
# Create a shell window in which we can type an execute tcl commands for
# debugging purposes.
#
#############################################################################
proc shellWindow {} {
  set w .shell_win

  if {[catch {toplevel $w}]} { 
    raise $w
    return 
  }

  wm title $w "TKGate: Tcl Shell"

  button $w.dismiss -text Dismiss -command "destroy $w"
  pack $w.dismiss -side bottom -anchor e -padx 5 -pady 5

  frame $w.b
  pack $w.b -side bottom -fill x -expand 1

  label $w.b.l -text "Command: "
  pack $w.b.l -side left -padx 5 -pady 5

  entry $w.b.e -textvariable shellCommand
  pack $w.b.e -fill x -padx 5 -pady 5 -expand 1

  bind $w.b.e <Return> shellExec

  focus $w.b.e

  text $w.text -bd 2 -relief sunken -width 60 -height 20 -yscrollcommand "$w.vb set"
  pack $w.text -padx 5 -pady 5 -fill both -expand 1 -side left

  scrollbar $w.vb -orient vertical -command "$w.text yview"
  pack $w.vb -side right -padx 5 -pady 5 -fill y -expand 1
}

#############################################################################
#
# Perform a standard wait for a dialog box.  We update all events, set
# a grab on the dialog box and wait for the dialog box to be destroyed.
# We then release the grab and call gat_syncInterface to cause any
# internal circuit changes to be synchronized with tcl/tk elements.
#
#############################################################################
set dialogWaitStack {}
proc dialogWait {w args} {
  global dialogWaitStack
  set dosync 1

  parseargs $args {-dosync}

  #
  # Put window on stack
  #
  lappend dialogWaitStack $w

  update
  grab set $w

  tkwait window $w

  if {[llength $dialogWaitStack] > 1 } {
    set dialogWaitStack [lrange $dialogWaitStack 0 end-1]
    set lastW [lindex $dialogWaitStack [expr [llength $dialogWaitStack]-1]]
    catch { grab release $w }
    grab set $lastW
  } else {
    catch { grab release $w }
    set dialogWaitStack {}
  }
  
  if {$dosync} {
    gat_syncInterface
  }
}

#############################################################################
#
# Return the top-level window that w is contained in.
#
proc gettoplevel {w} {
  while {1} {
    set pw [winfo parent $w]
    if { $pw == "" || $pw == "."} break
    set w $pw
  }
  return $w
}

#############################################################################
#
# If a number is prepended with a '*', scale it by the current font scale.
#
proc rescale {n} {
  if {[string index $n 0] == "*"} {
    return [expr int([getFontScale dialogExpFont 13] * [string range $n 1 end])]
  }

  return $n
}

#############################################################################
#
# Return non-zero if $c is a character from a word ( alphanumeric or "_").
#
proc iswordchar {c} {
  if { [string is alnum $c] || $c == "_" } {
    return 1
  }
  return 0
}

#############################################################################
#
# Return non-zero if $c is a character from a word including task names ( alphanumeric, "_" or "$").
#
proc istaskwordchar {c} {
  if { [string is alnum $c] || $c == "_"  || $c == "\$" } {
    return 1
  }
  return 0
}

#############################################################################
#
# Find the position of the first occurance of $word in $line with the
# restriction that $word must be surrounded by non-word characters or at the
# start or end of a line.  Returns -1 if $word is not found
#
proc findword {line word} {
  set llen [string length $line]
  set wlen [string length $word]
  set p 0
  while {[set p [string first $word [string range $line $p end]]] >= 0 } {
    if { ($p == 0 || ! [iswordchar [string index $line [expr $p - 1 ]]]) \
	     && ($p+$wlen >= $llen || ! [iswordchar [string index $line [expr $p + $wlen ]]]) } {
      break
    }
  }

  return $p
}


proc lpop {_l} {
  upvar $_l l

  set l [lrange $l 0 [expr [llength $l] - 2]]
}

proc lscan {l args} {
  set i 0

  foreach v $args {
    upvar $v _v$i

    set _v$i [lindex $l $i]
    incr i
  }
}

proc llast {l} {
  set n [llength $l]
  if {$n > 0} {
    return [lindex $l [expr $n - 1]]
  } else {
    return ""
  }
}

proc makeFriendlyChar {c} {
  if {[string is graph $c] || $c == " " } { return $c }
  set n 0
  binary scan $c c n

  return \\[format %03o $n]
}

proc findLibraryFile {name} {
  global tkg_simVLibPath 

  foreach directory $tkg_simVLibPath {
    set directory [namespace eval :: "eval concat $directory"]
    if {[file exists $directory/$name]} {
      return $directory/$name
    }
    if {[file exists $directory/$name.v]} {
      return $directory/$name.v
    }
  }

  return ""
}

proc encodeBits {b value} {
  set n [llength $value] 

  set out 0
  for {set i 0} {$i < $n} {incr i} {
    if {[lindex $value $i]} {
      set out [expr $out | (1 << $i)]
    }
  }
  return $out
}

proc validate_hex {s} {
  if {[scan $s %x n] != 1} {
    set n 0
  }
  return $n
}

proc validate_posint {s} {
  if {[scan $s %d n] != 1 || $n < 1} {
    set n 1
  }
  return $n
}

proc validate_nonnegint {s} {
  if {[scan $s %d n] != 1 || $n < 0} {
    set n 0
  }
  return $n
}

proc validate_int {s} {
  if {[scan $s %d n] != 1} {
    set n 0
  }
  return $n
}

#
# Create a basic toolbar button
#
proc toolbutton {w img act help args} {
  set state normal

  parseargs $args {-state}

  if {[catch {button $w -image [gifI $img] -takefocus 0 -relief flat -command $act -overrelief raised -state $state}]} {
    button $w -image [gifI $img] -takefocus 0 -relief flat -command $act  -state $state
  }

  if { $help != ""} {
    helpon $w [m $help]
  }
}

#
# Validate function for bit size selector.  All chars must be digits, and there
# is a 3 char maximum.
#
proc hexValidate {w act cur c newv} {
  if {$act == 1} {
    return [string is xdigit $c]
  }
  
  return 1
}

proc _incdecDelta {varName args} {
  upvar \#0 $varName v

  set min 0
  set max 2147483648
  set format %f
  set delta 1
  parseargs $args {-delta -min -max -format}
  
  scan $v $format n

  set newValue [expr $n + $delta]

  if { $newValue < $min } { set newValue $min }
  if { $newValue > $max } { set newValue $max }
  set v [format $format $newValue]
}

proc incdecEntry {w args} {
  frame $w

  entry $w.e

  set class ""
  set variable ""
  set min 0
  set max 1e20
  set width 8
  set format %f
  set justify right
  set validatecommand ""
  set font [$w.e cget -font]
  set delta 1
  parseargs $args {-variable -width -min -max -class -delta -format -justify -validatecommand -font}

  if { $validatecommand != "" } {
    $w.e configure -bg white -width $width -textvariable $variable  -justify $justify \
	-validate key -validatecommand "$validatecommand %W %d %s %S %P" \
	-invalidcommand bell -font $font
  } else {
    $w.e configure -bg white -width $width -textvariable $variable  -justify $justify -font $font
  }
  button $w.up -image [gifI up.gif] -command "_incdecDelta $variable -delta $delta -max $max -min $min -format $format"
  button $w.dn -image [gifI down.gif] -command "_incdecDelta $variable -delta [expr -$delta] -max $max -min $min -format $format"

  if { $class != "" } {
    bindtags $w.e [concat [list $class] [bindtags $w.e]]
  }

  pack $w.e -side left
  pack $w.up -side top -fill y -expand 1
  pack $w.dn -side bottom -fill y -expand 1
}

proc replaceSwitchValue {cmd sname value} {
  set i [lsearch $cmd $sname]
  if {$i < 0} return $cmd
  incr i
  return [lreplace $cmd $i $i $value]
} 

proc incdecEntry_configure {w args} {
  parseargs $args {-min -max -delta}

  set upcommand [$w.up cget -command]
  set dncommand [$w.dn cget -command]

  if {[info exists min]} {
    set upcommand [replaceSwitchValue $upcommand -min $min]
    set dncommand [replaceSwitchValue $dncommand -min $min]
  }
  if {[info exists max]} {
    set upcommand [replaceSwitchValue $upcommand -max $max]
    set dncommand [replaceSwitchValue $dncommand -max $max]
  }
  if {[info exists delta]} {
    set upcommand [replaceSwitchValue $upcommand -delta $delta]
    set dncommand [replaceSwitchValue $dncommand -delta [expr -$delta]]
  }

  $w.up configure -command $upcommand
  $w.dn configure -command $dncommand
}

proc chooseInterval {D} {

  set G [expr exp(int(log($D)/log(10.0)+0.999999999)*log(10.0))]
  set Q [expr ($G-$D)/$G ]
  
  if {$Q >= 0.8} { return [expr $G*0.02 ] }

  if {$Q >= 0.6} { return [expr $G*0.05 ] }

  return [expr $G*0.1 ]
}

#
# add pads on the top and botton of a window packed with "pack"
#
proc packPad {w args} {

  set toppad 10
  set bottompad 10

  parseargs $args {-pad -toppad -bottompad}
  if {[info exists pad]} {
    set toppad $pad
    set bottompad $pad
  }

  frame $w.pad_top -height [rescale *$toppad]
  frame $w.pad_bottom -height [rescale *$bottompad]
  pack $w.pad_top -side top
  pack $w.pad_bottom -side bottom
}

proc linkVars_change {v1 v2 n args} {
  global linkVars_assoc
  upvar \#0 $v1 _v1
  upvar \#0 $v2 _v2

  set assoc $linkVars_assoc($v1:$v2)

  if {$n == 1} {
    set p [assocn $_v1 0 $assoc]
    set new_v2 [lindex $p 1]
#    puts "$v1 changed to $_v1, $v2 will become $new_v2"
    if { $new_v2 != $_v2 } {
      set _v2 $new_v2
    }
  } else {
    set p [assocn $_v2 1 $assoc]
    set new_v1 [lindex $p 0]
#    puts "$v2 changed to $_v2, $v1 will become $new_v1"
    if { $new_v1 != $_v1 } {
      set _v1 $new_v1
    }
  }
}

#############################################################################
#
# Link values of v1 and v2 with an association list.  When one variable
# changes, the other will change to reflect corresponding value in the
# association list.
#
proc linkVars {v1 v2 assoc} {
  global linkVars_assoc
  upvar \#0 $v1 _v1
  upvar \#0 $v2 _v2

  set linkVars_assoc($v1:$v2) $assoc
  trace variable _v1 w "linkVars_change $v1 $v2 1"
  trace variable _v2 w "linkVars_change $v1 $v2 2"
}

