#include "bubble.h"
#include <stdlib.h>

bubble_t *bubble_init(int player_id){
    bubble_t *new_bubble = malloc(sizeof(bubble_t));
    if(!new_bubble){
        return NULL;
    }

    new_bubble->owner_id = player_id;
    new_bubble->duration = 0;
    new_bubble->direction = 0;
    new_bubble->status = 0;
    new_bubble->absorbed_frame_animation = 0;
    
    return new_bubble;
}

int on_bubble_collision(bubble_t *bubble, int collider_id, SDL_Rect collider){
    //player collision
    if(collider_id == 0 || collider_id == 1){
        if((collider.y + collider.h) >= bubble->rect.y && (collider.y + collider.h) <= (bubble->rect.y + (bubble->rect.h /4))){
            if(collider.x >= bubble->rect.x && collider.x <= (bubble->rect.x + bubble->rect.w)){
                return 2;
            }
            if((collider.x + collider.w) >= bubble->rect.x && (collider.x + collider.w) <= (bubble->rect.x + bubble->rect.w)){
                return 2;
            }
            if((collider.x + (collider.w / 2)) >= bubble->rect.x && (collider.x + (collider.w / 2)) <= (bubble->rect.x + bubble->rect.w)){
                return 2;
            }
        }
        return 1;
    }else if(collider_id >= 2){
        return 3;
    }
    return 0;
}

int bubble_update(bubble_t *bubble){
    //bubble not activeted
    if(bubble->status == 0){
        return -1;
    }

    //shoot bubble
    if(bubble->shoot_distance_offset > 0){
        bubble->rect.x += (1 * bubble->direction);
        bubble->shoot_distance_offset--;
        if(bubble->shoot_distance_offset <= 0 && bubble->status == 1){
            bubble->status = 2;
        }
    }
      
    //float bubble
    if(bubble->status != 1 && bubble->shoot_distance_offset <= 8){
        if(bubble->duration % 2 == 0){
            bubble->rect.y--;
        } 
    }

    bubble->duration--;
    
    if(bubble->duration <= 0){
        return 0;
    }
    return 1;
}

void reset_bubble(bubble_t *bubble){
    bubble->status = 0;
    bubble->direction = 0;
    bubble->duration = 0; 
}