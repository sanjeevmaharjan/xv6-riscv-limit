#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    setcputimelimit(100); // Set CPU time limit (100 ticks ~= 10 second)
    volatile uint64 x = 0;
    while (1)
        x++;
    exit(0);
}
