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
# Last edit by hansen on Fri Sep 21 22:03:18 2007
#
set cms_status 0
set debugMessageNum 0

######################################################################
#
# Display a simple error message with an OK button to dismiss
#
proc errmsg {msg} {
  tk_messageBox -default ok -icon error -type ok -message $msg
}

proc tkScreenChanged args {
#  puts "tkScreenChanged $args"
}

proc confirmMsg {msg} {
  global msg_font cms_status

  set cms_status 0

  set result [tk_messageBox -default ok -icon question -type okcancel -message $msg]

  if { "$result" == "ok" } { set cms_status 1 }

  return $cms_status
}

proc tkg_verilogErr {fn msg} {
  InfoPanel::log "$fn, $msg" -color red
}

#
# Post a debugging message with a dismiss button
#
proc tkg_debugMessage {msg} {
  global debugMessageNum

  scan [winfo pointerxy .] "%d %d" x y

  set w .tempmsg$debugMessageNum
  incr debugMessageNum
  toplevel $w -bg bisque

  wm geometry $w +[expr $x + 5]+[expr $y - 30]

  label $w.l -text $msg -bg bisque -justify left -font boldLabelFont
  button $w.d -text [m b.dismiss] -command "destroy $w"
  pack $w.l -padx 4 -pady 4
  pack $w.d -padx 4 -pady 4 -fill x

}


#
# Post a message which lasts only as long as the mouse is depressed
#
proc tkg_tempMessage {msg} {
  scan [winfo pointerxy .] "%d %d" x y

  catch { destroy .tempmsg }
  toplevel .tempmsg -bg bisque

  wm geometry .tempmsg +[expr $x + 5]+[expr $y - 30]
  wm transient .tempmsg .
  wm overrideredirect .tempmsg 1

  label .tempmsg.l -text $msg -bg bisque -justify left -font boldLabelFont
  pack .tempmsg.l -padx 4 -pady 4
}

proc tkg_hideTempMessage {} {
  catch { destroy .tempmsg }
}

