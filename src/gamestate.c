#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NLETTERS 27 /* a-z, and BLANK tile */
#define NTILES 100 /* number of tiles in the game */
#define EMPTY '.' /* the EMPTY square */
#define BOARDSIZE 15 /* board dimensions */
#define MAXWORD (BOARDSIZE+1) /* longest word plus null byte */

typedef int bool;
enum {true, false};

typedef struct GameState GameState;
typedef struct Player Player;
typedef struct Move Move;

struct Move {
        Player *p; /* player who made the move */
        char tiles[MAXWORD]; /* tiles to be placed */
        bool skip[BOARDSIZE]; /* squares to skip during placement */
        int x, y; /* (across) start location of move */
        Move *next; /* linked list next pointer */
};

struct GameState {
        char board[BOARDSIZE][BOARDSIZE]; /* array of chars */
        Move *moves; /* move history stack */
        char freetiles[NTILES]; /* shuffled array of tiles */
        int nexttile; /* index into freetiles array */
};

struct Player {
        int score; /* total score of player */
        unsigned int rack[NLETTERS]; /* counts of tiles in hand */
};

static void player_add_tiles(Player *p, char *tiles);
static void player_remove_tiles(Player *p, char *tiles);

static void gs_put_tiles(GameState *gs, char *tiles, bool skip[], int x, int y);
static void gs_remove_tiles(GameState *gs, char *tiles, bool skip[], int x, int y);
static void gs_put_tile(GameState *gs, char tile, int x, int y);
static char gs_get_tile(GameState *gs, int x, int y);
static char gs_request_tile(GameState *gs);
static char gs_putback_tile(GameState *gs);
static void gs_remove_tile(GameState *gs, int x, int y);
static void gs_refill_rack(GameState *gs, Move *move);
static void gs_rewind_rack(GameState *gs, Move *move);

static void move_push(Move **moves, Move *move);
static Move *move_pop(Move **moves);

Move *move_new(Player *p, GameState *gs, char *word, int x, int y) {
        int i;
        Move *m = malloc(sizeof(Move));
        if (!m)
                return NULL;
        m->p = p;
        m->x = x;
        m->y = y;
        memset(m->skip, false, BOARDSIZE);
        for (i = 0; *word; word++, x++) {
                char tile = gs_get_tile(gs, x, y);
                if (tile >= 'a' && tile <= 'z')
                        m->skip[x] = true;
                else
                        m->tiles[i++] = *word;
        }
        m->tiles[i] = '\0';
        return m;
}

void player_init(Player *p) {
        p->score = 0;
        memset(p->rack, 0, sizeof(unsigned int) * NLETTERS);
}

/* http://en.wikipedia.org/wiki/Fisher-Yates_shuffle */
static void shuffle(char a[], unsigned int size) {
        int i;
        for (i = size-1; i > 0; i--) {
                int j = random() % i;
                char tmp = a[j];
                a[j] = a[i];
                a[i] = tmp;
        }
}

static void freetiles_init(char freetiles[]) {
        int tile_distribution[] = {9, 2, 2, 4, 12, 2, 3, 2, 9, 1, 1, 4, 2, 6,
                8, 2, 1, 6, 4, 6, 4, 2, 2,1, 2, 1, 2};
        char c;
        int i;
        for (i = 0, c = 'a'; c <= 'z'; c++) {
                while (tile_distribution[c-'a']--)
                        freetiles[i++] = c;
        }
}

void gs_init(GameState *gs) {
        int i, j;
        for (i = 0; i < BOARDSIZE; i++) {
                for (j = 0; j < BOARDSIZE; j++)
                        gs->board[i][j] = EMPTY;
        }
        gs->moves = NULL;
        gs->nexttile = 0;
        freetiles_init(gs->freetiles);
        shuffle(gs->freetiles, NTILES);
}

static void print_freetiles(GameState *gs) {
        int i;
        for (i = 0; i < NTILES; i++)
                printf("%c ", gs->freetiles[i]);
}

