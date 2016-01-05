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

#declare Plastic_Text =
  texture {
    pigment { rgb <.87, .8, .77>  }
    normal {
	bozo 0.05
	scale .1
   }
    finish {
      specular 0.05
      roughness 0.1
      ambient 0.6
//      diffuse 0.9
      reflection 0.01
    }
  }

#declare GreenPlastic_Text =
  texture {
    pigment { rgb <.7, .87, .67>  }
    normal {
	bozo 0.05
	scale .1
   }
    finish {
      specular 0.05
      roughness 0.1
      ambient 0.6
//      diffuse 0.9
      reflection 0.01
    }
  }

#declare RedPlastic_Text =
  texture {
    pigment { rgb <.87, .5, .47>  }
    normal {
	bozo 0.05
	scale .1
   }
    finish {
      specular 0.05
      roughness 0.1
      ambient 0.6
//      diffuse 0.9
      reflection 0.01
    }
  }

#declare DarkPlastic_Text =
  texture {
   #local X = 0.5;

    pigment { rgb <X*.87, X*.8, X*.77>  }
    normal {
	bozo 0.05
	scale .1
   }
    finish {
      specular 0.05
      roughness 0.1
      ambient 0.6
//      diffuse 0.9
      reflection 0.01
    }
  }




#macro RoundBox(W,H,D,R)
  union {
    sphere { <-W,-H,-D>, R }
    sphere { <-W,-H,D>, R }
    sphere { <-W,H,-D>, R }
    sphere { <-W,H,D>, R }
    sphere { <W,-H,-D>, R }
    sphere { <W,-H,D>, R }
    sphere { <W,H,-D>, R }
    sphere { <W,H,D>, R }

    cylinder { -W*x, W*x, R translate <0,H,D> }
    cylinder { -W*x, W*x, R translate <0,H,-D> }
    cylinder { -W*x, W*x, R translate <0,-H,D> }
    cylinder { -W*x, W*x, R translate <0,-H,-D> }

    cylinder { -D*z, D*z, R translate <W,H,0> }
    cylinder { -D*z, D*z, R translate <-W,H,0> }
    cylinder { -D*z, D*z, R translate <W,-H,0> }
    cylinder { -D*z, D*z, R translate <-W,-H,0> }

    cylinder { -H*y, H*y, R translate <W,0,D> }
    cylinder { -H*y, H*y, R translate <-W,0,D> }
    cylinder { -H*y, H*y, R translate <W,0,-D> }
    cylinder { -H*y, H*y, R translate <-W,0,-D> }

    box { <-W,-H-R,-D>, <W,H+R,D> }
    box { <-W,-H,-D-R>, <W,H,D+R> }
    box { <-W-R,-H,-D>, <W+R,H,D> }
  }
#end



#declare BlackBox =
  union {
    #local W = 10;
    #local H = 10;
    #local D = 10;
    #local R = 0.5;

    // main box
    RoundBox(W/2,H/2,D/2,R)

    texture { DarkPlastic_Text }
  }

union {
  object { BlackBox }
//  rotate y*40
//  rotate z*-20
}


#declare CV = 1;
  sky_sphere {
    pigment {
      colour red CV green CV blue CV
    }
  }

//
// Where are we anyway?
//
#declare DX = 10.5;
camera {
  location  <-10.0*DX, 20.0*DX, -35*DX>

  look_at <0.0, 0.0,  0.0>
}

light_source {
  <100,200, -300>
  colour White *1.2
}












