#define _POSIX_C_SOURCE 199309L // enable POSIX functions (clock_gettime, nanosleep)
#include "timer.h"
#include <time.h>

static struct timespec m_start_time; // stores when timer_start() was called
static int m_active = 0;            // 1 if timer is running, 0 if stopped

void timer_sleep(int milliseconds) {
    // convert ms to nanoseconds (1 ms = 1,000,000 ns)
    nanosleep(&(struct timespec){0, milliseconds * 1000 * 1000}, NULL);
}

void timer_start() {
    // CLOCK_MONOTONIC: a clock that only moves forward (unaffected by system time changes)
    clock_gettime(CLOCK_MONOTONIC, &m_start_time);
    m_active = 1;
}

float timer_ElapsedTime() {
    if (!m_active) {
        return 0.0f;
    }
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    // seconds difference + nanoseconds difference converted to seconds
    float elapsed = (now.tv_sec - m_start_time.tv_sec)
                  + (now.tv_nsec - m_start_time.tv_nsec) / 1e9f;
    return elapsed;
}

void timer_stop() {
    m_active = 0;
}
