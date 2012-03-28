#include "scrabble.h"

Board board = {
"...............",
"...............",
"...............",
"...............",
"...............",
"...............",
"...............",
"..r.....g......",
"..at....o......",
"..t.....o......",
"........d......",
"...............",
"...............",
"...............",
"..............."};

int main(void) {
        int rack[26] = {0};
        Word *legalwords;
        int row;

        rack['a'-'a'] = 1;
        rack['r'-'a'] = 1;
        rack['t'-'a'] = 1;
        rack['s'-'a'] = 1;
        rack['p'-'a'] = 1;

        if (lexicon_init("dict.txt") < 0) {
                perror("lexicon_init()");
                return -1;
        }

        for (row = 0; row < SIZE; row++) {
                legalwords = movegen(board, row, rack);
                wordlist_output(legalwords, stdout);
                wordlist_free(legalwords);
        }

        lexicon_free();

        return 0;
}
