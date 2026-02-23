#include "orders.h"
#include "../driver/elevio.h"

static int orders[N_FLOORS][N_BUTTONS];  // privat til denne modulen

void orders_init(void) {
    for(int i = 0; i < N_FLOORS; i++){
        for(int j = 0; j < N_BUTTONS; j++){
            orders[i][j] = 0;
        }
    }
}

void orders_update(void) {
    // Check for new orders from buttons and add them to the order list
    for (int f = 0; f < N_FLOORS; f++) {
        for (int b = 0; b < N_BUTTONS; b++) {
            if (elevio_callButton(f, b)) {
                orders_set(f, b);
            }
        }
    }
}

void orders_set(int floor, ButtonType button) {
    orders[floor][button] = 1;
}

void orders_clear_at_floor(int floor) {
    for(int i = 0; i < N_BUTTONS; i++){
        orders[floor][i] = 0;
    }
}

int orders_check(int floor, ButtonType button) {
    return orders[floor][button];
}

int orders_any(void) {
    // returner 1 om det finnes noen bestilling, 0 ellers
    for(int i = 0; i < N_FLOORS; i++){
        for(int j = 0; j < N_BUTTONS; j++){
            if(orders[i][j]){
                return 1;
            }
        }
    }
    return 0;
}

int orders_should_stop(int floor, MotorDirection dirn) {
    if (orders[floor][BUTTON_CAB]) {
        return 1;
    }
    if (orders[floor][BUTTON_HALL_DOWN] && dirn == DIRN_DOWN) {
        return 1;
    }
    if (orders[floor][BUTTON_HALL_UP] && dirn == DIRN_UP) {
        return 1;
    }

    // No matching order here — but stop anyway if no orders ahead
    int ordersAhead = 0;
    if (dirn == DIRN_UP) {
        for (int i = floor + 1; i < N_FLOORS; i++)
            for (int j = 0; j < N_BUTTONS; j++)
                if (orders[i][j]) { ordersAhead = 1; break; }
    } else if (dirn == DIRN_DOWN) {
        for (int i = floor - 1; i >= 0; i--)
            for (int j = 0; j < N_BUTTONS; j++)
                if (orders[i][j]) { ordersAhead = 1; break; }
    }
    if (!ordersAhead) {
        for (int j = 0; j < N_BUTTONS; j++)
            if (orders[floor][j]) return 1;
    }

    return 0;

}

MotorDirection orders_choose_direction(int floor, MotorDirection dirn) {

    int ordersAbove = 0;
    int ordersBelow = 0;
    for(int i = floor + 1; i < N_FLOORS; i++){
        for(int j = 0; j < N_BUTTONS; j++){
            if(orders[i][j]){
                ordersAbove = 1;
                break;
            }
        }
        if(ordersAbove) break;
    }
    for(int i = floor - 1; i >= 0; i--){
        for(int j = 0; j < N_BUTTONS; j++){
            if(orders[i][j]){
                ordersBelow = 1;
                break;
            }
        }
        if(ordersBelow) break;
    }

    if (dirn == DIRN_UP) {
        if (ordersAbove) {
            return DIRN_UP; // fortsett opp hvis det finnes bestillinger over
        } else if (ordersBelow) {
            return DIRN_DOWN; // snu og kjør ned hvis det finnes bestillinger under
        } else {
            return DIRN_STOP; // ingen bestillinger, stopp
        }
    } else if (dirn == DIRN_DOWN) {
        if (ordersBelow) {
            return DIRN_DOWN; // fortsett ned hvis det finnes bestillinger under
        } else if (ordersAbove) {
            return DIRN_UP; // snu og kjør opp hvis det finnes bestillinger over
        } else {
            return DIRN_STOP; // ingen bestillinger, stopp
        }
    } else if (dirn == DIRN_STOP) {
        if (ordersAbove) {
            return DIRN_UP; // start opp hvis det finnes bestillinger over
        } else if (ordersBelow) {
            return DIRN_DOWN; // start ned hvis det finnes bestillinger under
        } else {
            return DIRN_STOP; // ingen bestillinger, hold deg i ro
        }
    }
    return DIRN_STOP; // fallback, bør ikke nås
}

MotorDirection orders_choose_direction_between_floors(int floor_above) {
    // Hvis vi er mellom etasjer, velg retning basert på bestillinger i begge retninger
    int ordersAbove = 0;
    int ordersBelow = 0;
    for(int i = floor_above; i < N_FLOORS; i++){
        for(int j = 0; j < N_BUTTONS; j++){
            if(orders[i][j]){
                ordersAbove = 1;
                return DIRN_UP; // fortsett opp hvis det finnes bestillinger over
            }
        }
        if(ordersAbove) break;
    }
    for(int i = floor_above - 1; i >= 0; i--){
        for(int j = 0; j < N_BUTTONS; j++){
            if(orders[i][j]){
                ordersBelow = 1;
                return DIRN_DOWN; // fortsett ned hvis det finnes bestillinger under
            }
        }
        if(ordersBelow) break;
    }
    return DIRN_STOP; // fallback
}

