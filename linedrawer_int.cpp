/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

/* This is the code you will need to replace for Lab 1.
 *
 * It contains two simple implementations that loop over the longest axis adding the gradient to the position on the other axis at each step.
 * The objective is for you to remove the use of floating point variables and work entirely with integers.
 * You should use Bresenhams Line Algorithm to achieve this.
 */

// This file replaces line_drawer.cpp an includes an integer only line drawer.

#include <iostream>
#include "linedrawer.h"

int draw_x_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    int xdir = 1;
    if (sx > ex) {
        xdir = -1;
    }

    int ydir = 1;
    if (sy > ey) {
        ydir = -1;
    }

    int x = sx;
    int y = sy;
    int dx = ex - sx;
    if (dx < 0) dx = -dx;

    int dy = ey - sy;
    if (dy < 0) dy = -dy;

    int fx = dx / 2; // start in the middle of pixel

    while (x != ex) {
        fb->plotPixel(x, y, 1.0f, 1.0f, 1.0f);

        fx = fx + dy;

        if (fx >= dx) {
            fx -= dx;
            y += ydir;
        }

        x += xdir;
    }

    return 0;
}

int draw_y_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    int xdir = 1;
    if (sx > ex) {
        xdir = -1;
    }

    int ydir = 1;
    if (sy > ey) {
        ydir = -1;
    }

    int x = sx;
    int y = sy;
    int dx = ex - sx;
    if (dx < 0) dx = -dx;

    int dy = ey - sy;
    if (dy < 0) dy = -dy;

    int fy = dy / 2; // start in the middle of pixel

    while (y != ey) {
        fb->plotPixel(x, y, 1.0f, 1.0f, 1.0f);

        fy = fy + dx;

        if (fy >= dy) {
            fy -= dy;
            x += xdir;
        }

        y += ydir;
    }

    return 0;
}

int draw_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    if ((sx == ex) && (sy == ey)) {
        return fb->plotPixel(sx, sy, 1.0f, 1.0f, 1.0f);

    } else if (((ex - sx) * (ex - sx)) >= ((ey - sy) * (ey - sy))) {
        return draw_x_line(fb, sx, sy, ex, ey);

    } else {
        return draw_y_line(fb, sx, sy, ex, ey);
    }
}
