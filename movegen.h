#ifndef HAS_MOVEGEN
#define HAS_MOVEGEN

#include "trie.h"

#define SIZE 15

typedef struct Word Word;
struct Word {
        char letters[SIZE+1]; /* '\0' terminated */
        Word *next;
};

typedef struct WordList WordList;
struct WordList {
        Word *head;
};

extern int crosschecks[SIZE];
extern const char *letters;

WordList *wordlist_new(void);
Word *word_new(char *letters);
Word *wordlist_add(WordList *ml, char *letters);

int anchors(char *board, int row);
void do_crosschecks(Trie *trie, char *board, int row);
char at_loc(char *board, int row, int col);
int loc_in_bounds(int row, int col);
int loc_has_tile(char *board, int row, int col);
int loc_adjacent_tile(char *board, int row, int col);
int valid_pivots(Trie *trie, char *prefix, char *suffix);
void board_print(char *board);

#endif
