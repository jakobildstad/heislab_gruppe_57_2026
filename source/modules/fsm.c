
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

void fsm_init(void){
    elevio_init();
    orders_init();
    lights_update();

    m_state = STATE_INITIALIZING;
    m_direction = DIRN_STOP;
    m_floor = -1;
}

void fsm_update(void){
    int floor = elevio_floorSensor();
    if (floor >= 0) {
        m_floor = floor;
    }

    // Check for emergency stop from ANY state
    if (elevio_stopButton()) {
        if (m_state != STATE_EMERGENCY_STOP) {
            elevio_motorDirection(DIRN_STOP);
            m_direction = DIRN_STOP;
            orders_init();                  // clear all orders (S2)
            lights_update_stop_lamp(1);     // stop lamp on (S2)
            lights_update();                // clear all order lamps

            if (floor >= 0) {
                lights_update_door_lamp(1); // open door if at floor (S3)
            }

            m_state = STATE_EMERGENCY_STOP;
        }
        return; // ignore everything else while stop is held (S4)
    }

    if (m_state != STATE_INITIALIZING){
        orders_update(); // check for new orders from buttons
    }

    switch (m_state) {
        case STATE_INITIALIZING:
            if (floor >= 0) {
                elevio_motorDirection(DIRN_STOP);
                m_state = STATE_IDLE;
            } else {
                elevio_motorDirection(DIRN_UP);
            }
            break;

        case STATE_IDLE:
            if (orders_any()) {
                m_direction = orders_choose_direction(m_floor, m_direction);
                if (m_direction == DIRN_STOP) {
                    // order is at current floor
                    lights_update_door_lamp(1);
                    orders_clear_at_floor(m_floor);
                    timer_start();
                    m_state = STATE_DOOR_OPEN;
                } else {
                    elevio_motorDirection(m_direction);
                    m_state = STATE_MOVING;
                }
            }
            break;

        case STATE_MOVING:
            if (floor >= 0 && orders_should_stop(floor, m_direction)) {
                elevio_motorDirection(DIRN_STOP);
                lights_update_door_lamp(1);
                orders_clear_at_floor(floor);
                timer_start();
                m_state = STATE_DOOR_OPEN;
            }
            break;

        case STATE_DOOR_OPEN:
            if (elevio_obstruction()) {
                timer_start(); // restart timer while obstructed (D3/D4)
            }
            if (timer_ElapsedTime() >= DOOR_OPEN_TIME) {
                lights_update_door_lamp(0);
                m_direction = orders_choose_direction(m_floor, m_direction);
                if (m_direction == DIRN_STOP) {
                    m_state = STATE_IDLE;
                } else {
                    elevio_motorDirection(m_direction);
                    m_state = STATE_MOVING;
                }
            }
            break;

        case STATE_EMERGENCY_STOP:
            // stop button was released — resume
            lights_update_stop_lamp(0);
            if (floor >= 0) {
                timer_start(); // start 3-second timer NOW (D3)
                m_state = STATE_DOOR_OPEN;
            } else {
                m_state = STATE_INITIALIZING;
            }
            break;
    }

    lights_update(); // update all lights based on current orders and floor
}
