#include "syslib.h"
#include <stdarg.h>
#include <unistd.h>

int sys_homework1(unsigned flags, endpoint_t proc_ep)
{
    message m;
    printf("%s:%d:%s - libsys function\n", __FILE__, __LINE__, __FUNCTION__);
    return(_kernel_call(SYS_HOMEWORK1, &m));
}
