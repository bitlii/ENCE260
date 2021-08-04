/** @file view.h
 *  @authors Jackie Chen, Scott Lily
 *  @date 13 Oct 2020
 *  @brief Header file for methods relating to displaying info onto the LED screen, as well as driving output to LED1.
 */

#ifndef VIEW_H
#define VIEW_H

#include "system.h"

/**
 * Display a 3-2-1 countdown for in between rounds.
 */
void display_countdown(void);

/**
 * Display the currently selected side on screen.
 */
void display_side(char side);

/**
 * Turn on LED 1.
 */
void led_on(void);

/**
 * Turn off LED 1.
 */
void led_off(void);

/**
 * Initialise the port to light up LED 1.
 */
void led_init(void);

/**
 * Display result onto screen.
 */
void display_result(uint8_t result);

#endif
