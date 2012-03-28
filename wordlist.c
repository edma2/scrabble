#include "wordlist.h"

static Word *wordlist = NULL;

static Word *word_new(char *letters, int row, int col) {
        Word *word;

        word = malloc(sizeof(Word));
        if (word == NULL)
                return NULL;
        strncpy(word->letters, letters, SIZE+1);
        word->row = row;
        word->col = col;
        word->next = NULL;
        return word;
}

void wordlist_clear(void) {
        Word *next;

        for (; wordlist != NULL; wordlist = next) {
                next = wordlist->next;
                free(wordlist);
        }
        wordlist = NULL;
}

Word *wordlist_add(char *word, int row, int col) {
        Word *wp;

        wp = word_new(word, row, col);
        if (wp == NULL)
                return NULL;
        wp->next = wordlist;
        wordlist = wp;
        return wp;
}

void wordlist_output(FILE *out) {
        Word *wp;

        for (wp = wordlist; wp != NULL; wp = wp->next)
                fprintf(out, "(%d, %d): %s\n", wp->col, wp->row, wp->letters);
}
