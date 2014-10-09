#include <stdio.h>
#include <stdlib.h>

int buffsize;
int clients;
int printers;

typedef struct {
  int *slots; // the actual buffer
  int length;
  int head;
  int tail;
} CircularBuffer;

CircularBuffer newCircularBuffer() {
  CircularBuffer cb;
  cb.length = 0;
  cb.head = 1;
  cb.tail = 0;
  cb.slots = (int*)malloc(sizeof(int) * buffsize);
  return cb;
}

int enqueue(CircularBuffer *cb, int num) {
  if (cb->length >= buffsize) {
	return (-1);
  }
  if (cb->tail == buffsize-1) {
	cb->tail = 0;
  } else {
	cb->tail++;
  }
  cb->slots[cb->tail] = num;
  cb->length++;
  return (0);
}

int dequeue(CircularBuffer *cb, int *ret) {
  int dq = 0;
  if (cb->length <= 0) {
    *ret = -1;
	return (-1);
  }
  printf("length: %d\n", cb->length);
  dq = cb->head;
  cb->slots[cb->head] = -1;
  if (cb->head == buffsize-1) {
    cb->head = 0;
  } else {
    cb->head++;
  }
  cb->length--;
  *ret = 0;
  return (dq);
}

int head(CircularBuffer *cb, int *ret) {
  if (cb->length <= 0) {
    *ret = -1;
    return (-1);
  }
  return cb->slots[cb->head];
}
#if 0
typedef struct {
  CircularBuffer cb;
  int slots
} threadArgs;
#endif
