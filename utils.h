#pragma once

#ifndef UTILS_H
#define UTILS_H

#if defined(__GNUC__) || \
    defined(__clang__) || \
    defined(__llvm__)
    #include <unistd.h>
    #define __TYPEOF__ __typeof__
    #define PAUSE(_ms) usleep(_ms * 1000) // usleep(microseconds)

#elif defined(__MINGW64__) || \
        defined(__MINGW32__) || \
        defined(__CYGWIN64__) || \
        defined(__CYGWIN__) || \
        defined(__MSYS__)
    #include <unistd.h>
    #define __TYPEOF__ typeof
    #define PAUSE(_ms) sleep(_ms / 1000) // sleep(seconds)

#else
    #include <windows.h>
    #define __TYPEOF__ __typeof__ // MSVC delete
    #define PAUSE(_ms) Sleep(_ms) // Sleep(milliseconds)

#endif

char* get_time_now_str(void );

#define _i32_max(__x, __y) \
        ({ \
            __TYPEOF__(__x) x = (__x); \
            __TYPEOF__(__y) y = (__y); \
            x > y ? x : y; \
        }) 

#define _i32_min(__x, __y) \
        ({ \
            __TYPEOF__(__x) x = (__x); \
            __TYPEOF__(__y) y = (__y); \
            x < y ? x : y; \
        }) 

#define any_ptr void*

#endif// #define UTILS_H
