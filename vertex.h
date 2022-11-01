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

// A four element vector class with lots of operators and common functions

#pragma once

#include "vector.h"

class Vertex : public Vector {
public:
    float w;

    Vertex() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
        w = 1.0f;
    }

    Vertex(float px, float py, float pz, float pw) {
        x = px;
        y = py;
        z = pz;
        w = pw;
    }

    Vertex(float px, float py, float pz) {
        x = px;
        y = py;
        z = pz;
        w = 1.0f;
    }


    Vertex operator+(Vector const &b) {
        Vertex r;
        r.x = this->x + b.x;
        r.y = this->y + b.y;
        r.z = this->z + b.z;
        r.w = this->w;
        return r;
    }

    Vertex operator-(Vector const &b) {
        Vertex r;
        r.x = this->x - b.x;
        r.y = this->y - b.y;
        r.z = this->z - b.z;
        r.w = this->w;
        return r;
    }

    Vertex operator-() {
        Vertex r;
        r.x = -this->x;
        r.y = -this->y;
        r.z = -this->z;
        r.w = -this->w;

        return r;
    }
};
