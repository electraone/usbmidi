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
/*    _ux_device_class_midi_interrupt_thread              PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function is the thread of the midi interrupt endpoint          */
/*                                                                        */ 
/*  INPUT                                                                 */ 
/*                                                                        */ 
/*    midi_class                                Address of midi class     */
/*                                                container               */ 
/*                                                                        */ 
/*  OUTPUT                                                                */ 
/*                                                                        */ 
/*    None                                                                */ 
/*                                                                        */ 
/*  CALLS                                                                 */ 
/*                                                                        */ 
/*    _ux_utility_event_flags_get           Get event flags               */
/*    _ux_device_class_midi_event_get        Get MIDI event               */
/*    _ux_device_stack_transfer_request     Request transfer              */ 
/*    _ux_utility_memory_copy               Copy memory                   */ 
/*    _ux_utility_thread_suspend            Suspend thread                */
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
VOID  _ux_device_class_midi_tx_thread(ULONG midi_class)
{

UX_SLAVE_CLASS              *class;
UX_SLAVE_CLASS_MIDI         *midi;
UX_SLAVE_DEVICE             *device;
UX_SLAVE_TRANSFER           *transfer_request_in;
UX_SLAVE_CLASS_MIDI_EVENT   midi_event;
UINT                        status;
UCHAR                       *buffer;
ULONG                       actual_flags;


    /* Cast properly the midi instance.  */
    UX_THREAD_EXTENSION_PTR_GET(class, UX_SLAVE_CLASS, midi_class)
    
    /* Get the midi instance from this class container.  */
    midi =  (UX_SLAVE_CLASS_MIDI *) class -> ux_slave_class_instance;
    
    /* Get the pointer to the device.  */
    device =  &_ux_system_slave -> ux_system_slave_device;
    
    /* This thread runs forever but can be suspended or resumed.  */
    while(1)
    {
        /* All MIDI events are on the interrupt endpoint IN, from the host.  */
        transfer_request_in =  &midi -> ux_device_class_midi_tx_endpoint -> ux_slave_endpoint_transfer_request;

        /* As long as the device is in the CONFIGURED state.  */
        while (device -> ux_slave_device_state == UX_DEVICE_CONFIGURED)
        { 
            /* Wait until we have a event sent by the application
               or a change in the idle state to send last or empty report.  */
            status =  _ux_utility_event_flags_get(&midi -> ux_device_class_midi_event_flags_group,
                                                    UX_DEVICE_CLASS_MIDI_EVENTS_MASK, UX_OR_CLEAR, &actual_flags,
                                                    midi -> ux_device_class_midi_event_wait_timeout);

			while (_ux_device_class_midi_event_get(midi, &midi_event) == UX_SUCCESS) {

				/* Prepare the event data payload from the midi event structure.  Get a pointer to the buffer area.  */
				buffer =  transfer_request_in -> ux_slave_transfer_request_data_pointer;

				/* Copy the event buffer into the target buffer.  */
				_ux_utility_memory_copy(buffer, &midi_event.ux_device_class_midi_packet, 4); /* Use case of memcpy is verified. */

				/* Send the request to the device controller.  */
				status =  _ux_device_stack_transfer_request(transfer_request_in, 4, 4);

				/* Check error code. We don't want to invoke the error callback
				   if the device was disconnected, since that's expected.  */
				if (status != UX_SUCCESS && status != UX_TRANSFER_BUS_RESET) {
					/* Error trap. */
					_ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, status);
				}
            }
        }
             
        /* We need to suspend ourselves. We will be resumed by the device enumeration module.  */
        _ux_utility_thread_suspend(&midi -> ux_slave_class_midi_tx_thread);
    }
}
