
#include "fsm.h"
#include "orders.h"
#include "lights.h"
#include "timer.h"
#include "../driver/elevio.h"

#define DOOR_OPEN_TIME 3.0f // seconds to keep door open

typedef enum {
    STATE_INITIALIZING,
    STATE_IDLE,
    STATE_MOVING,
    STATE_DOOR_OPEN,
    STATE_EMERGENCY_STOP
} ElevatorState;

static ElevatorState m_state;
static MotorDirection m_direction;  // remembers current/last direction
static int m_floor;                 // last known floor
static int m_floor_above_or_current = -1;      // last known state above current floor
static MotorDirection m_direction_before_stop; // to remember direction before emergency stop


void fsm_init(void){
    elevio_init();
    orders_init();
    lights_update();

    m_state = STATE_INITIALIZING;
    m_direction = DIRN_STOP;
    m_floor = -1;

    // after stop:
    m_floor_above_or_current = -1;
}

void fsm_update(void){
    int floor = elevio_floorSensor();
    if (floor >= 0) {
        m_floor = floor;
    }

    // Check for emergency stop from ANY state (S4, S5, S6) unless initializing
    if (elevio_stopButton() && m_state != STATE_INITIALIZING) {
        if (m_state != STATE_EMERGENCY_STOP) {
            elevio_motorDirection(DIRN_STOP);   // stop motor immediately (S4)
            m_direction_before_stop = m_direction; // remember direction before stop
            m_direction = DIRN_STOP;
            orders_init();                      // clear all orders (S5)
            lights_update_stop_lamp(1);         // stop lamp on (L6)
            lights_update();                    // clear order lamps (S5)

            if (floor >= 0) {
                lights_update_door_lamp(1);     // open door if at floor (D3)
            }

            m_state = STATE_EMERGENCY_STOP;
        }
        return; // ignore everything while stop is held (S6)
    }

    // Only poll buttons after initialization (O2)
    if (m_state != STATE_INITIALIZING){
        orders_update();
    }

    switch (m_state) {
        case STATE_INITIALIZING:
            // Move up until we reach a known floor (O1)
            if (floor >= 0) {
                elevio_motorDirection(DIRN_STOP);
                m_state = STATE_IDLE;
            } else {
                elevio_motorDirection(DIRN_UP);
            }
            break;

        case STATE_IDLE:
            // Wait for orders, then choose direction (H4)
            if (orders_any()) {
                if (floor >= 0) {
                    m_direction = orders_choose_direction(m_floor, m_direction);
                    if (m_direction == DIRN_STOP) {
                        // Order at current floor — open door directly
                        lights_update_door_lamp(1);
                        orders_clear_at_floor(m_floor);     // (H3)
                        timer_start();
                        m_state = STATE_DOOR_OPEN;
                    } else {
                        elevio_motorDirection(m_direction);
                        m_state = STATE_MOVING;
                    }
                } else {
                    // If between floors
                    m_direction = orders_choose_direction_between_floors(m_floor_above_or_current);
                    elevio_motorDirection(m_direction);
                    m_state = STATE_MOVING;
                }
            }
            break;

        case STATE_MOVING:
            // Stop and open door when we reach a floor with a matching order
            if (floor >= 0 && orders_should_stop(floor, m_direction)) {
                elevio_motorDirection(DIRN_STOP);
                lights_update_door_lamp(1);             // (D1)
                orders_clear_at_floor(floor);           // (H3)
                timer_start();
                m_state = STATE_DOOR_OPEN;
            }
            break;

        case STATE_DOOR_OPEN:
            // Keep door open while obstructed (D4)
            if (elevio_obstruction()) {
                timer_start();
            }
            // Close door after 3 seconds and choose next action (D1)
            if (timer_ElapsedTime() >= DOOR_OPEN_TIME) {
                lights_update_door_lamp(0);
                m_direction = orders_choose_direction(m_floor, m_direction);
                if (m_direction == DIRN_STOP) {
                    m_state = STATE_IDLE;               // (H4)
                } else {
                    elevio_motorDirection(m_direction);
                    m_state = STATE_MOVING;
                }
            }
            break;

        case STATE_EMERGENCY_STOP:
            // Stop button was released — resume (S7)
            lights_update_stop_lamp(0);
            if (floor >= 0) {
                timer_start();                          // door stays open 3 more seconds (D3)
                m_state = STATE_DOOR_OPEN;
            } else {
                // If not at floor
                if (m_direction_before_stop == DIRN_UP) {
                    m_floor_above_or_current = m_floor + 1;
                } else {
                    m_floor_above_or_current = m_floor;
                }
                m_state = STATE_IDLE;
            }
            break;
    }

    lights_update();
}
