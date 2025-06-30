#include "ray_caster.h"

rc_sprite_t *init_sprites(){
    rc_sprite_t *sprites = (rc_sprite_t *)malloc(sizeof(rc_sprite_t) * NUM_SPRITES);
    if(!sprites){
        fprintf(stderr, "Failed To Allocate Spritns\n");
        return NULL;
    }

    sprites[0] = create_sprite(640, 630,  9); // barrel
    sprites[1] = create_sprite(250, 600, 11); // table
    sprites[2] = create_sprite(300, 400, 12); // guard
    sprites[3] = create_sprite(660, 690,  9); // barrel
    sprites[4] = create_sprite(250, 600, 10); // light
    sprites[5] = create_sprite( 90, 100, 13); // armor

    return sprites;
}

rc_sprite_t create_sprite(float x, float y, int texture_id){
    return (rc_sprite_t){.pos.x = x, .pos.y = y, .texture_id = texture_id};
}

void rc_render_map_sprites(rc_sprite_t *sprites, float scale_factor){
    for(int i = 0; i < NUM_SPRITES; i++){
        float sprite_size = 15;
        float sprite_x = (sprites[i].pos.x - sprite_size * 0.5f)* scale_factor;
        float sprite_y = (sprites[i].pos.y - sprite_size * 0.5f)* scale_factor;
        render_rect(
            sprite_x,
            sprite_y,
            sprite_size * scale_factor,
            sprite_size * scale_factor, 
            sprites[i].visible? 0xFF00FFFF: 0xFF444444
        );
    }
}

void rc_render_sprite_projection(raycaster_t *rc){
    if(!rc || !rc->player || !rc->sprites) return;

    rc_sprite_t *visible_sprites[NUM_SPRITES] = {0};
    int num_visible_sprites = 0;
    float fov_padding = 15 * (PI / 180);

    // Calculate Sprite Distance And Angle
    for(int i = 0; i < NUM_SPRITES; i++){
        float sprite_player_angle = rc->player->angle - atan2(rc->sprites[i].pos.y - rc->player->pos.y, rc->sprites[i].pos.x - rc->player->pos.x);
        if(sprite_player_angle > PI) sprite_player_angle -= TWO_PI;
        if(sprite_player_angle < -PI) sprite_player_angle += TWO_PI;
        sprite_player_angle = fabs(sprite_player_angle);

        if(sprite_player_angle >= (rc->fov * 0.5f) + fov_padding){
            rc->sprites[i].visible = 0;
            continue;
        }

        rc->sprites[i].visible = 1;
        rc->sprites[i].angle = sprite_player_angle;
        rc->sprites[i].dist = rc_vec_mag(rc->sprites[i].pos.x, rc->sprites[i].pos.y, rc->player->pos.x, rc->player->pos.y);
        visible_sprites[num_visible_sprites] = &rc->sprites[i];
        num_visible_sprites++;
    }

    int swapped;
    for(int i = 0; i < num_visible_sprites - 1; i++){
        swapped = 0;
        for(int j = 0; j < num_visible_sprites - i - 1; j++){
            if(visible_sprites[j]->dist < visible_sprites[j + 1]->dist){
                rc_sprite_t *tmp = visible_sprites[j + 1];
                visible_sprites[j + 1] = visible_sprites[j];
                visible_sprites[j] = tmp;
                swapped = 1;
            }
        }
        if(!swapped) break;
    }

    for(int i = 0; i < num_visible_sprites; i++){
        rc_sprite_t *sprite = visible_sprites[i];
        texture_t texture = rc->textures[sprite->texture_id];

        float perp_dist = sprite->dist * cos(sprite->angle);
        float proj_sprite_h = (rc->board->cell_size / perp_dist) * rc->dist_to_proj;
        float proj_sprite_w = proj_sprite_h * texture.aspect_ratio;

        float sprite_start_y = (rc->res.y * 0.5f) - (proj_sprite_h * 0.5f);
        if(sprite_start_y < 0) sprite_start_y = 0;

        float sprite_end_y = sprite_start_y + proj_sprite_h;
        if(sprite_end_y > rc->res.y) sprite_end_y = rc->res.y;

        float sprite_angle = atan2(sprite->pos.y - rc->player->pos.y, sprite->pos.x - rc->player->pos.x) - rc->player->angle;
        float sprite_screen_x = tan(sprite_angle) * rc->dist_to_proj;

        float sprite_left_x = (rc->res.x * 0.5f) + sprite_screen_x - (proj_sprite_w * 0.5f);
        float sprite_right_x = sprite_left_x + proj_sprite_w;


        for(int j = sprite_left_x; j < sprite_right_x; j++){
            if(j < 0 || j >= rc->ray_count || sprite->dist > rc->rays[j].dist) continue;
            float texel_width = texture.width / proj_sprite_w;
            int texture_offset_x = (j - sprite_left_x) * texel_width;
            for(int k = sprite_start_y; k < sprite_end_y; k++){

                if(j >= 0 && j < rc->res.x && k >= 0 && k < rc->res.y){
                    int dist_from_top = k + (proj_sprite_h * 0.5f) - (rc->res.y * 0.5f);
                    int texture_offset_y = dist_from_top * (texture.height / proj_sprite_h);

                    u32 texel_color = texture.buffer[(texture.width * texture_offset_y) + texture_offset_x];
                    if(texel_color != HEX_ALPHA_KEY)
                        render_pixel(j, k, texel_color);
                }

            }
        }

    }
    
}