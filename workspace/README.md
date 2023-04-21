# EN.605.612.SP23 Operating Systems Project 3 #
Changes made for this project is shown in
https://github.com/zavvio/EN.605.612.SP23/pull/3/files

### Steps to perform before testing the "homework" device driver: ###
- Check that device driver is available in /service/homework
- Check that device file is available in /dev/homework, create it if not exist via command "mknod /dev/homework c 65 0"
- Test files/scripts are located in \<repo\>/workspace
  - Duplicated copies are available in the submitted VM's /usr/src/workspace
- Give execution permission to all the *.sh test scripts, e.g. "chmod +x /usr/src/workspace/\*.sh"
- Compile all \*.c tests files by executing the compile_all.sh script

### Main Test: ###
For each test below, follow the console prompt to run next section of each test by pressing Enter key.
(For Part I)
- Run the basic comprehensive test suite "regression_test";
- Also run the "test_blocking_read" which spawns various background processes to test the blocking mechanism;
(For Part II)
- Run the "mem" program in either ~/mem or /usr/src/workspace/mem and follow the prompt.

### Other Customizable Tests: ###
- try_open  
open and close the device
- reader [size]
  - (optional) size - valid data size is 4 (bytes) or above, lower or negative number yields error

  Read integer content from device's current slot
- writer [value] [size]
  - (optional) value - write value to current slot; default is 30
  - (optional) size - valid data size is 4 (bytes) or above, lower or negative number yields error

  Write integer content to device's current slot
- ioctl_getslot  
Return index of current slot, between 0 to 4, inclusive.
- ioctl_slot [index]
  - (optional) index - slot number, 0 to 4 inclusive, anything else yields error; default is 2  

  Set current slot to new index
- ioctl_clearslot  
invalidate and clear current slot content to 0
- try_ioctl [index]
  - (optional) index - slot number, 0 to 4 inclusive, anything else yields error; default is 2  

  Perform identical action as ioctl_slot followed by a ioctl_getslot

### Helper Scripts: ###
- up.sh  
Start the "homework" driver
- down.sh  
Stop the "homework" driver
- restart.sh  
Restart the currently running "homework" driver, working data are wiped
- update.sh  
Live update to the latest copy of /service/homework driver, working data are preserved when driver restarts after update