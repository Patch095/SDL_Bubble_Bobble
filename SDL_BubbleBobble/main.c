#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 720
#define JUMP_START_OFFSET 56
#define MAX_PLAYER_BUBBLE 20

#include "cpu/map/map.h"
#include "cpu/player/player.h"
#include "cpu/bubble/bubble.h"
#include "cpu/enemies/enemy.h"

#define PLAYER_ONE_LEFT_00 "textures/p1_left_0.png"
#define PLAYER_ONE_LEFT_01 "textures/p1_left_1.png"
#define PLAYER_ONE_LEFT_SHOOT_00 "textures/p1_left_shoot_0.png"
#define PLAYER_ONE_LEFT_SHOOT_01 "textures/p1_left_shoot_1.png"
#define PLAYER_ONE_LEFT_JUMP_00 "textures/p1_left_jump_0.png"
#define PLAYER_ONE_LEFT_JUMP_01 "textures/p1_left_jump_1.png"
#define PLAYER_ONE_LEFT_FALL_00 "textures/p1_left_fall_0.png"
#define PLAYER_ONE_LEFT_FALL_01 "textures/p1_left_fall_1.png"

#define PLAYER_ONE_RIGHT_00 "textures/p1_right_0.png"
#define PLAYER_ONE_RIGHT_01 "textures/p1_right_1.png"
#define PLAYER_ONE_RIGHT_SHOOT_00 "textures/p1_right_shoot_0.png"
#define PLAYER_ONE_RIGHT_SHOOT_01 "textures/p1_right_shoot_1.png"
#define PLAYER_ONE_RIGHT_JUMP_00 "textures/p1_right_jump_0.png"
#define PLAYER_ONE_RIGHT_JUMP_01 "textures/p1_right_jump_1.png"
#define PLAYER_ONE_RIGHT_FALL_00 "textures/p1_right_fall_0.png"
#define PLAYER_ONE_RIGHT_FALL_01 "textures/p1_right_fall_1.png"

#define PLAYER_ONE_BUBBLE_EMPTY "textures/p1_bubble_empty.png"
#define PLAYER_ONE_BUBBLE_SHOOT "textures/p1_bubble_shoot.png"

#define PLAYER_ONE_BUBBLE_ZEN_00 "textures/p1_bubble_zen_0.png"
#define PLAYER_ONE_BUBBLE_ZEN_01 "textures/p1_bubble_zen_1.png"
#define PLAYER_ONE_BUBBLE_ZEN_02 "textures/p1_bubble_zen_2.png"

#define PLAYER_ONE_BUBBLE_MIGHTA_00 "textures/p1_bubble_mighta_0.png"
#define PLAYER_ONE_BUBBLE_MIGHTA_01 "textures/p1_bubble_mighta_1.png"
#define PLAYER_ONE_BUBBLE_MIGHTA_02 "textures/p1_bubble_mighta_2.png"

#define ENEMY_ZEN_CHAN_LEFT_00 "textures/zen_left_0.png"
#define ENEMY_ZEN_CHAN_LEFT_01 "textures/zen_left_1.png"
#define ENEMY_ZEN_CHAN_LEFT_02 "textures/zen_left_2.png"
#define ENEMY_ZEN_CHAN_LEFT_03 "textures/zen_left_3.png"

#define ENEMY_ZEN_CHAN_RIGHT_00 "textures/zen_right_0.png"
#define ENEMY_ZEN_CHAN_RIGHT_01 "textures/zen_right_1.png"
#define ENEMY_ZEN_CHAN_RIGHT_02 "textures/zen_right_2.png"
#define ENEMY_ZEN_CHAN_RIGHT_03 "textures/zen_right_3.png"

#define ENEMY_MIGTH_A_LEFT_00 "textures/mighta_left_0.png"
#define ENEMY_MIGTH_A_LEFT_01 "textures/mighta_left_1.png"
#define ENEMY_MIGTH_A_LEFT_02 "textures/mighta_left_2.png"
#define ENEMY_MIGTH_A_LEFT_03 "textures/mighta_left_3.png"
#define ENEMY_MIGTH_A_LEFT_04 "textures/mighta_left_4.png"

#define ENEMY_MIGTH_A_RIGHT_00 "textures/mighta_right_0.png"
#define ENEMY_MIGTH_A_RIGHT_01 "textures/mighta_right_1.png"
#define ENEMY_MIGTH_A_RIGHT_02 "textures/mighta_right_2.png"
#define ENEMY_MIGTH_A_RIGHT_03 "textures/mighta_right_3.png"
#define ENEMY_MIGTH_A_RIGHT_04 "textures/mighta_right_4.png"

#define BULLET_MIGTH_A_00 "textures/mighta_bullet_0.png"
#define BULLET_MIGTH_A_01 "textures/mighta_bullet_1.png"
#define BULLET_MIGTH_A_02 "textures/mighta_bullet_2.png"
#define BULLET_MIGTH_A_03 "textures/mighta_bullet_3.png"

#define PLAYER_ONE_LIFE "textures/p1_life_token.png"

#define MAP_TILE_WALL "textures/wall.png"

#define FILE_PATH "cpu/map/level_"

enemy_t* spawn_enemies_on_level(level_t *level, int level_number){
    enemy_t *level_enemies = malloc(sizeof(enemy_t) * level->enemies_in_level);
      
    int dir = (level_number + 1) % 2;
    if(dir == 0){
        dir = -1;
    }

    int max_y = MAP_TILES_Y;
    int max_x = MAP_TILES_X;
    int enemy_index = 0;
    for(int y = 0; y < max_y; y++){
        for(int x = 0; x < max_x; x++){
            enemy_t *new_enemy = NULL;
            if(level->level_layout[y][x] == 4){
                new_enemy = enemy_init(2);
            }else if(level->level_layout[y][x] == 5){
                new_enemy = enemy_init(3);
            }
            if(new_enemy){
                //return NULL;
                
                new_enemy->rect.x = x * TILE_W;
                new_enemy->rect.y = (y * TILE_H) - 1;

                //new_enemy->direction = dir;
                new_enemy->direction = -1;

                level_enemies[enemy_index] = *new_enemy;
                level->enemies_alive++;

                enemy_index++;
                if(enemy_index >= level->enemies_in_level){
                    return level_enemies;                    
                }
            }
        }
    }
    return level_enemies;
}

level_t* load_level_from_file(int level_number, int *p1_spawn_pos_x, int *p1_spawn_pos_y){
    //int first_digit = level_number % 10;
    //int second_digit = level_number / 10;
    level_t *level;
    switch(level_number){
        case 01:
            level = load_level("cpu/map/level_01.h");
            break;
        case 02:
            level = load_level("cpu/map/level_02.h");
            break;
        case 03:
            level = load_level("cpu/map/level_03.h");
            break;
        case 04:
            level = load_level("cpu/map/level_04.h");
            break;        
        case 05:
            level = load_level("cpu/map/level_05.h");
            break;
        default:
            break;
    }
    if(!level){
        SDL_Log("error load level %d", level_number);
    }
    SDL_Log("Loaded level: %s\n", level->level_name);

    int max_y = MAP_TILES_Y;
    int max_x = MAP_TILES_X;
    for(int y = 0; y < max_y; y++){
        for(int x = 0; x < max_x; x++){  
            if(level->level_layout[y][x] == 2){
                *p1_spawn_pos_x = x;
                *p1_spawn_pos_y = y;
            }
        }
    }
    return level;
}

