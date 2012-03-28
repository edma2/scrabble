#ifndef HAS_WORDLIST
#define HAS_WORDLIST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"

typedef struct Word Word;
struct Word {
        char letters[SIZE+1];
        int row, col;
        Word *next;
};

Word *wordlist_add(Word **wl, char *word, int row, int col);
void wordlist_free(Word *wl);
void wordlist_output(Word *wl, FILE *out);

#endif
