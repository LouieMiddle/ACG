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


/* This is the entry point function for the program you need to create for lab two.
 * You should not need to modify this code.
 * It creates a framebuffer, loads a triangle mesh object,
 * calls the drawing function to render the object and then outputs the framebuffer as a ppm file.
 *
 * Required c++ files are: main_lab2.cpp framebuffer.cpp linedrawer.cpp polymesh_object.cpp hit.cpp
 *
 * This will produce an image file called test.ppm. You can convert this a png file for viewing using
 *
 * pnmtopng test.ppm > test.png
 *
 * You are expected to fill in the missing code in polymesh.cpp.
 */

#include "framebuffer.h"
#include "linedrawer.h"
#include "polymesh_object.h"
#include "transform.h"

//int main(int argc, char *argv[]) {
//    // Create a framebuffer
//    FrameBuffer *fb = new FrameBuffer(1024, 1024);
//
//    // The following transform allows 4D homogeneous coordinates to be transformed. It moves the supplied teapot model to somewhere visible.
//    Transform *transform = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
//                                         0.0f, 0.0f, 1.0f, -2.7f,
//                                         0.0f, 1.0f, 0.0f, 5.0f,
//                                         0.0f, 0.0f, 0.0f, 1.0f);
//
//    // Read in the teapot model.
//    PolyMesh *pm = new PolyMesh((char *) "teapot_smaller.ply", false);
//    pm->apply_transform(*transform);
//
//    // For each triangle in the model
//    for (int i = 0; i < pm->triangle_count; i += 1) {
//        // The following lines project the point onto the 2D image from 3D space.
//        float x0 = (pm->vertex[pm->triangle[i][0]].x / pm->vertex[pm->triangle[i][0]].z) * 700.0 + 512.0;
//        float y0 = (pm->vertex[pm->triangle[i][0]].y / pm->vertex[pm->triangle[i][0]].z) * -700.0 + 256.0;
//        float x1 = (pm->vertex[pm->triangle[i][1]].x / pm->vertex[pm->triangle[i][1]].z) * 700.0 + 512.0;
//        float y1 = (pm->vertex[pm->triangle[i][1]].y / pm->vertex[pm->triangle[i][1]].z) * -700.0 + 256.0;
//        float x2 = (pm->vertex[pm->triangle[i][2]].x / pm->vertex[pm->triangle[i][2]].z) * 700.0 + 512.0;
//        float y2 = (pm->vertex[pm->triangle[i][2]].y / pm->vertex[pm->triangle[i][2]].z) * -700.0 + 256.0;
//
//        // then draw the three edges.
//        draw_line(fb, (int) x0, (int) y0, (int) x1, (int) y1);
//        draw_line(fb, (int) x1, (int) y1, (int) x2, (int) y2);
//        draw_line(fb, (int) x2, (int) y2, (int) x0, (int) y0);
//        printf(".");
//    }
//
//    // Output the framebuffer.
//    fb->writeRGBFile((char *) "lm2166_lab2.ppm");
//
//    return 0;
//}
