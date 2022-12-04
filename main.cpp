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
#include <string>

// these are core raytracing classes
#include "framebuffer.h"
#include "scene.h"

// classes that contain our objects to be rendered, all derived from Object
#include "polymesh_object.h"
#include "sphere_object.h"
#include "plane_object.h"
#include "quadratic_object.h"
#include "csg_object.h"

// classes that contain our lights, all derived from Light
#include "directional_light.h"
#include "point_light.h"

// classes that contain the materials applied to an object, all derived from Material
#include "phong_material.h"
#include "falsecolour_material.h"
#include "global_material.h"
#include "compound_material.h"

//classes that contain cameras, all derived from Camera
#include "simple_camera.h"
#include "full_camera.h"

using namespace std;

// you will find it useful during development/debugging to create multiple functions that fill out the scene.
void build_scene(Scene &scene) {
    // The following move_teapot allows 4D homogeneous coordinates to be transformed. It moves the supplied teapot model to somewhere visible.
    Transform *move_teapot = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
                                           0.0f, 0.0f, 1.0f, -1.0f,
                                           0.0f, 1.0f, 0.0f, 10.0f,
                                           0.0f, 0.0f, 0.0f, 1.0f);

    Transform *rotate_180 = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
                                          0.0f, -1.0f, 0.0f, 0.0f,
                                          0.0f, 0.0f, -1.0f, 0.0f,
                                          0.0f, 0.0f, 0.0f, 1.0f);

    //  Read in the teapot model.
    PolyMesh *teapot = new PolyMesh("teapot_smaller.ply", true);
    teapot->apply_transform(*move_teapot);

    Sphere *refractive_sphere = new Sphere(Vertex(0.0f, -0.2f, 1.0f), 0.3f);
    Sphere *reflective_sphere = new Sphere(Vertex(0.5f, 0.0f, 0.0f), 0.3f);
    Sphere *reflective_sphere2 = new Sphere(Vertex(-0.5f, 0.0f, 0.0f), 0.3f);

    Plane *back_white_plane = new Plane(0.0f, 0.0f, 1.0f, -2.0f);
    Plane *top_blue_plane =new Plane(0.0f, 1.0f, 0.0f, -2.0f);
    Plane *bottom_white_plane =new Plane(0.0f, 1.0f, 0.0f, 2.0f);
    Plane *right_green_plane = new Plane(1.0f, 0.0f, 0.0f, -2.0f);
    Plane *left_red_plane = new Plane(1.0f, 0.0f, 0.0f, 2.0f);

    Quadratic *quadratic_sphere = new Quadratic(2.0f, 0.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 5.0f, 0.0f, -1.0f);
    // Hyperboloid of one sheet with axis of symmetry at the x-axis
    Quadratic *quadratic_hyperboloid = new Quadratic(-3.0f, 0.0f, 0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 6.0f, 0.0f, -1.0f);

    DirectionalLight *dl = new DirectionalLight(Vector(1.0f, -1.0f, 1.0f), Colour(1.0f, 1.0f, 1.0f, 0.0f));
    PointLight *pl = new PointLight(Vertex(0.0f, 1.0f, 0.0f), Colour(10.0f, 10.0f, 10.0f, 0.0f));

//    scene.add_light(dl);
    scene.add_light(pl);

    Phong *white = new Phong(Colour(0.2f, 0.2f, 0.2f), Colour(0.4f, 0.4f, 0.4f), Colour(0.4f, 0.4f, 0.4f), 40.f);
    Phong *red = new Phong(Colour(0.2f, 0.0f, 0.0f), Colour(0.4f, 0.0f, 0.0f), Colour(0.4f, 0.4f, 0.4f), 40.f);
    Phong *green = new Phong(Colour(0.0f, 0.2f, 0.0f), Colour(0.0f, 0.4f, 0.0f), Colour(0.4f, 0.4f, 0.4f), 40.f);
    Phong *blue = new Phong(Colour(0.0f, 0.0f, 0.2f), Colour(0.0f, 0.0f, 0.4f), Colour(0.4f, 0.4f, 0.4f), 40.f);
    Phong *yellow = new Phong(Colour(0.2f, 0.2f, 0.0f), Colour(0.4f, 0.4f, 0.0f), Colour(0.4f, 0.4f, 0.4f), 40.f);
    Phong *purple = new Phong(Colour(0.2f, 0.0f, 0.2f), Colour(0.4f, 0.0f, 0.4f), Colour(0.4f, 0.4f, 0.4f), 40.f);
    Phong *light_blue = new Phong(Colour(0.0f, 0.2f, 0.2f), Colour(0.0f, 0.4f, 0.4f), Colour(0.4f, 0.4f, 0.4f), 40.f);
    GlobalMaterial *refractive_glass = new GlobalMaterial(&scene, 1.52f, true);
    GlobalMaterial *reflective_glass = new GlobalMaterial(&scene, 1.52f, false);
    CompoundMaterial *reflective_white_glass = new CompoundMaterial(2);
    reflective_white_glass->include_material(white);
    reflective_white_glass->include_material(reflective_glass);

    Sphere *test_red_sphere = new Sphere(Vertex(0.5f, 0.0f, 0.0f), 0.3f);
    test_red_sphere->set_material(red);
