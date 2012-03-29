#include "common.h"

void grab(char c) {
        rack[c-'a'] += 1;
}

int main(void) {
        int row;

        grab('a');
        grab('r');
        grab('t');
        grab('s');
        grab('p');
        grab(BLANK);

        if (lexicon_init("dict.txt") < 0) {
                perror("lexicon_init()");
                return -1;
        }

        for (row = 0; row < SIZE; row++)
                movegen(row);
        Word *best = bestmove();
        printf("(%d, %d): %s = %d points\n", best->col, best->row, best->letters, best->score);

        wordlist_free(legalwords);
        lexicon_free();

        return 0;
}
