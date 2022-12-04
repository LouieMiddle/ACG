#pragma once

#include "light.h"

// light where each ray is spawned at a different location within given limits

class PointLight : public Light {
public:
    Vertex position;
    Colour intensity;

    PointLight();

    PointLight(Vertex p_position, Colour p_intensity);

    bool get_direction(Vertex &surface, Vector &dir, float &dist);

    void get_intensity(Vertex &surface, Colour &intensity);
};
