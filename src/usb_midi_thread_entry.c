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
    ioport_level_t level = IOPORT_LEVEL_HIGH;

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

        /* Until the device stays configured.  */
        while (device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
        {
            /* Get the interface.  USB MIDI interface runs on second interface.  */
            interface =  device->ux_slave_device_first_interface;
            interface = interface->ux_slave_interface_next_interface;

            /* Form that interface, derive the MIDI owner.  */
            midi = interface->ux_slave_interface_class_instance;
            usbMIDI = midi;

            tx_thread_sleep(100);
#if (1)
            if(IOPORT_LEVEL_LOW == level)
            {
                level = IOPORT_LEVEL_HIGH;
            }
            else
            {
                level = IOPORT_LEVEL_LOW;
            }

            g_ioport.p_api->pinWrite(IOPORT_PORT_06_PIN_01, level);

            sendNoteOn(60, 127, 16, 2);
            //sendNoteOff(60, 127, 16, 2);
            //sendControlChange(1, 127, 16, 2);
#endif
        }
    }
}
