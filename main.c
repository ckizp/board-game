#include <stdio.h>
#include <stdlib.h>
#include "board_game.h"
#define initialize(...) var_initialize((board_game_variables){__VA_ARGS__})

int main()
{
    board_game ps4 = initialize(10, 10, 4, 0, 1); // 10 colonnes, 8 lignes, 4 pions d'affilée pour victoire, et gravité activée
    start_game(ps4);

    return 0;
}
