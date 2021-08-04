/** @file ball.c
 *  @authors Jackie Chen, Scott Lily
 *  @date 11 Oct 2020
 *  @brief Module which handles the updating ball information such as current position and side on the matrix, if it is active, and hit detection.
 */


#include "system.h"
#include "ball.h"
#include "tinygl.h"
#include "ir_uart.h"

/**
 * Create a new ball with x,y coordinates, current field, and replace an inactive ball in the balls array (if possible).
 */
void new_ball(uint8_t y, uint8_t field, balls_t *balls)
{
    // Default defender side
    uint8_t x = (field == ATTACK_FIELD) ? INITIAL_BALL_ATT_POS : INITIAL_BALL_DEF_POS;

    // If all balls in the balls_t array is active then no new ball will be created.
    for (int i = 0; i < MAX_ACTIVE_BALLS; i++) {
        if (!balls->array[i].active) {
            ball_t new_ball = {x, y, ON, field};
            balls->array[i] = new_ball;

            tinygl_point_t initial_pos = {new_ball.x, new_ball.y};
            tinygl_pixel_set(initial_pos, ON);

            return;
        }
    }
}

/**
 * Move the ball up/down one pixel (depending on which side the ball is on).
 */
void move_ball(ball_t *ball)
{
    (ball->field == ATTACK_FIELD) ? ball->x-- : ball->x++;
}

/**
 * Updates the information of the inputted ball as it travels across the LED matrix.
 * If
 */
uint8_t update_ball(ball_t *ball)
{
    tinygl_pixel_set(tinygl_point(ball->x, ball->y), OFF);

    // If ball is beyond the matrix.
    if (ball->x > TOP_ROW) {
        ball->active = OFF;

        // If this was an attacking ball, create a new ball on the defense field via the IR
        if (ball->field == ATTACK_FIELD) {
            send_ball(ball);
        }

    } else {
        move_ball(ball);

        tinygl_point_t new_pos = {ball->x, ball->y};
        uint8_t area_active = tinygl_pixel_get(new_pos);

        // If the new position is occupied by something else (opposing player).
        if (area_active == 1 && ball->field == DEFEND_FIELD) {
            ball->active = OFF;
            return area_active;
        } else {
            tinygl_pixel_set(tinygl_point(ball->x, ball->y), ON);
        }
    }
    return 0;
}

/**
 * Sends the y-position of the ball to the defender screen.
 */
void send_ball(ball_t *ball)
{
    ir_uart_putc(ball->y);
}

/**
 * Reset all balls in the array to inactive.
 */
void reset_balls(balls_t *balls)
{
    for (uint8_t i = 0; i < MAX_ACTIVE_BALLS; i++) {
        balls->array[i].active = OFF;
    }
}
