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

// Material is the base class for materials.

#pragma once

#include "colour.h"
#include "ray.h"
#include "hit.h"

class Material {
public:
    // compute_once is called once per intersection
    virtual Colour compute_once(Ray &viewer, Hit &hit, int recurse) {
        Colour result;
        result.r = 0.0f;
        result.g = 0.0f;
        result.b = 0.0f;
        return result;
    }

    // compute_per_light is called for each light that reaches a surface
    virtual Colour compute_per_light(Vector &viewer, Hit &hit, Vector &ldir) {
        Colour result;
        result.r = 0.0f;
        result.g = 0.0f;
        result.b = 0.0f;
        return result;
    }

    // You will need additional material methods to support Photon-mapping.
};
