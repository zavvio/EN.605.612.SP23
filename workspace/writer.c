#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

/*
 * Writes an integer from the current slot
 * The value can be optionally provided on the command line.
 */
int
main (int argc, char *argv[])
{
    int fd;
    int foo;
    int ret;
    int size;

    fd = open (HMWRK_DEV, O_RDWR);
    if (fd < 0)
        perror ("open"), exit (-1);

    foo = 30;
    if (argc > 1)
    {
        foo = atoi (argv[1]);
    }
    size = sizeof(foo);
    if (argc > 2)
    {
        size = atoi (argv[2]);
    }

    if ((ret = write (fd, &foo, size)) < 0)
    {
        perror ("write");
        exit (1);
    }
    printf ("[pid:%d] %s() - Writing %d to slot, ret = %d\n", getpid(), __FUNCTION__, foo, ret);
    close (fd);
    exit (0);
}
