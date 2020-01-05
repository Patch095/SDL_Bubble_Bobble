#include "player.h"
#include <stdio.h>
#include <stdlib.h>

player_t *player_init(int player_id){
    player_t *new_player = malloc(sizeof(player_t));
    if(!new_player){
        return NULL;
    }

    new_player->player_id = player_id;
    if(player_id == 0){
        new_player->direction = 1;
    }else if(player_id == 1){
        new_player->direction = -1;
    }

    new_player->score = 0;

    new_player->is_shooting = 0;
    new_player->jumping_state = 0;
    new_player->jump_offset = 0;
    
    new_player->rect.x = 0;
    new_player->rect.y = 0;
    new_player->rect.w = 0;
    new_player->rect.h = 0;

    insered_coin(new_player);

    return new_player;
}

void insered_coin(player_t *player){
    player->life = PLAYER_SPAWN_LIFE;
    player->is_alive = player->life > 0;
}

void player_spawn(player_t *player, int start_x, int start_y){
    if(!player){
        return;
    }

    player->direction = player->player_id;

    player->rect.x = start_x;
    player->rect.y = start_y;

    player->animation_frame = 0;
}

void add_a_life(player_t *player){
    if(player->life < PLAYER_MAX_LIFE){
        player->life++;
    }
}
void on_player_death(player_t *player){
    player->life--; 
    player->is_alive = player->life > 0;
}

/*
void player_wall_collision(player_t *player, SDL_Rect player_rect, SDL_Rect collider){
    if(collider.y < player_rect.y && collider.y + collider.w > player_rect.y){
        if(collider.x <= player_rect.x + player_rect.w && collider.x + collider.w >= player_rect.x){
            if(player->x > (collider.x + (collider.w/2))){
                player->x = collider.x + collider.w + 1;
            }else if(player->x < (collider.x + (collider.w/2))){
                player->x = collider.x - player_rect.w - 1;
            }    
        }
        //else if(){
        //    player->x = collider.x + collider.w;
        //}
    }
}

int can_collide(int new_x, SDL_Rect player_rect, SDL_Rect collider){
    if(collider.x <= new_x.x && collider.x + collider.w >= new_x){
        return 1;
    }
    return 0;
}*/