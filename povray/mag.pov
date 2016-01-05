//
// Magnifying Glass
// by Jeffery P. Hansen
//

//global_settings { assumed_gamma 2.2 }

#include "shapes.inc"
#include "colors.inc"
#include "textures.inc"

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


#declare LensGlass = 
texture {
    pigment { rgbf<1.0, 1.0, 1.0, 0.7> }
    finish {
      specular 1
      roughness 0.001
      ambient 0
      diffuse 0
      reflection 0.1
      ior 1.5
  }
}



/* Back wall */
 plane {
    z, 100
    hollow on
    texture { pigment { color red 0.5 green 0.5 blue 1.0 } }
 }


#declare Flat_Enough = 0.01;

union {

   // Rim
   difference {
     intersection {
        object { Cylinder_Z scale <1.0, 1.0, 1.0> }
        plane { z, -0.1 inverse }
        plane { z, 0.1 }
     }
     intersection {
        object { Cylinder_Z scale <0.8, 0.8, 1.0> }
     }
     texture { Rim_Texture }
   }

   // Handle
   intersection {
      object { Cylinder_X scale <0.16, 0.16, 0.16> }
      plane { x, 0 inverse }
      plane { x, 1.3 }
      texture { Sandalwood }
     translate 1*x
   }

//   sphere { <1,1,2> 1.5 texture { Sandalwood } }

   // Lens
   intersection {
     object { Paraboloid_Z scale <1,1,0.2>  translate -0.17*z }
     object { Paraboloid_Z scale <1,1,-0.2> translate 0.17*z }
     texture { LensGlass }
   }

    rotate <20, 40, -45>
}

camera {
   location  <0.0, 0.0, -5.0>
   direction <0.0, 0.0,  1.0>
   up        <0.0, 1.0,  0.0>
//   right     <4/3, 0.0,  0.0>
}

light_source { <50.0, 40.0, -20.0> colour White *2 }
light_source { <-99.0, 79.0, 20.0> colour White *1 }
//light_source { <0, 20.0, 20.0> colour White *1 }




