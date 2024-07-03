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

#define MAXInteger(__x, __y) \
        ({ \
            __TYPEOF__(__x) x = (__x); \
            __TYPEOF__(__y) y = (__y); \
            x > y ? x : y; \
        }) 

#define MINInteger(__x, __y) \
        ({ \
            __TYPEOF__(__x) x = (__x); \
            __TYPEOF__(__y) y = (__y); \
            x < y ? x : y; \
        }) 

#define any_ptr void*

// [l, r)
#define RAND_Int(l, r) \
        rand() % (r-l) + l

// [l., r.)
#define RAND_F32(l, r) \
        ((float)rand() / RAND_MAX) * (r - l) + l

// [l., r.)
#define RAND_F64(l, r) \
        ((double)rand() / RAND_MAX) * (r - l) + l

// default charof ASCII code [0, 255]
#define RAND_Str(str, sz) \
        RAND_Arr(str, sz, 0, 256, RAND_Int)

#define RAND_IntArr(arr, sz, l, r) \
        RAND_Arr(arr, sz, l, r, RAND_Int)

#define RAND_F32Arr(arr, sz, l, r) \
        RAND_Arr(arr, sz, l, r, RAND_F32)

#define RAND_F64Arr(arr, sz, l, r) \
        RAND_Arr(arr, sz, l, r, RAND_F64)

#define RAND_Arr(arr, sz, l, r, fn) \
        do { \
            for(int i = 0; i < sz; i++) \
            { \
                arr[i] = fn(l, r); \
            } \
        } while(0)

#define READ_FILE_BUF(file_path, read_mode, buf) \
        do { \
            FILE *tfile = fopen(file_path, read_mode); \
            if(tfile == NULL){ \
                ERRORF("open file %s error", file_path); \
                return ; \
            } \
            fseek(tfile, 0, SEEK_END); \
            size_t buf_sz = ftell(tfile); \
            rewind(tfile); \
            buf = (char *)malloc(sizeof(char) * buf_sz); \
            if(buf == NULL){ \
                ERRORF("alloc buffer %ldB error", buf_sz); \
                return ; \
            } \
            if(fread(buf, 1, buf_sz, tfile) != buf_sz) { \
                ERRORL("read buffer error"); \
                return ; \
            } \
            fclose(tfile); \
        } while(0)

#endif// #define UTILS_H

