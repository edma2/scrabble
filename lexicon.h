#ifndef HAS_LEXICON
#define HAS_LEXICON

#include "trie.h"

extern Trie *lexicon;

int lexicon_init(char *path);
int lexicon_contains(char *s);
void lexicon_free(void);

#endif
