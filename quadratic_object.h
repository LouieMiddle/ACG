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

// The Quadratic Object defines and intersects with quadratic object_list.

#pragma once

#include "object.h"

class Quadratic : public Object {
    float A, B, C, D, E, F, G, H, I, J, min_x, max_x, min_y, max_y, min_z, max_z;
public:
    Quadratic(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j,
              float p_min_x, float p_max_x, float p_min_y, float p_max_y, float p_min_z, float p_max_z);

    Hit *intersection(Ray ray);

    void apply_transform(Transform &trans);

    Vector calc_normal(Vertex intersection);
};
