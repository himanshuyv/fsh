#ifndef __QUEUE_H
#define __QUEUE_H

#include "command.h"

#define T char*

typedef struct NodeStruct {
    T val;
    struct NodeStruct* next;
    struct NodeStruct* prev;
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
void pop(Queue q);
T front(Queue q);
void freeQueue(Queue q);
#endif