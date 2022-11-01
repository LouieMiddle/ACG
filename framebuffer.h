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

// The FrameBuffer class creates a framebuffer with rgba and depth and can write them to a ppm file.

#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

typedef struct {
    float red;
    float green;
    float blue;
    float depth;
} Pixel;

class FrameBuffer {
    Pixel *framebuffer;
public:
    int width;
    int height;

    FrameBuffer(int w, int h);

    int plotPixel(int x, int y, float red, float green, float blue);

    int plotDepth(int x, int y, float depth);

    int getPixel(int x, int y, float &red, float &green, float &blue);

    int getDepth(int x, int y, float &depth);

    int writeRGBFile(char *filename);

    int writeDepthFile(char *filename);
};

#endif
