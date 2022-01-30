#include "usb_midi_thread.h"
#include "ux_device_class_midi.h"
#include "usb_midi_api.h"

/* USB MIDI entry function */
void usb_midi_thread_entry(void)
{
    UX_SLAVE_DEVICE*    device;
    UX_SLAVE_INTERFACE* interface;
    UX_SLAVE_CLASS_MIDI* midi;
    UX_SLAVE_CLASS_MIDI_EVENT midi_event;

    ioport_level_t level = IOPORT_LEVEL_LOW;

    /* Get the pointer to the device.  */
    device =  &_ux_system_slave->ux_system_slave_device;

    /* reset the HID event structure.  */
    ux_utility_memory_set(&midi_event, 0, sizeof(UX_SLAVE_CLASS_MIDI_EVENT));

    while (1)
    {
        /* Is the device configured ? */
        while (device->ux_slave_device_state != UX_DEVICE_CONFIGURED)
        {
            /* Then wait.  */
            tx_thread_sleep(10);
        }

        g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_00, IOPORT_LEVEL_HIGH);

        /* Until the device stays configured.  */
        while (device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
        {
            if(IOPORT_LEVEL_LOW == level)
            {
                level = IOPORT_LEVEL_HIGH;
            }
            else
            {
                level = IOPORT_LEVEL_LOW;
            }

            g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, level);

            /* Get the interface.  USB MIDI interface runs on second interface.  */
            interface =  device->ux_slave_device_first_interface;
            interface = interface->ux_slave_interface_next_interface;

            /* Form that interface, derive the MIDI owner.  */
            midi = interface->ux_slave_interface_class_instance;

            tx_thread_sleep(10);


            UX_SLAVE_CLASS_MIDI_PACKET packet;

            packet.data = sendNoteOn();
            midi_event.ux_device_class_midi_packet.data = packet.data;

#if (0)
            /* Cable 0 and CIN 9 (note off).  */
            midi_event.ux_device_class_midi_event_buffer[0] = packet.byte_0;

            /* Note off (9). */
            midi_event.ux_device_class_midi_event_buffer[1] = packet.midi_0;

            /* Note nr  */
            midi_event.ux_device_class_midi_event_buffer[2] = packet.midi_1;

            /* Velocity.  */
            midi_event.ux_device_class_midi_event_buffer[3] = packet.midi_2;
#endif
            /* Set the keyboard event.  */
            ux_device_class_midi_event_set(midi, &midi_event);

        }
    }
}
