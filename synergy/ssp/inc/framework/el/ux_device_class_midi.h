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
/**   MIDI Class                                                           */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/ 
/*                                                                        */ 
/*  COMPONENT DEFINITION                                   RELEASE        */ 
/*                                                                        */ 
/*    ux_device_class_midi.h                               PORTABLE C     */
/*                                                           6.1.3        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Chaoqiong Xiao, Microsoft Corporation                               */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */ 
/*    This file contains all the header and extern functions used by the  */
/*    USBX MIDI class.                                                    */
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
/*  12-31-2020     Chaoqiong Xiao           Modified comment(s),          */
/*                                            added Get/Set Protocol      */
/*                                            request support,            */
/*                                            resulting in version 6.1.3  */
/*                                                                        */
/**************************************************************************/

#ifndef UX_DEVICE_CLASS_MIDI_H
#define UX_DEVICE_CLASS_MIDI_H

/* Define MIDI Class constants.  */

#define UX_DEVICE_CLASS_MIDI_CLASS                                  0x01
#define UX_DEVICE_CLASS_MIDI_SUBCLASS                               0X01
#define UX_DEVICE_CLASS_MIDI_PROTOCOL                               0X00

#ifndef UX_DEVICE_CLASS_MIDI_MAX_EVENTS_QUEUE
#define UX_DEVICE_CLASS_MIDI_MAX_EVENTS_QUEUE                       16
#endif

#define UX_DEVICE_CLASS_MIDI_NEW_EVENT                              1
#define UX_DEVICE_CLASS_MIDI_NEW_IDLE_RATE                          2
#define UX_DEVICE_CLASS_MIDI_EVENTS_MASK                            3

typedef union UX_SLAVE_CLASS_MIDI_PACKET_UNION
{
	struct {
		UCHAR byte_0;
		UCHAR midi_0;
		UCHAR midi_1;
		UCHAR midi_2;
	};
	UINT data;
} UX_SLAVE_CLASS_MIDI_PACKET;


typedef struct UX_SLAVE_CLASS_MIDI_EVENT_STRUCT
{
	UX_SLAVE_CLASS_MIDI_PACKET      ux_device_class_midi_packet;
} UX_SLAVE_CLASS_MIDI_EVENT;

#define UX_SLAVE_CLASS_MIDI_EVENT_SIZE 4

/* Define MIDI structure.  */

typedef struct UX_SLAVE_CLASS_MIDI_STRUCT
{

    UX_SLAVE_INTERFACE              *ux_slave_class_midi_interface;
    UX_THREAD                       ux_slave_class_midi_tx_thread;
    UX_THREAD                       ux_slave_class_midi_rx_thread;
    UCHAR                           *ux_slave_class_midi_tx_thread_stack;
    UCHAR                           *ux_slave_class_midi_rx_thread_stack;
    UX_SLAVE_ENDPOINT               *ux_device_class_midi_tx_endpoint;
    UX_SLAVE_ENDPOINT               *ux_device_class_midi_rx_endpoint;
    UINT                            ux_device_class_midi_state;
    UINT                            (*ux_device_class_midi_callback)(struct UX_SLAVE_CLASS_MIDI_STRUCT *midi, UX_SLAVE_CLASS_MIDI_EVENT *);
    VOID                            (*ux_slave_class_midi_instance_activate)(VOID *);
    VOID                            (*ux_slave_class_midi_instance_deactivate)(VOID *);
    UX_EVENT_FLAGS_GROUP            ux_device_class_midi_event_flags_group;
    ULONG                           ux_device_class_midi_event_idle_rate;
    ULONG                           ux_device_class_midi_event_wait_timeout;
    ULONG                           ux_device_class_midi_protocol;
    UX_SLAVE_CLASS_MIDI_EVENT       *ux_device_class_midi_event_array;
    UX_SLAVE_CLASS_MIDI_EVENT       *ux_device_class_midi_event_array_head;
    UX_SLAVE_CLASS_MIDI_EVENT       *ux_device_class_midi_event_array_tail;
    UX_SLAVE_CLASS_MIDI_EVENT       *ux_device_class_midi_event_array_end;
} UX_SLAVE_CLASS_MIDI;

/* Define MIDI initialization command structure.  */

typedef struct UX_SLAVE_CLASS_MIDI_PARAMETER_STRUCT
{

    VOID                    (*ux_slave_class_midi_instance_activate)(VOID *);
    VOID                    (*ux_slave_class_midi_instance_deactivate)(VOID *);
    UINT                    (*ux_device_class_midi_parameter_callback)(struct UX_SLAVE_CLASS_MIDI_STRUCT *midi, UX_SLAVE_CLASS_MIDI_EVENT *);
} UX_SLAVE_CLASS_MIDI_PARAMETER;


/* Define MIDI Class function prototypes.  */
UINT  _ux_device_class_midi_activate(UX_SLAVE_CLASS_COMMAND *command);
UINT  _ux_device_class_midi_deactivate(UX_SLAVE_CLASS_COMMAND *command);
UINT  _ux_device_class_midi_entry(UX_SLAVE_CLASS_COMMAND *command);
VOID  _ux_device_class_midi_tx_thread(ULONG midi_class);
VOID  _ux_device_class_midi_rx_thread(ULONG midi_class);
UINT  _ux_device_class_midi_initialize(UX_SLAVE_CLASS_COMMAND *command);
UINT  _ux_device_class_midi_uninitialize(UX_SLAVE_CLASS_COMMAND *command);
UINT  _ux_device_class_midi_event_set(UX_SLAVE_CLASS_MIDI *midi,
                                      UX_SLAVE_CLASS_MIDI_EVENT *midi_event);
UINT  _ux_device_class_midi_event_get(UX_SLAVE_CLASS_MIDI *midi,
                                      UX_SLAVE_CLASS_MIDI_EVENT *midi_event);

VOID ux_device_class_midi_send_packet(UINT packet);

/* Define Device MIDI Class API prototypes.  */

#define ux_device_class_midi_entry        _ux_device_class_midi_entry
#define ux_device_class_midi_event_set    _ux_device_class_midi_event_set
#define ux_device_class_midi_event_get    _ux_device_class_midi_event_get

#define ux_device_class_midi_protocol_get(midi) (midi -> ux_device_class_midi_protocol)


#endif
