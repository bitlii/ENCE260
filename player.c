/** @file player.c
 *  @authors Jackie Chen, Scott Lily
 *  @date 11 Oct 2020
 *  @brief Module that stores methods relating to the player and their movements in the game.
 */

#include "system.h"
#include "player.h"
#include "tinygl.h"

/**
 * Move player right on the screen.
 */
void player_move_left(player_t *player) 
{
    if (player->pos < 6) {
        // ATTACKER
        if (player->field == ATTACK_FIELD) {
            tinygl_pixel_set(tinygl_point (BOTTOM_ROW, player->pos), 0);
            player->pos++;
            tinygl_pixel_set(tinygl_point (BOTTOM_ROW, player->pos), 1);

        // DEFENDER
        } else {
            if (player->hits == 0 && player->pos < 5) {
                display_player_off(player);
                player->pos++;
                display_player_on(player);
            }

            else if (player->hits == 1) {
                display_player_off(player);
                player->pos++;
                display_player_on(player);
            }
        }
    }
}

/**
 * Move player right on the screen.
 */
void player_move_right(player_t *player) 
{
    if (player->pos > 0) {
        // ATTACKER
        if (player->field == ATTACK_FIELD) {
            tinygl_pixel_set(tinygl_point (BOTTOM_ROW, player->pos), 0);
            player->pos--;
            tinygl_pixel_set(tinygl_point (BOTTOM_ROW, player->pos), 1);

        // DEFENDER
        } else {
            display_player_off(player);
            player->pos--;
            display_player_on(player);

        }
    }
}

/**
 * Deal damage to the player, and return '1' if player dies from the hit, else 0.
 */
uint8_t player_hit(player_t *player)
{
    uint8_t dead = 0;

    display_player_off(player);
    player->hits++;

    if (player->hits < MAX_HITS) {
        display_player_on(player);
    } else {
        dead = 1;
    }

    return dead;
}

/**
 * Display player onto the screen. Display depends on which field and hits the player is on.
 */
void display_player_on(player_t *player)
{
    if (player->field == ATTACK_FIELD) {
        tinygl_pixel_set (tinygl_point (BOTTOM_ROW, player->pos), 1);

    } else {
        if (player->hits == 0) {
        tinygl_draw_box(tinygl_point (BOTTOM_ROW - 1, player->pos + 1),
                        tinygl_point (BOTTOM_ROW, player->pos),
                        1);

        } else {
            tinygl_pixel_set (tinygl_point (BOTTOM_ROW, player->pos), 1);

        }
    }
}

/**
 * Turn off the display of the player on screen.
 */
void display_player_off(player_t *player)
{
    if (player->field == ATTACK_FIELD) {
        tinygl_pixel_set (tinygl_point (BOTTOM_ROW, player->pos), 0);

    } else {
        if (player->hits == 0) {
        tinygl_draw_box(tinygl_point (BOTTOM_ROW - 1, player->pos + 1),
                        tinygl_point (BOTTOM_ROW, player->pos),
                        0);

        } else {
            tinygl_pixel_set (tinygl_point (BOTTOM_ROW, player->pos), 0);

        }
    }
}

/**
 * Reset the position, hit count, and field to default values.
 */
void reset_player(player_t *player)
{
    player->pos = CENTER_POS;
    player->hits = 0;
    player->field = NO_FIELD;
}