static void print_board(GameState *gs) {
        int x, y;
        for (x = 0; x < BOARDSIZE; x++) {
                for (y = 0; y < BOARDSIZE; y++)
                        printf("%c", gs_get_tile(gs, x, y));
                printf("\n");
        }
}

static void print_rack(Player *p) {
        int i;
        for (i = 0; i < NLETTERS; i++)
                printf("%c: %d\n", 'a'+i, p->rack[i]);
}

/* Make a move, modifying the game state and player's rack */
void gs_step(GameState *gs, Move *move) { 
        move_push(&gs->moves, move);
        gs_put_tiles(gs, move->tiles, move->skip, move->x, move->y);
        player_remove_tiles(move->p, move->tiles);
        gs_refill_rack(gs, move);
}

/* Revert game and players to previous state */
void gs_rewind(GameState *gs) {
        Move *undo = move_pop(&gs->moves);
        gs_remove_tiles(gs, undo->tiles, undo->skip, undo->x, undo->y);
        player_add_tiles(undo->p, undo->tiles);
        gs_rewind_rack(gs, undo);
        free(undo);
}

/* Request next random tile from game state */
static char gs_request_tile(GameState *gs) {
        return gs->freetiles[gs->nexttile++];
}

/* Put back last requested tile, returning the tile returned */
static char gs_putback_tile(GameState *gs) {
        return gs->freetiles[--gs->nexttile];
}

/* Refill the rack of the player who made the last move */
static void gs_refill_rack(GameState *gs, Move *move) {
        int count;
        for (count = strlen(move->tiles); count > 0; count--)
                move->p->rack[gs_request_tile(gs)-'a']++;
}

/* Put back tiles taken for the last move */
static void gs_rewind_rack(GameState *gs, Move *move) {
        int count;
        for (count = strlen(move->tiles); count > 0; count--)
                move->p->rack[gs_putback_tile(gs)-'a']--;
}

/* Add tiles to player's rack */
static void player_add_tiles(Player *p, char *tiles) {
        for (; *tiles; tiles++)
                p->rack[*tiles-'a']++;
}

/* Remove tiles from player's rack */
static void player_remove_tiles(Player *p, char *tiles) {
        for (; *tiles; tiles++)
                p->rack[*tiles-'a']--;
}

/* Add tiles to board */
static void gs_put_tiles(GameState *gs, char *tiles, bool skip[], int x, int y) {
        for (; *tiles; x++) {
                if (skip[x])
                        continue;
                gs_put_tile(gs, *tiles, x, y);
                tiles++;
        }
}

/* Remove tiles from board */
static void gs_remove_tiles(GameState *gs, char *tiles, bool skip[], int x, int y) {
        for (; *tiles; x++) {
                if (skip[x])
                        continue;
                gs_remove_tile(gs, x, y);
                tiles++;
        }
}

/* Put tile on board located at x and y */
static void gs_put_tile(GameState *gs, char tile, int x, int y) {
        gs->board[y][x] = tile;
}

/* Put tile on board located at x and y */
static char gs_get_tile(GameState *gs, int x, int y) {
        return gs->board[y][x];
}

/* Replace tile at x and y with an EMPTY square */
static void gs_remove_tile(GameState *gs, int x, int y) {
        gs_put_tile(gs, EMPTY, x, y);
}

/* Append move to front of moves */
static void move_push(Move **moves, Move *move) {
        move->next = *moves;
        *moves = move;
}

/* Remove and return top move from move stack */
static Move *move_pop(Move **moves) {
        Move *move = *moves;
        *moves = move->next;
        return move;
}

int main(void) {
        GameState gs;
        Move *m;
        Player p;

        gs_init(&gs);
        shuffle(gs.freetiles, NTILES);

        player_init(&p);
        player_add_tiles(&p, "hello");
        print_rack(&p);

        m = move_new(&p, &gs, "hello", 4, 5);
        gs_step(&gs, m);
        print_board(&gs);
        print_rack(&p);

        gs_rewind(&gs);
        print_board(&gs);
        print_rack(&p);

        return 0;
}
