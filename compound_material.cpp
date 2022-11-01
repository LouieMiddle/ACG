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

 // A simple Phong based lighting model.

#include "compound_material.h"


CompoundMaterial::CompoundMaterial(int p_number)
{
	number = p_number;
	for (int i = 0; i < number; i += 1)
	{
		materials[i] = 0;
	}
}

CompoundMaterial::~CompoundMaterial()
{
}

void CompoundMaterial::include_material(Material *p_new)
{
	for (int i = 0; i < number; i += 1)
	{
		if (materials[i] == 0)
		{
			materials[i] = p_new;
			return;
		}
	}
}

// The compute_once() method supplies the ambient term.
Colour CompoundMaterial::compute_once(Ray& viewer, Hit& hit, int recurse)
{
	Colour result = Colour(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < number; i += 1)
	{
		if (materials[i] == 0) return result;

		result += materials[i]->compute_once(viewer, hit, recurse);
	}

	return result;
}

// The compute_per_light() method supplies the diffuse and specular terms.
Colour CompoundMaterial::compute_per_light(Vector& viewer, Hit& hit, Vector& ldir)
{
	Colour result;

	for (int i = 0; i < number; i += 1)
	{
		if (materials[i] == 0) return result;

		result += materials[i]->compute_per_light(viewer, hit, ldir);
	}

	return result;
}

