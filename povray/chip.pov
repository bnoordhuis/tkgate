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

object { Chip }

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
