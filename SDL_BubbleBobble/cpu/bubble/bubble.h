#define SDL_MAIN_HANDLED
#include <SDL.h>

#define BUBBLE_DURATION_TIMER 300
#define SHOOT_OFFSET 40

typedef struct bubble{
    SDL_Rect rect;

    int owner_id;

    int direction;
    int shoot_distance_offset;
    int status;     //0 = inactive  // 1 = shoot    // 2 = empty    //3 = enemy inside
    
    int abserbed_id;
    int absorbed_type;

    int duration;

    int absorbed_frame_animation;
} bubble_t;

bubble_t *bubble_init(int owner_id);

int on_bubble_collision(bubble_t *bubble, int collider_id, SDL_Rect collider);

void bubble_update(bubble_t *bubble);

void reset_bubble(bubble_t *bubble);