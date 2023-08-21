#ifndef __QUEUE_H
#define __QUEUE_H

#include "command.h"

#define T char*

typedef struct NodeStruct {
    T val;
    struct NodeStruct* next;
} NodeStruct;

typedef struct NodeStruct* Node;

typedef struct QueueStruct {
    size_t size;
    Node front;
    Node rear;
} QueueStruct;

typedef QueueStruct* Queue;

Queue newQueue();
void push(Queue q, T val);
T pop(Queue q);
T front(Queue q);

#endif