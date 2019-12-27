#define SDL_MAIN_HANDLED
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#include "cpu/map/map.h"
#include "cpu/player/player.h"

#define PLAYER_ONE_RIGHT_00 "textures/p1_right_00.png"
#define PLAYER_ONE_RIGHT_01 "textures/p1_right_01.png"

#define PLAYER_ONE_LEFT_00 "textures/p1_left_00.png"
#define PLAYER_ONE_LEFT_01 "textures/p1_left_01.png"

#define MAP_TILE_WALL "textures/brick.png"

#define FILE_PATH "cpu/map/level_"
//player 20x20
//map_tile 40x40
//x=12 y=12

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

    player->x = spawn_pos_x;
    player->y = spawn_pos_y;
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

    if(!image_p1_right_00 | !image_p1_right_01 | !image_p1_left_00 | !image_p1_left_01){
        SDL_Log("Unable to load image: %s", SDL_GetError());
        goto quit;
    }

    SDL_Texture *p1_right_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_right_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);

    SDL_Texture *p1_left_00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);
    SDL_Texture *p1_left_01 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, p1_width, p1_height);

    if (!p1_right_00 | !p1_right_01 | !p1_left_00 | !p1_left_01){
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
    player_t *player1 = insered_coin(0);
    //int p1x = 0;
    //int p1y = 0;
    SDL_Rect p1_rect;
    p1_rect.w = p1_width;
    p1_rect.h = p1_height;

    int time = 0;
    int p1_direction = 0;   //0 = dx, 1 = sx
    SDL_Texture *p1_right = p1_right_00;
    SDL_Texture *p1_left = p1_left_00;
    SDL_Texture *p1_texture = p1_right;

//Level load
    int level_number = 00;
    int p1_spawn_tile_x = 0;
    int p1_spawn_tile_y = 0;
    if(0 <= level_number && level_number <= 99){
        //all level beated, YOU WIN!, game over
    }
    level_t *level = load_level_from_file(++level_number, &p1_spawn_tile_x, &p1_spawn_tile_y);
    //SDL_Log("after load lv: x %d, y %d\n", p1_spawn_tile_x, p1_spawn_tile_y);

    //level_t *level = load_level("cpu/map/level_01.h");
    //level_t *level = load_level("level_01.h");
    if(!level){
        SDL_Log("error load level %d", level_number);
    }

    /*
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
            //set player spawn
            if(level->level_layout[y][x] == 2){
                p1x = x * wall_width;
                p1y = (y * wall_height) + (wall_height - p1_height);
            }
        }
    }*/
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
            //set player spawn
            /*if(level->level_layout[y][x] == 2){
               player1->x = x * wall_width;
               player1->y = (y * wall_height) + (wall_height - p1_height);
               SDL_Log("x %d, y %d\n", x, y);
               SDL_Log("x %d, y %d\n", player1->x, player1->y);
            }*/
        }
    }

    //SDL_Log("x %d, y %d\n", p1_spawn_tile_x, p1_spawn_tile_y);
    int p1_spawn_pos_x = p1_spawn_tile_x * wall_width;
    int p1_spawn_pos_y = (p1_spawn_tile_y * wall_height) + (wall_height - p1_height);
    //SDL_Log("x %d, y %d\n", p1_spawn_pos_x, p1_spawn_pos_y);
    //SDL_Log("x %d, y %d\n", p1_spawn_tile_x * wall_width, (p1_spawn_tile_y * wall_height) + (wall_height - p1_height));
    spawn_player_in_level(player1, p1_spawn_pos_x, p1_spawn_pos_y);
    //SDL_Log("x %d, y %d\n", player1->x, player1->y);

//GAME LOOP
    for(;;){
        //get inputs
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                goto quit;
            }else{
                if(event.type == SDL_KEYUP){
                    switch (event.key.keysym.sym){
                        case SDLK_w:
                            //jump
                            break;
                        case SDLK_s:
                            //shoot
                            break;
                        default:
                            break;
                    }
                }if(event.type == SDL_KEYDOWN){
                    switch (event.key.keysym.sym){
                        case SDLK_ESCAPE:
                            goto quit;
                            break;
                        case SDLK_a:
                            //p1x -= 3;
                            player1->x -= 3;
                            p1_direction = 1;
                            //p1_texture = p1_left;
                            break;
                        case SDLK_d:
                            //p1x += 3;
                            player1->x += 3;
                            p1_direction = 0;
                            //p1_texture = p1_right;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        //update player sprites for animations
        if(p1_direction == 0){
            p1_texture = p1_right;
        }else if(p1_direction == 1){
            p1_texture = p1_left;
        }

        time++;
        if(time % 40 == 0){
            if(p1_right == p1_right_00 && p1_left == p1_left_00){
                p1_right = p1_right_01;
                p1_left = p1_left_01;
            }else if(p1_right == p1_right_01 && p1_left == p1_left_01){
                p1_right = p1_right_00;
                p1_left = p1_left_00;
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

        //draw p1
        //p1_rect.x = p1x;
        //p1_rect.y = p1y;

        p1_rect.x = player1->x;
        p1_rect.y = player1->y;

        SDL_RenderCopy(renderer, p1_texture, NULL, &p1_rect);

        SDL_RenderPresent(renderer);
    }

quit:

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