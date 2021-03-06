#define SDL_MAIN_HANDLED
#include <SDL.h>

#define PLAYER_WH 16

#define PLAYER_SPAWN_LIFE 3
#define PLAYER_MAX_LIFE 5

#define INVINCIBILITY_TIME 30

typedef struct player{
    SDL_Rect rect;
    int life;
    unsigned long score;
    int player_id;      //0 = p1    1 = p2

    int direction;      //-1 = left 1 = right
    int is_shooting;    //0 = no    1 = yes
    int jumping_state;  //0 = no    1 = jumping     -1 = falling
    int jump_offset;
    int is_alive;       //0 = no    1 = yes

    int animation_frame;

    int respawn_invicibility;
} player_t;

player_t *player_init(int player_id);

void insered_coin(player_t *player);

void player_spawn(player_t *player, int start_x, int start_y);

void add_a_life(player_t *player);
void on_player_death(player_t *player);

//void player_wall_collision(player_t *player, SDL_Rect player_rect, SDL_Rect collider);

//int can_collide(int new_x, SDL_Rect player_rect, SDL_Rect collider);

/*

The log10, abs, and floor functions are provided by math.h. For example:

int nDigits = floor(log10(abs(the_integer))) + 1;

*/