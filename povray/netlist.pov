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
//      metallic
      specular 0.15
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
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
      object { NOrGate translate <0.3,0.7,0> texture { Gate_Text }}
      object { NOrGate translate <0.3,-0.7,0> texture { Gate_Text }}

      // Outputs
      box { <0, -0.05, 0.001>, <1.2, 0.05, 0.05> translate <1.13,1,0> texture { Bus_Text }} 
      box { <0, -0.05, 0.001>, <1.2, 0.05, 0.05> translate <1.13,-.4,0> texture { Bus_Text }} 

      // Bus inputs
      box { <0, -0.05, 0.001>, <0.5, 0.05, 0.05> translate <-.2,0.9,0> texture { Bus_Text }} 
      box { <0, -0.05, 0.001>, <0.5, 0.05, 0.05> translate <-.2,-.3,0> texture { Bus_Text }} 

      // Bus crossing
      box { <-0.05, -0.05, 0.001>, <1.25, 0.05, 0.05> rotate 90*z translate <-.2,-.3,0> texture { Bus_Text }} 

      // Bus feed wire
      box { <1, -0.05, 0.001>, <2, 0.05, 0.05> translate <-2.2,0.3,0> texture { Bus_Text }} 

      // 1-bit inputs
      box { <0, -0.035, 0.001>, <0.5, 0.035, 0.035> translate <-.2,1.1,0> texture { Gate_Text }} 
      box { <1, -0.0355, 0.001>, <2.5, 0.035, 0.035> translate <-2.2,-.5,0> texture { Gate_Text }} 
      box { <0, -0.035, 0.001>, <0.5, 0.035, 0.035> rotate 90*z translate <-.2,1.1-0.035,0> texture { Gate_Text }} 


      union {
        starttext(0.4, 0.5, 0.03, 0.15)
        putchar(char_G)
        putchar(char_4)
        putchar(char_2)
	texture { Gate_Text }
      }

      union {
        starttext(0.4, -0.9, 0.03, 0.15)
        putchar(char_G)
        putchar(char_3)
        putchar(char_5)
	texture { Gate_Text }
      }

      union {
        starttext(1.4, -0.65, 0.03, 0.15)
        putchar(char_W)
        putchar(char_6)
        putchar(char_1)
	texture { Bus_Text }
      }

      union {
        starttext(1.4, 0.75, 0.03, 0.15)
        putchar(char_W)
        putchar(char_8)
        putchar(char_7)
	texture { Bus_Text }
      }


      rotate 90*x
    }

    #local i = 0;
    #local PS = W - 0.05;
    #local Q = 0.3;
    #local PW = 0.05;
    #local PD = 0.05;
    #local L = 0.7;
    #while (i <= 3)
      #local Z = -1 + Q + (i/3.0)*(2-2*Q);
//      box { <-PS,-PD,Z-PW>, <-(PS+L),PD,Z+PW> texture { Pin_Text } }
//      box { <PS,-PD,Z-PW>, <PS+L,PD,Z+PW> texture { Pin_Text } }

      #local i = i + 1;
    #end
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
