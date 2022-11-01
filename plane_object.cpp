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

#include "plane_object.h"

Plane::Plane(float p_a, float p_b, float p_c, float p_d)
{
	a = p_a;
	b = p_b;
	c = p_c;
	d = p_d;
}

Hit* Plane::intersection(Ray ray)
{
	float U = a * ray.position.x + b * ray.position.y + c * ray.position.z + d;
	float V = a * ray.direction.x + b * ray.direction.y + c * ray.direction.z;

	if (V == 0.0f)
	{
		//parallel to plane
		if (U < 0.0f)
		{
			//is inside
			Hit* hit1 = new Hit();
			Hit* hit2 = new Hit();
			hit1->entering = true;
			hit1->t = -10000000000.0f; // infinity
			hit1->what = this;
			hit2->entering = false;
			hit2->t = 10000000000.0f; // infinity
			hit2->what = this;
			hit1->next = hit2;
			hit2->next = 0;
			return hit1;
		}

		//is outside
		return 0;
	} if (V > 0)
	{
		float t = U/-V;
		Hit* hit1 = new Hit();
		Hit* hit2 = new Hit();
		hit1->entering = true;
		hit1->t = -10000000000.0f; // infinity
		hit1->what = this;
		hit2->entering = false;
		hit2->t = t; // infinity
		hit2->position = ray.position + t * ray.direction;
		hit2->normal = Vector(a, b, c);
		if (hit2->normal.dot(ray.direction) > 0.0)
		{
			hit2->normal.negate();
		}
		hit2->what = this;
		hit1->next = hit2;
		hit2->next = 0;
		return hit1;
	}
	else
	{
		float t = U / -V;
		Hit* hit1 = new Hit();
		Hit* hit2 = new Hit();
		hit1->entering = true;
		hit1->t = t; // infinity
		hit1->position = ray.position + t * ray.direction;
		hit1->normal = Vector(a, b, c);
		if (hit1->normal.dot(ray.direction) > 0.0)
		{
			hit1->normal.negate();
		}
		hit1->what = this;
		hit2->entering = false;
		hit2->t = 10000000000.0f; // infinity
		hit2->what = this;
		hit1->next = hit2;
		hit2->next = 0;
		return hit1;
	}
}

void Plane::apply_transform(Transform& trans)
{
	Transform ti = trans.inverse().transpose();
	Vertex v = Vertex(a, b, c, d);

	ti.apply(v);

	a = v.x;
	b = v.y;
	c = v.z;
	d = v.w;
}