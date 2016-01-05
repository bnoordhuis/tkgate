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

#declare Pi = 3.1415926;


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

#declare Gold_Text =
  texture {
    pigment { rgb <0.8, 0.7, 0.05>  }
    finish {
      metallic
      specular 0.3
//      roughness 0.1
      ambient 0.5
      diffuse 0.2
      reflection .2
    }
  }

#declare Inard_Text =
  texture {
    pigment { rgb <0.3, 0.3, 0.05>  }
    finish {

      metallic
      specular 0.05
//      roughness 0.1
      ambient 0.3
      diffuse 0.2
//      reflection .2
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
      ambient 0.2
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
    object { cylinder { 0.0*z, 0.3*z, 0.5 }  }
    object { cylinder { z, -z, 0.35 }  }
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
    box { <0.0,0.45,0.0>, <0.25,0.6,0.3> }
    box { <0.0,0.0,0.0>, <0.25,0.15,0.3> }

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

#macro Gear(X,Y,R,N,A)
  union {
    #local T = 0.08;
    cylinder { 0*x, T*x, R }
    #local i = 0;
    #local w = (1.0/N)*Pi*R;
    #while (i < N)
      #local a = i*360.0/N;
      object { box { <0,0,-w/2>, <1*T,w,w/2> } rotate 0*x translate R*y rotate a*x}
      #local i = i + 1;
    #end
    rotate A*360.0/N*x
    translate <0, X, Y>
    texture { Gold_Text } 
  }
#end


#declare Inards =
  union {
    Gear(0.14, 0.16, 0.03, 10, 0.5)
    Gear(0.12, 0.09, 0.03, 10, 0)
    Gear(0.03, 0.1, 0.05, 14, 0.25)
  } 

#declare Circuit =
  union {
    #local W = 0.75;
    #local H = 1.0;
    #local D = 0.1;
    #local T = 0.1;

    union {
      intersection {
        object { OrGate translate <0,-0.3,0> texture { Gate_Text }}
        object { box { <0.07,0,0>, <0.1,0.2,0.2> inverse rotate 45*z translate <.6, 0.02, 0.05> } texture { Inard_Text } }
      }
      object { Inards  rotate 45*z translate <.6, 0.02, 0.05> } 

      // Outputs
//      cylinder { 0*x, 0.25*x, 0.035  translate <0.83,0,0.05> texture { Wire_Text }} 

      // Inputs
//      cylinder { 0*x, 0.25*x, 0.035  translate <-.25,-0.1,0.05> texture { Wire_Text }} 
//      cylinder { 0*x, 0.25*x, 0.035  translate <-.25,0.1,0.05> texture { Wire_Text }} 
    }
  }

union {
  object { Circuit rotate 50*y}
  translate <0,0.1,0>
}


//#declare CV = 0.85098039215686274509;
#declare CV = 1;

  sky_sphere {
    pigment {
//      colour red 0.8 green 0.8 blue 0.8
//      colour red 1 green 1 blue 1
      colour red CV green CV blue CV
    }
  }


//
// Where are we anyway?
//
camera {
  location  <0.5, 0.6, -1.75>
//  location  <0.6, 0.6, -0.7>
  look_at <0, 0,  0>
}

light_source {
  <-1.0, 30.0, -10>
  colour White *3
}
