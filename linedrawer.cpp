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

#include <iostream>
#include "linedrawer.h"

int draw_x_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    int dx = ex - sx;
    int dy = ey - sy;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int d = 2 * dx - dy;
    int x = sx;

    for (int y = sy; y <= ey; y++) {
        fb->plotPixel(x, y, 1.0f, 1.0f, 1.0f);
        if (d > 0) {
            x = x + xi;
            d = d + 2 * (dx - dy);
        } else {
            d = d + 2 * dx;
        }
    }

    return 0;
}

int draw_y_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    int dx = ex - sx;
    int dy = ey - sy;
    int yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int d = 2 * dy - dx;
    int y = sy;

    for (int x = sx; x <= ex; x++) {
        fb->plotPixel(x, y, 1.0f, 1.0f, 1.0f);
        if (d > 0) {
            y = y + yi;
            d = d + 2 * (dy - dx);
        } else {
            d = d + 2 * dy;
        }
    }

    return 0;
}

int draw_line(FrameBuffer *fb, int sx, int sy, int ex, int ey) {
    if (abs(ey - sy) < abs(ex - sx)) {
        if (sx > ex) {
            return draw_y_line(fb, ex, ey, sx, sy);
        } else {
            return draw_y_line(fb, sx, sy, ex, ey);
        }
    } else {
        if (sy > ey) {
            return draw_x_line(fb, ex, ey, sx, sy);
        } else {
            return draw_x_line(fb, sx, sy, ex, ey);
        }
    }
}
