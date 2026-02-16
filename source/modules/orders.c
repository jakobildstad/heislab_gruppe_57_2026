#include "orders.h"
#include "../driver/elevio.h"

static int orders[N_FLOORS][N_BUTTONS];  // privat til denne modulen

void orders_init(void) {
    // nullstill hele arrayen

    for(int i = 0; i < N_FLOORS; i++){
        for(int j = 0; j < N_BUTTONS; j++){
            orders[i][j] = 0;
        }
    }
}

void orders_set(int floor, ButtonType button) {
    orders[floor][button] = 1;
}

void orders_clear_at_floor(int floor) {
    // fjern ALLE bestillinger i denne etasjen (alle 3 knappetypene)
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
    // returner 1 hvis heisen bør stoppe her, gitt retning

    if (orders[floor][BUTTON_CAB]) {
        return 1; // alltid stopp for cab-bestillinger
    }
    else if (orders[floor][BUTTON_HALL_DOWN] && dirn == DIRN_DOWN) {
        return 1; // stopp for hall-down bestillinger når vi kjører ned
    }
    else if (orders[floor][BUTTON_HALL_UP] && dirn == DIRN_UP) {
        return 1; // stopp for hall-up bestillinger når vi kjører opp
    }
    return 0; // ellers, ikke stopp
}

MotorDirection orders_choose_direction(int floor, MotorDirection dirn) {
    // bestem hvilken retning heisen bør kjøre
    // sjekk om det finnes bestillinger over/under nåværende etasje
    // returner DIRN_UP, DIRN_DOWN, eller DIRN_STOP

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
    }
}
