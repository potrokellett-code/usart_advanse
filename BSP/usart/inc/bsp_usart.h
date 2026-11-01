/******************************************************************************
 * Copyright (C) 2026 luo. All Rights Reserved.
 *
 * @file    bsp_led.h
 *
 * @par dependencies
 * -
 *
 * @author  luo | dainchuang
 *
 * @brief   Provide the HAL APIs of led and corresponding opetions.
 *
 * Processing flow:
 *
 * @version V1.0 2026.9.19
 *
 * @note    
 *****************************************************************************/
 
#ifndef __BSP_USART_H__
#define __BSP_USART_H__ 

//******************************** Include **********************************//
#include "gpio.h"
#include "main.h"

#include "cmsis_os.h"
#include "semphr.h"

#include "usart.h"
#include "app_usart.h"

#include "elog.h"
#include "mid_circular_buffer.h"
//******************************** Include **********************************//


//******************************** Task Handles *****************************//
extern       osThreadId_t       usart_taskHandle;

extern const osThreadAttr_t     usart_task_attributes;
//******************************** Task Handles  ****************************//


//******************************** Queue Handles ****************************//

//******************************** Queue Handles ****************************//

//******************************** Defines **********************************//
extern circular_buffer_t *circular_buffer;
extern uint8_t            dma_buf[32];

//******************************** Defines **********************************//


//******************************** Declaring ********************************//
/**
 * @brief ADC启动任务函数
 *
 * @param[in]     void *argument
 *
 * @return        void
 */
void         usart_task_func  (void *argument);

//******************************** Declaring ********************************//



#endif // End of __BSP_USART_H__ 
