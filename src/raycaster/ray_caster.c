#include "ray_caster.h"

int map_has_wall_at(rc_board_t *board, float x, float y){
    if(!board || !board->map) return -1;
    if(x < 0 || x > board->dimensions.x || y < 0 || y > board->dimensions.y) return 1;
    int map_index_x = floor(x / board->cell_size);
    int map_index_y = floor(y / board->cell_size);
    return board->map[map_index_y][map_index_x];
}

void cast_ray(raycaster_t *rc, float ray_angle, int col_id){
    if(!rc) return;

    ray_angle = normalize_angle(ray_angle);

    float tan_a = tan(ray_angle); // Tangent of Ray Angle

    int facing_down = ray_angle > 0 && ray_angle < PI;
    int facing_up = !facing_down;

    int facing_right = ray_angle < PI * 0.5f || ray_angle > PI * 1.5f; 
    int facing_left = !facing_right;

    float x_intercept, y_intercept;
    float x_step, y_step;

    // HORIZONTAL RAY/WALL INTERCECTION

    int horz_wall_hit = 0; // Boolean value
    float horz_hit_x = 0;
    float horz_hit_y = 0;
    int horz_wall_data = 0;

    // Find The y-coordinate Of The Closest Horizontal Grid Intersection By Rounding
    y_intercept = floor(rc->player->pos.y / rc->board->cell_size) * rc->board->cell_size;
    y_intercept += facing_down? rc->board->cell_size: 0;

    // Find The x-coordinate Of The Closest Horizontal Grid Using Trig
    x_intercept = rc->player->pos.x + (y_intercept - rc->player->pos.y) / tan_a;

    // Calculate The Increment x_step And y_step
    y_step = rc->board->cell_size * (facing_up? -1: 1);

    x_step = rc->board->cell_size / tan_a;
    x_step *= (facing_left && x_step > 0)? -1: 1;
    x_step *= (facing_right && x_step < 0)? -1: 1;

    // Grab The Horizontal Hits And Increment
    float next_horz_hit_x = x_intercept;
    float next_horz_hit_y = y_intercept;

    while(next_horz_hit_x > 0 && next_horz_hit_x < rc->board->dimensions.x && next_horz_hit_y > 0 && next_horz_hit_y < rc->board->dimensions.y){
        float x_to_check = next_horz_hit_x;
        float y_to_check = next_horz_hit_y + (facing_up? -1: 0);

        int map_data = map_has_wall_at(rc->board, x_to_check, y_to_check);
        if(map_data != 0){
            horz_hit_x = next_horz_hit_x;
            horz_hit_y = next_horz_hit_y;
            horz_wall_data = map_data;
            horz_wall_hit = 1;
            break;
        }
        else{
            next_horz_hit_x += x_step;
            next_horz_hit_y += y_step;
        }
    }

    // VERTICAL RAY/WALL INTERSECTION

    int vert_wall_hit = 0; // Boolean value
    float vert_hit_x = 0;
    float vert_hit_y = 0;
    int vert_wall_data = 0;

    // Find The x-coordinate Of The Closest Vertical Grid Intersection By Rounding
    x_intercept = floor(rc->player->pos.x / rc->board->cell_size) * rc->board->cell_size;
    x_intercept += facing_right? rc->board->cell_size: 0;

    // Find The y-coordinate Of The Closest Vertical Grid Using Trig
    y_intercept = rc->player->pos.y + (x_intercept - rc->player->pos.x) * tan_a;

    // Calculate The Increment x_step And y_step
    x_step = rc->board->cell_size * (facing_left? -1: 1);

    y_step = rc->board->cell_size * tan_a;
    y_step *= (facing_up && y_step > 0)? -1: 1;
    y_step *= (facing_down && y_step < 0)? -1: 1;

    // Grab The Vertical Hits And Increment
    float next_vert_hit_x = x_intercept;
    float next_vert_hit_y = y_intercept;

    while(next_vert_hit_x > 0 && next_vert_hit_x < rc->board->dimensions.x && next_vert_hit_y > 0 && next_vert_hit_y < rc->board->dimensions.y){
        float x_to_check = next_vert_hit_x + (facing_left? -1: 0);
        float y_to_check = next_vert_hit_y;

        int map_data = map_has_wall_at(rc->board, x_to_check, y_to_check);
        if(map_data != 0){
            vert_hit_x = next_vert_hit_x;
            vert_hit_y = next_vert_hit_y;
            vert_wall_data = map_data;
            vert_wall_hit = 1;
            break;
        }
        else{
            next_vert_hit_x += x_step;
            next_vert_hit_y += y_step;
        }
    }

    float horizontal_dist = horz_wall_hit? (pow(horz_hit_x - rc->player->pos.x, 2.0f) + pow(horz_hit_y - rc->player->pos.y, 2.0f)): __FLT_MAX__;
    float vertical_dist = vert_wall_hit? (pow(vert_hit_x - rc->player->pos.x, 2.0f) + pow(vert_hit_y - rc->player->pos.y, 2.0f)): __FLT_MAX__;

    if(vertical_dist < horizontal_dist){
        rc->rays[col_id].dist = sqrt(vertical_dist);
        rc->rays[col_id].collision_data = vert_wall_data;
        rc->rays[col_id].hit_vertical = 1;
        rc->rays[col_id].hit_x = vert_hit_x;
        rc->rays[col_id].hit_y = vert_hit_y;
    }
    else{
        rc->rays[col_id].dist = sqrt(horizontal_dist);
        rc->rays[col_id].collision_data = horz_wall_data;
        rc->rays[col_id].hit_vertical = 0;
        rc->rays[col_id].hit_x = horz_hit_x;
        rc->rays[col_id].hit_y = horz_hit_y;
    }
    rc->rays[col_id].angle = ray_angle;
}

