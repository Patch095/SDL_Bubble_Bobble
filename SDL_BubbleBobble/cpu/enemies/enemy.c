#include "enemy.h"
#include <stdlib.h>

enemy_t *enemy_init(int enemy_id){
    enemy_t *new_enemy = malloc(sizeof(enemy_t));
    if(!new_enemy){
        return NULL;
    }

    new_enemy->type_id = enemy_id;
    if(enemy_id == 2){
        new_enemy->number_of_animation_frame = 4;
    }else{
        new_enemy->number_of_animation_frame = 2;
    }
    new_enemy->animation_frame = 0;

    new_enemy->direction = 0;
    new_enemy->jump_offset = 0;
    new_enemy->status = 0;
    new_enemy->special_action_chance = 0;
    new_enemy->absorbed_timer = 0;
    
    new_enemy->rect.w = SPRITE_WH;
    new_enemy->rect.h = SPRITE_WH;

    return new_enemy;
}