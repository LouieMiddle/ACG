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

#include "simple_camera.h"

SimpleCamera::SimpleCamera() {
    fov = 0.5;
}

SimpleCamera::SimpleCamera(float p_f) {
    fov = p_f;
}

void SimpleCamera::get_ray_pixel(int p_x, int p_y, Ray &p_ray) {
    float fx = ((float) p_x + 0.5f) / (float) width;
    float fy = ((float) p_y + 0.5f) / (float) height;

    p_ray.position.x = 0.0f;
    p_ray.position.y = 0.0f;
    p_ray.position.z = 0.0f;
    p_ray.position.w = 1.0f;
    p_ray.direction.x = (fx - 0.5f);
    p_ray.direction.y = (0.5f - fy);
    // Distance between the camera position and the plane
    p_ray.direction.z = fov;
    p_ray.direction.normalise();
}

void SimpleCamera::render(Environment &env, FrameBuffer &fb) {
    width = fb.width;
    height = fb.height;

    for (int y = 0; y < height; y += 1) {
        for (int x = 0; x < width; x += 1) {
            Ray ray;

            get_ray_pixel(x, y, ray);

            Colour colour;
            float depth;

            env.raytrace(ray, 5, colour, depth);

            fb.plotPixel(x, y, colour.r, colour.g, colour.b);
            fb.plotDepth(x, y, depth);
        }

        cerr << "#" << flush;
    }
}