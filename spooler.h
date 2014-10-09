#include <stdio.h>

int buffsize;
int clients;
int printers;

typedef struct {
  int length;
  int head;
  int tail;
} CircularBuffer;

int enqueue(CircularBuffer *cb, int slots[], int num) {
  if (cb->length >= buffsize) {
	return (-1);
  }
  if (cb->tail == buffsize-1) {
	cb->tail = 0;
  } else {
	cb->tail++;
  }
  slots[cb->tail] = num;
  cb->length++;
  printf("head: %d, tail: %d\n", cb->head, cb->tail);
  return (0);
}

int dequeue(CircularBuffer *cb, int slots[], int *ret) {
  int dq = 0;
  if (cb->length <= 0) {
    *ret = -1;
	return (-1);
  }
  printf("length: %d\n", cb->length);
  dq = cb->head;
  slots[cb->head] = -1;
  if (cb->head == buffsize-1) {
    cb->head = 0;
  } else {
    cb->head++;
  }
  cb->length--;
  *ret = 0;
  printf("head: %d, tail: %d\n", cb->head, cb->tail);
  return (dq);
}

int head(CircularBuffer *cb, int slots[], int *ret) {
  if (cb->length <= 0) {
    *ret = -1;
    return (-1);
  }
  return slots[cb->head];
}
