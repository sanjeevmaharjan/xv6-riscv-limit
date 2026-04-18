#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    setmemlimit(4096 * 10); // Set memory limit 10 pages (1 page = 4096 bytes)
    char *p;
    int i = 0;
    while (1)
    {
        p = sbrk(4096);
        if (p == (char *)-1)
        {
            printf("process pid %d Memory limit reached after allocating %d pages\n", getpid(), i);
            break;
        }
        i++;
    }
    printf("Allocated %d pages\n", i);
    sleep(200); // Set some time to observe the process in the process list (ps command)
    exit(0);
}