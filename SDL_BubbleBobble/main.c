#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 720
#define JUMP_START_OFFSET 56

#include "cpu/map/map.h"
#include "cpu/player/player.h"

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

#define MAP_TILE_WALL "textures/wall.png"

#define PLAYER_ONE_LIFE "textures/p1_life_token.png"

#define FILE_PATH "cpu/map/level_"

level_t* load_level_from_file(int level_number, int *p1_spawn_pos_x, int *p1_spawn_pos_y){
    //int first_digit = level_number % 10;
    //int second_digit = level_number / 10;
    level_t *level;
    switch(level_number){
        case 01:
            level = load_level("cpu/map/level_01.h");
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

void spawn_player_in_level(player_t *player, int spawn_pos_x, int spawn_pos_y){
    if(!player){
        return;
    }

    player->rect.x = spawn_pos_x;
    player->rect.y = spawn_pos_y;
}

int apply_gravity(level_t *level, SDL_Rect rect){
    int update_y = rect.y + rect.h + 1;
    if(get_tile_value(level, update_y, rect.x) != 1 && get_tile_value(level, update_y, rect.x + rect.w) != 1){
        return 1;
    }
    return 0;
}

int main(int argc, char **argv){
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
    player1->rect.w = p1_width;
    player1->rect.h = p1_height;

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
    if(0 <= level_number && level_number <= 99){
        //all level beated, YOU WIN!, game over
    }
    level_t *level = load_level_from_file(++level_number, &p1_spawn_tile_x, &p1_spawn_tile_y);
    if(!level){
        SDL_Log("error load level %d", level_number);
    }

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

    int p1_spawn_pos_x = p1_spawn_tile_x * wall_width;
    int p1_spawn_pos_y = (p1_spawn_tile_y * wall_height) + (wall_height - p1_height) - 1;
    spawn_player_in_level(player1, p1_spawn_pos_x, p1_spawn_pos_y);

    SDL_Rect player1_life_rects[PLAYER_MAX_LIFE];
    for(int i = 0; i < PLAYER_MAX_LIFE; i++){
        player1_life_rects[i].x = i * life_width;
        player1_life_rects[i].y = (max_y * wall_height) - life_height;
        player1_life_rects[i].w = life_width;
        player1_life_rects[i].h = life_height;
    }

//GAME LOOP
    for(;;){
        //get inputs
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                goto quit;
            }else{
                if(event.type == SDL_KEYUP && player1->is_alive){
                    switch (event.key.keysym.sym){
                        case SDLK_w:
                            //jump
                            player1->jump_offset = JUMP_START_OFFSET;
                            break;
                        case SDLK_s:
                            //shoot
                            player1->is_shooting = 0;
                            break;
                        default:
                            break;
                    }
                }if(event.type == SDL_KEYDOWN && player1->is_alive){
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            goto quit;
                            break;
                        case SDLK_a:
                            int update_x = player1->rect.x - 3;
                            if(get_tile_value(level, player1->rect.y, update_x) != 1 && get_tile_value(level, player1->rect.y + player1->rect.h, update_x) != 1){
                                player1->rect.x -= 3;
                                p1_shoot = p1_left_shoot;
                                player1->direction = 1;
                            }
                            break;
                        case SDLK_d:
                            update_x = player1->rect.x + player1->rect.w + 3;
                            if(get_tile_value(level, player1->rect.y, update_x) != 1 && get_tile_value(level, player1->rect.y + player1->rect.h, update_x) != 1){
                                player1->rect.x += 3;
                                p1_shoot = p1_right_shoot;
                                player1->direction = 0;
                            }
                            break;
                        case SDLK_s:
                            //shoot
                            if(player1->is_shooting == 0){
                               player1->is_shooting = 1; 
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
            if(p1_right == p1_right_00 && p1_left == p1_left_00){
                p1_right = p1_right_01;
                p1_left = p1_left_01;
                p1_right_shoot = p1_right_shoot_01;
                p1_left_shoot = p1_left_shoot_01;
                p1_right_jump = p1_right_jump_01;
                p1_left_jump = p1_left_jump_01;
                p1_right_falling = p1_right_fall_01;
                p1_left_falling = p1_left_fall_01;
            }else if(p1_right == p1_right_01 && p1_left == p1_left_01){
                p1_right = p1_right_00;
                p1_left = p1_left_00;
                p1_right_shoot = p1_right_shoot_00;
                p1_left_shoot = p1_left_shoot_00;
                p1_right_jump = p1_right_jump_00;
                p1_left_jump = p1_left_jump_00;
                p1_right_falling = p1_right_fall_00;
                p1_left_falling = p1_left_fall_00;
            }
        }
        
        //draw
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

        //draw p1
        if(player1->is_alive){
            //update player sprites for animations
            if(player1->direction == 0){
                p1_jump = p1_right_jump;
                p1_shoot = p1_right_shoot;
                p1_falling = p1_right_falling;
                p1_texture = p1_right;
            }else if(player1->direction == 1){
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
            if(player1->jumping_state != 1 && apply_gravity(level, player1->rect) == 1){
                player1->jumping_state = -1;
                player1->rect.y += 1;
            }
            SDL_RenderCopy(renderer, p1_texture, NULL, &player1->rect);
        }
        SDL_RenderPresent(renderer);
    }

quit:
    //textures
    if (p1_left_00){
        SDL_DestroyTexture(p1_left_00);
    }
    if (p1_left_01){
        SDL_DestroyTexture(p1_left_01);
    }    
    if (p1_right_00){
        SDL_DestroyTexture(p1_right_00);
    }    
    if (p1_right_01){
        SDL_DestroyTexture(p1_right_01);
    }
    if (p1_left_shoot_00){
        SDL_DestroyTexture(p1_left_shoot_00);
    }
    if (p1_left_shoot_01){
        SDL_DestroyTexture(p1_left_shoot_01);
    }    
    if (p1_right_shoot_00){
        SDL_DestroyTexture(p1_right_shoot_00);
    }
    if (p1_right_shoot_01){
        SDL_DestroyTexture(p1_right_shoot_01);
    }
    if (p1_left_jump_00){
        SDL_DestroyTexture(p1_left_jump_00);
    }
    if (p1_left_jump_01){
        SDL_DestroyTexture(p1_left_jump_01);
    }    
    if (p1_right_jump_00){
        SDL_DestroyTexture(p1_right_jump_00);
    }    
    if (p1_right_jump_01){
        SDL_DestroyTexture(p1_right_jump_01);
    }
    if (p1_left_fall_00){
        SDL_DestroyTexture(p1_left_fall_00);
    }
    if (p1_left_fall_01){
        SDL_DestroyTexture(p1_left_fall_01);
    }    
    if (p1_right_fall_00){
        SDL_DestroyTexture(p1_right_fall_00);
    }
    if (p1_right_fall_01){
        SDL_DestroyTexture(p1_right_fall_01);
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
//clang -I .\SDL2-2.0.10\include\ -o main.exe main.c -L .\SDL2-2.0.10\lib\x64\ -l SDL2 .\cpu/map/map.c .\cpu/player/player.c