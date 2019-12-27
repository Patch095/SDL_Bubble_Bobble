#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

level_t *load_level(const char* file_name){
    //open and read file
    FILE *f = NULL;
    fopen_s(&f, file_name, "rb");
    if(!f){
        printf("ERROR! open file\n");
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(file_size);

    size_t rlen = fread(data, 1, file_size, f);
    if (rlen != file_size){
        printf("ERROR! reading the level file %s\n", file_name);
        return NULL;
    }
    fclose(f);

    //create the new level
    level_t *new_level = malloc(sizeof(level_t));
    if(!new_level){
        return NULL;
    }
    new_level->level_name = file_name;

    //load file.h in new level
    int index_x = 0;
    int index_y = 0;
    int counter = 0;
    for (size_t i = 0; i < file_size; i++){
        char symbol = data[i];  
        switch(symbol){
            case '\n' | '\r':
                index_y++;
                index_x = 0;
                break;
            case ' ':
                continue;
                break;
            case '0':
                new_level->level_layout[index_y][index_x] = 0;
                index_x++;
                break;
            case '1':
                new_level->level_layout[index_y][index_x] = 1;
                index_x++;
                break;
            case '2':
                new_level->level_layout[index_y][index_x] = 2;
                index_x++;
                break;
            default:
                //printf("%c = ", symbol);
                //int int_symbol = (int)symbol;
                //printf("%d \n", int_symbol);
                //new_level->level_layout[index_y][index_x] = symbol;
                //index_x++;
                break;
        }
    }

    free(data);
    return new_level;
}