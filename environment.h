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
// Environment is the base class for raytracing. We use this in material to do recursion as that allows
// Scene which is derived from this to depend (indirectly) on Material.

#pragma once

#include "ray.h"
#include "colour.h"

class Environment {
public:

	// shoot a ray into the environment and get the colour and depth.
	// recurse indicates the level of recursion permitted.
	virtual void raytrace(Ray ray, int recurse, Colour& colour, float& depth)
	{
		colour.r = 0.0f;
		colour.g = 0.0f;
		colour.b = 0.0f;
		depth = 100000000.0f;
	}

	// raytrace a shadow ray. returns true if intersection found between 0 and limit along ray.
	virtual bool shadowtrace(Ray, float limit)
	{
		return false;
	}
};
