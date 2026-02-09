#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"



int main(){
    // Initialize the elevator hardware web server connection.
    elevio_init();
    
    printf("Press the stop button on the elevator panel to exit\n");

    // Set initial direction to up.
    elevio_motorDirection(DIRN_UP);

    // Infinite loop
    while(1){
        int floor = elevio_floorSensor();

        // Set direction to up if we're at the bottom floor.
        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

        // Set direction to down if we're at the top floor.
        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }

        // Check all buttons and set the corresponding button lamps based on whether they are pressed.
        for(int f = 0; f < N_FLOORS; f++){ 
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        // Set the stop lamp based on whether the obstruction switch is active.
        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        // Check if the stop button is pressed, and if so, stop the motor and exit the loop.
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break; // Exit program
        }
        
        // Slow down the loop to roughly 50 iterations per second.
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
