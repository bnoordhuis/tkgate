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

#declare VersionSave_Temp = version;
#version 1.0;
#declare Paper_Text =
  texture {
    image_map { <0, 0, 0> png "textedittext.png" }
    scale <1.38, 1.03, 1.3>
    translate <-0.7, 0.5, 0>
    ambient  0.2
    diffuse 0.5
//    phong 1.0
//    phong_size 80
  }
#version VersionSave_Temp;

#declare Paper =
  union {
    #local W = 0.7;
    #local H = 0.5;
    #local D = 0.01;

    box { <-W,-H,-D>, <W, H, D> }

    texture { Paper_Text }
  }


union {
  object { Paper }

  rotate x*30
  rotate y*30
//  rotate z*90
  translate -0.5*x
}


//#declare CV = 0.85098039215686274509;
#declare CV = 1;

  sky_sphere {
    pigment {
//      colour red 0.8 green 0.8 blue 0.8
//      colour red 1 green 1 blue 1
      colour red CV green CV blue CV
    }
  }


//
// Where are we anyway?
//
camera {
  location  <0.2, 0.0, -3>
  look_at <0.2, 0.0,  0.0>
}

light_source {
  <-30.0, 30.0, -3>
  colour White *2
}
