//these declarations are only the bodies for the leaves below
#declare BODY=
height_field
{
	tga "LEAF.tga"
	water_level .01
	smooth
	translate <0,0,-0.5>
}
#declare BODY_tex=
object
{
	BODY
	pigment
	{
		image_map
		{
			gif "LEAF.gif"
			interpolate 2
		}
		rotate <90,0,0>
		translate <0,0,-.5>
	}
}
#declare BODY_cut=
height_field
{
	tga "LEAFx.tga"
	smooth
	water_level .001
	scale <1,1.1,1>
	translate <0,0,-.5>
}
#declare BLOS_BODY=
height_field
{
	tga "BLOSSOM.tga"
	water_level .05
	smooth
	translate <-.5,0,-0.5>
}


//XXXXXXXXXXXXX L E A V E S XXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//Simple leaves
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//LEAF - leaf for a good and faster preview and for small simple leaf-shapes; it's only a sphere
#declare LEAF=
sphere
{
	<.5,0,0>,.5
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//CSG_LEAF - for small simple leaf-shapes; with concave underside; needs more memory than LEAF
#declare CSG_LEAF=
sphere
{
	<.5,0,0>,.5
	clipped_by
	{
		cylinder
		{
			<.5,0,0>,<.5,1.1,0>,.5
			scale <1,1,2/3>
		}
	}
	scale <1,1,3/2>
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//Better height-field-leaves
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//HF_LEAF - with natural outline; good for many leaf-types
#declare HF_LEAF=
object
{
	BODY
	clipped_by
	{
		object
		{
			BODY_cut
			scale <.999,1,.99>
		}
	}
}

//HF_LEAF_tex - this leaf has the same shape than LEAF, but with a realistic texture;
//the disadvantage is that all leaves at the tree get the same texture/coloring
#declare HF_LEAF_tex=
object
{
	BODY_tex
	clipped_by
	{
		object
		{
			BODY_cut
			scale <.999,1,.99>
		}
	}
}

//HF_PALMLEAF - for long, slim leaves you better use the following PALMLEAVES because of better structure-scaling
#declare HF_PALMLEAF=
object
{
	BODY
	clipped_by
	{
		object
		{
			BODY_cut
			scale <.999,1,.5>
		}
	}
	scale <1,1,2>
}

//HF_PALMLEAF_tex - this leaf has the same shape than PALMLEAF, but with a realistic texture;
//the disadvantage is that all leaves at the tree get the same texture/coloring
#declare HF_PALMLEAF_tex=
object
{
	BODY_tex
	clipped_by
	{
		object
		{
			BODY_cut
			scale <.999,1,.4>
		}
	}
	scale <1,1,2.5>
}

//HF_PALMLEAF1 - this leaf is equal to PALMLEAF, but with a round tip
#declare HF_PALMLEAF1=
object
{
	BODY
	translate <-.5,0,0>
	scale <1,1,1>
	clipped_by
	{
		cylinder
		{
			<0,-2,0>,<0,2,0>,.5
			scale <.99,1,.4>
		}
	}
	scale <1,1,2.5>
	translate <.5,0,0>
}

//HF_PALMLEAF1_tex - and again with its own texture
#declare HF_PALMLEAF1_tex=
object
{
	BODY_tex
	translate <-.5,0,0>
	scale <1,1,1>
	clipped_by
	{
		cylinder
		{
			<0,-2,0>,<0,2,0>,.5
			scale <.99,1,.4>
		}
	}
	scale <1,1,2.5>
	translate <.5,0,0>
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//Some blossoms
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//BLOSSOM1 - with six round small leaves
#declare BLOSSOM1=
object
{
	BLOS_BODY
	scale <1,.5,1>
	clipped_by
	{
		torus
		{
			.275,.24
			scale <1.0,1.6,1.0>
			inverse
		}
	}
	scale <1,2,1>
	rotate <0,0,-90>
	translate <-.05,0,0>
	finish{ambient .2}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//BLOSSOM2 - with six slim small leaves
#declare BLOSSOM2=
object
{
	BLOS_BODY
	scale <1,.5,1>
	clipped_by
	{
		torus
		{
			.275,.23
			scale <1.0,1.85,1.0>
			inverse
		}
	}
	scale <1,2,1>
	rotate <0,0,-90>
	translate <-.05,0,0>
	finish{ambient .2}
}





