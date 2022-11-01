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

// Transform is a class to store, manipulate and apply transforms.

#pragma once

#include "vertex.h"

class Transform {
public:
  
  float matrix[4][4];

  Transform()
  {
      matrix[0][0] = 1.0f;
      matrix[0][1] = 0.0f;
      matrix[0][2] = 0.0f;
      matrix[0][3] = 0.0f;
      matrix[1][0] = 0.0f;
      matrix[1][1] = 1.0f;
      matrix[1][2] = 0.0f;
      matrix[1][3] = 0.0f;
      matrix[2][0] = 0.0f;
      matrix[2][1] = 0.0f;
      matrix[2][2] = 1.0f;
      matrix[2][3] = 0.0f;
      matrix[3][0] = 0.0f;
      matrix[3][1] = 0.0f;
      matrix[3][2] = 0.0f;
      matrix[3][3] = 1.0f;
  };
  
  Transform(float a, float b, float c, float d,
	    float e, float f, float g, float h,
	    float i, float j, float k, float l,
	    float m, float n, float o, float p)
  {
      matrix[0][0] = a;
      matrix[0][1] = b;
      matrix[0][2] = c;
      matrix[0][3] = d;
      matrix[1][0] = e;
      matrix[1][1] = f;
      matrix[1][2] = g;
      matrix[1][3] = h;
      matrix[2][0] = i;
      matrix[2][1] = j;
      matrix[2][2] = k;
      matrix[2][3] = l;
      matrix[3][0] = m;
      matrix[3][1] = n;
      matrix[3][2] = o;
      matrix[3][3] = p;
  };

  void apply(Vertex &in, Vertex &out)
  {
    out.x = matrix[0][0] * in.x
          + matrix[0][1] * in.y
          + matrix[0][2] * in.z
          + matrix[0][3] * in.w;
    out.y = matrix[1][0] * in.x
          + matrix[1][1] * in.y
          + matrix[1][2] * in.z
          + matrix[1][3] * in.w;
    out.z = matrix[2][0] * in.x
          + matrix[2][1] * in.y
          + matrix[2][2] * in.z
          + matrix[2][3] * in.w;
    out.w = matrix[3][0] * in.x
          + matrix[3][1] * in.y
          + matrix[3][2] * in.z
          + matrix[3][3] * in.w;
  };
  
  void apply(Vertex &in)
  {
    float x,y,z,w;
    
    x = matrix[0][0] * in.x
      + matrix[0][1] * in.y
      + matrix[0][2] * in.z
      + matrix[0][3] * in.w;
    y = matrix[1][0] * in.x
      + matrix[1][1] * in.y
      + matrix[1][2] * in.z
      + matrix[1][3] * in.w;
    z = matrix[2][0] * in.x
      + matrix[2][1] * in.y
      + matrix[2][2] * in.z
      + matrix[2][3] * in.w;
    w = matrix[3][0] * in.x
      + matrix[3][1] * in.y
      + matrix[3][2] * in.z
      + matrix[3][3] * in.w;

    in.x = x;
    in.y = y;
    in.z = z;
    in.w = w;
  };

  void apply(Vector& in)
  {
      float x, y, z, w;

      x = matrix[0][0] * in.x
          + matrix[0][1] * in.y
          + matrix[0][2] * in.z;
      y = matrix[1][0] * in.x
          + matrix[1][1] * in.y
          + matrix[1][2] * in.z;
      z = matrix[2][0] * in.x
          + matrix[2][1] * in.y
          + matrix[2][2] * in.z;

      in.x = x;
      in.y = y;
      in.z = z;
  };

  Transform operator*(Transform &rhs)
  {
      Transform r;

      for (int x = 0; x < 4; x += 1)
      {
          for (int y = 0; y < 4; y += 1)
          {
              r.matrix[x][y] = (matrix[x][0] * rhs.matrix[0][y]) + (matrix[x][1] * rhs.matrix[1][y]) + (matrix[x][2] * rhs.matrix[2][y]) + (matrix[x][3] * rhs.matrix[3][y]);
          }
      }

      return r;
  }

