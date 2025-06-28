#include "ray_caster.h"

raycaster_t *init_raycaster(int res_width, int res_height){

    raycaster_t *ray_caster = malloc(sizeof(raycaster_t));
    if(!ray_caster){
        fprintf(stderr, "Failed To Allocate RayCaster\n");
        return NULL;
    }

    ray_caster->board = rc_load_board("resrcs\\boards\\default_board.txt", 64);
    if(!ray_caster->board){
        fprintf(stderr, "Failed To Load Board\n");
    }

    ray_caster->player = rc_init_player(ray_caster->board->dimensions.x * 0.5f, ray_caster->board->dimensions.y * 0.5f);
    if(!ray_caster->player){
        fprintf(stderr, "Failed To Init Player\n");
    }

    // NoteWorthy Resolutions Are 320x240 And 1440x900
    ray_caster->res = (vec2i_t){res_width, res_height};
    ray_caster->fov = 75 * (PI / 180);
    ray_caster->ray_count = ray_caster->res.x;
    ray_caster->dist_to_proj = (ray_caster->res.x * 0.5f) / tan(ray_caster->fov * 0.5f);

    ray_caster->rays = malloc(sizeof(ray_t) * ray_caster->ray_count);
    if(!ray_caster->rays){
        rc_clean_up(ray_caster);
        return NULL;
    }
    ray_caster->wall_textures = load_wall_textures();

    // Init Color Buffer
    init_buffer(ray_caster->res.x, ray_caster->res.y);
    
    return ray_caster;
}

void rc_cast_ray(raycaster_t *rc, float ray_angle, int col_id){
    if(!rc) return;

    int dof = 0, dof_max = 24;
    ray_angle = rc_normalize_angle(ray_angle);

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

    while(dof < dof_max){
        float x_to_check = next_horz_hit_x;
        float y_to_check = next_horz_hit_y + (facing_up? -1: 0);

        int map_data = rc_map_has_wall_at(rc->board, x_to_check, y_to_check);
        if(map_data < 0) dof = dof_max;
        else if(map_data != 0){
            horz_hit_x = next_horz_hit_x;
            horz_hit_y = next_horz_hit_y;
            horz_wall_data = map_data;
            horz_wall_hit = 1;
            dof = dof_max;
        }
        else{
            next_horz_hit_x += x_step;
            next_horz_hit_y += y_step;
            dof++;
        }
    }

    dof = 0;
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

    while(dof < dof_max){
        float x_to_check = next_vert_hit_x + (facing_left? -1: 0);
        float y_to_check = next_vert_hit_y;

        int map_data = rc_map_has_wall_at(rc->board, x_to_check, y_to_check);
        if(map_data < 0) dof = dof_max;
        else if(map_data != 0){
            vert_hit_x = next_vert_hit_x;
            vert_hit_y = next_vert_hit_y;
            vert_wall_data = map_data;
            vert_wall_hit = 1;
            dof = dof_max;
        }
        else{
            next_vert_hit_x += x_step;
            next_vert_hit_y += y_step;
            dof++;
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

void rc_cast_all_rays(raycaster_t *rc){
    if(!rc) return;
    for(int col_id = 0; col_id < rc->ray_count; col_id++){
        float ray_angle = rc->player->angle + atan(((col_id - rc->ray_count * 0.5f) / rc->dist_to_proj));
        rc_cast_ray(rc, ray_angle, col_id);
    }
}

float rc_normalize_angle(float angle){
    angle = fmod(angle, TWO_PI);
    if(angle < 0) angle += TWO_PI;
    return angle;
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
    rc_move_player(rc->player, rc->board, delta_time);
    rc_cast_all_rays(rc);
}