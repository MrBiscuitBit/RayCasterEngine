#pragma once

#include "render.h"
#include "textures.h"

#define PI 3.14159265f
#define TWO_PI 6.28318530f

#define NUM_SPRITES 6

typedef struct RC_Vec2     vec2_t;
typedef struct RC_Vec2i    vec2i_t;

typedef struct Ray         ray_t;
typedef struct RC_Board    rc_board_t;
typedef struct RC_Sprite   rc_sprite_t;
typedef struct RC_Player   rc_player_t;
typedef struct RayCaster   raycaster_t;

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

struct RC_Sprite{
    vec2_t pos;
    float dist;
    float angle;
    int texture_id;
    int visible;
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
    vec2i_t res;
    int texture_count;
    texture_t *textures;
    rc_sprite_t *sprites;
};

// Initialization
rc_player_t   *rc_init_player(float pos_x, float pos_y);
rc_board_t    *rc_load_board(const char *board_path, int cell_size);
raycaster_t   *init_raycaster(int res_width, int res_height);
rc_sprite_t   *init_sprites();
rc_sprite_t    create_sprite(float x, float y, int texture_id);

// Core Logic

void           rc_normalize_angle(float *angle);
float          rc_vec_mag(float x0, float y0, float x1, float y1);
int            rc_map_has_wall_at(rc_board_t *board, float x, float y);

void           rc_cast_ray(raycaster_t *rc, float ray_angle, int col_id);
void           rc_cast_all_rays(raycaster_t *rc);
void           rc_move_player(rc_player_t *player, rc_board_t *board, float delta_time);
void           rc_update(raycaster_t *rc, float delta_time);


// Input
void           rc_input_player(rc_player_t *player, const bool *key_state);

// Rendering
void           rc_render_map_tiles(rc_board_t *board, float scale_factor);
void           rc_render_map_player(rc_player_t *player, float scale_factor);
void           rc_render_map_rays(raycaster_t *rc, float scale_factor);
void           rc_render_map_sprites(rc_sprite_t *sprites, float scale_factor);
void           rc_render_wall_projection(raycaster_t *rc);
void           rc_render_sprite_projection(raycaster_t *rc);

// Cleanup
void           rc_clean_board(rc_board_t *rc_board);
void           rc_clean_up(raycaster_t *rc);