#define SDL_MAIN_HANDLED
#include <SDL.h>

#define ABSORBE_DURATION_TIMER 120
#define SPRITE_WH 16

typedef struct enemy{
    SDL_Rect rect;

    int type_id;    //start from 2 (0 p1, 1 p2)
    //2 = base enemy
    int number_of_animation_frame;
    int animation_frame;

    int direction;
    int jump_offset;

    int status;     //0 = dead  1 = alive   2 = absorbed    3 = angry

    int special_action_chance;

    int absorbed_timer;
} enemy_t;

enemy_t *enemy_init(int enemy_id);

void enemy_update(enemy_t *enemy);

void change_direction(enemy_t *enemy);
void jump(enemy_t *enemy);

void on_death(enemy_t *enemy);