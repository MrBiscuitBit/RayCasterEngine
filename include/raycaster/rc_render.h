#pragma once

#include "ray_caster.h"

void render_color_buffer(SDL_Renderer *ren, raycaster_t *rc);
void clear_color_buffer(raycaster_t *rc, uint32_t clear_color);
void render_map(SDL_Renderer *ren, rc_board_t *board, float scale_factor);
void render_player(SDL_Renderer *ren, rc_player_t *player, float scale_factor);
void render_rays(SDL_Renderer *ren, raycaster_t *rc, float scale_factor);
void prepare_wall_projection(raycaster_t *rc);
void rc_render_all(SDL_Renderer *ren, raycaster_t *rc, float scale_factor, int ren_map, int ren_player, int ren_rays, int ren_proj);