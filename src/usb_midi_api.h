#ifndef USB_MIDI_API_H
#define USB_MIDI_API_H

#include <stdint.h>
#include "ux_device_class_midi.h"


extern UX_SLAVE_CLASS_MIDI* usbMIDI;


void send(uint8_t type, uint8_t data1, uint8_t data2, uint8_t channel, uint8_t cable);
void begin(void);
void end(void);
void sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable);
void sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable);
void sendPolyPressure(uint8_t note, uint8_t pressure, uint8_t channel, uint8_t cable);
void sendAfterTouchPoly(uint8_t note, uint8_t pressure, uint8_t channel, uint8_t cable);
void sendControlChange(uint8_t control, uint8_t value, uint8_t channel, uint8_t cable);
void sendProgramChange(uint8_t program, uint8_t channel, uint8_t cable);
void sendAfterTouch(uint8_t pressure, uint8_t channel, uint8_t cable);
void sendPitchBend(int value, uint8_t channel, uint8_t cable);
void sendRealTime(uint8_t type, uint8_t cable);
void sendTimeCodeQuarterFrame(uint8_t type, uint8_t value, uint8_t cable);
void sendSongPosition(uint16_t beats, uint8_t cable);
void sendSongSelect(uint8_t song, uint8_t cable);
void sendTuneRequest(uint8_t cable);
void beginRpn(uint16_t number, uint8_t channel, uint8_t cable);
void sendRpnValue(uint16_t value, uint8_t channel, uint8_t cable);
void sendRpnIncrement(uint8_t amount, uint8_t channel, uint8_t cable);
void sendRpnDecrement(uint8_t amount, uint8_t channel, uint8_t cable);
void endRpn(uint8_t channel, uint8_t cable);
void beginNrpn(uint16_t number, uint8_t channel, uint8_t cable);
void sendNrpnValue(uint16_t value, uint8_t channel, uint8_t cable);
void sendNrpnIncrement(uint8_t amount, uint8_t channel, uint8_t cable);
void sendNrpnDecrement(uint8_t amount, uint8_t channel, uint8_t cable);
void endNrpn(uint8_t channel, uint8_t cable);

#endif /* USB_MIDI_API_H */
