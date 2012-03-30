#include "common.h"

Word *bestmove(void) {
        Word *wp, *best = NULL;;
        int max = 0;

        for (wp = legalwords; wp != NULL; wp = wp->next) {
                if (wp->score > max) {
                        max = wp->score;
                        best = wp;
                }
        }
        return best;
}

static Word *word_new(char *letters, int row, int col, int score) {
        Word *word;

        word = malloc(sizeof(Word));
        if (word == NULL)
                return NULL;
        strncpy(word->letters, letters, MAXWORD);
        word->row = row;
        word->col = col;
        word->score = score;
        word->next = NULL;
        return word;
}

void wordlist_free(Word *wl) {
        Word *next;

        for (; wl != NULL; wl = next) {
                next = wl->next;
                free(wl);
        }
}

Word *wordlist_add(Word **wl, char *word, int row, int col, int score) {
        Word *wp;

        wp = word_new(word, row, col, score);
        if (wp == NULL)
                return NULL;
        wp->next = *wl;
        *wl = wp;
        return wp;
}

void wordlist_output(Word *wl, FILE *out) {
        for (; wl != NULL; wl = wl->next)
                fprintf(out, "(%d, %d): %s\n", wl->col, wl->row, wl->letters);
}
