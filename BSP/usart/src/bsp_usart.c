/******************************************************************************
 * Copyright (C) 2026 luo. All Rights Reserved.
 *
 * @file    bsp_usart.c
 *
 * @par dependencies
 * -bsp_usart.h
 *
 * @author  luo | dainchuang
 *
 * @brief   Provide the HAL APIs of adc and corresponding operations.
 *
 * Processing flow:
 *
 * @version V1.0 2026.9.19
 *
 * @note    
 *****************************************************************************/
 
#include "bsp_usart.h"

//******************************** Task Handle ******************************//
osThreadId_t usart_taskHandle;

const osThreadAttr_t usart_task_attributes     = {
  .name = "usart_task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
//******************************** Task Handle ******************************//


//******************************** 变量 *************************************//
circular_buffer_t *circular_buffer       = NULL;
uint8_t            rec             = 0;
uint8_t            dma_buf[32]     = {0};
uint8_t            dma_idx         = 0;
//******************************** 变量 *************************************//


//******************************** Queue Handles ****************************//

//******************************** Queue Handles ****************************//
 

//******************************** Defines **********************************//
/**
 * @brief 生产者任务：启动ADC采样，用两块缓冲区交替把数据交给日志任务
 *
 * Processing flow:
 * 
 *
 * Steps:
 * 
 *
 * @param[in]     void *argument
 *
 * @return void
 */
void usart_task_func(void *argument)
{
  /***************** 变量 **************/
  uint32_t my_task = 0;
  /***************** 变量 **************/


  /***************** 应用 **************/
  elog_i("bsp_usart","usart task running");

  circular_buffer = create_empty_circular_buffer();
  if(NULL == circular_buffer)
  {
    elog_e("bsp_usart","creat buffer error");
  }

  //HAL_UART_Receive_IT(&huart1, &rec, 1);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dma_buf, 32);
  for(;;)
 	{
    xTaskNotifyWait(0, 0x01, &my_task, portMAX_DELAY);

    log_i("bsp usart get notify");
    xTaskNotify(app_usart_taskHandle, 0, eIncrement);
  }
  /***************** 应用 **************/
}

/**
 * @brief 中断接收到数据之后存入缓冲区并通知前端任务

 * -Step
 * -1.先判断传入结构体指针是否空
 * -2.判断缓冲区是否已满
 * -3.在头指针位置写入数据
 * -4.读指针加一
 *
 * @param[in]     circular_buffer_t* p_buffer：传入环形缓冲区对应结构体指针
 *                data_type_t data：传入的数据
 *
 * @return        uint8_t: 0xFF:错误，结构体指针是空的
 *                         0xFE:缓冲区已满
 *                         0x00:写入成功
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  BaseType_t xHigherPrioritytaskWoken = pdFALSE;
  if(NULL == circular_buffer)
  {
    log_e("error NULL pointer of circular_buffer");
    return;
  }

  insert_data(circular_buffer, rec);

  xTaskNotifyFromISR(usart_taskHandle, 0x01, eSetBits, 
                         &xHigherPrioritytaskWoken);
  HAL_UART_Receive_IT(&huart1, &rec, 1);
  portYIELD_FROM_ISR(xHigherPrioritytaskWoken);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  BaseType_t xHigherPrioritytaskWoken = pdFALSE;
  if(dma_idx != sizeof(dma_buf))
  {
    for(uint8_t i = dma_idx; i < Size; i++)
    {
      //log_i("insert idx: %d", i);
      insert_data(circular_buffer, dma_buf[i]);
    }
    dma_idx = Size;
  }
  else
  {
    for(uint8_t i = 0; i < Size; i++)
    {
      //log_i("insert idx: %d", i);
      insert_data(circular_buffer, dma_buf[i]);
    }
    dma_idx = Size;
  }

  xTaskNotifyFromISR(usart_taskHandle, 0x01, eSetBits, 
                         &xHigherPrioritytaskWoken);
  portYIELD_FROM_ISR(xHigherPrioritytaskWoken);
}
//******************************** Defines **********************************//


