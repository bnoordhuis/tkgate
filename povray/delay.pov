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

#declare LensGlass = 
texture {
    pigment { rgbf<1.0, 1.0, 1.0, 0.95> }
    finish {
      specular 1
      roughness 0.001
      ambient 0
      diffuse 0
      reflection 0.1
      ior 1.5
  }
}

#declare Ink_Text =
  texture {
    pigment { rgb <0.05, 0.05, 0.05>  }
    finish {
      specular 0.0
      ambient 0.0
    }
  }

#declare Gold_Text =
  texture {
    pigment { rgb <0.6, 0.5, 0.05>  }
    finish {
      metallic
      specular 0.3
//      roughness 0.1
      ambient 0.5
      diffuse 0.2
      reflection .2
    }
  }

#declare BugHead_Text =
  texture {
    pigment { rgb <0.05, 0.1, 0.05>  }
    finish {
//      metallic
      specular 0.15
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare Wire_Text =
  texture {
    pigment { rgb <0.05, 0.6, 0.05>  }
    finish {
//      metallic
      specular 0.15
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare BugShell_Text =
  texture {

    pigment { gradient z scale .03
	color_map {
	    [0.0, 1 color rgb <0.05, 0.2, 0.05>
	              color rgb <0.05, 0.6, 0.05>] }}

//    pigment {  }
    finish {
      metallic
      specular 0.2
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
      roughness 0.1
      ambient 0.8
      diffuse 0.2
    }
  }

#declare Paper =
  union {
    #local W = 0.5;
    #local H = 0.7;
    #local D = 0.01;

    box { <-W,-H,-D>, <W, H, D> }

    texture { Paper_Text }
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
      box { <0, -0.05, 0.001>, <2, 0.05, 0.05> translate <1.13,1,0> texture { Bus_Text }} 
      box { <0, -0.05, 0.001>, <2, 0.05, 0.05> translate <1.13,-.4,0> texture { Bus_Text }} 

      // Bus inputs
      box { <0, -0.05, 0.001>, <0.5, 0.05, 0.05> translate <-.2,0.9,0> texture { Bus_Text }} 
      box { <0, -0.05, 0.001>, <0.5, 0.05, 0.05> translate <-.2,-.3,0> texture { Bus_Text }} 

      // Bus crossing
      box { <0, -0.05, 0.001>, <2, 0.05, 0.05> translate <-2.2,0.3,0> texture { Bus_Text }} 

      // Bus feed wire
      box { <-0.05, -0.05, 0.001>, <1.25, 0.05, 0.05> rotate 90*z translate <-.2,-.3,0> texture { Bus_Text }} 

      // 1-bit inputs
      box { <0, -0.035, 0.001>, <0.5, 0.035, 0.035> translate <-.2,1.1,0> texture { Wire_Text }} 
      box { <0, -0.0355, 0.001>, <2.5, 0.035, 0.035> translate <-2.2,-.5,0> texture { Wire_Text }} 
      box { <0, -0.035, 0.001>, <0.5, 0.035, 0.035> rotate 90*z translate <-.2,1.1-0.035,0> texture { Wire_Text }} 


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


#declare Rim_Texture = 
texture {
    pigment { rgb <0.658824, 0.658824, 0.658824> }
    finish {
        ambient 0.3
        diffuse 0.7
        reflection 0.15
        brilliance 6
        specular 0.8
        roughness 0.1
    }
}

#macro Put1Number(R,B,A,HH,C)
    union {
      starttext(0, 0, 0.03, 0.15)
      putchar(C)
      translate -HH*y

      rotate 90*x
      rotate -A*y
      translate B*y
      translate R*z
      rotate A*y
    }
#end

#macro Put2Number(R,B,A,HH,C1,C2)
    union {
      starttext(0, 0, 0.03, 0.15)
      kern(-0.4) 
      putchar(C1)
      putchar(C2)
      translate -HH*y

      rotate 90*x
      rotate -A*y
      translate B*y
      translate R*z
      rotate A*y
    }
#end

#macro Numbers(R,B)
  union {
    #local HH = 0.07;

    Put2Number(R,B,0,HH,char_1,char_2)
    Put1Number(R,B,30,HH,char_1)
    Put1Number(R,B,60,HH,char_2)
    Put1Number(R,B,90,HH,char_3)
    Put1Number(R,B,120,HH,char_4)
    Put1Number(R,B,150,HH,char_5)
    Put1Number(R,B,180,HH,char_6)
    Put1Number(R,B,210,HH,char_7)
    Put1Number(R,B,240,HH,char_8)
    Put1Number(R,B,270,HH,char_9)
    Put2Number(R,B,300,HH,char_1,char_0)
    Put2Number(R,B,330,HH,char_1,char_1)

    texture { Ink_Text }
  }
#end

#macro Hand(H,L,A)
  union {
    #local R = 0.05;
    cylinder { (H-0.01)*y, H*y, R }

    box { <-.02,H-0.01,0>,  <.02,H,L> }
    rotate A*y

    texture { Ink_Text }
  }
#end

#macro Lens(R)
   union {
     #local LT = 0.03;
     cylinder { -LT*y, 0*y, R }
     texture { LensGlass }
   }
  
#end

#declare Watch =
  union {
    #local T = 0.1;
    #local R = 1;
    #local IR = 0.95;
    #local B = 0.2*T;

    intersection {
      union {
        cylinder { T*y, -T*y, 1 }
        torus { R, T }
      }
      cylinder { T*(B-0.01)*y, T*1.1*y, R inverse }
      texture { Gold_Text }
    }
    cylinder { 0*y, T*B*y, R texture { Paper_Text } }

    // Center pin
    cylinder { 0*y, 0.1*y, 0.01 texture { Ink_Text } }

    Hand(0.1, 0.8, 65)
    Hand(0.1, 0.6, 272)
    Numbers(0.85*R,B)

    object { Circuit scale 0.15 translate <0,1.1*T*B,-.4> }


    object { Lens(R) translate (T-0.05)*y }
  }

union {
  object { Watch }
  rotate -50*x
}


#declare CV = 0.85098039215686274509;
sky_sphere {
  pigment {
    colour red CV green CV blue CV
  }
}


//
// Where are we anyway?
//
camera {
  location  <2, 0.0, -4>
  look_at <0, 0.0,  0.0>
}

light_source {
  <-10.0, 10.0, -30>
  colour White *3
}
