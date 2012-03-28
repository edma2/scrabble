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

WordList *wordlist_new(void);
Word *word_new(char *letters);
Word *wordlist_add(WordList *ml, char *letters);

extern const char *letters;

extern int crosschecks[SIZE];
extern int anchors[SIZE];
extern WordList left_parts;

void do_anchors(char *board, int row);
void do_crosschecks(char *board, int row);
void do_left_parts(Node *np, int limit, int tiles[26]);

char at_loc(char *board, int row, int col);
int loc_in_bounds(int row, int col);
int loc_has_tile(char *board, int row, int col);
int loc_adjacent_tile(char *board, int row, int col);

int valid_pivots(char *prefix, char *suffix);

#endif
