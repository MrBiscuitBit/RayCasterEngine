#include "ray_caster.h"

int is_line_empty(const char *line){
    while(*line){
        if(!isspace(*line)) return 0;
        line++;
    }
    return 1;
}

rc_board_t *rc_load_board(const char *board_path, int cell_size){
    if(!board_path){
        fprintf(stderr, "Missing Board Path In Function: RC Load Board");
        return NULL;
    }
    FILE *board_file = fopen(board_path, "r");
    if(!board_file){
        fprintf(stderr, "Failed To Open Board Path In Function: RC Load Board");
        return NULL;
    }

    int columns, rows;
    fscanf(board_file, "{%d, %d}", &columns, &rows);  
    
    rc_board_t *new_board = malloc(sizeof(rc_board_t));
    if(!new_board){
        fprintf(stderr, "Failed To Allocate New Board In Function: RC Load Board");
        fclose(board_file);
        return NULL;
    }
    new_board->map = malloc(sizeof(int*) * rows);
    if(!new_board->map){
        fprintf(stderr, "Failed To Allocate New Board Rows In Function: RC Load Board");
        free(new_board);
        fclose(board_file);
        return NULL;
    }

    int buffer_size = 512;
    char line[buffer_size];
    for(int i = 0; i < rows; i++){
        // Allocate Row Columns
        new_board->map[i] = malloc(sizeof(int) * columns);
        if(!new_board->map[i]){
            fprintf(stderr, "Failed To Allocate New Board Columns (Row: %d) In Function: RC Load Board", i);
            rc_clean_board(new_board);
            free(new_board);
            fclose(board_file);
            return NULL;
        }
        // Fill Row Columns From File
        fgets(line, buffer_size, board_file);
        if(is_line_empty(line)){
            i--;
            continue;
        }

        char *token = strtok(line, ",\n");
        for(int j = 0; j < columns && token != NULL; j++){
            new_board->map[i][j] = atoi(token);
            token = strtok(NULL, ",\n");
        }
    }

    new_board->cell_size = cell_size;
    new_board->cols = columns;
    new_board->rows = rows;
    new_board->dimensions.x = new_board->cols * new_board->cell_size;
    new_board->dimensions.y = new_board->rows * new_board->cell_size;

    fclose(board_file);
    return new_board;
}

int rc_map_has_wall_at(rc_board_t *board, float x, float y){
    if(!board || !board->map) return -2;
    if(x < 0 || x > board->dimensions.x || y < 0 || y > board->dimensions.y) return -1;
    int map_index_x = floor(x / board->cell_size);
    int map_index_y = floor(y / board->cell_size);
    return board->map[map_index_y][map_index_x];
}

void rc_clean_board(rc_board_t *rc_board){
    if(!rc_board) return;
    if(rc_board->map != NULL){
        for(int i = 0; i < rc_board->rows; i++){
            if(rc_board->map[i])
                free(rc_board->map[i]);
        }
        free(rc_board->map);
        rc_board->map = NULL;
    }
}