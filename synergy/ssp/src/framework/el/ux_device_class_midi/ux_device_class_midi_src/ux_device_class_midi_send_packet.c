#define UX_SOURCE_CODE


/* Include necessary system files.  */

#include "ux_api.h"
#include "ux_device_class_midi.h"
#include "ux_device_stack.h"

extern UX_SLAVE_CLASS_MIDI* usbMIDI;

VOID ux_device_class_midi_send_packet(UINT packet)
{
    UX_SLAVE_CLASS_MIDI_EVENT midi_event;

    midi_event.ux_device_class_midi_packet.data = packet;
    ux_device_class_midi_event_set (usbMIDI, &midi_event);
}
