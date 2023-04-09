rm -rf *.o regression_test ioctl_slot ioctl_getslot ioctl_clearslot reader try_ioctl try_open writer
clang regression_test.c -o regression_test
clang ioctl_slot.c -o ioctl_slot
clang ioctl_getslot.c -o ioctl_getslot
clang ioctl_clearslot.c -o ioctl_clearslot
clang reader.c -o reader
clang try_ioctl.c -o try_ioctl
clang try_open.c -o try_open
clang writer.c -o writer
clang test_blocking_read.c -o test_blocking_read
