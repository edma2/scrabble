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
        rack['a'-'a'] = 1;
        rack['r'-'a'] = 2;
        rack['t'-'a'] = 1;

        if (lexicon_init("dict.txt") < 0) {
                perror("lexicon_init()");
                return -1;
        }

        int i;
        for (i = 0; i < SIZE; i++)
                movegen(board, i, rack);
        wordlist_output(stdout);

        lexicon_free();
        wordlist_clear();

        return 0;
}
