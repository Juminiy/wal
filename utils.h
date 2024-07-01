#pragma once

#ifndef UTILS_H
#define UTILS_H

#if defined(__GNUC__) || \
    defined(__clang__) || \
    defined(__llvm__)
    #define __TYPEOF__ __typeof__
    #define PAUSE(ms) usleep(ms)

#elif defined(__MINGW64__) || \
        defined(__MINGW32__) || \
        defined(__CYGWIN64__) || \
        defined(__CYGWIN__) || \
        defined(__MSYS__)
    #define __TYPEOF__ typeof
    #define PAUSE(ms) sleep(ms / 1000000)

#else
    #define __TYPEOF__ __typeof__
    #define PAUSE(ms) Sleep(ms * 1000)

#endif

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
