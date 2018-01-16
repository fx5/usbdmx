#include "usleep.h"
#include <chrono>
#include <thread>

int usleep(useconds_t usec) {
    std::this_thread::sleep_for(std::chrono::microseconds(usec));
    return 0;
}
