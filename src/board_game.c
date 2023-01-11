#include "board_game.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

board_game initialize(short int rows, short int columns, short int win_count, short int gravity) {
    short int i, j;
    board_game playground;
    playground.rows = rows;
    playground.columns = columns;
    playground.win_count = win_count;
    playground.gravity = gravity;

    playground.grid = (char**) malloc(columns * sizeof(void*));
    if (!playground.grid) {
        exit(-1);
    }

    for (i = 0; i < columns; i++) {
        playground.grid[i] = (char*) malloc(rows);
        if (!playground.grid[i]) {
            for (j = 0; j < i; j++)
                free(playground.grid[j]);
            free(playground.grid);
        }
    }

    for (i = 0; i < columns; i++) {
        for (j = 0; j < rows; j++)
            playground.grid[i][j] = 32;
    }

    return playground;
}

board_game var_initialize(board_game_variables var) {
    short int rows = var.rows ? var.rows : 10;
    short int columns = var.columns ? var.rows : 10;
    if (rows <= 0 || columns <= 0)
        exit(-2);
    short int win_count;
    if (!var.win_count) {
        if (rows < columns)
            win_count = rows;
        else
            win_count = columns;
    } else
        win_count = var.win_count;
    short int gravity = var.gravity ? var.gravity : 0;

    return initialize(rows, columns, win_count, gravity);
}

void end_game(board_game playground) {
    short int i;
    for (i = 0; i < playground.columns; i++) {
        free(playground.grid[i]);
    }
    free(playground.grid);
}

void display_playground(board_game playground) {
    short int i, j;
    for (i = 0; i < playground.rows; i++) {
        for (j = 0; j < playground.columns; j++)
            printf("|%2c", playground.grid[j][i]);
        if (!playground.gravity)
            printf("|%2hd\n", i+1);
        else printf("|\n");
    }
    for (i = 1; i <= playground.columns; i++)
        printf("|%2hd", i);
    printf("|\n");
}

void clear_playground(board_game playground) {
    short int i, j;
    for (i = 0; i < playground.rows; i++) {
        for (j = 0; j < playground.columns; j++)
            playground.grid[i][j] = 32;
    }
}

int isWon(board_game playground, char player) {
    short int i, j, k;
    for (i = 0; i <= playground.rows - playground.win_count; i++) {
        for (j = 0; j <= playground.columns - playground.win_count; j++) {
            for (k = 0; k < playground.win_count; k++) {
                if (playground.grid[j+k][i] != player)
                    break;
                if (k ==  playground.win_count-1)
                    return 1;
            }
            for (k = 0; k < playground.win_count; k++) {
                if (playground.grid[j][i+k] != player)
                    break;
                if (k == playground.win_count-1)
                    return 1;
            }
            for (k = 0; k < playground.win_count; k++) {
                if (playground.grid[j+k][i+k] != player)
                    break;
                if (k == playground.win_count-1)
                    return 1;
            }
        }
    }
    return 0;
}

int play(board_game playground, short int played_column, short int played_row, char player) {
    if (played_column < 0 || played_column >= playground.columns)
            return 0;
    if (playground.gravity) {
        short int j;
        for (j = playground.rows-1; j >= 0; j--) {
            if (playground.grid[played_column][j] == 32) {
                playground.grid[played_column][j] = player;
                return 1;
            }
        }
    } else {
        if (played_row < 0 || played_row >= playground.rows || playground.grid[played_column][played_row] != 32)
            return 0;
        playground.grid[played_column][played_row] = player;
    }
    return 0;
}

void start_game(board_game playground) {
    char player;
    short int err;
    short int played_column = 0, played_row = 0;
    srand(time(NULL));
    if (rand() % 2)
        player = 'x';
    else
        player = 'o';
    do {
        printf("Voici le plateau de jeu :\n");
        if (played_column)
            printf("Le dernier joueur a jou%c %c la colonne %hd\n", 130, 133, played_column);
        printf("C'est au tour du joueur %c de jouer\n\n", player);
        display_playground(playground);
        printf("\nSaisissez le num%cro de la colonne o%c vous souhaitez jouer : ", 130, 151);
        scanf("%hd", &played_column);
        if (!playground.gravity) {
            printf("Saisissez le num%cro de la ligne o%c vous souhaitez jouer : ", 130, 151);
            scanf("%hd", &played_row);
        }
        err = play(playground, played_column-1, played_row-1, player);
        if (err) {
            if (player == 'x')
                player = 'o';
            else
                player = 'x';
        }
        system("cls");
    } while(!isWon(playground, player));

    if (isWon(playground, player))
        printf("Le joueur %c a gagn%c !", player, 130);
    printf("\nSouhaitez-vous rejouer ? ");
    short int choice;
    scanf("%hd", &choice);
    if (choice) {
        system("cls");
        clear_playground(playground);
        start_game(playground);
    }
    end_game(playground);
}
