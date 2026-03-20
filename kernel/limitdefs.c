#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"

extern char end[]; // symbol defined in linker script (kernel.ld)

#define TOTAL_MEM ((uint64)PHYSTOP)
#define KERNEL_END ((uint64)end)

#define total_mem_bytes (TOTAL_MEM - KERNEL_END)
const uint total_cpu_percent = 100; // or read from number of harts if you want SMP support

uint global_cpu_limit_percent = 100;
struct spinlock cpu_limit_lock;
uint global_mem_limit_percent = 100;
struct spinlock mem_limit_lock;

uint
mempercent(uint64 bytes)
{
    return ((uint64)bytes * 10000LL) / total_mem_bytes;
}
