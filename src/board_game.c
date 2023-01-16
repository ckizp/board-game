#include "board_game.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

board_game initialize(short int rows, short int columns, short int win_count, short int gravity, short int bot) {
    short int i, j;
    board_game playground;
    playground.rows = rows;
    playground.columns = columns;
    playground.win_count = win_count;
    playground.gravity = gravity;
    playground.bot = bot;

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
    short int bot = var.bot ? var.bot : 0;

    return initialize(rows, columns, win_count, gravity, bot);
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
    short int i, j, count;

    for (int i = 0; i < playground.rows; i++) {
        for (int j = 0; j <= playground.columns - playground.win_count; j++) {
            count = 0;
            for (int k = 0; k < playground.win_count; k++) {
                if (playground.grid[i][j+k] == player) {
                    count++;
                }
                if (count == playground.win_count) {
                    return 1;
                }
            }
        }
    }
    for (int i = 0; i <= playground.rows - playground.win_count; i++) {
        for (int j = 0; j < playground.columns; j++) {
            count = 0;
            for (int k = 0; k < playground.win_count; k++) {
                if (playground.grid[i+k][j] == player) {
                    count++;
                }
                if (count == playground.win_count) {
                    return 1;
                }
            }
        }
    }
    for (int i = 0; i <= playground.rows - playground.win_count; i++) {
        for (int j = 0; j <= playground.columns - playground.win_count; j++) {
            count = 0;
            for (int k = 0; k < playground.win_count; k++) {
                if (playground.grid[i+k][j+k] == player) {
                    count++;
                }
                if (count == playground.win_count) {
                    return 1;
                }
            }
        }
    }
    for (int i = 0; i <= playground.rows - playground.win_count; i++) {
        for (int j = playground.win_count - 1; j < playground.columns; j++) {
            count = 0;
            for (int k = 0; k < playground.win_count; k++) {
                if (playground.grid[i+k][j-k] == player) {
                    count++;
                }
                if (count == playground.win_count) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

short int* getBestPos(board_game playground, char player) {
    short int i, j, k, count;
    short int* pos = malloc(2 * sizeof(short int));
    char enemy;

    if (player == 'x')
        enemy = 'o';
    else
        enemy = 'x';

    for (i = 0; i < playground.rows; i++) {
        for (j = 0; j <= playground.columns - playground.win_count; j++) {
            count = 0;
            for (k = 0; k < playground.win_count; k++) {
                if (playground.grid[j+k][i] == player)
                    count++;
            }
            if (count == playground.win_count-1) {
                for (k = 0; k < playground.win_count; k++) {
                    if (playground.grid[j+k][i] == enemy) {
                        pos[0] = j+k;
                        pos[1] = i;
                        return pos;
                    }
                }
            } else if (count == 1) {
                for (k = 0; k < playground.win_count; k++) {
                    if (playground.grid[j+k][i] == 32) {
                        pos[0] = j+k;
                        pos[1] = i;
                        return pos;
                    }
                }
            }
        }
    }
    for (i = 0; i <= playground.rows - playground.win_count; i++) {
        for (j = 0; j < playground.columns; j++) {
            count = 0;
            for (k = 0; k < playground.win_count; k++) {
                if (playground.grid[j][i+k] == player)
                    count++;
            }
            if (count == playground.win_count-1) {
                for (k = 0; k < playground.win_count; k++) {
                    if (playground.grid[j][i+k] == enemy) {
                        pos[0] = j;
                        pos[1] = i+k;
                        return pos;
                    }
                }
            } else if (count == 1) {
                for (k = 0; k < playground.win_count; k++) {
                    if (playground.grid[j][i+k] == 32) {
                        pos[0] = j;
                        pos[1] = i+k;
                        return pos;
                    }
                }
            }
        }
    }
    for (i = 0; i <= playground.rows - playground.win_count; i++) {
        for (j = 0; j <= playground.columns - playground.win_count; j++) {
            count = 0;
            for (k = 0; k < playground.win_count; k++) {
                if (playground.grid[j+k][i+k] == player)
                    count++;
            }
            if (count == playground.win_count-1) {
                for (k = 0; k < playground.win_count; k++) {
                    if (playground.grid[j+k][i+k] == enemy) {
                        pos[0] = j+k;
                        pos[1] = i+k;
                        return pos;
                    }
                }
            } else if (count == 1) {
                for (k = 0; k < playground.win_count; k++) {
                    if (playground.grid[j+k][i+k] == 32) {
                        pos[0] = j+k;
                        pos[1] = i+k;
                        return pos;
                    }
                }
            }
        }
    }
    for (i = 0; i <= playground.rows - playground.win_count; i++) {
        for (j = playground.win_count - 1; j < playground.columns; j++) {
            count = 0;
            for (k = 0; k < playground.win_count; k++) {
                if (playground.grid[i+k][j-k] == player)
                    count++;
            }
            if (count == playground.win_count-1) {
                for (k = 0; k < playground.win_count; k++) {
                    if (playground.grid[j+k][i-k] == enemy) {
                        pos[0] = j+k;
                        pos[1] = i+k;
                        return pos;
                    }
                }
            } else if (count == 1) {
                for (k = 0; k < playground.win_count; k++) {
                    if (playground.grid[j+k][i-k] == 32) {
                        pos[0] = j+k;
                        pos[1] = i+k;
                        return pos;
                    }
                }
            }
        }
    }

    srand(time(NULL));
    pos[0] = rand() % playground.columns;
    pos[1] = rand() % playground.rows;
    return pos;
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
        return 1;
    }
    return 0;
}

void start_game(board_game playground) {
    char player;
    char enemy;
    short int* pos;
    if (!pos)
        exit(-3);
    short int err;
    short int played_column = 0, played_row = 0;
    srand(time(NULL));
    if (rand() % 2)
        player = 'x';
    else
        player = 'o';
    if (playground.bot) {
        if (player == 'x')
            enemy = 'o';
        else
            enemy = 'x';
    }
    do {
        printf("Voici le plateau de jeu :\n");
        if (played_column > 0 && played_column <=10)
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
        if (!err) {
                system("cls");
            continue;
        }

        if (isWon(playground, player))
            break;

        if (!playground.bot) {
            if (err) {
                if (player == 'x')
                    player = 'o';
                else
                    player = 'x';
            }
        } else {
            pos = getBestPos(playground, enemy);
            play(playground, pos[1], pos[0], enemy);
            free(pos);
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
    free(pos);
    end_game(playground);
}
