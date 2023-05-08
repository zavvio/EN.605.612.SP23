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
    // Restarting device driver
    printf("========== Attempt to Stop and Start device driver ==========\n");
    system("service down homework");
    sleep(3);
    system("service up /service/homework -dev /dev/homework");
    sleep(3);

    // Make 7 Read to uninitialized slot 0
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 1st read on slot 0 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 2nd read on slot 0 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 3rd read on slot 0 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 4th read on slot 0 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 5th read on slot 0 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 6th read on slot 0, should fail as request queue size for each slot is 5. <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 7th read on slot 0, should fail as request queue size for each slot is 5. <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();

    // Switch to uninitialized slot 2 and read
    system("/usr/src/workspace/ioctl_slot 2");
    printf(">>>>> Switched to slot 2. <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 1st read on slot 2 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();

    // Switch to uninitialized slot 3 and read
    system("/usr/src/workspace/ioctl_slot 3");
    printf(">>>>> Switched to slot 3. <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 1st read on slot 3 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();

    // Restarting device driver
    printf("========== Attempt to live-update device driver with state retained ==========\n");
    system("/usr/src/workspace/update.sh");
    sleep(5);

    // Switch to slot 0 and write
    system("/usr/src/workspace/ioctl_slot 0");
    printf(">>>>> Switched to slot 0. <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/writer 12345 &");
    printf(">>>>> Written value to slot 0 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Read initialized slot 0 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();

    // Switch to slot 2 and write
    system("/usr/src/workspace/ioctl_slot 2");
    printf(">>>>> Switched to slot 2. <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/writer 77777 &");
    printf(">>>>> Written value to slot 2 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Read slot 2 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();

    // Clear slot 2 and read
    system("/usr/src/workspace/ioctl_clearslot");
    printf(">>>>> Cleared slot 2. <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Spawned 1st read on slot 2 after clear slot <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/writer 24680 &");
    printf(">>>>> Written value to slot 2 <<<<<\n..... Press <ENTER> to continue testing .....\n");
    getchar();
    system("/usr/src/workspace/reader &");
    printf(">>>>> Read slot 2 <<<<<\n..... Press <ENTER> to finish testing .....\n");
    getchar();

    exit(0);
}
