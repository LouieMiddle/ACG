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

FullCamera::FullCamera(float f, Vertex &p_position, Vector &p_look_at, Vector &p_up) {
    fov = f;
    position = p_position;
    look_at = p_look_at;

    w = position - look_at;
    w.normalise();

    p_up.cross(w, up);
    up.normalise();

    w.cross(up, right);
}

void FullCamera::get_ray_pixel(int p_x, int p_y, Ray &ray) {
    ray.position = position;

    float fx = ((float) p_x + 0.5f) / (float) width;
    float fy = ((float) p_y + 0.5f) / (float) height;
    float xv = (fx - 0.5f);
    float yv = (0.5f - fy);

    ray.direction = xv * up + yv * right - fov * w;
    ray.direction.normalise();
}

void FullCamera::render(Environment &env, FrameBuffer &fb) {
    width = fb.width;
    height = fb.height;

    // y = row, x = column
    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            Ray ray;

            get_ray_pixel(x, y, ray);

            Colour colour = Colour(0.0f, 0.0f, 0.0f, 0.0f);
            float depth = 0.0f;

            env.raytrace(ray, 2, colour, depth);

            fb.plotPixel(x, y, colour.r, colour.g, colour.b);
            fb.plotDepth(x, y, depth);
        }

        cerr << "#" << flush;
    }
}
