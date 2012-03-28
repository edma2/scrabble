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

        rack['a'-'a'] = 1;
        rack['r'-'a'] = 2;
        rack['t'-'a'] = 1;
        rack['s'-'a'] = 1;

        if (lexicon_init("dict.txt") < 0) {
                perror("lexicon_init()");
                return -1;
        }

        legalwords = movegen(board, 10, rack);
        wordlist_output(legalwords, stdout);

        wordlist_free(legalwords);
        lexicon_free();
        return 0;
}
