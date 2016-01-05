//
// Gate Logo
// by Jeffery P. Hansen
//
// Designed to be 49x43 (old size  41x35)

//global_settings { assumed_gamma 2.2 }

#include "shapes.inc"
#include "chars.inc"
#include "colors.inc"
#include "textures.inc"

#declare PostRadius = 0.5;			// Radius of posts
#declare AngleStart = 9;			// Start angle of TkGate text
#declare AngleSpan = 21.0;			// Number of degrees though which to position text
#declare AngleStep = -(AngleSpan/8.0);		// Number of degrees per letter

#declare DrToe1Len = 0.1;
#declare DrToe2Len = 0.08;
#declare DrToeR = 0.035;
#declare DrToeA1 = 20;
#declare DrToeA2 = -40;

#declare DrThumb1Len = 0.04;
#declare DrThumb2Len = 0.04;
#declare DrThumbR = 0.04;
#declare DrThumbA1 = 5;
#declare DrThumbA2 = -20;

#declare DrClawStart = 0;
#declare DrClawEnd = 0.1;
#declare DrClawR = 0.02;

#declare ClawText =
  texture {
    pigment { color Silver }
    normal {
      crackle 0.2
      scale 0.2
    }
    finish {
      metallic
      specular .05
      roughness 0.1
      ambient 0.2
      diffuse 0.4
      reflection .2
    }
  }

#declare PlateText =
  texture {
    pigment { 
	rgb <.4,0,0>
    }
    finish {
      metallic
      specular .05
      roughness 0.1
      ambient 0.2
      diffuse 0.4
      reflection .01
    }
  }

#declare DragonHide =
  texture {
    pigment { 
	rgb <.8,.2,.2>
    }
    normal {
      	crackle 0.7
	scale 0.25
    }
    finish {
      metallic
      specular .05
      roughness 0.1
      ambient 0.2
      diffuse 0.4
    }
  }

#declare Sand_Text =
  texture {
    pigment {
      colour red 0.95 green 0.64 blue 0.38
    }

    normal {
      crackle .25
      turbulence 0.2
      scale <1,0.05,0.1>
      rotate 30*y
    }
    finish { ambient 0.7 diffuse 0.4 }
  }

//
// The ground
//
#declare SandDunes =
  intersection {
    plane { z, 0 }
    height_field {
      png "hills.png"
      smooth
      translate <-0.5,0,-0.85>
      scale <40,0.5,40>
    }

    hollow on
    texture { Sand_Text }
  }

#declare GrassTexture = 
  texture {
      pigment {
	waves
	turbulence 0.01
        colour_map{
             [0.0 colour rgb <0.2,0.4,0.1>]
             [0.98 colour rgb <0.28,0.49,0.17>]
             [1.0 colour rgb <0.38,0.39,0.11>]
        }
      }
      scale <0.001,0.0001,.15>
      finish{ ambient 1 diffuse 0.6 }
  }

//
// The ground
//
#declare SandDunes =
  intersection {
    plane { z, 0 }
    height_field {
      png "hills.png"
      smooth
      translate <-0.5,0,-0.85>
      scale <40,0.5,40>
    }

    hollow on
    texture { Sand_Text }
  }

#declare GrassHill =
  intersection {
    plane { z, .1 inverse }
    height_field {
      png "hills.png"
      smooth
      translate -0.5*x
      scale <40,3,40>
    }
    hollow on
    texture { GrassTexture }
  }

#declare GateGround =
  union {
    object { SandDunes }
    object { GrassHill }
  }
//
// A texture for wrought iron.
//
#declare Wrought_Iron = 
  texture {
    pigment {
      granite
      color_map {
        [0.0 1.0  colour red 0.2 green 0.2 blue 0.2
                  colour red 0.0 green 0.0 blue 0.0]
      }
    }
    finish { ambient 0 diffuse 0.6 metallic 1}
    normal { wrinkles 1 scale 0.11 }
  }

//
// A period
//
#declare char_Period=
  cylinder {<0,0.5,0>,<0,0.5,1>,0.5}


#declare Post_Stone =
 texture{
   pigment{
      colour red 0.95 green 0.95 blue 0.7
   }
   normal{
     granite 0.05
     scale 0.24
   }
   finish{phong 0.8 phong_size 200 diffuse 0.4 }
 }


