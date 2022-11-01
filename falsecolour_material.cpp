/****************************************************************************
*
* krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
*
* I've put a lot of time and effort into this code. For the last decade
* it's been used to introduce hundreds of students at multiple universities
* to raytracing. It forms the basis of your coursework but you are free
* to continue using/developing forever more. However, I ask that you don't
* share the code or your derivitive versions publicly. In order to continue
* to be used for coursework and in particular assessment it's important that
* versions containing solutions are not searchable on the web or easy to
* download.
*
* If you want to show off your programming ability, instead of releasing
* the code, consider generating an incredible image and explaining how you
* produced it.
*/

 // This class maps the x,y,z components of the normal to the r,g,b components
 // of the returned colour. A useful debug tool. -1 to +1 maps to 0 to 1

#include "falsecolour_material.h"

#include <math.h>

// The compute_once() method maps the normal to the colour to aid debug.
Colour FalseColour::compute_once(Ray& viewer, Hit& hit, int recurse)
{
	Colour result;
	result.r = (hit.normal.x + 1.0f) * 0.5f;
	result.g = (hit.normal.y + 1.0f) * 0.5f;
	result.b = (hit.normal.z + 1.0f) * 0.5f;
	return result;
}

// The compute_per_light() method makes no contribution.
Colour FalseColour::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
	Colour result = Colour(0.0f, 0.0f, 0.0f);

	return result;
}

