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
	int slot = 2;

	if (argc > 1)
		slot = atoi (argv[1]);
	if (slot < 0 || slot > 4)
	{
		printf ("%s() - Bad slot %d\n", __FUNCTION__, slot);
		exit (1);
	}
	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
		perror ("open"), exit (-1);

	if (ioctl (fd, HIOCSLOT, &slot) < 0)
	{
		perror ("ioctl");
	}
	printf ("[pid:%d] %s() - Set slot to %d\n", getpid(), __FUNCTION__, slot);
	close (fd);
	exit (0);
}
