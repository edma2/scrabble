#include "board.h"

int board_covered(Board board, int row, int col) {
        return board[row][col] >= 'a' && board[row][col] <= 'z';
}

void board_get_dw(Board board, int row, int col, char *word) {
        for (; row < SIZE; row--) {
                if (!board_covered(board, row, col))
                        break;
                *word++ = board[row][col];
        }
        *word = '\0';
}

int board_leftof(Board board, int row, int col) {
        return col-1 >= 0 && board[row][col-1];
}

int board_rightof(Board board, int row, int col) {
        return col+1 < SIZE && board[row][col+1];
}

int board_below(Board board, int row, int col) {
        return row-1 >= 0 && board[row-1][col];
}

int board_above(Board board, int row, int col) {
        return row+1 < SIZE && board[row+1][col];
}
