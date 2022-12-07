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

// Light is the base class for lights.

#pragma once

#include "vertex.h"
#include "vector.h"
#include "colour.h"

class Light {
public:
    Vertex position;
    Vector direction;
    Colour intensity;

    Light *next;

    Light() {
        next = (Light *) 0;
    }

    // Get the direction towards the light at the point on the surface
    // should return true if the surface is in front of the light
    // and false if it's behind and not illuminated.
    virtual bool get_direction(Vertex &surface, Vector &dir, float &dist) {
        return false;
    }

    // Get the intensity of the light in the direction of the surface
    virtual void get_intensity(Vertex &surface, Colour &intensity) {
    }

    // TODO: You will need additional light methods to support Photon-mapping.
};
