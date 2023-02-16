/* The kernel call implemented in this file:
 *   m_type:	SYS_HOMEWORK1
 */

#include "kernel/system.h"
#include "kernel/vm.h"
#include <minix/type.h>
#include <assert.h>

/*===========================================================================*
 *          do_homework1                                                     *
 *===========================================================================*/
int do_homework1(struct proc * caller, message * m_ptr)
{
    printf("%s:%d:%s Kernel call do_homework1 triggered.\n", __FILE__, __LINE__, __FUNCTION__);
    return(OK); 
}
