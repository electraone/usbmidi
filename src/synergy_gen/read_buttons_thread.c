/* generated thread source file - do not edit */
#include "read_buttons_thread.h"

TX_THREAD read_buttons_thread;
void read_buttons_thread_create(void);
static void read_buttons_thread_func(ULONG thread_input);
static uint8_t read_buttons_thread_stack[1024] BSP_PLACE_IN_SECTION_V2(".stack.read_buttons_thread") BSP_ALIGN_VARIABLE_V2(BSP_STACK_ALIGNMENT);
void tx_startup_err_callback(void *p_instance, void *p_data);
void tx_startup_common_init(void);
extern bool g_ssp_common_initialized;
extern uint32_t g_ssp_common_thread_count;
extern TX_SEMAPHORE g_ssp_common_initialized_semaphore;

void read_buttons_thread_create(void)
{
    /* Increment count so we will know the number of ISDE created threads. */
    g_ssp_common_thread_count++;

    /* Initialize each kernel object. */

    UINT err;
    err = tx_thread_create (&read_buttons_thread, (CHAR*) "Button Reader", read_buttons_thread_func, (ULONG) NULL,
                            &read_buttons_thread_stack, 1024, 1, 1, 1, TX_AUTO_START);
    if (TX_SUCCESS != err)
    {
        tx_startup_err_callback (&read_buttons_thread, 0);
    }
}

static void read_buttons_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize common components */
    tx_startup_common_init ();

    /* Initialize each module instance. */

    /* Enter user code for this thread. */
    read_buttons_thread_entry ();
}
