##
## pane.tcl
## Paned Window management
##
## Paned Window Procs inspired by code by Stephen Uhler @ Sun.
## Thanks to John Ellson (ellson@lucent.com) for bug reports & code ideas.
## Copyright 1996 Jeffrey Hobbs.
##
## jhobbs@cs.uoregon.edu, http://www.cs.uoregon.edu/~jhobbs/
##
## source standard_disclaimer.tcl
## source beer_ware.tcl

##############################################################################
##
## This file contains modifications by Jeffery P. Hansen.  Modifications to
## this file only can be considered public domain.
##

##------------------------------------------------------------------
## PROCEDURE
##	pane
##
## DESCRIPTION
##	paned window management function
##
## METHODS
##
##  pane configure <widget> ?<widget> ...? ?<option> <value>?
##  pane <widget> ?<widget> ...? ?<option> <value>?
##	Sets up the management of the named widgets as paned windows.
##
##	OPTIONS
##	-dynamic	Whether to dynamically resize or to resize only
##			when the user lets go of the handle
##	-orient		Orientation of window to determing tiling.
##			Can be either horizontal (default) or vertical.
##	-parent		A master widget to use for the slaves.
##			Defaults to the parent of the first widget.
##	-handlelook	Options to pass to the handle during 'frame' creation.
##	-handleplace	Options to pass to the handle during 'place'ment.
##			Make sure you know what you're doing.
##
##  pane forget <master> ?<slave> ...?
##	If called without a slave name, it forgets all slaves and removes
##	all handles, otherwise just removes the named slave(s) and redraws.
##
##  pane info <slave>
##	Returns the value of [place info <slave>].
##
##  pane slaves <master>
##	Returns the slaves currently managed by <master>.
##
##  pane master <slave>
##	Returns the master currently managing <slave>.
##
## BEHAVIORAL NOTES
##	pane is a limited interface to paned window management.  Error
##  catching is minimal.  When you add more widgets to an already managed
##  parent, all the fractions are recalculated.  Handles have the name
##  $parent.__h#, and will be created/destroyed automagically.  You must
##  use 'pane forget $parent' to clean up what 'pane' creates, otherwise
##  critical state info about the parent pane will not be deleted.  This
##  could support -before/after without too much effort if the desire
##  was there.  Because this uses 'place', you have to take the same care
##  to size the parent yourself.
##
## VERSION 0.92
##
## EXAMPLES AT END OF FILE
##

proc pane {opt args} {
    global PANE

    switch -glob -- $opt {
	co* { eval pane_config $args }
	cg* { eval pane_cget $args }
	f* {
	    set p [lindex $args 0]
	    if {[info exists PANE($p,w)]} {
		if {[llength $args]==1} {
		    foreach w $PANE($p,w) { catch {place forget $w} }
		    foreach w [array names PANE $p,*] { unset PANE($w) }
		    if {![catch {winfo children $p} kids]} {
			foreach w $kids {
			    if {[string match *.__h* $w]} { destroy $w }
			}
		    }
		} else {
		    foreach w [lrange $args 1 end] {
			place forget $w
			set i [lsearch -exact $PANE($p,w) $w]
			set PANE($p,w) [lreplace $PANE($p,w) $i $i]
		    }
		    if [llength $PANE($p,w)] {
			eval pane_config $PANE($p,w)
		    } else {
			pane forget $p
		    }
		}
	    } else {
		
	    }
	}
	i* { return [place info $args] }
	s* {
	    if {[info exists PANE($args,w)]} {
		return $PANE($args,w)
	    } {
		return {}
	    }
	}
	m* {
	    foreach w [array names PANE *,w] {
		if {[lsearch $PANE($w) $args] != -1} {
		    regexp {([^,]*),w} $w . res
		    return $res
		}
	    }
	    return -code error \
		    "no master found. perhaps $args is not a pane slave?"
	}
	default { eval pane_config [list $opt] $args }
    }
}

##
## PRIVATE FUNCTIONS
##
## I don't advise playing with these because they are slapped together
## and delicate.  I don't recommend calling them directly either.
##

;proc pane_cget args {
  global PANE

  set value ""
  set p [lindex $args 0]

  if {$PANE($p,o) == "horizontal" } {
    set target "-relwidth"
  } else {
    set target "-relheight"
  }

  switch -glob -- [lindex $args 1] {
    -f* {
      set value {}
      foreach w $PANE($p,w) {
	foreach {opt val} [place info $w] {
	  if { $opt == $target } {
	    lappend value $val
	  }
	}
      }
    }
  }

  

  return $value
}


