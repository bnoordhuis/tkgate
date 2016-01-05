//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Persistence of Vision 3.1 Include File   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// File: TOMTREE.inc                        XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Desc: BAUMGENERATOR FÜR POVRAY           XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Date: MARCH 2000                         XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Auth: TOM AUST                           XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#declare M_BL=LEAF_LENGTH;
#declare M_BH=LEAF_HEIGHT; 
#declare M_BB=LEAF_BREADTH;
#declare M_BN_X=MAX_LEAF_TILT;
#declare M_BN_Y=MAX_LEAF_TURN;
#declare M_BN_Z=LEAF_INCLINATION;
#declare M_BN_Z_SCOPE=LEAF_INCLINATION_SCOPE; 
#declare BL_N_Z=BLOSSOM_INCLINATION;
#declare M_BL_L=BLOSSOM_LENGTH;
#declare M_BL_B=BLOSSOM_WIDTH;
#declare BL_F=BLOSSOM_COLOR;
#declare S_MA_R=BUNCH_MAJOR_RADIUS+0.000001;
#declare S_MI_R=BUNCH_MINOR_RADIUS+0.000001;
#declare V_S_T=VERTICAL_BUNCH_FUNNEL; 
#declare H_S_T=HORIZONTAL_BUNCH_FUNNEL;
#declare S_R_T=ROTATION_BUNCH_FUNNEL;
#declare BL_Z=BLOSSOMS;
#declare BZ=LEAVES;  
#declare SZ=BUNCHES;
#declare ZWZ=TWIGS;
#declare AZ=BRANCHES;
#if (ZWZ>0)
#if (SZ/ZWZ>1)
	#declare B_A_T=BUNCH_AREA_AT_TWIG*((SZ/ZWZ)/((SZ/ZWZ)-1))+0.000001;
	#declare S_NEIG=HORIZONTAL_BUNCH_ALIGNMENT*((SZ/ZWZ)/((SZ/ZWZ)-1));
	#declare V_S_NEIG=VERTICAL_BUNCH_ALIGNMENT*((SZ/ZWZ)/((SZ/ZWZ)-1));
#else
	#declare B_A_T=BUNCH_AREA_AT_TWIG+0.000001;
	#declare S_NEIG=HORIZONTAL_BUNCH_ALIGNMENT;
	#declare V_S_NEIG=VERTICAL_BUNCH_ALIGNMENT;
#end
#else
	#declare B_A_T=BUNCH_AREA_AT_TWIG+0.000001;
	#declare S_NEIG=HORIZONTAL_BUNCH_ALIGNMENT;
	#declare V_S_NEIG=VERTICAL_BUNCH_ALIGNMENT;
#end
#declare S_NEIG=S_NEIG*S_NEIG;
#declare V_S_NEIG=V_S_NEIG*V_S_NEIG;
#declare ZL=FIRST_BUNCH_AT_TWIG+B_A_T+1.1;
#declare S_VJ_A=BUNCH_TAPER_OFF_AT_BRANCH;
#declare S_VJ_B=BUNCH_TAPER_OFF_AT_TREE; 
#declare S_VJ_Z=BUNCH_TAPER_OFF_AT_TWIG;
#declare S_W_S=BUNCH_ALTERNATE_SWITCH;
#declare S_A=FIRST_BUNCH_AT_TWIG+0.000001;
#declare Z_A=FIRST_TWIG_AT_BRANCH+0.000001;
#if (AZ>0) 
#if (ZWZ/AZ>1) 
	#declare Z_B=TWIG_AREA_AT_BRANCH*((ZWZ/AZ)/((ZWZ/AZ)-1))+0.000001;
	#declare ZW_NEIG=HORIZONTAL_TWIG_ALIGNMENT*((ZWZ/AZ)/((ZWZ/AZ)-1));
	#declare V_ZW_NEIG=VERTICAL_TWIG_ALIGNMENT*((ZWZ/AZ)/((ZWZ/AZ)-1)); 
#else
	#declare Z_B=TWIG_AREA_AT_BRANCH+0.000001;
	#declare ZW_NEIG=HORIZONTAL_TWIG_ALIGNMENT;
	#declare V_ZW_NEIG=VERTICAL_TWIG_ALIGNMENT; 
#end 
#else
	#declare Z_B=TWIG_AREA_AT_BRANCH+0.000001;
	#declare ZW_NEIG=HORIZONTAL_TWIG_ALIGNMENT;
	#declare V_ZW_NEIG=VERTICAL_TWIG_ALIGNMENT; 
#end
#declare ZW_NEIG=ZW_NEIG*ZW_NEIG;
#declare V_ZW_NEIG=V_ZW_NEIG*V_ZW_NEIG;
#declare H_S_N=HORIZONTAL_BUNCH_ANGLE; 
#declare V_S_N=VERTICAL_BUNCH_ANGLE;
#declare H_S_N_SP=HORIZONTAL_BUNCH_ANGLE_SCOPE; 
#declare V_S_N_SP=VERTICAL_BUNCH_ANGLE_SCOPE;
#declare S_DREH=BUNCH_TURN_SWITCH;
#declare S_DREH_VERSCHIEBUNG=BUNCH_TURN_SHIFT;
#declare H_ZW_N=HORIZONTAL_TWIG_ANGLE; 
#declare V_ZW_N=VERTICAL_TWIG_ANGLE;
#declare H_ZW_N_SP=HORIZONTAL_TWIG_ANGLE_SCOPE; 
#declare V_ZW_N_SP=VERTICAL_TWIG_ANGLE_SCOPE;
#declare ZW_W_S=TWIG_ALTERNATE_SWITCH;   
#declare ZW_DREH=TWIG_TURN_SWITCH;
#declare AA=FIRST_BRANCH_AT_TRUNK;
#if (AZ>1)
	#declare AB=BRANCH_AREA_AT_TRUNK*(AZ/(AZ-1))+0.000001; 
#else
	#declare AB=BRANCH_AREA_AT_TRUNK+0.000001; 
#end
#declare BG_X=TRUNK_BEND_X;
#declare BG_Z=TRUNK_BEND_Z;
#declare FZ_X=TRUNK_BEND_X_FREQUENCY; 
#declare FZ_Z=TRUNK_BEND_Z_FREQUENCY;
#declare SW=TRUNK_CURVE;
#declare SWFZ=TRUNK_CURVE_FREQUENCY;
#declare KK=TRUNK_SNAG;
#declare SKKFZ=TRUNK_SNAG_FREQUENCY;
#declare ZW_L=ZL+S_MA_R+S_MI_R;
#declare ZE=TWIG_TIP;
#declare AST_L=(Z_A+Z_B+ZW_L); 
#declare AST_ST=BRANCH_THICKNESS_CHANGE;
#declare AST_VK=BRANCH_REDUCTION; 
#declare AST_VJ=BRANCH_TAPER; 
#declare AST_DREH=BRANCH_TURN_SWITCH;
#declare A_W_S=BRANCH_ALTERNATE_SWITCH;
#declare BH=(AA+AB+AST_L);
#declare AW=BRANCH_CURVE*BH/AST_L;
#declare A_KK=BRANCH_SNAG*BH/AST_L; 
#declare A_BG=BRANCH_BEND*BH/AST_L;
#declare ABFZ=BRANCH_BEND_FREQUENCY*.7*(AST_L/(Z_A+Z_B+ZW_L*(1-AST_VJ)));
#declare ZBFZA=TWIG_CURVE_FREQUENCY_ADJUSTMENT*(ZW_L/(ZL+(S_MA_R+S_MI_R)*(1-S_VJ_Z)))/(AST_L/(Z_A+Z_B+ZW_L*(1-AST_VJ)));
#declare RFZA=RAMI_CURVE_FREQUENCY_ADJUSTMENT/(ZW_L/(ZL+(S_MA_R+S_MI_R)*(1-S_VJ_Z)));
#declare AKKFZ=BRANCH_SNAG_FREQUENCY*1.75;
#declare AWFZ=BRANCH_CURVE_FREQUENCY*1.6;
#declare H_ABG_S=HORIZONTAL_BRANCH_BEND_SWITCH;
#declare B_F_1=BOTTOM_COLOR_1; 
#declare B_F_2=BOTTOM_COLOR_2;
#declare K_F_1=TOP_COLOR_1;  
#declare K_F_2=TOP_COLOR_2; 
#declare UE_B_K=TRANSITION_BOTTOM_TOP;
#declare UE_T=TRANSITION_TURB;
#declare ST_F_1=SUNKEN_TRUNK_COLOR;
#declare ST_F_2=RAISED_TRUNK_COLOR;  
#declare ST_VJ=TRUNK_TAPER;
#declare WA=FIRST_ROOT_AT_TRUNK;
#declare W_BG=ROOT_BEND; 
#declare WBFZ=ROOT_BEND_FREQUENCY;
#declare W_W=ROOT_CURVE;
#declare WWFZ=ROOT_CURVE_FREQUENCY;
#declare W_KK=ROOT_SNAG;  
#declare WKKFZ=ROOT_SNAG_FREQUENCY;
#declare W_ST=ROOT_THICKNESS_CHANGE;  
#declare W_L=ROOT_LENGTH;
#declare W_DREH=ROOT_TURN_SWITCH;
#declare ZW_VST=TWIG_THICKNESS_CHANGE; 
#declare AST_E=BRANCH_TIP;
#declare H_AW_S=HORIZONTAL_BRANCH_CURVE_SWITCH; 
#declare A_L_W=BRANCH_LONGITUDINAL_CURVE;
#declare ZW_K_A=TWIG_CURVE_ADJUSTMENT;
#declare AST_SPIEL=BRANCH_RANDOM;
#declare ZW_SPIEL=TWIG_RANDOM;
#declare ASTNEIGUNG=BRANCH_ANGLE;
#declare A_N_SP=BRANCH_ANGLE_SCOPE;
#if (AZ>1)
	#declare A_NEIG=BRANCH_ALIGNMENT*(AZ/(AZ-1)); 
