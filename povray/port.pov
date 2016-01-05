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


#macro starttext(xpos,ypos,size,space)
  #declare CP_X = xpos;
  #declare CP_Y = ypos;
  #declare CP_LEFT = xpos;
  #declare CP_SPACE = space;
  #declare CP_SIZE = size;
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

#declare Gate_Text =
  texture {
    pigment { rgb <0.05, 0.05, 0.6>  }
    finish {
//      metallic
      specular 0.15
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare BugHead_Text =
  texture {
    pigment { rgb <0.05, 0.1, 0.05>  }
    finish {
//      metallic
      specular 0.15
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare Wire_Text =
  texture {
    pigment { rgb <0.05, 0.6, 0.05>  }
    finish {
//      metallic
      specular 0.15
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare BugShell_Text =
  texture {

    pigment { gradient z scale .03
	color_map {
	    [0.0, 1 color rgb <0.05, 0.2, 0.05>
	              color rgb <0.05, 0.6, 0.05>] }}

//    pigment {  }
    finish {
      metallic
      specular 0.2
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare Bus_Text =
  texture {
    pigment { rgb <0.6, 0.05, 0.6>  }
    finish {
//      metallic
      specular 0.15
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare Pin_Text =
  texture {
    pigment { rgb <1,0,0>  }
    finish {
      metallic
      specular 1
//      roughness 0.1
//      ambient 0.2
//      diffuse 0.2
      reflection .2
    }
  }



#declare Circuit =
  union {
    #local W = 0.75;
    #local H = 1.0;
    #local D = 0.1;
    #local T = 0.1;

    intersection {
      // Outputs
      box { <-1, -1, 0.1>, <1, 1, -0.1> } 
      box { <-1, -1, 1>, <1, 1, -1> scale 0.85 inverse} 

      texture { Bus_Text }
    }

    union {
      #local Y = -0.7;
      sphere { <0,0,0> 1 scale <.09,.09,.09> translate <1,Y,0> }
      cylinder { 0*x, 5*x, 0.05 translate <1,Y,0> }
      
      union {
        starttext(0.6, Y-0.07, 0.03, 0.15)
        putchar(char_Z)
        putchar(char_7)
 	translate <0,0,-.1>
      }
      texture { Gate_Text }
    }
    union {
      #local Y = -0.4;
      sphere { <0,0,0> 1 scale <.09,.09,.09> translate <1,Y,0> }
      cylinder { 0*x, 5*x, 0.05 translate <1,Y,0> }
      
      union {
        starttext(0.6, Y-0.07, 0.03, 0.15)
        putchar(char_Z)
        putchar(char_6)
 	translate <0,0,-.1>
      }
      texture { Gate_Text }
    }
    union {
      #local Y = -0.1;
      sphere { <0,0,0> 1 scale <.09,.09,.09> translate <1,Y,0> }
      cylinder { 0*x, 5*x, 0.05 translate <1,Y,0> }
      
      union {
        starttext(0.6, Y-0.07, 0.03, 0.15)
        putchar(char_Z)
        putchar(char_5)
 	translate <0,0,-.1>
      }
      texture { Gate_Text }
    }
  }


union {
   intersection {
      object { Circuit }
      plane { x, 0 inverse }
      plane { x, 1.5 }
      plane { y, 0 }
   }

   rotate 55*y
   rotate 30*x
}


#declare CV = 0.85098039215686274509;
sky_sphere {
  pigment {
    colour red CV green CV blue CV
  }
}


//
// Where are we anyway?
//
camera {
  location  <0.2, 0.0, -4.3>
  look_at <0.2, 0.0,  0.0>
}

light_source {
  <-30.0, 30.0, -30>
  colour White *3
}
