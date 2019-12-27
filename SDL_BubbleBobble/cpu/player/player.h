typedef struct player{
    int x;
    int y;
    int life;
    unsigned long score;
    const char* player_name;
} player_t;

player_t *insered_coin(int player_id);

void player_spawn(player_t *player, int start_x, int start_y);