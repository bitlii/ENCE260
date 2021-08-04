/** @file ball.h
 *  @authors Jackie Chen, Scott Lily
 *  @date 11 Oct 2020
 *  @brief Header file that stores info relating to ball concept of the game.
 */

#ifndef BALL_H
#define BALL_H

#include "system.h"

#define MAX_ACTIVE_BALLS 6

#define ATTACK_FIELD 0
#define DEFEND_FIELD 1

#define OFF 0
#define ON 1

#define INITIAL_BALL_ATT_POS 3
#define INITIAL_BALL_DEF_POS 0
#define TOP_ROW 4

/**
 * Struct to keep track of ball information. 'active' tells us if it is on the field or not.
 * 'field' tells us which microcontroller we are on (defender side or attacker side).
 * */
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t active;
    uint8_t field;
} ball_t;

/**
 * Struct that holds an array
 */
typedef struct {
    ball_t array[MAX_ACTIVE_BALLS];
} balls_t;

/**
 * Creates a new ball to be fired.
 */
void new_ball(uint8_t y, uint8_t field, balls_t *balls);

/**
 * Move location of the ball. Can only go up/down on x axis.
 */
void move_ball(ball_t *ball);

/**
 * Update information of the ball.
 */
uint8_t update_ball(ball_t *ball);

/**
 * Blasts an IR signal to send a ball to opposing player.
 */
void send_ball(ball_t *ball);

/**
 * Set all balls in the array to inactive.
 */
void reset_balls(balls_t *balls);

#endif
