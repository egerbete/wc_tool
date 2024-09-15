//
// Created by Balázs Horváth on 16.09.2024.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum {
    OUTSIDE_WORD,
    INSIDE_WORD,
} State;

typedef enum {
    BYTES,
    LINES,
    WORDS,
    CHARACTERS,
    NO_FLAG,
} Type;

void count_from_file(char* file_name, Type type);
void count_from_stdin(Type type);
void print_usage(void);


int main(int argc, char* argv[]) {
    int opt;

    opterr = 0;
    if ((opt = getopt(argc, argv, "hc:l:w:m:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage(); break;
            case 'c':
                count_from_file(optarg, BYTES); break;
            case 'l':
                count_from_file(optarg, LINES); break;
            case 'w':
                count_from_file(optarg, WORDS); break;
            case 'm':
                count_from_file(optarg, CHARACTERS); break;
            case '?':
                if (optopt == 'c') {
                    count_from_stdin(BYTES);
                } else if (optopt == 'l') {
                    count_from_stdin(LINES);
                } else if (optopt == 'w') {
                    count_from_stdin(WORDS);
                } else if (optopt == 'm') {
                    count_from_stdin(CHARACTERS);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option '-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
                }
                return EXIT_FAILURE;
            default:
                abort();
        }
    } else if (argc == 2) {
        count_from_file(argv[1], NO_FLAG);
    } else if (argc == 1) {
        count_from_stdin(NO_FLAG);
    }

    return 0;
}


void count_from_file(char* file_name, Type type) {
    int c;
    size_t bytes = 0;
    size_t lines = 0;
    size_t words = 0;
    size_t characters = 0;

    FILE* file_ptr = fopen(file_name, "rb");

    if (file_ptr == NULL) {
        fprintf(stderr, "Unable to open %s!\n", file_name);
        exit(EXIT_FAILURE);
    }

    State in_word = OUTSIDE_WORD;
    while ((c = fgetc(file_ptr)) != EOF) {
        // count bytes
        ++bytes;

        // count (utf-8) characters
        if ((c & 0xC0) != 0x80) {
            ++characters;
        }

        // count new lines
        if (c == '\n') {
            ++lines;
        }

        // count words
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            in_word = OUTSIDE_WORD;
        } else if (in_word == OUTSIDE_WORD) {
            in_word = INSIDE_WORD;
            ++words;
        }
    }

    fclose(file_ptr);

    switch (type) {
        case BYTES:
            printf("%8lu %s\n", bytes, file_name); break;
        case LINES:
            printf("%8lu %s\n", lines, file_name); break;
        case WORDS:
            printf("%8lu %s\n", words, file_name); break;
        case CHARACTERS:
            printf("%8lu %s\n", characters, file_name); break;
        case NO_FLAG:
            printf("%8lu %8lu %8lu %s\n", lines, words, bytes, file_name); break;
        default:
            exit(EXIT_FAILURE);
    }
}


void count_from_stdin(Type type) {
    int c;
    size_t bytes = 0;
    size_t lines = 0;
    size_t words = 0;
    size_t characters = 0;

    State in_word = OUTSIDE_WORD;
    while ((c = getchar()) != EOF) {
        // count bytes
        ++bytes;

        // count (utf-8) characters
        if ((c & 0xC0) != 0x80) {
            ++characters;
        }

        // count new lines
        if (c == '\n') {
            ++lines;
        }

        // count words
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
            in_word = OUTSIDE_WORD;
        } else if (in_word == OUTSIDE_WORD) {
            in_word = INSIDE_WORD;
            ++words;
        }
    }

    switch (type) {
        case BYTES:
            printf("%8lu\n", bytes); break;
        case LINES:
            printf("%8lu\n", lines); break;
        case WORDS:
            printf("%8lu\n", words); break;
        case CHARACTERS:
            printf("%8lu\n", characters); break;
        case NO_FLAG:
            printf("%8lu %8lu %8lu\n", lines, words, bytes); break;
        default:
            exit(EXIT_FAILURE);
    }
}


void print_usage(void) {
    char* usage_text =
        "\n"
        "    wc_tool – word, line, character, and byte count\n\n"

        "    A simple version of the Unix command line tool /wc/ to practice C programming.\n"
        "    Inspired by: https://codingchallenges.fyi/challenges/challenge-wc\n\n"

        "    wc [-clmwh] [file]\n\n"

        "    The wc utility displays the number of lines, words, and bytes contained in a single input file,\n"
        "    or standard input (if no file is specified) to the standard output. A line is defined as a string\n"
        "    of characters delimited by a ⟨newline⟩ character.\n"
        "    A word is defined as a string of characters delimited by white space characters.\n"
        "    White space characters are the set of characters for which the iswspace() function returns true.\n\n"

        "    -c    The number of bytes in the input file or standard input.\n"
        "    -l    The number of lines in the input file or standard input.\n"
        "    -m    The number of characters in the input file or standard input.\n"
        "          If the current locale does not support multibyte characters, this is equivalent to the -c option.\n"
        "    -w    The number of words in the input file or standard input.\n"
        "    -h    Print this usage message.\n\n"

        "    The default action is equivalent to specifying the -c, -l and -w options.\n\n"

        "    If no file is specified, the standard input is used and no file name is displayed.\n"
        "    The prompt will accept input until receiving EOF, or [^D] in most environments.\n\n"
    ;

    printf("%s", usage_text);
}