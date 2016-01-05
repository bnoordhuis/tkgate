#include "colors.inc"
global_settings {
  assumed_gamma 2.2
  hf_gray_16
}

#declare PScale = 5.0;

#declare FlatTexture =
  pigment {
    Black
  }

#declare HillyTexture =
  pigment {
    bozo
    turbulence 0.1
    colour_map {
      [0.0 	0.1*White]
      [1.0  White]
    }
    scale (1.0/PScale)*y
  }


object {
  plane { z, 0}

  hollow on
  texture {
    gradient y
    scale PScale*y
    texture_map {
       [0.0 HillyTexture]
       [0.7 HillyTexture]
       [0.95 FlatTexture]
    }
  }
}

camera {
  location <0, 0, -10>
  look_at 0
}


light_source { <0, 20, -100> color White }
