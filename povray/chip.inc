#declare Chip_Text =
  texture {
    pigment { rgb <.1,.1,.1>  }
    finish {
//      metallic
      specular 1
//      roughness 0.1
//      ambient 0.2
//      diffuse 0.2
//      reflection .2
    }
  }

#declare Pin_Text =
  texture {
    pigment { Silver }
    finish {
      metallic
      specular 1
//      roughness 0.1
//      ambient 0.2
//      diffuse 0.2
      reflection .2
    }
  }


#declare Chip =
  union {
    box { <-0.5,-0.1,-1>, <0.5, 0.1, 1> texture { Chip_Text } }

    #local i = 0;
    #local Q = 0.3;
    #local W = 0.15;
    #local L = 0.3;
    #while (i <= 3)
      #local Z = -1 + Q + (i/3.0)*(2-2*Q);
      box { <-0.45,0,Z-W>, <-0.45-L,-0.1,Z+W> texture { Pin_Text } }
      box { <0.45,0,Z-W>, <0.45+L,-0.1,Z+W> texture { Pin_Text } }

      #local i = i + 1;
    #end

    rotate x*-90
    rotate y*0
  }
