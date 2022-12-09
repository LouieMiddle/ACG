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

#pragma once

#include "colour.h"
#include "ray.h"
#include "object.h"
#include "light.h"
#include "hit.h"
#include "environment.h"
#include "photon.h"
#include "alglib/stdafx.h"
#include "alglib/alglibmisc.h"
#include "photon_map.h"
#include "utils.h"

// Scene is a class that is used to build a scene database of object_list
// and light_list and then trace a ray through it.

using namespace alglib;

class Scene : public Environment {
public:
    Object *object_list;
    Light *light_list;

    // Photon map for normal and shadow photons
    PhotonMap *photon_map;

    // Second photon map for caustics as described by Jensen
    PhotonMap *caustic_photon_map;

    Scene();

    // Trace a ray through the scene and return its colour. This function
    // is the one that should recurse down the reflection/refraction tree within a material
    void raytrace(Ray ray, int recurse, Colour &colour, float &depth);

    void add_object(Object *obj);

    void add_light(Light *light);

    void set_photon_map(PhotonMap *p_photon_map);

    Colour estimate_radiance(Hit &hit, const vector<Photon *>& local_photons);
};
