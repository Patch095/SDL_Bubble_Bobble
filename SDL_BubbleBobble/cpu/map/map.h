#define MAP_TILES_Y 12
#define MAP_TILES_X 12

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
} level_t;

level_t *load_level(const char* file_name);