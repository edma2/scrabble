#include "trie.h"

/* Returns a new Trie or NULL on failure */
Trie *trie_new(void) {
        Trie *trie;

        trie = malloc(sizeof(Trie));
        if (trie == NULL)
                return NULL;
        trie->root = node_new();
        if (trie->root == NULL) {
                free(trie);
                return NULL;
        }
        trie->size = 1;
        return trie;
}

/* Frees memory allocated to TRIE */
void trie_free(Trie *trie) {
        node_free(trie->root);
        free(trie);
}

/* Returns a new Node with no children, or NULL on failure */
Node *node_new(void) {
        return calloc(1, sizeof(Node)); /* Zero out memory */
}

/* Frees memory allocated to NP */
void node_free(Node *np) {
        char c;

        for (c = 'a'; c <= 'z'; c++) {
                if (node_child(np, c) != NULL)
                        node_free(node_child(np, c));
        }
        free(np);
}

/* Returns the child of NP indexed by C */
Node *node_child(Node *np, char c) {
        return np->children[c - 'a'];
}

/* Adds a new node to the index of NP specified at C.
 * Returns false on failure. */
Node *node_add_child(Node *np, char c) {
        return np->children[c - 'a'] = node_new();
}

/* Returns the last node of TRIE traversed when scanning the string S. If SI is
 * not NULL, it will store the address of some suffix of S not found. */
Node *trie_lookup(Trie *trie, char *s, char **si) {
        Node *np, *child;

        for (np = trie->root; *s != '\0'; s++) {
                child = node_child(np, *s);
                if (child == NULL)
                        break;
                np = child;
        }
        /* Scribble down the address where search failed */
        if (si != NULL)
                *si = s;
        return np;
}

/* Inserts S into TRIE. Returns the new node marking the end of S, or NULL on
 * memory failure. */
Node *trie_insert(Trie *trie, char *s) {
        Node *np;
        char *si;

        np = trie_lookup(trie, s, &si);
        for (s = si; *s != '\0'; s++) {
                np = node_add_child(np, *s);
                if (np == NULL)
                        return NULL;
                trie->size++;
        }
        np->end_of_word = 1;
        return np;
}
