#include <minix/drivers.h>
#include <minix/chardriver.h>
#include <stdio.h>
#include <stdlib.h>
#include <minix/ds.h>
#include <sys/ioc_homework.h>
#include "homework.h"

#define PROJECT3_ASSIGNMENT

/*
 * Function prototypes for the homework driver.
 */
static int homework_open(devminor_t minor, int access, endpoint_t user_endpt);
static int homework_close(devminor_t minor);
static ssize_t homework_read(devminor_t minor, u64_t position, endpoint_t endpt,
    cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
static ssize_t homework_write(devminor_t minor, u64_t position, endpoint_t endpt,
	cp_grant_id_t grant, size_t size, int flags, cdev_id_t id);
static int homework_ioctl(devminor_t minor, unsigned long request, endpoint_t endpt,
	cp_grant_id_t grant, int flags, endpoint_t user_endpt, cdev_id_t id);

/* SEF functions and variables. */
static void sef_local_startup(void);
static int sef_cb_init(int type, sef_init_info_t *info);
static int sef_cb_lu_state_save(int);
static int lu_state_restore(void);

/* Entry points to the homework driver. */
static struct chardriver homework_tab =
{
    .cdr_open	= homework_open,
    .cdr_close	= homework_close,
    .cdr_read	= homework_read,
    .cdr_write  = homework_write,
    .cdr_ioctl  = homework_ioctl,
};

#ifdef STRING_EXAMPLE
#define MAX_STRING_BUFFER_SIZE 256
static char string_buffer[MAX_STRING_BUFFER_SIZE] = "Initial homework driver buffer content\n";
#endif

#ifdef PROJECT3_ASSIGNMENT
#define MAX_NUM_OF_SLOTS 5
static int32_t slots[MAX_NUM_OF_SLOTS] = {0};
static uint8_t slots_status[MAX_NUM_OF_SLOTS] = {FALSE};
static uint32_t i_slot = 0;
#endif

/** State variable to count the number of times the device has been opened.
 * Note that this is not the regular type of open counter: it never decreases.
 */
static int open_counter;
static uint32_t initialized;

static int homework_open(devminor_t UNUSED(minor), int UNUSED(access),
    endpoint_t UNUSED(user_endpt))
{
    printf("%s() - Being accessed by %d process(es).\n", __FUNCTION__, ++open_counter);
    if (!initialized)
    {
        i_slot = 0;
        memset(slots, 0, sizeof(slots));
        memset(slots_status, 0, sizeof(slots_status));
        initialized = 1;
        printf("%s() - Initialized.\n", __FUNCTION__);
    }
    return OK;
}

static int homework_close(devminor_t UNUSED(minor))
{
    open_counter--;
    printf("%s()\n", __FUNCTION__);
    return OK;
}

static ssize_t homework_read(devminor_t UNUSED(minor), u64_t position,
    endpoint_t endpt, cp_grant_id_t grant, size_t size, int UNUSED(flags),
    cdev_id_t UNUSED(id))
{
#ifdef STRING_EXAMPLE
    u64_t dev_size;
    char *ptr;
    int ret;
    char *buf = HOMEWORK_MESSAGE;
    char *buf = string_buffer;

    printf("homework_read()\n");

    /* This is the total size of our device. */
    dev_size = (u64_t) strlen(buf);
    dev_size = (u64_t) MAX_STRING_BUFFER_SIZE;

    /* Check for EOF, and possibly limit the read size. */
    if (position >= dev_size) return 0;		/* EOF */
    if (position + size > dev_size)
        size = (size_t)(dev_size - position);	/* limit size */

    /* Copy the requested part to the caller. */
    ptr = buf + (size_t)position;
    if ((ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) ptr, size)) != OK)
        return ret;

    /* Return the number of bytes read. */
    return size;
#endif

#ifdef PROJECT3_ASSIGNMENT
    int ret;
    if (size < 4) // lower-bounded by 4
    {
        printf("%s() - [ERROR] Size %d is smaller than 4.\n", __FUNCTION__, size);
        return EINVAL;
    }
    if (size > 4) size = 4; // upper-bounded by 4
    if (slots_status[i_slot] == FALSE)
        printf("%s() - [WARNING] Slot %d is uninitialized.\n", __FUNCTION__, i_slot);
    if ((ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) &slots[i_slot], size)) != OK)
    {
        printf("%s() - [ERROR] sys_safecopyto failed, ret = %d.\n", __FUNCTION__, ret);
        return ret;
    }
    printf("%s() - Slot %d = %d.\n", __FUNCTION__, i_slot, slots[i_slot]);
    return size;
#endif
}

static ssize_t homework_write(devminor_t UNUSED(minor), u64_t UNUSED(position), endpoint_t endpt,
	cp_grant_id_t grant, size_t size, int UNUSED(flags), cdev_id_t UNUSED(id))
{
#ifdef STRING_EXAMPLE
    int ret;
    printf("homework_write()\n");
    memset(string_buffer, 0, MAX_STRING_BUFFER_SIZE);
    if (size > MAX_STRING_BUFFER_SIZE)
        size = MAX_STRING_BUFFER_SIZE;
    if ((ret = sys_safecopyfrom(endpt, grant, 0, (vir_bytes) string_buffer, size)) != OK)
        return ret;
#endif

#ifdef PROJECT3_ASSIGNMENT
    int ret;
    if (size < 4) // lower-bounded by 4
    {
        printf("%s() - [ERROR] Size %d is smaller than 4.\n", __FUNCTION__, size);
        return EINVAL;
    }
    if (size > 4) size = 4; // upper-bounded by 4
    if ((ret = sys_safecopyfrom(endpt, grant, 0, (vir_bytes) &slots[i_slot], size)) != OK)
    {
        printf("%s() - [ERROR] sys_safecopyfrom failed, ret = %d.\n", __FUNCTION__, ret);
        return ret;
    }
    slots_status[i_slot] = TRUE;
    printf("%s() - Wrote %d to slot %d.\n", __FUNCTION__, slots[i_slot], i_slot);
    if (open_counter > 1)
        printf("%s() - [WARNING] %d concurrent sessions, please explicitly verify result.\n", __FUNCTION__, open_counter);
    return size;
#endif
}

