#include "enemy.h"
#include <stdlib.h>

enemy_t *enemy_init(int enemy_id){
    enemy_t *new_enemy = malloc(sizeof(enemy_t));
    if(!new_enemy){
        return NULL;
    }

    bullet_t *new_bullet = NULL;

    new_enemy->type_id = enemy_id;
    if(enemy_id == 2){
        new_enemy->number_of_animation_frame = 4;
    }else if(enemy_id == 3){
        new_enemy->number_of_animation_frame = 5;
        //might-a bullet
        new_bullet = malloc(sizeof(bullet_t));
        if(!new_bullet){
            return NULL;
        }
        new_bullet->speed = 2;
        new_bullet->owner_id = 3;
        new_bullet->status = 0;
        new_bullet->rect.w = MIGHT_BULLET_W;
        new_bullet->rect.h = MIGHT_BULLET_H;
        new_bullet->number_of_animation_frame = 4;
        new_bullet->animation_frame = 0;
    }
    else{
        new_enemy->number_of_animation_frame = 2;
    }
    new_enemy->animation_frame = 0;

    new_enemy->bullet = new_bullet;

    new_enemy->direction = 0;
    new_enemy->jump_offset = 0;
    new_enemy->status = 0;
    new_enemy->special_action_chance = 0;
    new_enemy->absorbed_timer = 0;
    
    new_enemy->rect.w = SPRITE_WH;
    new_enemy->rect.h = SPRITE_WH;

    return new_enemy;
}

int enemy_update(enemy_t *enemy, int rand_chance){
    if(enemy->status != 1){
        return 0;
    }

    int use_special = enemy->special_action_chance + rand_chance;    
    if(use_special >= 200){
        switch(enemy->type_id){
            //zen-chan specials
            case 2:
                if(rand_chance%2 == 0){
                    return 1;
                }else if(rand_chance%2 == 1){
                    enemy->direction *= -1;
                    enemy->special_action_chance = 0;
                    return 0;
                }
                break;
            //migt-a specials
            case 3:
                if(enemy->bullet->status == 0){
                    enemy->bullet->direction_x = enemy->direction;
                    int bullet_x = enemy->rect.x;
                    if(enemy->direction == 1){
                        bullet_x += enemy->rect.w;
                    }
                    enemy->bullet->rect.x = bullet_x + enemy->direction;
                    enemy->bullet->rect.y = enemy->rect.y;
                    enemy->bullet->animation_frame = 0;
                    enemy->bullet->status = 1;

                    enemy->direction *= -1;
                    enemy->special_action_chance = 0;
                    return 0;
                }
        }
    }

    enemy->special_action_chance++;
    return 0;
}