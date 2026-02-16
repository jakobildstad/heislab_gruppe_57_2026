#include "modules/fsm.h"
#include "modules/timer.h"

int main(){
    fsm_init();
    while(1){
        fsm_update();
        // Slow down the loop to roughly 50 iterations per second.
        timer_sleep(20);
    }
}
