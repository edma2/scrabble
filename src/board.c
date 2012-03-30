#include "common.h"

Board board = {
       /*0123456789!@#$%*/
/* 0 */ "...............",
/* 1 */ "...............",
/* 2 */ "...............",
/* 3 */ "...............",
/* 4 */ "...............",
/* 5 */ "...............",
/* 6 */ "..........p...f",
/* 7 */ ".......airier.l",
/* 8 */ "..........l...a",
/* 9 */ ".........feet.s",
/*10 */ "............o.h",
/*11 */ "..........mopey",
/*12 */ "..........e.e..",
/*13 */ ".........rated.",
/*14 */ "..........na..."};

/*
 * 1 = normal
 * 2 = DL
 * 3 = TL
 * 4 = DW
 * 5 = TW
 */
int multipliers[SIZE][SIZE] = {
        {1,1,1,5,1,1,3,1,3,1,1,5,1,1,1},
        {1,1,2,1,1,4,1,1,1,4,1,1,2,1,1},
        {1,2,1,1,2,1,1,1,1,1,2,1,1,2,1},
        {5,1,1,3,1,1,1,4,1,1,1,3,1,1,5},
        {1,1,2,1,1,1,2,1,2,1,1,1,2,1,1},
        {1,4,1,1,1,3,1,1,1,3,1,1,1,4,1},
        {3,1,1,1,2,1,1,1,1,1,2,1,1,1,3},
        {1,1,1,4,1,1,1,1,1,1,1,4,1,1,1},
        {3,1,1,1,2,1,1,1,1,1,2,1,1,1,3},
        {1,4,1,1,1,3,1,1,1,3,1,1,1,4,1},
        {1,1,2,1,1,1,2,1,2,1,1,1,2,1,1},
        {5,1,1,3,1,1,1,4,1,1,1,3,1,1,5},
        {1,2,1,1,2,1,1,1,1,1,2,1,1,2,1},
        {1,1,2,1,1,4,1,1,1,4,1,1,2,1,1},
        {1,1,1,5,1,1,3,1,3,1,1,5,1,1,1}};

int values[27] = {
        1, /* A */
        3, /* B */
        3, /* C */
        2, /* D */
        1, /* E */
        4, /* F */
        2, /* G */
        4, /* H */
        1, /* I */
        8, /* J */
        5, /* K */
        1, /* L */
        3, /* M */
        1, /* N */
        1, /* O */
        3, /* P */
        10,/* Q */
        1, /* R */
        1, /* S */
        1, /* T */
        1, /* U */
        4, /* V */
        4, /* W */
        8, /* X */
        4, /* Y */
        10,/* Z */
        0};/* BLANK */

int wordscore(char *word, int row, int col, bool across) {
        int wscore, wmult, lmult, lscore, mult;

        for (wscore = 0, wmult = 1; *word != '\0'; word++) {
                mult = multipliers[row][col];
                lmult = (!filled(row, col) && mult <= 3) ? mult : 1;
                wmult *= (!filled(row, col) && mult > 3) ? mult-2 : 1;
                lscore = values[*word - 'a'];
                wscore += lscore * lmult;
                if (across)
                        col++;
                else
                        row++;
        }
        return wscore * wmult;
}

void board_flip(void) {
        Board transpose;
        int row, col;

        for (row = 0; row < SIZE; row++) {
                for (col = 0; col < SIZE; col++)
                        transpose[row][col] = board[col][row];
        }
        for (row = 0; row < SIZE; row++) {
                for (col = 0; col < SIZE; col++)
                        board[row][col] = transpose[row][col];
        }
}

void replay_moves(FILE *out) {
        Word *wp;
        char *s;
        char saved[MAXWORD];
        int col, row;

        for (wp = legalwords; wp != NULL; wp = wp->next) {
                row = wp->row;
                col = wp->col;
                strncpy(saved, board[row], sizeof(saved));
                for (s = wp->letters; *s != '\0'; s++, col++) {
                        if (filled(row, col))
                                continue;
                        board[row][col] = *s;
                }
                board_output(out);
                printf("%d points\n", wp->score);
                printf("--------------------------------\n");
                strncpy(board[row], saved, sizeof(saved));
        }
}

void board_output(FILE *out) {
        int row;
        for (row = 0; row < SIZE; row++)
                fprintf(out, "%s\n", board[row]);
}

bool filled(int row, int col) {
        return board[row][col] >= 'a' && board[row][col] <= 'z';
}

void get_downword(int row, int col, char *word) {
        for (; filled(row, col); row++)
                *word++ = board[row][col];
        *word = '\0';
}

void get_acrossword(int row, int col, char *word) {
        for (; filled(row, col); col++)
                *word++ = board[row][col];
        *word = '\0';
}

void get_acrossword_left(int row, int col, char *word) {
        for (; rightof_tile(row, col); col--)
                ;
        get_acrossword(row, col, word);
}

static int downword_above_startrow(int row, int col) {
        for (; below_tile(row, col); row--)
                ;
        return row;
}

void get_downword_above(int row, int col, char *word) {
        row = downword_above_startrow(row, col);
        get_downword(row, col, word);
}

int downword_score_above(int row, int col, char *word) {
        row = downword_above_startrow(row, col);
        return wordscore(word, row, col, false);
}

int downword_score_below(int row, int col, char *word) {
        return wordscore(word, row+1, col, false);
}

void get_downword_below(int row, int col, char *word) {
        get_downword(row+1, col, word);
}

bool rightof_tile(int row, int col) {
        return col+1 < SIZE && filled(row, col+1);
}

bool leftof_tile(int row, int col) {
        return col-1 >= 0 && filled(row, col-1);
}

bool above_tile(int row, int col) {
        return row+1 < SIZE && filled(row+1, col);
}

bool below_tile(int row, int col) {
        return row-1 >= 0 && filled(row-1, col);
}

bool nextto_tile(int row, int col) {
        return rightof_tile(row, col)
                || leftof_tile(row, col)
                || below_tile(row, col)
                || above_tile(row, col);
}
