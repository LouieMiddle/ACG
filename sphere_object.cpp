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

#include "sphere_object.h"
#include <math.h>

Sphere::Sphere(Vertex c, float r) {
    center = c;
    radius = r;
}

Hit *Sphere::intersection(Ray ray) {
    Vector ro;

    // offset ray by sphere position
    // equivalent to transforming ray into local sphere space

    ro.x = ray.position.x - center.x;
    ro.y = ray.position.y - center.y;
    ro.z = ray.position.z - center.z;

    float a = (float) ray.direction.dot(ray.direction);
    float b = (float) (2.0 * ray.direction.dot(ro));
    float c = (float) ro.dot(ro) - radius * radius;

    float disc = b * b - 4 * a * c;

    if (disc < 0.0f) {
        return 0; // a negative value indicates no intersection.
    }

    // an intersection has been found, record details in hit objects

    float ds = sqrtf(disc);

    float q = (b > 0) ? -0.5 * (b + ds) : -0.5 * (b - ds);

    float t0 = q / a;
    float t1 = c / q;

    if (t0 > t1) {
        swap(t0, t1);
    }

    Hit *hit0 = 0;
    if (t0 > 0)  {
        hit0 = new Hit();
        hit0->what = this;
        hit0->t = t0;
        hit0->entering = true;

        hit0->position = ray.position + hit0->t * ray.direction;
        hit0->normal = hit0->position - center;
        hit0->normal.normalise();

        if (t1 < 0) {
            hit0->next = 0;
            return hit0;
        }
    }

    Hit *hit1 = new Hit();
    hit1->what = this;
    hit1->t = t1;
    hit1->entering = false;

    hit1->position = ray.position + hit1->t * ray.direction;
    hit1->normal = hit1->position - center;
    hit1->normal.normalise();

    hit1->next = 0;

    if (hit0 != 0) {
        hit0->next = hit1;
        return hit0;
    } else {
        delete hit0;
        return hit1;
    }
}

void Sphere::apply_transform(Transform &transform) {
    transform.apply(center);
}
