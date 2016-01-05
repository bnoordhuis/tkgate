//
// Gate Logo
// by Jeffery P. Hansen
//

//global_settings { assumed_gamma 2.2 }

#include "shapes.inc"
#include "chars.inc"
#include "colors.inc"
#include "textures.inc"
#include "chip.inc"

#declare XMark =
  union {
    box { <-0.1,-1,0>, <0.1,1,0.1> }
    intersection {
      cylinder { 0*z, 0.1*z, 1} 
      cylinder { -0.1*z, 0.2*z, 0.85 inverse } 
    }
    texture { pigment { rgb <1,0,0> }
	finish {
	 specular 0.2
	 diffuse 0.8
	 roughness 0.1
	}
    }
    scale 0.8
    rotate 45*z
  }

object { Chip }	
object { XMark translate -0.5*z }	

sky_sphere {
  pigment {
    colour red 0.8 green 0.8 blue 0.8
  }
}

//
// Where are we anyway?
//
camera {
  location  <0.0, 0.0, -2.5>
  look_at <0.0, 0.0,  0.0>
}

light_source {
  <-30.0, 30.0, -15>
  colour White *5
}
