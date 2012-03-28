#include "scrabble.h"

char *board = "...............\
...............\
...............\
...............\
...............\
...............\
...............\
..r............\
..at...........\
..t............\
...............\
...............\
...............\
...............\
...............";

int main(void) {
        int nwords;

        lexicon = trie_new();
        nwords = trie_load(lexicon, "dict.txt");
        if (nwords < 0) {
                fprintf(stderr, "Failed to load dictionary!\n");
                return -1;
        }

        int i;
        for (i = 0; i < SIZE; i++) {
                do_crosschecks(board, i);
                do_anchors(board, i);
        }

        int tiles[26] = {0};
        tiles['a'-'a'] = 1;
        tiles['r'-'a'] = 2;
        tiles['t'-'a'] = 1;
        do_left_parts(lexicon->root, 3, tiles);

        Word *wp;
        for (wp = left_parts.head; wp != NULL; wp = wp->next)
                printf("%s\n", wp->letters);

        trie_free(lexicon);
        wordlist_free(&left_parts);

        return 0;
}