//
// The sky
//
#declare GateSky = 
  sphere { <0.0, 0.0, 0.0>, 3500.0
    hollow on
    pigment {
      onion
      colour_map {
        [0.0 1.0  colour red 0.1 green 0.3 blue 1.0
                  colour red 0.1 green 0.6 blue 1.0]
      }
      quick_colour red 0.7 green 0.7 blue 1.0
      scale <6000.0, 1700.0, 4000.0>
      translate <-1200.0, 220.0, 2500.0>
    }
    finish {
      ambient 0.9
      diffuse 0.0
    }
  }


//
// The clouds
//
#declare GateClouds = 
  plane { y, 300.0
    hollow on
    pigment {
      bozo
      turbulence 1
      colour_map {
        [0.0 0.4   colour red 1.0 green 1.0 blue 1.0 filter 1.0
                   colour red 1.0 green 1.0 blue 1.0 filter 1.0]
        [0.4 0.8   colour red 1.0 green 1.0 blue 1.0 filter 1.0
                   colour red 0.8 green 0.9 blue 0.9]
        [0.8 1.001 colour red 0.9 green 0.9 blue 0.9
                   colour red 0.8 green 0.8 blue 0.8]
      }
      quick_colour red 0.7 green 0.7 blue 1.0
      scale <1000.0, 200.0, 800.0>
    }
    finish {
      ambient 0.7
      diffuse 0.0
    }

    translate -450*x
    rotate 6*y
  }



#declare CowThickness = 0.5;
#declare CowLegLength = 0.5;
#declare CowLegThickness = 0.1;

#declare Cow_Texture = 
texture {
    pigment {
	bozo
	frequency 0.9
	scale .15
        color_map {
         [0.1 colour rgb <0,0,0> ]
         [0.2 colour rgb <1,1,1> ]
        }
    }
}

#declare CowLeg = 
  union {
    cylinder { 0*y, CowLegLength*y, CowLegThickness }
    object { sphere { <0,CowLegLength,0>, CowLegThickness } }
  }



#declare ClawPos =
  transform {
    rotate -5*y
    translate DrToe2Len*x
    rotate DrToeA2*y
    translate DrToe1Len*x
    rotate DrToeA1*y
  }

#declare ThumbClawPos =
  transform {
    rotate -5*y

    translate DrThumb2Len*x
    rotate DrThumbA2*y
    translate DrThumb1Len*x
    rotate DrThumbA1*y
  }

#declare DragonFoot = 
  union {
    #local SA = 25;
    #local SB = 85;
    blob {
      threshold 0.01

      cylinder { 0*x, DrToe1Len*x, DrToeR,1 rotate DrToeA1*y rotate 0*z }
      cylinder { 0*x, DrToe2Len*x, DrToeR,1 rotate DrToeA2*y translate DrToe1Len*x rotate DrToeA1*y rotate 0*z }

      cylinder { 0*x, DrToe1Len*x, DrToeR,1 rotate DrToeA1*y rotate SA*z }
      cylinder { 0*x, DrToe2Len*x, DrToeR,1 rotate DrToeA2*y translate DrToe1Len*x rotate DrToeA1*y rotate SA*z  }

      cylinder { 0*x, DrToe1Len*x, DrToeR,1 rotate DrToeA1*y rotate -SA*z }
      cylinder { 0*x, DrToe2Len*x, DrToeR,1 rotate DrToeA2*y translate DrToe1Len*x rotate DrToeA1*y rotate -SA*z  }

      cylinder { 0*x, DrThumb1Len*x, DrThumbR,1 rotate DrThumbA1*y rotate -SB*z }
      cylinder { 0*x, DrThumb2Len*x, DrThumbR,1 rotate DrThumbA2*y translate DrThumb1Len*x rotate DrThumbA1*y rotate -SB*z  }


      //      scale <1,1,0.75>

      texture { DragonHide }
    }
    union {
      cone { <DrClawStart,0,0>, DrClawR <DrClawEnd,0,0>, 0 transform ClawPos }
      cone { <DrClawStart,0,0>, DrClawR <DrClawEnd,0,0>, 0 transform ClawPos rotate SA*z}
      cone { <DrClawStart,0,0>, DrClawR <DrClawEnd,0,0>, 0 transform ClawPos rotate -SA*z}
      cone { <DrClawStart,0,0>, DrClawR <DrClawEnd,0,0>, 0 transform ThumbClawPos rotate -SB*z}
      //      scale <1,0.5,1>

      texture { ClawText }
    }
    rotate 0*y
    rotate 90*x
  }

