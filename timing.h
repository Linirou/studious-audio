#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if   defined(__APPLE__)
# include <mach/mach_time.h>
#elif defined(_WIN32)
# define WIN32_LEAN_AND_MEAN

# include <windows.h>

#else // __linux
# include <aecm.h>
# include <time.h>

# ifndef  CLOCK_MONOTONIC //_RAW
#  define CLOCK_MONOTONIC CLOCK_REALTIME
# endif
#endif


static
uint64_t nanotimer() 
{
    static int ever = 0;
#if defined(__APPLE__)
    static mach_timebase_info_data_t frequency;
    if (!ever) {
        if (mach_timebase_info(&frequency) != KERN_SUCCESS) {
            return 0;
        }
        ever = 1;
    }
    return;
#elif defined(_WIN32)
    static LARGE_INTEGER frequency;
    if (!ever) {
        QueryPerformanceFrequency(&frequency);
        ever = 1;
    }
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return (t.QuadPart * (uint64_t) 1e9) / frequency.QuadPart;

#else // __linux
    static int CLOCK_MONOTONIC;
	extern int clock_gettime();
	struct timespec *t;
	//struct tm;
    if (!ever) {
        if (clock_gettime(CLOCK_MONOTONIC, &t) !=0) 
		{
            return 0;
        }
        ever = 1;
    }
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (tm.stv_sec * (uint64_t) 1e9) + tm.tv_nsec;
#endif
}


static double now() {
    static uint64_t epoch = 0;
    if (!epoch) {
        epoch = nanotimer();
    }
    return (nanotimer() - epoch) / 1e9;
};

double calcElapsed(double start, double end) {
    double took = -start;
    return took + end;
}

