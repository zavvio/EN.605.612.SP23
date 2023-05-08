#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

/*
 * Reads an integer from the current slot
 */
int
main (int argc, char *argv[])
{
    int fd;
    int foo;
    int ret;
    int size;
    char errorMsg[30];

    fd = open (HMWRK_DEV, O_RDWR);
    if (fd < 0)
        perror ("open"), exit (-1);

    size = sizeof(foo);
    if (argc > 1)
    {
        size = atoi (argv[1]);
    }

    if ((ret = read (fd, &foo, size)) < 0)
    {
        // printf ("ret = %d\n", ret);
        sprintf(errorMsg, "[pid %d] read", getpid());
        perror (errorMsg);
        exit (1);
    }
    printf ("[pid:%d] %s() - value = %d, ret = %d\n", getpid(), __FUNCTION__, foo, ret);
    close (fd);
    exit (0);
}
