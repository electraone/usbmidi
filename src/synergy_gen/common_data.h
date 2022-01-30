/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_fmi.h"
#include "r_fmi_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "ux_api.h"

/* USBX Host Stack initialization error callback function. User can override the function if needed. */
void ux_v2_err_callback(void *p_instance, void *p_data);

#if !defined(NULL)
/* User Callback for Host Event Notification (Only valid for USB Host). */
extern UINT NULL(ULONG event, UX_HOST_CLASS *host_class, VOID *instance);
#endif

#if !defined(NULL)
/* User Callback for Device Event Notification (Only valid for USB Device). */
extern UINT NULL(ULONG event);
#endif

#ifdef UX_HOST_CLASS_STORAGE_H
            /* Utility function to get the pointer to a FileX Media Control Block for a USB Mass Storage device. */
            UINT ux_system_host_storage_fx_media_get(UX_HOST_CLASS_STORAGE * instance, UX_HOST_CLASS_STORAGE_MEDIA ** p_storage_media, FX_MEDIA ** p_fx_media);
#endif
void ux_common_init0(void);

/* Function prototype for the function to register the USBX Host Class Mass Storage. */
void ux_host_stack_class_register_storage(void);
/* Function prototype for the function to notify a USB event from the USBX Host system. */
UINT ux_system_host_change_function(ULONG event, UX_HOST_CLASS *host_class, VOID *instance);
/* memory pool allocation used by USBX system. */
extern CHAR g_ux_pool_memory[];
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;
#ifndef NULL
void NULL(transfer_callback_args_t *p_args);
#endif
#include "ux_api.h"
#include "ux_dcd_synergy.h"
#include "sf_el_ux_dcd_fs_cfg.h"
void g_sf_el_ux_dcd_fs_0_err_callback(void *p_instance, void *p_data);
#include "ux_api.h"
#include "ux_dcd_synergy.h"

/* USBX Device Stack initialization error callback function. User can override the function if needed. */
void ux_device_err_callback(void *p_instance, void *p_data);
void ux_device_init0(void);
void ux_device_remove_compiler_padding(unsigned char *p_device_framework, UINT length);
#include "ux_api.h"
#include "ux_device_class_hid.h"
/* USBX Device Class HID Entry Function. */
extern UINT ux_device_class_hid_entry(UX_SLAVE_CLASS_COMMAND *command);

/* Function for USBX Device Class Keyboard HID Instance Activate/Deactivate Function Alias. */
#ifndef NULL
extern VOID NULL(VOID *hid_instance);
#endif
#ifndef NULL
extern VOID NULL(VOID *hid_instance);
#endif	
/* Function for USBX Device Class Mouse HID Instance Activate/Deactivate Function Alias. */
#ifndef NULL
extern VOID NULL(VOID *hid_instance);
#endif
#ifndef NULL
extern VOID NULL(VOID *hid_instance);
#endif
#ifndef ux_hid_device_callback
extern UINT ux_hid_device_callback(UX_SLAVE_CLASS_HID *hid, UX_SLAVE_CLASS_HID_EVENT *hid_event);
#endif
#ifndef NULL
extern UINT NULL(UX_SLAVE_CLASS_HID *hid, UX_SLAVE_CLASS_HID_EVENT *hid_event);
#endif
void ux_device_class_hid_init0(void);
/** FMI on FMI Instance. */
extern const fmi_instance_t g_fmi;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
/** ELC Instance */
extern const elc_instance_t g_elc;
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
void g_common_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* COMMON_DATA_H_ */
