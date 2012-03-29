#include "common.h"

int rack[26];
Word *legalwords;

static char partial[SIZE+1];

static void partial_push(char c) {
        int len = strlen(partial);
        partial[len] = c;
        partial[len+1] = '\0';
}

static void partial_pop(void) {
        partial[strlen(partial)-1] = '\0';
}

static void rack_remove(char c) {
        rack[c-'a']--;
}

static void rack_add(char c) {
        rack[c-'a']++;
}

static bool rack_has(char c) {
        return rack[c-'a'] > 0;
}

static void extright_with_char(Node *np, int row, int col, char c);
static bool in_crosscheck_set(int col, char c);

static void extright(Node *np, int row, int col, bool anchor) {
        char c;

        if (filled(row, col)) {
                extright_with_char(np, row, col, board[row][col]);
                return;
        }
        if (!anchor && np->end_of_word) {
                int start = col - strlen(partial);
                wordlist_add(&legalwords, partial, row, start);
        }
        for (c = 'a'; c <= 'z'; c++) {
                if (rack_has(c) && in_crosscheck_set(col, c)) {
                        rack_remove(c);
                        extright_with_char(np, row, col, c);
                        rack_add(c);
                }
        }
}

static void extright_with_char(Node *np, int row, int col, char c) {
        Node *child;

        child = node_child(np, c);
        if (child == NULL)
                return;
        partial_push(c);
        extright(child, row, col+1, false);
        partial_pop();
}

static void leftpart(Node *np, int limit, int row, int col) {
        char c;

        if (!limit) {
                extright(np, row, col, true);
                return;
        }
        for (c = 'a'; c <= 'z'; c++) {
                Node *child = node_child(np, c);
                if (child == NULL || !rack_has(c))
                        continue;
                rack_remove(c);
                partial_push(c);
                leftpart(child, limit-1, row, col);
                partial_pop();
                rack_add(c);
        }
}

static bool anchors[SIZE];

static bool in_anchors_set(int col) {
        return anchors[col];
}

static void get_anchors(int row) {
        int col;

        for (col = 0; col < SIZE; col++) {
                if (filled(row, col)) {
                        anchors[col] = false;
                        continue;
                }
                anchors[col] = adjtile(row, col);
        }
}

static uint32_t crosschecks[SIZE];

static bool in_crosscheck_set(int col, char c) {
        return crosschecks[col] & (1 << (c-'a'));
}

static int pivots(char *prefix, char *suffix) {
        int i, p, pivots;
        char word[SIZE+1];
        static char *letters = "abcdefghijklmnopqrstuvwxyz";

        if (*prefix == '\0' && *suffix == '\0')
                return ~0;
        for (i = 0; *prefix != '\0'; prefix++)
                word[i++] = *prefix;
        p = i;
        word[p] = '?';
        for (i++; *suffix != '\0'; suffix++)
                word[i++] = *suffix;
        word[i] = '\0';
        pivots = 0;
        for (i = 0; i < strlen(letters); i++) {
                word[p] = letters[i];
                if (lexicon_contains(word))
                        pivots |= (1<<i);
        }
        return pivots;
}

static int startrow(int row, int col) {
        for (; below_tile(row, col); row--)
                ;
        return row;
}

static void get_crosschecks(int row) {
        char prefix[SIZE+1];
        char suffix[SIZE+1];
        int col;

        for (col = 0; col < SIZE; col++) {
                if (filled(row, col)) {
                        crosschecks[col] = 0;
                        continue;
                }
                get_downword(startrow(row, col), col, prefix);
                get_downword(row+1, col, suffix);
                crosschecks[col] = pivots(prefix, suffix);
        }
}

static int startcol(int row, int col) {
        for (; rightof_tile(row, col); col--)
                ;
        return col;
}

void movegen(int row) {
        int col, last;
        Node *np;

        get_anchors(row);
        get_crosschecks(row);
        for (last = col = 0; col < SIZE; col++) {
                if (!in_anchors_set(col))
                        continue;
                if (rightof_tile(row, col)) {
                        get_acrossword(row, startcol(row, col), partial);
                        np = trie_lookup(lexicon, partial, NULL);
                        extright(np, row, col, true);
                } else {
                        leftpart(lexicon->root, col-last, row, col);
                        last = col;
                }
        }
}
