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

#declare Paint_Finish =
  finish {
    specular 0.5
    reflection 0.2
  }

#macro ColorDab(X,Y,C)
    sphere { <0,0,0>, PR
	scale <1,PH,1>
	translate <X,0,Y>
	texture { 
	  pigment { rgb C }
	  finish { Paint_Finish }
	}
    }
#end

#declare Palate =
  union {
    #local W = 0.75;
    #local H = 1.0;
    #local D = 0.05;
    #local T = 0.1;
    #local PR = 0.15;
    #local PH = 0.2;

    intersection {
      cylinder { -D*y, 0*y, 1 scale <W,1,H> }
      cylinder { -1*y, 1*y, 0.15 translate -0.7*z inverse }
      
      texture { T_Wood8 }
    }


    ColorDab(0.0, -0.1, <1,0,0>)
    ColorDab(0.3,0.1, <0,1,0>)
    ColorDab(-0.3,0.1, <0,0,1>)
    ColorDab(-0.4,-0.3, <0,0,0>)
    ColorDab(0.4,-0.3, <1,1,1>)

    ColorDab(-0.3,0.4, <0,1,1>)
    ColorDab(0.32,0.45, <1,0,1>)
    ColorDab(0.05,0.6, <1,1,0>)

    rotate x*-40
    rotate y*20
    rotate z*-30
  }


object { Palate }


  sky_sphere {
    pigment {
      colour red 0.8 green 0.8 blue 0.8
    }
  }


//
// Where are we anyway?
//
camera {
  location  <0.0, 0.0, -3>
  look_at <0.0, 0.0,  0.0>
}

light_source {
  <-30.0, 30.0, -30>
  colour White *5
}