#macro DragonLeg(A1,A2,R1,R2,R3,FS) 
  #local YS = 1;
  #local L1 = 0.4;	// Thiegh length
  #local L2 = 0.3;	// Calf length

  union {
    blob {
      threshold 0.25
      sphere { <0,0,0>, R1, 1 scale <1,1.3,1> translate -0.3*R1*y }
      cylinder { 0*y, -L1*y, R2, 1}
      cylinder { 0*y, -L2*y, R3, 1 rotate z*A2 translate -L1*y}

      scale <1,YS,0.5>
      rotate -A1*z

      texture { DragonHide }
    }

    object {
        DragonFoot
	scale FS
        rotate -(A2-A1)*z
	translate -(L2+0.05)*y
	rotate z*A2
	translate -L1*y
	rotate -A1*z

	translate -(0.08)*x
	translate -(1-FS)*y

	texture { DragonHide }
    }

    translate <0,0.7,0.1>
  }
#end

#macro DragonBackLeg(A1,A2) 
  DragonLeg(A1,A2,0.2,0.12,0.10,1.0)
#end

#macro DragonFrontLeg(A1,A2) 
  DragonLeg(A1,A2,0.2,0.1,0.08,0.85)
#end

#declare AndGateDragon = 
   union {
     union {
       box { <0.0,0.0,0.0>, <0.8,1.1,CowThickness> }
       object { cylinder { 0.0*z, CowThickness*z, 0.55 }  translate <0.8,0.55,0.0>  }
       translate CowLegLength*y
     }

/*
     object { CowLeg }
     object { CowLeg translate <0.75,0,0> }
     object { CowLeg translate <0,0,CowThickness> }
     object { CowLeg translate <0.75,0,CowThickness> }
*/

     object { DragonBackLeg(45,100) translate 0.95*x }
     object { DragonBackLeg(45,100) scale <1,1,-1> translate 0.95*x  }
     object { DragonBackLeg(45,100) translate 0.1*x }
     object { DragonBackLeg(45,100) scale <1,1,-1> translate 0.1*x  }
    
     texture { DragonHide }
   }


//
// Define the posts that go on the sides of the gate. 
//
#declare Post = 
  union {
    intersection {
      object { box { <-PostRadius,0, -PostRadius>, <PostRadius, 3.3, PostRadius> }  }
      object { box { <-PostRadius,0, -PostRadius>, <PostRadius, 3.3, PostRadius> } rotate 45*y  }
    }

    object { sphere { <0,0,0>, PostRadius }  translate 3.3*y }
    object { sphere { <0,0,0>, PostRadius }  translate 3.8*y }
    texture { Post_Stone }
  }

#declare SpikeR = 0.2;

//
// A spike within the gate.  D is the distance from the center and is used both
// to place the spike and to compute its height.
//
#macro Spike(D)
   #local H = sin(acos(D/8.3))*8.3 - 5;
   union {
     object { cylinder { 0.2*y, H*y, SpikeR } }
     merge {
       object { Sphere scale <SpikeR, SpikeR, SpikeR>  translate 0.1*y }
       object { Cone_Y scale <SpikeR, 0.1, SpikeR>  translate 0.2*y }
       object { Cone_Y scale <SpikeR, SpikeR, SpikeR>  translate 0.025*y }
       translate H*y
     }
     translate D*x
   }
#end

//
// A spike on the fence
//
#declare FenceSpike = 
  union {
    object { cylinder { 0.2*y, 3.0*y, SpikeR } }
    merge {
      object { Sphere scale <SpikeR, SpikeR, SpikeR>  translate 0.1*y }
      object { Cone_Y scale <SpikeR, 0.1, SpikeR>  translate 0.2*y }
      object { Cone_Y scale <SpikeR, SpikeR, SpikeR>  translate 0.025*y }
      translate 3.0*y
    }
  }

//
// Basic arc (actually a ring) that is used as parts of the OR gate.
//
#declare GateArc=
  difference {
    object { cylinder { 0.0*z, 0.1*z, 0.5 }  }
    object { cylinder { z, -z, 0.4 }  }
  }

