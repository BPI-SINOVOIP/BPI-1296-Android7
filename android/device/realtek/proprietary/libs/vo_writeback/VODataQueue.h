#ifndef VOWB_DATA_QUEUE_H
#define VOWB_DATA_QUEUE_H

#include <stdint.h>
typedef struct {
    uint8_t*    buffer;
    uint32_t    size;
    uint32_t    itemSize;
    uint32_t    count;
    uint32_t    front;
    uint32_t    rear;
} DataQueue;

DataQueue* DataQueue_Create(
        uint32_t    itemCount,
        uint32_t    itemSize
);

void DataQueue_Destroy(
        DataQueue*  queue
);

int DataQueue_Enqueue(
        DataQueue*  queue,
        void*       data
);

void* DataQueue_Dequeue(
        DataQueue*  queue
);

void DataQueue_Flush(
        DataQueue*  queue
);

void* DataQueue_Peek(
        DataQueue*  queue
);

int DataQueue_Count(
        DataQueue*  queue
);
#endif
