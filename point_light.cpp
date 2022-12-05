#include "point_light.h"

using namespace std;

PointLight::PointLight() {
    Light();
}

PointLight::PointLight(Vertex p_position, Colour p_intensity) {
    Light();

    position = p_position;
    intensity = p_intensity;
}

bool PointLight::get_direction(Vertex &surface, Vector &dir, float &dist) {
    dir = surface - position;
    dir.normalise();
    dist = dir.length();
    return true;
}

void PointLight::get_intensity(Vertex &surface, Colour &level) {
    float r2 = (surface - position).len_sqr();
    level = intensity * (1 / (4 * (float) M_PI * r2));
}