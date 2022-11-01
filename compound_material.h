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

// CompoundMaterial is a Material that applies multiple other materials to a surface.
// it can be used to combine a PhongMaterial and a GlobalMaterial on a single surface.

#pragma once

#include "material.h"

class CompoundMaterial : public Material {
public:
	int number;
	Material *materials[10];

	// the constructor takes a single parameter, the maximum number of materials to combine.
	CompoundMaterial(int p_number);
	~CompoundMaterial();

	// include material added another material to the compound material
	void include_material(Material *p_new);

	Colour compute_once(Ray& viewer, Hit& hit, int recurse);

	Colour compute_per_light(Vector& viewer, Hit& hit, Vector& ldir);
};
