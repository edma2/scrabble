#ifndef HAS_MOVEGEN
#define HAS_MOVEGEN

#include "trie.h"
#include "board.h"
#include "lexicon.h"
#include "wordlist.h"

Word *movegen(Board board, int row, int rack[26]);

#endif