void spawn_player_in_level(player_t *player, int spawn_tile_x, int spawn_tile_y){
    if(!player || player->is_alive == 0){
        return;
    }

    if(player->player_id == 0){
        player->direction = 1;
    }else if(player->player_id == 1){
        player->direction = -1;
    }

    int spawn_pos_x = spawn_tile_x * TILE_W;
    int spawn_pos_y = (spawn_tile_y * TILE_H) + (TILE_H - PLAYER_WH) - 1;

    player->jump_offset = 0;

    player->rect.x = spawn_pos_x;
    player->rect.y = spawn_pos_y;
}

bubble_t *get_bubble(int owner_id, bubble_t *bubbles){   
    int first_shoot_bubble_timer = BUBBLE_DURATION_TIMER + 1;
    int bubble_id = -1;
    if(bubbles[0].owner_id != owner_id){
        return NULL;
    }

    for(int i = 0; i < MAX_PLAYER_BUBBLE; i++){       
        bubble_t *bubble = &bubbles[i];
        if (bubble->duration < first_shoot_bubble_timer){
            bubble_id = i;
            first_shoot_bubble_timer = bubble->duration;
        }
        if(bubble->status == 0){
            return bubble;
        }
    }
    reset_bubble(&bubbles[bubble_id]);
    return &bubbles[bubble_id];
}

void player_shoot(player_t *player, bubble_t *bubble){
    if(player->is_alive == 0){
        return;
    }
    if(player->player_id != bubble->owner_id){
        return;
    }
    int start_x = player->rect.x + (player->rect.w * player->direction);
    int start_y = player->rect.y;// + 2;// - (player->rect.h + 2);

    bubble->rect.x = start_x;
    bubble->rect.y = start_y;
    bubble->direction = player->direction;
    bubble->shoot_distance_offset = SHOOT_OFFSET;

    bubble->duration = BUBBLE_DURATION_TIMER;
    bubble->status = 1;
}

void bubble_wall_collision(level_t *level, bubble_t *bubble){
    if(!bubble || bubble->status == 0){
        return;
    }

    int next_x = bubble->rect.x + (2 * bubble->direction);
    if(bubble->direction == 1){
        next_x += bubble->rect.w;
    }

    if(bubble->status == 1){
        if(get_tile_value(level, bubble->rect.y, next_x) == 1 && get_tile_value(level, bubble->rect.y + bubble->rect.h, next_x) == 1){
            bubble->status = 0;
        }
    }

    if(bubble->rect.x < TILE_W){
        bubble->rect.x = TILE_W + 1;
    }
    if(bubble->rect.x + bubble->rect.w >= (TILE_W * 15)){
        bubble->rect.x  = (TILE_W * 15) - 1 - bubble->rect.w ;
    }
}

int rect_collision(SDL_Rect rect_0, SDL_Rect rect_1){
    int has_collide = 0;    //0 = no    1 = yes

    if(rect_0.y >= rect_1.y && rect_0.y <= rect_1.y + rect_1.h){
        if(rect_0.x >= rect_1.x && rect_0.x <= rect_1.x + rect_1.w){
            has_collide = 1;
        }
    }  
    if(rect_0.y + rect_0.h >= rect_1.y && rect_0.y + rect_0.h <= rect_1.y + rect_1.h){
        if(rect_0.x + rect_0.w >= rect_1.x && rect_0.x + rect_0.w <= rect_1.x+ rect_1.w){
            has_collide = 1;
        }
    }

    return has_collide;
}

int apply_gravity(level_t *level, SDL_Rect rect){
    int update_y = rect.y + rect.h + 1;

    if(get_tile_value(level, update_y, rect.x) == -1){
        return 2;
    }

    if(get_tile_value(level, update_y, rect.x) != 1 && get_tile_value(level, update_y, rect.x + rect.w) != 1){
        return 1;
    }

    return 0;
}

void enemy_absorb(bubble_t *bubble, enemy_t *enemy, int enemy_id){
    if(!bubble | !enemy | bubble->status != 1){
        return;
    }

    enemy->status = 2;
    enemy->absorbed_timer = ABSORBE_DURATION_TIMER;
    bubble->status = 3;
    bubble->absorbed_type = enemy->type_id;
    bubble->absorbed_id = enemy_id;
    bubble->duration = ABSORBE_DURATION_TIMER;
}
void release_enemy(bubble_t *bubble, enemy_t *enemy_in_level, int enemy_numbers){
    if(bubble->status != 3){
        return;
    }
    if(bubble->absorbed_id < 0 || bubble->absorbed_id >= enemy_numbers){
        return;
    }

    enemy_t *enemy_to_release = &enemy_in_level[bubble->absorbed_id];
    if(!enemy_to_release || enemy_to_release->type_id != bubble->absorbed_type){
        return;
    }

    enemy_to_release->rect.x = bubble->rect.x;
    enemy_to_release->rect.y = bubble->rect.y;
    enemy_to_release->status = 1;

    return;
}

int main(int argc, char **argv){
    srand(time(NULL));
    int ret = 0;

//INIT SDL
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return ret;
    }
    window = SDL_CreateWindow("Bubble Bobble", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window){
        SDL_Log("Unable to open window: %s", SDL_GetError());
        goto quit;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer){
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        goto quit;
    }

