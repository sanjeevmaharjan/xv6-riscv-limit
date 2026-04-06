#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    char *p;
    int i = 0;
    while (1)
    {
        p = sbrk(4096);
        if (p == (char *)-1)
        {
            printf("sbrk failed at iteration %d\n", i);
            break;
        }
        i++;
    }
    printf("Allocated %d pages\n", i);
    sleep(2000);
    exit(0);
}