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
/*    _ux_device_class_midi_event_get                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function checks if there is an event from the application      */ 
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    midi                                     Address of midi class      */
/*    event                                    Pointer of the event       */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    status                                   UX_SUCCESS if there is an  */ 
/*                                             event                      */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_utility_memory_copy                  Copy memory                */
/*                                                                        */ 
/*  CALLED BY                                                             */ 
/*                                                                        */ 
/*    ThreadX                                                             */ 
/*                                                                        */ 
/*  RELEASE HISTORY                                                       */ 
/*                                                                        */ 
/*    DATE              NAME                      DESCRIPTION             */ 
/*                                                                        */ 
/*  05-19-2020     Chaoqiong Xiao           Initial Version 6.0           */
/*  09-30-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            verified memset and memcpy  */
/*                                            cases,                      */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _ux_device_class_midi_event_get(UX_SLAVE_CLASS_MIDI *midi,
                                      UX_SLAVE_CLASS_MIDI_EVENT *midi_event)
{

    UX_SLAVE_CLASS_MIDI_EVENT       *current_midi_event;
    UX_SLAVE_DEVICE                 *device;

    /* If trace is enabled, insert this event into the trace buffer.  */
    UX_TRACE_IN_LINE_INSERT(UX_TRACE_DEVICE_CLASS_MIDI_EVENT_GET, midi, midi_event, 0, 0, UX_TRACE_DEVICE_CLASS_EVENTS, 0, 0)

    /* Get the pointer to the device.  */                   
    device =  &_ux_system_slave -> ux_system_slave_device;
    
    /* Check the device state.  */
    if (device -> ux_slave_device_state != UX_DEVICE_CONFIGURED)
        return(UX_DEVICE_HANDLE_UNKNOWN);

    /* Check if the head and the tail of the event array is the same.  */
    if (midi -> ux_device_class_midi_event_array_head ==
        midi -> ux_device_class_midi_event_array_tail)

        /* No event to report.  */
        return(UX_ERROR);        

    /* There is an event to report, get the current pointer to the event.  */
    current_midi_event =  midi -> ux_device_class_midi_event_array_tail;

    /* fill in the event structure from the user.  */
    _ux_utility_memory_copy(&midi_event -> ux_device_class_midi_packet, &current_midi_event -> ux_device_class_midi_packet, 4); /* Use case of memcpy is verified. */

    /* Adjust the tail pointer.  Check if we are at the end.  */
    if ((current_midi_event + 1) == midi -> ux_device_class_midi_event_array_end)

        /* We are at the end, go back to the beginning.  */
        midi -> ux_device_class_midi_event_array_tail =  midi -> ux_device_class_midi_event_array;
        
    else        
        /* We are not at the end, increment the tail position.  */
        midi -> ux_device_class_midi_event_array_tail++;

    /* Return event status to the user.  */
    return(UX_SUCCESS);
}

