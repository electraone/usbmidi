#include "usb_midi_thread.h" // This is bad - the headers of SSP are not including the types
#include "usb_midi_api.h"


uint32_t sendNoteOn(void)
{
	UX_SLAVE_CLASS_MIDI_PACKET packet = {
	  .byte_0 = 0x09,
	  .midi_0 = 0x90,
	  .midi_1 = 0x70,
	  .midi_2 = 0x3f
	};

	return (packet.data);
}

uint32_t sendNoteOff(void)
{
	UX_SLAVE_CLASS_MIDI_PACKET packet = {
	  .byte_0 = 0x08,
	  .midi_0 = 0x80,
	  .midi_1 = 0x70,
	  .midi_2 = 0x3f
	};

	return (packet.data);
}
