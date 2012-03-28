#include "scrabble.h"

void print_crosschecks(void);

int main(void) {
        Trie *trie;
        int nwords;

        trie = trie_new();
        nwords = trie_load(trie, "dict.txt");
        if (nwords < 0) {
                fprintf(stderr, "Failed to load dictionary!\n");
                return -1;
        }
        printf("%d words loaded\n", nwords);
        printf("size: %u MB\n", trie->size * sizeof(Node)/1000000);

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

        int i;
        for (i = 0; i < SIZE; i++) {
                do_crosschecks(trie, board, i);
                do_anchors(board, i);
        }

        trie_free(trie);

        return 0;
}

void print_crosschecks(void) {
        int i, j;
        for (i = 0; i < SIZE; i++) {
                /* Not interesting */
                if (crosschecks[i] == ~0)
                        continue;
                for (j = 0; j < SIZE; j++) {
                        if (crosschecks[i] & (1<<j))
                                printf("%c ,", letters[j]);
                }
        }
}
