///****************************************************************************
// *
// * krt - Ken's Raytracer - Coursework Edition. (C) Copyright 1993-2022.
// *
// * I've put a lot of time and effort into this code. For the last decade
// * it's been used to introduce hundreds of students at multiple universities
// * to raytracing. It forms the basis of your coursework but you are free
// * to continue using/developing forever more. However, I ask that you don't
// * share the code or your derivitive versions publicly. In order to continue
// * to be used for coursework and in particular assessment it's important that
// * versions containing solutions are not searchable on the web or easy to
// * download.
// *
// * If you want to show off your programming ability, instead of releasing
// * the code, consider generating an incredible image and explaining how you
// * produced it.
// */
//
///* This is the entry point function for the program you need to create for lab one.
// * You should not need to modify this code.
// * It creates a framebuffer, calls the drawing function and then outputs the framebuffer as a ppm file.
// *
//
// * Required c++ files are: main_lab1.cpp framebuffer.cpp linedrawer.cpp
// *
// * This will produce an image file called test.ppm. You can convert this a png file for viewing using
// *
// * pnmtopng test.ppm > test.png
// *
// * You are expected to replace the line drawing function with one that only uses integers and no floats.
// */
//
//#include <math.h>
//
//#include "framebuffer.h"
//#include "linedrawer.h"
//
//int main(int argc, char *argv[])
//{
//  // Create a framebuffer
//  FrameBuffer *fb = new FrameBuffer(512,512);
//
//  // Generate 64 radial lines around the center of the image.
//  for (float i = 0; i < M_PI * 2; i += M_PI / 32.0f)
//  {
//    // Generate a simple 2D vector
//    float x = cos(i);
//    float y = sin(i);
//
//    // draw a line
//    draw_line(fb, 256 + (int)(48.0f*x), 256 + (int)(48.0f*y), 256 + (int)(240.0f*x), 256 + (int)(240.0f*y));
//  }
//
//  // output the framebuffer
//  fb->writeRGBFile((char *)"test.ppm");
//
//  // we're done.
//  return 0;
//}
