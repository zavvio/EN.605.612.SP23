#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ioc_homework.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

/*
 * Modifies the slot index.  The default
 * is slot 2, but the slot index can be optionally
 * set on the command line.
 */

int
main (int argc, char *argv[])
{
	int fd;
	int nslot = -1;

	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
		perror ("open"), exit (-1);

	if (ioctl (fd, HIOCGETSLOT, &nslot) < 0)
	{
		perror ("ioctl");
	}
	printf ("[pid:%d] %s() - Current slot is %d\n", getpid(), __FUNCTION__, nslot);
	close (fd);
	exit (0);
}
