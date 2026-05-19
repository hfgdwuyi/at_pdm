#include <stdio.h>
#include "GM_Queue.h"

BOOL_t GM_initialize_queue(QUEUE_t* queue, U32_t size, U8_t *addr)
{
    if(queue==NULL || addr == NULL)
    {
        return BOOL_FALSE;
    }
    else if(size == 0)
    {
        return BOOL_FALSE;
    }
    
    queue->total_size = size;
    queue->used_size = 0;
    queue->free_size = size;
    queue->head = 0;
    queue->tail = 0;
    queue->mutex = MUX_UNLOCK;
    queue->data = addr;

    return BOOL_TRUE;
}

BOOL_t GM_check_queue_status(QUEUE_t* queue)
{
    if(queue == NULL)
    {
        return BOOL_FALSE;
    }

    if (queue->data == NULL)
    {
        return BOOL_FALSE;
    }

    if (queue->mutex == MUX_LOCK)
    {
        return BOOL_FALSE;
    }

    return BOOL_TRUE;
}

//CHECK FULL
BOOL_t GM_check_queue_space_full(QUEUE_t* queue, U32_t *used_size)
{
    if(queue==NULL || used_size==NULL)
    {
        return BOOL_FALSE;
    }

    *used_size = queue->used_size;

    if(queue->total_size == queue->used_size)
    {
        return BOOL_FALSE;
    }

    return BOOL_TRUE;
}

//CHECK NULL
BOOL_t GM_check_queue_space_null(QUEUE_t* queue, U32_t *free_size)
{
    if(queue==NULL || free_size==NULL)
    {
        return BOOL_FALSE;
    }

    *free_size = queue->free_size;

    if(queue->total_size == queue->free_size)
    {
        return BOOL_FALSE;
    }

    return BOOL_TRUE;
}

//READ DATA
BOOL_t GM_read_data_from_queue(QUEUE_t* queue, void *rd_data, U32_t size)
{
    U8_t *data = NULL;
    U32_t i = 0;

    if(queue==NULL || rd_data==NULL)
    {
        return BOOL_FALSE;
    }

    if(size > queue->used_size)
    {
        return BOOL_FALSE;
    }

    data = (U8_t*)rd_data;

    for(i=0;i<size;i++)
    {
        *(data+i) = *(queue->data+queue->head);
        queue->head = (queue->head+1)%queue->total_size;
        queue->used_size--;
        queue->free_size++;
    }

    return BOOL_TRUE;
}

//WRITE DATA
BOOL_t GM_write_data_to_queue(QUEUE_t* queue, void *wr_data, U32_t size)
{
    U8_t *data = NULL;
    U32_t i = 0;

    if(queue==NULL || wr_data==NULL)
    {
        return BOOL_FALSE;
    }

    if(size > queue->free_size)
    {
        return BOOL_FALSE;
    }

    data = (U8_t*)wr_data;

    for(i=0;i<size;i++)
    {
        *(queue->data+queue->tail) = *(data+i);
        queue->tail = (queue->tail+1)%queue->total_size;
        queue->used_size++;
        queue->free_size--;
    }
    
    return BOOL_TRUE;
}

void GM_get_double_buffer_queue_status(DB_QUEUE_t* queue, U32_t *rest_1, U32_t *rest_2, U8_t *cur, U8_t *latest)
{
    *rest_1 = queue->fifo1.free_size;
    *rest_2 = queue->fifo1.free_size;
    *cur = queue->cur_rd_fifo;
    *latest = queue->latest_wr_fifo;
}




BOOL_t GM_initialize_double_buffer_queue(DB_QUEUE_t* queue, U32_t size, U8_t *fifo1, U8_t *fifo2)
{
    if(queue==NULL || fifo1==NULL || fifo2==NULL)
    {
        return BOOL_FALSE;
    }
    else if(size == 0)
    {
        return BOOL_FALSE;
    }

    queue->latest_wr_fifo = DB_FIFO_1;
    queue->cur_rd_fifo = DB_FIFO_NONE;

    queue->fifo1.total_size = size;
    queue->fifo1.used_size = 0;
    queue->fifo1.free_size = size;
    queue->fifo1.head = 0;
    queue->fifo1.tail = 0;
    queue->fifo1.mutex = MUX_UNLOCK;
    queue->fifo1.data = fifo1;

    queue->fifo2.total_size = size;
    queue->fifo2.used_size = 0;
    queue->fifo2.free_size = size;
    queue->fifo2.head = 0;
    queue->fifo2.tail = 0;
    queue->fifo2.mutex = MUX_UNLOCK;
    queue->fifo2.data = fifo2;

    return BOOL_TRUE;
}

