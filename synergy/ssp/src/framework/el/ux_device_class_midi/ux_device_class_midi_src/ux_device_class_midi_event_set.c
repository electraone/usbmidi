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
/*    _ux_device_class_midi_event_set                     PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function sends an event to the midi class. It is processed     */
/*    asynchronously by the interrupt thread.                             */ 
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
/*    _ux_utility_event_flags_set              Set event flags            */
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
/*                                            cases, used UX prefix to    */
/*                                            refer to TX symbols instead */
/*                                            of using them directly,     */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
UINT  _ux_device_class_midi_event_set(UX_SLAVE_CLASS_MIDI *midi,
                                      UX_SLAVE_CLASS_MIDI_EVENT *midi_event)
{
    UX_SLAVE_CLASS_MIDI_EVENT   *current_midi_event;
    UX_SLAVE_CLASS_MIDI_EVENT   *next_midi_event;

    if (!midi) {
    	return(UX_SUCCESS);
    }

    /* If trace is enabled, insert this event into the trace buffer.  */
    UX_TRACE_IN_LINE_INSERT(UX_TRACE_DEVICE_CLASS_MIDI_EVENT_SET, midi, midi_event, 0, 0, UX_TRACE_DEVICE_CLASS_EVENTS, 0, 0)

    /* Current position of the head.  */
    current_midi_event =  midi -> ux_device_class_midi_event_array_head;
    
    /* If the pointer is NULL, the round robin buffer has not been activated.  */
    if (current_midi_event == UX_NULL)
        return (UX_ERROR);
    
    /* Calculate the next position.  */
    if ((current_midi_event + 1) == midi -> ux_device_class_midi_event_array_end) {
        /* We are at the end, go back to the beginning.  */
        next_midi_event =  midi -> ux_device_class_midi_event_array;
    }
    else {
        /* We are not at the end, increment the head position.  */
        next_midi_event = current_midi_event + 1;
    }
    

    /* Any place left for this event ? */
    if (next_midi_event == midi -> ux_device_class_midi_event_array_tail)
        return (UX_ERROR);

    /* There is an event to report, get the current pointer to the event.  */
    current_midi_event =  midi -> ux_device_class_midi_event_array_head;

	/* Update the head.  */
	midi -> ux_device_class_midi_event_array_head = next_midi_event;

	/* Store the data itself.  */
	_ux_utility_memory_copy(&current_midi_event -> ux_device_class_midi_packet, &midi_event -> ux_device_class_midi_packet, 4); /* Use case of memcpy is verified. */

	/* Set an event to wake up the interrupt thread.  */
	_ux_utility_event_flags_set(&midi -> ux_device_class_midi_event_flags_group, UX_DEVICE_CLASS_MIDI_NEW_EVENT, UX_OR);

    /* Return event status to the user.  */
    return(UX_SUCCESS);
}

