#include "ray_caster.h"

void rc_render_map(rc_board_t *board, float scale_factor){
    /*
        SDL_FRect map_bg = {0, 0, board->dimensions.x * scale_factor, board->dimensions.y * scale_factor};
        SDL_SetRenderDrawColorFloat(ren, CLR_LOGL);
        SDL_RenderFillRect(ren, &map_bg);
    */
    int cell_size = board->cell_size * scale_factor;
    for(int i = 0; i < board->rows; i++){
        for(int j = 0; j < board->cols; j++){
            int tile_x = j * board->cell_size * scale_factor;
            int tile_y = i * board->cell_size * scale_factor;
            /*
                (board->map[i][j] != 0)? SDL_SetRenderDrawColorFloat(ren,  CLR_WHITE): SDL_SetRenderDrawColorFloat(ren, CLR_BLACK);
                SDL_FRect tile_rect = {tile_x, tile_y, board->cell_size * scale_factor - 1, board->cell_size * scale_factor - 1};
                SDL_RenderFillRect(ren, &tile_rect);
            */
            u32 color = (board->map[i][j] != 0)? 0xFFFFFFFF: 0xFF000000;
            render_rect(tile_x, tile_y, cell_size, cell_size, color);
        }
    }
}

void rc_render_player(rc_player_t *player, float scale_factor){
    /*
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
    */

    float player_x = player->pos.x - (player->width * 0.5f);
    float player_y = player->pos.y - (player->height * 0.5f);
    render_rect(
        player_x * scale_factor,
        player_y * scale_factor,
        player->width * scale_factor,
        player->height * scale_factor,
        0xFF00FFFF);
    render_line(
        player->pos.x * scale_factor, 
        player->pos.y * scale_factor,
        player->pos.x * scale_factor + cos(player->angle) * 50 * scale_factor, 
        player->pos.y * scale_factor + sin(player->angle) * 50 * scale_factor,
        0xFF00FFFF
    );
}

void rc_render_rays(raycaster_t *rc, float scale_factor){
    for(int i = 0; i < rc->ray_count; i++){
        render_line(
            rc->player->pos.x * scale_factor,
            rc->player->pos.y * scale_factor,
            rc->rays[i].hit_x * scale_factor,
            rc->rays[i].hit_y * scale_factor,
            0xFF0000FF
        );
    }
}

void rc_prepare_wall_projection(raycaster_t *rc){
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
            render_pixel(i, j, 0xFF333333);
            //rc->ren->color_buffer[(rc->res.x * j) + i] = 0xFF333333;
        }
        
        int texture_offset_x = rc->rays[i].hit_vertical? (int)rc->rays[i].hit_y % rc->board->cell_size: (int)rc->rays[i].hit_x % rc->board->cell_size;
        for(int j = wall_pixel_start; j < wall_pixel_end; j++){
            int dist_from_top = j + (wall_strip_h * 0.5f) - (rc->res.y * 0.5f);
            int texture_offset_y = dist_from_top * ((float)rc->wall_textures[ray_data].height / wall_strip_h);
            uint32_t texel_color = rc->wall_textures[ray_data].buffer[(rc->wall_textures[ray_data].width * texture_offset_y) + texture_offset_x];
            render_pixel(i, j, texel_color);
            //rc->color_buffer[(rc->res.x * j) + i] = texel_color;
        }

        for(int j = wall_pixel_end; j < rc->res.y; j++){
            render_pixel(i, j, 0xFF777777);
            //rc->ren->color_buffer[(rc->res.x * j) + i] = 0xFF777777;
        }
    }

}