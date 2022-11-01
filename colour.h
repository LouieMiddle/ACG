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

// Colour is a class to store and maniplulate an rgba colour.

#pragma once

class Colour {
public:
	float r;
	float g;
	float b;
	float a;

	Colour()
	{
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		a = 1.0f;
	}

	Colour(float p_r, float p_g, float p_b)
	{
		r = p_r;
		g = p_g;
		b = p_b;
		a = 1.0;
	}

	Colour(float pr, float pg, float pb, float pa)
	{
		r = pr;
		g = pg;
		b = pb;
		a = pa;
	}

	void scale(Colour &scaling)
	{
		r *= scaling.r;
		g *= scaling.g;
		b *= scaling.b;
		a *= scaling.a;
	}

	void add(Colour &adjust)
	{
		r += adjust.r;
		g += adjust.g;
		b += adjust.b;
	}

	Colour operator* (Colour& other)
	{
		Colour result;
		result.r = r * other.r;
		result.g = g * other.g;
		result.b = b * other.b;
		result.a = a * other.a;
		return result;
	}

	Colour operator* (Colour other)
	{
		Colour result;
		result.r = r * other.r;
		result.g = g * other.g;
		result.b = b * other.b;
		result.a = a * other.a;
		return result;
	}

	Colour operator+ (Colour& other)
	{
		Colour result;
		result.r = r + other.r;
		result.g = g + other.g;
		result.b = b + other.b;
		result.a = a + other.a;
		return result;
	}


	Colour operator+ (Colour other)
	{
		Colour result;
		result.r = r + other.r;
		result.g = g + other.g;
		result.b = b + other.b;
		result.a = a + other.a;
		return result;
	}

	Colour operator* (float rhs)
	{
		Colour result;
		result.r = r * rhs;
		result.g = g * rhs;
		result.b = b * rhs;
		result.a = a;
		return result;
	}

	Colour operator+= (Colour& other)
	{
		r += other.r;
		g += other.g;
		b += other.b;
		a += other.a;
		return *this;
	}

	Colour operator+= (Colour other)
	{
		r += other.r;
		g += other.g;
		b += other.b;
		a += other.a;
		return *this;
	}

	Colour operator*= (Colour& other)
	{
		r *= other.r;
		g *= other.g;
		b *= other.b;
		a *= other.a;
		return *this;
	}

	Colour operator*= (Colour other)
	{
		r *= other.r;
		g *= other.g;
		b *= other.b;
		a *= other.a;
		return *this;
	}
};
