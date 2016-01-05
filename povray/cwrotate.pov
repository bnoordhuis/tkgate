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
#declare MyAluminum1 =
    texture {
	Chrome_Metal
	normal {
	   bumps -0.4
	   scale <1, 0.001, 0.001>
	}
     }

//
// Based on Brushed_Alumium with slightly smaller bumps
//
#declare MyAluminum2 =
    texture {
	Chrome_Metal
	normal {
	   bumps -0.1
	   scale <1, 0.001, 0.001>
	}
     }

//
// Based on Brushed_Alumium with slightly smaller bumps
//
#declare MyAluminum3 =
texture {
    pigment { rgb <0.458824, 0.458824, 0.858824> }
    finish {
        ambient 0.3
        diffuse 0.7
        reflection 0.05
        brilliance 8
        specular 0.5
        roughness 0.1
    }
    normal {
	   bumps -0.1
	   scale <1, 0.001, 0.001>
    }
}

#declare R = 1;
#declare RI = 0.33;
#declare RC = 0.7;

#declare AA = 45;
#declare AD = 0.5;

#declare A = 30;

#declare RotateButton =
  union {
    intersection {
      torus { R, RI rotate 90*x } 
      union {
        plane { x,0 rotate (90-A)*z   }
        plane { x,0 rotate (-90+A)*z  }
      }	
//	plane {z, -1 inverse}
    }
    object { cone { <0,0,0>, RC, <1,0,0>, 0 }  rotate z*-90 translate R*x rotate z*A }
    texture { MyAluminum3 }
  }


union {
  object { RotateButton translate 0*x }
}

//
// Where are we anyway?
//
camera {
  location  <2, -2, -44>		// Full view
  look_at <0,0,0>
}

//
// Let there be light!
//
light_source { <-50.0, 50.0, -10> colour White*1 }
light_source { <-10, -10, -20> colour White*1 }
