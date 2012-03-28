#ifndef HAS_BOARD
#define HAS_BOARD

#define SIZE 15

typedef char Board[SIZE][SIZE];

int board_covered(Board board, int row, int col);
int board_empty(Board board, int row, int col);
void board_get_dw(Board board, int row, int col, char *word);
int board_leftof(Board board, int row, int col);
int board_rightof(Board board, int row, int col);
int board_below(Board board, int row, int col);
int board_above(Board board, int row, int col);

#endif
