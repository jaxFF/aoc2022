#ifndef WIN32_SPECIFIC_H
#define WIN32_SPECIFIC_H

global LARGE_INTEGER win32_global_freq = {0};
global LARGE_INTEGER win32_global_start_time = {0};
global b32 win32_global_init = false;

b32 RtlQueryPerformanceCounter(LARGE_INTEGER*);

global u64 os_time_elapsed_ticks() {
    u64 result = 0;

    if (!win32_global_init) {
        QueryPerformanceFrequency(&win32_global_freq);
        QueryPerformanceCounter(&win32_global_start_time);
        win32_global_init = true;
    }

    LARGE_INTEGER time_now = {0};
    QueryPerformanceCounter(&time_now);
    result = ((u64)time_now.HighPart << 32) | time_now.LowPart;
    result -= ((u64)win32_global_start_time.HighPart << 32) | win32_global_start_time.LowPart;
    return result;
}

global u64 os_ticks_per_second() {
    u64 result = 0;
    if (!win32_global_init) {
        QueryPerformanceFrequency(&win32_global_freq);
        QueryPerformanceCounter(&win32_global_start_time);
        win32_global_init = true;
    }
    result = (((u64)win32_global_freq.HighPart << 32) | win32_global_freq.LowPart);
    return result;
}

global int os_cmd_line_pause() {
    return system("pause");
}

#endif
