#ifndef HAS_MOVEGEN
#define HAS_MOVEGEN

#include "trie.h"

#define SIZE 15

extern int crosschecks[SIZE];
extern const char *letters;

int anchors(char *board, int row);
void do_crosschecks(Trie *trie, char *board, int row);
char at_loc(char *board, int row, int col);
int loc_in_bounds(int row, int col);
int loc_has_tile(char *board, int row, int col);
int loc_adjacent_tile(char *board, int row, int col);
int valid_pivots(Trie *trie, char *prefix, char *suffix);
void board_print(char *board);

#endif
