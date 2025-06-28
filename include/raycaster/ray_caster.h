#pragma once

#include "render.h"
#include "textures.h"

#define PI 3.14159265
#define TWO_PI 6.28318530

typedef struct RC_Vec2 vec2_t;
typedef struct RC_Vec2i vec2i_t;

typedef struct Ray ray_t;
typedef struct RC_Board rc_board_t;
typedef struct RC_Player rc_player_t;
typedef struct RC_Renderer rc_renderer_t;
typedef struct RayCaster raycaster_t;

struct RC_Vec2{
    float x, y;
};

struct RC_Vec2i{
    int x, y;
};

struct RC_Board{
    int **map;
    int cols, rows;
    int cell_size;
    vec2i_t dimensions;
};

struct RC_Player{
    vec2_t pos;
    float width, height;
    int turn_left, turn_right;
    int walk_forward, walk_backward;
    float angle;
    float walk_speed, turn_speed;
};

struct Ray{
    float angle;
    float hit_x, hit_y;
    float dist;
    int hit_vertical;
    int collision_data; // The Value In The Collided Wall
};

struct RayCaster{
    rc_board_t *board;
    rc_player_t *player;
    ray_t *rays;
    int ray_count;
    float dist_to_proj;
    float fov;
    rc_renderer_t *ren;
    vec2i_t res;
    texture_t *wall_textures;
};

rc_player_t *rc_init_player(float pos_x, float pos_y);
rc_board_t *rc_load_board(const char *board_path, int cell_size);
raycaster_t *init_raycaster(int res_width, int res_height);
float rc_normalize_angle(float angle);
int rc_map_has_wall_at(rc_board_t *board, float x, float y);
void rc_cast_ray(raycaster_t *rc, float ray_angle, int col_id);
void rc_cast_all_rays(raycaster_t *rc);
void rc_move_player(rc_player_t *player, rc_board_t *board, float delta_time);
void rc_clean_board(rc_board_t *rc_board);
void rc_clean_up(raycaster_t *rc);
void rc_update(raycaster_t *rc, float delta_time);
void rc_input_player(rc_player_t *player, const bool *key_state);
void rc_render_map(rc_board_t *board, float scale_factor);
void rc_render_player(rc_player_t *player, float scale_factor);
void rc_render_rays(raycaster_t *rc, float scale_factor);
void rc_prepare_wall_projection(raycaster_t *rc);