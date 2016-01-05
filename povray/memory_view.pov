//
// Memory chip with insides visible
// by Jeffery P. Hansen
//

//global_settings { assumed_gamma 2.2 }

#include "shapes.inc"
#include "chars.inc"
#include "colors.inc"
#include "textures.inc"
#include "woods.inc"
#include "chip.inc"

#macro starttext(xpos,ypos,tsize,space)
  #declare CP_X = xpos;
  #declare CP_Y = ypos;
  #declare CP_LEFT = xpos;
  #declare CP_SPACE = space;
  #declare CP_SIZE = tsize;
#end

#macro newline()
  #declare CP_X = CP_LEFT;
  #declare CP_Y = CP_Y - (BoardHeight+BoardGap);
#end

#macro kern(C)
  #declare CP_X = CP_X + CP_SPACE*C;
#end

#macro putchar(C)
    object {
	C scale <CP_SIZE,CP_SIZE,CP_SIZE>
	translate <CP_X,CP_Y,-.01> 
    }

  #declare CP_X = CP_X + CP_SPACE ;
#end

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
    pigment { rgbf<1.0, 1.0, 1.0, 0.95> }
    finish {
      specular 1
      roughness 0.001
      ambient 0
      diffuse 0
      reflection 0.1
      ior 1.5
  }
}

#declare MagGlass =
union {

   // Rim
   difference {
     intersection {
        object { Cylinder_Z scale <1.0, 1.0, 1.0> }
        plane { z, -0.1 inverse }
        plane { z, 0.1 }
     }
     intersection {
        object { Cylinder_Z scale <0.9, 0.9, 1.0> }
     }
     texture { Rim_Texture }
   }

   // Handle
   intersection {
      object { Cylinder_X scale <0.16, 0.16, 0.16> }
      plane { x, 0 inverse }
      plane { x, 2 }
      texture { T_Wood4 }
     translate 1*x
   }

//   sphere { <1,1,2> 1.5 texture { Sandalwood } }

   // Lens
   intersection {
//     object { Paraboloid_Z scale <1,1,0.25>  translate -0.17*z }
//     object { Paraboloid_Z scale <1,1,-0.25> translate 0.17*z }

//     object { Paraboloid_Z scale <1,1,0.1>  translate -0.105*z }
//     object { Paraboloid_Z scale <1,1,-0.1> translate 0.105*z }

     object { Paraboloid_Z scale <1,1,0.2>  translate -0.15*z }
     object { Paraboloid_Z scale <1,1,-0.2> translate 0.15*z }

     texture { LensGlass }
   }

    rotate <20, 40, -45>
}

#declare Paper_Text =
  texture {
    pigment { rgb <1,0.95,0.95>  }
    finish {
      specular 0.1
      roughness 0.1
      ambient 0.2
      diffuse 0.6
    }
  }

#declare ChipHole =
object {
   box { <-.4,-.5 , -0.05>, <0.4, 0.5, -0.2> }
   texture {Chip_Text }
}

#declare ChipContent =
union {
   object {
     box { <-.4,-.5 , -0.05>, <0.4, 0.5, -.06> }
     texture {Paper_Text }
   }
  
   //
   // xxxxxxxxxxxxxxxxxx
   // You must be bored.
   // Y?u ??st ?e bored.
   //
   union {
     starttext(-.30, .3, 0.02, 0.1)
     putchar(char_5)
     putchar(char_9)
     kern(0.5)
     putchar(char_6)
     putchar(char_F)
     kern(0.5)
     putchar(char_7)
     putchar(char_5)
     #declare CP_X = CP_LEFT;
     #declare CP_Y = CP_Y - 0.15;

     putchar(char_2)
     putchar(char_0)
     kern(0.5)
     putchar(char_6)
     putchar(char_D)
     kern(0.5)
     putchar(char_7)
     putchar(char_5)
     #declare CP_X = CP_LEFT;
     #declare CP_Y = CP_Y - 0.15;
	

     putchar(char_7)
     putchar(char_3)
     kern(0.5)
     putchar(char_7)
     putchar(char_4)
     kern(0.5)
     putchar(char_2)
     putchar(char_0)
     #declare CP_X = CP_LEFT;
     #declare CP_Y = CP_Y - 0.15;
	
     putchar(char_6)
     putchar(char_2)
     kern(0.5)
     putchar(char_6)
     putchar(char_5)
     kern(0.5)
     putchar(char_2)
     putchar(char_0)
     #declare CP_X = CP_LEFT;
     #declare CP_Y = CP_Y - 0.15;
     
     putchar(char_6)
     putchar(char_2)
     kern(0.5)
     putchar(char_6)
     putchar(char_F)
     kern(0.5)
     putchar(char_7)
     putchar(char_2)
     #declare CP_X = CP_LEFT;
     #declare CP_Y = CP_Y - 0.15;

     putchar(char_6)
     putchar(char_5)
     kern(0.5)
     putchar(char_6)
     putchar(char_4)
     kern(0.5)
     putchar(char_2)
     putchar(char_E)
     #declare CP_X = CP_LEFT;
     #declare CP_Y = CP_Y - 0.15;
	
     translate -0.06*z
   }
}

union {
  difference { object { Chip} object { ChipHole }}  
  object { ChipContent } 
  scale 2
}

object { MagGlass rotate -45*x scale 0.6 translate <.45,.6,-0.9> }

  sky_sphere {
    pigment {
      colour red 0.8 green 0.8 blue 0.8
    }
  }


//
// Where are we anyway?
//
camera {
  location  <0.2, 0.0, -6>
  look_at <0.2, 0.0,  0.0>
}

light_source {
  <-30.0, 30.0, -30>
  colour White *2
}
