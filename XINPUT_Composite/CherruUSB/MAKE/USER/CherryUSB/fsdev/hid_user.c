/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "usbd_core.h"
#include "usbd_hid.h"
#include "usbd_gamepad.h"

extern volatile __uint8_t Flag_ex;

#define USBD_VID           0x3537-1
#define USBD_PID           0x1040

// #define USBD_VID           0xFFFF
// #define USBD_PID           0xFFFF

#define USBD_VID1           0x045e
#define USBD_PID1           0x028e
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define HID_INT_EP          0x81
#define HID_INT_EP_SIZE     8
#define HID_INT_EP_INTERVAL 10

#define USB_CONFIG_SIZE       34
#define HID_KEYBOARD_REPORT_DESC_SIZE 63
#define USBD_CUSTOM_HID_REPORT_KEYBOARD_SIZE 67
#define USBD_CUSTOM_HID_REPORT_KEYPAD_SIZE 47

static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0X0114, 0x01)
};

__ALIGN_BEGIN static uint8_t config_descriptor[105] __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESCRIPTOR_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  105,
  /* wTotalLength: Bytes returned */
  0x00,
  0x03,         /*bNumInterfaces:  interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xC0,         /*bmAttributes: bus powered */
  0xA0,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    0x09,         /*bLength: Interface Descriptor size*/
  USB_DESCRIPTOR_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x00,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x02,         /*bNumEndpoints*/
  0xFF,         /*bInterfaceClass: CUSTOM_HID*/
  0x5D,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of CUSTOM_HID *************************/
  /* 18 */
  0x10, 0x21, 0x10, 0x1, 
  0x1, 0x24, 0x81, 0x14, 
  0x3, 0x0, 0x3, 0x13, 
  0x2, 0x0, 0x3, 0x0,
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESCRIPTOR_TYPE_ENDPOINT, /*bDescriptorType:*/
  0x81,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  0x20, /*wMaxPacketSize: 2 Byte max */
  0x00,
  0x01,          /*bInterval: Polling Interval */
  /* 34 */

  0x07,          /* bLength: Endpoint Descriptor size */
  USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
  0x02,  /*bEndpointAddress: Endpoint Address (OUT)*/
  0x03, /* bmAttributes: Interrupt endpoint */
  0x20,  /* wMaxPacketSize: 2 Bytes max  */
  0x00,
  0x08,  /* bInterval: Polling Interval */

  /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESCRIPTOR_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x01,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x02,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: CUSTOM_HID*/
  0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x01,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of CUSTOM_HID *************************/
  /* 18 */
  0x09,         /*bLength: CUSTOM_HID Descriptor size*/
  HID_DESCRIPTOR_TYPE_HID, /*bDescriptorType: CUSTOM_HID*/
  0x11,         /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  USBD_CUSTOM_HID_REPORT_KEYBOARD_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESCRIPTOR_TYPE_ENDPOINT, /*bDescriptorType:*/
  0x82,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  0x08, /*wMaxPacketSize: 2 Byte max */
  0x00,
  0x02,          /*bInterval: Polling Interval */
  /* 34 */

  0x07,          /* bLength: Endpoint Descriptor size */
  USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
  0x01,  /*bEndpointAddress: Endpoint Address (OUT)*/
  0x03, /* bmAttributes: Interrupt endpoint */
  0x08,  /* wMaxPacketSize: 2 Bytes max  */
  0x00,
  0x05,  /* bInterval: Polling Interval */
  /* 41 */

//    /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESCRIPTOR_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  0x02,         /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x01,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: CUSTOM_HID*/
  0x01,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  /******************** Descriptor of CUSTOM_HID *************************/
  /* 18 */
  0x09,         /*bLength: CUSTOM_HID Descriptor size*/
  HID_DESCRIPTOR_TYPE_HID, /*bDescriptorType: CUSTOM_HID*/
  0x11,         /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  USBD_CUSTOM_HID_REPORT_KEYPAD_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESCRIPTOR_TYPE_ENDPOINT, /*bDescriptorType:*/
  0X83,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  0x08, /*wMaxPacketSize: 2 Byte max */
  0x00,
  0x02          /*bInterval: Polling Interval */
  /* 34 */

//  0x07,          /* bLength: Endpoint Descriptor size */
//  USB_DESC_TYPE_ENDPOINT, /* bDescriptorType: */
//  0X01,  /*bEndpointAddress: Endpoint Address (OUT)*/
//  0x03, /* bmAttributes: Interrupt endpoint */
//  CUSTOM_HID_EPOUT_SIZE,  /* wMaxPacketSize: 2 Bytes max  */
//  0x00,
//  CUSTOM_HID_FS_BINTERVAL,  /* bInterval: Polling Interval */
  /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */

};

