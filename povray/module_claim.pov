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
#include "metals.inc"

#declare Ink_Text =
  texture {
    pigment { rgb <0.05, 0.05, 0.05>  }
    finish {
      specular 0.0
      ambient 0.0
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

#declare Bark_Text =
  texture {
    pigment { 
	wrinkles
	turbulence 2
	color_map {
 	[0 rgb <.2,.1,.05>]
 	[0.4 rgb <.2,.1,.05>]
 	[0.7 rgb <0,0,0>]
 	[1 rgb <0,0,0>]
        }
    }

    normal {
      wrinkles 1
      scale .1
    }
    finish {
      roughness 5
      ambient 0.5
    }
  }

#declare Erase_Text =
  texture {
    pigment { rgb <.6,.3,.3>  }
    finish {
      specular 0
      roughness 0.3
      ambient 0.2
    }
  }

#declare Yellow_Paint =
  texture {
    pigment { rgb <.9,0.9,0.1>  }
    finish {
      specular 0.2
      ambient 0.1
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

#declare Chip =
  union {
    #local W = 0.75;
    #local H = 1.0;
    #local D = 0.1;
    #local T = 0.15;

    intersection {
      box { <-W,-D,-H>, <W, D, H> }
      box { <-(W-T),-2*D,-(H-T)>, <W-T, 2*D, H-T> inverse }
      texture { Chip_Text }
    }
    object { box { <-(W-T),-D,-(H-T)>, <W-T,D,H-T> } texture { Paper_Text }}

    object { text { ttf "cyrvetic.ttf" "ALU32" 0.3, 0 }
	scale 0.3
	rotate 90*x
	translate <-0.5,0.101,0.0>
	texture { Ink_Text }
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
  }

#declare BoardWidth = 1.25;
#declare BoardHeight = 0.5;
#declare BoardThickness = 0.1;
#declare PostWidth = 0.15;
#declare PostHeight = 1.5;

#declare Board =
  difference {
    box { <-BoardWidth/2,0,0>, <BoardWidth/2,BoardHeight,BoardThickness> }
  }

#declare SignBase =
  union {
    object { Board translate (PostHeight-BoardHeight)*y}
    box { <-PostWidth/2,0,0>, <PostWidth/2,PostHeight,PostWidth> translate BoardThickness*z }

    //Other possible woods: 18, 19, 20, 29, 30, 31
    texture { T_Wood18 scale 0.5 }
  } 


/////////////////////////////////////////////////////////////////////////////
//
// Put up the sign base and subtract out all of the letters.
//
/////////////////////////////////////////////////////////////////////////////
#declare Sign =
  union {
    object { SignBase }

    object { text { ttf "cyrvetic.ttf" "Mine!" 0.1, 0 }
	scale 0.4
	translate <-0.4,1.101,-0.01>
	texture { Ink_Text }
    }
  }


union {
  object { Chip }
  object { Sign rotate -30*y rotate -30*x rotate 30*z translate -0.75*y }
  rotate y*40
  rotate z*-20
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
  location  <0.0, 0.0, -5.8>
  look_at <0.0, 0.0,  0.0>
}

light_source {
  <-20.0, 10.0, -30>
  colour White *5
}
