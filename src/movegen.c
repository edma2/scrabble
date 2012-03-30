#include "common.h"

int rack[27] = {0}; /* last tile represents blank tile */
Word *legalwords;

static char partial[SIZE+1];
static uint32_t crosschecks[SIZE];
static int crosssums[SIZE];
static bool anchors[SIZE];

static void partial_reset(void);
static void partial_push(char c);
static void partial_pop(void);
static void rack_remove(char c);
static void rack_add(char c);
static bool rack_has(char c);
static bool rack_has_blank(void);
static void get_crosschecks(int row);
static void get_anchors(int row);
static void extright(Node *np, int row, int col, bool anchor);
static void extright_with_char(Node *np, int row, int col, char c);
static void leftpart(Node *np, int limit, int row, int col);
static bool in_anchors_set(int col);
static bool in_crosscheck_set(int col, char c);
static int pivots(char *prefix, char *suffix);
static int crossscore(char *word, int row, int col);

void movegen(int row) {
        int col, last_anchor;
        Node *np;

        get_anchors(row);
        get_crosschecks(row);
        for (last_anchor = col = 0; col < SIZE; col++) {
                if (!in_anchors_set(col))
                        continue;
                if (rightof_tile(row, col)) {
                        get_acrossword_left(row, col, partial);
                        np = trie_lookup(lexicon, partial, NULL);
                        extright(np, row, col, true);
                        partial_reset();
                } else {
                        leftpart(lexicon->root, col-last_anchor, row, col);
                }
                last_anchor = col;
        }
}

static void partial_reset(void) {
        partial[0] = '\0';
}

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

static bool rack_has_blank(void) {
        return rack[BLANK-'a'] > 0;
}

static int crossscore(char *word, int row, int col) {
        int sum, mult;

        for (sum = 0; *word != '\0'; word++, col++) {
                if (!crosssums[col])
                        continue;
                mult = multipliers[row][col];
                if (mult <= 3)
                        sum += crosssums[col];
                else
                        sum += (values[*word-'a'] + crosschecks[col])*(mult-2);
        }
        return sum;
}

static void extright(Node *np, int row, int col, bool anchor) {
        char c, tile;

        if (filled(row, col)) {
                extright_with_char(np, row, col, board[row][col]);
                return;
        }
        if (!anchor && np->end_of_word) {
                int start = col - strlen(partial);
                int score = wordscore(partial, row, start, true);
                score += crossscore(partial, row, start);
                wordlist_add(&legalwords, partial, row, start, score);
        }
        for (c = 'a'; c <= 'z'; c++) {
                if (!in_crosscheck_set(col, c))
                        continue;
                if (rack_has(c) || rack_has_blank()) {
                        tile = (!rack_has(c) && rack_has_blank()) ? BLANK : c;
                        rack_remove(tile);
                        extright_with_char(np, row, col, c);
                        rack_add(tile);
                }
        }
}

static void extright_with_char(Node *np, int row, int col, char c) {
        Node *child;

        if (col >= SIZE)
                return;
        child = node_child(np, c);
        if (child == NULL)
                return;
        partial_push(c);
        extright(child, row, col+1, false);
        partial_pop();
}

static void leftpart(Node *np, int limit, int row, int col) {
        char c, tile;

        if (limit < 0)
                return;
        extright(np, row, col, true);
        for (c = 'a'; c <= 'z'; c++) {
                Node *child = node_child(np, c);
                if (child == NULL)
                        continue;
                if (rack_has(c) || rack_has_blank()) {
                        tile = (!rack_has(c) && rack_has_blank()) ? BLANK : c;
                        rack_remove(tile);
                        partial_push(c);
                        leftpart(child, limit-1, row, col);
                        partial_pop();
                        rack_add(tile);
                }
        }
}

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
                anchors[col] = nextto_tile(row, col);
        }
}

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

static void get_crosschecks(int row) {
        char prefix[SIZE+1];
        char suffix[SIZE+1];
        int col;

        for (col = 0; col < SIZE; col++) {
                if (filled(row, col)) {
                        crosschecks[col] = 0;
                        crosssums[col] = 0;
                        continue;
                }
                get_downword_above(row, col, prefix);
                get_downword_below(row, col, suffix);
                crosschecks[col] = pivots(prefix, suffix);
                crosssums[col] = downword_score_above(row, col, prefix) +
                        downword_score_below(row, col, suffix);
        }
}