uint8_t vendor_xbox[] = 
{
    0x28, 0x0, 0x0, 0x0, 
    0x0, 0x1, 0x4, 0x0, 
    0x1, 0x0, 0x0, 0x0, 
    0x0, 0x0, 0x0, 0x0, 
    0x0, 0x1, 0x58, 0x55, 
    0x53, 0x42, 0x31, 0x30, 
    0x0, 0x0, 0x0, 0x0, 
    0x0, 0x0, 0x0, 0x0, 
    0x0, 0x0, 0x0, 0x0, 
    0x0, 0x0, 0x0, 0x0
};
 uint8_t msos_string_descriptor[] = {
    0x12,              // bLength = 18
    0x03,              // bDescriptorType (String)
    'M',0,'S',0,'F',0,'T',0,'1',0,'0',0,'0',0,  // "MSFT100" (UTF-16LE)
    0xB0,              // bMS_VendorCode = 0xB0 (自定义，步骤2中使用)
    0x00               // bPad
};
const struct usb_msosv1_descriptor XBOX_msosv1_descriptor = 
{
    .string = msos_string_descriptor,
    .compat_id = vendor_xbox,
    .vendor_code = 0xB0,
    .comp_id_property = 0
};
static const uint8_t device_quality_descriptor[] = {
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x00,
    0x00,
};

static const char *string_descriptors[] = {
    (const char[]){ 0x09, 0x04 }, /* Langid */
    "BaiJiang",                  /* Manufacturer */
    "BaiJiang xbox",         /* Product */
     "V1.0",                 /* Serial Number */
};

static const uint8_t *device_descriptor_callback(uint8_t speed)
{
    return device_descriptor;
}

static const uint8_t *config_descriptor_callback(uint8_t speed)
{
    return config_descriptor;
}

static const uint8_t *device_quality_descriptor_callback(uint8_t speed)
{
    return device_quality_descriptor;
}

static const char *string_descriptor_callback(uint8_t speed, uint8_t index)
{
    if (index >= (sizeof(string_descriptors) / sizeof(char *))) {
        return NULL;
    }
    return string_descriptors[index];
}

const struct usb_descriptor hid_descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .device_quality_descriptor_callback = device_quality_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback,
    .msosv1_descriptor = &XBOX_msosv1_descriptor
};

__ALIGN_BEGIN uint8_t CUSTOM_HID_ReportDesc_KEYBOARD[USBD_CUSTOM_HID_REPORT_KEYBOARD_SIZE]  __ALIGN_END =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)

    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x04,                    //   USAGE_MINIMUM (Keyboard a and A)
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x62,                    //   REPORT_COUNT (98)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x06,                    //   REPORT_SIZE (6)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0xc0                           // END_COLLECTION
};

__ALIGN_BEGIN uint8_t CUSTOM_HID_ReportDesc_KEYPAD[USBD_CUSTOM_HID_REPORT_KEYPAD_SIZE]  __ALIGN_END =
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    
    0xa1, 0x00,                    //   COLLECTION (Physical)
    
//    0X85, 0X01,                    // REPORT ID(02)
    
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x10,                    //     USAGE_MAXIMUM (Button 16)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x10,                    //     REPORT_COUNT (16)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x32,                    //     Usage (Z)
    0x09, 0x33,                    //     Usage (Rx)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    
    0xc0,                          //     END_COLLECTION
    0xc0,                           // END_COLLECTION
};
unsigned char keyaa[118] = {0x5, 0x1, 0x9, 0x5, 0xa1, 0x1, 0xa1, 0x0, 0x85, 0x1, 0x5, 0x9, 0x19, 0x1, 0x29, 0x10, 0x15, 0x0, 0x25, 0x1, 0x95, 0x10, 0x75, 0x1, 0x81, 0x2, 0x5, 0x1, 0x9, 0x30, 0x9, 0x31, 0x9, 0x32, 0x9, 0x33, 0x15, 0x0, 0x26, 0xff, 0x0, 0x75, 0x8, 0x95, 0x4, 0x81, 0x2, 0xc0, 0xc0, 0x5, 0x1, 0x9, 0x6, 0xa1, 0x1, 0x85, 0x2, 0x5, 0x7, 0x19, 0xe0, 0x29, 0xe7, 0x15, 0x0, 0x25, 0x1, 0x95, 0x8, 0x75, 0x1, 0x81, 0x2, 0x5, 0x7, 0x19, 0x4, 0x29, 0x65, 0x15, 0x0, 0x25, 0x1, 0x95, 0x62, 0x75, 0x1, 0x81, 0x2, 0x95, 0x1, 0x75, 0x6, 0x81, 0x3, 0x5, 0x8, 0x19, 0x1, 0x29, 0x5, 0x15, 0x0, 0x25, 0x1, 0x95, 0x5, 0x75, 0x1, 0x91, 0x2, 0x95, 0x1, 0x75, 0x3, 0x91, 0x3, 0xc0};

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

#define HID_MAX_MPS 64
volatile uint8_t read_buffer[HID_MAX_MPS];
volatile uint8_t out_flag = 0;

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t hid_state = HID_STATE_IDLE;

