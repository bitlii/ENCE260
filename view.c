/** @file view.c
 *  @authors Jackie Chen, Scott Lily 
 *  @date 13 Oct 2020
 *  @brief Module for methods display game info onto the screen, as well as driving output to LED1.
 */

#include "view.h"
#include "system.h"
#include "tinygl.h"
#include "stdlib.h"
#include <avr/io.h>


/**
 * Display a 3-2-1 countdown for in between rounds.
 */
void display_countdown(void)
{
    tinygl_clear();
    tinygl_text_speed_set (20);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_draw_message ("3 2 1  ", tinygl_point(0, 4), 1);
    
}

/**
 * Display the currently selected side on screen
 * 'A' - attack or 'D' - defence.
 */
void display_side(char side)
{
    tinygl_clear();
    tinygl_text_speed_set (20);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);
    tinygl_draw_char(side, tinygl_point(0, 4));
}

/**
 * Turn on LED 1.
 */
void led_on(void)
{
    PORTC |= (1 << 2);
}

/**
 * Turn off LED 1.
 */
void led_off(void)
{
    PORTC &= ~(1 << 2);
}

/**
 * Initialise the port to light up LED 1.
 */
void led_init(void)
{
    DDRC |= (1 << 2);
}

/**
 * Display result onto screen.
 */
void display_result(uint8_t result)
{
    tinygl_clear();
    tinygl_text_speed_set (20);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set(TINYGL_TEXT_DIR_ROTATE);

    if (result) {
        tinygl_text ("WINNER!!!");

    } else {
        tinygl_text ("LOSER...");

    }
}