void cast_all_rays(raycaster_t *rc){
    if(!rc) return;
    for(int col_id = 0; col_id < rc->ray_count; col_id++){
        float ray_angle = rc->player->angle + atan(((col_id - rc->ray_count * 0.5f) / rc->dist_to_proj));
        cast_ray(rc, ray_angle, col_id);
    }
}

float normalize_angle(float angle){
    angle = fmod(angle, TWO_PI);
    if(angle < 0) angle += TWO_PI;
    return angle;
}

void input_player(rc_player_t *player, const bool *key_state){
    if(!player || !key_state) return;

    if(key_state[SDL_SCANCODE_RIGHT]) player->turn_right = 1;
    if(key_state[SDL_SCANCODE_LEFT]) player->turn_left = -1;
    if(key_state[SDL_SCANCODE_W]) player->walk_forward = 1;
    if(key_state[SDL_SCANCODE_S]) player->walk_backward = -1;
}

void move_player(rc_player_t *player, rc_board_t *board, float delta_time){

    int turn_dir = player->turn_right + player->turn_left;
    player->angle += turn_dir * player->turn_speed * delta_time;

    int walk_dir = player->walk_forward + player->walk_backward;
    float move_step = walk_dir * player->walk_speed * delta_time;

    float new_player_x = player->pos.x + cos(player->angle) * move_step;
    float new_player_y = player->pos.y + sin(player->angle) * move_step;

    if(!map_has_wall_at(board, new_player_x, new_player_y)){ 
        player->pos.x = new_player_x;
        player->pos.y = new_player_y;
    }

    player->turn_left = 0;
    player->turn_right = 0;
    player->walk_forward = 0;
    player->walk_backward = 0;
}

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

void rc_clean_up(raycaster_t *rc){  
    if(!rc) return;

    if(rc->board){
        rc_clean_board(rc->board);
        free(rc->board);
        rc->board = NULL;
    }

    if(rc->player){
        free(rc->player);
        rc->player = NULL;
    }

    if(rc->rays){
        free(rc->rays);
        rc->rays = NULL;
    }
}

void rc_update(raycaster_t *rc, float delta_time){
    if(!rc) return;
    move_player(rc->player, rc->board, delta_time);
    cast_all_rays(rc);
}