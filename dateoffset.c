#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/syscall.h>

static time_t date_offset = 0;
static bool initialized = false;

static void init_offset (time_t seconds)
{
    const char * const env = getenv("DATE_OFFSET");
    if(env == NULL) return;

    if (sizeof(time_t) <= sizeof(int)) {
        date_offset = atoi(env);
    }
    else {
        date_offset = atoll(env);
    }
    
    date_offset -= seconds;
    initialized = true;
}

static long real_gettime_syscall(long clock, struct timespec *ts)
{
    long ret;
    asm("syscall" : "=a" (ret) :
    "0" (__NR_clock_gettime), "D" (clock), "S" (ts) : "memory");
    return ret;
}

int clock_gettime(clockid_t clk_id, struct timespec *tp) {

    long ret = real_gettime_syscall(clk_id, tp);
    if (clk_id == CLOCK_REALTIME) {

        if (!initialized) {
            init_offset(tp->tv_sec);
        }

        tp->tv_sec += date_offset;
    }
    
    return ret;
}
