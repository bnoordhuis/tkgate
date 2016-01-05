// Gate scene with dragon
// by Jeffery P. Hansen
//
// For full picture use
//  povray +SR230 +ER500 +W960 +H720 +A +D +P +i dragongate.pov
//
// For foot adjustment use:
//  povray +SR430 +ER500 +SC125 +EC350 +W960 +H720 +A +D +P +i dragongate.pov
//

#declare showTrees = 0;
#declare showPosts = 1;
#declare showFence = 1;
#declare showDragons = 1;
#declare showSign = 1;
#declare showCacti = 1;
#declare DGView = 1;		/*	1 = regular view
					2 = left dragon feet
					3 = right dragon feet 
					4 = cactus view
					5 = closeup?
					6 = sign closeup
					7 = gate closeup
				*/

#include "shapes.inc"
#include "chars.inc"
#include "colors.inc"
#include "textures.inc"
#include "woods.inc"
#if (showTrees)
#include "OLIVE.inc"
#include "TOMTREE.inc"
#end
#include "dragon.inc"



#declare cactusRand = seed(42);


#declare PostRadius = 0.35;			// Radius of posts
#declare AngleStart = 9;			// Start angle of TkGate text
#declare AngleSpan = 21.0;			// Number of degrees though which to position text
#declare AngleStep = -(AngleSpan/8.0);		// Number of degrees per letter

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

#declare Cactus_Text = 
  texture {
    pigment {
      rgb <.1,.57,.1>
    }
    normal {
      leopard 0.1
      turbulence .1
      scale <0.005,0.05,0.005>
    }
    finish { ambient 0.3 diffuse 0.6 }
  }

#declare CactusSpine_Text = 
  texture {
    pigment {
      rgb <.9,.9,.1>
    }
    finish { ambient 0.3 diffuse 0.6 }
  }

//
// A period
//
#declare char_Period=
  cylinder {<0,0.5,0>,<0,0.5,1>,0.5}


#declare Post_Stone =
 texture{
   pigment{White}
   normal{
     granite 0.075
     scale 0.24
   }
   finish{phong 0.1 phong_size 200 diffuse 0.2 ambient 0.5 }
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
      finish{ ambient 0.2 diffuse 0.6 }
  }

#declare OldGrassTexture = 
  texture {
      pigment {
	bozo
	turbulence 0.1
	colour_map {
          [0.0 1.0   colour red 0.2 green 1.0 blue 0.2 filter 0
                     colour red 0.2 green 0.8 blue 0.2 filter 0]
 	}
      }
      normal{
   	wrinkles 0.1
	scale 0.24
      }


      finish { ambient 0.2 diffuse 0.2 }
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

#declare XGrassHill =
  union {
//    height_field {
//      png "hills2.png"
//      smooth
//      translate -0.5*x
//      translate 5*z
//      scale <200,50,200>
//    }
    height_field {
      png "hills2.png"
      smooth
      translate -0.5*x
      scale <50,3,50>
    }
    intersection {
      plane { y, 0 }
      plane { z, 0 inverse }
    }

    hollow on
//    texture { GrassTexture }
    texture { pigment { checker color Red, color Green } }
  }


#declare GateGround =
  union {
    object { SandDunes }
    object { GrassHill }
  }

#declare CowOrFlat=0.5;
#declare CowHeight=1.1;
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
    finish { ambient 0.2 }
}

#declare CowLeg = 
  union {
    cylinder { 0*y, CowLegLength*y, CowLegThickness }
    object { sphere { <0,CowLegLength,0>, CowLegThickness } }
  }


