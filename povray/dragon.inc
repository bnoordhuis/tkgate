// Dragon
// by Jeffery P. Hansen
//
#include "glass.inc"

#declare ShowHead = 1;
#declare ShowWings = 1;
#declare ShowBody = 1;
#declare ShowLegs = 1;
#declare ShowFrills = 1;
#declare ShowGem = 1;

#declare DrBodyR = 1.5;
#declare DrBodyStart = -3;
#declare DrBodyEnd = 3;

#declare DrToe1Len = 1;
#declare DrToe2Len = 0.8;
#declare DrToeR = 0.35;
#declare DrToeA1 = 20;
#declare DrToeA2 = -40;

#declare DrThumb1Len = 0.4;
#declare DrThumb2Len = 0.4;
#declare DrThumbR = 0.4;
#declare DrThumbA1 = 5;
#declare DrThumbA2 = -20;

#declare DrClawStart = 0;
#declare DrClawEnd = 1;
#declare DrClawR = 0.2;

#declare Pi = 3.1415926;

// This can go in a "normal"
//
//	slope_map {
//	      [0   <0, 1>]   // start at bottom and slope up
//	      [0.5 <1, 1>]   // halfway through reach top still climbing
//	      [0.5 <1,-1>]   // abruptly slope down
//	      [1   <0,-1>]   // finish on down slope at bottom
//	}


#declare Gem_Texture =
texture { 
    NBglass
    pigment { rgbf <0.1, 1, 1, 0.9> }
}

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

#declare ChestPlate_Text =
  texture {
    pigment { 
	rgb <.7,.7,.2>
    }
    normal {
	crackle 0.7
	scale 0.5
    }
    finish {
      metallic
      specular .05
      roughness 0.1
      ambient 0.2
      diffuse 0.4
      reflection 0.5
    }
  }

#declare DragonHide =
  texture {
    pigment { 
	rgb <.8,.2,.2>
    }
    /*
    pigment { 
	wrinkles
	scale 1
	turbulence 1
	color_map {
 	[0.0 rgb <1,0,0>]
 	[0.1 rgb <1,0,0>]
 	[0.2 rgb <1,1,0>]
 	[0.4 rgb <0,1,0>]
 	[0.6 rgb <0,1,1>]
 	[0.8 rgb <0,0,1>]
        }
    }
    */

    normal {
      //	crackle 0.7
      	crackle 0.1
	scale 0.5
    }
    finish {
      metallic
      specular .05
      roughness 0.1
      ambient 0.2
      diffuse 0.4
    }
  }

#declare NostrilInsides =
  texture {
    pigment { 
	rgb <.2,0,0>
    }
    normal {
	crackle 0.7
	scale 0.5
    }
    finish {
      specular 0
      roughness 0.1
      ambient 0.0
      diffuse 0.1
    }
  }

#declare DragonWingText =
  texture {
    pigment { 
	wrinkles
	scale .1
	color_map {
 	[0 rgbf <.8,.2,.2,.7>]
 	[0.35 rgbf <.8,.2,.2,.7>]
 	[0.75 rgbf <.1,.1,.1,.7>]
        }
    }
//    normal {
// 	agate 5
//	scale 2
//	scale 0.05
//    }
    finish {
      metallic
      specular .05
      roughness 0.1
      ambient 0.2
      diffuse 0.4
//      reflection 0.6
      reflection 1
    }
  }

#declare DragonFrillText =
  texture {
    #local F = 0.1;
    pigment { 
	wrinkles
	scale .1
	turbulence 0.1
	color_map {
 	[0 rgbf <.4,.4,.6,F>]
 	[0.1 rgbf <1,1,0,F>]
 	[0.6 rgbf <1,0,0,F>]
        }
    }
//    normal {
// 	agate 5
//	scale 2
//	scale 0.05
//    }
    finish {
      //      metallic
      //      specular .05
      // roughness 0.1
      ambient 1
      diffuse 0.5
      reflection 0.1
    }
  }

#declare XDragonWingText =
  texture {
    pigment { 
	rgbf <.8,.2,.2,.3>
    }
    normal {
 	bozo 2
	scale 0.05
    }
    finish {
      metallic
      specular .05
      roughness 0.1
      ambient 0.2
      diffuse 0.4
      reflection 0.2
    }
  }

