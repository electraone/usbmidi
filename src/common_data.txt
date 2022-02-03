/* generated common source file - do not edit */
#include "common_data.h"
#include "ux_device_class_midi.h"

UCHAR _ux_system_slave_class_midi_name[] =                                  "ux_slave_class_midi";


extern UINT ux_midi_device_callback(UX_SLAVE_CLASS_MIDI *midi,
        UX_SLAVE_CLASS_MIDI_EVENT *midi_event);

/***********************************************************************************************************************
 * Helpers
 **********************************************************************************************************************/
#define LSB(n) ((n) & 0xFF)
#define MSB(n) (((n) >> 8) & 0xFF)
#define MIDI_INTERFACE_JACK_PAIR(a, b, c, d, e) \
6, 0x24, 0x02, 0x01, (a), (e), \
6, 0x24, 0x02, 0x02, (b), 0, \
9, 0x24, 0x03, 0x01, (c), 1, (b), 1, (e), \
9, 0x24, 0x03, 0x02, (d), 1, (a), 1, 0,

/***********************************************************************************************************************
 * Config variables
 **********************************************************************************************************************/
#define VENDOR_ID 0x1FC9
#define PRODUCT_ID 0x82CF
#define BCD_DEVICE 0x0213
#define MIDI_NUM_CABLES 3
#define MIDI_TX_ENDPOINT 1
#define MIDI_TX_SIZE 64
#define MIDI_RX_ENDPOINT 2
#define MIDI_RX_SIZE 64


/***********************************************************************************************************************
 * Electra One Descriptor definitions
 **********************************************************************************************************************/
/* Define the size of MIDI Descriptor to be used */
#define USB__DESCRIPTOR_TYPE      (1)

/***********************************************************************************************************************
 * USB Electra Interface Descriptor for FS mode g_usb_interface_descriptor_midi0
 **********************************************************************************************************************/
