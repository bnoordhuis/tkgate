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

#declare R = 0.25;
#declare R2 = 0.6;

#declare ArrowButton =
  union {
   object { cylinder { -0.65*x, 0*x, R } }
   object { cone { <0,0,0>, R2, <0.6,0,0>, 0 }}
   texture { MyAluminum3 }
  }


union {
  object { ArrowButton rotate 90*z }
}

//
// Where are we anyway?
//
camera {
  location  <0, 0, -20>		// Full view
  look_at <0,0,0>
}

//
// Let there be light!
//
light_source { <-10.0, 10.0, -10> colour White*1.5 }
light_source { <10, -20, -10> colour White*1 }
