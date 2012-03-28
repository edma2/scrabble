#ifndef HAS_BOARD
#define HAS_BOARD

#define SIZE 15

typedef char Board[SIZE][SIZE+1];

int filled(Board board, int row, int col);
void get_downword(Board board, int row, int col, char *word);
void get_acrossword(Board board, int row, int col, char *word);
int adj_to_tile(Board board, int row, int col);

#endif
