//
// Gate Logo
// by Jeffery P. Hansen
//

//global_settings { assumed_gamma 2.2 }

#include "shapes.inc"
#include "chars.inc"
#include "colors.inc"
#include "textures.inc"
#include "woods.inc"
#include "stones.inc"

#macro starttext(xpos,ypos,size,space)
  #declare CP_X = xpos;
  #declare CP_Y = ypos;
  #declare CP_LEFT = xpos;
  #declare CP_SPACE = space;
  #declare CP_SIZE = size;
#end

#macro newline()
  #declare CP_X = CP_LEFT;
  #declare CP_Y = CP_Y - (BoardHeight+BoardGap);
#end

#macro kern(C)
  #declare CP_X = CP_X + CP_SPACE*C;
#end

#macro putchar(C)
    object {
	C scale <CP_SIZE,CP_SIZE,CP_SIZE>
	translate <CP_X,CP_Y,-.01> 
    }

  #declare CP_X = CP_X + CP_SPACE ;
#end

#declare Gate_Text =
  texture {
    pigment { rgb <0.05, 0.05, 0.6>  }
    finish {
      specular 0.15
      ambient 0.5
    }
  }

#declare Text_Text =
  texture {
    pigment { rgb <0.05, 0.05, 0.05>  }
    finish {
      specular 0.15
      ambient 0.5
    }
  }

#declare Bus_Text =
  texture {
    pigment { rgb <0.6, 0.05, 0.6>  }
    finish {
//      metallic
      specular 0.15
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare Pin_Text =
  texture {
    pigment { rgb <1,0,0>  }
    finish {
      metallic
      specular 1
//      roughness 0.1
//      ambient 0.2
//      diffuse 0.2
      reflection .2
    }
  }

#declare Paper_Text =
  texture {
    pigment { rgb <1,0.95,0.95>  }
    finish {
      specular 0.1
      roughness 0.2
      ambient 0.2
      diffuse 0.2
    }
  }

#declare Paper =
  union {
    #local W = 1.5;
    #local H = 2;
    #local D = 0.01;

    box { <-W,-H,-D>, <W, H, 0> }

    texture { Paper_Text }
  }

#declare Desk =
  union {
    #local W = 4;
    #local H = 4;
    #local D = 1;

    box { <-W,-H,-D>, <W, H, 0> }

    texture { T_Grnt21 }
  }

//
// Basic arc (actually a ring) that is used as parts of the OR gate.
//
#declare GateArc=
  difference {
    object { cylinder { 0.0*z, 0.1*z, 0.5 }  }
    object { cylinder { z, -z, 0.4 }  }
  }

//
// Back arc of an OR gate
//
#declare OrBack = 
  intersection {
    object { GateArc }
    plane {x, 0 inverse }
    plane {y, -0.3 inverse }
    plane {y, 0.3  }

    translate <-0.4,0.3,0.0>
  }

//
// Basic OR gate.
//
#declare OrGate = 
  union {
    box { <0.0,0.5,0.0>, <0.25,0.6,0.1> }
    box { <0.0,0.0,0.0>, <0.25,0.1,0.1> }

    //
    // The back of the OR gate
    //
    object { OrBack }

    //
    // Top-half of OR curve.
    //
    intersection {
      intersection {
	object { GateArc  }
        plane {x, 0 inverse }
        translate <0.25,0.1,0.0>
      }
      plane {y, 0.3 inverse }
    }

    //
    // Bottom-half of OR curve.
    //
    intersection {
      intersection {
	object { GateArc  }
        plane {x, 0 inverse }
        translate <0.25,0.5,0.0>
      }
      plane {y,0.3 }
    }
  }

#declare NOrGate = 
  union {
//    box { <0,0,0>, <0.1,0.05,0.05> translate <0.9,0.275,0>}
//    box { <0,0,0>, <0.1,0.05,0.05> translate <-0.1,0.45,0>}
//    box { <0,0,0>, <0.1,0.05,0.05> translate <-0.1,0.1,0>}

    object { OrGate }
      difference {
        object { cylinder { 0.0*z, 0.1*z, 0.15 }  }
        object { cylinder { z, -z, 0.05 }  }
	translate <0.75, 0.3, 0>
    }
  }

#declare Circuit =
  union {
    #local W = 0.75;
    #local H = 1.0;
    #local D = 0.1;
    #local T = 0.1;

    union {
      starttext(-.9, 1.3, 0.03, 0.2)
      putchar(char_M)
      putchar(char_O)
      putchar(char_D)
      putchar(char_U)
      putchar(char_L)
      putchar(char_E)
      kern(1)
      putchar(char_M)
      putchar(char_A)
      putchar(char_I)
      putchar(char_N)
      texture { Text_Text }
      rotate 90*x
    }

    union {
      starttext(-.6, 1, 0.03, 0.2)
      putchar(char_R)
      putchar(char_E)
      putchar(char_G)
      kern(1)
      putchar(char_LSqu)
      putchar(char_7)
      putchar(char_Perc)
      putchar(char_0)
      putchar(char_RSqu)
      kern(1)
      putchar(char_A)
      kern(0.5)
      putchar(char_B)
      kern(0.5)
      putchar(char_C)
      kern(1)
      texture { Text_Text }
      rotate 90*x
    }
    union {
      starttext(-.6, 0.7, 0.03, 0.2)
      putchar(char_I)
      putchar(char_N)
      putchar(char_I)
      putchar(char_T)
      putchar(char_I)
      putchar(char_A)
      putchar(char_L)
      texture { Text_Text }
      rotate 90*x
    }

    union {
      starttext(-.3, 0.4, 0.03, 0.2)
      putchar(char_B)
      putchar(char_E)
      putchar(char_G)
      putchar(char_I)
      putchar(char_N)
      texture { Text_Text }
      rotate 90*x
    }
    union {
      starttext(0, 0.1, 0.03, 0.2)
      putchar(char_A)
      putchar(char_Perc)
      putchar(char_B)
      putchar(char_Astr)
      putchar(char_C)
      texture { Text_Text }
      rotate 90*x
    }
    union {
      starttext(0, -0.2, 0.03, 0.2)
      putchar(char_B)
      putchar(char_Perc)
      putchar(char_A)
      putchar(char_Astr)
      putchar(char_C)
      texture { Text_Text }
      rotate 90*x
    }

    union {
      starttext(-.3, -0.5, 0.03, 0.2)
      putchar(char_E)
      putchar(char_N)
      putchar(char_D)
      texture { Text_Text }
      rotate 90*x
    }
    union {
      starttext(-.9, -0.8, 0.03, 0.2)
      putchar(char_E)
      putchar(char_N)
      putchar(char_D)
      putchar(char_M)
      putchar(char_O)
      putchar(char_D)
      putchar(char_U)
      putchar(char_L)
      putchar(char_E)
      texture { Text_Text }
      rotate 90*x
    }
  }

union {
  object { Circuit translate 0.01*y }
  object { Paper rotate x*-90 rotate y*90 translate <0.5,0,0.2> }
  object { Desk rotate x*-90 rotate y*90 translate <0.5,-0.01,0.2> }
    rotate x*-90
    rotate y*40
    rotate z*-20
    translate -0.5*x
}

//  sky_sphere {
//    pigment {
//      colour red 0.8 green 0.8 blue 0.8
//    }
//  }



//
// Where are we anyway?
//
camera {
  location  <0.2, 0.0, -14>
  look_at <0.2, 0.0,  0.0>
}

light_source {
  <-30.0, 30.0, -30>
  colour White *3
}
