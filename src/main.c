//standard library
#include <stdio.h>
#include <stdint.h>
//freertos modules
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "hooks.h"

//libopencm3  modulesk
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/syscfg.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
//custom modules
#include "usbd.h"

#define auto_reload_timer_period  pdMS_TO_TICKS(1000)

usbd_device*  usbd_dev;
// BUFFER
static uint16_t state_buffer[4];

void vApplicationStackOverflowHook(xTaskHandle *pxTask __attribute((unused)),
                                    signed portCHAR *pcTaskName __attribute((unused)))
{
    while(1)
    {
        
    }
}



void usb_poll(void* pvParameters);

// this callback function will be called at specific interval by FREERTOS Daemon Task
static void prvAutoReloadTimerCallback(TimerHandle_t xTimer)
{
    char* replyMessage =  "Done \r\n";
    replyToHost(usbd_dev, replyMessage, strlen(replyMessage));
    gpio_toggle(GPIOA, GPIO5);
}
    


static void setupHardware(void);


int main(void)
{
  

    setupHardware();


    //FREERTOS
    TimerHandle_t xAutoReloadTimer;
    TaskHandle_t xUSBPollTask;
    BaseType_t xTimerStarted = pdFALSE;
    BaseType_t xTaskCreated = pdFALSE;


    xTaskCreated = xTaskCreate(usb_poll,
                "usb_poll",
                1024,
                (void*) usbd_dev,
                9,
                &xUSBPollTask);

    
    xAutoReloadTimer = xTimerCreate("autoreload",
                                auto_reload_timer_period,
                                pdTRUE,
                                0,
                                prvAutoReloadTimerCallback);

    if(xAutoReloadTimer != NULL) 
    { 
        xTimerStarted = xTimerStart(xAutoReloadTimer, pdMS_TO_TICKS(0));
        if(xTaskCreated==pdPASS && xTimerStarted == pdPASS)
        {
            vTaskStartScheduler();
        }
    }
    while(true)
    {
        
    }
    return 0;
}



static void setupHardware(void)
{
    //set the clock
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_84MHZ]);
    rcc_periph_clock_enable(RCC_GPIOA);
    //rcc_periph_clock_enable(RCC_OTGFS);
    RCC_CSR |= RCC_CSR_LSION;

    //set led2 gpio 
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);

    //usb setup 
    usb_gpio_setup();
    //initizalize usb
    usbd_dev = usb_initialize();
    usbd_register_set_config_callback(usbd_dev, cdcacm_set_config);    

    //gpio_set(GPIOA, GPIO5);
}


// a function for a task
void usb_poll(void* pvParameters)
{
    while(1)
    {
        usbd_poll((usbd_device*)pvParameters);        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}