  Transform inverse()
  {
      Transform r;

      r.matrix[0][0] = this->matrix[1][1] * this->matrix[2][2] * this->matrix[3][3] -
          this->matrix[1][1] * this->matrix[2][3] * this->matrix[3][2] -
          this->matrix[2][1] * this->matrix[1][2] * this->matrix[3][3] +
          this->matrix[2][1] * this->matrix[1][3] * this->matrix[3][2] +
          this->matrix[3][1] * this->matrix[1][2] * this->matrix[2][3] -
          this->matrix[3][1] * this->matrix[1][3] * this->matrix[2][2];

      r.matrix[1][0] = -this->matrix[1][0] * this->matrix[2][2] * this->matrix[3][3] +
          this->matrix[1][0] * this->matrix[2][3] * this->matrix[3][2] +
          this->matrix[2][0] * this->matrix[1][2] * this->matrix[3][3] -
          this->matrix[2][0] * this->matrix[1][3] * this->matrix[3][2] -
          this->matrix[3][0] * this->matrix[1][2] * this->matrix[2][3] +
          this->matrix[3][0] * this->matrix[1][3] * this->matrix[2][2];

      r.matrix[2][0] = this->matrix[1][0] * this->matrix[2][1] * this->matrix[3][3] -
          this->matrix[1][0] * this->matrix[2][3] * this->matrix[3][1] -
          this->matrix[2][0] * this->matrix[1][1] * this->matrix[3][3] +
          this->matrix[2][0] * this->matrix[1][3] * this->matrix[3][1] +
          this->matrix[3][0] * this->matrix[1][1] * this->matrix[2][3] -
          this->matrix[3][0] * this->matrix[1][3] * this->matrix[2][1];

      r.matrix[3][0] = -this->matrix[1][0] * this->matrix[2][1] * this->matrix[3][2] +
          this->matrix[1][0] * this->matrix[2][2] * this->matrix[3][1] +
          this->matrix[2][0] * this->matrix[1][1] * this->matrix[3][2] -
          this->matrix[2][0] * this->matrix[1][2] * this->matrix[3][1] -
          this->matrix[3][0] * this->matrix[1][1] * this->matrix[2][2] +
          this->matrix[3][0] * this->matrix[1][2] * this->matrix[2][1];

      r.matrix[0][1] = -this->matrix[0][1] * this->matrix[2][2] * this->matrix[3][3] +
          this->matrix[0][1] * this->matrix[2][3] * this->matrix[3][2] +
          this->matrix[2][1] * this->matrix[0][2] * this->matrix[3][3] -
          this->matrix[2][1] * this->matrix[0][3] * this->matrix[3][2] -
          this->matrix[3][1] * this->matrix[0][2] * this->matrix[2][3] +
          this->matrix[3][1] * this->matrix[0][3] * this->matrix[2][2];

      r.matrix[1][1] = this->matrix[0][0] * this->matrix[2][2] * this->matrix[3][3] -
          this->matrix[0][0] * this->matrix[2][3] * this->matrix[3][2] -
          this->matrix[2][0] * this->matrix[0][2] * this->matrix[3][3] +
          this->matrix[2][0] * this->matrix[0][3] * this->matrix[3][2] +
          this->matrix[3][0] * this->matrix[0][2] * this->matrix[2][3] -
          this->matrix[3][0] * this->matrix[0][3] * this->matrix[2][2];

      r.matrix[2][1] = -this->matrix[0][0] * this->matrix[2][1] * this->matrix[3][3] +
          this->matrix[0][0] * this->matrix[2][3] * this->matrix[3][1] +
          this->matrix[2][0] * this->matrix[0][1] * this->matrix[3][3] -
          this->matrix[2][0] * this->matrix[0][3] * this->matrix[3][1] -
          this->matrix[3][0] * this->matrix[0][1] * this->matrix[2][3] +
          this->matrix[3][0] * this->matrix[0][3] * this->matrix[2][1];

      r.matrix[3][1] = this->matrix[0][0] * this->matrix[2][1] * this->matrix[3][2] -
          this->matrix[0][0] * this->matrix[2][2] * this->matrix[3][1] -
          this->matrix[2][0] * this->matrix[0][1] * this->matrix[3][2] +
          this->matrix[2][0] * this->matrix[0][2] * this->matrix[3][1] +
          this->matrix[3][0] * this->matrix[0][1] * this->matrix[2][2] -
          this->matrix[3][0] * this->matrix[0][2] * this->matrix[2][1];

      r.matrix[0][2] = this->matrix[0][1] * this->matrix[1][2] * this->matrix[3][3] -
          this->matrix[0][1] * this->matrix[1][3] * this->matrix[3][2] -
          this->matrix[1][1] * this->matrix[0][2] * this->matrix[3][3] +
          this->matrix[1][1] * this->matrix[0][3] * this->matrix[3][2] +
          this->matrix[3][1] * this->matrix[0][2] * this->matrix[1][3] -
          this->matrix[3][1] * this->matrix[0][3] * this->matrix[1][2];

      r.matrix[1][2] = -this->matrix[0][0] * this->matrix[1][2] * this->matrix[3][3] +
          this->matrix[0][0] * this->matrix[1][3] * this->matrix[3][2] +
          this->matrix[1][0] * this->matrix[0][2] * this->matrix[3][3] -
          this->matrix[1][0] * this->matrix[0][3] * this->matrix[3][2] -
          this->matrix[3][0] * this->matrix[0][2] * this->matrix[1][3] +
          this->matrix[3][0] * this->matrix[0][3] * this->matrix[1][2];

      r.matrix[2][2] = this->matrix[0][0] * this->matrix[1][1] * this->matrix[3][3] -
          this->matrix[0][0] * this->matrix[1][3] * this->matrix[3][1] -
          this->matrix[1][0] * this->matrix[0][1] * this->matrix[3][3] +
          this->matrix[1][0] * this->matrix[0][3] * this->matrix[3][1] +
          this->matrix[3][0] * this->matrix[0][1] * this->matrix[1][3] -
          this->matrix[3][0] * this->matrix[0][3] * this->matrix[1][1];

      r.matrix[3][2] = -this->matrix[0][0] * this->matrix[1][1] * this->matrix[3][2] +
          this->matrix[0][0] * this->matrix[1][2] * this->matrix[3][1] +
          this->matrix[1][0] * this->matrix[0][1] * this->matrix[3][2] -
          this->matrix[1][0] * this->matrix[0][2] * this->matrix[3][1] -
          this->matrix[3][0] * this->matrix[0][1] * this->matrix[1][2] +
          this->matrix[3][0] * this->matrix[0][2] * this->matrix[1][1];

      r.matrix[0][3] = -this->matrix[0][1] * this->matrix[1][2] * this->matrix[2][3] +
          this->matrix[0][1] * this->matrix[1][3] * this->matrix[2][2] +
          this->matrix[1][1] * this->matrix[0][2] * this->matrix[2][3] -
          this->matrix[1][1] * this->matrix[0][3] * this->matrix[2][2] -
          this->matrix[2][1] * this->matrix[0][2] * this->matrix[1][3] +
          this->matrix[2][1] * this->matrix[0][3] * this->matrix[1][2];

      r.matrix[1][3] = this->matrix[0][0] * this->matrix[1][2] * this->matrix[2][3] -
          this->matrix[0][0] * this->matrix[1][3] * this->matrix[2][2] -
          this->matrix[1][0] * this->matrix[0][2] * this->matrix[2][3] +
          this->matrix[1][0] * this->matrix[0][3] * this->matrix[2][2] +
          this->matrix[2][0] * this->matrix[0][2] * this->matrix[1][3] -
          this->matrix[2][0] * this->matrix[0][3] * this->matrix[1][2];

      r.matrix[2][3] = -this->matrix[0][0] * this->matrix[1][1] * this->matrix[2][3] +
          this->matrix[0][0] * this->matrix[1][3] * this->matrix[2][1] +
          this->matrix[1][0] * this->matrix[0][1] * this->matrix[2][3] -
          this->matrix[1][0] * this->matrix[0][3] * this->matrix[2][1] -
          this->matrix[2][0] * this->matrix[0][1] * this->matrix[1][3] +
          this->matrix[2][0] * this->matrix[0][3] * this->matrix[1][1];

      r.matrix[3][3] = this->matrix[0][0] * this->matrix[1][1] * this->matrix[2][2] -
          this->matrix[0][0] * this->matrix[1][2] * this->matrix[2][1] -
          this->matrix[1][0] * this->matrix[0][1] * this->matrix[2][2] +
          this->matrix[1][0] * this->matrix[0][2] * this->matrix[2][1] +
          this->matrix[2][0] * this->matrix[0][1] * this->matrix[1][2] -
          this->matrix[2][0] * this->matrix[0][2] * this->matrix[1][1];

      float det = this->matrix[0][0] * r.matrix[0][0] + this->matrix[0][1] * r.matrix[1][0] + this->matrix[0][2] * r.matrix[2][0] + this->matrix[0][3] * r.matrix[3][0];

      if (det == 0) return r;

      det = 1.0 / det;

      for (int i = 0; i < 4; i++)
      {
          for (int j = 0; j < 4; j++)
          {
              r.matrix[i][j] = r.matrix[i][j] * det;
          }
      }

      return r;
  }

  Transform transpose()
  {
      Transform r;

      for (int x = 0; x < 4; x += 1)
      {
          for (int y = 0; y < 4; y += 1)
          {
              r.matrix[x][y] = matrix[y][x];
          }
      }
      return r;
  }
};
