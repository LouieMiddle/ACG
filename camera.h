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

// Camera is the base class for cameras.
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "ray.h"
#include "environment.h"
#include "framebuffer.h"

class Camera {
public:

	// an empty constructor
	Camera()
	{
	}

	// Given an environment (typically the derived Scene class, fill in the framebuffer pixels, both colour and depth.
	virtual void render(Environment& env, FrameBuffer& fb)
	{
	}

	
};

#endif
