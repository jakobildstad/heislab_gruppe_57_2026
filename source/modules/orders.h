#pragma once

#include "driver/elevio.h"

/**
 * @brief Initializes the order module by clearing all orders.
 */
void orders_init(void);

/**
 * @brief Registers a new order.
 *
 * @param floor Floor number (0 to N_FLOORS-1).
 * @param button Which button was pressed (BUTTON_HALL_UP, BUTTON_HALL_DOWN, or BUTTON_CAB).
 */
void orders_set(int floor, ButtonType button);

/**
 * @brief Clears all orders at a given floor (all button types).
 *
 * @param floor Floor number (0 to N_FLOORS-1).
 */
void orders_clear_at_floor(int floor);

/**
 * @brief Checks if a specific order exists.
 *
 * @param floor Floor number (0 to N_FLOORS-1).
 * @param button Which button type to check.
 * @return 1 if the order exists, 0 otherwise.
 */
int orders_check(int floor, ButtonType button);

/**
 * @brief Checks if there are any active orders.
 *
 * @return 1 if at least one order exists, 0 otherwise.
 */
int orders_any(void);

/**
 * @brief Determines whether the elevator should stop at the given floor.
 *
 * Stops for cab orders, and for hall orders matching the current direction.
 *
 * @param floor Current floor (0 to N_FLOORS-1).
 * @param dirn Current direction of travel.
 * @return 1 if the elevator should stop, 0 otherwise.
 */
int orders_should_stop(int floor, MotorDirection dirn);

/**
 * @brief Chooses the next direction based on pending orders.
 *
 * Continues in the current direction if there are orders ahead,
 * otherwise reverses or stops.
 *
 * @param floor Current floor (0 to N_FLOORS-1).
 * @param dirn Current/last direction of travel.
 * @return DIRN_UP, DIRN_DOWN, or DIRN_STOP.
 */
MotorDirection orders_choose_direction(int floor, MotorDirection dirn);
