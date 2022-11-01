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

// Hit is a class to store and maniplulate information about an intersection
// between a ray and an object.

#pragma once

#include <iostream>
using namespace std;

#include "vertex.h"
#include "vector.h"

class Object;

class Hit {
public:
	static Hit *free_pool;
	static int allocated;
	static int pool_size;

	float   t;			// The intersection distance
	bool    entering;   // True if entering an object, false if leaving
	Hit    *next;		// The next hit in a list along a ray
	Object *what;		// the object that has been hit
	Vertex  position;	// the position of intersection
	Vector  normal;		// the normal at the point of intersection

	void* operator new(size_t size);
	void operator delete(void* p);
};
