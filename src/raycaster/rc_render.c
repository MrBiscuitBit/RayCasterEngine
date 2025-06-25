#include "rc_render.h"

void render_color_buffer(SDL_Renderer *ren, raycaster_t *rc){
    SDL_UpdateTexture(rc->ren->color_buffer_texture, NULL, rc->ren->color_buffer, (int)((uint32_t)rc->res.x * sizeof(uint32_t)));
    SDL_RenderTexture(ren, rc->ren->color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(raycaster_t *rc, uint32_t clear_color){
    for(int x = 0; x < rc->res.x * rc->res.y; x++){
            rc->ren->color_buffer[x] = clear_color;
    }
}

void render_map(SDL_Renderer *ren, rc_board_t *board, float scale_factor){

    SDL_FRect map_bg = {0, 0, board->dimensions.x * scale_factor, board->dimensions.y * scale_factor};
    SDL_SetRenderDrawColorFloat(ren, CLR_LOGL);
    SDL_RenderFillRect(ren, &map_bg);

    for(int i = 0; i < board->rows; i++){
        for(int j = 0; j < board->cols; j++){
            int tile_x = j * board->cell_size * scale_factor;
            int tile_y = i * board->cell_size * scale_factor;
            
            (board->map[i][j] != 0)? SDL_SetRenderDrawColorFloat(ren,  CLR_WHITE): SDL_SetRenderDrawColorFloat(ren, CLR_BLACK);
            SDL_FRect tile_rect = {tile_x, tile_y, board->cell_size * scale_factor - 1, board->cell_size * scale_factor - 1};
            SDL_RenderFillRect(ren, &tile_rect);
        }
    }

}

void render_player(SDL_Renderer *ren, rc_player_t *player, float scale_factor){
    SDL_SetRenderDrawColorFloat(ren, CLR_YELLOW);
    SDL_FRect player_rect = {
        player->pos.x * scale_factor,
        player->pos.y * scale_factor,
        player->width * scale_factor,
        player->height * scale_factor
    };
    player_rect.x -= player_rect.w * 0.5f;
    player_rect.y -= player_rect.h * 0.5f;
    SDL_RenderFillRect(ren, &player_rect);

    SDL_RenderLine(
        ren,
        player->pos.x * scale_factor, 
        player->pos.y * scale_factor,
        player->pos.x * scale_factor + cos(player->angle) * 30 * scale_factor, 
        player->pos.y * scale_factor + sin(player->angle) * 30 * scale_factor
    );
}

void render_rays(SDL_Renderer *ren, raycaster_t *rc, float scale_factor){
    SDL_SetRenderDrawColorFloat(ren, CLR_RED);
    for(int i = 0; i < rc->ray_count; i++){
        SDL_RenderLine(
            ren,
            rc->player->pos.x * scale_factor,
            rc->player->pos.y * scale_factor,
            rc->rays[i].hit_x * scale_factor,
            rc->rays[i].hit_y * scale_factor
        );
    }
}

void prepare_wall_projection(raycaster_t *rc){
    if(!rc) return;

    // Saving On Calling Tan Thousands Of Times Per Frame
    float tan_half_fov = tan(rc->fov * 0.5f);

    for(int i = 0; i < rc->ray_count; i++){
        int ray_data = rc->rays[i].collision_data - 1;
        float perp_distance = cos(rc->rays[i].angle - rc->player->angle) * rc->rays[i].dist;
        float dist_to_proj = (rc->res.x * 0.5f) / tan_half_fov;
        float proj_wall_h = dist_to_proj * (rc->board->cell_size / perp_distance);

        int wall_strip_h = (int)proj_wall_h;

        float wall_pixel_start = (rc->res.y * 0.5) - (wall_strip_h * 0.5f);
        float wall_pixel_end = wall_pixel_start + wall_strip_h;
        if(wall_pixel_start < 0) wall_pixel_start = 0;
        if(wall_pixel_end > rc->res.y) wall_pixel_end = rc->res.y;

        for(int j = 0; j < wall_pixel_start; j++){
            rc->ren->color_buffer[(rc->res.x * j) + i] = 0xFF333333;
        }
        
        int texture_offset_x = rc->rays[i].hit_vertical? (int)rc->rays[i].hit_y % rc->board->cell_size: (int)rc->rays[i].hit_x % rc->board->cell_size;
        for(int j = wall_pixel_start; j < wall_pixel_end; j++){
            int dist_from_top = j + (wall_strip_h * 0.5f) - (rc->res.y * 0.5f);
            int texture_offset_y = dist_from_top * ((float)rc->ren->wall_textures[ray_data].height / wall_strip_h);
            uint32_t texel_color = rc->ren->wall_textures[ray_data].buffer[(rc->ren->wall_textures[ray_data].width * texture_offset_y) + texture_offset_x];
            rc->ren->color_buffer[(rc->res.x * j) + i] = texel_color;
        }

        for(int j = wall_pixel_end; j < rc->res.y; j++){
            rc->ren->color_buffer[(rc->res.x * j) + i] = 0xFF777777;
        }
    }

}

void rc_render_all(SDL_Renderer *ren, raycaster_t *rc, float scale_factor, int ren_map, int ren_player, int ren_rays, int ren_proj){
    if(!ren || !rc) return;

    clear_color_buffer(rc, 0xFFFFFFFF);
    if(ren_proj) prepare_wall_projection(rc);
    render_color_buffer(ren, rc);

    if(ren_map) render_map(ren, rc->board, scale_factor);
    if(ren_rays) render_rays(ren, rc, scale_factor);
    if(ren_player) render_player(ren, rc->player, scale_factor);
}