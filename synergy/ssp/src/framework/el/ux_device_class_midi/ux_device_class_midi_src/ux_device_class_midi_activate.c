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
/**   Device MIDI Class                                                    */
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
/*    _ux_device_class_midi_activate                       PORTABLE C     */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This function activates an instance of a MIDI device.               */
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
/*    _ux_utility_thread_resume             Resume thread                 */ 
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
/*  12-31-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            added Get/Set Protocol      */
/*                                            request support,            */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/
UINT  _ux_device_class_midi_activate(UX_SLAVE_CLASS_COMMAND *command)
{
                                          
UX_SLAVE_INTERFACE                      *interface;
UX_SLAVE_CLASS_MIDI                     *midi;
UX_SLAVE_CLASS                          *class;
UX_SLAVE_ENDPOINT                       *endpoint_tx;
UX_SLAVE_ENDPOINT                       *endpoint_rx;

    /* Get the class container.  */
    class =  command -> ux_slave_class_command_class_ptr;

    /* Get the class instance in the container.  */
    midi =  (UX_SLAVE_CLASS_MIDI *) class -> ux_slave_class_instance;

    /* Get the interface that owns this instance.  */
    interface =  (UX_SLAVE_INTERFACE  *) command -> ux_slave_class_command_interface;
    
    /* Store the class instance into the interface.  */
    interface -> ux_slave_interface_class_instance =  (VOID *)midi;
         
    /* Now the opposite, store the interface in the class instance.  */
    midi -> ux_slave_class_midi_interface =  interface;
    
    /* Locate the TX endpoint.  */
    endpoint_tx =  interface -> ux_slave_interface_first_endpoint;

    /* Check if interrupt IN endpoint exists.  */
    while (endpoint_tx != UX_NULL)
    {

        if ((endpoint_tx -> ux_slave_endpoint_descriptor.bEndpointAddress & UX_ENDPOINT_DIRECTION) == UX_ENDPOINT_IN &&
            (endpoint_tx -> ux_slave_endpoint_descriptor.bmAttributes & UX_MASK_ENDPOINT_TYPE) == UX_BULK_ENDPOINT)
        {

            /* It's right endpoint we need.  */
            break;
        }

        /* Try next endpoint.  */
        endpoint_tx =  endpoint_tx -> ux_slave_endpoint_next_endpoint;
    }

    /* Check if we found right endpoint.  */
    if (endpoint_tx == UX_NULL)
        return (UX_ERROR);


    /* Locate the RX endpoint.  */
    endpoint_rx =  interface -> ux_slave_interface_first_endpoint;

    /* Check if interrupt IN endpoint exists.  */
    while (endpoint_tx != UX_NULL)
    {

        if ((endpoint_rx -> ux_slave_endpoint_descriptor.bEndpointAddress & UX_ENDPOINT_DIRECTION) == UX_ENDPOINT_OUT &&
            (endpoint_rx -> ux_slave_endpoint_descriptor.bmAttributes & UX_MASK_ENDPOINT_TYPE) == UX_BULK_ENDPOINT)
        {

            /* It's right endpoint we need.  */
            break;
        }

        /* Try next endpoint.  */
        endpoint_rx =  endpoint_rx -> ux_slave_endpoint_next_endpoint;
    }

    /* Check if we found right endpoint.  */
    if (endpoint_rx == UX_NULL)
        return (UX_ERROR);

    /* Default MIDI protocol is report protocol.  */
    midi -> ux_device_class_midi_protocol = UX_DEVICE_CLASS_MIDI_PROTOCOL;

    /* Save the endpoint in the midi instance.  */
    midi -> ux_device_class_midi_tx_endpoint = endpoint_tx;

    /* Resume thread.  */
    _ux_utility_thread_resume(&class -> ux_slave_class_thread);
    
    /* If there is a activate function call it.  */
    if (midi -> ux_slave_class_midi_instance_activate != UX_NULL)
    {        

        /* Invoke the application.  */
        midi -> ux_slave_class_midi_instance_activate(midi);
    }

    /* If trace is enabled, insert this event into the trace buffer.  */
    UX_TRACE_IN_LINE_INSERT(UX_TRACE_DEVICE_CLASS_MIDI_ACTIVATE, midi, 0, 0, 0, UX_TRACE_DEVICE_CLASS_EVENTS, 0, 0)

    /* If trace is enabled, register this object.  */
    UX_TRACE_OBJECT_REGISTER(UX_TRACE_DEVICE_OBJECT_TYPE_INTERFACE, midi, 0, 0, 0)

    /* Return completion status.  */
    return(UX_SUCCESS);
}

