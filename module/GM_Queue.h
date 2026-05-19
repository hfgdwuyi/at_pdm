/**
* Copyright (C) Siemens Healthcare GmbH [2017-2020], All Rights Reserved
*
* Project: General function library
*
* @file GM_queue.h
* @brief Provides a data structure template for queues
* @details
*
*/

#ifndef __GM_QUEUE_H__
#define __GM_QUEUE_H__

#include "data_type.h"

#define DB_FIFO_NONE    0
#define DB_FIFO_1       1
#define DB_FIFO_2       2

#pragma pack(1)

typedef struct _QUEUE_t
{
    U32_t total_size;
    U32_t used_size;
    U32_t free_size;
    U32_t head;
    U32_t tail;
    MUTEX_t mutex;
    U8_t *data;
}QUEUE_t;

typedef struct _DB_QUEUE_t
{
    QUEUE_t fifo1;
    QUEUE_t fifo2;
    U8_t cur_rd_fifo;
    U8_t latest_wr_fifo;
}DB_QUEUE_t;

#pragma pack()

BOOL_t GM_initialize_queue(QUEUE_t* queue, U32_t size, U8_t *addr);
BOOL_t GM_check_queue_status(QUEUE_t* queue);
BOOL_t GM_check_queue_space_full(QUEUE_t* queue, U32_t *used_size);
BOOL_t GM_check_queue_space_null(QUEUE_t* queue, U32_t *free_size);
BOOL_t GM_read_data_from_queue(QUEUE_t* queue, void *rd_data, U32_t size);
BOOL_t GM_write_data_to_queue(QUEUE_t* queue, void *wr_data, U32_t size);

BOOL_t GM_initialize_double_buffer_queue(DB_QUEUE_t* queue, U32_t size, U8_t *fifo1, U8_t *fifo2);
BOOL_t GM_read_data_from_double_buffer_queue(DB_QUEUE_t* queue, void *rd_data, U32_t *size);
BOOL_t GM_write_data_to_double_buffer_queue(DB_QUEUE_t* queue, void *wr_data, U32_t size);

void GM_get_double_buffer_queue_status(DB_QUEUE_t* queue, U32_t *rest_1, U32_t *rest_2, U8_t *cur, U8_t *latest);

#endif /**< __GM_QUEUE_H__ */
/*--------------------------------End of file---------------------------------*/
