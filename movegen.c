#include "movegen.h"

static int valid_word(char *word);
const char *letters = "abcdefghijklmnopqrstuvwxyz";

/* Each element of this array represents the crosscheck for a square of some
 * row. Each crosscheck is represented as a bit vector mapped over the @letters
 * string above. If bit @j is on, then the @letters[@j] is in the set. */
int crosschecks[SIZE];

int anchors[SIZE];
WordList left_parts = {.head=NULL};

/*
 * @np          Trie node pointer
 * @limit       how long the partial can be
 * @tiles       each element is the count for that letter
 */
void do_left_parts(Node *np, int limit, int tiles[26]) {
        char c;
        static char partial[SIZE+1];

        if (!limit) {
                wordlist_add(&left_parts, partial);
                return;
        }

        /* For each edge in trie node */
        for (c = 'a'; c <= 'z'; c++) {
                int len;

                if (node_child(np, c) == NULL || !tiles[c-'a'])
                        continue;

                /* Take out a tile */
                tiles[c-'a']--;

                /* Stick it to the end of partial */
                len = strlen(partial);
                partial[len] = c;
                partial[len+1] = '\0';

                do_left_parts(node_child(np, c), limit-1, tiles);

                /* Undo everything */
                partial[len] = '\0';
                tiles[c-'a']++;
        }
}

void wordlist_free(WordList *wl) {
        Word *word, *next;

        for (word = wl->head; word != NULL; word = next) {
                next = word->next;
                free(word);
        }
}

WordList *wordlist_new(void) {
        WordList *wl;

        wl = malloc(sizeof(WordList));
        if (wl == NULL)
                return NULL;
        wl->head = NULL;
        return wl;
}

Word *word_new(char *letters) {
        Word *word;

        word = malloc(sizeof(Word));
        if (word == NULL)
                return NULL;
        strncpy(word->letters, letters, SIZE+1);
        word->next = NULL;
        return word;
}

Word *wordlist_add(WordList *wl, char *letters) {
        Word *word;

        word = word_new(letters);
        if (word == NULL)
                return NULL;
        /* Appending to front takes O(1) time */
        word->next = wl->head;
        wl->head = word;
        return word;
}

void do_anchors(char *board, int row) {
        int col;

        for (col = 0; col < SIZE; col++) {
                if (loc_has_tile(board, row, col))
                        continue;
                anchors[col] = loc_adjacent_tile(board, row, col);
        }
}

int loc_adjacent_tile(char *board, int row, int col) {
        return (loc_in_bounds(row-1, col)
            && loc_has_tile(board, row-1, col))
            || (loc_in_bounds(row+1, col)
            && loc_has_tile(board, row+1, col))
            || (loc_in_bounds(row, col-1)
            && loc_has_tile(board, row, col-1))
            || (loc_in_bounds(row, col+1)
            && loc_has_tile(board, row, col+1));
}

/* Fills up crosschecks array for @jth row of @board */
void do_crosschecks(char *board, int row) {
        char prefix[SIZE];
        char suffix[SIZE];
        int col, i, j;

        for (col = 0; col < SIZE; col++) {
                /* Skip if square is occupied */
                if (loc_has_tile(board, row, col)) {
                        crosschecks[col] = 0;
                        continue;
                }

                /* If (@col, @row) is below a down word, build up prefix */
                for (j = row; loc_in_bounds(j-1, col) &&
                                loc_has_tile(board, j-1, col); j--)
                        ;
                for (i = 0; j != row; j++)
                        prefix[i++] = at_loc(board, j, col);
                prefix[i] = '\0';

                /* Now, the suffix */
                i = 0;
                for (j = row; loc_in_bounds(j+1, col) &&
                                loc_has_tile(board, j+1, col); j++)
                        suffix[i++] = at_loc(board, j+1, col);
                suffix[i] = '\0';

                if (!suffix[0] && !prefix[0])
                        crosschecks[col] = ~0; /* unconstrained */
                else
                        crosschecks[col] = valid_pivots(prefix, suffix);
        }
}

static int valid_word(char *word) {
        return trie_has(lexicon, word);
}

/* Returns valid pivots of prefix + suffix as a bit vector */
int valid_pivots(char *prefix, char *suffix) {
        int i, pivot, pivots;
        char word[SIZE];

        /* Build up word */
        for (i = 0; *prefix != '\0'; prefix++)
                word[i++] = *prefix;
        /* Leave a space for pivot */
        pivot = i++;
        for (; *suffix != '\0'; suffix++)
                word[i++] = *suffix;
        word[i] = '\0';

        for (pivots = i = 0; i < strlen(letters); i++) {
                word[pivot] = letters[i];
                if (valid_word(word))
                        pivots |= 1<<i;
        }
        return pivots;
}

int loc_in_bounds(int row, int col) {
        return row >= 0 && row < SIZE && col >= 0 && col < SIZE;
}

/* Returns the square located at @row and @col of @board */
char at_loc(char *board, int row, int col) {
        return *(board + row * SIZE + col);
}

/* Returns nonzero if @row and @col of @board is covered by a tile */
int loc_has_tile(char *board, int row, int col) {
        char c = at_loc(board, row, col);
        return c >= 'a' && c <= 'z';
}
