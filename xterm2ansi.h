#ifndef XTERMTOANSI_H
#define XTERMTOANSI_H

#define MAX_ESCAPE_CODE_LENGTH 64
#define MAX_LINE_LENGTH 4096
#define MAX_DISTANCE 256

typedef struct Color {
  int r;
  int g;
  int b;
  int code;
} Color;

static const Color ansi_colors[16];

int xterm_to_ansi(int color);
int rgb_to_ansi(int r, int g, int b);
void xterm_to_rgb(int color, int *r, int *g, int *b);
double color_distance(int r1, int g1, int b1, int r2, int g2, int b2);

#endif
