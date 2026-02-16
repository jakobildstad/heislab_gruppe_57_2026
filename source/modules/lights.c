#include "lights.h"
#include "orders.h"
#include "../driver/elevio.h"

void lights_update_floor_indicator(int floor) {
    elevio_floorIndicator(floor);
}

void lights_update_door_lamp(int value) {
    elevio_doorOpenLamp(value);
}

void lights_update_stop_lamp(int value) {
    elevio_stopLamp(value);
}

void lights_update(void) {
    // Update all order button lamps based on current orders.
    for (int floor = 0; floor < N_FLOORS; floor++) {
        for (int btn = 0; btn < N_BUTTONS; btn++) {
            elevio_buttonLamp(floor, btn, orders_check(floor, btn));
        }
    }
    // Update floor indicator based on current floor sensor reading if in floor.
    int current_floor = elevio_floorSensor();
    if (current_floor >= 0) {
        lights_update_floor_indicator(current_floor);
    }
    // Stop lamp and door lamp are controlled by the FSM, not here.
}
