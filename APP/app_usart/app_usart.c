#include "app_usart.h"

//******************************** Task Handle ******************************//
osThreadId_t app_usart_taskHandle;

const osThreadAttr_t app_usart_task_attributes     = {
  .name = "app_usart_task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
//******************************** Task Handle ******************************//


/**
 * @brief 串口后端任务：对数据进行解包

 * -Step
 * -1.检测到包头，开始输出
 * -2.检测到包尾，停止输出
 *
 * @param[in]     void *argument
 *
 * @return        void
 */
 void app_usart_task_func(void *argument)
 {
  /***************** 变量 **************/
  uint8_t read_data = 0;
  uint8_t rx_state  = 0;
  uint16_t rx_idx   = 0;
  uint8_t  rx_buf[5]   = {0};
  uint16_t sum       = 0;
  /***************** 变量 **************/

  for(;;)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    do
    {
      if(0x00 == get_data(circular_buffer, &read_data))
      {
        //log_i("get data successful");
        //log_i("get data: %02x", read_data);
        switch(read_data)
        {
          case 0xFE:
          rx_state = 1;
          rx_idx = 0;
          sum    = 0;
					break;
          default:
          if(1 == rx_state)
          {
            if(0xFF == read_data) 
            {
              if(0 == rx_idx)
              {
                log_e("error data");
              }
              else
              {
                sum -= rx_buf[rx_idx - 1];
                if(rx_buf[rx_idx - 1] != (sum & 0xFF))
                {
                  log_e("data judge fail");
                }
                else
                {
                  elog_hexdump("buf data", 16, rx_buf, rx_idx - 1);
                }
              }

              rx_idx = 0;
              sum    = 0;
              rx_state = 0;
            }
            else
            {
              if(rx_idx < sizeof(rx_buf))
              {
                rx_buf[rx_idx++] = read_data;
                sum += read_data;
              }
              else
              {
                log_e("data oversize");
                rx_idx   = 0;
                sum      = 0;
                rx_state = 0;
              }
            }
          }
          break;
        }
      }
    }while(0x01 == buffer_is_empty(circular_buffer));
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, dma_buf, 32);
 
  }
 }

