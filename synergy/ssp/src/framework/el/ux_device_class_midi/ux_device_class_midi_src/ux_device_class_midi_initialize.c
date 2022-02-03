/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/

/**************************************************************************/
/**                                                                       */ 
/** USBX Component                                                        */ 
/**                                                                       */
/**   Device MIDI Class                                                   */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define UX_SOURCE_CODE


/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_device_class_midi.h"
#include "ux_device_stack.h"


/**************************************************************************/ 
/*                                                                        */ 
/*  FUNCTION                                               RELEASE        */ 
/*                                                                        */ 
/*    _ux_device_class_midi_initialize                     PORTABLE C     */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function initializes the USB MIDI device.                      */
/*    This function is called by the class register function. It is only  */ 
/*    done once.                                                          */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    command                              Pointer to midi command        */
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    Completion Status                                                   */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_utility_memory_allocate           Allocate memory               */
/*    _ux_utility_memory_free               Free memory                   */
/*    _ux_utility_thread_create             Create thread                 */
/*    _ux_utility_thread_delete             Delete thread                 */
/*    _ux_utility_event_flags_create        Create event flags group      */
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    USBX Source Code                                                    */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            used UX prefix to refer to  */
/*                                            TX symbols instead of using */
/*                                            them directly,              */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _ux_device_class_midi_initialize(UX_SLAVE_CLASS_COMMAND *command)
{
                                          
UX_SLAVE_CLASS_MIDI                      *midi;
UX_SLAVE_CLASS_MIDI_PARAMETER            *midi_parameter;
UX_SLAVE_CLASS                          *class;
UINT                                    status = UX_SUCCESS;


    /* Get the class container.  */
    class =  command -> ux_slave_class_command_class_ptr;

    /* Create an instance of the device midi class.  */
    midi =  _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY, sizeof(UX_SLAVE_CLASS_MIDI));

    /* Check for successful allocation.  */
    if (midi == UX_NULL)
        return(UX_MEMORY_INSUFFICIENT);

    /* Save the address of the MIDI instance inside the MIDI container.  */
    class -> ux_slave_class_instance = (VOID *) midi;





    /* Allocate some memory for the bulk out thread stack. */

    midi -> ux_slave_class_midi_rx_thread_stack =
            _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY, UX_THREAD_STACK_SIZE);

    /* Check for successful allocation.  */
    if (midi -> ux_slave_class_midi_rx_thread_stack  == UX_NULL)
        status = UX_MEMORY_INSUFFICIENT;

    /* Bulk endpoint treatment needs to be running in a different thread. So start
       a new thread. We pass a pointer to the midi instance to the new thread.  This thread
       does not start until we have a instance of the class. */
    if (status == UX_SUCCESS)
    {
        status =  _ux_utility_thread_create(&midi -> ux_slave_class_midi_rx_thread , "ux_slave_class_midi_bulkout_thread",
                    _ux_device_class_midi_rx_thread,
                    (ULONG) (ALIGN_TYPE) class, (VOID *) midi -> ux_slave_class_midi_rx_thread_stack ,
                    UX_THREAD_STACK_SIZE, UX_THREAD_PRIORITY_CLASS,
                    UX_THREAD_PRIORITY_CLASS, UX_NO_TIME_SLICE, UX_DONT_START);

        /* Check the creation of this thread.  */
        if (status != UX_SUCCESS)
            status = UX_THREAD_ERROR;
    }

    UX_THREAD_EXTENSION_PTR_SET(&(midi -> ux_slave_class_rndis_bulkout_thread), class)







    /* Allocate some memory for the thread stack. */
    class -> ux_slave_class_thread_stack =  
            _ux_utility_memory_allocate(UX_NO_ALIGN, UX_REGULAR_MEMORY, UX_THREAD_STACK_SIZE);
    
    /* Check for successful allocation.  */
    if (class -> ux_slave_class_thread_stack == UX_NULL)
        status = UX_MEMORY_INSUFFICIENT;

    /* This instance needs to be running in a different thread. So start
       a new thread. We pass a pointer to the class to the new thread.  This thread
       does not start until we have a instance of the class. */
    if (status == UX_SUCCESS)
        status =  _ux_utility_thread_create(&class -> ux_slave_class_thread, "ux_slave_midi_bulkin_thread",
                    _ux_device_class_midi_tx_thread,
                    (ULONG) (ALIGN_TYPE) class, (VOID *) class -> ux_slave_class_thread_stack,
                    UX_THREAD_STACK_SIZE, UX_THREAD_PRIORITY_CLASS,
                    UX_THREAD_PRIORITY_CLASS, UX_NO_TIME_SLICE, UX_DONT_START);

    /* Check the creation of this thread.  */
    if (status == UX_SUCCESS)
    {

        UX_THREAD_EXTENSION_PTR_SET(&(class -> ux_slave_class_thread), class)

        /* Get the pointer to the application parameters for the midi class.  */
        midi_parameter =  command -> ux_slave_class_command_parameter;

        /* Store the callback function.  */
        midi -> ux_device_class_midi_callback                   = midi_parameter -> ux_device_class_midi_parameter_callback;

        /* Create the event array.  */
        midi -> ux_device_class_midi_event_array =  _ux_utility_memory_allocate_mulc_safe(UX_NO_ALIGN, UX_REGULAR_MEMORY, sizeof(UX_SLAVE_CLASS_MIDI_EVENT), UX_DEVICE_CLASS_MIDI_MAX_EVENTS_QUEUE);

        /* Check for successful allocation.  */
        if (midi -> ux_device_class_midi_event_array != UX_NULL)
        {

            /* Initialize the head and tail of the notification round robin buffers. 
               At first, the head and tail are pointing to the beginning of the array.  */
            midi -> ux_device_class_midi_event_array_head =  midi -> ux_device_class_midi_event_array;
            midi -> ux_device_class_midi_event_array_tail =  midi -> ux_device_class_midi_event_array;
            midi -> ux_device_class_midi_event_array_end  =  midi -> ux_device_class_midi_event_array + UX_DEVICE_CLASS_MIDI_MAX_EVENTS_QUEUE;

            /* Store the start and stop signals if needed by the application.  */
            midi -> ux_slave_class_midi_instance_activate = midi_parameter -> ux_slave_class_midi_instance_activate;
            midi -> ux_slave_class_midi_instance_deactivate = midi_parameter -> ux_slave_class_midi_instance_deactivate;

            /* By default no event wait timeout.  */
            midi -> ux_device_class_midi_event_wait_timeout = 10; //UX_WAIT_FOREVER;

            /* Create a event flag group for the midi class to synchronize with the event interrupt thread.  */
            status =  _ux_utility_event_flags_create(&midi -> ux_device_class_midi_event_flags_group, "ux_device_class_midi_event_flag");

            /* Check status.  */
            if (status == UX_SUCCESS)
                return(status);

            /* It's event error. */
            status =  UX_EVENT_ERROR;
        }
        else
            status =  UX_MEMORY_INSUFFICIENT;

        /* Delete thread.  */
        _ux_utility_thread_delete(&class -> ux_slave_class_thread);
    }
    else
        status = (UX_THREAD_ERROR);

    /* Free stack. */
    if (class -> ux_slave_class_thread_stack)
        _ux_utility_memory_free(class -> ux_slave_class_thread_stack);

    /* Unmount instance. */
    class -> ux_slave_class_instance =  UX_NULL;

    /* Free MIDI instance. */
    _ux_utility_memory_free(midi);

    /* Return completion status.  */
    return(status);
}