#declare AndGateCow = 
   union {
     union {
       box { <0.0,0.0,0.0>, <0.8,1.1,CowThickness> }
       object { cylinder { 0.0*z, CowThickness*z, 0.55 }  translate <0.8,0.55,0.0>  }
       translate CowLegLength*y
     }

     object { CowLeg }
     object { CowLeg translate <0.75,0,0> }
     object { CowLeg translate <0,0,CowThickness> }
     object { CowLeg translate <0.75,0,CowThickness> }

     texture { Cow_Texture }
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


#declare OrGateCow = 
   union {
     intersection {
       box { <0.0,0.0,0.0>, <CowOrFlat,CowHeight,CowThickness> }
       object { cylinder { -2*z, 2*z, CowHeight inverse } translate <-CowHeight*0.85,CowHeight/2,0> }
       translate CowLegLength*y
     }

     intersection {
       object { cylinder { 0*z, CowThickness*z, CowHeight } translate <CowOrFlat,0,0> }
       object { cylinder { -2*z, 2*z, CowHeight } translate <CowOrFlat,CowHeight,0> }
       plane { x, 0.5 inverse }
       translate CowLegLength*y
     }

     object { CowLeg }
     object { CowLeg translate <0.75,0,0> }
     object { CowLeg translate <0,0,CowThickness> }
     object { CowLeg translate <0.75,0,CowThickness> }

     texture { Cow_Texture translate 10*x }
   }

//
// Define the posts that go on the sides of the gate. 
//
#declare Post = 
  union {
//    cylinder { 0*y, 4.25*y, PostRadius }
    intersection {
      object { box { <-PostRadius,0, -PostRadius>, <PostRadius, 4.25, PostRadius> }  }
      object { box { <-PostRadius,0, -PostRadius>, <PostRadius, 4.25, PostRadius> } rotate 45*y  }
    }

    object { sphere { <0,0,0>, PostRadius }  translate 4.25*y }
    object { sphere { <0,0,0>, PostRadius }  translate 4.75*y }
//    texture { White_Marble }
    texture { Post_Stone }
//    texture { PinkAlabaster }
  }

//
// A spike within the gate.  D is the distance from the center and is used both
// to place the spike and to compute its height.
//
#macro Spike(D)
   #local H = sin(acos(D/8.3))*8.3 - 5;
   union {
     object { cylinder { 0.2*y, H*y, 0.05 } }
     merge {
       object { Sphere scale <0.05, 0.05, 0.05>  translate 0.1*y }
       object { Cone_Y scale <0.05, 0.1, 0.05>  translate 0.2*y }
       object { Cone_Y scale <0.05, 0.05, 0.05>  translate 0.025*y }
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
    object { cylinder { 0.2*y, 4.0*y, 0.05 } }
    merge {
      object { Sphere scale <0.05, 0.05, 0.05>  translate 0.1*y }
      object { Cone_Y scale <0.05, 0.1, 0.05>  translate 0.2*y }
      object { Cone_Y scale <0.05, 0.05, 0.05>  translate 0.025*y }
      translate 4.0*y
    }
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


#declare DoorBase = 
  union {
    Spike(1.7) 
    Spike(1.4) 
    Spike(1.1) 
    Spike(0.8) 
    Spike(0.5) 
    Spike(0.2) 
  }

#declare Fence =
  union {
    //
    // Fence cross beams 
    //
    box { <2.0,0.5,-0.05>, <50.0,0.55,0.05> } 
    object { box { <2.0,0.5,-0.05>, <50.0,0.55,0.05> } translate 3.3*y }

    //
    // Fence
    //
    #local i = 0;
    #while (i < 60)
      object { FenceSpike translate (2.6+0.4*i)*x }
      #local i = i + 1;
    #end
  }

#macro Wire(X1,Y1,X2,Y2)
  #local D = sqrt((X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2));
  #local R = 90-atan2((X2-X1),(Y2-Y1))*180/3.1415926;
  object {
    box { <0,0,0>, <D,0.05,0.1> }
    rotate R*z
    translate <X1,Y1,0>
  }
#end

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

//
// This is the lettering in the logo.
//
#declare Lettering = 
  intersection {
    union {
      union {
        object { char_T translate -2*x translate 88*y rotate (AngleStart+0*AngleStep)*z }
        object { char_K translate -2*x translate 88*y rotate (AngleStart+1*AngleStep)*z }
        object { char_G translate -2*x translate 88*y rotate (AngleStart+2*AngleStep)*z }
        object { char_A translate -2*x translate 88*y rotate (AngleStart+3*AngleStep)*z }
        object { char_T translate -2*x translate 88*y rotate (AngleStart+4*AngleStep)*z }
        object { char_E translate -2*x translate 88*y rotate (AngleStart+5*AngleStep)*z }
        object { char_2 translate -2*x translate 88*y rotate (AngleStart+6.5*AngleStep)*z }
        object { char_Period translate -2*x translate 88*y rotate (AngleStart+7.25*AngleStep)*z }
        object { char_0 translate -2*x translate 88*y rotate (AngleStart+8*AngleStep)*z }
	scale <1.0,1.0,0.5>
      }

      intersection {
        cylinder { 0*z, 0.05*z, 88 }
        cylinder { -.01*z, 0.06*z, 87 inverse }
      }
      intersection {
        cylinder { 0*z, 0.05*z, 94 }
        cylinder { -.01*z, 0.06*z, 93 inverse }
      }

      translate -90*y
      scale 0.1
    }
    plane { x, -2 inverse }
    plane { x, 2 }
  }


#declare LeftDoor = 
  union {
    intersection {
      object { DoorBase scale <-1,1,1> }
      object { FlipFlopCutOutLeft translate <0,0,-0.05> inverse }
    }
    object { FlipFlopLeft translate <0,0,-0.05> }

    translate 1.9*x
  }

#declare RightDoor = 
  union {
    intersection {
      object { DoorBase }
      object { FlipFlopCutOutRight translate <0,0,-0.05> inverse }
    }
    object { FlipFlopRight translate <0,0,-0.05> }

    translate -1.9*x
  }

#declare Brackets =
  union {
    box { <0,0,0>, <0.3,0.05,0.1> translate <-2.125,2.8,0> }	// Upper-left bracket
    box { <0,0,0>, <0.3,0.05,0.1> translate <-2.125,0.45,0> }	// Lower-left bracket
    box { <0,0,0>, <0.3,0.05,0.1> translate <1.825,2.45> }	// Upper-right bracket
    box { <0,0,0>, <0.3,0.05,0.1> translate <1.825,0.8,0> }	// Lower-right bracket
  }

//
// This is all the ironwork in the gate
//
#declare IronWork =
  union {
    object { Lettering translate <0,4,-0.05> }
    object { Fence }
    object { Fence scale <-1,1,1> }

    object { Brackets translate -0.05*z }

    object { LeftDoor rotate -0*y translate -1.9*x }
    object { RightDoor rotate 0*y translate 1.9*x }

    texture { Wrought_Iron }
  }

   fog {
    distance 1500
    color rgb<0.3, 0.4, 1.0>
    fog_type 2
    fog_offset 100
    fog_alt 1
    turbulence 0.1
    turb_depth 0.2
  }


  sky_sphere {
    pigment {
      gradient y
      colour_map {
//        [0.0 1.0  colour red 0.1 green 0.3 blue 1.0
//                  colour red 0.1 green 0.6 blue 1.0]

//        [0.0 1.0  colour red 0.4 green 0.4 blue 1.0
//                  colour red 0.6 green 0.7 blue 1.0]

        [0.0 1.0
		  colour red 0.3 green 0.5 blue 0.6
                  colour red 0.3 green 0.4 blue 1.0
	]


//        [0.0 1.0  colour red 0.0 green 0.0 blue 0.0
//                  colour red 0.0 green 0.0 blue 1.0]
      }
      scale 0.5
    }
  }


#declare BoardWidth=2.8;
#declare BoardHeight=0.5;
#declare BoardGap = 0.025;

#declare Board =
  difference {
    box { <0,0,0>, <BoardWidth,BoardHeight,0.1> }
  }

#declare SignBase =
  union {
    object { Board translate (1.5+0*(BoardHeight+BoardGap))*y }
    object { Board translate (1.5+1*(BoardHeight+BoardGap))*y }
    object { Board translate (1.5+2*(BoardHeight+BoardGap))*y }

    box { <0.0,0,0.12>, <0.1,1.5+3*BoardHeight+2*BoardGap,0.22> }
    box { <BoardWidth-0.1,0,0.12>, <BoardWidth,1.5+3.1*BoardHeight,0.22> }

    translate -BoardWidth/2*x

    //Other possible woods: 18, 19, 20, 29, 30, 31
    texture { T_Wood31 scale 0.5 }
  } 

#declare CP_X = -BoardWidth/2+0.2;
#declare CP_Y = 2.7;

#macro newline()
  #declare CP_X = -BoardWidth/2+0.2;
  #declare CP_Y = CP_Y - (BoardHeight+BoardGap);
#end

#macro kern(C)
  #declare CP_X = CP_X + 0.22*C;
#end

#macro putchar(C)
    object {
	C scale <0.05,0.05,0.05>
	translate <CP_X,CP_Y,-.01> 
	texture { T_Wood4 }
    }

  #declare CP_X = CP_X + 0.22;
#end

#macro putschar(C)
    object {
	C scale <0.04,0.04,0.04>
	translate <CP_X,CP_Y,-.01> 
	texture { T_Wood4 }
    }

  #declare CP_X = CP_X + 0.22*0.04/0.05;
#end

/////////////////////////////////////////////////////////////////////////////
//
// Put up the sign base and subtract out all of the letters.
//
/////////////////////////////////////////////////////////////////////////////
#declare Sign =
  difference {
    object { SignBase }

//Opening Gates to
//Digital Circuit
//Simulation

    putchar(char_T)
    putschar(char_H)
    putschar(char_E)
    kern(0.8)
    putchar(char_G)
    putschar(char_A)
    putschar(char_T)
    putschar(char_E)
    putschar(char_W)
    putschar(char_A)
    putschar(char_Y)
    kern(0.8)
    putchar(char_T)
    putschar(char_O)

    newline()

    putchar(char_D)
    kern(-.15)
    putschar(char_I)
    kern(-.15)
    putschar(char_G)
    kern(-.15)
    putschar(char_I)
    kern(-.15)
    putschar(char_T)
    putschar(char_A)
    putschar(char_L)
    kern(0.8)
    putchar(char_C)
    kern(-.15)
    putschar(char_I)
    kern(-.15)
    putschar(char_R)
    putschar(char_C)
    putschar(char_U)
    kern(-.15)
    putschar(char_I)
    kern(-.15)
    putschar(char_T)
    newline()

    putchar(char_S)
    kern(-.15)
    putschar(char_I)
    kern(-.15)
    putschar(char_M)
    putschar(char_U)
    putschar(char_L)
    putschar(char_A)
    putschar(char_T)
    kern(-.15)
    putschar(char_I)
    kern(-.15)
    putschar(char_O)
    putschar(char_N)
  }

#declare SpineSet =
  union {
    #local SpineR = 0.005;
    #local SpineLen = 0.1;

    object { cone { <0,0,0>, SpineR, <SpineLen,0,0>, 0.0 } rotate 20*z }
    object { cone { <0,0,0>, SpineR, <SpineLen,0,0>, 0.0 } rotate -20*z rotate 20*y }
    object { cone { <0,0,0>, SpineR, <SpineLen,0,0>, 0.0 } rotate -20*z rotate -20*y }
    texture { CactusSpine_Text } 
  }

#macro SpineRow(X1,X2,Y)
  union {
    #local X = X1;
    #while (X <= X2)
      object { SpineSet rotate 90*y translate X*x }
      #local X = X + 0.05;
    #end
    translate Y*y
  }
#end

#declare CactusAndLeaf =
  union {
    #local TT = 0.05;
    union {
      box { <-0.35,-0.25,-TT>, <0,0.25,TT> }
      cylinder { -TT*z, TT*z, 0.25 }
      translate <-0.25,0,0> 
    }

    object { SpineRow(-0.55,-0.1,0) translate -TT*z }
    object { SpineRow(-0.55,-0.1,0.1) translate -TT*z }
    object { SpineRow(-0.55,-0.1,-0.1) translate -TT*z }
    object { SpineRow(-0.55,-0.15,0.2) translate -TT*z }
    object { SpineRow(-0.55,-0.15,-0.2) translate -TT*z }
    union {
      object { SpineRow(-0.55,-0.1,0) translate -TT*z }
      object { SpineRow(-0.55,-0.1,0.1) translate -TT*z }
      object { SpineRow(-0.55,-0.1,-0.1) translate -TT*z }
      object { SpineRow(-0.55,-0.15,0.2) translate -TT*z }
      object { SpineRow(-0.55,-0.15,-0.2) translate -TT*z }
      scale <1,1,-1>
    }

    scale 0.9
  }

#declare CactusOrLeaf =
  union {
    #local TT = 0.05;
    intersection {

      union {
        box { <0,0,-TT>, <0.25,0.5,TT> }
        intersection {
	  box { <0,0,-TT>, <0.7,0.5,TT> }
	  object { cylinder { -1*z, 1*z, 0.5 } translate <0.25,0,0> }
	  object { cylinder { -1*z, 1*z, 0.5 } translate <0.25,0.5,0> }
        }
      }
      object { cylinder { -1*z, 1*z, 0.5 } inverse translate <-0.4,0.25,0> }
      translate <-.7,-.25,0>
    }

    object { SpineRow(-0.55,-0.1,0) translate -TT*z }
    object { SpineRow(-0.55,-0.15,0.1) translate -TT*z }
    object { SpineRow(-0.55,-0.15,-0.1) translate -TT*z }
    object { SpineRow(-0.6,-0.3,0.2) translate -TT*z }
    object { SpineRow(-0.6,-0.3,-0.2) translate -TT*z }
    union {
      object { SpineRow(-0.55,-0.1,0) translate -TT*z }
      object { SpineRow(-0.55,-0.15,0.1) translate -TT*z }
      object { SpineRow(-0.55,-0.15,-0.1) translate -TT*z }
      object { SpineRow(-0.6,-0.3,0.2) translate -TT*z }
      object { SpineRow(-0.6,-0.3,-0.2) translate -TT*z }
      scale <1,1,-1>
    }

    scale 0.9
  }

#macro Cactus()
  union {
    #local CW = 0.2;
    #local CH = 0.4;

    sphere { <0,0,0>, CW scale <1,CH/CW,1> }

    #local RZ1 = 20*(rand(cactusRand)-1)-5;
    #local RZ2 = 20*(rand(cactusRand)-0);
    #local RX1 = 20*(rand(cactusRand)-0.5);
    #local RX2 = 20*(rand(cactusRand)-0.5);
    #local RY = 60*(rand(cactusRand)-0.5);
    #local L1 = rand(cactusRand);
    #local L2 = rand(cactusRand);

    #if (L1 < 0.5)
      object { CactusOrLeaf rotate 30*x rotate RX1*z rotate RZ1*z translate <-0.15,-0.1,0> }
    #else
      object { CactusAndLeaf rotate 30*x rotate RX1*z rotate RZ1*z translate <-0.18,-0.1,0> }
    #end

    #if (L2 < 0.5)
      object { CactusOrLeaf rotate 180*z rotate RX2*x rotate RZ2*z translate <0.15,0.1,0> }
    #else
      object { CactusAndLeaf rotate 180*z rotate RX2*x rotate RZ2*z translate <0.18,0.1,0> }
    #end

    #local A = 0;
    #while (A < 360)


	#local B = -30;
	#while (B < 60)
	  #local DX = CW*cos(B*Pi/180);
	  #local DY = CH*sin(B*Pi/180);


 	  object { SpineSet 
	    rotate B*z
	    translate <DX,DY,0>
	    rotate A*y }

	  #local B = B + 10;
        #end

	#local A = A + 30;
    #end    

    rotate RY*y
    texture { Cactus_Text }
  }
#end

#declare Cacti =
  union {
    object { Cactus() translate <-5,.45,-8> }
    object { Cactus() translate <-8.9,.7,-4.7> } // Wing reflecting
    object { Cactus() translate <5.5,.4,-7> }
    object { Cactus() translate <2,.42,-8> }
    object { Cactus() translate <8,.45,-6> }

  }

//
// Put it all together here.
//
object { GateGround }					// The ground
object { GateClouds }					// The clouds
#if (showPosts)
  object { Post translate -(1.95+PostRadius)*x }	// The left gate post
  object { Post translate (1.95+PostRadius)*x }		// The right gate post
#end
#if (showFence)
  object { IronWork }					// The iron work
#end
#if (showDragons)
  #if (DGView = 7)
    object { Dragon(3) rotate 0*y scale 0.4 translate <2,11,20> }
  #else
    object { Dragon(1) rotate 30*y scale 0.4 translate <-4.5,2.3,-4> }
    object { Dragon(2) rotate 30*y scale <-0.4,0.4,0.4> translate <4.3,2.1,-4> }
  #end
#end
#if (showSign)
  object { Sign rotate -40*y translate <-3.8,0,-10> }
#end
#if (showCacti)
  object { Cacti }
#end
#if (showTrees)
  object { TREE scale 6 translate <-11,0.2,5>  }
  object { TREE scale 6 translate <-4,0.8,7> }
  object { TREE scale 6 translate <10,1.3,7> }
#end

//
// Where are we anyway?
//
camera {
  #switch (DGView)
  #case (1)
    location  <6.0, 7.25, -20>		// Full view
    look_at <0,2.25,0>
  #break
  #case (2)
    location  <-4.5, 2, -8>		// left dragon feet
    look_at <-4.5,0,-4>
  #break
  #case (3)
    location  <4.3, 2, -8>		// left dragon feet
    look_at <4.4,0,-4>
  #break
  #case (4)
    location  <-5, 0.6, -9.5>		// Cactus view
    look_at <-5,.6,-8>
  #break
  #case (5)
    location  <0.0, 2.25, -8>		// Closeup
    look_at <0,2.25,0>
  #break
  #case (6)				// Sign closeup
    location <-3,2,-13>
    look_at <-3.8,2,-10>
    up <0,1,0>
  #break
  #case (7)				// Gate closeup
    location <0,2.5,-6>
    look_at <0,2.5,0>
    up <0,1,0>
  #end
}

//light_source { <50.0, 40.0, -20.0> colour White *2 }
//light_source { <-99.0, 79.0, 20.0> colour White *1 }
//light_source { <0, 5.0, -20.0> colour White *1 }
//light_source { <0, 20.0, 20.0> colour White *1 }


//
// Let there be light!
//
#if (DGView = 7)
light_source {
  <200.0, 250.0, -250>
  colour White *3
  shadowless
}

light_source {
  <-50.0, 90.0, -30>
  colour White*0.1
  shadowless
}
#else
light_source {
  <200.0, 250.0, -250>
  colour White *3
}

light_source {
  <-50.0, 90.0, -30>
  colour White*0.1
  shadowless
}
#end

