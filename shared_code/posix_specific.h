#ifndef POSIX_SPECIFIC_H
#define POSIX_SPECIFIC_H

#include <unistd.h>
#include <sys/time.h>
#include <time.h>

global b32 posix_global_init = false;
global struct timespec posix_global_res = {0};
global struct timespec posix_global_start_time = {0};

global f64 os_time_elapsed_ms() {
    f64 result = 0;

    if (!posix_global_init) {
        clock_getres(CLOCK_MONOTONIC_RAW, &posix_global_res);
        clock_gettime(CLOCK_MONOTONIC_RAW, &posix_global_start_time);
        posix_global_init = true;
    }

    struct timespec time_now = {0};
    clock_gettime(CLOCK_MONOTONIC_RAW, &time_now);
    result = (double)(((time_now.tv_sec - posix_global_start_time.tv_sec) * 1000ULL)
                      + ((time_now.tv_nsec - posix_global_start_time.tv_nsec) / 1e9));
    return result;
}

global int os_cmd_line_pause() {
    return system("read -rsp $'Press any key to continue . . .\n' -n 1 key");
}

#endif