;proc pane_config args {
    global PANE

    array set opt {orn none par {} dyn 0 hpl {} hlk {} }
    set wids {}
    for {set i 0;set num [llength $args];set cargs {}} {$i<$num} {incr i} {
	set arg [lindex $args $i]
	if [winfo exists $arg] { lappend wids $arg; continue }
	set val [lindex $args [incr i]]
	switch -glob -- $arg {
	    -dy*	{ set opt(dyn) [regexp -nocase {^(1|yes|true|on)$} $val] }
	    -o*		{ set opt(orn) $val }
	    -p*		{ set opt(par) $val }
	    -handlep*	{ set opt(hpl) $val }
	    -handlel*	{ set opt(hlk) $val }
	    -dec*	{ set opt(dec) $val }
	    -f*         { set opt(frc) $val } 
	    default	{ return -code error "unknown option \"$arg\"" }
	}
    }
    if {[string match {} $wids]} {
	return -code error "no widgets specified to configure"
    }
    if {[string compare {} $opt(par)]} {
	set p $opt(par)
    } else {
	set p [winfo parent [lindex $wids 0]]
    }
    if {[string match none $opt(orn)]} {
	if {![info exists PANE($p,o)]} { set PANE($p,o) h }
    } else {
	set PANE($p,o) $opt(orn)
    }
    if {[string match h* $PANE($p,o)]} {
	set owh height; set wh width; set xy x; set hv h
    } else {
	set owh width; set wh height; set xy y; set hv v
    }
    if ![info exists PANE($p,w)] { set PANE($p,w) {} }
    foreach w [winfo children $p] {
	if {[string match *.__h* $w]} { destroy $w }
    }
    foreach w $wids {
	set i [lsearch -exact $PANE($p,w) $w]
	if {$i<0} { lappend PANE($p,w) $w }
    }
    set ll [llength $PANE($p,w)]
    set frac [expr {1.0/$ll}]
    set pos 0.0
    array set hndconf $opt(hlk)
    if {![info exists hndconf(-$wh)]} {
	set hndconf(-$wh) 4
    }

    set index 0
    foreach w $PANE($p,w) {
      if {[info exists opt(frc)]} {
	set frac [lindex $opt(frc) $index]
	incr index
      }

      place forget $w
      place $w -in $p -rel$owh 1 -rel$xy $pos -$wh -$hndconf(-$wh) \
	  -rel$wh $frac -anchor nw
      raise $w
      set pos [expr $pos+$frac]
    }
    place $w -$wh 0

  set index 0
  while {[incr ll -1]} {
    if {[info exists opt(frc)]} {
      set frac [lindex $opt(frc) $index]
      incr index
    }

    set h [eval frame [list $p.__h$ll] -bd 2 -relief sunken \
	       -cursor sb_${hv}_double_arrow [array get hndconf]]
    eval place [list $h] -rel$owh 1 -rel$xy [expr $frac*$ll] \
	-$xy -$hndconf(-$wh) -anchor nw $opt(hpl)
    raise $h
    bind $h <ButtonPress-1> "pane_constrain $p $h \
		[lindex $PANE($p,w) [expr $ll-1]] [lindex $PANE($p,w) $ll] \
		$wh $xy $opt(dyn)"

    if {[info exists opt(dec)]} {
      $opt(dec) $h -orient $opt(orn)

      foreach hc [winfo children $h] {
	bind $hc <ButtonPress-1> "pane_constrain $p $h \
		[lindex $PANE($p,w) [expr $ll-1]] [lindex $PANE($p,w) $ll] \
		$wh $xy $opt(dyn) $hc"
      }

    }
  }
}

;proc pane_constrain {p h w0 w1 wh xy d args} {
    global PANE


    if { $args != "" } {
      set hc $args
    } else {
      set hc $h
    }

    regexp -- "\-rel$xy (\[^ \]+)" [place info $w0] junk t0
    regexp -- "\-rel$xy (\[^ \]+).*\-rel$wh (\[^ \]+)" \
	    [place info $w1] junk t1 t2
    set offset [expr ($t1+$t2-$t0)/10.0]

    set psize [winfo $wh $p]
#original constraints
#    array set PANE [list XY [winfo root$xy $p] WH [winfo $wh $p].0 \
	    W0 $w0 W1 $w1 XY0 $t0 XY1 [expr $t1+$t2] \
	    C0 [expr $t0+$offset] C1 [expr $t1+$t2-$offset]]

# jph - modified constraint
    array set PANE [list XY [winfo root$xy $p] WH [winfo $wh $p].0 \
	    W0 $w0 W1 $w1 XY0 $t0 XY1 [expr $t1+$t2] \
	    C0 [expr $t0+10.0/$psize] C1 [expr $t1+$t2]]


    bind $hc <B1-Motion> "pane_motion %[string toup $xy] $p $h $wh $xy $d"
    if !$d {
	bind $hc <ButtonRelease-1> \
		"pane_motion %[string toup $xy] $p $h $wh $xy 1"
    }
}

;proc pane_motion {X p h wh xy d} {
  global PANE
  set f [expr ($X-$PANE(XY))/$PANE(WH)]
  if {$f<$PANE(C0)} { set f $PANE(C0)}
  if {$f>$PANE(C1)} { set f $PANE(C1)}
  if $d {
    place $PANE(W0) -rel$wh [expr $f-$PANE(XY0)]
    place $h -rel$xy $f
    place $PANE(W1) -rel$wh [expr $PANE(XY1)-$f] -rel$xy $f

  } else {
    place $h -rel$xy $f
  }
}

##
## EXAMPLES
##
## These auto-generate for the plugin
##
if [info exists embed_args] {
    ## Hey, super-pane the one toplevel we get!
    pane [frame .0] [frame .1]
    ## Use the line below for a good non-plugin example
    #toplevel .0; toplevel .1
    pane [listbox .0.0] [listbox .0.1] -dynamic 1
    pane [frame .1.0] [frame .1.1] -dyn 1
    pane [listbox .1.0.0] [listbox .1.0.1] [listbox .1.0.2] -orient vertical
    pack [label .1.1.0 -text "Text widget:"] -fill x
    pack [text .1.1.1] -fill both -expand 1
    set i [info procs]
    foreach w {.0.0 .0.1 .1.0.0 .1.0.1 .1.0.2 .1.1.1} { eval $w insert end $i }
}
##
## END EXAMPLES
##
## EOF
