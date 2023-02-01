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

#include "full_camera.h"

FullCamera::FullCamera() {
    fov = 0.5;
    position = Vertex(0.0f, 0.0f, 0.0f, 1.0f);
    lookat = Vector(0.0f, 0.0f, 1.0f);
    up = Vector(0.0f, 1.0f, 0.0f);
    right = Vector(1.0f, 0.0f, 0.0f);
}

FullCamera::FullCamera(float f, Vertex &p_position, Vector &p_lookat, Vector &p_up) {
    fov = f;
    position = p_position;
    lookat = p_lookat;
    up = p_up;
    up.cross(lookat, right);
    lookat.cross(right, up);
    up.normalise();
    right.normalise();
    lookat.normalise();
}

void FullCamera::get_ray_pixel(int p_x, int p_y, Ray &ray) {
    float fx = ((float) p_x + 0.5f) / (float) width;
    float fy = ((float) p_y + 0.5f) / (float) height;

    ray.position = position;
    ray.direction = ((fx - 0.5f) * right) + ((0.5f - fy) * up) + (fov * lookat);
    ray.direction.normalise();
}

void FullCamera::render(Environment &env, FrameBuffer &fb) {
    width = fb.width;
    height = fb.height;

    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            Ray ray;

            get_ray_pixel(x, y, ray);

            Colour colour = Colour(0.0f, 0.0f, 0.0f);
            float depth = 0.0f;

            env.raytrace(ray, 5, colour, depth);

            fb.plotPixel(x, y, colour.r, colour.g, colour.b);
            fb.plotDepth(x, y, depth);
        }

        cerr << "#" << flush;
    }
}
