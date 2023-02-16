#include "syslib.h"
#include <stdarg.h>
#include <unistd.h>

int sys_homework1(unsigned flags, endpoint_t proc_ep)
{
    message m;
    return(_kernel_call(SYS_HOMEWORK1, &m));
}
