#ifndef BOARD_GAME_H_INCLUDED
#define BOARD_GAME_H_INCLUDED

typedef struct board_game_variables {
    short int rows;
    short int columns;
    short int win_count;
    short int gravity;
    short int bot;
} board_game_variables;

typedef struct board_game {
    short int rows;
    short int columns;
    short int win_count;
    short int gravity;
    short int bot;
    char winner;
    char** grid;
} board_game;

board_game initialize(short int, short int, short int, short int, short int);
board_game var_initialize(board_game_variables);
void end_game(board_game);

int play(board_game, short int, short int, char);
void clear_playground(board_game);
void display_playground(board_game);
void start_game(board_game);
int isWon(board_game, char);
short int* getBestPos(board_game, char);

#endif // BOARD_GAME_H_INCLUDED
