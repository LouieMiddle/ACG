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

/* This is the top level for the program you need to create for lab three and four.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// these are core raytracing classes
#include "framebuffer.h"
#include "scene.h"

// classes that contain our objects to be rendered, all derived from Object
#include "polymesh_object.h"
#include "sphere_object.h"
#include "plane_object.h"

// classes that contain our lights, all derived from Light
#include "directional_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "phong_material.h"
#include "falsecolour_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"

using namespace std;

// you will find it useful during development/debugging to create multiple functions that fill out the scene.
void build_scene(Scene &scene) {
    // The following transform allows 4D homogeneous coordinates to be transformed.It moves the supplied teapot model to somewhere visible.
    Transform *transform = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, -2.7f,
                                         0.0f, 1.0f, 0.0f, 5.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);

    //  Read in the teapot model.
    PolyMesh *teapot = new PolyMesh("teapot_smaller.ply", false);
    teapot->apply_transform(*transform);

    Sphere *sphere = new Sphere(Vertex(0.0f, 0.0f, 1.0f), 0.4f);

    Plane *plane = new Plane(0, 0, 1, -5);

    DirectionalLight *dl1 = new DirectionalLight(Vector(1.0f, -1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 0.0f));

    scene.add_light(dl1);

    FalseColour* falseColour = new FalseColour();
    Phong* red = new Phong(Colour(0.1f, 0.0f, 0.0f), Colour(0.4f, 0.0f, 0.0f), Colour(0.2f, 0.2f, 0.2f), 40.f);
    Phong* green = new Phong(Colour(0.0f, 0.1f, 0.0f), Colour(0.0f, 0.4f, 0.0f), Colour(0.2f, 0.2f, 0.2f), 40.f);
    Phong* blue = new Phong(Colour(0.0f, 0.0f, 0.1f), Colour(0.0f, 0.0f, 0.4f), Colour(0.2f, 0.2f, 0.2f), 40.f);

    plane->set_material(blue);

    scene.add_object(plane);

    teapot->set_material(red);

    scene.add_object(teapot);

    sphere->set_material(green);

    scene.add_object(sphere);
}


//// This is the entry point function to the program.
int main(int argc, char *argv[]) {
    int width = 512;
    int height = 512;
    FrameBuffer *fb = new FrameBuffer(width, height);

    Scene scene;
    build_scene(scene);

    Vertex position = *new Vertex(0.0f, 0.0f, 0.0f);
    Vector look_at = *new Vector(0.0f, 0.0f, 1.0f);
    Vector up = *new Vector(0.0f, 1.0f, 0.0f);
    Camera *camera = new FullCamera(0.5f, position, look_at, up);

    // Camera generates rays for each pixel in the framebuffer and records colour + depth.
    camera->render(scene, *fb);

    // Output the framebuffer colour and depth as two images
    fb->writeRGBFile((char *) "test.ppm");
    fb->writeDepthFile((char *) "depth.ppm");

    cerr << "\nDone.\n" << flush;

    // a debug check to make sure we don't leak hits.
    cerr << "Hit Pool " << Hit::pool_size << " Allocated " << Hit::allocated << "\n" << flush;
    return 0;
}