static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            break;
        case USBD_EVENT_CONNECTED:
            break;
        case USBD_EVENT_DISCONNECTED:
            break;
        case USBD_EVENT_RESUME:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_CONFIGURED:
        usbd_ep_start_read(busid, 0x02, (uint8_t*)read_buffer, HID_MAX_MPS);
            hid_state = HID_STATE_IDLE;
            out_flag = 1;
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;

        default:
            break;
    }
}

void usbd_hid_int_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    hid_state = HID_STATE_IDLE;
}

static struct usbd_endpoint hid_in_ep1 = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = 0x81
};
static struct usbd_endpoint hid_in_ep2 = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = 0x82
};
static struct usbd_endpoint hid_in_ep3 = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = 0x83
};
static struct usbd_endpoint hid_in_ep4 = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = 0x84
};
static struct usbd_endpoint hid_in_ep5 = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = 0x85
};
static struct usbd_endpoint hid_in_ep6 = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = 0x86
};
static struct usbd_endpoint hid_in_ep7 = {
    .ep_cb = usbd_hid_int_callback,
    .ep_addr = 0x87
};

void usbd_hid_out_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    hid_state = HID_STATE_IDLE;
    switch (ep)
    {
    case 0x02: 
        /* code */
        usbd_ep_start_read(busid, ep, (uint8_t*)read_buffer, HID_MAX_MPS);
        break;
    default:
        break;
    }
}

static struct usbd_endpoint hid_out_ep1 = {
    .ep_cb = usbd_hid_out_callback,
    .ep_addr = 0x01
};
static struct usbd_endpoint hid_out_ep2 = {
    .ep_cb = usbd_hid_out_callback,
    .ep_addr = 0x02
};
static struct usbd_endpoint hid_out_ep3 = {
    .ep_cb = usbd_hid_out_callback,
    .ep_addr = 0x03
};
static struct usbd_endpoint hid_out_ep4 = {
    .ep_cb = usbd_hid_out_callback,
    .ep_addr = 0x04
};
static struct usbd_endpoint hid_out_ep5 = {
    .ep_cb = usbd_hid_out_callback,
    .ep_addr = 0x05
};
static struct usbd_endpoint hid_out_ep6 = {
    .ep_cb = usbd_hid_out_callback,
    .ep_addr = 0x06
};
static struct usbd_endpoint hid_out_ep7 = {
    .ep_cb = usbd_hid_out_callback,
    .ep_addr = 0x07
};

struct usbd_interface intf0,intf1,intf2,intf3;

void hid_keyboard_init(uint8_t busid, uintptr_t reg_base)
{

    usbd_desc_register(busid, &hid_descriptor);

    usbd_add_interface(busid, (struct usbd_interface *)usbd_gamepad_xinput_init_intf(&intf0));
    usbd_add_interface(busid, usbd_hid_init_intf(busid, &intf1, CUSTOM_HID_ReportDesc_KEYBOARD, 67));
    usbd_add_interface(busid, usbd_hid_init_intf(busid, &intf2, CUSTOM_HID_ReportDesc_KEYPAD, 47));

    usbd_add_endpoint(busid, &hid_in_ep1);
    usbd_add_endpoint(busid, &hid_in_ep2);
    usbd_add_endpoint(busid, &hid_in_ep3);
    usbd_add_endpoint(busid, &hid_in_ep4);
    usbd_add_endpoint(busid, &hid_in_ep5);
    usbd_add_endpoint(busid, &hid_in_ep6);
    usbd_add_endpoint(busid, &hid_in_ep7);

    usbd_add_endpoint(busid, &hid_out_ep1);
    usbd_add_endpoint(busid, &hid_out_ep2);
    usbd_add_endpoint(busid, &hid_out_ep3);
    usbd_add_endpoint(busid, &hid_out_ep4);
    usbd_add_endpoint(busid, &hid_out_ep5);
    usbd_add_endpoint(busid, &hid_out_ep6);
    usbd_add_endpoint(busid, &hid_out_ep7);
    
    
    usbd_initialize(busid, reg_base, usbd_event_handler);
}

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t write_buffer[64];

void hid_keyboard_test(uint8_t busid)
{
    const uint8_t sendbuffer[8] = { 0x00, 0x00, HID_KBD_USAGE_A, 0x00, 0x00, 0x00, 0x00, 0x00 };

    if(usb_device_is_configured(busid) == false) {
        return;
    }

    memcpy(write_buffer, sendbuffer, 8);
    hid_state = HID_STATE_BUSY;
    usbd_ep_start_write(busid, HID_INT_EP, write_buffer, 8);
    while (hid_state == HID_STATE_BUSY) {
    }
}

void hid_write_ep(uint8_t busid,uint8_t* data,uint16_t len,uint8_t ep)
{
    if(usb_device_is_configured(busid) == false) {
        return;
    }

    //memcpy(write_buffer, data, len);
    for(int a = 0;a < len;a++)
    {
        write_buffer[a] = data[a];
    }
    hid_state = HID_STATE_BUSY;
    usbd_ep_start_write(busid, ep, write_buffer, (uint32_t)len);
    while (hid_state == HID_STATE_BUSY) {
    }
}
