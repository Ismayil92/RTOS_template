#include <libopencm3/stm32/gpio.h>
#include "usbd.h"




void usb_gpio_setup(void)
{
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11);
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO12);
	gpio_set_af(GPIOA, GPIO_AF10, GPIO11 | GPIO12);
}


usbd_device* usb_initialize(void)
{
	usbd_device* usbd_dev;
	usbd_dev = usbd_init(&otgfs_usb_driver, &dev, &config,
						 usb_strings, 3,
						 usbd_control_buffer, sizeof(usbd_control_buffer));

	//disable usb vbussense				 
	OTG_FS_GCCFG |= OTG_GCCFG_NOVBUSSENS;
	OTG_FS_GCCFG &= ~OTG_GCCFG_VBUSASEN;
	OTG_FS_GCCFG &= ~OTG_GCCFG_VBUSBSEN;

	return usbd_dev;
}


void cdcacm_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
	(void)wValue;
	usbd_ep_setup(usbd_dev, 0x01, USB_ENDPOINT_ATTR_BULK, MAX_PACKET_SIZE, cdcacm_data_rx_cb);
	usbd_ep_setup(usbd_dev, 0x82, USB_ENDPOINT_ATTR_BULK, MAX_PACKET_SIZE, NULL);
	usbd_ep_setup(usbd_dev, 0x83, USB_ENDPOINT_ATTR_INTERRUPT, 8, NULL);

	usbd_register_control_callback(usbd_dev, USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
								   USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT, cdcacm_control_request);

	initialized = true;
}

void cdcacm_data_rx_cb(usbd_device *usbd_dev, uint8_t ep)
{
	(void)ep;
	char buff[64];
	int len = usbd_ep_read_packet(usbd_dev, DEVICE_ADDRESS, (void*)buff, sizeof(buff));	
}



void replyToHost(usbd_device* _usb_dev, char* _reply_buffer, uint16_t _len)
{	
	usbd_ep_write_packet(_usb_dev, PC_ADDRESS, _reply_buffer, _len);
}


