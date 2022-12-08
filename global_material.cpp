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
void GlobalMaterial::fresnel(float &kr, float cos_i) {
    float eta_i = 1.0f, eta_t = ior;
    // The ray starts in the medium that is not air so swap the refractive indexes
    if (cos_i > 0.0f) {
        swap(eta_i, eta_t);
    }
    // Compute sin_t using Snell's law
    float sin_t = (eta_i / eta_t) * sqrtf(max(0.0f, 1.0f - cos_i * cos_i));
    // Total internal reflection
    if (sin_t >= 1) {
        kr = 1;
    } else {
        float cos_t = sqrtf(max(0.0f, 1.0f - sin_t * sin_t));
        cos_i = fabsf(cos_i);
        float rs = ((eta_t * cos_i) - (eta_i * cos_t)) / ((eta_t * cos_i) + (eta_i * cos_t));
        float rp = ((eta_i * cos_i) - (eta_t * cos_t)) / ((eta_i * cos_i) + (eta_t * cos_t));
        kr = (rs * rs + rp * rp) / 2.0f;
    }
}

// TODO: Cite scratch a pixel
void GlobalMaterial::refract_ray(Vector &view, Vector &normal, Vector &refract_ray, float cos_i) {
    float eta_i = 1.0f, eta_t = ior;
    Vector n = normal;
    if (cos_i < 0.0f) {
        cos_i = -cos_i;
    } else {
        // The ray starts in the medium that is not air so swap the refractive indexes and negate normal
        swap(eta_i, eta_t);
        n = -normal;
    }

    float eta = eta_i / eta_t;
    float cos_t = sqrt(1.0f - (eta * eta) * (1.0f - cos_i * cos_i));
    refract_ray = eta * view + (eta * cos_i - cos_t) * n;
}

// reflection and recursion computation
Colour GlobalMaterial::compute_once(Ray &viewer, Hit &hit, int recurse) {
    Colour result = Colour(0.0f, 0.0f, 0.0f);

    if (recurse > 0) {
        float cos_i = viewer.direction.dot(hit.normal);
        bool outside = cos_i < 0;

        float kr;
        fresnel(kr, cos_i);

        Vector bias = 0.0001f * hit.normal;

        if (transmissive && kr < 1.0f) {
            Ray refraction_ray;
            float refraction_depth;
            Colour refract_colour = Colour(0.0f, 0.0f, 0.0f);
            refract_ray(viewer.direction, hit.normal, refraction_ray.direction, cos_i);
            refraction_ray.direction.normalise();
            refraction_ray.position = outside ? hit.position - bias : hit.position + bias;
            environment->raytrace(refraction_ray, recurse - 1, refract_colour, refraction_depth);
            result += refract_colour * (1 - kr);
        }

        Ray reflected_ray;
        float reflected_depth;
        Colour reflect_colour = Colour(0.0f, 0.0f, 0.0f);
        reflected_ray.direction = viewer.direction - 2.0f * cos_i * hit.normal;
        reflected_ray.direction.normalise();
        reflected_ray.position = outside ? hit.position + bias : hit.position - bias;
        environment->raytrace(reflected_ray, recurse - 1, reflect_colour, reflected_depth);

        result = transmissive ? result + reflect_colour * kr : result + reflect_colour * 0.8;
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