//LOAD TEXTURES
    //p1 textures
    int p1_width, p1_height, p1_comp;
    
    unsigned char *image_p1_right_00 = stbi_load(PLAYER_ONE_RIGHT_00, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_right_01 = stbi_load(PLAYER_ONE_RIGHT_01, &p1_width, &p1_height, &p1_comp, 4);
    
    unsigned char *image_p1_left_00 = stbi_load(PLAYER_ONE_LEFT_00, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_left_01 = stbi_load(PLAYER_ONE_LEFT_01, &p1_width, &p1_height, &p1_comp, 4);
    
    unsigned char *image_p1_left_shoot_00 = stbi_load(PLAYER_ONE_LEFT_SHOOT_00, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_left_shoot_01 = stbi_load(PLAYER_ONE_LEFT_SHOOT_01, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_right_shoot_00 = stbi_load(PLAYER_ONE_RIGHT_SHOOT_00, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_right_shoot_01 = stbi_load(PLAYER_ONE_RIGHT_SHOOT_01, &p1_width, &p1_height, &p1_comp, 4);

    unsigned char *image_p1_left_jump_00 = stbi_load(PLAYER_ONE_LEFT_JUMP_00, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_left_jump_01 = stbi_load(PLAYER_ONE_LEFT_JUMP_01, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_right_jump_00 = stbi_load(PLAYER_ONE_RIGHT_JUMP_00, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_right_jump_01 = stbi_load(PLAYER_ONE_RIGHT_JUMP_01, &p1_width, &p1_height, &p1_comp, 4);

    unsigned char *image_p1_left_fall_00 = stbi_load(PLAYER_ONE_LEFT_FALL_00, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_left_fall_01 = stbi_load(PLAYER_ONE_LEFT_FALL_01, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_right_fall_00 = stbi_load(PLAYER_ONE_RIGHT_FALL_00, &p1_width, &p1_height, &p1_comp, 4);
    unsigned char *image_p1_right_fall_01 = stbi_load(PLAYER_ONE_RIGHT_FALL_01, &p1_width, &p1_height, &p1_comp, 4);

    if(!image_p1_right_00 | !image_p1_right_01 | !image_p1_left_00 | !image_p1_left_01 | !image_p1_left_shoot_00  | !image_p1_left_shoot_01 | !image_p1_right_shoot_00 | !image_p1_right_shoot_01 | !image_p1_left_jump_00  | !image_p1_left_jump_01 | !image_p1_right_jump_00 | !image_p1_right_jump_01 |!image_p1_left_fall_00 |!image_p1_left_fall_01 |!image_p1_right_fall_00 |!image_p1_right_fall_01){
        SDL_Log("Unable to load image: %s", SDL_GetError());
        goto quit;
    }

    SDL_Texture *p1_right_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_right_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    
    SDL_Texture *p1_left_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_left_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    
    SDL_Texture *p1_left_shoot_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_left_shoot_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_right_shoot_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_right_shoot_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);

    SDL_Texture *p1_left_jump_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_left_jump_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_right_jump_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_right_jump_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);

    SDL_Texture *p1_left_fall_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_left_fall_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_right_fall_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_right_fall_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);

    if (!p1_right_00 | !p1_right_01 | !p1_left_00 | !p1_left_01 |!p1_left_shoot_00 |!p1_left_shoot_01 |!p1_right_shoot_00 |!p1_right_shoot_01 |!p1_left_jump_00 |!p1_left_jump_01 |!p1_right_jump_00 |!p1_right_jump_01 |!p1_left_fall_00 |!p1_left_fall_01 |!p1_right_fall_00 |!p1_right_fall_01){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }
    
    Uint8 *pixels = NULL;
    int pitch = 0;
    
    //p1 right
    if(SDL_LockTexture(p1_right_00, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_right_00, pitch * p1_height);
    free(image_p1_right_00);
    SDL_UnlockTexture(p1_right_00);
        
    if(SDL_LockTexture(p1_right_01, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_right_01, pitch * p1_height);
    free(image_p1_right_01);
    SDL_UnlockTexture(p1_right_01);
    
    //p1 left
    if(SDL_LockTexture(p1_left_00, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_left_00, pitch * p1_height);
    free(image_p1_left_00);
    SDL_UnlockTexture(p1_left_00);
    
    if(SDL_LockTexture(p1_left_01, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_left_01, pitch * p1_height);
    free(image_p1_left_01);
    SDL_UnlockTexture(p1_left_01);

    //p1 shoot
    if(SDL_LockTexture(p1_left_shoot_00, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_left_shoot_00, pitch * p1_height);
    free(image_p1_left_shoot_00);
    SDL_UnlockTexture(p1_left_shoot_00);

    if(SDL_LockTexture(p1_left_shoot_01, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_left_shoot_01, pitch * p1_height);
    free(image_p1_left_shoot_01);
    SDL_UnlockTexture(p1_left_shoot_01);

    if(SDL_LockTexture(p1_right_shoot_00, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_right_shoot_00, pitch * p1_height);
    free(image_p1_right_shoot_00);
    SDL_UnlockTexture(p1_right_shoot_00);

    if(SDL_LockTexture(p1_right_shoot_01, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_right_shoot_01, pitch * p1_height);
    free(image_p1_right_shoot_01);
    SDL_UnlockTexture(p1_right_shoot_01);

    //p1 jump
    if(SDL_LockTexture(p1_left_jump_00, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_left_jump_00, pitch * p1_height);
    free(image_p1_left_jump_00);
    SDL_UnlockTexture(p1_left_jump_00);

    if(SDL_LockTexture(p1_left_jump_01, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_left_jump_01, pitch * p1_height);
    free(image_p1_left_jump_01);
    SDL_UnlockTexture(p1_left_jump_01);

    if(SDL_LockTexture(p1_right_jump_00, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_right_jump_00, pitch * p1_height);
    free(image_p1_right_jump_00);
    SDL_UnlockTexture(p1_right_jump_00);

    if(SDL_LockTexture(p1_right_jump_01, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_right_jump_01, pitch * p1_height);
    free(image_p1_right_jump_01);
    SDL_UnlockTexture(p1_right_jump_01);

    //p1 fall
    if(SDL_LockTexture(p1_left_fall_00, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_left_fall_00, pitch * p1_height);
    free(image_p1_left_fall_00);
    SDL_UnlockTexture(p1_left_fall_00);

    if(SDL_LockTexture(p1_left_fall_01, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_left_fall_01, pitch * p1_height);
    free(image_p1_left_fall_01);
    SDL_UnlockTexture(p1_left_fall_01);

    if(SDL_LockTexture(p1_right_fall_00, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_right_fall_00, pitch * p1_height);
    free(image_p1_right_fall_00);
    SDL_UnlockTexture(p1_right_fall_00);

    if(SDL_LockTexture(p1_right_fall_01, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_right_fall_01, pitch * p1_height);
    free(image_p1_right_fall_01);
    SDL_UnlockTexture(p1_right_fall_01);

    //p1_bubbles
    int bubble_width, bubble_height, bubble_comp;
    unsigned char *image_p1_bubble_empty = stbi_load(PLAYER_ONE_BUBBLE_EMPTY, &bubble_width, &bubble_height, &bubble_comp, 4);
    unsigned char *image_p1_bubble_shoot = stbi_load(PLAYER_ONE_BUBBLE_SHOOT, &bubble_width, &bubble_height, &bubble_comp, 4);
    if(!image_p1_bubble_empty | !image_p1_bubble_shoot){
        SDL_Log("Unable to load image: %s", SDL_GetError());
        goto quit;
    }
        
    SDL_Texture *p1_bubble_empty = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bubble_width, bubble_height);
    SDL_Texture *p1_bubble_shoot = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bubble_width, bubble_height);
    if (!p1_bubble_empty | !p1_bubble_shoot){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    if(SDL_LockTexture(p1_bubble_empty, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_bubble_empty, pitch * bubble_height);
    free(image_p1_bubble_empty);
    SDL_UnlockTexture(p1_bubble_empty);

    if(SDL_LockTexture(p1_bubble_shoot, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_bubble_shoot, pitch * bubble_height);
    free(image_p1_bubble_shoot);
    SDL_UnlockTexture(p1_bubble_shoot);

    //bubble absorb zen-chan
    unsigned char *image_p1_bubble_zen_0 = stbi_load(PLAYER_ONE_BUBBLE_ZEN_00, &bubble_width, &bubble_height, &bubble_comp, 4);
    unsigned char *image_p1_bubble_zen_1 = stbi_load(PLAYER_ONE_BUBBLE_ZEN_01, &bubble_width, &bubble_height, &bubble_comp, 4);
    unsigned char *image_p1_bubble_zen_2 = stbi_load(PLAYER_ONE_BUBBLE_ZEN_02, &bubble_width, &bubble_height, &bubble_comp, 4);
    if(!image_p1_bubble_zen_0 | !image_p1_bubble_zen_1 |!image_p1_bubble_zen_2){
        SDL_Log("Unable to load image: %s", SDL_GetError());
        goto quit;
    }
    
    SDL_Texture *p1_bubble_zen_0 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bubble_width, bubble_height);
    SDL_Texture *p1_bubble_zen_1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bubble_width, bubble_height);
    SDL_Texture *p1_bubble_zen_2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bubble_width, bubble_height);

    if (!p1_bubble_zen_0 | !p1_bubble_zen_1 |!p1_bubble_zen_2){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    if(SDL_LockTexture(p1_bubble_zen_0, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_bubble_zen_0, pitch * bubble_height);
    free(image_p1_bubble_zen_0);
    SDL_UnlockTexture(p1_bubble_zen_0);

    if(SDL_LockTexture(p1_bubble_zen_1, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_bubble_zen_1, pitch * bubble_height);
    free(image_p1_bubble_zen_1);
    SDL_UnlockTexture(p1_bubble_zen_1);

    if(SDL_LockTexture(p1_bubble_zen_2, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_bubble_zen_2, pitch * bubble_height);
    free(image_p1_bubble_zen_2);
    SDL_UnlockTexture(p1_bubble_zen_2);

    //bubble absorb might-a
    unsigned char *image_p1_bubble_might_0 = stbi_load(PLAYER_ONE_BUBBLE_MIGHTA_00, &bubble_width, &bubble_height, &bubble_comp, 4);
    unsigned char *image_p1_bubble_might_1 = stbi_load(PLAYER_ONE_BUBBLE_MIGHTA_01, &bubble_width, &bubble_height, &bubble_comp, 4);
    unsigned char *image_p1_bubble_might_2 = stbi_load(PLAYER_ONE_BUBBLE_MIGHTA_02, &bubble_width, &bubble_height, &bubble_comp, 4);
    if(!image_p1_bubble_might_0 | !image_p1_bubble_might_1 |!image_p1_bubble_might_2){
        SDL_Log("Unable to load image: %s", SDL_GetError());
        goto quit;
    }
    
    SDL_Texture *p1_bubble_might_0 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bubble_width, bubble_height);
    SDL_Texture *p1_bubble_might_1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bubble_width, bubble_height);
    SDL_Texture *p1_bubble_might_2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bubble_width, bubble_height);

    if (!p1_bubble_might_0 | !p1_bubble_might_1 |!p1_bubble_might_2){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    if(SDL_LockTexture(p1_bubble_might_0, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_bubble_might_0, pitch * bubble_height);
    free(image_p1_bubble_might_0);
    SDL_UnlockTexture(p1_bubble_might_0);

    if(SDL_LockTexture(p1_bubble_might_1, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_bubble_might_1, pitch * bubble_height);
    free(image_p1_bubble_might_1);
    SDL_UnlockTexture(p1_bubble_might_1);

    if(SDL_LockTexture(p1_bubble_might_2, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_bubble_might_2, pitch * bubble_height);
    free(image_p1_bubble_might_2);
    SDL_UnlockTexture(p1_bubble_might_2);

    int enemy_width, enemy_height, enemy_comp;
    //enemy zen-chan
    unsigned char *image_zen_left_0 = stbi_load(ENEMY_ZEN_CHAN_LEFT_00, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_zen_left_1 = stbi_load(ENEMY_ZEN_CHAN_LEFT_01, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_zen_left_2 = stbi_load(ENEMY_ZEN_CHAN_LEFT_02, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_zen_left_3 = stbi_load(ENEMY_ZEN_CHAN_LEFT_03, &enemy_width, &enemy_height, &enemy_comp, 4);

    unsigned char *image_zen_right_0 = stbi_load(ENEMY_ZEN_CHAN_RIGHT_00, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_zen_right_1 = stbi_load(ENEMY_ZEN_CHAN_RIGHT_01, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_zen_right_2 = stbi_load(ENEMY_ZEN_CHAN_RIGHT_02, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_zen_right_3 = stbi_load(ENEMY_ZEN_CHAN_RIGHT_03, &enemy_width, &enemy_height, &enemy_comp, 4);
    if(!image_zen_left_0 | !image_zen_left_1 |!image_zen_left_2 |!image_zen_left_3 | !image_zen_right_0 | !image_zen_right_1 |!image_zen_right_2 |!image_zen_right_3){
        SDL_Log("Unable to load image: %s", SDL_GetError());
        goto quit;
    }

    SDL_Texture *zen_left_0 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *zen_left_1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *zen_left_2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *zen_left_3 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);

    SDL_Texture *zen_right_0 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *zen_right_1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *zen_right_2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *zen_right_3 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    if(!zen_left_0 | !zen_left_1 |!zen_left_2 | !zen_left_3 | !zen_right_0 | !zen_right_1 |!zen_right_2 | !zen_right_3){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    if(SDL_LockTexture(zen_left_0, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_zen_left_0, pitch * enemy_height);
    free(image_zen_left_0);
    SDL_UnlockTexture(zen_left_0);

    if(SDL_LockTexture(zen_left_1, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_zen_left_1, pitch * enemy_height);
    free(image_zen_left_1);
    SDL_UnlockTexture(zen_left_1);

    if(SDL_LockTexture(zen_left_2, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_zen_left_2, pitch * enemy_height);
    free(image_zen_left_2);
    SDL_UnlockTexture(zen_left_2);
    
    if(SDL_LockTexture(zen_left_3, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_zen_left_3, pitch * enemy_height);
    free(image_zen_left_3);
    SDL_UnlockTexture(zen_left_3);

    if(SDL_LockTexture(zen_right_0, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_zen_right_0, pitch * enemy_height);
    free(image_zen_right_0);
    SDL_UnlockTexture(zen_right_0);

    if(SDL_LockTexture(zen_right_1, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_zen_right_1, pitch * enemy_height);
    free(image_zen_right_1);
    SDL_UnlockTexture(zen_right_1);

    if(SDL_LockTexture(zen_right_2, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_zen_right_2, pitch * enemy_height);
    free(image_zen_right_2);
    SDL_UnlockTexture(zen_right_2);
    
    if(SDL_LockTexture(zen_right_3, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_zen_right_3, pitch * enemy_height);
    free(image_zen_right_3);
    SDL_UnlockTexture(zen_right_3);

    //enemy might-a
    unsigned char *image_might_left_0 = stbi_load(ENEMY_MIGTH_A_LEFT_00, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_might_left_1 = stbi_load(ENEMY_MIGTH_A_LEFT_01, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_might_left_2 = stbi_load(ENEMY_MIGTH_A_LEFT_02, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_might_left_3 = stbi_load(ENEMY_MIGTH_A_LEFT_03, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_might_left_4 = stbi_load(ENEMY_MIGTH_A_LEFT_04, &enemy_width, &enemy_height, &enemy_comp, 4);

    unsigned char *image_might_right_0 = stbi_load(ENEMY_MIGTH_A_RIGHT_00, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_might_right_1 = stbi_load(ENEMY_MIGTH_A_RIGHT_01, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_might_right_2 = stbi_load(ENEMY_MIGTH_A_RIGHT_02, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_might_right_3 = stbi_load(ENEMY_MIGTH_A_RIGHT_03, &enemy_width, &enemy_height, &enemy_comp, 4);
    unsigned char *image_might_right_4 = stbi_load(ENEMY_MIGTH_A_RIGHT_04, &enemy_width, &enemy_height, &enemy_comp, 4);
    if(!image_might_left_0 | !image_might_left_1 |!image_might_left_2 |!image_might_left_3 |!image_might_left_4 | !image_might_right_0 | !image_might_right_1 |!image_might_right_2 |!image_might_right_3 |!image_might_right_4){
        SDL_Log("Unable to load image: %s", SDL_GetError());
        goto quit;
    }

    SDL_Texture *might_left_0 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *might_left_1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *might_left_2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *might_left_3 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *might_left_4 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);

    SDL_Texture *might_right_0 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *might_right_1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *might_right_2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *might_right_3 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    SDL_Texture *might_right_4 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, enemy_width, enemy_height);
    if(!might_left_0 | !might_left_1 |!might_left_2 | !might_left_3 | !might_left_4 | !might_right_0 | !might_right_1 |!might_right_2 | !might_right_3 | !might_right_4){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    if(SDL_LockTexture(might_left_0, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_left_0, pitch * enemy_height);
    free(image_might_left_0);
    SDL_UnlockTexture(might_left_0);

    if(SDL_LockTexture(might_left_1, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_left_1, pitch * enemy_height);
    free(image_might_left_1);
    SDL_UnlockTexture(might_left_1);

    if(SDL_LockTexture(might_left_2, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_left_2, pitch * enemy_height);
    free(image_might_left_2);
    SDL_UnlockTexture(might_left_2);

    if(SDL_LockTexture(might_left_3, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_left_3, pitch * enemy_height);
    free(image_might_left_3);
    SDL_UnlockTexture(might_left_3);

    if(SDL_LockTexture(might_left_4, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_left_4, pitch * enemy_height);
    free(image_might_left_4);
    SDL_UnlockTexture(might_left_4);

    if(SDL_LockTexture(might_right_0, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_right_0, pitch * enemy_height);
    free(image_might_right_0);
    SDL_UnlockTexture(might_right_0);

    if(SDL_LockTexture(might_right_1, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_right_1, pitch * enemy_height);
    free(image_might_right_1);
    SDL_UnlockTexture(might_right_1);

    if(SDL_LockTexture(might_right_2, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_right_2, pitch * enemy_height);
    free(image_might_right_2);
    SDL_UnlockTexture(might_right_2);    
    
    if(SDL_LockTexture(might_right_3, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_right_3, pitch * enemy_height);
    free(image_might_right_3);
    SDL_UnlockTexture(might_right_3);
    
    if(SDL_LockTexture(might_right_4, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_might_right_4, pitch * enemy_height);
    free(image_might_right_4);
    SDL_UnlockTexture(might_right_4); 

    //bullets
    //might-a
    int bullet_might_width, bullet_might_height, bullet_might_comp;
    unsigned char *image_bullet_might_0 = stbi_load(BULLET_MIGTH_A_00, &bullet_might_width, &bullet_might_height, &bullet_might_comp, 4); 
    unsigned char *image_bullet_might_1 = stbi_load(BULLET_MIGTH_A_01, &bullet_might_width, &bullet_might_height, &bullet_might_comp, 4);
    unsigned char *image_bullet_might_2 = stbi_load(BULLET_MIGTH_A_02, &bullet_might_width, &bullet_might_height, &bullet_might_comp, 4);
    unsigned char *image_bullet_might_3 = stbi_load(BULLET_MIGTH_A_03, &bullet_might_width, &bullet_might_height, &bullet_might_comp, 4);
    if(!image_bullet_might_0 |!image_bullet_might_1 |!image_bullet_might_2 |!image_bullet_might_3){
        SDL_Log("Unable to load image %s: %s", PLAYER_ONE_LIFE, SDL_GetError());
        goto quit;
    }

    SDL_Texture *bullet_might_0 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bullet_might_width, bullet_might_height);
    SDL_Texture *bullet_might_1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bullet_might_width, bullet_might_height);
    SDL_Texture *bullet_might_2 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bullet_might_width, bullet_might_height);
    SDL_Texture *bullet_might_3 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, bullet_might_width, bullet_might_height);
    if (!bullet_might_0 |!bullet_might_1 |!bullet_might_2 |!bullet_might_3){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    if(SDL_LockTexture(bullet_might_0, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_bullet_might_0, pitch * bullet_might_height);
    free(image_bullet_might_0);
    SDL_UnlockTexture(bullet_might_0);

    if(SDL_LockTexture(bullet_might_1, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_bullet_might_1, pitch * bullet_might_height);
    free(image_bullet_might_1);
    SDL_UnlockTexture(bullet_might_1);

    if(SDL_LockTexture(bullet_might_2, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_bullet_might_2, pitch * bullet_might_height);
    free(image_bullet_might_2);
    SDL_UnlockTexture(bullet_might_2);

    if(SDL_LockTexture(bullet_might_3, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_bullet_might_3, pitch * bullet_might_height);
    free(image_bullet_might_3);
    SDL_UnlockTexture(bullet_might_3);

    //life token
    int life_width, life_height, life_comp;
    unsigned char *image_p1_life = stbi_load(PLAYER_ONE_LIFE, &life_width, &life_height, &life_comp, 4);
    if(!image_p1_life){
        SDL_Log("Unable to load image %s: %s", PLAYER_ONE_LIFE, SDL_GetError());
        goto quit;
    }

    SDL_Texture *p1_life = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, life_width, life_height);
    if (!p1_life){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    if(SDL_LockTexture(p1_life, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_p1_life, pitch * life_height);
    free(image_p1_life);
    SDL_UnlockTexture(p1_life);

    //walls
    int wall_width, wall_height, wall_comp;
    unsigned char *image_wall = stbi_load(MAP_TILE_WALL, &wall_width, &wall_height, &wall_comp, 4);
    if(!image_wall){
        SDL_Log("Unable to load image %s: %s", MAP_TILE_WALL, SDL_GetError());
        goto quit;
    }

    SDL_Texture *wall = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, wall_width, wall_height);
    if (!wall){
        SDL_Log("Unable to create texture: %s", SDL_GetError());
        goto quit;
    }

    if(SDL_LockTexture(wall, NULL, (void **)&pixels, &pitch)){
        SDL_Log("unable to map texture into address space");
        goto quit;
    }
    memcpy(pixels, image_wall, pitch * wall_height);
    free(image_wall);
    SDL_UnlockTexture(wall);

//Player 1 init
    player_t *player1 = player_init(0);
    if(!player1){
        goto quit;
    }
    player1->rect.w = p1_width;
    player1->rect.h = p1_height;
    bubble_t *p1_bubbles = malloc(sizeof(bubble_t) * MAX_PLAYER_BUBBLE);
    for(int i = 0; i < MAX_PLAYER_BUBBLE; i++){
        p1_bubbles[i] = *bubble_init(player1->player_id);
    
        p1_bubbles[i].rect.w = bubble_width;
        p1_bubbles[i].rect.h = bubble_height;
    }

    int time = 0;
    SDL_Texture *p1_right = p1_right_00;
    SDL_Texture *p1_left = p1_left_00;

    SDL_Texture *p1_right_shoot = p1_right_shoot_00;
    SDL_Texture *p1_left_shoot = p1_left_shoot_00;
    SDL_Texture *p1_shoot = p1_right_shoot;

    SDL_Texture *p1_right_jump = p1_right_jump_00;
    SDL_Texture *p1_left_jump = p1_left_jump_00;
    SDL_Texture *p1_jump = p1_right_jump;

    SDL_Texture *p1_right_falling = p1_right_fall_00;
    SDL_Texture *p1_left_falling = p1_left_fall_00;
    SDL_Texture *p1_falling = p1_right_falling;

    SDL_Texture *p1_texture = p1_right;
//Level load
    int level_number = 00;
    int p1_spawn_tile_x = 0;
    int p1_spawn_tile_y = 0;
    
    int max_y = MAP_TILES_Y;
    int max_x = MAP_TILES_X;
    int index = 0;
    SDL_Rect map_tiles_rects[MAP_TILES_Y * MAP_TILES_X];
    for(int y = 0; y < max_y; y++){
        for(int x = 0; x < max_x; x++){
            map_tiles_rects[index].x = x * wall_width;
            map_tiles_rects[index].y = y * wall_height;
            map_tiles_rects[index].w = wall_width;
            map_tiles_rects[index].h = wall_height;
            index++;
        }
    }

    SDL_Rect player1_life_rects[PLAYER_MAX_LIFE];
    for(int i = 0; i < PLAYER_MAX_LIFE; i++){
        player1_life_rects[i].x = i * life_width;
        player1_life_rects[i].y = (max_y * wall_height) - life_height;
        player1_life_rects[i].w = life_width;
        player1_life_rects[i].h = life_height;
    }
    
    if(0 <= level_number && level_number <= 99){
        //all level beated, YOU WIN!, game over
    }
   
start_level:
    level_t *level = load_level_from_file(++level_number, &p1_spawn_tile_x, &p1_spawn_tile_y);
    if(!level){
        SDL_Log("error load level %d", level_number);
    }

    //load enemies    
    enemy_t *level_enemies = spawn_enemies_on_level(level, level_number);

    spawn_player_in_level(player1, p1_spawn_tile_x, p1_spawn_tile_y);

    for(int i = 0; i < level->enemies_in_level; i++){
        level_enemies[i].status = 1;
    }

//GAME LOOP
    for(;;){
        //SDL_Log("start loop\n");
        //get inputs
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                goto quit;
            }else{

                if(event.type == SDL_KEYUP && player1->is_alive){
                    switch (event.key.keysym.sym){
                        case SDLK_s:
                            //shoot
                            player1->is_shooting = 0;
                            break;
                        default:
                            break;
                    }
                }

                if(event.type == SDL_KEYDOWN && player1->is_alive){
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            goto quit;
                            break;
                        case SDLK_w:
                            //jump
                            if(player1->jumping_state == 0){
                                player1->jump_offset = JUMP_START_OFFSET;
                            }
                            break;
                        case SDLK_a:
                            player1->direction = -1;
                            int update_x = player1->rect.x + (3 * player1->direction);
                            if(get_tile_value(level, player1->rect.y, update_x) != 1 && get_tile_value(level, player1->rect.y + player1->rect.h, update_x) != 1){
                                player1->rect.x = update_x;
                            }
                            break;
                        case SDLK_d:
                            player1->direction = 1;
                            update_x = player1->rect.x + (3 * player1->direction);
                            if(get_tile_value(level, player1->rect.y, update_x + player1->rect.w) != 1 && get_tile_value(level, player1->rect.y + player1->rect.h, update_x + player1->rect.w) != 1){
                                player1->rect.x = update_x;
                            }
                            break;
                        case SDLK_s:
                            if(player1->is_shooting == 0){
                               player1->is_shooting = 1;
                               bubble_t *bubble =  get_bubble(player1->player_id, p1_bubbles);
                               player_shoot(player1, bubble);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
        
        //animation time
        time++;
        if(time % 40 == 0){
            player1->animation_frame = ++player1->animation_frame % 2;
            if(player1->animation_frame == 0){
                p1_right = p1_right_00;
                p1_left = p1_left_00;
                p1_right_shoot = p1_right_shoot_00;
                p1_left_shoot = p1_left_shoot_00;
                p1_right_jump = p1_right_jump_00;
                p1_left_jump = p1_left_jump_00;
                p1_right_falling = p1_right_fall_00;
                p1_left_falling = p1_left_fall_00;
            }else if(player1->animation_frame == 1){
                p1_right = p1_right_01;
                p1_left = p1_left_01;
                p1_right_shoot = p1_right_shoot_01;
                p1_left_shoot = p1_left_shoot_01;
                p1_right_jump = p1_right_jump_01;
                p1_left_jump = p1_left_jump_01;
                p1_right_falling = p1_right_fall_01;
                p1_left_falling = p1_left_fall_01;
            }

            for(int i = 0; i < level->enemies_in_level; i++){
                if(level_enemies[i].status != 0){
                    level_enemies[i].animation_frame = ++level_enemies[i].animation_frame % level_enemies[i].number_of_animation_frame;
                }
            }

            for(int i = 0; i < MAX_PLAYER_BUBBLE; i++){
                if(p1_bubbles[i].status == 3){
                    p1_bubbles[i].absorbed_frame_animation = ++p1_bubbles[i].absorbed_frame_animation % 4;
                }
            }
        }
        if(time % 5 == 0){       
            for(int i = 0; i < level->enemies_in_level; i++){
                if(level_enemies[i].status != 0){
                    if(level_enemies[i].bullet != NULL && level_enemies[i].bullet->status == 1){
                        level_enemies[i].bullet->animation_frame = ++level_enemies[i].bullet->animation_frame % level_enemies[i].bullet->number_of_animation_frame;
                    }
                }
            }
        }

        //draw background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        //draw map
        int i = 0;
        for(int y = 0; y < max_y; y++){
            for(int x = 0; x < max_x; x++){
                int value = level->level_layout[y][x];
                if(value == 1){
                    SDL_RenderCopy(renderer, wall, NULL, &map_tiles_rects[i]);
                }
                i++;                
            }
        }
        //draw p1 lifes
        for(i = 0; i < player1->life; i++){
            SDL_RenderCopy(renderer, p1_life, NULL, &player1_life_rects[i]);
        }

        //p1 update and draw
        if(player1->is_alive){
            //update player sprites for animations
            if(player1->respawn_invicibility > 0){
                player1->respawn_invicibility--;
            }

            if(player1->direction == 1){
                p1_jump = p1_right_jump;
                p1_shoot = p1_right_shoot;
                p1_falling = p1_right_falling;
                p1_texture = p1_right;
            }else if(player1->direction == -1){
                p1_jump = p1_left_jump;
                p1_shoot = p1_left_shoot;
                p1_falling = p1_left_falling;
                p1_texture = p1_left;
            }
          
            if(player1->jumping_state == 1){
                p1_texture = p1_jump;
            }else if(player1->jumping_state == -1){
                p1_texture = p1_falling;
            }

            if(player1->is_shooting){
                p1_texture = p1_shoot;
            }
            
            if(player1->jump_offset > 0){
                player1->rect.y -= 2;
                player1->jump_offset -= 2;
            }

            player1->jumping_state = player1->jump_offset > 0; 
            if(player1->jumping_state != 1 && apply_gravity(level, player1->rect) == 2){
                player1->jumping_state = -1;
                player1->rect.y = MAP_TILES_Y;
            }
            if(player1->jumping_state != 1 && apply_gravity(level, player1->rect) == 1){
                player1->jumping_state = -1;
                player1->rect.y += 1;
            }
            SDL_RenderCopy(renderer, p1_texture, NULL, &player1->rect);
        }

        //update bubbles
        for(int i = 0; i < MAX_PLAYER_BUBBLE; i++){
            if (p1_bubbles[i].status != 0){
                //update x and y
                int is_bubble_active = bubble_update(&p1_bubbles[i]);
                //if duration is over free enemy if status == 3(enemy inside), then go inactive
                
                if(is_bubble_active == 0){
                    if(p1_bubbles[i].status == 3){
                        if(get_tile_value(level, p1_bubbles[i].rect.y + p1_bubbles[i].rect.h, p1_bubbles[i].rect.x) == 1 || get_tile_value(level, p1_bubbles[i].rect.y + p1_bubbles[i].rect.h, p1_bubbles[i].rect.x + p1_bubbles[i].rect.w) == 1){
                            p1_bubbles[i].duration += 2;
                        }
                        else{
                            release_enemy(&p1_bubbles[i], level_enemies, level->enemies_in_level);
                        }
                    }
                    if(p1_bubbles[i].duration <= 0){
                        reset_bubble(&p1_bubbles[i]);
                    }
                }

                //else if(is_bubble_active == 1){
                if(p1_bubbles[i].duration > 0){

                    //check wall collision
                    bubble_wall_collision(level, &p1_bubbles[i]);
                    //check player collision
                    if(rect_collision(p1_bubbles[i].rect, player1->rect)){
                        int collision_result = on_bubble_collision(&p1_bubbles[i], player1->player_id, player1->rect);                   
                        if(player1->jumping_state == -1 && collision_result == 2){
                            /*
                            if(event.type == SDL_KEYDOWN){
                                if(event.key.keysym.sym == SDLK_w){
                                    player1->jump_offset = JUMP_START_OFFSET / 2;
                                }
                            }
                            */
                            player1->jump_offset = JUMP_START_OFFSET / 2;
                        }else if(collision_result == 1 || collision_result == 2){
                            if(p1_bubbles[i].status == 3){
                                level->enemies_alive--;
                            }
                            reset_bubble(&p1_bubbles[i]);
                        }
                    }
                    //check enemies collision
                    for(int j = 0; j < level->enemies_in_level; j++){
                        if(level_enemies[j].status == 1){
                            if(rect_collision(p1_bubbles[i].rect, level_enemies[j].rect)){
                                int collision_result = on_bubble_collision(&p1_bubbles[i], level_enemies[j].type_id, level_enemies[j].rect);
                                if(collision_result == 3 && p1_bubbles[i].status == 1){
                                    enemy_absorb(&p1_bubbles[i], &level_enemies[j], j);
                                }
                            }
                        }
                    }

                    //check bubble collision
                    if(p1_bubbles[i].status != 1){
                        for(int j = 0; j < MAX_PLAYER_BUBBLE; j++){
                            if(p1_bubbles[j].status == 1){
                                if(i != j){
                                    if(rect_collision(p1_bubbles[i].rect, p1_bubbles[j].rect)){
                                        p1_bubbles[i].shoot_distance_offset = p1_bubbles[j].shoot_distance_offset / 4;
                                    }
                                }                                
                            }
                        }                        
                    }

                    //draw bubble
                    switch(p1_bubbles[i].status){
                        case 01:
                            SDL_RenderCopy(renderer, p1_bubble_shoot, NULL, &p1_bubbles[i].rect);
                            break;
                        case 02:
                            SDL_RenderCopy(renderer, p1_bubble_empty, NULL, &p1_bubbles[i].rect);
                            break;
                        case 03:
                            SDL_Texture *enemy_trapped;
                            if(p1_bubbles[i].absorbed_type == 2){
                                if(p1_bubbles[i].absorbed_frame_animation == 0){
                                    enemy_trapped = p1_bubble_zen_0;
                                }else if(p1_bubbles[i].absorbed_frame_animation == 1 || p1_bubbles[i].absorbed_frame_animation == 3){
                                    enemy_trapped = p1_bubble_zen_1;
                                }else if(p1_bubbles[i].absorbed_frame_animation == 2){
                                    enemy_trapped = p1_bubble_zen_2;
                                }
                            }else if(p1_bubbles[i].absorbed_type == 3){
                                if(p1_bubbles[i].absorbed_frame_animation == 0){
                                    enemy_trapped = p1_bubble_might_0;
                                }else if(p1_bubbles[i].absorbed_frame_animation == 1 || p1_bubbles[i].absorbed_frame_animation == 3){
                                    enemy_trapped = p1_bubble_might_1;
                                }else if(p1_bubbles[i].absorbed_frame_animation == 2){
                                    enemy_trapped = p1_bubble_might_2;
                                }
                            }
                            SDL_RenderCopy(renderer, enemy_trapped, NULL, &p1_bubbles[i].rect);
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        //update enemies
        for(int i = 0; i < level->enemies_in_level; i++){
            if(level_enemies[i].status == 1){
                //apply gravity
                if(level_enemies[i].jump_offset == 0 && apply_gravity(level, level_enemies[i].rect) == 2){
                    level_enemies[i].rect.y = MAP_TILES_Y;
                }

                if(player1->jumping_state != 1 && apply_gravity(level, player1->rect) == 2){
                    player1->jumping_state = -1;
                    player1->rect.y = MAP_TILES_Y;
                }

                if(level_enemies[i].jump_offset == 0 && apply_gravity(level, level_enemies[i].rect) == 1){
                    level_enemies[i].rect.y += 1;
                }else{
                    if(enemy_update(&level_enemies[i], rand() % 30) == 1){
                        level_enemies[i].jump_offset = JUMP_START_OFFSET;
                        level_enemies[i].special_action_chance = 0;
                    }

                    if(level_enemies[i].jump_offset > 0){
                        level_enemies[i].rect.y -= 2;
                        level_enemies[i].jump_offset -= 2;
                    }else{
                        int update_x = level_enemies[i].rect.x + level_enemies[i].direction;
                        if(level_enemies[i].direction == 1){
                            if(get_tile_value(level, level_enemies[i].rect.y, update_x + level_enemies[i].rect.w) != 1 && get_tile_value(level, level_enemies[i].rect.y + level_enemies[i].rect.h, update_x + level_enemies[i].rect.w) != 1){
                                level_enemies[i].rect.x = update_x;
                            }else{
                                level_enemies[i].direction = -1;
                            }
                        }else if(level_enemies[i].direction == -1){
                            if(get_tile_value(level, level_enemies[i].rect.y, update_x) != 1 && get_tile_value(level, level_enemies[i].rect.y + level_enemies[i].rect.h, update_x) != 1){
                                level_enemies[i].rect.x = update_x;
                            }else{
                                level_enemies[i].direction = 1;
                            }
                        }
                    }
                }

                //check player collision
                if(rect_collision(level_enemies[i].rect, player1->rect)){
                    if(player1->respawn_invicibility == 0){
                        on_player_death(player1);
                        spawn_player_in_level(player1, p1_spawn_tile_x, p1_spawn_tile_y);
                    }
                }
                
                SDL_Texture *enemy_texture;
                if(level_enemies[i].type_id == 2){
                    //zen-chan textures
                    if(level_enemies[i].direction == -1){
                        switch(level_enemies[i].animation_frame){
                            case 0:
                                enemy_texture = zen_left_0;
                                break;
                            case 1:
                                enemy_texture = zen_left_1;
                                break;
                            case 2:
                                enemy_texture = zen_left_2;
                                break;
                            case 3:
                                enemy_texture = zen_left_3;
                                break;
                        }
                    }else if(level_enemies[i].direction == 1){
                        switch(level_enemies[i].animation_frame){
                            case 0:
                                enemy_texture = zen_right_0;
                                break;
                            case 1:
                                enemy_texture = zen_right_1;
                                break;
                            case 2:
                                enemy_texture = zen_right_2;
                                break;
                            case 3:
                                enemy_texture = zen_right_3;
                                break;
                        }
                    }
                }else if(level_enemies[i].type_id == 3){
                    //might-a textures
                    if(level_enemies[i].direction == -1){
                        switch(level_enemies[i].animation_frame){
                            case 0:
                                enemy_texture = might_left_0;
                                break;
                            case 1:
                                enemy_texture = might_left_1;
                                break;
                            case 2:
                                enemy_texture = might_left_2;
                                break;
                            case 3:
                                enemy_texture = might_left_3;
                                break;
                            case 4:
                                enemy_texture = might_left_4;
                                break;
                        }
                    }else if(level_enemies[i].direction == 1){
                        switch(level_enemies[i].animation_frame){
                            case 0:
                                enemy_texture = might_right_0;
                                break;
                            case 1:
                                enemy_texture = might_right_1;
                                break;
                            case 2:
                                enemy_texture = might_right_2;
                                break;
                            case 3:
                                enemy_texture = might_right_3;
                                break;
                            case 4:
                                enemy_texture = might_right_4;
                                break;
                        }
                    }
                }
                SDL_RenderCopy(renderer, enemy_texture, NULL, &level_enemies[i].rect);
            }   

            //bullet update
            if(level_enemies[i].bullet != NULL && level_enemies[i].bullet->status == 1){
                //move/check wall collision                    
                int bullet_update_x = level_enemies[i].bullet->rect.x + (level_enemies[i].bullet->direction_x * level_enemies[i].bullet->speed);
                if(level_enemies[i].bullet->direction_x == 1){
                    if(get_tile_value(level, level_enemies[i].bullet->rect.y, bullet_update_x + level_enemies[i].bullet->rect.w) != 1 && get_tile_value(level, level_enemies[i].bullet->rect.y + level_enemies[i].bullet->rect.h, bullet_update_x + level_enemies[i].bullet->rect.w) != 1){
                        level_enemies[i].bullet->rect.x = bullet_update_x;
                    }else{
                        level_enemies[i].bullet->status = 0;
                    }
                }else if(level_enemies[i].bullet->direction_x == -1){
                    if(get_tile_value(level, level_enemies[i].bullet->rect.y, bullet_update_x) != 1 && get_tile_value(level, level_enemies[i].bullet->rect.y + level_enemies[i].bullet->rect.h, bullet_update_x) != 1){
                        level_enemies[i].bullet->rect.x = bullet_update_x;
                    }else{
                        level_enemies[i].bullet->status = 0;
                    }
                }
                //int bullet_update_y
                   
                if(get_tile_value(level, level_enemies[i].bullet->rect.y, bullet_update_x) != 1 && get_tile_value(level, level_enemies[i].bullet->rect.y + level_enemies[i].bullet->rect.h, bullet_update_x) != 1){
                    level_enemies[i].bullet->rect.x = bullet_update_x;
                }else{
                    level_enemies[i].bullet->status = 0;
                }

                //check player collision
                if(rect_collision(level_enemies[i].bullet->rect, player1->rect)){
                    if(player1->respawn_invicibility == 0){
                        on_player_death(player1);
                        level_enemies[i].bullet->status = 0;
                        spawn_player_in_level(player1, p1_spawn_tile_x, p1_spawn_tile_y);
                    }
                }

                //draw bullet
                SDL_Texture *bullet_texture;
                if(level_enemies[i].bullet->owner_id == 3){
                    switch(level_enemies[i].bullet->animation_frame){
                        case 0:
                            bullet_texture = bullet_might_0;
                            break;
                        case 1:
                            bullet_texture = bullet_might_1;
                            break;
                        case 2:
                            bullet_texture = bullet_might_2;
                            break;
                        case 3:
                            bullet_texture = bullet_might_3;
                            break;
                    }
                }
                SDL_RenderCopy(renderer, bullet_texture, NULL, &level_enemies[i].bullet->rect);                
            }        
        }

        //go to next level
        if(level->enemies_alive <= 0){
            goto start_level;
        }
        //game over
        if(player1->is_alive == 0){
            goto quit;
        }

        SDL_RenderPresent(renderer);
    }

quit:
    if(level_enemies){
        for(int i = 0; i < level->enemies_in_level; i++){
            free(&level_enemies[i]);
        }
        free(level_enemies);
    }

    if (p1_bubbles){
        for(int i = 0; i < MAX_PLAYER_BUBBLE; i++){
            free(&p1_bubbles[i]);
        }
        free(p1_bubbles);
    }

    if(player1){
        free(player1);
    }

    if(level){
        free(level);
    }

    //textures
    if(p1_left_00){
        SDL_DestroyTexture(p1_left_00);
    }
    if(p1_left_01){
        SDL_DestroyTexture(p1_left_01);
    }    
    if(p1_right_00){
        SDL_DestroyTexture(p1_right_00);
    }    
    if(p1_right_01){
        SDL_DestroyTexture(p1_right_01);
    }
    if(p1_left_shoot_00){
        SDL_DestroyTexture(p1_left_shoot_00);
    }
    if(p1_left_shoot_01){
        SDL_DestroyTexture(p1_left_shoot_01);
    }    
    if(p1_right_shoot_00){
        SDL_DestroyTexture(p1_right_shoot_00);
    }
    if(p1_right_shoot_01){
        SDL_DestroyTexture(p1_right_shoot_01);
    }
    if(p1_left_jump_00){
        SDL_DestroyTexture(p1_left_jump_00);
    }
    if(p1_left_jump_01){
        SDL_DestroyTexture(p1_left_jump_01);
    }    
    if(p1_right_jump_00){
        SDL_DestroyTexture(p1_right_jump_00);
    }    
    if(p1_right_jump_01){
        SDL_DestroyTexture(p1_right_jump_01);
    }
    if(p1_left_fall_00){
        SDL_DestroyTexture(p1_left_fall_00);
    }
    if(p1_left_fall_01){
        SDL_DestroyTexture(p1_left_fall_01);
    }    
    if(p1_right_fall_00){
        SDL_DestroyTexture(p1_right_fall_00);
    }
    if(p1_right_fall_01){
        SDL_DestroyTexture(p1_right_fall_01);
    }

    if(p1_bubble_shoot){
        SDL_DestroyTexture(p1_bubble_shoot);
    }    
    if(p1_bubble_empty){
        SDL_DestroyTexture(p1_bubble_empty);
    }

    if(p1_bubble_zen_0){
        SDL_DestroyTexture(p1_bubble_zen_0);
    }
    if(p1_bubble_zen_1){
        SDL_DestroyTexture(p1_bubble_zen_1);
    }
    if(p1_bubble_zen_2){
        SDL_DestroyTexture(p1_bubble_zen_2);
    }

    if(zen_left_0){
        SDL_DestroyTexture(zen_left_0);
    }
    if(zen_left_1){
        SDL_DestroyTexture(zen_left_1);
    }
    if(zen_left_2){
        SDL_DestroyTexture(zen_left_2);
    }
    if(zen_left_3){
        SDL_DestroyTexture(zen_left_3);
    }
    if(zen_right_0){
        SDL_DestroyTexture(zen_right_0);
    }
    if(zen_right_1){
        SDL_DestroyTexture(zen_right_1);
    }
    if(zen_right_2){
        SDL_DestroyTexture(zen_right_2);
    }
    if(zen_right_3){
        SDL_DestroyTexture(zen_right_3);
    }

    if(p1_life){
        SDL_DestroyTexture(p1_life);
    }
    if(wall){
        SDL_DestroyTexture(wall);
    }

    //SDL
    if (renderer){
        SDL_DestroyRenderer(renderer);
    }
    if (window){
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
    return ret;
}

//clang -c -o cpu/map/map.o .\cpu/map/map.c
//clang -c -o cpu/player/player.o .\cpu/player/player.c
//clang -c -o cpu/bubble/bubble.o .\cpu/bubble/bubble.c
//clang -c -o cpu/enemies/enemy.o .\cpu/enemies/enemy.c

//clang -I .\SDL2-2.0.10\include\ -o main.exe main.c -L .\SDL2-2.0.10\lib\x64\ -l SDL2 .\cpu/map/map.c .\cpu/player/player.c .\cpu/bubble/bubble.c .\cpu/enemies/enemy.c