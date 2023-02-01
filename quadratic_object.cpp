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

#include "quadratic_object.h"
#include <math.h>

using namespace std;

Quadratic::Quadratic(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j,
                     float p_min_x, float p_max_x, float p_min_y, float p_max_y, float p_min_z, float p_max_z) {
    A = a;
    B = b;
    C = c;
    D = d;
    E = e;
    F = f;
    G = g;
    H = h;
    I = i;
    J = j;
    min_x = p_min_x;
    max_x = p_max_x;
    min_y = p_min_y;
    max_y = p_max_y;
    min_z = p_min_z;
    max_z = p_max_z;
}

Vector Quadratic::calc_normal(Vertex intersection) {
    Vector normal;
    normal.x = A * intersection.x + B * intersection.y + C * intersection.z + D;
    normal.y = B * intersection.x + E * intersection.y + F * intersection.z + G;
    normal.z = C * intersection.x + F * intersection.y + H * intersection.z + I;
    return normal;
}

Hit *Quadratic::intersection(Ray ray) {
    Vertex P = ray.position;
    Vector Di = ray.direction;

    // Last term is H not G (unlike the short note)
    float Aq = (A * Di.x * Di.x) + (2 * B * Di.x * Di.y) + (2 * C * Di.x * Di.z) + (E * Di.y * Di.y) + (2 * F * Di.y * Di.z) + (H * Di.z * Di.z);
    float Bq = 2 * ((A * P.x * Di.x) + (B * (P.x * Di.y + Di.x * P.y)) + (C * (P.x * Di.z + Di.x * P.z)) + (D * Di.x) + (E * P.y * Di.y) + (F * (P.y * Di.z + Di.y * P.z)) + (G * Di.y) + (H * P.z * Di.z) + (I * Di.z));
    float Cq = (A * P.x * P.x) + (2 * B * P.x * P.y) + (2 * C * P.x * P.z) + (2 * D * P.x) + (E * P.y * P.y) + (2 * F * P.y * P.z) + (2 * G * P.y) + (H * P.z * P.z) + (2 * I * P.z) + J;

    float disc = (Bq * Bq) - (4 * Aq * Cq);
    float t0, t1;

    if (disc < 0.0f) {
        return 0;
    } else if (disc == 0.0f) {
        t0 = -Cq / Bq;
    } else {
        t0 = (-Bq - sqrt(disc)) / (2.0f * Aq);
        t1 = (-Bq + sqrt(disc)) / (2.0f * Aq);
    }

    Hit *hit0 = new Hit();
    hit0->what = this;
    hit0->t = t0;
    hit0->entering = true;

    hit0->position = P + hit0->t * Di;
    hit0->normal = calc_normal(hit0->position);
    hit0->normal.normalise();

    if (hit0->normal.dot(ray.direction) > 0.0f) {
        hit0->normal.negate();
    }

    Hit *hit1 = new Hit();
    hit1->what = this;
    hit1->t = t1;
    hit1->entering = false;

    hit1->position = P + hit1->t * Di;
    hit1->normal = calc_normal(hit1->position);
    hit1->normal.normalise();

    if (hit1->normal.dot(ray.direction) > 0.0f) {
        hit1->normal.negate();
    }

    hit0->next = hit1;
    hit1->next = 0;

    // TODO: Add capping off quadratic

    return hit0;
}

void Quadratic::apply_transform(Transform &trans) {
    Transform initial = Transform(A, B, C, D, B, E, F, G, C, F, H, I, D, G, I, J);
    Transform result = trans.transpose() * initial * trans;

    A = result.matrix[0][0];
    B = result.matrix[0][1];
    C = result.matrix[0][2];
    D = result.matrix[0][3];
    E = result.matrix[1][1];
    F = result.matrix[1][2];
    G = result.matrix[1][3];
    H = result.matrix[2][2];
    I = result.matrix[2][3];
    J = result.matrix[3][3];
}