static volatile const unsigned char g_usb_interface_descriptor_midi_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_interface_desc_fs") BSP_ALIGN_VARIABLE_V2(1) = {

    /****************************************************************
     * Audio Control Class Interface Descriptor Requirement 9 bytes *
     ****************************************************************/
    0x9, /* 0 bLength */
    UX_INTERFACE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x00, /* 2 bInterfaceNumber */
    0x00, /* 4 bAlternateSetting  */
    0x00, /* 5 bNumEndpoints      */
    0x01, /* 6 bInterfaceClass    : Audio */
    0x01, /* 7 bInterfaceSubClass : Audio Control */
    0x00, /* 8 bInterfaceProtocol : unused */
    0x00, /* 9 iInterface Index   unused */

    /****************************************************************
     * Audio Control Interface Descriptor                           *
     ****************************************************************/
    0x9, /* 0 bLength */
    0x24, /* 1 bDescriptorType : Audio Control CS Interface */
    0x01, /* 2 bDescriptorSubtype: Header sub-type */
    0x00, /* 3 bcdADC: revision of class specification 1.0  LSB*/
    0x01, /* 3 bcdADC: revision of class specification 1.0  MSB */
    0x9, /* 4 wTotalLength: LSB */
    0x00, /* 5 wTotalLength: MSB */
    0x01, /* 6 bInCollection: Number of streaming interfqces */
    0x01, /* 7 baInterfaceNr(1) */

    /****************************************************************
     * Standard MS Interface Descriptor                             *
     ****************************************************************/
    9, /* 0 bLength */
    UX_INTERFACE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x01, /* 2 bInterfaceNumber */
    0x00, /* 4 bAlternateSetting  */
    0x02, /* 5 bNumEndpoints      */
    0x01, /* 6 bInterfaceClass    : Audio */
    0x03, /* 7 bInterfaceSubClass : Midi Streaming */
    0x00, /* 8 bInterfaceProtocol : unused */
    0x00, /* 9 iInterface Index   unused */

    /****************************************************************
     * MIDI MS Interface Header, USB MIDI 6.1.2.1, page 21          *
     ****************************************************************/
    7, /* 0 bLength */
    0x24, /* 1 bDescriptorType: CS_INTERFACE */
    0x01, /* 2 bDescriptorSubtype: MS_HEADER */
    0x00, /* 3 bcdMSC: revision 01.00 LSB*/
    0x01, /* 4 bcdMSC: revision 01.00 MSB*/
    LSB (7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES)), /* wTotalLength: LSB */
    MSB (7 + ((6 + 6 + 9 + 9) * MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES) + 9 + (4 + MIDI_NUM_CABLES)), /* wTotalLength: MSB */

    /****************************************************************
     * MIDI jacks definitions                                       *
     ****************************************************************/
    #if (MIDI_NUM_CABLES >= 1)
    MIDI_INTERFACE_JACK_PAIR (1, 2, 3, 4, 4)
    #endif
    #if (MIDI_NUM_CABLES >= 2)
    MIDI_INTERFACE_JACK_PAIR (5, 6, 7, 8, 5)
    #endif
    #if (MIDI_NUM_CABLES >= 3)
    MIDI_INTERFACE_JACK_PAIR (9, 10, 11, 12, 6)
    #endif

    /****************************************************************
     * Standard Bulk OUT Endpoint Descriptor, B.5.1, page 42        *
     ****************************************************************/
    9, /* bLength */
    0x05, /* bDescriptorType: ENDPOINT */
    MIDI_RX_ENDPOINT, /* bEndpointAddress */
    0x02, /* bmAttributes: Bulk */
    MIDI_RX_SIZE, 0, /* wMaxPacketSize */
    0x00, /* bInterval */
    0x00, /* bRefresh */
    0x00, /* bSynchAddress */

    /****************************************************************
     * Class-specific MS Bulk OUT Endpoint Descriptor, B.5.2,page 42*
     ****************************************************************/
    (4 + MIDI_NUM_CABLES), /* bLength */
    0x25, /* bDescriptorSubtype: CS_ENDPOINT */
    0x01, /* bJackType: MS_GENERAL */
    MIDI_NUM_CABLES, /* bNumEmbMIDIJack: number of jacks */
    1, /* BaAssocJackID(1): jack ID #1 */
    #if (MIDI_NUM_CABLES >= 2)
    5, /* BaAssocJackID(1): jack ID #5 */
    #endif
    #if (MIDI_NUM_CABLES >= 3)
    9, /* BaAssocJackID(1): jack ID #9 */
    #endif

    /****************************************************************
     * Standard Bulk IN Endpoint Descriptor, B.5.1, page 42         *
     ****************************************************************/
    9, /* bLength */
    0x05, /* bDescriptorType: ENDPOINT */
    MIDI_TX_ENDPOINT | 0x80, /* bEndpointAddress */
    0x02, /* bmAttributes: Bulk */
    MIDI_TX_SIZE, 0, /* wMaxPacketSize */
    0x00, /* bInterval */
    0x00, /* bRefresh */
    0x00, /* bSynchAddress */

    /****************************************************************
     * Class-specific MS Bulk IN Endpoint Descriptor, B.5.2, page 42*
     ****************************************************************/
    (4 + MIDI_NUM_CABLES), /* bLength */
    0x25, /* bDescriptorSubtype: CS_ENDPOINT */
    0x01, /* bJackType: MS_GENERAL */
    MIDI_NUM_CABLES, /* bNumEmbMIDIJack: number of jacks */
    3, /* BaAssocJackID(1):jack ID #3 */
    #if (MIDI_NUM_CABLES >= 2)
    7, /* BaAssocJackID(1):jack ID #7 */
    #endif
    #if (MIDI_NUM_CABLES >= 3)
    11,  /* BaAssocJackID(1):jack ID #11 */
    #endif
};

#if defined(__ICCARM__)
        #define ux_v2_err_callback_WEAK_ATTRIBUTE
        #pragma weak ux_v2_err_callback  = ux_v2_err_callback_internal
        #elif defined(__GNUC__)
        #define ux_v2_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("ux_v2_err_callback_internal")))
        #endif