//
// Back arc of an OR gate
//
#declare OrBack = 
  intersection {
    object { GateArc }
    plane {x, 0 inverse }
    plane {y, -0.3 inverse }
    plane {y, 0.3  }

    translate <-0.4,0.3,0.0>
  }

//
// Basic OR gate.
//
#declare OrGate = 
  union {
    box { <0.0,0.5,0.0>, <0.25,0.6,0.1> }
    box { <0.0,0.0,0.0>, <0.25,0.1,0.1> }

    //
    // The back of the OR gate
    //
    object { OrBack }

    //
    // Top-half of OR curve.
    //
    intersection {
      intersection {
	object { GateArc  }
        plane {x, 0 inverse }
        translate <0.25,0.1,0.0>
      }
      plane {y, 0.3 inverse }
    }

    //
    // Bottom-half of OR curve.
    //
    intersection {
      intersection {
	object { GateArc  }
        plane {x, 0 inverse }
        translate <0.25,0.5,0.0>
      }
      plane {y,0.3 }
    }
  }

//
// Cut out of NOR gate.
//
#declare UnNOrGate = 
  union {
    intersection {
      box { <-0.001,0.1,0>, <0.25,0.5,0.1> }
      object { cylinder { -0.1*z, 0.2*z, 0.5 } translate <-0.4,0.3, 0.0> inverse}
    }
    intersection {
      object { cylinder { -0.001*z, 0.2*z, 0.4 } translate <0.25,0.5, 0> }
      object { cylinder { -0.1*z, 0.2*z, 0.4 } translate <0.25,0.1, 0> }
      plane {x, 0.25 inverse }
    }
    object { cylinder { -0.001*z, 0.1*z, 0.05 } translate <0.75, 0.3, 0> }
  }


#declare NOrGate = 
  union {
    box { <0,0,0>, <0.1,0.05,0.05> translate <0.9,0.275,0>}
    box { <0,0,0>, <0.1,0.05,0.05> translate <-0.1,0.45,0>}
    box { <0,0,0>, <0.1,0.05,0.05> translate <-0.1,0.1,0>}

    object { OrGate }
      difference {
        object { cylinder { 0.0*z, 0.1*z, 0.15 }  }
        object { cylinder { z, -z, 0.05 }  }
	translate <0.75, 0.3, 0>
    }
  }

#macro Wire(X1,Y1,X2,Y2)
  #local D = sqrt((X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2));
  #local R = 90-atan2((X2-X1),(Y2-Y1))*180/3.1415926;
  object {
    box { <0,0,0>, <D,0.15,0.1> }
    rotate R*z
    translate <X1,Y1,0>
  }
#end

#declare DoorBase = 
  union {
    Spike(1.5) 
    Spike(0.9) 
    Spike(0.3) 

    Wire(0.05,2.75,  1.875,2.75)
    Wire(0.05,0.5,  1.875,0.5)
  }

#declare Fence =
  union {
    //
    // Fence cross beams 
    //
    box { <2.0,0.5,-0.05>, <4.0,0.55,0.05> } 
    object { box { <2.0,0.5,-0.05>, <4.0,0.55,0.05> } translate 2.3*y }

    //
    // Fence
    //
    object { FenceSpike translate 2.6*x }
    object { FenceSpike translate 2.9*x }
    object { FenceSpike translate 3.2*x }
    object { FenceSpike translate 3.5*x }
    object { FenceSpike translate 3.8*x }
  }

//
// Left half of the master/slave flip-flop circuit
//
#declare FlipFlopLeft =
  union {
    object { NOrGate translate < -1.2,2.3,0> }
    object { NOrGate translate < -1.2,0.4,0> }

    Wire(-1.875,0.5,   -1.175,0.5)				// Lower-left input B
    Wire(-1.875,2.75,  -1.175 ,2.75)				// Upper-left input A

    Wire(-0.4,2.575,  -0.025,2.575)				// Upper stage connect

    Wire(-0.4,0.675,  -0.025,0.675)				// Lower stage connect


    //
    // UL to LL feedback path
    //
    Wire(-0.1,2.575,    -0.1,2.075)				// out/down from UL
    Wire(-0.08,2.125,    -1.575,1.25)				// Diagonal
    Wire(-1.575,1.25,   -1.575,0.85)				// Lower-left input A vertical
    Wire(-1.575,0.85,   -1.175,0.85)				// Lower-left input A

    //
    // LL to UL feedback path
    //
    union {
      Wire(-0.1,2.575,    -0.1,2.075)				// out/down from UL
      Wire(-0.08,2.125,    -1.575,1.25)				// Diagonal
      Wire(-1.575,1.25,   -1.575,0.85)				// Lower-left input A vertical
      Wire(-1.575,0.85,   -1.175,0.85)				// Lower-left input A
      scale <1,-1,1>
      translate 3.3*y
    }
  }

