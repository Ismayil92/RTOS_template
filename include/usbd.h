#ifndef USBD_H
#define USBD_H

#include <libopencm3/usb/cdc.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/dwc/otg_fs.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "usb_descriptors.h"

static const uint8_t PC_ADDRESS = 0x82;
static const uint8_t DEVICE_ADDRESS = 0x01;


// control request buffer
static uint8_t usbd_control_buffer[128];

static const char *usb_strings[] = {
	"STMicroelectronics",
	"STM32 Virtual Com Port",
	"204338704D46",
};


void cdcacm_set_config(usbd_device *usbd_dev, uint16_t wValue);
void cdcacm_data_rx_cb(usbd_device *usbd_dev, uint8_t ep);


void usb_gpio_setup(void);

usbd_device* usb_initialize(void);

void sendReady(void);

void resetDevice(void);

void replyToHost(usbd_device* _usb_dev, char* _reply_buffer, uint16_t _len);

void decodeBuffer(const char* _buff, uint16_t* out);


#endif