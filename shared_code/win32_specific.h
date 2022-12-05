#ifndef WIN32_SPECIFIC_H
#define WIN32_SPECIFIC_H

#define MICROSOFT_WINDOWS_WINBASE_H_DEFINE_INTERLOCKED_CPLUSPLUS_OVERLOADS 0
#include <windows.h>

global LARGE_INTEGER win32_global_freq = {0};
global LARGE_INTEGER win32_global_start_time = {0};
global b32 win32_global_init = false;

global f64 os_time_elapsed_ms() {
    f64 result = 0;

    if (!win32_global_init) {
        QueryPerformanceFrequency(&win32_global_freq);
        QueryPerformanceCounter(&win32_global_start_time);
        win32_global_init = true;
    }

    LARGE_INTEGER time_now = {0};
    QueryPerformanceCounter(&time_now);
    result = (double)(time_now.QuadPart - win32_global_start_time.QuadPart);
    result *= (1.0f/((f64)win32_global_freq.QuadPart));
    return result;
}

global int os_cmd_line_pause() {
    return system("pause");
}

#endif
