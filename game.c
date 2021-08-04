/** @file game.c
 *  @authors Jackie Chen, Scott Lily
 *  @date 01 Oct 2020
 *  @brief A dodgeball-survival game where an attacker must throw balls at the defender to win, and the defender must survive for as long as possible.
 */

#include "system.h"
#include "pio.h"
#include "navswitch.h"
#include "tinygl.h"
#include "font3x5_1.h"
#include "ir_uart.h"
#include "task.h"
#include "ball.h"
#include "player.h"
#include "view.h"

#define NAVSWITCH_RATE 100
#define DISPLAY_RATE 500
#define SHOOTSPEED_RATE 200
#define BALL_RATE 2
#define IR_RATE 100
#define FIELD_WIDTH 6

typedef enum {STATE_SETUP, STATE_PLAYING, STATE_FINISH} state_t;
state_t state = STATE_SETUP;

char side = 'A';
uint8_t game_over = 0;
uint8_t round_one_over = 0;
uint8_t round_one_time = 0;
uint8_t round_two_time = 0;

int shoot_tick = 100;
int max_shoot_tick = 100;
int shootspeed_tick = 0;
int max_shootspeed_tick = 100;

balls_t balls;
player_t player;

/**
 * Check the times of both players and determine the winner.
 */
void determine_winner(void)
{
    uint8_t result = 0;

    if (player.field == DEFEND_FIELD) {
        if (round_one_time < round_two_time) {
            result = 1;
        }
    }

    if (player.field == ATTACK_FIELD) {
        if (round_one_time > round_two_time) {
            result = 1;
        }
    }

    display_result(result);
    state = STATE_FINISH;
}

/**
 * Reset game values to default and switch to setup state.
 */
void reset_game(void)
{
    tinygl_clear();
    shoot_tick = 100;
    max_shoot_tick = 100;
    shootspeed_tick = 0;
    max_shootspeed_tick = 100;

    side = 'A';
    game_over = 0;
    round_one_over = 0;
    round_one_time = 0;
    round_two_time = 0;

    reset_player(&player);
    reset_balls(&balls);
    display_side(side);
    state = STATE_SETUP;
}

/**
 * Start/restart game.
 */
void game_start(void) 
{
    tinygl_clear();

    if(!round_one_over) {
        // Round 1
        player.field = (side == 'A') ? ATTACK_FIELD : DEFEND_FIELD;

    } else {
        // Round 2, switch sides.
        player.field = (player.field == ATTACK_FIELD) ? DEFEND_FIELD : ATTACK_FIELD;
        reset_balls(&balls);

        shoot_tick = 100;
        max_shoot_tick = 100;
        shootspeed_tick = 0;
        max_shootspeed_tick = 100;
    }

    display_countdown();
    state = STATE_PLAYING;
}

/**
 * End the round, or game when player dies.
 */
void end_round(void)
{
    // Returns 1 if the hit kills player.
    if (!round_one_over) {
        round_one_over = 1;

        if (player.field == DEFEND_FIELD) {
            ir_uart_putc('R');
            game_start();
        }

    } else {
        // Game over
        reset_balls(&balls);
        if (player.field == DEFEND_FIELD) {
            ir_uart_putc('G');
        }
        if (!game_over) {
            game_over = 1;
            determine_winner();
        }
    }
}

/**
 * Task that deals with tracking and upating ball information.
 */
static void ball_task(__unused__ void *data)
{
    // Loop through each active ball and update position.
    for (int i = 0; i < MAX_ACTIVE_BALLS; i++) {
        if (balls.array[i].active) {
            uint8_t successful_hit = update_ball(&balls.array[i]);

            // If player_hit() returns 1, the player is dead.
            if (successful_hit && player_hit(&player)) {
                end_round();
            }
        }
    }
}

/**
 * Task that periodically updates display.
 */
static void display_task(__unused__ void *data)
{
    tinygl_update();
}

/**
 * Change side of player.
 */
void change_side(void)
{
    side = (side == 'A') ? 'D' : 'A';
}


/** 
 * Task that handles and get navswitch input.
 */
