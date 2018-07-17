#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main()
{
    struct timeval tm;
    gettimeofday(&tm, NULL);
    struct tm *pt = localtime(&(tm.tv_sec));
    char buf[27];

    strftime(buf, 27, "%F %T", pt);
    printf("buf=[%s]\n", buf);
}
