#pragma once



/**
 * @file timer.h
 * @brief Provides a simple interface for sleeping for a specified number of milliseconds.
 *
 * This module abstracts away the details of using nanosleep and provides a convenient function
 * for sleeping in milliseconds, which is commonly needed in the elevator control system.
 * @param milliseconds The number of milliseconds to sleep.
 */
void timer_sleep(int milliseconds);

/**
 * @brief Starts a timer. This function can be used to mark the beginning of a timed operation.
 * 
 */
void timer_start();


/** @brief Returns the elapsed time since the last timer_start() call in seconds. 
 * This function can be used to measure how long an operation has taken.
 * 
 * @return Elapsed time in seconds since the last timer_start() call.
*/
float timer_ElapsedTime();

/**
 * @brief Stops the timer. This function can be used to mark the end of a timed operation.
 * It can be used in conjunction with timer_start() and timer_ElapsedTime() to measure elapsed time.
 */
void timer_stop();