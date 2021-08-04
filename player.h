/** @file player.h
 *  @authors Jackie Chen, Scott Lily
 *  @date 11 Oct 2020
 *  @brief Header file that stores info relating to player and their movements in the game.
 */

#ifndef PLAYER_H
#define PLAYER_H

#define BOTTOM_ROW 4
#define CENTER_POS 3;

#define MAX_HITS 2

#define ATTACK_FIELD 0
#define DEFEND_FIELD 1
#define NO_FIELD     9

typedef struct {
    uint8_t pos;
    uint8_t hits;
    uint8_t field;
} player_t;

/**
 * Move player left of screen.
 */
void player_move_left(player_t *player);

/**
 * Move player right of screen.
 */
void player_move_right(player_t *player);

/**
 * Deal damage to the player, and return '1' if player dies from the hit, else 0.
 */
uint8_t player_hit(player_t *player);

/**
 * Display player on screen.
 */
void display_player_on(player_t *player);

/**
 * Turn off the player on screen.
 */
void display_player_off(player_t *player);

/**
 * Reset the information of the player to default values.
 */
void reset_player(player_t *player);

#endif
