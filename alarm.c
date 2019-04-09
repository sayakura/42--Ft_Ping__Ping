#include "ping.h"

double    caltime(struct timeval end, struct timeval start)
{
    if ((end.tv_usec -= start.tv_usec) < 0)
    {
        --end.tv_sec;
        end.tv_usec += 1000000;
    }
    end.tv_sec -= start.tv_sec;
    return -(double)(end.tv_sec * 1000.0 + end.tv_usec / 1000.0);
}

int main(void)
{
    struct timeval end, start;

    gettimeofday(&start, NULL);
    //code
    sleep(1);
    gettimeofday(&end, NULL);
    printf("Elapsed Time = %.3f\n", caltime(start, end));
    return (0);
}