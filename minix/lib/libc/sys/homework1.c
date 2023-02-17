#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <string.h>
#include <unistd.h>

int homework1(void)
{
  message m;
  memset(&m, 0, sizeof(m));
  printf("%s:%d:%s - libc function\n", __FILE__, __LINE__, __FUNCTION__);
  return(_syscall(PM_PROC_NR, PM_HOMEWORK, &m));
}
