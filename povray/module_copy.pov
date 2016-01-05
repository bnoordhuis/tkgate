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
    pigment { rgb <0.7*.87, 0.7*.8, 0.7*.77>  }
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


#declare Ink_Text =
  texture {
    pigment { rgb <0.05, 0.05, 0.05>  }
    finish {
      specular 0.0
      ambient 0.0
    }
  }

#declare Chip_Text =
  texture {
    pigment { rgb <.6,0,.6>  }
    finish {
//      metallic
      specular 0.2
//      roughness 0.1
      ambient 0.5
//      diffuse 0.2
//      reflection .2
    }
  }

#declare Erase_Text =
  texture {
    pigment { rgb <.6,.3,.3>  }
    finish {
      specular 0
      roughness 0.3
      ambient 0.2
    }
  }

#declare Yellow_Paint =
  texture {
    pigment { rgb <.9,0.9,0.1>  }
    finish {
      specular 0.2
      ambient 0.1
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

#declare Paper_Text =
  texture {
    pigment { rgb <1,0.95,0.95>  }
    finish {
      specular 0.1
      roughness 0.1
      ambient 0.8
      diffuse 0.2
    }
  }

#declare Pencil =
  union {
    #local L = 1.5;
    #local R = 0.15;
    #local Q = 0.03;
    #local X = 10;
    #local D = 0.5;
    #local E = 0.3;
    intersection {
      intersection {
        box { <0,-R,-R>, <L,R,R> }
        object { box { <0,-R,-R>, <L,R,R> } rotate 45*x }
        texture { Yellow_Paint }
      }
      object { cone { <0.0,0,0> , 0, <D*X, 0, 0>, R*X} texture { T_Wood3} }
    }
    intersection {
      object { cylinder { 0*x, L*x, Q } }
      object { cone { <-0.01,0,0> , 0, <D*X, 0, 0>, R*X} }
    }
    object { cylinder { L*x, (L+E)*x, R } texture { Erase_Text } }
    object { cylinder { L*x, (L+E/2)*x, 1.01*R } texture { T_Silver_1A } }
  }

#declare Chip =
  union {
    #local W = 0.75;
    #local H = 1.0;
    #local D = 0.1;
    #local T = 0.15;

    intersection {
      box { <-W,-D,-H>, <W, D, H> }
      box { <-(W-T),-2*D,-(H-T)>, <W-T, 2*D, H-T> inverse }
      texture { Chip_Text }
    }
    object { box { <-(W-T),-D,-(H-T)>, <W-T,D,H-T> } texture { Paper_Text }}

    object { text { ttf "cyrvetic.ttf" "ALU32" 0.3, 0 }
	scale 0.3
	rotate 90*x
	translate <-0.5,0.101,0.55>
	texture { Ink_Text }
    }

    #local i = 0;
    #local PS = W - 0.05;
    #local Q = 0.3;
    #local PW = 0.05;
    #local PD = 0.05;
    #local L = 0.3;
    #while (i <= 3)
      #local Z = -1 + Q + (i/3.0)*(2-2*Q);
      box { <-PS,-PD,Z-PW>, <-(PS+L),PD,Z+PW> texture { Pin_Text } }
      box { <PS,-PD,Z-PW>, <PS+L,PD,Z+PW> texture { Pin_Text } }
      #local i = i + 1;
    #end

    rotate x*-90
  }


#macro Paper(W,H,D)
  union {

    box { <-W/2,-H/2,-D/2>, <W/2, H/2, D/2> }

    texture { Paper_Text }
  }
#end

//
// A cut-out the same size as a tray
//
#macro TrayForm(W,H,D)
  object {
       box { <-TW/2,-TH/2,-TD/2>, <TW/2,TH/2,TD/2>  }
  }
#end

#macro Tray(W,H,D) 
  union {
    #local X = 0.25;
    #local Q = 0.25;

    intersection {
      box { <-W/2,-H/2,-D/2>, <W/2,H/2,D/2> }
      box { <-W/2+X,-H/2+X,-D/2+X>, <W/2-X,H,D/2> inverse}
      texture { Plastic_Text } 
    }

    box { <-W/2,H/2-Q,-D/2>, <W/2,H/2,D/2> texture { NBglass } }

    object { Paper(W*.8,H*.8,D*.8)  }

  }
#end

#macro OutTray(W,H,D) 
  union {
    #local X = 0.25;
    #local Q = 0.25;

    intersection {
      box { <-W/2,-H/2,-D/2>, <W/2,H/2,D/2> }
      box { <-W/2+X,-H/2+X,-D/2+X>, <W/2-X,H,D/2> inverse}
      texture { Plastic_Text } 
    }
    object { Chip rotate 90*x rotate 180*y scale 5 translate 1*z }
  }
#end

#macro OutputWell(W,H,D)
  union {
    #local Q = 5;
    #local HH1 = -0.5;
    #local HH2 = -2;
    intersection {
      plane { z, D/2 rotate -10*x }
      plane { z, -D/2 inverse }
      plane { y, -H/2 inverse rotate 30*x }
      plane { x, W/2  }
      plane { x, -W/2 inverse  }
      plane { y, H/2 }
    }
    box { <-W/2*.9,HH1,D/2> , <W/2*.9,HH2,D/2 + Q> }
  }
#end

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


#macro Display(W,H,D)
  union {
    box { <-W/2, -H/2, -D/2>, <W/2,H/2,D/2>     texture { Plastic_Text } }
    union {
//      box { <-W/2,H/5 , -D/2>, <-W/2+2,-H/5,-D/2-0.25>    texture { DarkPlastic_Text } }
//      box { <W/2-2,H/5 , -D/2>, <W/2,-H/5,-D/2-0.25>    texture { DarkPlastic_Text } }

      box { <-W/2,H/5 , -D/2>, <W/2,-H/5,-D/2-0.25>    texture { DarkPlastic_Text } }

      box { <-W/2,H/5 , -D/2>, <W/2,-H/5,-D/2-0.5>     texture { NBglass } }
      translate 0.2*H*y
    }
  }
#end

#declare Buttons =
  union {
    sphere { <0,0,0>, 1 scale <0.6,0.3,1> translate <-1,0,0> texture {GreenPlastic_Text}}
    sphere { <0,0,0>, 1 scale <0.6,0.3,1> translate <-1,0,-3>   texture {RedPlastic_Text}}

    #local d = 0;
    #while ( d < 4 )
      sphere { <0,0,0>, 1 scale <0.6,0.3,0.1> translate <1,0,-d> texture { Plastic_Text}}
      #local d = d + 1;
    #end
  }


#declare Copier =
  union {
    #local W = 20;
    #local H = 10;
    #local D = 20;
    #local TW = 12;
    #local TH = 2;
    #local TD = 10;
    #local Tout = -(D/2);
    #local T1y = 2;
    #local T2y = -2;
    #local Xshft = -2;
    #local R = 0.5;
    #local PW = 12;
    #local PD = 14;

    // main box
    intersection {
       RoundBox(W/2,H/2,D/2,R)
//       box { <-W/2,-H/2,-D/2>, <W/2,H/2,D/2> }
       object { TrayForm(TW,TH,TD) inverse translate <Xshft,T1y,Tout> }
       object { TrayForm(TW,TH,TD) inverse translate <Xshft,T2y,Tout> }
//       object { OutputWell(TW,H*0.7,D*0.5) inverse translate <Xshft,H/2,0> }

      box { <-PW/2,0,-PD/2>, <PW/2,H/2,PD/2> inverse translate <-3,1,0> }

      texture { Plastic_Text }
    }

    box { <-PW/2,H/2-Q,-PD/2>, <PW/2,H/2,PD/2> texture { NBglass } translate <-3,0.5,0> }

    box { <-PW/2,H/2-Q,-PD>, <PW/2,H/2,0> texture { Plastic_Text } rotate 30*x translate <-3,0.5,PD/2> }

    object { Chip rotate 90*x rotate 180*y rotate 180*z scale 5 translate <-3,6,1> }

    object { Buttons translate <W/2-3,H/2 + R,-3> }

    object { Display(5,3,3) rotate x*30 translate <(W/2-3),H/2,0> }
    
    object { OutTray(TW,TH/2,TD) translate <Xshft,T1y,Tout> }
    object { Tray(TW,TH,TD) translate <Xshft,T2y,Tout> }
  }

union {
  object { Copier }
//  rotate y*40
//  rotate z*-20
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
#declare DX = 1.5;
camera {
  location  <-10.0*DX, 20.0*DX, -35*DX>

  look_at <0.0, 0.0,  0.0>
}

light_source {
  <100,200, -300>
  colour White *1.2
}