void ux_v2_err_callback(void *p_instance, void *p_data)
ux_v2_err_callback_WEAK_ATTRIBUTE;
/* Static memory pool allocation used by USBX system. */
CHAR g_ux_pool_memory[18432];
/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void ux_v2_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void ux_v2_err_callback_internal(void *p_instance, void *p_data);
void ux_v2_err_callback_internal(void *p_instance, void *p_data) {
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED(p_instance);
    SSP_PARAMETER_NOT_USED(p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
}


/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ux_common_init0(void)
 **********************************************************************************************************************/
void ux_common_init0(void) {
    UINT status_ux_init;

    /** Initialize the USBX system. */
    status_ux_init = ux_system_initialize((CHAR*) g_ux_pool_memory, 18432, UX_NULL, 0);

    if (UX_SUCCESS != status_ux_init) {
        ux_v2_err_callback(NULL, &status_ux_init);
    }
}


#define g_sf_el_ux_dcd_fs_0_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("g_sf_el_ux_dcd_fs_0_err_callback_internal")))
void g_sf_el_ux_dcd_fs_0_err_callback(void *p_instance, void *p_data)
g_sf_el_ux_dcd_fs_0_err_callback_WEAK_ATTRIBUTE;
#if (SF_EL_UX_CFG_FS_IRQ_IPL != BSP_IRQ_DISABLED)
            /* USBFS ISR vector registering. */
            #if !defined(SSP_SUPPRESS_ISR_g_sf_el_ux_dcd_fs_0) && !defined(SSP_SUPPRESS_ISR_USB)
            SSP_VECTOR_DEFINE_UNIT(usbfs_int_isr, USB, FS, INT, 0);
            #endif
#endif


/* Prototype function for USBX DCD Initializer. */
void ux_dcd_initialize(void);

#undef SYNERGY_NOT_DEFINED
#define SYNERGY_NOT_DEFINED (1)

/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void g_sf_el_ux_dcd_fs_0_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void g_sf_el_ux_dcd_fs_0_err_callback_internal(void *p_instance, void *p_data);
void g_sf_el_ux_dcd_fs_0_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED(p_instance);
    SSP_PARAMETER_NOT_USED(p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
}


/***********************************************************************************************************************
 * The definition of wrapper interface for USBX Synergy Port DCD Driver to get a transfer module instance.
 **********************************************************************************************************************/
static UINT g_sf_el_ux_dcd_fs_0_initialize_transfer_support(ULONG dcd_io)
{
    UX_DCD_SYNERGY_TRANSFER dcd_transfer;
    dcd_transfer.ux_synergy_transfer_tx = (transfer_instance_t *)NULL;
    dcd_transfer.ux_synergy_transfer_rx = (transfer_instance_t *)NULL;
    return (UINT)ux_dcd_synergy_initialize_transfer_support(dcd_io, (UX_DCD_SYNERGY_TRANSFER *)&dcd_transfer);
}


/***********************************************************************************************************************
 * Initializes USBX Device Controller Driver.
 **********************************************************************************************************************/
void ux_dcd_initialize(void) {
    UINT status;

    /* Initializes the USB device controller, enabling DMA transfer if transfer module instances are given. */
    status = (UINT) ux_dcd_synergy_initialize(R_USBFS_BASE);

    if (UX_SUCCESS != status) {
        g_sf_el_ux_dcd_fs_0_err_callback(NULL, &status);
    }
} /* End of function ux_dcd_initialize() */


#define ux_device_err_callback_WEAK_ATTRIBUTE   __attribute__ ((weak, alias("ux_device_err_callback_internal")))

void ux_device_err_callback(void *p_instance, void *p_data)
ux_device_err_callback_WEAK_ATTRIBUTE;

/* Definition for the special linker section for USB */
extern uint32_t __usb_dev_descriptor_start_fs;
extern uint32_t __usb_descriptor_end_fs;


/* Counter to calculate number of Interfaces */
static uint8_t g_usbx_number_of_interface_count = 0;


/***********************************************************************************************************************
 * USB Device Descriptor for FS mode
 **********************************************************************************************************************/
static volatile const unsigned char device_framework_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_device_desc_fs") BSP_ALIGN_VARIABLE_V2(1) = {
    0x12, /* 0 bLength */
    UX_DEVICE_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    0x10, /* 2 bcdUSB BCD(1.1) */
    0x01, /* 3 bcdUSB */
    0x00, /* 4 bDeviceClass    : Device Class */
    0x00, /* 5 bDeviceSubClass : none */
    0x00, /* 6 bDeviceProtocol : none */
    0x40, /* 7 bMaxPacketSize0 */
    LSB(VENDOR_ID), /* 8 idVendor */
    MSB(VENDOR_ID), /* 9 idVendor */
    LSB(PRODUCT_ID), /* 10 idProduct */
    MSB(PRODUCT_ID), /* 11 idProduct */
    LSB(BCD_DEVICE), /* 12 bcdDevice */
    MSB(BCD_DEVICE), /* 13 bcdDevice */
    0x01, /* 14 iManufacturer */
    0x02, /* 15 iProduct */
    0x03, /* 16 iSerialNumber */
    0x01 /* 17 bNumConfigurations */
};

