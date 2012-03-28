#include "scrabble.h"

Board board = {
"...............",
"...............",
"...............",
"...............",
"...............",
"...............",
"...............",
"..r............",
"..at...........",
"..t............",
"...............",
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

        movegen(board, 5, rack);
        wordlist_output(stdout);

        lexicon_free();
        wordlist_clear();

        return 0;
}
