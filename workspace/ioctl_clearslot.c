#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ioc_homework.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"

/*
 * Clear the current slot.
 */

int
main (int argc, char *argv[])
{
	int fd;
	// int slot = 2;

	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
		perror ("open"), exit (-1);

	if (ioctl (fd, HIOCCLEARSLOT, NULL) < 0)
	{
		perror ("ioctl");
	}
	printf ("%s() - Cleared current slot\n", __FUNCTION__);
	close (fd);
	exit (0);
}
