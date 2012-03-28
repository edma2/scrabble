#include "board.h"

int filled(Board board, int row, int col) {
        return board[row][col] >= 'a' && board[row][col] <= 'z';
}

void get_downword(Board board, int row, int col, char *word) {
        for (; row < SIZE; row++) {
                if (!filled(board, row, col))
                        break;
                *word++ = board[row][col];
        }
        *word = '\0';
}

void get_acrossword(Board board, int row, int col, char *word) {
        for (; col < SIZE; col++) {
                if (!filled(board, row, col))
                        break;
                *word++ = board[row][col];
        }
        *word = '\0';
}

int adj_to_tile(Board board, int row, int col) {
        return (col-1 >= 0 && filled(board, row, col-1))
            || (col+1 < SIZE && filled(board, row, col+1))
            || (row-1 >= 0 && filled(board, row-1, col))
            || (row+1 < SIZE && filled(board, row+1, col));
}
