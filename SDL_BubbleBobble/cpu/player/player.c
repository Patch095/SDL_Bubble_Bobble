#include "player.h"
#include <stdio.h>
#include <stdlib.h>

player_t *insered_coin(int player_id){
    player_t *new_player = malloc(sizeof(player_t));
    if(!new_player){
        return NULL;
    }

    if(player_id == 0){
        new_player->player_name = "player1";
    }else if(player_id == 0){
        new_player->player_name = "player1";
    }

    new_player->life = 3;
    new_player->score = 0;

    return new_player;
}

void player_spawn(player_t *player, int start_x, int start_y){
    if(!player){
        return;
    }

    player->x = start_x;
    player->y = start_y;
}