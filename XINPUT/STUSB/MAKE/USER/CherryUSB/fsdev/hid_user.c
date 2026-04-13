/*
 * Copyright (c) 2024, sakumisu
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "usbd_core.h"
#include "usbd_hid.h"

#define USBD_VID           0x045e
#define USBD_PID           0x028e
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define HID_INT_EP          0x81
#define HID_INT_EP_SIZE     8
#define HID_INT_EP_INTERVAL 10

#define USB_CONFIG_SIZE       34
#define HID_KEYBOARD_REPORT_DESC_SIZE 63

static const uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0XFF, 0XFF, 0XFF, USBD_VID, USBD_PID, 0X0110, 0x01)
};
static const uint8_t config_descriptor[] = {
    XINPUT_CONFIG_DESCRIPTOR
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
static const char *string_descriptors[] = 
{
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
    .string_descriptor_callback = string_descriptor_callback
};


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
        out_flag = 1;
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
