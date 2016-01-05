//
// Gate Logo
// by Jeffery P. Hansen
//

//global_settings { assumed_gamma 2.2 }

#include "shapes.inc"
#include "chars.inc"
#include "colors.inc"
#include "textures.inc"

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

#declare Text_Text =
  texture {
    pigment { rgb <0.05, 0.05, 0.05>  }
    finish {
      specular 0.15
      ambient 0.5
    }
  }

#declare Chip_Text =
  texture {
    pigment { rgb <.6,0,.6>  }
    finish {
//      metallic
      specular 0.2
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
      specular 0.01
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

    union {    
      union {
        object { text { ttf "cyrvetic.ttf" "Name: add8" 0.03, 0 } scale .3 }
	translate <-.9, 1.3,-0.02>

      }
      union {
        object { text { ttf "cyrvetic.ttf" "Pos:  (95,74)" 0.03, 0 } scale .3 }
	translate <-.9, 1,-0.02>
      }
      union {
        object { text { ttf "cyrvetic.ttf" "Hide:  Yes" 0.03, 0 } scale .3 }
	translate <-.9, 0.7,-0.02>
      }
      union {
        object { text { ttf "cyrvetic.ttf" "Anch:  No" 0.03, 0 } scale .3 }
	translate <-.9, 0.4,-0.02>
      }

      scale 0.4
      translate -0.01*z
      texture { Text_Text }
    }
    texture { Paper_Text }
  }

#declare Chip =
  union {
    #local W = 0.75;
    #local H = 1.0;
    #local D = 0.1;
    #local T = 0.1;

    intersection {
      box { <-W,-D,-H>, <W, D, H> }
      box { <-(W-T),-2*D,-(H-T)>, <W-T, 2*D, H-T> inverse }
      
      texture { Chip_Text }
    }

    #local i = 0;
    #local PS = W - 0.05;
    #local Q = 0.3;
    #local PW = 0.05;
    #local PD = 0.05;
    #local L = 0.7;
    #while (i <= 3)
      #local Z = -1 + Q + (i/3.0)*(2-2*Q);
      box { <-PS,-PD,Z-PW>, <-(PS+L),PD,Z+PW> texture { Pin_Text } }
      box { <PS,-PD,Z-PW>, <PS+L,PD,Z+PW> texture { Pin_Text } }

      #local i = i + 1;
    #end

    rotate x*-90
    rotate y*40
    rotate z*-20
  }


object { Chip }
object { Paper }

//#declare CV = 0.85098039215686274509;
#declare CV = 1;

  sky_sphere {
    pigment {
      colour red CV green CV blue CV
    }
  }


//
// Where are we anyway?
//
camera {
  location  <0.0, 0.0, -5.8>
  look_at <0.0, 0.0,  0.0>
}

light_source {
  <-20.0, 10.0, -30>
  colour White *5
}
