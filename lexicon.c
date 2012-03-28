#include "lexicon.h"

Trie *lexicon;

int lexicon_init(char *path) {
        FILE *fp;
        char buf[1024];
        int nwords = 0;

        lexicon = trie_new();
        if (lexicon == NULL)
                return -1;
        fp = fopen(path, "r");
        while (fgets(buf, sizeof(buf), fp) != NULL) {
                buf[strlen(buf)-1] = '\0';
                if (trie_insert(lexicon, buf) == NULL) {
                        trie_free(lexicon);
                        return -1;
                }
                nwords++;
        }
        fclose(fp);
        return nwords;
}

int lexicon_contains(char *s) {
        char *si;
        Node *np;

        np = trie_lookup(lexicon, s, &si);
        return np->end_of_word && *si == '\0';
}

void lexicon_free(void) {
        trie_free(lexicon);
}
