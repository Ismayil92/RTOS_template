#ifndef HOOKS_H
#define HOOKS_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

extern void vApplicationTickHook(void)
{

}

extern void vTaskSwitchContect(void)
{

}

extern void vApplicationIdleHook(void)
{
    while(1)
    {

    }
}


extern void vApplicationMallocFailedHook(void)
{

}

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pTaskName);

#endif