#define USB_CFG_DESCRIPTOR_bNumInterfaces     (0x02)
#define USB_CFG_DESCRIPTOR_wTotalLength       (0x00A5)


/***********************************************************************************************************************
 * USB Configuration Descriptor for FS mode
 **********************************************************************************************************************/
static volatile const unsigned char ux_cfg_descriptor_full_speed[] BSP_PLACE_IN_SECTION_V2(".usb_config_desc_fs") BSP_ALIGN_VARIABLE_V2(1) = {
    0x09, /* 0 bLength */
    UX_CONFIGURATION_DESCRIPTOR_ITEM, /* 1 bDescriptorType */
    LSB(USB_CFG_DESCRIPTOR_wTotalLength), /* 2 wTotalLength : This will be calculated at run-time. */
    MSB(USB_CFG_DESCRIPTOR_wTotalLength), /* 3 wTotalLength : This will be calculated at run-time. */
    (uint8_t)(USB_CFG_DESCRIPTOR_bNumInterfaces), /* 4 bNumInterfaces */
    0x01, /* 5 bConfigurationValue : Fixed to 1 since only one configuration is supported. */
    0x00, /* 6 iConfiguration */
    0x80, /* 7 bmAttributes */
    0xFA, /* 8 bMaxPower */
};


/***********************************************************************************************************************
 * USB Language Framework (Default setting for US English)
 **********************************************************************************************************************/

static volatile const UCHAR language_id_framework_default[] = {
  (uint8_t)(0x0409), /* Supported Language Code */
  (uint8_t)(0x0409 >> 8) /* US English as the default */
};


/***********************************************************************************************************************
 * USB String Framework (Manufacturer, ProductName, Serial Number)
 **********************************************************************************************************************/

#define STRING_FRAMEWORK_LENGTH 102
static volatile const UCHAR string_framework[] = {
        /* Manufacturer string descriptor : Index 1 */
        0x09, 0x04,
        0x01,
        11,
        'e', 'l', 'e', 'c', 't', 'r', 'a', '.', 'o', 'n', 'e',

        /* Product string descriptor : Index 2 */
        0x09, 0x04,
        0x02,
        18,
        'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'C', 'o', 'n', 't', 'r', 'o', 'l', 'l', 'e', 'r',

        /* Serial Number string descriptor : Index 3 */
        0x09, 0x04,
        0x03,
        9,
        'E', 'O', '-', '1', '2', '3', '4', '5', '6',

        /* Port label 1 */
        0x09, 0x04,
        0x04,
        14,
        'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'P', 'o', 'r', 't', ' ', '1' ,

        /* Port label 2 */
        0x09, 0x04,
        0x05,
        14,
        'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'P', 'o', 'r', 't', ' ', '2' ,

        /* Port label 3 */
        0x09, 0x04,
        0x06,
        12,
        'E', 'l', 'e', 'c', 't', 'r', 'a', ' ', 'C', 'T', 'R', 'L'
};


/*******************************************************************************************************************//**
 * @brief      This is a weak example initialization error function.  It should be overridden by defining a user  function 
 *             with the prototype below.
 *             - void ux_device_err_callback(void * p_instance, void * p_data)
 *
 * @param[in]  p_instance arguments used to identify which instance caused the error and p_data Callback arguments used to identify what error caused the callback.
 **********************************************************************************************************************/