//    scene.add_object(test_red_sphere);

    teapot->set_material(red);
//    scene.add_object(teapot);

    refractive_sphere->set_material(refractive_glass);
    reflective_sphere->set_material(reflective_white_glass);
    reflective_sphere2->set_material(reflective_white_glass);
//    scene.add_object(refractive_sphere);
    scene.add_object(reflective_sphere);
    scene.add_object(reflective_sphere2);

    back_white_plane->set_material(white);
    top_blue_plane->set_material(blue);
    bottom_white_plane->set_material(white);
    right_green_plane->set_material(green);
    left_red_plane->set_material(red);
    scene.add_object(back_white_plane);
    scene.add_object(top_blue_plane);
    scene.add_object(bottom_white_plane);
    scene.add_object(right_green_plane);
    scene.add_object(left_red_plane);

    quadratic_sphere->set_material(red);
    quadratic_hyperboloid->set_material(red);
//    scene.add_object(quadratic_sphere);
//    scene.add_object(quadratic_hyperboloid);

    Sphere *red_sphere = new Sphere(Vertex(-0.2f, 0.0f, 1.0f), 0.5f);
    Sphere *blue_sphere = new Sphere(Vertex(0.0f, 0.0f, 1.0f), 0.5f);
    Sphere *green_sphere = new Sphere(Vertex(0.2f, 0.0f, 1.0f), 0.5f);
    blue_sphere->set_material(blue);
    red_sphere->set_material(red);
    green_sphere->set_material(green);
    CSG *blue_diff_red = new CSG(CSG_DIFF, blue_sphere, red_sphere);
//    blue_diff_red->apply_transform(*rotate_180);
//    scene.add_object(blue_diff_red);
    CSG *blue_diff_red_diff_green = new CSG(CSG_DIFF, blue_diff_red, green_sphere);
//    scene.add_object(blue_diff_red_diff_green);

    Sphere *yellow_sphere = new Sphere(Vertex(-0.2f, 0.25f, 1.0f), 0.5f);
    Sphere *purple_sphere = new Sphere(Vertex(0.0f, 0.25f, 1.0f), 0.5f);
    Sphere *light_blue_sphere = new Sphere(Vertex(0.2f, 0.25f, 1.0f), 0.5f);
    yellow_sphere->set_material(yellow);
    purple_sphere->set_material(purple);
    light_blue_sphere->set_material(light_blue);
    CSG *purple_diff_yellow = new CSG(CSG_DIFF, purple_sphere, yellow_sphere);
    CSG *purple_diff_yellow_diff_light_blue = new CSG(CSG_DIFF, purple_diff_yellow, light_blue_sphere);
    CSG *yellow_diff_purple_union_blue_diff_red_diff_green = new CSG(CSG_UNION, purple_diff_yellow_diff_light_blue, blue_diff_red_diff_green);
//    scene.add_object(yellow_diff_purple_union_blue_diff_red_diff_green);
}


// This is the entry point function to the program.
int main(int argc, char *argv[]) {
    int width = 512;
    int height = 512;
    // Create a framebuffer
    FrameBuffer *fb = new FrameBuffer(width, height);

    // Create a scene
    Scene scene;

    // Setup the scene
    build_scene(scene);

    // Declare a camera
    Vertex position = *new Vertex(0.0f, 0.0f, -1.0f);
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
