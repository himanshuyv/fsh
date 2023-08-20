#ifndef __QUEUE_H
#define __QUEUE_H

#include "command.h"

#define T Command*

typedef struct NodeStruct {
    T val;
    NodeStruct* next;
} NodeStruct;

typedef NodeStruct* Node;

typedef struct QueueStruct {
    size_t size;
    Node front;
    Node rear;
} QueueStruct;

typedef QueueStruct* Queue;

void push(Queue q, T val);
T pop(Queue q);
T front(Queue q);

#endif