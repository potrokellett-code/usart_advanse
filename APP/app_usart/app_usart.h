#ifndef __APP_USART_H
#define __APP_USART_H

//******************************** Include **********************************//
#include "main.h"
#include "bsp_usart.h"
//******************************** Task Handles *****************************//
extern       osThreadId_t       app_usart_taskHandle;

extern const osThreadAttr_t     app_usart_task_attributes;
//******************************** Task Handles  ****************************//


/**
 * @brief 串口后端任务函数
 *
 * @param[in]     void *argument
 *
 * @return        void
 */
void         app_usart_task_func  (void *argument);

#endif
