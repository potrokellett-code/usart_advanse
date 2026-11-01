#include "mid_circular_buffer.h"

/**
 * @brief 创建环形缓冲区函数

 * -Step
 * -1.分配内存 
 * -2.对内存初始化，赋值0
 *
 * @param[in]     void：不需要传入参数
 *
 * @return        circular_buffer_t *: 传回结构体指针
 * 
 * @note 传回的结构体是空的，没有进行任何赋值
 */
circular_buffer_t * create_empty_circular_buffer(void)
{
  circular_buffer_t * p_buffer_temp = NULL;
  //1.分配内存
  p_buffer_temp = (circular_buffer_t *)malloc(sizeof(circular_buffer_t));
  if(NULL == p_buffer_temp)
  {
    log_e("error create_empty_circular_buffer");
    return NULL;
  }

  //2.对内存初始化，赋值0
  memset(p_buffer_temp, 0, sizeof(circular_buffer_t));

  return p_buffer_temp;
}

/**
 * @brief 判断环形缓冲区是否已空

 * -Step
 * -1.先判断传入结构体指针是否空
 * -2.判断缓冲区头和尾是否相等作为缓冲区已空的判据
 *
 * @param[in]     circular_buffer_t* p_buffer：传入环形缓冲区对应结构体指针
 *
 * @return        uint8_t: 0xff:错误，结构体指针是空的
 *                         0x00:缓冲区已空
 *                         0x01:缓冲区没空
 */
uint8_t buffer_is_empty(circular_buffer_t* p_buffer)
{
  //1.先判断传入结构体指针是否空 
  if(NULL == p_buffer)
  {
    return 0xFF;
  }

  //2.判断缓冲区头和尾是否想等作为缓冲区已空的判据
  if(p_buffer->head == p_buffer -> tail)
  {
    return 0x00;
  }
  else
  {
    return 0x01;
  }
}

/**
 * @brief 判断环形缓冲区是否已满

 * -Step
 * -1.先判断传入结构体指针是否空
 * -2.判断缓冲区头和尾是否想等作为缓冲区已空的判据
 *
 * @param[in]     circular_buffer_t* p_buffer：传入环形缓冲区对应结构体指针
 *
 * @return        uint8_t: 0xff:错误，结构体指针是空的
 *                         0x00:缓冲区已满
 *                         0x01:缓冲区没满
 */
uint8_t buffer_is_full(circular_buffer_t* p_buffer)
{
  if(NULL == p_buffer)
  {
    return 0xFF;
  }
  if((((p_buffer -> head) % CIRCULAR_BUFFER_SIZE) + 1) ==
      ((p_buffer -> tail) % CIRCULAR_BUFFER_SIZE))
  {
    return 0x00;
  }
  else
  {
    return 0x01;
  }
}

/**
 * @brief 插入一个数据

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
uint8_t insert_data(circular_buffer_t *p_buffer,data_type_t data)
{
  if(NULL == p_buffer)
  {
    return 0xFF;
  }
  if(0x00 == buffer_is_full(p_buffer))
  {
    return 0xFE;
  }

  p_buffer -> data[(p_buffer -> head) % CIRCULAR_BUFFER_SIZE] = data;

  p_buffer -> head++;

  return 0x00;
}

/**
 * @brief 读取一个数据

 * -Step
 * -1.先判断传入结构体指针是否空
 * -2.判断缓冲区是否已满
 * -3.在头指针位置写入数据
 * -4.读指针加一
 *
 * @param[in]     circular_buffer_t* p_buffer：传入环形缓冲区对应结构体指针
 *                data_type_t *data：传入存放读取数据的变量地址
 *
 * @return        uint8_t: 0xFF:错误，结构体指针是空的
 *                         0xFE:缓冲区已空
 *                         0x00:读取成功
 */
uint8_t get_data(circular_buffer_t *p_buffer,data_type_t *data)
{
  if(NULL == p_buffer)
  {
    return 0xFF;
  }
  if(0x00 == buffer_is_empty(p_buffer))
  {
    return 0xFE;
  }

  *data = p_buffer -> data[(p_buffer -> tail) % CIRCULAR_BUFFER_SIZE];
  p_buffer -> tail++;

  return 0;
}
