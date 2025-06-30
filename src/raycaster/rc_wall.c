#include "ray_caster.h"

void rc_render_wall_projection(raycaster_t *rc){
    if(!rc) return;

    // Saving On Calling Tan Thousands Of Times Per Frame
    float tan_half_fov = tan(rc->fov * 0.5f);

    for(int i = 0; i < rc->ray_count; i++){
        int ray_data = rc->rays[i].collision_data - 1;
        float perp_distance = cos(rc->rays[i].angle - rc->player->angle) * rc->rays[i].dist;
        float dist_to_proj = (rc->res.x * 0.5f) / tan_half_fov;
        float proj_wall_h = dist_to_proj * (rc->board->cell_size / perp_distance);

        float wall_start_y = (rc->res.y * 0.5) - (proj_wall_h * 0.5f);
        float wall_end_y = wall_start_y + proj_wall_h;
        if(wall_start_y < 0) wall_start_y = 0;
        if(wall_end_y > rc->res.y) wall_end_y = rc->res.y;

        for(int j = 0; j < wall_start_y; j++){
            render_pixel(i, j, 0xFF333333);
        }
        
        int texture_offset_x = rc->rays[i].hit_vertical? (int)rc->rays[i].hit_y % rc->board->cell_size: (int)rc->rays[i].hit_x % rc->board->cell_size;
        for(int j = wall_start_y; j < wall_end_y; j++){
            int dist_from_top = j + (proj_wall_h * 0.5f) - (rc->res.y * 0.5f);
            int texture_offset_y = dist_from_top * ((float)rc->textures[ray_data].height / proj_wall_h);

            u32 texel_color = rc->textures[ray_data].buffer[(rc->textures[ray_data].width * texture_offset_y) + texture_offset_x];
            if(rc->rays[i].hit_vertical) rc_change_color_intensity(&texel_color, 0.75);

            render_pixel(i, j, texel_color);
        }

        for(int j = wall_end_y; j < rc->res.y; j++){
            render_pixel(i, j, 0xFF777777);
        }
    }

}