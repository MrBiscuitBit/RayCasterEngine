#include "ray_caster.h"

rc_player_t *rc_init_player(float pos_x, float pos_y){
    rc_player_t *player = malloc(sizeof(rc_player_t));
    if(!player){
        fprintf(stderr, "Failed To Allocate Player\n");
        return NULL;
    }

    player->pos.x = pos_x;
    player->pos.y = pos_y;
    player->width = player->height = 15;
    player->angle = PI * 0.5f;
    player->turn_right = 0; 
    player->turn_right = 0;
    player->walk_forward = 0;
    player->walk_backward = 0;
    player->walk_speed = 100;
    player->turn_speed = 75 * (PI / 180);
    return player;
}

void rc_input_player(rc_player_t *player, const bool *key_state){
    if(!player || !key_state) return;

    if(key_state[SDL_SCANCODE_RIGHT]) player->turn_right = 1;
    if(key_state[SDL_SCANCODE_LEFT]) player->turn_left = -1;
    if(key_state[SDL_SCANCODE_W]) player->walk_forward = 1;
    if(key_state[SDL_SCANCODE_S]) player->walk_backward = -1;
}

void rc_move_player(rc_player_t *player, rc_board_t *board, float delta_time){

    int turn_dir = player->turn_right + player->turn_left;
    player->angle += turn_dir * player->turn_speed * delta_time;

    int walk_dir = player->walk_forward + player->walk_backward;
    float move_step = walk_dir * player->walk_speed * delta_time;

    float new_player_x = player->pos.x + cos(player->angle) * move_step;
    float new_player_y = player->pos.y + sin(player->angle) * move_step;

    if(!rc_map_has_wall_at(board, new_player_x, new_player_y)){ 
        player->pos.x = new_player_x;
        player->pos.y = new_player_y;
    }

    player->turn_left = 0;
    player->turn_right = 0;
    player->walk_forward = 0;
    player->walk_backward = 0;
}