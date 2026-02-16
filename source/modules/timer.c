#include <time.h>

void timer_sleep(int milliseconds){
    nanosleep(&(struct timespec){0, milliseconds*1000*1000}, NULL);
}

void timer_start(){
}