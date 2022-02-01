#include "read_buttons_thread.h"

/* Button Reader entry function */
void read_buttons_thread_entry(void)
{
    /* TODO: add your own code here */
    while (1)
    {
        /* Define the units to be used with the threadx sleep function */
        const uint32_t threadx_tick_rate_Hz = 100;
        /* Set the blink frequency (must be <= threadx_tick_rate_Hz */
        const uint32_t freq_in_hz = 2;
        /* Calculate the delay in terms of the threadx tick rate */
        const uint32_t delay = threadx_tick_rate_Hz/freq_in_hz;

        ioport_level_t level = IOPORT_LEVEL_HIGH;

        while (1)
        {
            /* Determine the next state of the LEDs */
            if(IOPORT_LEVEL_LOW == level)
            {
                level = IOPORT_LEVEL_HIGH;
            }
            else
            {
                level = IOPORT_LEVEL_LOW;
            }

            g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_02, level);

            /* Delay */
            tx_thread_sleep (delay);
        }
    }
}
