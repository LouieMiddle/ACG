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

// Object is the base class for objects.
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "ray.h"
#include "hit.h"
#include "material.h"
#include "transform.h"

class Object {
public:
    Object *next;
    Material *material;

    Vertex centre;
    float radius;

    Object() {
        next = (Object *) 0;
        material = (Material *) 0;
    }

    // specify the material this object uses
    virtual void set_material(Material *p_m) {
        material = p_m;
    }

    // Given a ray, if this object intersects it, result all the points of intersection
    // resturn null if no intersections
    virtual Hit *intersection(Ray ray) {
        return 0;
    }

    // apply a transform to this object.
    virtual void apply_transform(Transform &trans) {
        return;
    }
};

#endif
