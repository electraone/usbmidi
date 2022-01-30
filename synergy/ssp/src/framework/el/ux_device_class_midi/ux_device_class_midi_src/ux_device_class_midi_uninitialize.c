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
/*    _ux_device_class_midi_uninitialize                  PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function uninitializes the USB MIDI device.                    */
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
/*    _ux_utility_thread_delete            Remove storage thread.         */ 
/*    _ux_utility_memory_free              Free memory used by storage    */ 
/*    _ux_utility_event_flags_delete       Remove flag event structure    */ 
/*                                                                        */ 
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
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _ux_device_class_midi_uninitialize(UX_SLAVE_CLASS_COMMAND *command)
{
                                          
UX_SLAVE_CLASS_MIDI                     *midi;
UX_SLAVE_CLASS                          *class;


    /* Get the class container.  */
    class =  command -> ux_slave_class_command_class_ptr;

    /* Get the class instance in the container.  */
    midi = (UX_SLAVE_CLASS_MIDI *) class -> ux_slave_class_instance;

    /* Remove MIDI thread.  */
    _ux_utility_thread_delete(&class -> ux_slave_class_thread);

    /* Remove the thread used by MIDI.  */
    _ux_utility_memory_free(class -> ux_slave_class_thread_stack);

    /* Delete the event flag group for the midi class.  */
    _ux_utility_event_flags_delete(&midi -> ux_device_class_midi_event_flags_group);

    /* Free memory for the array. */
    _ux_utility_memory_free(midi -> ux_device_class_midi_event_array);

    /* Free the resources.  */
    _ux_utility_memory_free(midi);

    /* Return completion status.  */
    return(UX_SUCCESS);
}

