#define MAP_TILES_Y 20
#define MAP_TILES_X 16

#define TILE_H 16
#define TILE_W 32

typedef struct level_map{
    const char* level_name;
    int level_layout[MAP_TILES_Y][MAP_TILES_X];
    //0 = empty
    //1 = wall
    
    //2 = p1 spawn
    //3 = p2 spawn

    //4 = enemy spawn
    //5 = ghost spawn
    //6 = phantom spawn
    //7 = jumpy spawn
    //8 = star spawn

    //9 = UI
    int enemies_in_level;
    int enemies_alive;
} level_t;

level_t *load_level(const char* file_name);

int get_tile_value(level_t *level, int y, int x);


//player jump -= 42 pixel

//256x 224y
//player 16x 16y

//map:player    16x 14y
//map tile = 16x 8y => 16 x_tile 28 y_tile

//map tile x20  y15