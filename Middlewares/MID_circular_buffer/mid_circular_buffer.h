#ifndef __MID_CIRCULAR_BUFFER__
#define __MID_CIRCULAR_BUFFER__

//******************************** Include **********************************//
#include <stdint.h>
#include "main.h"
#include "stdlib.h"
#include "string.h"
//******************************** Include **********************************//



//******************************** Defines **********************************//
#define CIRCULAR_BUFFER_SIZE 100

typedef uint8_t data_type_t;   

typedef struct {
    data_type_t data[CIRCULAR_BUFFER_SIZE];
    uint32_t    head;
    uint32_t    tail;
}circular_buffer_t;

//******************************** Defines **********************************//



//******************************** Declaring ********************************//
circular_buffer_t * create_empty_circular_buffer(void);

uint8_t buffer_is_empty(circular_buffer_t* p_buffer);

uint8_t buffer_is_full(circular_buffer_t* p_buffer);

uint8_t insert_data(circular_buffer_t* p_buffer, data_type_t data);

uint8_t get_data(circular_buffer_t* p_buffer, data_type_t *data);

#endif
