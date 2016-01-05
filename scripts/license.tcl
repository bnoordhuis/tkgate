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
# Last edit by hansen on Wed Mar 30 00:19:59 2005
#

proc showDocFile {label file L} {
  global tkg_gateHome

  set w .docwin
  set i 0
  while { [catch { toplevel $w$i}] } {
    incr i
  }
  set w $w$i

  wm resizable $w 0 0
  wm title $w $label
  wm transient $w .


  button $w.dismiss -text [m b.dismiss] -command "destroy $w"
  scrollbar $w.scroll -command "$w.text yview"
  text $w.text -yscrollcommand "$w.scroll set"

  pack $w.dismiss -side bottom -fill x
  pack $w.text $w.scroll -side left -fill y

  foreach s $L {
    $w.text insert end "$s\n"
  }

  if { $file != "" } {
    set f [open $tkg_gateHome/doc/$file]
    $w.text insert end [read $f]
    close $f
  }
  $w.text configure -state disabled
}

proc showLicense {} {
    global bd tkg_progVer tkg_copyright tkg_mailContact tkg_description

    set L {}
    lappend L "    TKGate $tkg_progVer - $tkg_description"
    lappend L ""
    lappend L "    $tkg_copyright"
    lappend L ""

    showDocFile "TkGate License" "license.txt" $L
}

proc showDocumentation {} {
  global bd tkg_progVer tkg_copyright tkg_mailContact tkg_description tkg_homepage tkg_localdoc

  gat_dohyperlink $tkg_localdoc

  if {0} {
    set L {}
    lappend L "TKGate $tkg_progVer - $tkg_description"
    lappend L ""
    lappend L "$tkg_copyright"
    lappend L ""
    lappend L "Currently there is no built-in documentation for TkGate other than the"
    lappend L "balloon help which can be activated by placing the mouse cursor over"
    lappend L "interface elements, and the online tutorials availble. Documentation"
    lappend L "can be found by pointing your Web browser at the TkGate home page:"
    lappend L ""
    lappend L "   $tkg_homepage"
    lappend L ""
    lappend L "or through the copy of the documentation included in this distribution at:"
    lappend L ""
    lappend L "   $tkg_localdoc"

    showDocFile "TkGate Documentation" "" $L
  }
}

proc showAbout {} {
  global bd tkg_progVer tkg_copyright tkg_mailContact tkg_description

  if { [catch { toplevel .about}] } {
    catch { raise .about }
    return;
  }
  wm title .about "About TkGate $tkg_progVer" 

  button .about.dismiss -text [m b.dismiss] -command "destroy .about"
  label .about.logo -relief groove -image [gifI biggatelogo.gif]
  label .about.label -text "TKGate $tkg_progVer - $tkg_description\n$tkg_copyright\n$tkg_mailContact"

  pack .about.logo -padx 10 -pady 10  -ipadx 10 -ipady 10
  pack .about.label -padx 10 -pady 10
  pack .about.dismiss -padx 10 -pady 10
}
