#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "xterm2ansi.h"
#include "utils.h"

void process_escape_code(char *escape_code) {
    int i = 0;
    int fg_set = 0;
    int bg_set = 0;
    int prev = 0;
    while (i < strlen(escape_code)) {
        char num[4] = {0};
        int j = 0;
        char last = 0;
        while (escape_code[i] != ';' && i < strlen(escape_code)) {
            num[j++] = escape_code[i++];
        }
        if (i < strlen(escape_code)) {
            last = escape_code[i];
        }
        int code = atoi(num);
        if (fg_set) {
            printf("%d", xterm_to_ansi(code));
            if(last) printf("%c", last);
        } else if (bg_set) {
            printf("%d", xterm_to_ansi(code) + 10);
            if(last) printf("%c", last);
        } else if (code != 38 && code != 48 && prev != 38 && prev != 48) {
            printf("%d", code);
            if(last) printf("%c", last);
        }
        fg_set = (prev == 38 && code == 5);
        bg_set = (prev == 48 && code == 5);
        prev = code;
        i++;
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

                    printf("\033[");
                    process_escape_code(escape_code);
                    printf("m");

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

