//
// Gate Logo
// by Jeffery P. Hansen
//

//global_settings { assumed_gamma 2.2 }

#include "shapes.inc"
#include "chars.inc"
#include "colors.inc"
#include "textures.inc"

#declare WireText = 
  texture {
    pigment { rgb <0, 0.7, 0 > }
    finish {
	specular 0.1
	reflection 0.3
	ambient 0.8
    }
  }

#declare BlobText = 
  texture {
    pigment { rgb <0, 0, 0.7 > }
    finish {
	specular 0.1
	reflection 0.3
	ambient 0.8
    }
  }

#declare WireNet =
  union {
    #local R = 0.075;
    #local R2 = 0.2;
    cylinder { -5*y, 5*y, R texture { WireText} }
    cylinder { -5*x, 0*x, R texture { WireText} translate -0.5*y }
    cylinder { 1*x, 0*x, R texture { WireText} translate 0.5*y }
    cylinder { 0.5*y, 5*y, R texture { WireText} translate 1*x }
    sphere { <1,0.5,0>, R texture { WireText} }

    sphere { <0,0.5,0>, R2 texture { BlobText} }
    sphere { <0,-0.5,0>, R2 texture { BlobText} }
  }


object { 
  WireNet

  rotate 40*x
//  rotate 30*y
}


/*
sky_sphere {
  pigment {
    colour red 0.8 green 0.8 blue 0.8
  }
}
*/


//
// Where are we anyway?
//
camera {
  location  <0.0, 0.0, -4>
  look_at <0.0, 0.0,  0.0>
}

light_source {
  <-30.0, 30.0, -15>
  colour White *1
}
