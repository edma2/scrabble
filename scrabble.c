#include "scrabble.h"

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

        trie_free(trie);

        char *board = "...............\
...............\
...............\
...............\
...............\
...............\
...............\
..r............\
..a............\
..t............\
...............\
...............\
...............\
...............\
...............";

        board_print(board);

        return 0;
}
