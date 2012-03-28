#include "movegen.h"

void board_print(const char *board) {
        int row, col;

        for (row = 0; row < SIZE; row++) {
                for (col = 0; col < SIZE; col++)
                        printf("%c", *(board + row * SIZE + col));
                printf("\n");
        }
}
