#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "xterm2ansi.h"
#include "utils.h"

int get_control_sequence(char *input) {
    char *sc_pos = strchr(input, ';');
    int i = 1;
    while (sc_pos != NULL && i < 3) {
        sc_pos = strchr(sc_pos + 1, ';');
        i++;
    }
    if (sc_pos != NULL) {
        return atoi(sc_pos + 1);
    }
    return -1;
}

void handle_control_sequence(char *escape_code) {
    int control_seq = get_control_sequence(escape_code);
    if (control_seq > 0) {
        printf("\033[%dm", control_seq);
    }
}

int main(int argc, char *argv[]) {
    FILE *input;
    char line[MAX_LINE_LENGTH];


    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "Error: Could not open input file %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    } else {
        input = stdin;
    }

    while (fgets(line, MAX_LINE_LENGTH, input)) {
        for (int i = 0; i < strlen(line); i++) {
            if (line[i] == '\033' && line[i + 1] == '[') {
                char *csi_end = line + i + 2;
                char *m_pos = strchr(csi_end, 'm');

                if (m_pos != NULL) {
                    char escape_code[MAX_ESCAPE_CODE_LENGTH];
                    int ec_len = m_pos - csi_end;
                    strncpy(escape_code, m_pos - ec_len, ec_len);
                    escape_code[ec_len] = '\0';

                    char *fg_pos = strstr(escape_code, SELECT_FG_STR);
                    char *bg_pos = strstr(escape_code, SELECT_BG_STR);
                    int is_fg = fg_pos != NULL;
                    int is_bg = bg_pos != NULL;
                    int control_seq = 0;

                    int diff;
                    if (fg_pos != NULL) {
                        diff = fg_pos - escape_code;
                    } else if (bg_pos != NULL) {
                        diff = bg_pos - escape_code;
                    } else {
                        diff = 0;
                    }
                    if (diff > 0) {
                        control_seq = atoi(escape_code);
                        printf("\033[%dm", control_seq);
                    }

                    if (is_fg && !is_bg) {
                        int color = atoi(escape_code + FG_STR_LEN + diff);
                        int nearest_color = xterm_to_ansi(color);
                        if (!control_seq) {
                            handle_control_sequence(escape_code);
                        }
                        printf("\033[%dm", nearest_color);
                    } else if (is_bg && !is_fg) {
                        int color = atoi(escape_code + BG_STR_LEN + diff);
                        int nearest_color = xterm_to_ansi(color);
                        if (!control_seq) {
                            handle_control_sequence(escape_code);
                        }
                        printf("\033[%dm", nearest_color + BG_DIFF);
                    } else if (is_fg && is_bg) {
                        int fg_color = atoi(fg_pos + FG_STR_LEN + diff);
                        int fg_nearest_color = xterm_to_ansi(fg_color);

                        int bg_color = atoi(bg_pos + BG_STR_LEN + diff);
                        int bg_nearest_color = xterm_to_ansi(bg_color);

                        printf("\033[%d;%dm", fg_nearest_color, \
                                bg_nearest_color + BG_DIFF);
                    } else if (strstr(escape_code, "0m") != NULL) {
                        printf("\033[0m");
                    } else {
                        // unknown escape code, output as is
                        printf("\033[%sm", escape_code);
                    }
                    i = m_pos - line;
                }
            } else {
                fputc(line[i], stdout);
            }
        }
    }

    fclose(input);

    return EXIT_SUCCESS;
}

