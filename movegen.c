#include "movegen.h"

static void get_anchors(Board board, int row, int anchors[SIZE]);
static void get_crosschecks(Board board, int row, int crosschecks[SIZE]);
static void get_leftparts(Node *np, int limit, int rack[26]);

void movegen(Board board, int row, int rack[26]) {
        int anchors[SIZE];
        int crosschecks[SIZE];

        get_anchors(board, row, anchors);
        get_crosschecks(board, row, crosschecks);
        get_leftparts(lexicon->root, 3, rack);
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

static void get_leftparts(Node *np, int limit, int rack[26]) {
        static char partial[SIZE+1];
        char c;
        int len;

        if (!limit) {
                wordlist_add(partial);
                return;
        }
        for (c = 'a'; c <= 'z'; c++) {
                if (node_child(np, c) == NULL || !rack_count(rack, c))
                        continue;
                take_off_rack(rack, c);
                len = strlen(partial);
                partial[len] = c;
                partial[len+1] = '\0';
                get_leftparts(node_child(np, c), limit-1, rack);
                partial[len] = '\0';
                put_on_rack(rack, c);
        }
}

static void get_anchors(Board board, int row, int anchors[SIZE]) {
        int col;

        for (col = 0; col < SIZE; col++) {
                if (board_covered(board, row, col)) {
                        anchors[col] = 0;
                        continue;
                }
                anchors[col] = board_leftof(board, row, col)
                            || board_rightof(board, row, col)
                            || board_below(board, row, col)
                            || board_above(board, row, col);
        }
}

static int pivots(char *prefix, char *suffix) {
        char *letters = "abcdefghijklmnopqrstuvwxyz";
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
                if (board_covered(board, row, col)) {
                        crosschecks[col] = 0;
                        continue;
                }
                for (top = row; top > 0; top--) {
                        if (!board_covered(board, top, col))
                                break;
                }
                board_get_dw(board, top, col, prefix);
                board_get_dw(board, row, col, suffix);
                crosschecks[col] = pivots(prefix, suffix);
        }
}
