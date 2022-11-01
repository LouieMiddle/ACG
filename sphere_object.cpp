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


Sphere::Sphere(Vertex c, float r)
{
	center = c;
	radius = r;
}

Hit *Sphere::intersection(Ray ray)
{

	Vector ro;

	// offset ray by sphere position
	// equivalent to transforming ray into local sphere space

	ro.x = ray.position.x - center.x;
	ro.y = ray.position.y - center.y;
	ro.z = ray.position.z - center.z;

	float a = (float)ray.direction.dot(ray.direction);
	float b = (float)(2.0 * ray.direction.dot(ro));
	float c = (float)ro.dot(ro) - radius*radius;

	float disc = b*b - 4 * a*c;

	if (disc < 0.0f)
	{
		return 0; // a negative value indicates no intersection.
	}

	// an intersection has been found, record details in hit objects

	float ds = sqrtf(disc);

	float t0 = (-b - ds) / 2.0f;
	float t1 = (-b + ds) / 2.0f;

	Hit* hit0 = new Hit();
	hit0->what = this;
	hit0->t = t0;
	hit0->entering = true;

	hit0->position = ray.position + hit0->t * ray.direction;
	hit0->normal = hit0->position - center;
	hit0->normal.normalise();

	if (hit0->normal.dot(ray.direction) > 0.0)
	{
		hit0->normal.negate();
	}

	Hit* hit1 = new Hit();
	hit1->what = this;
	hit1->t = t1;
	hit1->entering = false;

	hit1->position = ray.position + hit1->t * ray.direction;
	hit1->normal = hit1->position - center;
	hit1->normal.normalise();

	if (hit1->normal.dot(ray.direction) > 0.0)
	{
		hit1->normal.negate();
	}

	hit0->next = hit1;
	hit1->next = 0;

	return hit0;
}

void Sphere::apply_transform(Transform& transform)
{
	transform.apply(center);
}