#declare DragonWingRibText =
  texture {
    pigment { 
	rgb <.5,0,0>
    }
    normal {
	crackle 0.2
	scale 0.01
    }
    finish {
      ambient 0.2
      diffuse 0.4
      specular .1
      reflection .1
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

#declare ToothText =
  texture {
    pigment { rgb <1,1,0.7> }
    finish {
      specular 0.1
      ambient 0.5
      diffuse 0.5 
    }
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
  #local YS = 1.0;
  #local L1 = 4;	// Thigh length
  #local L2 = 3;	// Calf length

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
	translate -(L2+0.5)*y
	rotate z*A2
	translate -L1*y
	rotate -A1*z

	translate -(0.8)*x
	translate -(1-FS)*y

	texture { PlateText }
    }

    translate DrBodyR*z
  }
#end

#macro DragonBackLeg(A1,A2) 
  DragonLeg(A1,A2,2,1.2,1.0,1.0)
#end

#macro DragonFrontLeg(A1,A2) 
  DragonLeg(A1,A2,2,1,0.8,0.85)
#end



#declare DragonPlate =
  union {
    #local H = sqrt(0.75);
    #local R1 = 0.3;
    #local R2 = 0.15;
    intersection {
      box { <-0.5,0,-0.05>,  <0.5,2,0.05> }
      plane {x, 0 inverse rotate -30*z translate -0.5*x }
      plane {x, 0 rotate 30*z translate 0.5*x }
    }
    difference {
      cylinder { -0.05*z, 0.05*z, R1 translate (H+R1-0.05)*y }
      cylinder { -0.06*z, 0.06*z, R2 translate(H+R1-0.05)*y }
    }
    texture { PlateText }
  }

#macro DragonPlateSet()
  union {
    //
    // Body plates
    //
    #declare X = DrBodyStart;
    #local T = transform { translate DrBodyStart*x }
    #declare S = 5.0;
    #while (X < DrBodyEnd)
      object { DragonPlate  transform T translate BS*(DrBodyR*cos(X/6)-.2)*y }

      #declare i = 0;
      #while(i < (S + 0.01))
        #local T = transform { translate 0.2*x rotate BodyRdeltaY*y transform T }
        #declare X = X + 0.2;
        #declare i = i + 1;
      #end
    #end

    //
    // Tail plates
    //
    #declare TRStart = DrBodyR*cos(DrBodyStart/6);
    #declare TailRadius = TRStart;
    #declare Y = 0;
    #declare X = DrBodyStart;
    #declare Rdelta = -2;
    #declare RZdelta = 2;
    #local T = transform { translate DrBodyStart*x }
    #while (TailRadius > .2)
      #declare PS = (TailRadius+2*TRStart)/(3*TRStart);
      object { DragonPlate scale PS transform T translate (BS*TailRadius-0.2)*y }

      #declare i = 0;
      #while(i < (S+0.01))
        #local T = transform { rotate Rdelta*y rotate RZdelta*z translate -0.2*x transform T }
        #declare TailRadius = TailRadius - 0.02;
        #declare Rdelta = Rdelta + TailDDY;
        #declare RZdelta = RZdelta + TailDDZ;
        #declare i = i + 1;
      #end
    #end

    //
    // Neck plates
    //
    #declare NeckRadius = DrBodyR*cos(DrBodyEnd/6);
    #declare Y = 0;
    #declare X = DrBodyStart;
    #declare Rdelta = NeckHorzStart;
    #declare RZdelta = NeckVertStart;
    #local T = transform { transform Tlegs }
    #while (NeckRadius > .5)
      #declare PS = (NeckRadius+3*TRStart)/(4*TRStart);
      object { DragonPlate scale PS transform T translate BS*(NeckRadius-0.2)*y }

      #declare i = 0;
      #while(i < (S+0.01))
        #local T = transform { rotate Rdelta*y rotate RZdelta*z translate 0.2*x transform T }
        #declare NeckRadius = NeckRadius - 0.02;
        #declare Rdelta = Rdelta + NeckHorzDD;
        #declare RZdelta = RZdelta + NeckVertDD;
        #declare i = i + 1;
      #end
    #end
  }
#end

#macro MainBodyBlob()
    blob {
      threshold 0.05


      //
      // Main body
      //
      #declare X = DrBodyStart;
      #local T = transform { translate DrBodyStart*x }
      #while (X < DrBodyEnd)
        cylinder { 0*x, 0.1*x, DrBodyR*cos(X/6),1  scale <1,BS,1> transform T }
        #local T = transform { translate 0.2*x rotate BodyRdeltaY*y transform T }

        #if (X < DrBodyEnd*0.5)
          #declare Twings = transform { transform T}
        #end

        #declare X = X + 0.2;
      #end
      #declare Tlegs = transform { transform T}

      //
      // Tail
      //
      #declare TailRadius = DrBodyR*cos(DrBodyStart/6);
      #declare Y = 0;
      #declare X = DrBodyStart;
      #declare Rdelta = -2;
      #declare RZdelta = 2;
      #local T = transform { translate DrBodyStart*x }
      #while (TailRadius > 0)
        cylinder { 0*x, 0.1*x, TailRadius,1 scale <1,BS,1> transform T }

        #local T = transform { rotate Rdelta*y rotate RZdelta*z translate -0.2*x transform T }

        #declare TailRadius = TailRadius - 0.02;
        #declare Rdelta = Rdelta + TailDDY;
        #declare RZdelta = RZdelta + TailDDZ;
      #end

      // Neck
      #declare NeckRadius = DrBodyR*cos(DrBodyEnd/6);
      #declare Y = 0;
      #declare X = DrBodyStart;
      #declare Rdelta = NeckHorzStart;
      #declare RZdelta = NeckVertStart;
      #local T = transform { transform Tlegs }
      #while (NeckRadius > .5)
        cylinder { 0*x, 0.1*x, NeckRadius,1 scale <1,BS,1> transform T }

        #local T = transform { rotate Rdelta*y rotate RZdelta*z translate 0.2*x transform T }

        #declare NeckRadius = NeckRadius - 0.02;
        #declare Rdelta = Rdelta + NeckHorzDD;
        #declare RZdelta = RZdelta + NeckVertDD;
      #end
      #declare Thead = transform { transform T}

      texture { DragonHide }
    }
#end

#macro BodyChestPlates()
    union {
      #local PlateS = 0.85;
      #local PlateDY = -(1-PlateS);

      //
      // Main body
      //
      #declare X = DrBodyStart;
      #local T = transform { translate DrBodyStart*x }
      #while (X < DrBodyEnd)
        cylinder { 0*x, 0.2*x, DrBodyR*cos(X/6)
	  scale <PlateS,PlateS*BS,PlateS> transform T translate PlateDY*y}
        #local T = transform { translate 0.2*x rotate BodyRdeltaY*y transform T }

        #if (X < DrBodyEnd*0.5)
          #declare Twings = transform { transform T}
        #end

        #declare X = X + 0.2;
      #end
      #declare Tlegs = transform { transform T}

      //
      // Tail
      //
      #declare TailRadius = DrBodyR*cos(DrBodyStart/6);
      #declare Y = 0;
      #declare X = DrBodyStart;
      #declare Rdelta = -2;
      #declare RZdelta = 2;
      #local T = transform { translate DrBodyStart*x }
      #while (TailRadius > 0)
        cylinder { 0*x, 0.2*x, TailRadius
	  scale <PlateS,PlateS*BS,PlateS> transform T translate PlateDY*y }

        #local T = transform { rotate Rdelta*y rotate RZdelta*z translate -0.2*x transform T }

        #declare TailRadius = TailRadius - 0.02;
        #declare Rdelta = Rdelta + TailDDY;
        #declare RZdelta = RZdelta + TailDDZ;
      #end

      // Neck
      #declare NeckRadius = DrBodyR*cos(DrBodyEnd/6);
      #declare Y = 0;
      #declare X = DrBodyStart;
      #declare Rdelta = NeckHorzStart;
      #declare RZdelta = NeckVertStart;
      #local T = transform { transform Tlegs }
      #while (NeckRadius > .5)
        cylinder { 0*x, 0.2*x, NeckRadius
	  scale <PlateS,PlateS*BS,PlateS> transform T translate PlateDY*y }

        #local T = transform { rotate Rdelta*y rotate RZdelta*z translate 0.2*x transform T }

        #declare NeckRadius = NeckRadius - 0.02;
        #declare Rdelta = Rdelta + NeckHorzDD;
        #declare RZdelta = RZdelta + NeckVertDD;
      #end
      #declare Thead = transform { transform T}

      texture { ChestPlate_Text }
    }
#end

//
// Put the pieces of the dragon body together here.  The local variables are
// visible within the macros. 
//
#macro DragonBody(N)
  union {
    #local BS = 1.5;
    #switch (N)
    #case(1)
      #local BodyRdeltaY = 1;
      #local NeckHorzStart = -1;
      #local NeckHorzDD = 0.085;
      #local NeckVertStart = 0;
      #local NeckVertDD = 0.09;
      #local TailDDY = 0.08;
      #local TailDDZ = -0.09;
    #break
    #case(2)
      #local BodyRdeltaY = 1;
      #local NeckHorzStart = -1;
      #local NeckHorzDD = 0.085;
      #local NeckVertStart = 0;
      #local NeckVertDD = 0.05;
      #local TailDDY = -0.04;
      #local TailDDZ = -0.09;
    #break
    #case(3)
      #local BodyRdeltaY = 1;
      #local NeckHorzStart = -1;
      #local NeckHorzDD = 0.00;
      #local NeckVertStart = 0;
      #local NeckVertDD = 0.0;
      #local TailDDY = 0.08;
      #local TailDDZ = -0.09;
    #end

    MainBodyBlob()
    DragonPlateSet()
    BodyChestPlates()
  }
#end

#declare XMark =
  union {
    #local Q=0.02;
    #local L=1;
    box { <-L,-Q,-Q>, <L,Q,Q> }
    box { <-Q,-L,-Q>, <Q,L,Q> }
    box { <-Q,-Q,-L>, <Q,Q,L> }
  }

#declare Tooth =
  mesh {
    triangle { <-1,0,0>, <0,0,-1>, <0,1,0> }
    triangle { <-1,0,0>, <0,0,1>, <0,1,0> }
    triangle { <1,0,0>, <0,0,-1>, <0,1,0> }
    triangle { <1,0,0>, <0,0,1>, <0,1,0> }

    scale <0.06,0.3,0.1>

    texture { ToothText }
  }

#macro UpperTeeth(ToothLen)
   union {
     #local FrontS = 0.6;
     #local A = 14;
     #local W1 = 0.98;
     #local W2 = 0.92;
     #local W3 = 0.82;

     #local X = 1.3;
     #while (X < ToothLen)
        object { Tooth rotate 90*y scale <0.8,-0.8,0.8>
	   translate X*x
	   rotate A*y
	   translate W1*z
	}
        object { Tooth rotate 90*y scale <0.8,-0.8,0.8>
	   translate X*x
	   rotate -A*y
	   translate -W1*z
	}
	#local X = X + 0.2;
     #end
     object { Tooth rotate 90*y scale <0.8,-1.1,0.8>
	translate X*x
	rotate A*y
	translate W1*z
     }
     object { Tooth rotate 90*y scale <0.8,-1.1,0.8>
	translate X*x
	rotate -A*y
	translate -W1*z
     }
     #local X = X + 0.1;

     object { Tooth rotate 90*y scale <FrontS,-FrontS,FrontS>
	rotate 32*y
	translate X*x
	rotate A*y
	translate W2*z
     }
     object { Tooth rotate 90*y scale <FrontS,-FrontS,FrontS>
	rotate -32*y
	translate X*x
	rotate -A*y
	translate -W2*z
     }
     #local X = X + 0.05;
     object { Tooth scale <FrontS,-FrontS,FrontS>
	translate X*x
	rotate A*y
	translate W3*z
     }
     object { Tooth rotate 90*y scale <FrontS,-FrontS,FrontS>
	rotate 92*y
	translate X*x
	rotate -A*y
	translate -W3*z
     }

     translate (HeadH*0.3+0.1)*y
   }
#end

#macro LowerTeeth(ToothStart,ToothLen)
   union {
     #local FrontS = 0.6;
     #local A = 13;
     #local W1 = 0.73;
     #local W2 = 0.62;
     #local W3 = 0.52;

     #local X = ToothStart;
     #while (X < ToothLen)
        object { Tooth rotate 90*y scale <0.8,0.8,0.8>
	   translate X*x
	   rotate A*y
	   translate W1*z
	}
        object { Tooth rotate 90*y scale <0.8,0.8,0.8>
	   translate X*x
	   rotate -A*y
	   translate -W1*z
	}
	#local X = X + 0.2;
     #end
     object { Tooth rotate 90*y scale <0.8,1.1,0.8>
	translate X*x
	rotate A*y
	translate W1*z
     }
     object { Tooth rotate 90*y scale <0.8,1.1,0.8>
	translate X*x
	rotate -A*y
	translate -W1*z
     }
     #local X = X + 0.15;

     object { Tooth rotate 90*y scale <FrontS,FrontS,FrontS>
	rotate 32*y
	translate X*x
	rotate A*y
	translate W2*z
     }
     object { Tooth rotate 90*y scale <FrontS,FrontS,FrontS>
	rotate -32*y
	translate X*x
	rotate -A*y
	translate -W2*z
     }
     #local X = X + 0.1;
     object { Tooth scale <FrontS,FrontS,FrontS>
	translate X*x
	rotate A*y
	translate W3*z
     }
     object { Tooth rotate 90*y scale <FrontS,FrontS,FrontS>
	rotate 92*y
	translate X*x
	rotate -A*y
	translate -W3*z
     }
   }
#end


#macro DragonJaw(A,JawStart,JawEnd,JawTop)
  object {
    #local X0 = JawStart/2;
    #local Y0 = JawTop*1.5;
    #local X1 = JawStart/2;
    #local Y1 = -JawTop*0.5;
    #local L = JawEnd-JawStart;

    #local JW = 0.7;
    #local JR = 0.6;
    #local JY = -Y1 - 0.1;
    #local JSY = 1;
    #local JSX = 0.8;
    #local JSZ = 0.35;
    #local JZR = 30;

    union {
      intersection {
        union {
	  intersection {
	    union {
	      intersection {     
                sphere { <0,0,0>, 1 scale <L/2,0.6,0.5>  translate <L/2+0.2,0,0> }
 	        plane { z, 0 rotate 13*y translate 0.8*z }
	        plane { z, 0 inverse rotate -13*y translate -0.8*z }

	        plane { x, 0 inverse }
	        plane { y, 0 }
	      }

	      // Under jaw balls
	      sphere { <0,0,-0.3>, 0.5 }
	      sphere { <0,0,0.3>, 0.5 }

	      // Jaw sides
              sphere { <0,0,0>, 1 scale <L*2/3,0.6,0.2> rotate -10*y  translate <L/3,-0.1,-0.5> }
              sphere { <0,0,0>, 1 scale <L*2/3,0.6,0.2>  rotate 10*y  translate <L/3,-0.1,0.5> }
            }

	    plane { y, 0 }
          }

          union {
            cylinder { -JW*z, JW*z, JR scale <JSX,JSY,1> translate <-X1, JY, 0> }
            sphere { <0,0,0>, JR scale <JSX,JSY,JSZ> translate <-X1, JY, -JW> }
            sphere { <0,0,0>, JR scale <JSX,JSY,JSZ> translate <-X1, JY, JW> }
	    rotate JZR*z
          }
        }

        // Mouth insides
        #local MD = 0.48;
        cone { <L-0.3,0,0>, 0.3, <0,0,0>, 0.65 inverse scale <1,MD,1> }
        cylinder { 0.01*x, -2*x, 0.65 inverse scale <1,MD,1> }
        sphere { <L-0.3,0,0>, 0.3 scale <1,MD,1> inverse }
        box { <-2,0,-0.65>,  <0,2,0.65> inverse }

      }

      object { LowerTeeth(0,L-0.2) }

      translate <X1,Y1,0>
    }


    translate <X0,Y0,0>
    rotate -A*z
  }
#end


//
// Basic arc (actually a ring) that is used as parts of the OR gate.
//
#macro DrGateArc(RT)
  difference {
    object { cylinder { 0.0*z, RT*z, 0.5+RT }  }
    object { cylinder { z, -z, 0.5 }  }
  }
#end

#macro XOrGateLine(RT,TT)
  union {
    intersection {
      box { <-2,0,-TT>, <0.7,0.5,TT> }
      plane { x, 0 rotate -60*z translate 0.3*x }
      plane { x, 0 rotate 55*z translate <0.3,0.5+2*RT,0> }

      object { cylinder { -1*z, 1*z, 0.5 } translate <-0.5-RT,0.5/2+RT/2,0> }
      object { cylinder { -1*z, 1*z, 0.5 } inverse translate <-0.6-RT,0.5/2+RT/2,0> }

      texture { DragonFrillText }
    }
    intersection {
      box { <-2,0,-TT/2>, <0.7,0.5,TT/2> }
      plane { x, 0 rotate -60*z translate 0.3*x }
      plane { x, 0 rotate 55*z translate <0.3,0.5+2*RT,0> }

      object { cylinder { -1*z, 1*z, 0.5 } translate <-0.39-RT,0.5/2+RT/2,0> }
      object { cylinder { -1*z, 1*z, 0.5 } inverse translate <-0.51-RT,0.5/2+RT/2,0> }

      texture { NBglass }
    }
  }
#end


#macro OrGateWingSurface(RT,TT)
  intersection {
    union {
      box { <0,RT/2,-TT>, <0.25,0.5+RT/2,TT> }
      intersection {
	box { <0,RT/2,-TT>, <0.7,0.5+RT/2,TT> }
	object { cylinder { 0.0*z, RT*z, 0.5+RT/2 } translate <0.25,0,-RT/2> }
	object { cylinder { 0.0*z, RT*z, 0.5+RT/2 } translate <0.25,0.5+RT,-RT/2> }
      }
    }
    object { cylinder { -1*z, 1*z, 0.5 } inverse translate <-0.4-RT,0.5/2+RT/2,0> }
  }
#end

#macro DrOrBack(RT) 
  intersection {
    #local W = 0.5 + RT;

    object { DrGateArc(RT) }
    plane {x, 0 inverse }
    plane {y, -W/2 inverse }
    plane {y, W/2  }

    translate <-(0.5-RT*1.5),W/2,-RT/2>
  }
#end


#macro OrGateWingRibs(RT,doBack,doFront)
  union {
    box { <0,0.5,-RT/2>, <0.25,0.5+RT,RT/2> }
    box { <0,0.0,-RT/2>, <0.25,RT,RT/2> }

    //
    // The back of the OR gate
    //
    #if (doBack)
      object { DrOrBack(RT) }
    #end

    //
    // Top-half of OR curve.
    //
    intersection {
      intersection {
	object { DrGateArc(RT)  }
        plane {x, 0 inverse }
        translate <0.25,0,-RT/2>
      }
      plane {y, 0.5/2 + RT/2 inverse }
      #if (!doFront)
        plane {x, 0.4 }
      #end
    }

    //
    // Bottom-half of OR curve.
    //
    intersection {
      intersection {
	object { DrGateArc(RT)  }
        plane {x, 0 inverse }
        translate <0.25,0.5+RT,-RT/2>
      }
      plane {y, 0.5/2 + RT/2 }
      #if (!doFront)
        plane {x, 0.4 }
      #end
    }

    texture { DragonWingRibText }
  }
#end

#macro OrGateFrill()
  union {
    #local RT = 0.03;
    #local TT = 0.001;

    object { OrGateWingRibs(RT,0,1) }
    object { OrGateWingSurface(RT,TT) texture { DragonFrillText } }
    translate <-0.75,-0.25,0>
  }
#end

#macro XOrGateFrill()
  union {
    #local RT = 0.03;
    #local TT = 0.001;

    XOrGateLine(RT,TT)
    object { OrGateWingRibs(RT,0,1) }
    object { OrGateWingSurface(RT,TT) texture { DragonFrillText } }
    translate <-0.75,-0.25,0>
  }
#end

//
// One section of the wing
//
#macro OrGateWingElement(RT,TT)
  union {
    object { OrGateWingRibs(RT,1,0) }
    object { OrGateWingSurface(RT,TT) texture { DragonWingText } }
  }
#end

//
// A complete wing
//
#declare DragonWing =
  union {
    #local RT = 0.03;
    #local TT = 0.001;

    object { OrGateWingElement(RT,TT) }
    object { OrGateWingElement(RT,TT) translate (0.5)*y }
    object { OrGateWingElement(RT,TT) translate (1)*y }

    rotate 90*x
#if (1)
    rotate 180*z
#else
    translate -0.5*x
#end

    scale 8
  }


#declare Eye = 
  object {
    #local C1 = 0.16;
    #local C2 = 0.195;

    sphere { <0,0,0>, 0.2 }

    texture {
      pigment {
        gradient x
        color_map {
          [0 rgb <1,1,0.6>]			// Milky white
          [C1 rgb <1,1,0.6>]			// Milky white
          [C1 rgb <0.05,0.3,0.05>]
          [C2 rgb <0.05,0.3,0.05>]
          [C2 rgb <0,0,0>]
          [1 rgb <0,0,0>]			// Black
        }
      }
      finish {
	specular 0.1
	reflection 0.3
	ambient 0.5
      }
    }
  }

#macro XYCord(X1,Y1,X2,Y2,S)
  #local D = sqrt((X1-X2)*(X1-X2) + (Y1-Y2)*(Y1-Y2));
  #local R = 90-atan2((X2-X1),(Y2-Y1))*180/Pi;
  object {
    cylinder { 0*x, D*x, S }
    rotate R*z
    translate <X1,Y1,0>
  }
#end


#macro DragonHead()
  union {
    #local HeadLen = 3;
    #local Hbase = 1;
    #local HeadW = 2;
    #local HeadH = 2;
    #local MouthRoof = (HeadH*0.3+0.1);

    #local EA = 35;

    #local NX = HeadLen-0.05;
    #local NY = 1.2;
    #local NZ = 0.15;

#if (1)
    intersection {
      union {
	intersection {
	  box { <0,0,-HeadW/2>, <HeadLen-0.06,HeadH,HeadW/2> }

	  // Side angles
	  plane { z, 0 rotate 12*y rotate -5*x translate Hbase*z  }
	  plane { z, 0 inverse rotate 5*x rotate -12*y translate -Hbase*z  }

	  // Nose and back of head angle
	  plane { y, 0 rotate -15*z translate HeadH*y  }
	  plane { y, 0 rotate 15*z translate (HeadH-0.4)*y  }

	 object { cylinder { -2*x, 4*x, 1 } rotate -15*z translate 0.9*y }
	}

	//
	// Eye ovals
	//
	sphere { <0,0, 0>, 1 scale <1,0.7,0.35> rotate -20*z rotate -EA*y  translate <1.1, 1.2, -0.3> }
	sphere { <0,0, 0>, 1 scale <1,0.7,0.35> rotate -20*z rotate EA*y   translate <1.1, 1.2,  0.3> }

	//
	// Middle nose ridges
	//
	sphere { <0,0,-0.9>, 1 scale <1.6,0.55,0.2> rotate -5*y  translate <1.15,1.0,0> }
	sphere { <0,0, 0.9>, 1 scale <1.6,0.55,0.2>  rotate 5*y  translate <1.15,1.0,0> }

	//
	// Edge nose ridges
	//
	sphere { <0,0,0>, 1 scale <1.8,0.6,0.2> rotate -12*y  translate <1.35,0.9,-0.6> }
	sphere { <0,0,0>, 1 scale <1.8,0.6,0.2>  rotate 12*y  translate <1.35,0.9,0.6> }

	
	//
	// Nose blob
	//
	blob {
	  threshold 0.05
	  sphere { <NX+0.05,NY,NZ>, 0.2, 1 }
	  sphere { <NX+0.05,NY,-NZ>, 0.2, 1 }

	  cylinder { 0*y, NY*y, 0.18,1 translate <NX,0,-NZ> }
	  cylinder { 0*y, NY*y, 0.18,1 translate <NX,0,NZ> }


	}

      }

      //
      // Eye socket cutouts
      //
      sphere { <0,0,0>, 1 inverse scale <0.5,0.2,0.2> rotate -20*z rotate -EA*y  translate <1.1, 1.6, -0.5> }
      sphere { <0,0,0>, 1 inverse scale <0.5,0.2,0.2> rotate -20*z rotate  EA*y  translate <1.1, 1.6, 0.5> }


      //	
      // Nostril cutouts
      //	
      union {
        cylinder { -0.08*x, 1*x, 0.05 scale <1,1,1> }
        box { <-0.15,-0.05,-0.05>, <1,0.0,0.05>  }
	inverse
	scale 1.5
	rotate <-1,0,5>
	translate <NX,NY,NZ>
	texture { NostrilInsides }
      }
      union {
        cylinder { -0.08*x, 1*x, 0.05 scale <1,1,1> }
        box { <-0.15,-0.05,-0.05>, <1,0.0,0.05>  }
	inverse
	scale 1.5
	rotate <1,0,5>
	translate <NX,NY,-NZ>
	texture { NostrilInsides }
      }



      // Mouth cut-out
      union {
	plane { y, MouthRoof inverse }
	plane { x, HeadLen/3  }
      }


      //
      // Mouth cavity cutout
      //
      cone { <-5,0,0>, 1.4,
 	     <0,0,0>, 0.2
		inverse
		scale <1,0.7,1>
		translate(HeadLen-0.3)*x
		translate MouthRoof*y
      }
      sphere { <0,0,0>, 0.2 inverse
		translate(HeadLen-0.3)*x
		translate MouthRoof*y
      }

      //
      // Back of head plane
      //
      plane { x, 0 inverse }
   }

	
   //
   // Upper teeth
   //
   UpperTeeth(HeadLen-0.2)

   #if (ShowFrills)
   object {
	OrGateFrill()
	scale 2
	rotate -60*y
	translate <0.3,1.2,-0.7>
   }
   object {
	OrGateFrill()
	scale 2
	rotate 60*y
	translate <0.3,1.2,0.7>
   }

   object {
	XOrGateFrill()
	scale 2
	rotate -60*y
	rotate 60*x
	translate <0.3,1.6,-0.5>
   }
   object {
	XOrGateFrill()
	scale 2
	rotate 60*y
	rotate -60*x
	translate <0.3,1.6,0.5>
   }
   #end

   object { Eye translate <0.7, 1.6, -0.55> }
   object { Eye translate <0.7, 1.6,  0.55> }
#end

   sphere { <0,0.5,0>, 1 }

   object { DragonJaw(20,HeadLen/3,HeadLen-0.3,MouthRoof) }

   texture { DragonHide }
  }
#end

#macro GemCut(TT)
intersection {
    #local CutD = TT*0.7;
    #local A = 30;
    plane { y,0 rotate -A*x translate <0,0,-(TT-CutD)> }
    plane { y,0 rotate -2*A*x translate <0,-CutD,-TT> }

    plane { y,0 rotate A*x translate <0,0,(TT-CutD)> }
    plane { y,0 rotate 2*A*x translate <0,-CutD,TT> }
}
#end

#macro GemHalfCut(TT)
intersection {
    #local CutD = TT*0.7;
    #local A = 30;
    plane { y,0 rotate -A*x translate <0,0,CutD> }
    plane { y,0 rotate -2*A*x translate <0,-CutD,0> }
}
#end

#declare GateGem =
  intersection {
    #local TT = 0.1;
    #local X0 = -0.45;
    #local R = 0.25;
    #local A = 30;
    #local CutD = 0.08;

    merge {
      box { <X0,-R,-TT>, <0,R,TT> }
      cylinder { -TT*z, TT*z, R }
    }

    object { GemCut(TT) translate R*y }
    object { GemCut(TT) scale <1,-1,1> translate -R*y }
    object { GemCut(TT) rotate 90*z translate X0*x }
    object { GemHalfCut(TT) rotate 90*y translate <X0,R,0> }
    object { GemHalfCut(TT) scale <1,-1,1> rotate 90*y translate <X0,-R,0> }


    #local Th = 60;
    #while (Th > -90)
      #local X = R*cos(Th*Pi/180);
      #local Y = R*sin(Th*Pi/180);
      object { GemCut(TT) rotate (Th-90)*z translate <X,Y,0> }
      #local Th = Th - 30;
    #end


    //    plane { y,0 rotate A*z translate <X0+CutD,R,0> }
    //    plane { y,0 rotate 2*A*z translate <X0,R-CutD,0> }

    //    plane { y,0 inverse rotate -A*z translate <X0+CutD,-R,0> }
    //    plane { y,0 inverse rotate -2*A*z translate <X0,-(R-CutD),0> }

    scale 3
    interior { ior 1.5 }
    texture { Gem_Texture }
  }

#macro Dragon(N) 
  union {
    #switch(N)
    #case(1)
      #local WingAngle = 40;
    #break
    #case(2)
      #local WingAngle = 55;
    #break
    #case(3)
      #local WingAngle = 40;
    #end


    #if (ShowBody)
      object { DragonBody(N) }
    #end
    #if (ShowLegs)
      #switch(N)  // Left dragon
      #case(1)
        object { DragonFrontLeg(45,100) transform Tlegs }
        object { DragonFrontLeg(65,130) scale <1,1,-1> transform Tlegs  }
        object { DragonBackLeg(45,100) translate DrBodyStart*x }
        object { DragonBackLeg(45,100) scale <1,1,-1> translate DrBodyStart*x  }
      #break
      #case(2)
        object { DragonFrontLeg(45,108) transform Tlegs }
        object { DragonFrontLeg(65,130) scale <1,1,-1> transform Tlegs  }
        object { DragonBackLeg(45,108) translate DrBodyStart*x }
        object { DragonBackLeg(45,100) scale <1,1,-1> translate DrBodyStart*x  }
      #break
      #case(3)
        object { DragonFrontLeg(70,160) transform Tlegs }
        object { DragonFrontLeg(70,160) scale <1,1,-1> transform Tlegs  }
        object { DragonBackLeg(70,160) translate DrBodyStart*x }
        object { DragonBackLeg(70,160) scale <1,1,-1> translate DrBodyStart*x  }
      #end
    #end
    #if (ShowWings)
      object { DragonWing scale <1,1,-1> transform Twings
	translate -1.5*z rotate WingAngle*x rotate -10*y}
      object { DragonWing scale <1,1,-1> transform Twings
	translate -1.5*z rotate WingAngle*x rotate -10*y
 		scale <1,1,-1> }
    #end
    #if (ShowHead)
      object { DragonHead() scale 0.8
        translate -0.5*y
        #switch(N)
        #case(1)
          rotate -70*z
        #break
	#case(2)
          rotate -50*z
	#break
	#case(3)
          rotate -0*z
        #end
        transform Thead
      }
    #end

    #if (ShowGem)
      #if (N != 3)
        object { GateGem translate <2.2,-4.2,-2.7> }
      #end
    #end

    translate 0.1*y
  }
#end