//
// Right half of the master/slave flip-flop circuit
//
#declare FlipFlopRight =
  union {
    object { NOrGate translate < 0.4,2.125,0> }
    object { NOrGate translate < 0.4,0.575,0> }

    Wire(1.3,2.4,  1.875,2.4)					// Upper-right output
    Wire(1.3,0.85,   1.875,0.85)				// Lower-right output

    Wire(0.025,2.575,  0.45,2.575)				// Upper stage connect

    Wire(0.025,0.675,  0.45,0.675)				// Lower stage connect


    // UR to LR feedback
    Wire(1.525,2.4,  1.525,2.125)				// UR
    Wire(1.545,2.155, 0.025,1.25)  				// diagonal
    Wire(0.025,1.25,  0.025,1.025)				// LR vertical
    Wire(0.025,1.025,  0.45,1.025)				// LR in

    union {
      Wire(1.525,2.4,  1.525,2.125)				// UR
      Wire(1.545,2.155, 0.025,1.25)  				// diagonal
      Wire(0.025,1.25,  0.025,1.025)				// LR vertical
      Wire(0.025,1.025,  0.45,1.025)				// LR in
      scale <1,-1,1>
      translate 3.3*y
    }
  }

#declare FlipFlopCutOutLeft =
  union {
    object { UnNOrGate translate < -1.2,2.3,0> }
    object { UnNOrGate translate < -1.2,0.4,0> }
  }

#declare FlipFlopCutOutRight =
  union {
    object { UnNOrGate translate < 0.4,2.125,0> }
    object { UnNOrGate translate < 0.4,0.575,0> }
  }

#declare LeftDoor = 
  union {
    object { DoorBase scale <-1,1,1> }

    translate 1.9*x
  }

#declare RightDoor = 
  union {
    object { DoorBase }

    translate -1.9*x
  }

#declare Brackets =
  union {
    box { <0,0,0>, <0.3,0.05,0.1> translate <-2.125,2.8,0> }	// Upper-left bracket
    box { <0,0,0>, <0.3,0.05,0.1> translate <-2.125,0.45,0> }	// Lower-left bracket
    box { <0,0,0>, <0.3,0.05,0.1> translate <1.825,2.8,0> }	// Upper-right bracket
    box { <0,0,0>, <0.3,0.05,0.1> translate <1.825,0.45,0> }	// Lower-right bracket
  }

//
// This is all the ironwork in the gate
//
#declare IronWork =
  union {
    #local A = 0;
    object { LeftDoor rotate -A*y translate -1.9*x }
    object { RightDoor rotate A*y translate 1.9*x }
    texture { Wrought_Iron }
  }


//
// Put it all together here.
//
//object { GateGround }					// The ground
//object { GateSky }					// The sky
//object { GateClouds }					// The clouds
//object { Post translate -(1.95+PostRadius)*x }		// The left gate post
//object { Post translate (1.95+PostRadius)*x }		// The right gate post
//object { IronWork }					// The iron work
//object { AndGateDragon scale 2.0 translate <0,0,-1> }

object { text { ttf "cyrvetic.ttf" "The balrog went to the store to buy apples." 0.25, .5*x } scale 2.2 rotate -60*y translate <-3.5,1,0> texture { Post_Stone} }

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
  location  <0.1, 2.2, -6>		// Full view
  direction <0.0, 0.0,  1.0>
}


//
// Let there be light!
//
//light_source { <50.0, 90.0, -30> colour White *3 }
//light_source { <-50.0, 90.0, -30> colour White }

light_source {
  <200.0, 250.0, -250>
  colour White *3
}

light_source {
  <-50.0, 90.0, -30>
  colour White*0.1
  shadowless
}
