#ifndef UX_USB_MIDI_API_H
#define UX_USB_MIDI_API_H

#include <stdint.h>
#include "ux_device_class_midi.h"

uint32_t sendNoteOn(void);
uint32_t sendNoteOff(void);

#endif /* UX_USB_MIDI_API_H */
