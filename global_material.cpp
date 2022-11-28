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

GlobalMaterial::GlobalMaterial(Environment *p_env, float p_ior) {
    environment = p_env;
    ior = p_ior;
}

void GlobalMaterial::fresnel(Vector &view, Vector &normal, float &kr) {
    float cosi = clamp(-1, 1, view.dot(normal));
    float etai = 1, etat = ior;
    // The ray starts in the medium that is not air so swap the refractive indexes
    if (cosi > 0) {
        std::swap(etai, etat);
    }
    // Compute sint using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));

    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    } else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (rs * rs + rp * rp) / 2;
    }
}

void GlobalMaterial::refract_ray(Vector &view, Vector &normal, Vector &refract_ray) {
    float cosi = clamp(-1, 1, view.dot(normal));
    float etai = 1, etat = ior;
    Vector n = normal;
    if (cosi < 0) {
        cosi = -cosi;
    } else {
        // The ray starts in the medium that is not air so swap the refractive indexes and negate normal
        std::swap(etai, etat);
        n = -normal;
    }

    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    if (k < 0) {
        refract_ray = Vector(0.0f, 0.0f, 0.0f);
    } else {
        refract_ray = eta * view + (eta * cosi - sqrtf(k)) * n;
    }
}

// reflection and recursion computation
Colour GlobalMaterial::compute_once(Ray &viewer, Hit &hit, int recurse) {
    Colour result = Colour(0.0f, 0.0f, 0.0f);

    if (recurse > 0) {
        float kr;
        fresnel(viewer.direction, hit.normal, kr);

        bool outside = viewer.direction.dot(hit.normal) < 0;

        Vector bias = 0.0001f * hit.normal;

        Colour refract_colour;
        if (kr < 1) {
            Ray refraction_ray;
            refract_ray(viewer.direction, hit.normal, refraction_ray.direction);
            refraction_ray.position = outside ? hit.position - bias : hit.position + bias;
            environment->raytrace(refraction_ray, recurse - 1, refract_colour, hit.t);
        }

        Ray reflected_ray;
        Colour reflect_colour;
        reflected_ray.direction = viewer.direction - 2 * hit.normal.dot(viewer.direction) * hit.normal;
        reflected_ray.direction.normalise();
        reflected_ray.position = outside ? hit.position - (0.0001f * reflected_ray.direction) : hit.position + (0.0001f * reflected_ray.direction);
        environment->raytrace(reflected_ray, recurse - 1, reflect_colour, hit.t);

        result = result + reflect_colour * kr + refract_colour * (1 - kr);
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
