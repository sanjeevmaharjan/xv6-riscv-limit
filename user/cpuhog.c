#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void)
{
    volatile uint64 x = 0;
    while (1)
        x++;
    exit(0);
}
