#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "xterm2ansi.h"

#define CSI_MAX_ARGS 16
#define CSI_MAX_LEN  64

typedef struct {
    int args [CSI_MAX_ARGS];
    int num_args;
    char buffer[CSI_MAX_LEN];
    int len;
} CSI_code;

enum state {
    STATE_NORMAL,
    STATE_ESCAPE,
    STATE_CSI,
};

void csi_conv_xterm_ansi(CSI_code csi_in, CSI_code *csi_out) {
    int color;
    int diff = 0;

    for (int i = 0; i < csi_in.num_args; i++) {
        switch (csi_in.args[i]) {
            case 48: /* background */
                diff = 10;
            case 38: /* foreground */
                if (csi_in.args[i + 1] == 5) {
                    color = xterm_to_ansi(csi_in.args[i + 2]);
                    csi_out->args[csi_out->num_args++] = color + diff;
                    i += 2;
                } else if (csi_in.args[i + 1] == 2) {
                    color = rgb_to_ansi(csi_in.args[i + 2],
                                        csi_in.args[i + 3],
                                        csi_in.args[i + 4]);
                    csi_out->args[csi_out->num_args++] = color + diff;
                    i += 4;
                }
                break;
            default:
                csi_out->args[csi_out->num_args++] = csi_in.args[i];
                break;
        }
    }
}

void parse_csi(CSI_code *csi) {
    char *cur = csi->buffer;
    char *end = NULL;
    long int num;
    while (cur < csi->buffer + csi->len) {
        num = strtol(cur, &end, 10);
        if (end == cur) {
            num = 0;
        }
        cur = end;
        csi->args[csi->num_args++] = num;
        if (csi->num_args >= CSI_MAX_ARGS) {
            break;
        }
        cur++;
    }
}

void csi_print(CSI_code csi, FILE *stream) {
    fputc('\033', stream);
    fputc('[', stream);
    for (int i = 0; i < csi.num_args; i++) {
        fprintf(stream, "%d", csi.args[i]);
        if (i < csi.num_args - 1) {
            fputc(';', stream);
        }
    }
    fputc('m', stream);
}

void put_conv_chr(char c, FILE *stream) {
    static enum state state = STATE_NORMAL;
    static CSI_code csi_in, csi_out;

    switch (state) {
        case STATE_NORMAL:
            if (c == '\033') {
                state = STATE_ESCAPE;
            } else {
                fputc(c, stream);
            }
            break;
        case STATE_ESCAPE:
            if (c == '[') {
                state = STATE_CSI;
                csi_in.num_args = 0;
                csi_in.len = 0;
            } else {
                state = STATE_NORMAL;
                fputc('\033', stream);
                fputc(c, stream);
            }
            break;
        case STATE_CSI:
            csi_in.buffer[csi_in.len++] = c;

            // NOTE: probably odd to process on max length?
            if (c == 'm' || csi_in.len >= CSI_MAX_LEN) {
                state = STATE_NORMAL;

                csi_in.buffer[csi_in.len] = '\0';
                parse_csi(&csi_in);

                csi_out.num_args = 0;
                csi_out.len = 0;
                csi_conv_xterm_ansi(csi_in, &csi_out);
                csi_print(csi_out, stream);
            }
            break;
    }
}

int main(int argc, char *argv[]) {
    FILE *input;

    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "Error: Could not open input file %s\n", argv[1]);
            return EXIT_FAILURE;
        }
    } else {
        input = stdin;
    }

    char c;
    while ((c = fgetc(input)) != EOF) {
        put_conv_chr(c, stdout);
    }

    return EXIT_SUCCESS;
}

