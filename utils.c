#include <math.h>
#include "xterm2ansi.h"

static const Color ansi_colors[] = {
    {0x00, 0x00, 0x00, 30}, /* black */
    {0xCD, 0x00, 0x00, 31}, /* red */
    {0x00, 0xCD, 0x00, 32}, /* green */
    {0xCD, 0xCD, 0x00, 33}, /* yellow */
    {0x00, 0x00, 0xEE, 34}, /* blue */
    {0xCD, 0x00, 0xCD, 35}, /* magenta */
    {0x00, 0xCD, 0xCD, 36}, /* cyan */
    {0xE5, 0xE5, 0xE5, 37}, /* white */
    {0x7F, 0x7F, 0x7F, 90}, /* bright black */
    {0xFF, 0x00, 0x00, 91}, /* bright red */
    {0x00, 0xFF, 0x00, 92}, /* bright green */
    {0xFF, 0xFF, 0x00, 93}, /* bright yellow */
    {0x5C, 0x5C, 0xFF, 94}, /* bright blue */
    {0xFF, 0x00, 0xFF, 95}, /* bright magenta */
    {0x00, 0xFF, 0xFF, 96}, /* bright cyan */
    {0xFF, 0xFF, 0xFF, 97}  /* bright white */
};

/* from xterm's 256colres.pl */
void xterm_to_rgb(int color, int *r, int *g, int *b) {
    if (color < 16) {
        *r = ansi_colors[color].r;
        *g = ansi_colors[color].g;
        *b = ansi_colors[color].b;
    } else if (color < 232) {
        color -= 16;
        int temp_r = (color / 36);
        int temp_g = ((color % 36) / 6);
        int temp_b = (color % 6);
        *r = temp_r ? temp_r * 40 + 55 : 0;
        *g = temp_g ? temp_g * 40 + 55 : 0;
        *b = temp_b ? temp_b * 40 + 55 : 0;
    } else {
        color -= 232;
        *r = *g = *b = (color * 10) + 8;
    }
}

int xterm_to_ansi(int color) {
    int r, g, b;
    xterm_to_rgb(color, &r, &g, &b);

    int nearest_color = 0;
    double min_distance = MAX_DISTANCE;

    for (int i = 0; i < 16; i++) {
        Color curr = ansi_colors[i];
        double distance = color_distance(r, g, b, curr.r, curr.g, curr.b);
        if (distance < min_distance) {
            min_distance = distance;
            nearest_color = curr.code;
        }
    }

    return nearest_color;
}

/* euclidean distance */
double color_distance(int r1, int g1, int b1, int r2, int g2, int b2) {
    return sqrt(pow(r2 - r1, 2) + pow(b2 - b1, 2) + pow(g2 - g1, 2));
}
