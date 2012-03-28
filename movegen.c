#include "movegen.h"

static char *letters = "abcdefghijklmnopqrstuvwxyz";

static void get_anchors(Board board, int row, int anchors[SIZE]);
static void get_crosschecks(Board board, int row, int crosschecks[SIZE]);
static void get_leftparts(Board board, int row, int rack[26]);

static void extend_right(Board board, char *partial, int row, int col, int
                rack[26], int crosschecks[SIZE], Node *np);

static void leftpartsgen(Node *np, int limit, int rack[26], int row, int col);
static int bit(int word, int n);

static int rack_count(int rack[26], char c);
static void put_on_rack(int rack[26], char c);
static void take_off_rack(int rack[26], char c);

void movegen(Board board, int row, int rack[26]) {
        int crosschecks[SIZE];
        Word *wp;
        Node *np;

        get_crosschecks(board, row, crosschecks);
        get_leftparts(board, row, rack);
        for (wp = wordlist; wp != NULL; wp = wp->next) {
                np = trie_lookup(lexicon, wp->letters, NULL);
                extend_right(board, wp->letters, wp->row, wp->col, rack,
                                crosschecks, np);
        }
}

static void extend_right(Board board, char *partial, int row, int col,
                        int rack[26], int crosschecks[SIZE], Node *np) {
        char c;
        int len;

        if (!filled(board, row, col)) {
                if (np->end_of_word) {
                        printf("Legal move: %s\n", partial);
                } else {
                        for (c = 'a'; c <= 'z'; c++) {
                                if (node_child(np, c) == NULL)
                                        continue;
                                if (rack_count(rack, c)
                                        && (bit(crosschecks[col], c-'a'))) {
                                        take_off_rack(rack, c);
                                        len = strlen(partial);
                                        partial[len] = c;
                                        partial[len+1] = '\0';
                                        extend_right(board, partial, row,
                                                        col+1, rack,
                                                        crosschecks,
                                                        node_child(np, c));
                                        partial[len] = '\0';
                                        put_on_rack(rack, c);
                                }
                        }
                }

        } else {
                c = board[row][col];
                if (node_child(np, c)) {
                        len = strlen(partial);
                        partial[len] = c;
                        partial[len+1] = '\0';
                        extend_right(board, partial, row, col+1, rack,
                                        crosschecks, node_child(np, c));
                        partial[len] = '\0';
                        put_on_rack(rack, c);
                }
        }
}

static void get_leftparts(Board board, int row, int rack[26]) {
        int anchors[SIZE];
        int col, i;
        char leftpart[SIZE+1];

        get_anchors(board, row, anchors);
        for (col = 0; col < SIZE; col++) {
                if (!anchors[col])
                        continue;
                if (col > 0 && filled(board, row, col-1)) {
                        for (i = col-1; i >= 0 && filled(board, row, i); i--)
                                ;
                        get_acrossword(board, row, i+1, leftpart);
                        wordlist_add(leftpart, row, i+1);
                } else {
                        int nonanch = 0;
                        for (i = col-1; i >= 0 && !anchors[i]; i--)
                                nonanch++;
                        leftpartsgen(lexicon->root, nonanch, rack, row, i+1);
                }
        }
}

static int bit(int word, int n) {
        return word & (1<<n);
}

static int rack_count(int rack[26], char c) {
        return rack[c-'a'];
}

static void take_off_rack(int rack[26], char c) {
        rack[c-'a']--;
}

static void put_on_rack(int rack[26], char c) {
        rack[c-'a']++;
}

static void leftpartsgen(Node *np, int limit, int rack[26], int row, int col) {
        static char partial[SIZE+1];
        char c;
        int len;

        if (!limit) {
                wordlist_add(partial, row, col);
                return;
        }
        for (c = 'a'; c <= 'z'; c++) {
                if (node_child(np, c) == NULL || !rack_count(rack, c))
                        continue;
                take_off_rack(rack, c);
                len = strlen(partial);
                partial[len] = c;
                partial[len+1] = '\0';
                leftpartsgen(node_child(np, c), limit-1, rack, row, col);
                partial[len] = '\0';
                put_on_rack(rack, c);
        }
}

static void get_anchors(Board board, int row, int anchors[SIZE]) {
        int col;

        for (col = 0; col < SIZE; col++) {
                if (filled(board, row, col)) {
                        anchors[col] = 0;
                        continue;
                }
                anchors[col] = adj_to_tile(board, row, col);
        }
}

static int pivots(char *prefix, char *suffix) {
        int i, p, pivots;
        char word[SIZE+1];

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

static void get_crosschecks(Board board, int row, int crosschecks[SIZE]) {
        char prefix[SIZE+1];
        char suffix[SIZE+1];
        int col, top;

        for (col = 0; col < SIZE; col++) {
                if (filled(board, row, col)) {
                        crosschecks[col] = 0;
                        continue;
                }
                for (top = row-1; top > 0; top--) {
                        if (!filled(board, top, col))
                                break;
                }
                get_downword(board, top+1, col, prefix);
                get_downword(board, row+1, col, suffix);
                crosschecks[col] = pivots(prefix, suffix);
        }
}
