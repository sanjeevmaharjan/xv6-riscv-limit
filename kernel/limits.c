#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"

int limitcpuread(struct file *f, int user_dst, uint64 dst, int n)
{
  // If offset > 0, we already returned the data once → EOF
  if (f->off > 0)
    return 0;

  char buf[32];
  int len;

  acquire(&cpu_limit_lock);
  len = itoa(global_cpu_limit_percent, buf);
  release(&cpu_limit_lock);

  buf[len++] = '\n';

  if (n < len)
    len = n;

  if (either_copyout(user_dst, dst, buf, len) < 0)
    return -1;

  // Advance offset so next read returns 0
  f->off += len;

  return len;
}

int limitcpuwrite(struct file *f, int user_src, uint64 src, int n)
{
  char buf[32];
  int len = n;

  if (len >= sizeof(buf))
    len = sizeof(buf) - 1;

  if (either_copyin(buf, user_src, src, len) < 0)
    return -1;

  buf[len] = 0;   // ensure null-terminated for parsing
  // Ignore pure newline writes
  if (len == 1 && (buf[0] == '\n' || buf[0] == '\r'))
    return n;

  // Strip trailing newline(s)
  while (len > 0 && (buf[len-1] == '\n' || buf[len-1] == '\r'))
    buf[--len] = 0;

  // parse integer (simple atoi)
  int sign = 1, i = 0, val = 0;

  if (buf[0] == '-') {
    sign = -1;
    i = 1;
  }

  for (; buf[i] >= '0' && buf[i] <= '9'; i++)
    val = val * 10 + (buf[i] - '0');

  val *= sign;

  acquire(&cpu_limit_lock);
  global_cpu_limit_percent = val;
  release(&cpu_limit_lock);

  return n;
}

void limits_init(void)
{
  initlock(&cpu_limit_lock, "cpu_limit_lock");
  initlock(&mem_limit_lock, "mem_limit_lock");

  devsw[CPUDEV].read = limitcpuread;
  devsw[CPUDEV].write = limitcpuwrite;
}
