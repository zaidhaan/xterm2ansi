#ifndef XTERMTOANSI_H
#define XTERMTOANSI_H

#define MAX_ESCAPE_CODE_LENGTH 16
#define MAX_LINE_LENGTH 4096
#define MAX_DISTANCE 256

typedef struct Color {
  int r;
  int g;
  int b;
  int code;
} Color;

static const Color ansi_colors[16];

int get_control_sequence(char *input);
void handle_control_sequence(char *escape_code);

int xterm_to_ansi(int color);
void xterm_to_rgb(int color, int *r, int *g, int *b);
double color_distance(int r1, int g1, int b1, int r2, int g2, int b2);

#endif