//READ DATA
BOOL_t GM_read_data_from_double_buffer_queue(DB_QUEUE_t* queue, void *rd_data, U32_t *size)
{
    U8_t *data = NULL;
    U32_t i = 0;
    U32_t len = 0;
    U8_t target_fifo = DB_FIFO_NONE;

    if(queue==NULL || rd_data==NULL)
    {
        return BOOL_FALSE;
    }

    if(queue->fifo1.used_size==0 && queue->fifo2.used_size==0)
    {
        return BOOL_FALSE;
    }

    data = (U8_t*)rd_data;

    //Get the target fifo that needs to be read.
    if(queue->latest_wr_fifo == DB_FIFO_1)
    {
        if (queue->fifo2.used_size == 0)
        {
            //old data does not exist read fifo [1]
            target_fifo = DB_FIFO_1;
        }
        else
        {
            //old data exists read fifo [2]
            target_fifo = DB_FIFO_2;
        }
    }
    else if(queue->latest_wr_fifo == DB_FIFO_2)
    {
        if (queue->fifo1.used_size == 0)
        {
            //old data does not exist read fifo [2]
            target_fifo = DB_FIFO_2;
        }
        else
        {
            //old data exists read fifo [1]
            target_fifo = DB_FIFO_1;
        }
    }

    //data operation
    if (target_fifo == DB_FIFO_1)
    {
        queue->cur_rd_fifo = DB_FIFO_1;

        if (*size > queue->fifo1.used_size)
        {
            len = queue->fifo1.used_size;
        }
        else
        {
            len = *size;
        }

        for(i=0;i<len;i++) 
        {
            *(data+i) = *(queue->fifo1.data+queue->fifo1.head);
            queue->fifo1.head = (queue->fifo1.head+1)%queue->fifo1.total_size;
            queue->fifo1.used_size--;
            queue->fifo1.free_size++;
        }

        *size = len;
        queue->cur_rd_fifo = DB_FIFO_NONE;
    }
    else if (target_fifo == DB_FIFO_2)
    {
        queue->cur_rd_fifo = DB_FIFO_2;

        if (*size > queue->fifo2.used_size)
        {
            len = queue->fifo2.used_size;
        }
        else
        {
            len = *size;
        }

        for(i=0;i<len;i++) 
        {
            *(data+i) = *(queue->fifo2.data+queue->fifo2.head);
            queue->fifo2.head = (queue->fifo2.head+1)%queue->fifo2.total_size;
            queue->fifo2.used_size--;
            queue->fifo2.free_size++;
        }

        *size = len;
        queue->cur_rd_fifo = DB_FIFO_NONE;
    }

    return BOOL_TRUE;
}

//WRITE DATA
BOOL_t GM_write_data_to_double_buffer_queue(DB_QUEUE_t* queue, void *wr_data, U32_t size)
{
    U8_t *data = NULL;
    U32_t i = 0;
    U8_t target_fifo = DB_FIFO_NONE;

    if(queue==NULL || wr_data==NULL)
    {
        return BOOL_FALSE;
    }

    if(queue->cur_rd_fifo == DB_FIFO_1)
    {//have mutex condition, write to fifo2
        target_fifo = DB_FIFO_2;
    }
    else if(queue->cur_rd_fifo == DB_FIFO_2)
    {//have mutex condition, write to fifo1
        target_fifo = DB_FIFO_1;
    }
    else
    {//no have mutex condition
        if(queue->latest_wr_fifo == DB_FIFO_1)
        {//write to fifo1
            target_fifo = DB_FIFO_1;
        }
        else if(queue->latest_wr_fifo == DB_FIFO_2)
        {//write to fifo2
            target_fifo = DB_FIFO_2;
        }
    }

    //printf("TARGET:%d\n", target_fifo);

    data = (U8_t*)wr_data;

    if (target_fifo == DB_FIFO_1)
    {
        if(size > queue->fifo1.free_size)
        {
            //printf("size=%d, fifo_free=%d\n",size, queue->fifo1.free_size);
            return BOOL_FALSE;
        }

        queue->latest_wr_fifo = DB_FIFO_1;

        for(i=0;i<size;i++)
        {
            *(queue->fifo1.data+queue->fifo1.tail) = *(data+i);
            queue->fifo1.tail = (queue->fifo1.tail+1)%queue->fifo1.total_size;
            queue->fifo1.used_size++;
            queue->fifo1.free_size--;
        }
    }
    else if (target_fifo == DB_FIFO_2)
    {
        if(size > queue->fifo2.free_size)
        {
            //printf("size=%d, fifo_free=%d\n",size, queue->fifo2.free_size);
            return BOOL_FALSE;
        }

        queue->latest_wr_fifo = DB_FIFO_2;

        for(i=0;i<size;i++)
        {
            *(queue->fifo2.data+queue->fifo2.tail) = *(data+i);
            queue->fifo2.tail = (queue->fifo2.tail+1)%queue->fifo2.total_size;
            queue->fifo2.used_size++;
            queue->fifo2.free_size--;
        }
    }

    return BOOL_TRUE;
}

/*--------------------------------End of file---------------------------------*/
