//
// Gate Logo
// by Jeffery P. Hansen
//

//global_settings { assumed_gamma 2.2 }

#include "shapes.inc"
#include "chars.inc"
#include "colors.inc"
#include "textures.inc"
#include "metals.inc"


//
// The sky
//
#declare CV = 0.85098039215686274509;
  sky_sphere {
    pigment {
      colour red CV green CV blue CV
    }
  }

//
// Based on Brushed_Alumium with slightly smaller bumps
//
#declare MyAluminum =
texture {
    pigment { rgb <0.7, 0.7, 0.7> }
    finish {
        ambient 0.3
        diffuse 0.7
        reflection 0.1
        brilliance 1
        specular 0.1
        roughness 0
    }
    normal {
	   bumps -0.1
	   scale <1, 0.001, 0.001>
    }
}




//
//
#declare Rubber =
texture {
    pigment { rgb <0.458824, 0.458824, 0.458824> }
    finish {
        ambient 0.3
        diffuse 0.7
        reflection 0.05
        brilliance 3
        specular 0.2
        roughness 0.1
    }
    normal {
	   bumps -0.1
	   scale <1, 0.1, 0.1>
    }
}


#declare R = 1;
#declare RI = 0.43;
#declare RC = 0.7;
#declare RIC = 0.2;

#declare RotateButton =
  union {
    torus { R, RI rotate 90*x     texture { Rubber } } 
    cone { <0,0,-0.4>, RIC, <0,0,-.3>, RC      texture { MyAluminum }}
    torus { RIC, 0.05 rotate 90*x translate -0.4*z     texture { Rubber } } 

  }


union {
  object { RotateButton translate 0*x rotate 65*y}
}

//
// Where are we anyway?
//
camera {
  location  <0, 0, -44>		// Full view
  look_at <0,0,0>
}

//
// Let there be light!
//
light_source { <-50.0, 50.0, -10> colour White*1 }
light_source { <-10, -10, -20> colour White*1 }
