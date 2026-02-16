#pragma once

/**
 * @brief Initializes the FSM, orders, and lights. Sets state to INITIALIZING.
 */
void fsm_init(void);

/**
 * @brief Runs one iteration of the elevator state machine.
 *
 * Should be called repeatedly in the main loop. Handles state transitions,
 * motor control, door timing, obstruction, and emergency stop.
 */
void fsm_update(void);
