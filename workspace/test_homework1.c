#include <sys/cdefs.h>
#include <lib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <minix/syslib.h>

int main(int argc, char **argv)
{
    int res = 0;
    message m;
    memset(&m, 0, sizeof(m));
    printf("%s:%d:%s - triggering system call with _syscall\n", __FILE__, __LINE__, __FUNCTION__);
    res = _syscall(PM_PROC_NR, PM_HOMEWORK, &m);
    printf("%s:%d:%s - triggering system call from libc\n", __FILE__, __LINE__, __FUNCTION__);
    res = homework1();
    return res;
}