static int homework_ioctl(devminor_t UNUSED(minor), unsigned long request, endpoint_t endpt,
	cp_grant_id_t grant, int UNUSED(flags), endpoint_t UNUSED(user_endpt), cdev_id_t UNUSED(id))
{
#ifdef PROJECT3_ASSIGNMENT
    uint32_t argument;
    int ret;

    switch(request)
    {
        case HIOCSLOT: // Set current slot index
            if ((ret = sys_safecopyfrom(endpt, grant, 0, (vir_bytes) &argument, sizeof(argument))) != OK)
            {
                printf("%s() - [ERROR] sys_safecopyfrom failed, ret = %d\n", __FUNCTION__, ret);
                return ret;
            }
            if (argument > 4)
            {
                printf("%s() - [ERROR] Attempted to set slot to %d but failed because it's not between 0 and 4.\n", __FUNCTION__, argument);
                return EINVAL;
            }
            i_slot = argument;
            printf("%s() - Set slot to %d\n", __FUNCTION__, i_slot);
            if (open_counter > 1)
                printf("%s() - [WARNING] %d concurrent sessions, please explicitly verify result.\n", __FUNCTION__, open_counter);
            break;
        case HIOCCLEARSLOT: // Invalidate and clear current slot content
            slots[i_slot] = 0;
            slots_status[i_slot] = FALSE;
            printf("%s() - Cleared slot %d\n", __FUNCTION__, i_slot);
            if (open_counter > 1)
                printf("%s() - [WARNING] %d concurrent sessions, please explicitly verify result.\n", __FUNCTION__, open_counter);
            break;
        case HIOCGETSLOT: // Get current slot index
            if ((ret = sys_safecopyto(endpt, grant, 0, (vir_bytes) &i_slot, sizeof(i_slot))) != OK)
            {
                printf("%s() - [ERROR] sys_safecopyto failed, ret = %d\n", __FUNCTION__, ret);
                return ret;
            }
            printf("%s() - Current slot is %d\n", __FUNCTION__, i_slot);
            break;
        default:
            break;
    }
#endif
    return OK;
}

static int sef_cb_lu_state_save(int UNUSED(state)) {
/* Save the state. */
    ds_publish_u32("open_counter", open_counter, DSF_OVERWRITE);
    ds_publish_u32("initialized", initialized, DSF_OVERWRITE);

#ifdef PROJECT3_ASSIGNMENT
    ds_publish_u32("i_slot", i_slot, DSF_OVERWRITE);
    ds_publish_mem("slots", slots, sizeof(slots), DSF_OVERWRITE);
    ds_publish_mem("slots_status", slots_status, sizeof(slots_status), DSF_OVERWRITE);
#endif
    return OK;
}

static int lu_state_restore() {
/* Restore the state. */
    u32_t value;
    size_t size;

    ds_retrieve_u32("open_counter", &value);
    ds_delete_u32("open_counter");
    open_counter = (int) value;
    ds_retrieve_u32("initialized", &initialized);
    ds_delete_u32("initialized");

#ifdef PROJECT3_ASSIGNMENT
    ds_retrieve_u32("i_slot", &i_slot);
    ds_delete_u32("i_slot");
    size = sizeof(slots);
    ds_retrieve_mem("slots", (char*)slots, &size);
    ds_delete_mem("slots");
    size = sizeof(slots_status);
    ds_retrieve_mem("slots_status", slots_status, &size);
    ds_delete_mem("slots_status");
#endif

    return OK;
}

static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);

    /*
     * Register live update callbacks.
     */
    /* - Agree to update immediately when LU is requested in a valid state. */
    sef_setcb_lu_prepare(sef_cb_lu_prepare_always_ready);
    /* - Support live update starting from any standard state. */
    sef_setcb_lu_state_isvalid(sef_cb_lu_state_isvalid_standard);
    /* - Register a custom routine to save the state. */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);

    /* Let SEF perform startup. */
    sef_startup();
}

static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
/* Initialize the homework driver. */
    int do_announce_driver = TRUE;

    open_counter = 0;
    switch(type) {
        case SEF_INIT_FRESH:
            initialized = 0;
            printf("\"homework\" driver started freshly.\n");
        break;

        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;

            printf("\"homework\" driver restarted after live update.\n");
        break;

        case SEF_INIT_RESTART:
            printf("\"homework\" driver restarted.\n");
        break;
    }

    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }

    /* Initialization completed successfully. */
    return OK;
}

int main(void)
{
    /*
     * Perform initialization.
     */
    sef_local_startup();

    /*
     * Run the main loop.
     */
    chardriver_task(&homework_tab);
    return OK;
}

