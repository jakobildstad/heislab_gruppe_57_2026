#pragma once

/**
 * @brief Updates the floor indicator to show the given floor.
 *
 * @param floor Floor number (0 to N_FLOORS-1).
 */
void lights_update_floor_indicator(int floor);

/**
 * @brief Sets the door open lamp on or off.
 *
 * @param value 1 for on, 0 for off.
 */
void lights_update_door_lamp(int value);

/**
 * @brief Sets the stop button lamp on or off.
 *
 * @param value 1 for on, 0 for off.
 */
void lights_update_stop_lamp(int value);

/**
 * @brief Updates all lights: order lamps, floor indicator, and stop lamp.
 *
 * Does not update door lamp — that is controlled by the FSM.
 */
void lights_update(void);
