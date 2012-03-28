#ifndef HAS_TRIE
#define HAS_TRIE

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node Node;
typedef struct Trie Trie;

struct Trie {
        Node *root;
        size_t size;
};

struct Node {
        Node *children[26];
        int end_of_word;
};

Node *node_new(void);
Node *node_child(Node *np, char c);
Node *node_add_child(Node *np, char c);
void node_free(Node *np);

Trie *trie_new(void);
Node *trie_lookup(Trie *trie, char *s, char **si);
Node *trie_insert(Trie *trie, char *s);
void trie_free(Trie *trie);

#endif
