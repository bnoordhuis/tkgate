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
# Last edit by hansen on Mon Feb 28 22:42:44 2005
#
set ho_ReqWin ""
set ho_ReqTag ""
set ho_PostWin ""
set ho_delay 500
set ho_color "bisque"

proc post_help {win tag} {
  global ho_ReqWin ho_ReqTag ho_PostWin ho_color tkg_helpBalloons
  global helpon_msg

  set msg ""
  catch { set msg $helpon_msg($tag) }

  if { $tkg_helpBalloons == 0 } { return }
  if { $win != $ho_ReqWin } { return }
  if { $tag != $ho_ReqTag } { return }
  if { $win == $ho_PostWin } { return }
  catch { destroy .helpon }

  scan [winfo pointerxy .] "%d %d" X Y

  catch {
    bind $win <Destroy> { catch { destroy .helpon } } 

    toplevel .helpon
    wm geometry .helpon +[expr $X + 5]+[expr $Y + 5]
    wm transient .helpon .
    wm overrideredirect .helpon 1
    label .helpon.msg -text $msg -justify left -bg $ho_color
    pack .helpon.msg

    bind .helpon <Visibility> { raise .helpon }

    set ho_PostWin $win
  }
}

proc req_help {win tag} {
  global ho_ReqWin ho_ReqTag ho_delay
  set ho_ReqWin $win
  set ho_ReqTag $tag
  after $ho_delay "post_help $win $tag"
}

proc cancel_help {} {
  global ho_ReqWin ho_ReqTag ho_PostWin
  catch { destroy .helpon }
  set ho_ReqWin ""
  set ho_ReqTag ""
  set ho_PostWin ""
}

proc helpon {win msg} {
  global helpon_msg

  set helpon_msg($win) $msg

  if {$msg == ""} {
     bind $win <Enter> ""
     bind $win <Leave> ""
  } else {
     bind $win <Enter> "+req_help $win $win"
     bind $win <Leave> "+cancel_help"
  }
}

proc helponitemcancel {win item} {
  global helpon_msg
  catch { 
    unset helpon_msg(${win}:$item)
  }
}

proc helponitem {win item msg} {
  global helpon_msg

  set helpon_msg(${win}:$item) $msg

  if {$msg == ""} {
    $win bind $item <Enter> ""
    $win bind $item <Leave> ""
  } else {
    $win bind $item <Enter> "+req_help $win ${win}:$item"
    $win bind $item <Leave> "+cancel_help"
  }
}