static void navswitch_task(__unused__ void *data)
{
    navswitch_update ();

    if (state == STATE_PLAYING) {
        if (player.field == ATTACK_FIELD) {
            if (navswitch_push_event_p(NAVSWITCH_WEST)) {
                // If the attacker can fire again, and there is no active ball in front of the player.
                if (shoot_tick >= max_shoot_tick && !tinygl_pixel_get(tinygl_point(3, player.pos))) {
                    new_ball(player.pos, ATTACK_FIELD, &balls);
                    shoot_tick = 0;
                    led_off();
                }
            }

            if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                player_move_right(&player);
            }

            if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                player_move_left(&player);
            }

            if (shoot_tick < max_shoot_tick) {
                shoot_tick++;
            } else {
                led_on();
            }
        }

        if (player.field == DEFEND_FIELD) {
            if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
                player_move_right(&player);
            }

            if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                player_move_left(&player);
            }
        }

    } else if (state == STATE_SETUP) {
        if (navswitch_push_event_p (NAVSWITCH_NORTH) || navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            change_side();
            display_side(side);
        }

        if (navswitch_push_event_p(NAVSWITCH_WEST)) {
            tinygl_clear();
            ir_uart_putc(side);
            game_start();
        }

        if (ir_uart_read_ready_p()) {
            tinygl_clear();
            side = (ir_uart_getc() == 'A') ? 'D' : 'A';
            game_start();
        }

    } else if (state == STATE_FINISH) {
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            reset_game();
        }
    }
}

/**
 * Task that keeps track of the ramp up of the attacker's fire rate.
 */
static void shootspeed_task(__unused__ void *data)
{
    if (player.field == ATTACK_FIELD) {
        if (shootspeed_tick >= max_shootspeed_tick) {
            shootspeed_tick = 0;

            if (max_shoot_tick > 0) {
                max_shoot_tick--;
            }
        }
        shootspeed_tick++;
    }

}

/**
 * Task that handles IR communications.
 */
static void ir_task(__unused__ void *data)
{
    // Check for data sent by other funkit
    if (ir_uart_read_ready_p ()) {
        char ch = ir_uart_getc ();

        // If on DEFENCE
        // Check it's a number between 0 and the FIELD_WIDTH
        // If so, position on defender field is (FIELD_WIDTH-ch)
        if (player.field == DEFEND_FIELD) {
            if (ch >= 0 && ch <= FIELD_WIDTH) {
                new_ball(FIELD_WIDTH - ch, DEFEND_FIELD, &balls);
            }
        }

        // If on ATTACK
        // Check it's a "R" for round-over or a "G" for game-over
        if (player.field == ATTACK_FIELD) {
            if (ch == 'R') {
                round_one_over = 1;
                game_start();
            }

            if (ch == 'G' && !game_over) {
                game_over = 1;
                determine_winner();
            }
        }
    }
}

/**
 * Task that tracks and increments the time the defender has been alive for (which is used for scoring).
 */
static void timer_task(__unused__ void *data)
{
    if (!game_over) {
        if (!round_one_over) {
            round_one_time++;
        } else {
            round_two_time++;
        }
    }
}

/**
 * Initialises all of the important tasks for the game.
 */
void setup_game(void)
{
    system_init ();
    navswitch_init ();
    ir_uart_init ();
    led_init();
    tinygl_init(DISPLAY_RATE);
    tinygl_font_set (&font3x5_1);

    reset_player(&player);
    display_side(side);
}


int main (void)
{
    // General initialisation
    setup_game();

    task_t tasks[] = {
        {.func = timer_task, .period = TASK_RATE},
        {.func = navswitch_task, .period = TASK_RATE / NAVSWITCH_RATE},
        {.func = display_task, .period = TASK_RATE / DISPLAY_RATE},
        {.func = shootspeed_task, .period = TASK_RATE / SHOOTSPEED_RATE},
        {.func = ball_task, .period = TASK_RATE / BALL_RATE},
        {.func = ir_task, .period = TASK_RATE / IR_RATE}
    };

    task_schedule(tasks, 6);

    return 0;
}


