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

#include "global_material.h"

#include <math.h>

GlobalMaterial::GlobalMaterial(Environment *p_env, float p_ior, bool p_transmissive) {
    environment = p_env;
    ior = p_ior;
    transmissive = p_transmissive;
}

// TODO: Cite scratch a pixel
void GlobalMaterial::fresnel(Vector &view, Vector &normal, float &kr, float cosi) {
    float etai = 1.0f, etat = ior;
    // The ray starts in the medium that is not air so swap the refractive indexes
    if (cosi > 0.0f) {
        std::swap(etai, etat);
    }
    // Compute sint using Snell's law
    float sint = (etai / etat) * sqrtf(std::max(0.0f, 1.0f - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    } else {
        float cost = sqrtf(std::max(0.0f, 1.0f - sint * sint));
        cosi = fabsf(cosi);
        float rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (rs * rs + rp * rp) / 2.0f;
    }
}

// TODO: Cite scratch a pixel
void GlobalMaterial::refract_ray(Vector &view, Vector &normal, Vector &refract_ray, float cosi) {
    float etai = 1.0f, etat = ior;
    Vector n = normal;
    if (cosi < 0.0f) {
        cosi = -cosi;
    } else {
        // The ray starts in the medium that is not air so swap the refractive indexes and negate normal
        std::swap(etai, etat);
        n = -normal;
    }

    float eta = etai / etat;
    float k = 1.0f - (eta * eta) * (1.0f - cosi * cosi);
    float cost = sqrt(k);
    refract_ray = eta * view + (eta * cosi - cost) * n;
}

// reflection and recursion computation
Colour GlobalMaterial::compute_once(Ray &viewer, Hit &hit, int recurse) {
    Colour result = Colour(0.0f, 0.0f, 0.0f);

    if (recurse > 0) {
        float cosi = clamp(-1.0f, 1.0f, viewer.direction.dot(hit.normal));
        bool outside = cosi < 0;

        float kr;
        fresnel(viewer.direction, hit.normal, kr, cosi);

        Vector bias = 0.0001f * hit.normal;

        Colour refract_colour = Colour(0.0f, 0.0f, 0.0f);
        if (transmissive && kr < 1.0f) {
            Ray refraction_ray;
            refract_ray(viewer.direction, hit.normal, refraction_ray.direction, cosi);
            refraction_ray.direction.normalise();
            refraction_ray.position = outside ? hit.position - bias : hit.position + bias;
            environment->raytrace(refraction_ray, recurse - 1, refract_colour, hit.t);
        }

        Ray reflected_ray;
        Colour reflect_colour = Colour(0.0f, 0.0f, 0.0f);
        reflected_ray.direction = viewer.direction - 2.0f * hit.normal.dot(viewer.direction) * hit.normal;
        reflected_ray.direction.normalise();
        reflected_ray.position = outside ? hit.position + bias : hit.position - bias;
        environment->raytrace(reflected_ray, recurse - 1, reflect_colour, hit.t);

        result = transmissive ? result + reflect_colour * kr + refract_colour * (1 - kr) : result + reflect_colour * 0.8;
    }

    return result;
}

Colour GlobalMaterial::compute_per_light(Vector &viewer, Hit &hit, Vector &ldir) {
    Colour result;

    result.r = 0.0f;
    result.g = 0.0f;
    result.b = 0.0f;

    return result;
}

float GlobalMaterial::clamp(float lower, float upper, float number) {
    if (number < lower) {
        return lower;
    }
    if (number > upper) {
        return upper;
    }
    return number;
}
