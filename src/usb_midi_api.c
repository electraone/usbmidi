#include "usb_midi_thread.h" // This is bad - the headers of SSP are not including the types
#include "usb_midi_api.h"

UX_SLAVE_CLASS_MIDI *usbMIDI;

void send(uint8_t type, uint8_t data1, uint8_t data2, uint8_t channel, uint8_t cable)
{
    UX_SLAVE_CLASS_MIDI_EVENT midi_event;
    UX_SLAVE_CLASS_MIDI_PACKET packet;

    if (cable >= 3)
    {
        return;
    }

    if (type < 0xF0)
    {
        if (type < 0x80)
        {
            return;
        }

        type &= 0xF0;
        packet.data = (type << 8) | (type >> 4) | ((cable & 0x0F) << 4) | (((channel - 1) & 0x0F) << 8)
                | ((data1 & 0x7F) << 16) | ((data2 & 0x7F) << 24);
    }
    else if (type >= 0xF8 || type == 0xF6)
    {
        packet.data = (type << 8) | 0x0F | ((cable & 0x0F) << 4);
    }
    else if (type == 0xF1 || type == 0xF3)
    {
        packet.data = (type << 8) | 0x02 | ((cable & 0x0F) << 4) | ((data1 & 0x7F) << 16);
    }
    else if (type == 0xF2)
    {
        packet.data = (type << 8) | 0x03 | ((cable & 0x0F) << 4) | ((data1 & 0x7F) << 16) | ((data2 & 0x7F) << 24);
    }

    midi_event.ux_device_class_midi_packet.data = packet.data;
    ux_device_class_midi_event_set (usbMIDI, &midi_event);
}

void begin(void)
{
}
void end(void)
{
}
void sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable)
{
    send (0x80, note, velocity, channel, cable);
}
void sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel, uint8_t cable)
{
    send (0x90, note, velocity, channel, cable);
}
void sendPolyPressure(uint8_t note, uint8_t pressure, uint8_t channel, uint8_t cable)
{
    send (0xA0, note, pressure, channel, cable);
}
void sendAfterTouchPoly(uint8_t note, uint8_t pressure, uint8_t channel, uint8_t cable)
{
    send (0xA0, note, pressure, channel, cable);
}
void sendControlChange(uint8_t control, uint8_t value, uint8_t channel, uint8_t cable)
{
    send (0xB0, control, value, channel, cable);
}
void sendProgramChange(uint8_t program, uint8_t channel, uint8_t cable)
{
    send (0xC0, program, 0, channel, cable);
}
void sendAfterTouch(uint8_t pressure, uint8_t channel, uint8_t cable)
{
    send (0xD0, pressure, 0, channel, cable);
}
void sendPitchBend(int value, uint8_t channel, uint8_t cable)
{
    if (value < -8192)
    {
        value = -8192;
    }
    else if (value > 8191)
    {
        value = 8191;
    }
    value += 8192;
    send (0xE0, value, value >> 7, channel, cable);
}

void sendRealTime(uint8_t type, uint8_t cable)
{
    switch (type)
    {
        case 0xF8: // Clock
        case 0xFA: // Start
        case 0xFB: // Continue
        case 0xFC: // Stop
        case 0xFE: // ActiveSensing
        case 0xFF: // SystemReset
            send (type, 0, 0, 0, cable);
        break;
        default: // Invalid Real Time marker
        break;
    }
}
void sendTimeCodeQuarterFrame(uint8_t type, uint8_t value, uint8_t cable)
{
    send (0xF1, ((type & 0x07) << 4) | (value & 0x0F), 0, 0, cable);
}
void sendSongPosition(uint16_t beats, uint8_t cable)
{
    send (0xF2, beats, beats >> 7, 0, cable);
}
void sendSongSelect(uint8_t song, uint8_t cable)
{
    send (0xF3, song, 0, 0, cable);
}
void sendTuneRequest(uint8_t cable)
{
    send (0xF6, 0, 0, 0, cable);
}
void beginRpn(uint16_t number, uint8_t channel, uint8_t cable)
{
    sendControlChange (101, number >> 7, channel, cable);
    sendControlChange (100, number, channel, cable);
}
void sendRpnValue(uint16_t value, uint8_t channel, uint8_t cable)
{
    sendControlChange (6, value >> 7, channel, cable);
    sendControlChange (38, value, channel, cable);
}
void sendRpnIncrement(uint8_t amount, uint8_t channel, uint8_t cable)
{
    sendControlChange (96, amount, channel, cable);
}
void sendRpnDecrement(uint8_t amount, uint8_t channel, uint8_t cable)
{
    sendControlChange (97, amount, channel, cable);
}
void endRpn(uint8_t channel, uint8_t cable)
{
    sendControlChange (101, 0x7F, channel, cable);
    sendControlChange (100, 0x7F, channel, cable);
}
void beginNrpn(uint16_t number, uint8_t channel, uint8_t cable)
{
    sendControlChange (99, number >> 7, channel, cable);
    sendControlChange (98, number, channel, cable);
}
void sendNrpnValue(uint16_t value, uint8_t channel, uint8_t cable)
{
    sendControlChange (6, value >> 7, channel, cable);
    sendControlChange (38, value, channel, cable);
}
void sendNrpnIncrement(uint8_t amount, uint8_t channel, uint8_t cable)
{
    sendControlChange (96, amount, channel, cable);
}
void sendNrpnDecrement(uint8_t amount, uint8_t channel, uint8_t cable)
{
    sendControlChange (97, amount, channel, cable);
}
void endNrpn(uint8_t channel, uint8_t cable)
{
    sendControlChange (99, 0x7F, channel, cable);
    sendControlChange (98, 0x7F, channel, cable);
}
