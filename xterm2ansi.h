#ifndef XTERMTOANSI_H
#define XTERMTOANSI_H

#define MAX_ESCAPE_CODE_LENGTH 16
#define MAX_LINE_LENGTH 4096
#define MAX_DISTANCE 256

#define SELECT_FG_STR "38;5;"
#define SELECT_BG_STR "48;5;"
#define FG_STR_LEN 5
#define BG_STR_LEN 5
#define BG_DIFF 10


extern const char *xterm_hex_colors[];

typedef struct Color {
  int r;
  int g;
  int b;
  int code;
} Color;

int get_control_sequence(char *input);
void handle_control_sequence(char *escape_code);

#endif
