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

#include "phong_material_photon_mapping.h"

#include <math.h>

PhongPhotonMapping::PhongPhotonMapping(Colour p_ambient, Colour p_diffuse, Colour p_specular, float p_power, float p_kd, float p_ks) {
    ambient = p_ambient;
    diffuse = p_diffuse;
    specular = p_specular;
    power = p_power;
    kd = p_kd;
    ks = p_ks;
}

Colour PhongPhotonMapping::compute_ambient() {
    Colour result;
    result = ambient;
    return result;
}

Colour PhongPhotonMapping::compute_diffuse(Hit &hit, Vector &ldir) {
    Colour result;

    float diff;

    Vector tolight;

    result.r = 0.0f;
    result.g = 0.0f;
    result.b = 0.0f;

    tolight = ldir;
    tolight.negate();

    diff = hit.normal.dot(tolight);

    // Scene.raytrace() does this test, but let's keep it here in case that changes or we get called from elsewhere.
    if (diff < 0.0f) // light is behind surface
    {
        return result;
    }

    // diffuse

    result += diffuse * diff;

    return result;
}

// The compute_per_light() method supplies the specular terms only for photon mapping
Colour PhongPhotonMapping::compute_per_light(Vector &viewer, Hit &hit, Vector &ldir) {
    Colour result;

    float diff;

    Vector tolight;
    Vector toviewer;

    result.r = 0.0f;
    result.g = 0.0f;
    result.b = 0.0f;

    tolight = ldir;
    tolight.negate();

    toviewer = viewer;
    toviewer.negate();

    diff = hit.normal.dot(tolight);

    // Scene.raytrace() does this test, but let's keep it here in case that changes or we get called from elsewhere.
    if (diff < 0.0f) // light is behind surface
    {
        return result;
    }

    // the specular component

    Vector r;

    hit.normal.reflection(tolight, r);
    r.normalise();

    float h = r.dot(toviewer);

    if (h > 0.0f) {
        float p = (float) pow(h, power);

        result += specular * p;
    }

    return result;
}
