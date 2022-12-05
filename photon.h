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

#pragma once

#include "ray.h"
#include "colour.h"

enum PhotonMode {
    PHOTON_DIRECT = 0,
    PHOTON_INDIRECT = 1,
    PHOTON_SHADOW = 2
};

class Photon {
public:
    Ray path;
    Colour intensity;
    PhotonMode mode;

    Photon(Ray p_path, Colour p_intensity, PhotonMode p_mode);
};

