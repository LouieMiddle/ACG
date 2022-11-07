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

#include "phong_material.h"

#include <math.h>

Phong::Phong(Colour p_ambient, Colour p_diffuse, Colour p_specular, float p_power) {
    ambient = p_ambient;
    diffuse = p_diffuse;
    specular = p_specular;
    power = p_power;
}

// The compute_once() method supplies the ambient term.
Colour Phong::compute_once(Ray &viewer, Hit &hit, int recurse) {
    Colour result;
    result.r = ambient.r;
    result.g = ambient.g;
    result.b = ambient.b;
    return result;
}

// The compute_per_light() method supplies the diffuse and specular terms.
Colour Phong::compute_per_light(Vector &viewer, Hit &hit, Vector &ldir) {
    Colour result = Colour(0.0f, 0.0f, 0.0f);

    float diff;

    ldir.negate();

    viewer.negate();

    diff = hit.normal.dot(ldir);

    if (diff < 0.0f) { // light is behind surface
        return result;
    }

    // diffuse
    result.r += diffuse.r * diff;
    result.g += diffuse.g * diff;
    result.b += diffuse.b * diff;

    // the specular component
    Vector r;

    hit.normal.reflection(ldir, r);
    r.normalise();

    float h;
    h = r.dot(viewer);

    if (h > 0.0f) {
        float p = (float) pow(h, power);

        result.r += specular.r * p;
        result.g += specular.g * p;
        result.b += specular.b * p;
    }

    return result;
}