void ux_device_err_callback_internal(void *p_instance, void *p_data);
void ux_device_err_callback_internal(void *p_instance, void *p_data)
{
    /** Suppress compiler warning for not using parameters. */
    SSP_PARAMETER_NOT_USED(p_instance);
    SSP_PARAMETER_NOT_USED(p_data);

    /** An error has occurred. Please check function arguments for more information. */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
}


/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ux_device_init0(void)
 **********************************************************************************************************************/
void ux_device_init0(void)
{
    UINT status_ux_device_init;

    /** Calculate the size of USBX String Framework. */
    ULONG string_framework_size = STRING_FRAMEWORK_LENGTH;
    UCHAR *p_string_framework = (UCHAR *) string_framework;

    /** Calculate the size of USB Language Framework. */
    ULONG language_framework_size = 2;
    UCHAR *p_language_framework = (UCHAR*) language_id_framework_default;

    /** Initialize the USB Device stack. */
    status_ux_device_init = ux_device_stack_initialize(
        NULL, 0,
        (UCHAR *)device_framework_full_speed, ((uint32_t)&__usb_descriptor_end_fs - (uint32_t)&__usb_dev_descriptor_start_fs),
        (UCHAR *)p_string_framework, string_framework_size,
        p_language_framework, language_framework_size, NULL);

    if (UX_SUCCESS != status_ux_device_init) {
        ux_device_err_callback(NULL, &status_ux_device_init);
    }

}







/***********************************************************************************************************************
 * USBX MIDI Class Parameter Configuration Function.
 **********************************************************************************************************************/
static UX_SLAVE_CLASS_MIDI_PARAMETER g_ux_device_class_midi_parameter;
static void g_ux_device_class_midi_setup(void);
static void g_ux_device_class_midi_setup(void)
{
    /* Initialize the MIDI class parameters for a keyboard.  */
    g_ux_device_class_midi_parameter.ux_device_class_midi_parameter_callback = ux_midi_device_callback;

    /* Register user callback functions.  */
    g_ux_device_class_midi_parameter.ux_slave_class_midi_instance_activate = NULL;
    g_ux_device_class_midi_parameter.ux_slave_class_midi_instance_deactivate = NULL;

    /* Initialize the device MIDI class. This class owns both interfaces starting with 1. */
    ux_device_stack_class_register(_ux_system_slave_class_midi_name,
            ux_device_class_midi_entry, 1, 0x01,
            (VOID*) &g_ux_device_class_midi_parameter);

    /* Counts up the number of Interfaces. */
    g_usbx_number_of_interface_count = (uint8_t)(g_usbx_number_of_interface_count + 1);
}


/*******************************************************************************************************************//**
 * @brief     Initialization function that the user can choose to have called automatically during thread entry.
 *            The user can call this function at a later time if desired using the prototype below.
 *            - void ux_device_class_midi_init0(void)
 **********************************************************************************************************************/
void ux_device_class_midi_init0(void) {
    /* Setups USB MIDI Class to create the instance (if required) */
    g_ux_device_class_midi_setup();

    /* USB DCD initialization (Only executed once after all the class registration completed). */
    if (g_usbx_number_of_interface_count == 1) { // USB_NUMBER_OF_INTERFACE
        /* Initializes USBX Device Control Driver */
        ux_dcd_initialize();
    }
}


const cgc_instance_t g_cgc = { .p_api = &g_cgc_on_cgc, .p_cfg = NULL };
/* Instance structure to use this module. */
const fmi_instance_t g_fmi = { .p_api = &g_fmi_on_fmi };
const ioport_instance_t g_ioport = { .p_api = &g_ioport_on_ioport, .p_cfg = NULL };
const elc_instance_t g_elc = { .p_api = &g_elc_on_elc, .p_cfg = NULL };


void g_common_init(void) {

    /** Call initialization function if user has selected to do so. */
    ux_common_init0();

    /** Call initialization function if user has selected to do so. */
    ux_device_init0();

    /** Call initialization function if user has selected to do so. */
    ux_device_class_midi_init0();

}
