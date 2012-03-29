#ifndef HAS_COMMON
#define HAS_COMMON

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SIZE 15
#define BLANK ('z'+1)

typedef char Board[SIZE][SIZE+1];
typedef int bool;
enum {false, true};

typedef struct Node Node;
struct Node {
        Node *children[26];
        int end_of_word;
};

typedef struct Trie Trie;
struct Trie {
        Node *root;
        size_t size;
};

typedef struct Word Word;
struct Word {
        char letters[SIZE+1];
        int row, col;
        Word *next;
        int score;
};

extern int rack[27];
extern Board board;
extern Trie *lexicon;
extern Word *legalwords;
extern int multipliers[SIZE][SIZE];
extern int values[27];

int lexicon_init(char *path);
int lexicon_contains(char *s);
void lexicon_free(void);

bool filled(int row, int col);
void get_downword(int row, int col, char *word);
void get_acrossword(int row, int col, char *word);
bool rightof_tile(int row, int col);
bool leftof_tile(int row, int col);
bool above_tile(int row, int col);
bool below_tile(int row, int col);
bool adjtile(int row, int col);

void replay_moves(FILE *out);
void board_output(FILE *out);
int wordscore(char *word, int row, int col, bool across);

void movegen(int row);

Node *node_new(void);
Node *node_child(Node *np, char c);
Node *node_add_child(Node *np, char c);
void node_free(Node *np);
Trie *trie_new(void);
Node *trie_lookup(Trie *trie, char *s, char **si);
Node *trie_insert(Trie *trie, char *s);
void trie_free(Trie *trie);

Word *wordlist_add(Word **wl, char *word, int row, int col, int score);
void wordlist_free(Word *wl);
void wordlist_output(Word *wl, FILE *out);
Word *bestmove(void);

#endif
