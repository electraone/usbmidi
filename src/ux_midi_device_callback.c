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

static ioport_level_t level = IOPORT_LEVEL_HIGH;

/* This example callback shows how to interpret an event for a HID keyboard. */
UINT ux_midi_device_callback(UX_SLAVE_CLASS_MIDI* midi, UX_SLAVE_CLASS_MIDI_EVENT* midi_event);
UINT ux_midi_device_callback(UX_SLAVE_CLASS_MIDI* midi, UX_SLAVE_CLASS_MIDI_EVENT* midi_event)
{
    /* Does nothing. */
    SSP_PARAMETER_NOT_USED(midi);
    SSP_PARAMETER_NOT_USED(midi_event);

    /* Determine the next state of the LEDs */
    if(IOPORT_LEVEL_LOW == level)
    {
        level = IOPORT_LEVEL_HIGH;
    }
    else
    {
        level = IOPORT_LEVEL_LOW;
    }

    g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, level);

    return UX_SUCCESS;
}

