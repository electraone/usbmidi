/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
#include "ux_api.h"
#include "ux_device_class_midi.h"

#include "common_data.h"

ULONG num_lock_flag  = UX_FALSE;
ULONG caps_lock_flag = UX_FALSE;

#define  MIDI_NUM_LOCK_MASK                 1
#define  MIDI_CAPS_LOCK_MASK                2


/* This example callback shows how to interpret an event for a HID keyboard. */
UINT ux_midi_device_callback(UX_SLAVE_CLASS_MIDI* midi, UX_SLAVE_CLASS_MIDI_EVENT* midi_event)
{
    /* Does nothing. */
    SSP_PARAMETER_NOT_USED(midi);

#if (0)
    /* There was an event.  Analyze it.  Is it NUM LOCK ? */
    if (midi_event->ux_device_class_midi_event_buffer[0] & MIDI_NUM_LOCK_MASK)
    {
        /* Set the Num lock flag.  */
        num_lock_flag = UX_TRUE;
    }
    else
    {
        /* Reset the Num lock flag.  */
        num_lock_flag = UX_FALSE;
    }

    /* There was an event.  Analyze it.  Is it CAPS LOCK ? */
    if (midi_event->ux_device_class_midi_event_buffer[0] & MIDI_CAPS_LOCK_MASK)
    {
        /* Set the Caps lock flag.  */
        caps_lock_flag = UX_TRUE;
    }
    else
    {
        /* Reset the Caps lock flag.  */
        caps_lock_flag = UX_FALSE;
    }
#endif

    return UX_SUCCESS;
}

