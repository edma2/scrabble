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

void wordlist_clear(void);
Word *wordlist_add(char *word, int row, int col);
void wordlist_output(FILE *out);

#endif
