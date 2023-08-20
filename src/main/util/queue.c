#include "../../header/headers.h"

Node newNode(T val) {
    Node ret = (Node) malloc(sizeof(NodeStruct));
    ret->val = val;
    ret->next = NULL;
    return ret;
}

void link(Node a, Node b) {
    if (a) a->next = b;
}

void freeNodeList(Node n) {
    if (n == NULL) return;
    freeNodeList(n->next);
    free(n);
}

Queue newQueue() {
    Queue ret = (Queue) malloc(sizeof(QueueStruct));
    ret->front = NULL;
    ret->rear = NULL;
    ret->size = 0;
}

void push(Queue q, T val) {
    if (q == NULL) return;
    Node newElement = newNode(val);
    if (q->size == 0) {
        q->front = q->rear = newElement;
    } else {
        link(q->rear, newElement);
        q->rear = newElement;
    }

    q->size++;
}

T pop(Queue q) {
    if (q == NULL) return NULL;
    if (q->size == 0) return NULL;
    if (q->size == 1) {
        free(q->front);
        q->front = q->rear = NULL;
    } else {
        Node oldFront = q->front;
        q->front = q->front->next;
        free(oldFront);
    }

    q->size--;
}

T front(Queue q) {
    if (q == NULL) return NULL;
    if (q->size == 0) return NULL;
    return q->front->val;
}

void freeQueue(Queue q) {
    freeNodeList(q->front);
}