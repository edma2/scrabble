#include "common.h"

void grab(char c) {
        rack[c-'a'] += 1;
}

int main(void) {
        int row;

        grab('i');
        grab('t');
        grab('t');
        grab('t');
        grab('v');
        grab('u');
        grab('w');

        if (lexicon_init("dict.txt") < 0) {
                perror("lexicon_init()");
                return -1;
        }

        for (row = 0; row < SIZE; row++)
                movegen(row);
        board_flip();
        for (row = 0; row < SIZE; row++)
                movegen(row);
        wordlist_output(legalwords, stdout);

        wordlist_free(legalwords);
        lexicon_free();

        return 0;
}
