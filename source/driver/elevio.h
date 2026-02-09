#pragma once //prevents multiple inclusions of this header file

// elevio.h - interface for the elevator hardware driver


#define N_FLOORS 4 

typedef enum { 
    DIRN_DOWN   = -1,
    DIRN_STOP   = 0,
    DIRN_UP     = 1
} MotorDirection;


#define N_BUTTONS 3

typedef enum { 
    BUTTON_HALL_UP      = 0,
    BUTTON_HALL_DOWN    = 1,
    BUTTON_CAB          = 2
} ButtonType;

/**
 * @brief Initializes the connection to the elevator hardware server.
 */
void elevio_init(void);


/**
 * @brief Sets the motor direction and sends a command to the elevator hardware server.
 *
 * @param dirn Direction to set the motor to (DIRN_UP, DIRN_DOWN, or DIRN_STOP) (MotorDirection enum).
 */
void elevio_motorDirection(MotorDirection dirn);


/**
 * @brief Sets the light in a button on or off.
 *
 * @param floor Floor number (0 to N_FLOORS-1).
 * @param button Which button (BUTTON_HALL_UP, BUTTON_HALL_DOWN, or BUTTON_CAB).
 * @param value 1 to turn on, 0 to turn off.
 */
void elevio_buttonLamp(int floor, ButtonType button, int value);


/**
 * @brief Sets the floor indicator light on the panel.
 *
 * @param floor Floor number to indicate (0 to N_FLOORS-1).
 */
void elevio_floorIndicator(int floor);


/**
 * @brief Sets the door open indicator lamp on or off.
 *
 * @param value 1 to turn on, 0 to turn off.
 */
void elevio_doorOpenLamp(int value);


/**
 * @brief Sets the stop button lamp on or off.
 *
 * @param value 1 to turn on, 0 to turn off.
 */
void elevio_stopLamp(int value);


/**
 * @brief Checks if a specific button is currently pressed.
 *
 * @param floor Floor number (0 to N_FLOORS-1).
 * @param button Which button (BUTTON_HALL_UP, BUTTON_HALL_DOWN, or BUTTON_CAB).
 * @return 1 if pressed, 0 if not pressed.
 */
int elevio_callButton(int floor, ButtonType button);


/**
 * @brief Reads the floor sensor to determine current floor.
 *
 * @return Floor number (0 to N_FLOORS-1) if at a floor, -1 if between floors.
 */
int elevio_floorSensor(void);


/**
 * @brief Checks if the stop button is currently pressed.
 *
 * @return 1 if pressed, 0 if not pressed.
 */
int elevio_stopButton(void);


/**
 * @brief Checks if the obstruction switch is currently active.
 *
 * @return 1 if obstructed, 0 if not obstructed.
 */
int elevio_obstruction(void);

