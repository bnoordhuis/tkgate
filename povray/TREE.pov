// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// Persistence of Vision 3.1 Scene Description File
// File: TREE.pov
// Desc: TREEGENERATOR EXAMPLE SCENE FILE 
// Date: MARCH 2000
// Auth: AUST
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//**********************************************GLOBAL SETTINGS
#default{texture{pigment{color rgb <1,1,1>}finish{ambient 0.0}}}
sky_sphere{pigment{color rgb 1}}
//*****************************************************INCLUDES

//*****************************************************TEXTURES

//*******************************************************BLOCKS

//*******************************************************CAMERA
camera
{
	direction 1.5*z
	right 1*x
	location <0,500,-1700>
	look_at <0,500,0>
}
//********************************************************LIGHT
light_source
{
	<0,500,-1000>
	color rgb <.3,.25,.2>
	shadowless
}

light_source
{
	<100000,180000,-100000>
	color rgb <2,2,2>
}
//***************************************************ATMOSPHERE

//*************************************************************
//*************************************************************
//*******************************************************ACTION
#include "PALM.inc"
#include "TOMTREE.inc"
object
{
	TREE
	scale 1000
}
//**********************************************************END
//*************************************************************
