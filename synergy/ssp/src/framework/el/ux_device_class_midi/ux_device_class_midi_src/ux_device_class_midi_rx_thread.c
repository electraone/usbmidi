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
/**   Device MIDI Class                                                  */
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
/*    _ux_device_class_midi_bulkout_thread                PORTABLE C      */
/*                                                           6.1.9        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function is the thread of the midi bulk out endpoint. It       */
/*    is waiting for the host to send data on the bulk out endpoint to    */
/*    the device.                                                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    midi_class                             Address of midi class        */
/*                                                container               */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _ux_device_stack_transfer_request     Request transfer              */
/*    _ux_network_driver_packet_received    Process received packet       */
/*    _ux_utility_long_get                  Get 32-bit value              */
/*    _ux_device_thread_suspend             Suspend thread                */
/*    nx_packet_allocate                    Allocate NetX packet          */
/*    nx_packet_release                     Release NetX packet           */
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
/*                                            prefixed UX to MS_TO_TICK,  */
/*                                            verified memset and memcpy  */
/*                                            cases,                      */
/*                                            resulting in version 6.1    */
/*  01-31-2022     Chaoqiong Xiao           Modified comment(s),          */
/*                                            refined macros names,       */
/*                                            resulting in version 6.1.10 */
/*                                                                        */
/**************************************************************************/
VOID  _ux_device_class_midi_rx_thread(ULONG midi_class)
{

UX_SLAVE_CLASS                  *class;
UX_SLAVE_CLASS_MIDI             *midi;
UX_SLAVE_DEVICE                 *device;
UX_SLAVE_TRANSFER               *transfer_request;
UINT                            status;
UX_SLAVE_CLASS_MIDI_EVENT       packet;



    /* Cast properly the rndis instance.  */
    UX_THREAD_EXTENSION_PTR_GET(class, UX_SLAVE_CLASS, midi_class)

    /* Get the rndis instance from this class container.  */
    midi =  (UX_SLAVE_CLASS_MIDI *) class -> ux_slave_class_instance;

    /* Get the pointer to the device.  */
    device =  &_ux_system_slave -> ux_system_slave_device;

    /* This thread runs forever but can be suspended or resumed.  */
    while(1)
    {

        /* Select the transfer request associated with BULK OUT endpoint.   */
        transfer_request =  &midi -> ux_device_class_midi_rx_endpoint -> ux_slave_endpoint_transfer_request;

        /* As long as the device is in the CONFIGURED state.  */
        while (device -> ux_slave_device_state == UX_DEVICE_CONFIGURED)
        {

            if (1)
            {

                /* And length.  */
                transfer_request -> ux_slave_transfer_request_requested_length =  UX_SLAVE_CLASS_MIDI_EVENT_SIZE;
                transfer_request -> ux_slave_transfer_request_actual_length =     0;


                /* Send the request to the device controller.  */
                status =  _ux_device_stack_transfer_request(transfer_request, UX_SLAVE_CLASS_MIDI_EVENT_SIZE,
                                                            UX_SLAVE_CLASS_MIDI_EVENT_SIZE);

                /* Check the completion code. */
                if (status == UX_SUCCESS)
                {
                   //data = ((uint8_t *)transfer_request -> ux_slave_transfer_request_data_pointer)[1];
                   packet = *((UX_SLAVE_CLASS_MIDI_EVENT *)transfer_request -> ux_slave_transfer_request_data_pointer);
                   midi->ux_device_class_midi_callback(midi, &packet);
                }
            }
            else
            {

                /* Packet allocation timed out. Note that the timeout value is
                   configurable.  */

                /* Error trap. No need for trace, since NetX does it.  */
                _ux_system_error_handler(UX_SYSTEM_LEVEL_THREAD, UX_SYSTEM_CONTEXT_CLASS, UX_MEMORY_INSUFFICIENT);
            }
        }

        /* We need to suspend ourselves. We will be resumed by the device enumeration module.  */
        _ux_utility_thread_suspend(&midi -> ux_slave_class_midi_rx_thread);
    }
}
