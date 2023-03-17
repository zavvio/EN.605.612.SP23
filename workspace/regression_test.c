#include <stdio.h>
#include <sys/types.h>
#include <sys/ioc_homework.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#define HMWRK_DEV "/dev/homework"
#define ERROR_VALUE 1

int fd;

int driver_open()
{
	fd = open(HMWRK_DEV, O_RDWR);
    return 0;
}

int driver_close()
{
	close(fd);
    return 0;
}

int driver_read(int arg_size)
{
    int foo;
    int ret;

    fd = open(HMWRK_DEV, O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return ERROR_VALUE;
    }

    if ((ret = read(fd, &foo, arg_size)) < 0)
    {
        // printf ("ret = %d\n", ret);
        perror("read");
        close(fd);
        return ERROR_VALUE;
    }
    printf("%s() - value = %d, ret = %d\n", __FUNCTION__, foo, ret);
    close(fd);
    return 0;
}

int driver_write(int arg_value, int arg_size)
{
    int ret;

    fd = open(HMWRK_DEV, O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return ERROR_VALUE;
    }

    if ((ret = write(fd, &arg_value, arg_size)) < 0)
    {
        perror("write");
        close(fd);
        return ERROR_VALUE;
    }
    printf("%s() - Wrote %d to slot, ret = %d\n", __FUNCTION__, arg_value, ret);
    close(fd);
    return 0;
}

int driver_ioctl_slot(int arg_slot)
{
	// int slot = arg_slot;
	// if (slot < 0 || slot > 4)
	// {
	// 	printf ("%s() - Bad slot %d\n", __FUNCTION__, slot);
	// 	return ERROR_VALUE;
	// }
	fd = open (HMWRK_DEV, O_RDWR);
	if (fd < 0)
    {
		perror("open");
        return ERROR_VALUE;
    }

	if (ioctl(fd, HIOCSLOT, &arg_slot) < 0)
	{
		perror("ioctl");
        close(fd);
        return ERROR_VALUE;
	}
	printf("%s() - Set slot to %d\n", __FUNCTION__, arg_slot);
	close(fd);
	return 0;
}

int driver_ioctl_getslot()
{
	int nslot = -1;

	fd = open(HMWRK_DEV, O_RDWR);
	if (fd < 0)
    {
		perror("open");
        return ERROR_VALUE;
    }

	if (ioctl(fd, HIOCGETSLOT, &nslot) < 0)
	{
		perror("ioctl");
        close(fd);
        return ERROR_VALUE;
	}
	printf("%s() - Current slot is %d\n", __FUNCTION__, nslot);
	close(fd);
	return 0;
}

int driver_ioctl_clearslot()
{
	fd = open(HMWRK_DEV, O_RDWR);
	if (fd < 0)
	{
        perror("open");
        return ERROR_VALUE;
    }

	if (ioctl(fd, HIOCCLEARSLOT, NULL) < 0)
	{
		perror("ioctl");
        close(fd);
        return ERROR_VALUE;
	}
	printf("%s() - Cleared current slot\n", __FUNCTION__);
	close(fd);
	return 0;
}

/*
 * Writes an integer from the current slot
 * The value can be optionally provided on the command line.
 */
int main(int argc, char *argv[])
{
    int int_size = 4;
    int error_count = 0;
    int tmp_fd;
    // Restarting device driver
    printf("========== Attempt to Stop and Start device driver ==========\n");
    system("service down homework");
    sleep(3);
    system("service up /service/homework -dev /dev/homework");
    sleep(3);

    // Basic open & close
    printf("========== Basic open & close ==========\n");
    error_count += driver_open();
    error_count += driver_close();
    printf("..... %s():%d - (%d errors so far) .....\n..... Press <ENTER> to continue testing .....\n", __FUNCTION__, __LINE__, error_count);
    getchar();
    // Concurrent open & close
    printf("========== Concurrent open & close ==========\n");
    error_count += driver_open();
    tmp_fd = open(HMWRK_DEV, O_RDWR);
    error_count += driver_close();
    close(tmp_fd);
    printf("..... %s():%d - (%d errors so far) .....\n..... Press <ENTER> to continue testing .....\n", __FUNCTION__, __LINE__, error_count);
    getchar();
    // Read before and after initialization (first write)
    printf("========== Read before and after initialization (first write) ==========\n");
    error_count += driver_read(int_size);
    error_count += driver_write(123, int_size);
    error_count += driver_read(int_size);
    printf("..... %s():%d - (%d errors so far) .....\n..... Press <ENTER> to continue testing .....\n", __FUNCTION__, __LINE__, error_count);
    getchar();
    // Change slot and check value
    printf("========== Change slot and check value ==========\n");
    error_count += driver_ioctl_getslot();
    error_count += driver_ioctl_slot(2);
    error_count += driver_read(int_size);
    printf("..... %s():%d - (%d errors so far) .....\n..... Press <ENTER> to continue testing .....\n", __FUNCTION__, __LINE__, error_count);
    getchar();
    // Write value to current slot then swap back to check old slot
    printf("========== Write value to current slot & swap back to check old slot ==========\n");
    error_count += driver_write(99999, int_size);
    error_count += driver_read(int_size);
    error_count += driver_ioctl_slot(0);
    error_count += driver_read(int_size);
    printf("..... %s():%d - (%d errors so far) .....\n..... Press <ENTER> to continue testing .....\n", __FUNCTION__, __LINE__, error_count);
    getchar();
    // Clear slot and check both slots
    printf("========== Clear slot and check both slots ==========\n");
    error_count += driver_ioctl_clearslot();
    error_count += driver_read(int_size);
    error_count += driver_ioctl_slot(2);
    error_count += driver_read(int_size);
    printf("..... %s():%d - (%d errors so far) .....\n..... Press <ENTER> to continue testing .....\n", __FUNCTION__, __LINE__, error_count);
    getchar();
    // Read with various size
    printf("========== Read with various size ==========\n");
    error_count += driver_read(1);
    error_count += driver_read(-5);
    error_count += driver_read(20);
    printf("..... %s():%d - (%d errors so far, 2 expected) .....\n..... Press <ENTER> to continue testing .....\n", __FUNCTION__, __LINE__, error_count);
    getchar();
    // Write with various size
    printf("========== Write with various size ==========\n");
    error_count += driver_write(24680, 1);
    error_count += driver_write(24680, -5);
    error_count += driver_write(24680, 20);
    printf("..... %s():%d - (%d errors so far, 4 expected) .....\n..... Press <ENTER> to continue testing .....\n", __FUNCTION__, __LINE__, error_count);
    getchar();
    // Set to different slots
    printf("========== Set to different slots ==========\n");
    error_count += driver_ioctl_slot(4);
    error_count += driver_ioctl_slot(5);
    error_count += driver_ioctl_slot(-1);
    printf("..... %s():%d - (%d errors so far, 6 expected) .....\n..... Press <ENTER> to complete testing .....\n", __FUNCTION__, __LINE__, error_count);
    return 0;
}
