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

// Hit is a class to store and maniplulate information about an intersection
// between a ray and an object.

#include "hit.h"

Hit* Hit::free_pool = (Hit *)0;

int Hit::allocated = 0;
int Hit::pool_size = 0;

void* Hit::operator new(size_t size)
{
	allocated += 1;

	if (free_pool == 0)
	{
		Hit* pool = new Hit[100];
		free_pool = pool;

		for (int i = 0; i < 99; i += 1)
		{
			pool[i].next = &pool[i+1];
		}
		pool[99].next = 0;

		pool_size += 100;
	}

	Hit* next = free_pool;
	free_pool = free_pool->next;
	next->next = 0;
	return next;
}

void Hit::operator delete(void* p)
{
	allocated -= 1;
	((Hit*)p)->next = free_pool;
	free_pool = (Hit*)p;
}

ostream& operator<<(ostream& os, const Hit& h)
{
  os << "Hit{" <<",[" << h.position.x << "," << h.position.y << "," << h.position.z << "],[" << h.normal.x << "," << h.normal.y << "," << h.normal.z << "]}\n";
  return os;
}
