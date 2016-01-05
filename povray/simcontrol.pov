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
  sky_sphere {
    pigment { rgb <0.7, 0.7, 1.0> }

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


#declare R = 0.5;

#macro ASquare(W,H)
  union {
    object {
      box { <-W,-H,-R>, <W,H,R> }
      texture { MyAluminum1 }
    }
    union {
      object { cylinder { -W*x, W*x, R } translate H*y }
      object { cylinder { -W*x, W*x, R } translate -H*y }
      object { cylinder { -H*y, H*y, R } translate W*x }
      object { cylinder { -H*y, H*y, R } translate -W*x }
      object { sphere { <W, H, 0>, R } }
      object { sphere { <-W, H, 0>, R } }
      object { sphere { <W, -H, 0>, R } }
      object { sphere { <-W, -H, 0>, R } }
      texture { MyAluminum2 }
    }
    scale 0.15
  }
#end

#declare PlayButton =
  union {
    intersection {
      plane { z, R }
      plane { z, -R inverse }
      plane { x, 0 inverse }
      plane { x, 0 rotate 60*z translate sqrt(3)*x}
      plane { x, 0 rotate -60*z translate sqrt(3)*x}
      texture { MyAluminum1 }
    }
    union {
      object { cylinder { -1*y, 1*y, R } translate 0*x }
      object { cylinder { 0*y, 2*y, R rotate 60*z translate sqrt(3)*x}  }
      object { cylinder { 0*y, 2*y, R rotate 120*z translate sqrt(3)*x}  }


      object { sphere { <sqrt(3), 0, 0>, R } }
      object { sphere { <0, 1, 0>, R } }
      object { sphere { <0, -1, 0>, R } }
      texture { MyAluminum2 }
    }
    scale 0.15
  }

#declare FFPart =
  union {
    object { cylinder { 0*y, sqrt(2)*y, R rotate 45*z translate 1*x}  }
    object { cylinder { 0*y, sqrt(2)*y, R rotate 135*z translate 1*x}  }
    object { sphere { <1, 0, 0>, R } }
    object { sphere { <0, 1, 0>, R } }
    object { sphere { <0, -1, 0>, R } }
  }

#declare FFButton =
  union {
    object { FFPart }
    object { FFPart translate 1.5*x }

  
    object { sphere { <0, 1, 0>, R } translate 3.5*x  }
    object { sphere { <0, -1, 0>, R } translate 3.5*x  }
     object { cylinder { -1*y, 1*y, R } translate 3.5*x }

    texture { MyAluminum2 }
    scale 0.15
  }

#declare FFButton2 =
  union {
    object { PlayButton }
    union {
      object { cylinder { -1*y, 1*y, R } translate (sqrt(3)+2.5*R)*x }
      object { cylinder { -1*y, 1*y, R } translate (sqrt(3)+5*R)*x }

      object { sphere { <0, 1, 0>, R } translate (sqrt(3)+2.5*R)*x  }
      object { sphere { <0, -1, 0>, R } translate (sqrt(3)+2.5*R)*x  }

      object { sphere { <0, 1, 0>, R } translate (sqrt(3)+5*R)*x  }
      object { sphere { <0, -1, 0>, R } translate (sqrt(3)+5*R)*x  }


      scale 0.15
    }
    texture { MyAluminum2 }
  }

#declare PlayButton2 =
  union {
    intersection {
      plane { z, R }
      plane { z, -R inverse }
      plane { x, -1 inverse }
      plane { y, -1 inverse  }
      plane { y, 1  }
      plane { x, 0 rotate 45*z translate 1*x}
      plane { x, 0 rotate -45*z translate 1*x}
      texture { MyAluminum1 }
    }
    union {
      object { cylinder { -1*x, 0*x, R } translate 1*y }
      object { cylinder { -1*x, 0*x, R } translate -1*y }
      object { cylinder { -1*y, 1*y, R } translate -1*x }

      object { cylinder { 0*y, sqrt(2)*y, R rotate 45*z translate 1*x}  }
      object { cylinder { 0*y, sqrt(2)*y, R rotate 135*z translate 1*x}  }


      object { sphere { <1, 0, 0>, R } }
      object { sphere { <0, 1, 0>, R } }
      object { sphere { <0, -1, 0>, R } }
      object { sphere { <-1, 1, 0>, R } }
      object { sphere { <-1, -1, 0>, R } }
      texture { MyAluminum2 }
    }
    scale 0.15
  }

#declare StopButton =
  ASquare(1,1)

#declare PauseButton =
  union {
    object { ASquare(0.13,1) translate -0.1*x }
    object { ASquare(0.13,1) translate 0.1*x }
  }


union {
  object { PlayButton translate -1*x }
  object { PauseButton translate 0*x }
  object { StopButton translate 1*x }
  object { FFButton translate 2*x }
}

//
// Where are we anyway?
//
camera {
  location  <0, 0, -5>		// Full view
  look_at <0,0,0>
}

//
// Let there be light!
//
light_source { <-50.0, 50.0, -10> colour White*1.5 }
light_source { <0, 0, -30> colour White*0.5 }
