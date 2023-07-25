//standard library
#include <stdio.h>
#include <stdint.h>
//freertos modules
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//libopencm3  modulesk
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/syscfg.h>
//custom modules
#include "usbd.h"



// BUFFER
static uint16_t state_buffer[4];




int main(void)
{

    usbd_device*  usbd_dev;

    //set the clock
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_OTGFS);
    RCC_CSR |= RCC_CSR_LSION;


    //set led2 gpio 
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

    //usb setup 
    usb_gpio_setup();
    //initizalize usb
    usbd_dev = usb_initialize();
    usbd_register_set_config_callback(usbd_dev, cdcacm_set_config);

    char* replyMessage =  "Done \r\n";

    gpio_set(GPIOA, GPIO5);
    while(true)
    {
        usbd_poll(usbd_dev);
        replyToHost(usbd_dev, replyMessage, strlen(replyMessage));
    }
    return 0;
}