#else	
	#declare A_NEIG=BRANCH_ALIGNMENT; 
#end 
#declare L_R_NR=LAST_RAMIFICATION_NUMBER;
#declare H_B_T=HORIZONTAL_SCAR_SHRINKING;
#declare V_B_T=VERTICAL_SCAR_SHRINKING;
#declare TRCA=TWIG_RAMIFICATION_CURVE_ADJUSTMENT;
#declare R_A=ROOT_ALIGNMENT;
#declare TT=TWIG_TAPER;
#declare A_STR=STRAIGHTEN_TOWARDS_TOP;
#declare T_STR=STRAIGHTEN_TOWARDS_BRANCH_END;
#declare FCaBE=FOLIAGE_COLOR_AT_BRANCH_END;
#declare BPTo=BRANCH_BARK_PROFILE_TAPER_OFF;
#declare arcTaT=BUNCH_ARC_AT_TWIG; 
#declare arcTaB=BUNCH_ARC_AT_BRANCH;
#declare BEE=BRANCH_END_EQUALIZER;
#declare ANS=EXTRA_NOISE_SWITCH;
#declare UNIT_1=AA+AB+(1-AST_VK)*(Z_A+Z_B+(1-AST_VJ)*(S_A+B_A_T+(S_MA_R+S_MI_R+M_BL)*(1-S_VJ_B)*(1-S_VJ_A)*(1-S_VJ_Z)));
#declare RINDE=
texture
{
	pigment
	{
		granite
		cubic_wave
		color_map
		{  
			[0 color rgb ST_F_1]
			[0.35 color rgb ST_F_2]
			[.5 color rgb ST_F_2*2]
			[0.65 color rgb ST_F_2]
			[1 color rgb ST_F_1]
		}
		scale <TRUNK_SCAR_WIDTH,TRUNK_SCAR_HEIGHT,TRUNK_SCAR_WIDTH>*3
	} 
	#if (SCAR_DEPTH>0)
	normal
	{
		granite SCAR_DEPTH/5
		rotate <0,0,0>
		scale <TRUNK_SCAR_WIDTH,TRUNK_SCAR_HEIGHT,TRUNK_SCAR_WIDTH>*3
	}
	#end
	finish
	{
		phong SCAR_DEPTH/5 phong_size 4 
	} 
}          
#declare RINDE1=
texture
{
	pigment
	{
		granite
		cubic_wave
		color_map
		{  
			[0 color rgb ST_F_1]
			[0.35 color rgb (B_F_1+B_F_2+K_F_1+K_F_2)/4]
			[.5 color rgb (B_F_1+B_F_2+K_F_1+K_F_2)/2]
			[0.65 color rgb (B_F_1+B_F_2+K_F_1+K_F_2)/4]
			[1 color rgb ST_F_1]
		}
		scale <TRUNK_SCAR_WIDTH,TRUNK_SCAR_HEIGHT,TRUNK_SCAR_WIDTH>*3
	} 
	#if (SCAR_DEPTH>0)
	normal
	{
		granite SCAR_DEPTH/5
		rotate <0,0,0>
		scale <TRUNK_SCAR_WIDTH,TRUNK_SCAR_HEIGHT,TRUNK_SCAR_WIDTH>*3
	}
	#end
	finish
	{
		phong SCAR_DEPTH/5 phong_size 4 
	} 
} 
#declare BOZO1 =
pigment
{
	bozo
	color_map
	{
		[0 color rgb B_F_1]
		[1 color rgb B_F_2]
	}
	scale BH/(250*(COLOR_TURB+.1)*BH)
}   
#declare BOZO2 =
pigment
{
	bozo
	color_map
	{
		[0 color rgb K_F_1]
		[1 color rgb K_F_2]
	}
	scale BH/(250*(COLOR_TURB+.1)*BH)
}
#declare LAUB=
texture
{
	pigment
	{
		gradient y 
		cubic_wave 
		turbulence UE_T
		omega UE_T
		pigment_map
		{
			[UE_B_K BOZO1]
			[1 BOZO2]
		}
		scale UNIT_1
	}
	finish
	{ 
		phong 0.4*FOLIAGE_GLOSS
		phong_size 20*FOLIAGE_GLOSS
	}
}
#if (AZ>=1)
#if (ZWZ>=1)
#if (SZ>=1)
#if (TWIG_RAMIFICATION_ORDER>0)
#if (TWIG_RAMIFICATION_STRENGTH>0)
#declare TWIGGI=
blob
{
	threshold TRUNK_RADIUS*3
	#declare A=1;
	#while (A<BH)
	sphere
	{
		<0,0,0>,TRUNK_RADIUS*5,TRUNK_RADIUS*10
		scale TRUNK_RADIUS/90		
		scale (BH-A)/BH				
		scale AST_ST
		scale BH/(S_MA_R+S_MI_R+M_BL)
		scale ZW_VST									
		scale TWIG_RAMIFICATION_THICKNESS_CHANGE 		
		scale (BH-AA*ST_VJ)/BH					
		scale (ZW_L-S_A)/ZW_L
		scale (AST_L-Z_A)/AST_L
		#if (S_VJ_A<0) scale 1/(1-S_VJ_A*(Z_A+Z_B)/AST_L) #end 
		#if (S_VJ_B<0) scale 1/(1-S_VJ_B*(AA+AB)/BH) #end
	 	#if (S_VJ_Z<1) scale 1/(1-(S_VJ_Z-1)*(S_A+B_A_T)/ZW_L) #end 
	 	scale 1/BUNCH_DEFORM
		scale 1/(1+arcTaT)
		scale 1/(1+arcTaB)
		translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*ZBFZA*RFZA*A/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*ZBFZA*RFZA*A/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*ZBFZA*RFZA*A/BH))>*((BH-A)/BH)*ZW_K_A*TRCA
		translate <AW*sin(pi/2*sin(5.456*AWFZ*ZBFZA*RFZA*A/BH)),AW*sin(pi/2*sin(5.123*AWFZ*ZBFZA*RFZA*A/BH)),AW*sin(pi/2*sin(4.789*AWFZ*ZBFZA*RFZA*A/BH))>*((BH-A)/BH)*ZW_K_A*TRCA
	  translate <A_BG*sin(pi/2*sin(5.371*ABFZ*ZBFZA*RFZA*A/BH)),A,A_BG*sin(pi/2*sin(5.937*ABFZ*ZBFZA*RFZA*A/BH))>*<ZW_K_A*TRCA,1,ZW_K_A*TRCA>
	  rotate <0,BRANCH_SPIRAL*sin(A/BH*(pi/2)),0>  	          
	}			
  #declare A=A+BH/TWIG_RAMIFICATION_STRENGTH/200;
	#end
}
#end
#end
#end
#end
#end
#if (AZ>=1)
#if (ZWZ>=1)
#if (SZ>=1)
#if (TWIG_RAMIFICATION_ORDER=1)
#if (TWIG_RAMIFICATION_STRENGTH>0)
#declare REISIG=
object
{
	TWIGGI 
	bounded_by{cylinder{<0,0,0>,<0,BH,0>,BH}}
}
#end 
#end
#end
#end
#end
#if (AZ>=1)
#if (ZWZ>=1)
#if (SZ>=1)
#if (TWIG_RAMIFICATION_ORDER=2)
#if (TWIG_RAMIFICATION_STRENGTH>0)
#if (LAST_RAMIFICATION_NUMBER>0)
#declare REISIG=
union
{
	object{TWIGGI}
	#declare A=BH/(L_R_NR+1);
	#while (A<BH)
	object
	{
		TWIGGI
		scale 1-A/BH		
		rotate <0,1234.567*sin(A*4.56*pi),0>
		#if (S_R_T>0)
			rotate <0,0,-(S_R_T/2)*(1-(A*A)/(BH*BH))>*(((L_R_NR+1)*(L_R_NR+1))/(((L_R_NR+1)*(L_R_NR+1))-1))
			rotate <0,777*sin(2.37*A*pi),0>
		#else
			rotate <0,0,V_S_T/2*(1-(A*A)/(BH*BH))*(sin(A*pi/2) #if (sin(A*pi/2)<0) -(1+sin(A*pi/2)) #else +(1-sin(A*pi/2)) #end)>*(((L_R_NR+1)*(L_R_NR+1))/(((L_R_NR+1)*(L_R_NR+1))-1))
			rotate <H_S_T/2*(1-(A*A)/(BH*BH))*(sin(A*pi) #if (sin(A*pi)<0) -(1+sin(A*pi)) #else +(1-sin(A*pi)) #end),0,0>*(((L_R_NR+1)*(L_R_NR+1))/(((L_R_NR+1)*(L_R_NR+1))-1))			
		#end		
		translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*ZBFZA*RFZA*A/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*ZBFZA*RFZA*A/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*ZBFZA*RFZA*A/BH))>*((BH-A)/BH)*ZW_K_A*TRCA
		translate <AW*sin(pi/2*sin(5.456*AWFZ*ZBFZA*RFZA*A/BH)),AW*sin(pi/2*sin(5.123*AWFZ*ZBFZA*RFZA*A/BH)),AW*sin(pi/2*sin(4.789*AWFZ*ZBFZA*RFZA*A/BH))>*((BH-A)/BH)*ZW_K_A*TRCA
	  translate <A_BG*sin(pi/2*sin(5.371*ABFZ*ZBFZA*RFZA*A/BH)),A,A_BG*sin(pi/2*sin(5.937*ABFZ*ZBFZA*RFZA*A/BH))>*<ZW_K_A*TRCA,1,ZW_K_A*TRCA>
	  rotate <0,BRANCH_SPIRAL*sin(A/BH*(pi/2)),0>   
	}    
	#declare A=A+BH/(L_R_NR+1);
	#end	
	bounded_by{cylinder{<0,0,0>,<0,BH,0>,BH}}
}
#end
#end
#end
#end
#end
#end  
#if (AZ>=1)
#if (ZWZ>=1)
#if (SZ>=1)
#if (TWIG_RAMIFICATION_ORDER=3)
#if (TWIG_RAMIFICATION_STRENGTH>0)
#if (LAST_RAMIFICATION_NUMBER>0)
#declare REISIG=
union
{ 
	object{TWIGGI}		
	#declare B=BH/(L_R_NR+1);
	#while (B<BH)	
  union
  {
		object{TWIGGI}		
		#declare A=BH/(L_R_NR+1);
		#while (A<BH)
		object
		{
			TWIGGI
			scale 1-A/BH			
			rotate <0,1234.567*sin(A*4.56*pi),0>			
			#if (S_R_T>0)
				rotate <0,0,-S_R_T/2*(1-(A)/(BH))>*((L_R_NR+1)/L_R_NR)
				rotate <0,777*sin(2.37*A*pi),0>
			#else
				rotate <0,0,V_S_T/2*(1-(A)/(BH))*(sin(A*pi/2) #if (sin(A*pi/2)<0) -(1+sin(A*pi/2)) #else +(1-sin(A*pi/2)) #end)>*((L_R_NR+1)/L_R_NR)
				rotate <H_S_T/2*(1-(A)/(BH))*(sin(A*pi) #if (sin(A*pi)<0) -(1+sin(A*pi)) #else +(1-sin(A*pi)) #end),0,0>*((L_R_NR+1)/L_R_NR)
			#end
			rotate <0,-1234.567*sin(B*4.56*pi)+180,0>			
			translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*ZBFZA*RFZA*A/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*ZBFZA*RFZA*A/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*ZBFZA*RFZA*A/BH))>*((BH-A)/BH)*ZW_K_A*TRCA
			translate <AW*sin(pi/2*sin(5.456*AWFZ*ZBFZA*RFZA*A/BH)),AW*sin(pi/2*sin(5.123*AWFZ*ZBFZA*RFZA*A/BH)),AW*sin(pi/2*sin(4.789*AWFZ*ZBFZA*RFZA*A/BH))>*((BH-A)/BH)*ZW_K_A*TRCA
		  translate <A_BG*sin(pi/2*sin(5.371*ABFZ*ZBFZA*RFZA*A/BH)),A,A_BG*sin(pi/2*sin(5.937*ABFZ*ZBFZA*RFZA*A/BH))>*<ZW_K_A*TRCA,1,ZW_K_A*TRCA>
		  rotate <0,BRANCH_SPIRAL*sin(A/BH*(pi/2)),0>
		}    
		#declare A=A+BH/(L_R_NR+1);
		#end
		scale 1-B/BH			
		rotate <0,1234.567*sin(B*4.56*pi),0>		
		#if (S_R_T>0)
			rotate <0,0,-S_R_T/2*(1-(B*B)/(BH*BH))>*(((L_R_NR+1)*(L_R_NR+1))/(((L_R_NR+1)*(L_R_NR+1))-1))
			rotate <0,777*sin(2.37*B*pi),0>
		#else
			rotate <0,0,V_S_T/2*(1-(B*B)/(BH*BH))*(sin(B*pi/2) #if (sin(B*pi/2)<0) -(1+sin(B*pi/2)) #else +(1-sin(B*pi/2)) #end)>*(((L_R_NR+1)*(L_R_NR+1))/(((L_R_NR+1)*(L_R_NR+1))-1))
			rotate <H_S_T/2*(1-(B*B)/(BH*BH))*(sin(B*pi) #if (sin(B*pi)<0) -(1+sin(B*pi)) #else +(1-sin(B*pi)) #end),0,0>*(((L_R_NR+1)*(L_R_NR+1))/(((L_R_NR+1)*(L_R_NR+1))-1))		
		#end		
		translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*ZBFZA*RFZA*B/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*ZBFZA*RFZA*B/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*ZBFZA*RFZA*B/BH))>*((BH-B)/BH)*ZW_K_A*TRCA
		translate <AW*sin(pi/2*sin(5.456*AWFZ*ZBFZA*RFZA*B/BH)),AW*sin(pi/2*sin(5.123*AWFZ*ZBFZA*RFZA*B/BH)),AW*sin(pi/2*sin(4.789*AWFZ*ZBFZA*RFZA*B/BH))>*((BH-B)/BH)*ZW_K_A*TRCA
	  translate <A_BG*sin(pi/2*sin(5.371*ABFZ*ZBFZA*RFZA*B/BH)),B,A_BG*sin(pi/2*sin(5.937*ABFZ*ZBFZA*RFZA*B/BH))>*<ZW_K_A*TRCA,1,ZW_K_A*TRCA>
	  rotate <0,BRANCH_SPIRAL*sin(B/BH*(pi/2)),0>
  }
  #declare B=B+BH/(L_R_NR+1);
	#end      	     		
	bounded_by{sphere{<0,0,0>,BH}}
}
#end
#end
#end
#end
#end
#end
#declare TREE=		
union
{  	
	#if (M_BL*M_BH*M_BB>0)
	#if (AZ>=1)
	#if (ZWZ>=1)
	#if (BZ>=1)
	#if (SZ>=1)
	#if (BZ/SZ>1)
	#declare STAUDE =
	union
	{ 
		#declare A=1;
		#while (A<100)
		object
		{
			LEAF_TYPE
			scale <M_BL*(sin(A*5.73*pi)+4)/4,M_BH*(sin(A*3.16*pi)+4)/4,M_BB*(sin(A*7.99*pi)+4)/4>
			rotate <M_BN_X*sin(7.11*A*pi),0,-M_BN_Z+M_BN_Z_SCOPE*sin(7.31*A*pi)>
			rotate <0,M_BN_Y*sin(7.17*A*pi),0>
			translate <S_MA_R+(S_MI_R*(1+sin(sin(pi*5.92*A)*pi*32.1))/2),0,0>
			#if (S_R_T>0)
				rotate <0,0,-S_R_T/4*(1+sin(sin(pi*2.37*A)*pi*12.3+pi/5))>
				rotate <777*sin(2.37*A*pi),0,0>
			#else
				rotate <0,0,V_S_T/2*sin(sin(pi*7.73*A)*pi*12.3+pi/3)>
				rotate <0,H_S_T/2*sin(sin(pi*2.37*A)*pi*12.3+pi/5),0>
			#end
		}
		#declare A=A+100/BZ*SZ;
		#end
	}
	#else
	#declare STAUDE = 
   object
	{
		LEAF_TYPE
		scale <M_BL,M_BH,M_BB>
		rotate <M_BN_X,0,-M_BN_Z>
		rotate <0,M_BN_Y,0>
		translate <S_MA_R+(S_MI_R),0,0>
	}
	#end
	union
	{
	#declare A=1;
	#while (A<BH) 
		#declare B=1;
		#while (B<100)
			#declare C=1;
			#while (C<ZL)
			object
			{
				STAUDE
				scale BUNCH_DEFORM
				scale (1+arcTaT*sin(pi*(C*((SZ/ZWZ+1)/(SZ/ZWZ))/ZL)))
				scale (1+arcTaB*sin(pi*(A*((ZWZ/AZ+1)/(ZWZ/AZ))/BH)))
				rotate <-1456*cos(AST_SPIEL*7.19*A*pi)*(1-ZW_W_S),0,0>
				rotate <-1234*sin(7.77*C*pi),0,0>*S_DREH_VERSCHIEBUNG
				rotate <-1234*sin(7.77*A*pi),0,0>*S_DREH_VERSCHIEBUNG
				rotate <-BRANCH_SPIRAL*sin(((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH*(pi/2)),0,0>
				rotate <-1234*sin(ZW_SPIEL*4.77*B*pi),0,0>*(1-ZW_W_S)
				rotate <-BRANCH_SPIRAL*sin((Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH*(pi/2)),0,0>
				rotate <0,(H_S_N_SP/2)*sin(7.77*C*pi),0>
				rotate <0,0,V_S_N_SP/2*sin(9.77*C*pi)>+<0,0,(V_S_N #if (SZ/ZWZ>1) -(V_S_N*(C*C)/(ZL*ZL)*V_S_NEIG) #end )>
				rotate <0, #if (SZ/ZWZ=1) (H_S_N-(H_S_N*(A*A)/(BH*BH)*S_NEIG))*((1-S_W_S)+(S_W_S*sin((A/(BH/ZWZ*AZ)-1/(BH/ZWZ*AZ))*pi-pi/2))) #else -(H_S_N*(C*C)/(ZL*ZL)*S_NEIG)*((1-S_W_S)+S_W_S*sin((C/(ZL/(SZ/ZWZ))-1/(ZL/(SZ/ZWZ)))*pi-pi/2)) #end ,0>
				rotate <1234*sin(7.77*C*pi),0,0>*S_DREH
				rotate <1234*sin(7.77*A*pi),0,0>*S_DREH
				scale (1-S_VJ_B*B/100)
				scale (1-S_VJ_Z*(C)/(ZL))
				scale (1-S_VJ_A*(A)/(BH))
				rotate <0,0,90>
				rotate <0,-1234*sin(4.77*A*pi),0>*ZW_DREH
				translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*((BH-((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)))/BH)*(ZW_L/BH)*ZW_K_A*(1-B/100*A_STR)*(1-A/BH*T_STR)
				translate <AW*sin(pi/2*sin(5.456*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),AW*sin(pi/2*sin(5.123*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),AW*sin(pi/2*sin(4.789*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*((BH-((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)))/BH)*(ZW_L/BH)*ZW_K_A*(1-B/100*A_STR)*(1-A/BH*T_STR)
				translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)+(1-ANS)*5.371*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH),((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)),A_BG*sin(pi/2*sin(5.937*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*(ZW_L/BH)*<1,1,H_ABG_S>*<ZW_K_A,1,ZW_K_A>*<1-B/100*A_STR,1,1-B/100*A_STR>*<(1-A/BH*T_STR),1,(1-A/BH*T_STR)>
				rotate <0,BRANCH_SPIRAL*sin(((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH*(pi/2)),0>
				rotate <0,1234*sin(4.77*A*pi),0>*ZW_DREH
				scale ((Z_A+Z_B+ZW_L*(1-AST_VJ))-Z_A*AST_VJ*AST_L/BH)/(Z_A+Z_B+ZW_L*(1-AST_VJ))				
				scale ((Z_B+ZW_L*(1-AST_VJ))-A*AST_VJ*Z_B/BH)/(Z_B+ZW_L*(1-AST_VJ))	
				scale BH/AST_L
				rotate <0,0,-90>
				rotate <0,0,V_ZW_N_SP*sin(9.77*A*A*pi)>+<0,0,(V_ZW_N-(V_ZW_N*(A*A)/(BH*BH)*V_ZW_NEIG))>
				rotate <0,H_ZW_N_SP*sin(7.77*A*A*pi),0>+<0,(H_ZW_N-(H_ZW_N*(A*A)/(BH*BH)*ZW_NEIG)),0>*((1-ZW_W_S)+(ZW_W_S*sin((A/(BH/ZWZ*AZ)-1/(BH/ZWZ*AZ))*pi-pi/2)))			
				rotate <1456*cos(AST_SPIEL*7.19*A*pi)*(1-ZW_W_S),0,0>
				rotate <0,0,90>
				rotate <0,1234*sin(ZW_SPIEL*4.77*B*pi),0>*(1-ZW_W_S)
	      rotate <0,-1234*sin(4.77*B*pi),0>*AST_DREH
	      translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*((BH-(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)))/BH)*(1-B/100*A_STR)
	  		translate <AW*sin(pi/2*sin(5.456*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),AW*sin(pi/2*sin(5.123*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),AW*sin(pi/2*sin(4.789*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*((BH-(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)))/BH)*<1,A_L_W,H_AW_S>*(1-B/100*A_STR)
	      translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)+(1-ANS)*(5.371*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)),A_BG*sin(ANS*pi/2*sin(5.937*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)+(1-ANS)*(5.937*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*<-1,1,H_ABG_S>*<1-B/100*A_STR,1,1-B/100*A_STR>
	      rotate <0,BRANCH_SPIRAL*sin((Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH*(pi/2)),0>
				rotate <0,1234*sin(4.77*B*pi),0>*AST_DREH
				rotate <0,0,-90>
				scale (AST_L/BH)*(1-B/100*AST_VK)
				rotate <0,0,ASTNEIGUNG+((B/100)*((90-ASTNEIGUNG)))*A_NEIG>+<0,0,A_N_SP*sin(9.77*B*B*pi)>
				rotate <0,1456*cos(AST_SPIEL*7.19*B*pi)*(1-A_W_S)+90*A_W_S*sin((B/(100/AZ)-1/(100/AZ))*pi-pi/2)+90,0>
				translate <KK*sin(pi/2*sin(5.123*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(4.789*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(5.456*SKKFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)
				translate <SW*sin(pi/2*sin(5.456*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(5.123*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(4.789*SWFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)			
				translate <BG_X*sin(pi/2*sin(5.371*FZ_X*1.5*(AA+(B*AB/100))/BH)),(AA+(B*AB/100)),BG_Z*sin(pi/2*sin(5.937*FZ_Z*1.5*(AA+(B*AB/100))/BH))>
				rotate <0,TRUNK_SPIRAL*sin((AA+(B*AB/100))/(BH*3/5)),0>	
			}
			#declare C=C+ZL/SZ*ZWZ;
			#end
		#declare B=B+100/AZ;
		#end
	#declare A=A+BH/ZWZ*AZ;
	#end
	texture{LAUB}
	}
	#end
	#end
	#end
	#end
	#end
	#if (M_BL_L*M_BL_B>0) 
	#if (AZ>=1)
	#if (ZWZ>=1)
	#if (BL_Z>=1)
	#if (SZ>=1)
	#if (BL_Z/SZ>1)
	#declare B_STAUDE =
	union
	{ 
		#declare A=1;
		#while (A<100*BL_Z)
		object
		{
			BLOSSOM
			scale <M_BL_L*(sin(A*5.73*pi)+4)/4,M_BL_B*(sin(A*3.16*pi)+4)/4,M_BL_B*(sin(A*7.99*pi)+4)/4>
			rotate <M_BN_X*sin(7.11*A*pi),0,BL_N_Z*(1+sin(9.33*A*pi))/2>
			rotate <0,M_BN_Y*sin(7.17*A*pi),0>
			translate <S_MA_R+(S_MI_R),0,0>
			#if (S_R_T>0)
				rotate <0,0,-S_R_T/4*(1+sin(sin(2.37*A*pi)*pi*12.3+pi/5))>
				rotate <777*sin(2.37*A*pi),0,0>
			#else
				rotate <0,0,V_S_T/2*sin(sin(7.73*A*pi)*pi*12.3+pi/3)>
				rotate <0,H_S_T/2*sin(sin(2.37*A*pi)*pi*12.3+pi/5),0>
			#end
			pigment{color rgb BL_F}
		}
		#declare A=A+100*SZ;
		#end
	}
	#else
	#declare B_STAUDE =
	object
	{
		BLOSSOM
		scale <M_BL_L,M_BL_B,M_BL_B>
		rotate <M_BN_X,0,BL_N_Z>
		rotate <0,M_BN_Y,0>
		translate <S_MA_R+(S_MI_R),0,0>
		pigment{color rgb BL_F}
	}
	#end
	union
	{
	#declare A=1;
	#while (A<BH) 
		#declare B=1;
		#while (B<100)
			#declare C=1;
			#while (C<ZL)
			object
			{
				B_STAUDE
				scale BUNCH_DEFORM
				scale (1+arcTaT*sin(pi*(C*((SZ/ZWZ+1)/(SZ/ZWZ))/ZL)))
				scale (1+arcTaB*sin(pi*(A*((ZWZ/AZ+1)/(ZWZ/AZ))/BH)))
				rotate <-1456*cos(AST_SPIEL*7.19*A*pi)*(1-ZW_W_S),0,0>
				rotate <-1234*sin(7.77*C*pi),0,0>*S_DREH_VERSCHIEBUNG
				rotate <-1234*sin(7.77*A*pi),0,0>*S_DREH_VERSCHIEBUNG
				rotate <-BRANCH_SPIRAL*sin(((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH*(pi/2)),0,0>
				rotate <-1234*sin(ZW_SPIEL*4.77*B*pi),0,0>*(1-ZW_W_S)
				rotate <-BRANCH_SPIRAL*sin((Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH*(pi/2)),0,0>
				rotate <0,H_S_N_SP/2*sin(7.77*C*pi),0>
				rotate <0,0,V_S_N_SP/2*sin(9.77*C*pi)>+<0,0,(V_S_N #if (SZ/ZWZ>1) -(V_S_N*(C*C)/(ZL*ZL)*V_S_NEIG) #end )>
				rotate <0, #if (SZ/ZWZ=1) (H_S_N-(H_S_N*(A*A)/(BH*BH)*S_NEIG))*((1-S_W_S)+(S_W_S*sin((A/(BH/ZWZ*AZ)-1/(BH/ZWZ*AZ))*pi-pi/2))) #else -(H_S_N*(C*C)/(ZL*ZL)*S_NEIG)*((1-S_W_S)+S_W_S*sin((C/(ZL/(SZ/ZWZ))-1/(ZL/(SZ/ZWZ)))*pi-pi/2)) #end ,0>
				rotate <1234*sin(7.77*C*pi),0,0>*S_DREH
				rotate <1234*sin(7.77*A*pi),0,0>*S_DREH
				scale (1-S_VJ_B*B/100)
				scale (1-S_VJ_Z*(C)/(ZL))
				scale (1-S_VJ_A*(A)/(BH))
				rotate <0,0,90>
				rotate <0,-1234*sin(4.77*A*pi),0>*ZW_DREH
				translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*((BH-((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)))/BH)*(ZW_L/BH)*ZW_K_A*(1-B/100*A_STR)*(1-A/BH*T_STR)
				translate <AW*sin(pi/2*sin(5.456*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),AW*sin(pi/2*sin(5.123*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),AW*sin(pi/2*sin(4.789*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*((BH-((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)))/BH)*(ZW_L/BH)*ZW_K_A*(1-B/100*A_STR)*(1-A/BH*T_STR)
				translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)+(1-ANS)*5.371*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH),((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)),A_BG*sin(pi/2*sin(5.937*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*(ZW_L/BH)*<1,1,H_ABG_S>*<ZW_K_A,1,ZW_K_A>*<1-B/100*A_STR,1,1-B/100*A_STR>*<(1-A/BH*T_STR),1,(1-A/BH*T_STR)>
				rotate <0,BRANCH_SPIRAL*sin(((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH*(pi/2)),0>
				rotate <0,1234*sin(4.77*A*pi),0>*ZW_DREH
				scale ((Z_A+Z_B+ZW_L*(1-AST_VJ))-Z_A*AST_VJ*AST_L/BH)/(Z_A+Z_B+ZW_L*(1-AST_VJ))				
				scale ((Z_B+ZW_L*(1-AST_VJ))-A*AST_VJ*Z_B/BH)/(Z_B+ZW_L*(1-AST_VJ))	
        scale BH/AST_L
				rotate <0,0,-90>
				rotate <0,0,V_ZW_N_SP*sin(9.77*A*A*pi)>+<0,0,(V_ZW_N-(V_ZW_N*(A*A)/(BH*BH)*V_ZW_NEIG))>
				rotate <0,H_ZW_N_SP*sin(7.77*A*A*pi),0>+<0,(H_ZW_N-(H_ZW_N*(A*A)/(BH*BH)*ZW_NEIG)),0>*((1-ZW_W_S)+(ZW_W_S*sin((A/(BH/ZWZ*AZ)-1/(BH/ZWZ*AZ))*pi-pi/2)))
				rotate <1456*cos(AST_SPIEL*7.19*A*pi)*(1-ZW_W_S),0,0>			
				rotate <0,0,90>
				rotate <0,1234*sin(ZW_SPIEL*4.77*B*pi),0>*(1-ZW_W_S)
	      rotate <0,-1234*sin(4.77*B*pi),0>*AST_DREH
	      translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*((BH-(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)))/BH)*(1-B/100*A_STR)
	  		translate <AW*sin(pi/2*sin(5.456*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),AW*sin(pi/2*sin(5.123*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),AW*sin(pi/2*sin(4.789*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*((BH-(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)))/BH)*<1,A_L_W,H_AW_S>*(1-B/100*A_STR)
	      translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)+(1-ANS)*(5.371*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)),A_BG*sin(ANS*pi/2*sin(5.937*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)+(1-ANS)*(5.937*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*<-1,1,H_ABG_S>*<1-B/100*A_STR,1,1-B/100*A_STR>
	      rotate <0,BRANCH_SPIRAL*sin((Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH*(pi/2)),0>
				rotate <0,1234*sin(4.77*B*pi),0>*AST_DREH
				rotate <0,0,-90>
				scale (AST_L/BH)*(1-B/100*AST_VK)
				rotate <0,0,ASTNEIGUNG+((B/100)*((90-ASTNEIGUNG)))*A_NEIG>+<0,0,A_N_SP*sin(9.77*B*B*pi)>
				rotate <0,1456*cos(AST_SPIEL*7.19*B*pi)*(1-A_W_S)+90*A_W_S*sin((B/(100/AZ)-1/(100/AZ))*pi-pi/2)+90,0>
				translate <KK*sin(pi/2*sin(5.123*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(4.789*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(5.456*SKKFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)
				translate <SW*sin(pi/2*sin(5.456*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(5.123*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(4.789*SWFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)			
				translate <BG_X*sin(pi/2*sin(5.371*FZ_X*1.5*(AA+(B*AB/100))/BH)),(AA+(B*AB/100)),BG_Z*sin(pi/2*sin(5.937*FZ_Z*1.5*(AA+(B*AB/100))/BH))>
				rotate <0,TRUNK_SPIRAL*sin((AA+(B*AB/100))/(BH*3/5)),0>	
			}
			#declare C=C+ZL/SZ*ZWZ;
			#end
		#declare B=B+100/AZ;
		#end
	#declare A=A+BH/ZWZ*AZ;
	#end
	}
	#end
	#end
	#end
	#end
	#end
	#if (AZ>=1)
	#if (ZWZ>=1)
	#if (SZ>=1)
	#if (TWIG_RAMIFICATION_ORDER>0)
	union
	{
	#declare A=1;
	#while (A<BH) 
		#declare B=1;
		#while (B<100)
			#declare C=1;
			#while (C<ZL)			
			object
			{
				REISIG
				scale (S_MA_R+S_MI_R+M_BL)/BH
				rotate <0,0,-90>
				scale BUNCH_DEFORM
				scale (1+arcTaT*sin(pi*(C*((SZ/ZWZ+1)/(SZ/ZWZ))/ZL)))
				scale (1+arcTaB*sin(pi*(A*((ZWZ/AZ+1)/(ZWZ/AZ))/BH)))
				rotate <-1456*cos(AST_SPIEL*7.19*A*pi)*(1-ZW_W_S),0,0>
				rotate <-1234*sin(7.77*C*pi),0,0>*S_DREH_VERSCHIEBUNG
				rotate <-1234*sin(7.77*A*pi),0,0>*S_DREH_VERSCHIEBUNG
				rotate <-BRANCH_SPIRAL*sin(((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH*(pi/2)),0,0>
				rotate <-1234*sin(ZW_SPIEL*4.77*B*pi),0,0>*(1-ZW_W_S)
				rotate <-BRANCH_SPIRAL*sin((Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH*(pi/2)),0,0>
				rotate <0,H_S_N_SP/2*sin(7.77*C*pi),0>
				rotate <0,0,V_S_N_SP/2*sin(9.77*C*pi)>+<0,0,(V_S_N #if (SZ/ZWZ>1) -(V_S_N*(C*C)/(ZL*ZL)*V_S_NEIG) #end )>
				rotate <0, #if (SZ/ZWZ=1) (H_S_N-(H_S_N*(A*A)/(BH*BH)*S_NEIG))*((1-S_W_S)+(S_W_S*sin((A/(BH/ZWZ*AZ)-1/(BH/ZWZ*AZ))*pi-pi/2))) #else -(H_S_N*(C*C)/(ZL*ZL)*S_NEIG)*((1-S_W_S)+S_W_S*sin((C/(ZL/(SZ/ZWZ))-1/(ZL/(SZ/ZWZ)))*pi-pi/2)) #end ,0>
				rotate <1234*sin(7.77*C*pi),0,0>*S_DREH
				rotate <1234*sin(7.77*A*pi),0,0>*S_DREH
				scale (1-S_VJ_B*B/100)
				scale (1-S_VJ_Z*(C)/(ZL))
				scale (1-S_VJ_A*(A)/(BH))
				rotate <0,0,90>
				rotate <0,-1234*sin(4.77*A*pi),0>*ZW_DREH
				translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*((BH-((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)))/BH)*(ZW_L/BH)*ZW_K_A*(1-B/100*A_STR)*(1-A/BH*T_STR)
				translate <AW*sin(pi/2*sin(5.456*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),AW*sin(pi/2*sin(5.123*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)),AW*sin(pi/2*sin(4.789*AWFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*((BH-((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)))/BH)*(ZW_L/BH)*ZW_K_A*(1-B/100*A_STR)*(1-A/BH*T_STR)
				translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH)+(1-ANS)*5.371*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH),((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L)),A_BG*sin(pi/2*sin(5.937*ABFZ*ZBFZA*((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH))>*<1,1,H_ABG_S>*(ZW_L/BH)*<ZW_K_A,1,ZW_K_A>*<1-B/100*A_STR,1,1-B/100*A_STR>*<(1-A/BH*T_STR),1,(1-A/BH*T_STR)>
				rotate <0,BRANCH_SPIRAL*sin(((S_A+((ZL-S_A)/ZL*C))*(BH/ZW_L))/BH*(pi/2)),0>
				rotate <0,1234*sin(4.77*A*pi),0>*ZW_DREH
				scale ((Z_A+Z_B+ZW_L*(1-AST_VJ))-Z_A*AST_VJ*AST_L/BH)/(Z_A+Z_B+ZW_L*(1-AST_VJ))				
				scale ((Z_B+ZW_L*(1-AST_VJ))-A*AST_VJ*Z_B/BH)/(Z_B+ZW_L*(1-AST_VJ))	
				scale BH/AST_L
				rotate <0,0,-90>
				rotate <0,0,V_ZW_N_SP*sin(9.77*A*A*pi)>+<0,0,(V_ZW_N-(V_ZW_N*(A*A)/(BH*BH)*V_ZW_NEIG))>
				rotate <0,H_ZW_N_SP*sin(7.77*A*A*pi),0>+<0,(H_ZW_N-(H_ZW_N*(A*A)/(BH*BH)*ZW_NEIG)),0>*((1-ZW_W_S)+(ZW_W_S*sin((A/(BH/ZWZ*AZ)-1/(BH/ZWZ*AZ))*pi-pi/2)))			
				rotate <1456*cos(AST_SPIEL*7.19*A*pi)*(1-ZW_W_S),0,0>
				rotate <0,0,90>
				rotate <0,1234*sin(ZW_SPIEL*4.77*B*pi),0>*(1-ZW_W_S)
	      rotate <0,-1234*sin(4.77*B*pi),0>*AST_DREH
	      translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*((BH-(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)))/BH)*(1-B/100*A_STR)
	  		translate <AW*sin(pi/2*sin(5.456*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),AW*sin(pi/2*sin(5.123*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),AW*sin(pi/2*sin(4.789*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*((BH-(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)))/BH)*<1,A_L_W,H_AW_S>*(1-B/100*A_STR)
	      translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)+(1-ANS)*(5.371*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)),A_BG*sin(ANS*pi/2*sin(5.937*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)+(1-ANS)*(5.937*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*<-1,1,H_ABG_S>*<1-B/100*A_STR,1,1-B/100*A_STR>
	      rotate <0,BRANCH_SPIRAL*sin((Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH*(pi/2)),0>
				rotate <0,1234*sin(4.77*B*pi),0>*AST_DREH
				rotate <0,0,-90>
				scale (AST_L/BH)*(1-B/100*AST_VK)
				rotate <0,0,ASTNEIGUNG+((B/100)*((90-ASTNEIGUNG)))*A_NEIG>+<0,0,A_N_SP*sin(9.77*B*B*pi)>
				rotate <0,1456*cos(AST_SPIEL*7.19*B*pi)*(1-A_W_S)+90*A_W_S*sin((B/(100/AZ)-1/(100/AZ))*pi-pi/2)+90,0>
				translate <KK*sin(pi/2*sin(5.123*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(4.789*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(5.456*SKKFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)
				translate <SW*sin(pi/2*sin(5.456*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(5.123*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(4.789*SWFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)			
				translate <BG_X*sin(pi/2*sin(5.371*FZ_X*1.5*(AA+(B*AB/100))/BH)),(AA+(B*AB/100)),BG_Z*sin(pi/2*sin(5.937*FZ_Z*1.5*(AA+(B*AB/100))/BH))>
				rotate <0,TRUNK_SPIRAL*sin((AA+(B*AB/100))/(BH*3/5)),0>	
			}
			#declare C=C+ZL/SZ*ZWZ;
			#end
		#declare B=B+100/AZ;
		#end
	#declare A=A+BH/ZWZ*AZ;
	#end	  
	}
	#end 
	#end
	#end 
	#end
  blob
	{
		threshold TRUNK_RADIUS*3
		#if (TRUNK_STRENGTH>0)
		#declare A=1;
		#while (A<=(AA+ #if (AZ>=1) ((AZ-1)/AZ)* #end AB))
		sphere
		{
			<0,0,0>,TRUNK_RADIUS*5,TRUNK_RADIUS*10
			scale TRUNK_RADIUS/90
			scale (BH-A*ST_VJ)/BH
			#if (PALM_TRUNK=0)
				scale <1,1,1>+<-BARK_PROFILE*.9,BARK_PROFILE*.25,BARK_PROFILE*.25>
				rotate <0,0,20.3*sin(3.12*A*pi)>
				rotate <0,12347.3*sin(3.77*A*pi),0>
			#else 
				scale <1,1,1>+<0,-PALM_TRUNK*.9,PALM_TRUNK*.5> 
				rotate <35,12347.3*sin(4.77*A*pi),0>
			#end
			#if (A>AA)
				scale (1-(A-AA)/AB*AST_VK)
				scale 1-((A-AA)/( #if (AZ>1) ((AZ-1)/AZ)* #end AB))*(1-AST_ST)
			#else
        scale 1
			#end
			translate <KK*sin(pi/2*sin(5.123*SKKFZ*A/BH)),KK*sin(pi/2*sin(4.789*SKKFZ*A/BH)),KK*sin(pi/2*sin(5.456*SKKFZ*A/BH))>*((BH-A)/BH)
			translate <SW*sin(pi/2*sin(5.456*SWFZ*A/BH)),SW*sin(pi/2*sin(5.123*SWFZ*A/BH)),SW*sin(pi/2*sin(4.789*SWFZ*A/BH))>*((BH-A)/BH)
			translate <BG_X*sin(pi/2*sin(5.371*FZ_X*1.5*A/BH)),A,BG_Z*sin(pi/2*sin(5.937*FZ_Z*1.5*A/BH))>
			rotate <0,TRUNK_SPIRAL*sin(A/(BH*3/5)),0>
		}			
    #declare A=A+BH/TRUNK_STRENGTH/1000;
		#end
		#else
		sphere{<0,0,0>,TRUNK_RADIUS*.01,TRUNK_RADIUS*10}
		#end
		#if (BRANCH_STRENGTH>0)
		#if (AZ>=1)
		#declare A=1;		
		#while (A<=(Z_A+ #if (ZWZ/AZ>1) ((ZWZ/AZ-1)/(ZWZ/AZ))* #end Z_B+ #if (ZWZ/AZ>1) (Z_B/(ZWZ/AZ))*AST_E+ #end AST_E*ZW_L*(1-AST_VJ))/AST_L*BH)      
      #declare B=1;
			#while (B<100)
			sphere
			{
				<0,0,0>,TRUNK_RADIUS*5,TRUNK_RADIUS*10 
				scale TRUNK_RADIUS/90
				scale ((Z_A+Z_B+ZW_L*(1-AST_VJ))-A*AST_VJ*AST_L/BH)/(Z_A+Z_B+ZW_L*(1-AST_VJ))				
				scale <1,1,1>+(1-BPTo*A/(Z_A+Z_B+ZL))*<-BARK_PROFILE*.9,BARK_PROFILE*.25,BARK_PROFILE*.25>*BRANCH_BARK_PROFILE_CHANGE
				scale (BH-AA*ST_VJ)/BH					
				scale (BH-((AB)*(B/100))*ST_VJ)/BH
				scale 1/(AST_L/BH)
				scale AST_ST
				#if (A>Z_A/AST_L*BH & Z_B>=1 & AST_E=0)
					scale (1-BEE*((A-Z_A/AST_L*BH)/( #if (ZWZ/AZ>1) ((ZWZ/AZ-1)/(ZWZ/AZ))* #end (Z_B/AST_L*BH)))*(1-ZW_VST))
				#else
        	scale 1
				#end
				rotate <0,12347.3*sin(3.77*A*A*pi),0>
				translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*A/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*A/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*A/BH))>*((BH-A)/BH)*(1-B/100*A_STR)        
				translate <AW*sin(pi/2*sin(5.456*AWFZ*A/BH)),AW*sin(pi/2*sin(5.123*AWFZ*A/BH)),AW*sin(pi/2*sin(4.789*AWFZ*A/BH))>*((BH-A)/BH)*<1,A_L_W,H_AW_S>*(1-B/100*A_STR)   
				translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*A/BH)+(1-ANS)*5.371*ABFZ*A/BH),A,A_BG*sin(ANS*pi/2*sin(5.937*ABFZ*A/BH)+(1-ANS)*5.937*ABFZ*A/BH)>*<-1,1,H_ABG_S>*<1-B/100*A_STR,1,1-B/100*A_STR>
				rotate <0,BRANCH_SPIRAL*sin(A/BH*(pi/2)),0>
				rotate <0,1234*sin(4.77*B*pi),0>*AST_DREH 									
				rotate <0,0,-90>
				scale (AST_L/BH)*(1-B/100*AST_VK)
				rotate <0,0,ASTNEIGUNG+((B/100)*((90-ASTNEIGUNG)))*A_NEIG>+<0,0,A_N_SP*sin(9.77*B*B*pi)>
				rotate <0,1456*cos(AST_SPIEL*7.19*B*pi)*(1-A_W_S)+90*A_W_S*sin((B/(100/AZ)-1/(100/AZ))*pi-pi/2)+90,0>
				translate <KK*sin(pi/2*sin(5.123*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(4.789*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(5.456*SKKFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)
				translate <SW*sin(pi/2*sin(5.456*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(5.123*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(4.789*SWFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)					
				translate <BG_X*sin(pi/2*sin(5.371*FZ_X*1.5*(AA+(B*AB/100))/BH)),(AA+(B*AB/100)),BG_Z*sin(pi/2*sin(5.937*FZ_Z*1.5*(AA+(B*AB/100))/BH))>
				rotate <0,TRUNK_SPIRAL*sin((AA+(B*AB/100))/(BH*3/5)),0>
			}
      #declare B=B+100/AZ;
			#end
    #declare A=A+BH/BRANCH_STRENGTH/300;
		#end		                              
		#end
		#end 
		#if (TWIG_STRENGTH>0)
		#if (AZ>=1)
		#if (ZWZ>=1)				
		#declare A=1;
		#while (A<BH)
			#declare B=1;
			#while (B<100)
				#declare C=1;
				#while (C<=(S_A+ #if (SZ/ZWZ>1) ((SZ/ZWZ-1)/(SZ/ZWZ))* #end B_A_T #if (SZ/ZWZ>1) +(B_A_T/(SZ/ZWZ))*ZE #end +ZE*(S_MA_R+S_MI_R)*(1-S_VJ_Z))/ZW_L*BH)
				sphere
				{
					<0,0,0>,TRUNK_RADIUS*5,TRUNK_RADIUS*10
					scale TRUNK_RADIUS/90
					scale ((S_A+B_A_T+(S_MA_R+S_MI_R)*(1-S_VJ_Z))-C*TT*ZW_L/BH)/((S_A+B_A_T+(S_MA_R+S_MI_R)*(1-S_VJ_Z)))
					scale (BH-((AB)*(B/100))*ST_VJ)/BH 
					scale (BH-AA*ST_VJ)/BH
					scale BH/AST_L
					scale AST_L/ZW_L
					scale ZW_VST*AST_ST
					translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*ZBFZA*C/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*ZBFZA*C/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*ZBFZA*C/BH))>*((BH-C)/BH)*ZW_K_A*(1-B/100*A_STR)*(1-A/BH*T_STR)
					translate <AW*sin(pi/2*sin(5.456*AWFZ*ZBFZA*C/BH)),AW*sin(pi/2*sin(5.123*AWFZ*ZBFZA*C/BH)),1*AW*sin(pi/2*sin(4.789*AWFZ*ZBFZA*C/BH))>*((BH-C)/BH)*ZW_K_A*(1-B/100*A_STR)*(1-A/BH*T_STR)
	        translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*ZBFZA*C/BH)+(1-ANS)*5.371*ABFZ*ZBFZA*C/BH),C,A_BG*sin(pi/2*sin(5.937*ABFZ*ZBFZA*C/BH))>*<1,1,H_ABG_S>*<ZW_K_A,1,ZW_K_A>*<1-B/100*A_STR,1,1-B/100*A_STR>*<(1-A/BH*T_STR),1,(1-A/BH*T_STR)>
	        rotate <0,BRANCH_SPIRAL*sin(C/BH*(pi/2)),0>
				scale ((Z_A+Z_B+ZW_L*(1-AST_VJ))-Z_A*AST_VJ*AST_L/BH)/(Z_A+Z_B+ZW_L*(1-AST_VJ))				
				scale ((Z_B+ZW_L*(1-AST_VJ))-A*AST_VJ*Z_B/BH)/(Z_B+ZW_L*(1-AST_VJ))				
	        rotate <0,1234*sin(4.77*A*pi),0>*ZW_DREH
					rotate <0,0,-90>
					scale ZW_L/BH					
					scale BH/AST_L
					rotate <0,0,V_ZW_N_SP*sin(9.77*A*A*pi)>+<0,0,(V_ZW_N-(V_ZW_N*(A*A)/(BH*BH)*V_ZW_NEIG))>
					rotate <0,H_ZW_N_SP*sin(7.77*A*A*pi),0>+<0,(H_ZW_N-(H_ZW_N*(A*A)/(BH*BH)*ZW_NEIG)),0>*((1-ZW_W_S)+(ZW_W_S*sin((A/(BH/ZWZ*AZ)-1/(BH/ZWZ*AZ))*pi-pi/2)))
					rotate <1456*cos(AST_SPIEL*7.19*A*pi)*(1-ZW_W_S),0,0>			
					rotate <0,0,90>	
					rotate <0,1234*sin(ZW_SPIEL*4.77*B*pi),0>*(1-ZW_W_S)					
			    rotate <0,-1234*sin(4.77*B*pi),0>*AST_DREH
					translate <A_KK*sin(pi/2*sin(5.123*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),A_KK*sin(pi/2*sin(4.789*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),A_KK*sin(pi/2*sin(5.456*AKKFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*((BH-(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)))/BH)*(1-B/100*A_STR)	      
			  	translate <AW*sin(pi/2*sin(5.456*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),AW*sin(pi/2*sin(5.123*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),AW*sin(pi/2*sin(4.789*AWFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*((BH-(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)))/BH)*<1,A_L_W,H_AW_S>*(1-B/100*A_STR)	
					translate <A_BG*sin(ANS*pi/2*sin(5.371*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)+(1-ANS)*(5.371*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)),(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH)),A_BG*sin(ANS*pi/2*sin(5.937*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH)+(1-ANS)*(5.937*ABFZ*(Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH))>*<-1,1,H_ABG_S>*<1-B/100*A_STR,1,1-B/100*A_STR>
					rotate <0,BRANCH_SPIRAL*sin((Z_A/AST_L*BH+(A*(Z_B/AST_L*BH)/BH))/BH*(pi/2)),0>
					rotate <0,1234*sin(4.77*B*pi),0>*AST_DREH
					rotate <0,0,-90>
					scale (AST_L/BH)*(1-B/100*AST_VK)
					rotate <0,0,ASTNEIGUNG+((B/100)*((90-ASTNEIGUNG)))*A_NEIG>+<0,0,A_N_SP*sin(9.77*B*B*pi)>
					rotate <0,1456*cos(AST_SPIEL*7.19*B*pi)*(1-A_W_S)+90*A_W_S*sin((B/(100/AZ)-1/(100/AZ))*pi-pi/2)+90,0>
					translate <KK*sin(pi/2*sin(5.123*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(4.789*SKKFZ*(AA+(B*AB/100))/BH)),KK*sin(pi/2*sin(5.456*SKKFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)
					translate <SW*sin(pi/2*sin(5.456*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(5.123*SWFZ*(AA+(B*AB/100))/BH)),SW*sin(pi/2*sin(4.789*SWFZ*(AA+(B*AB/100))/BH))>*((BH-(AA+(B*AB/100)))/BH)			
					translate <BG_X*sin(pi/2*sin(5.371*FZ_X*1.5*(AA+(B*AB/100))/BH)),(AA+(B*AB/100)),BG_Z*sin(pi/2*sin(5.937*FZ_Z*1.5*(AA+(B*AB/100))/BH))>
					rotate <0,TRUNK_SPIRAL*sin((AA+(B*AB/100))/(BH*3/5)),0>
				}
				#declare C=C+BH/200/TWIG_STRENGTH;
				#end 
			#declare B=B+100/AZ;
			#end
		#declare A=A+BH/ZWZ*AZ;
		#end
		#end
		#end
		#end
		#if (ROOT_STRENGTH>0)
		#if (ROOTS>=1)
		#declare A=1;
		#while (A<BH)		
			#declare B=1;
			#while (B<100)
			sphere
			{
				<0,0,0>,TRUNK_RADIUS*5,TRUNK_RADIUS*10
				scale (BH-A)/BH
				scale TRUNK_RADIUS/90
				scale W_ST 
				scale BH/W_L
				scale <1,1,1>+<-BARK_PROFILE*.9,BARK_PROFILE*.25,BARK_PROFILE*.25>*ROOT_BARK_PROFILE_CHANGE
				rotate <0,12347.3*sin(3.77*A*pi),0>
				translate <W_KK*sin(pi/2*sin(5.789*WKKFZ*A/BH)),W_KK*sin(pi/2*sin(4.4321*WKKFZ*A/BH)),W_KK*sin(pi/2*sin(5*WKKFZ*A/BH))>*((BH-A)/BH)
				translate <W_W*sin(pi/2*sin(5.123*WWFZ*A/BH)),W_W*sin(pi/2*sin(4.789*WWFZ*A/BH)),W_W*sin(pi/2*sin(5.456*WWFZ*A/BH))>*((BH-A)/BH)
				translate <W_BG*sin(pi/2*sin(5.371*WBFZ*A/BH)),A,W_BG*sin(pi/2*sin(5.937*WBFZ*A/BH))> 
				rotate <0,1234.773*sin(4.77*B*pi),0>*W_DREH
				rotate <0,0,-90>
				rotate <0,180,0>					
				scale W_L/BH 					
				rotate <0,0,ROOT_ANGLE+((B/100)*R_A*((90-ROOT_ANGLE)))>
				rotate <0,1456.371*sin(ROOT_RANDOM*71.19*B*pi)+180,0>
				translate <KK*sin(pi/2*sin(5.123*SKKFZ*(WA-(WA*B/100))/BH)),KK*sin(pi/2*sin(4.789*SKKFZ*(WA-(WA*B/100))/BH)),KK*sin(pi/2*sin(5.456*SKKFZ*(WA-(WA*B/100))/BH))>*((BH-(WA-(WA*B/100)))/BH)
				translate <SW*sin(pi/2*sin(5.456*SWFZ*(WA-(WA*B/100))/BH)),SW*sin(pi/2*sin(5.123*SWFZ*(WA-(WA*B/100))/BH)),SW*sin(pi/2*sin(4.789*SWFZ*(WA-(WA*B/100))/BH))>*((BH-(WA-(WA*B/100)))/BH)				
				translate <BG_X*sin(pi/2*sin(5.371*FZ_X*1.5*(WA-(WA*B/100))/BH)),(WA-(WA*B/100)),BG_Z*sin(pi/2*sin(5.937*FZ_Z*1.5*(WA-(WA*B/100))/BH))>
				rotate <0,TRUNK_SPIRAL*sin((WA-(WA*B/100))/(BH*3/5)),0>	
			}
			#declare B=B+100/ROOTS;
			#end
    #declare A=A+BH/150/ROOT_STRENGTH;
		#end
		#end
		#end		
	} 
	texture
	{ 
		#if (H_B_T>0)
		onion
		texture_map
		{
		[0 RINDE scale 1/(<AST_L,V_B_T*UNIT_1/H_B_T,AST_L>)]
		[H_B_T #if (FCaBE=1) RINDE1 #else RINDE #end scale .3/(<AST_L,V_B_T*UNIT_1/H_B_T,AST_L>)]
		[1 #if (FCaBE=1) LAUB #else RINDE #end scale .1/(<AST_L,V_B_T*UNIT_1/H_B_T,AST_L>)]
		}
		scale <AST_L,V_B_T*UNIT_1/H_B_T,AST_L>
		translate <0,0,0>
		#else 
		RINDE
		#end
	}
	scale 1/UNIT_1	
} 
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX END
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX