#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>
#include <unistd.h>

int buffsize;
int clients;
int printers;

sem_t mutex;
sem_t full;
sem_t empty;

#define err_exit(no, msg) \
    errno = no; perror(msg); exit(EXIT_FAILURE);

//============== Buffer Declaration =================

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

//================== End Buffer Declaration ==============

typedef struct {
  CircularBuffer *cb;
  int tid;
} targs;

static void *producer(void *arg) {
    targs *threadArgs = arg;
    CircularBuffer *cb = threadArgs->cb;
    int cid = threadArgs->tid;
    int pages;
    srand (time(NULL));

    int count = 0;
    while (count < 5) {
	pages = rand() % 10 + 1;
	
	sem_wait(&empty);
	sem_wait(&mutex);
	int tail = cb->tail;
	enqueue(cb, pages);
	printf("Client %d has %d pages to print, putting request in Buffer[%d]\n",
		cid, pages, tail);
	sem_post(&mutex);
	sem_post(&full);

	count++;
	sleep(5); // sleep a certain time before making a request
    }
}

static void *consumer(void *arg) {
    targs *threadArgs = arg;
    CircularBuffer *cb = threadArgs->cb;
    int cid = threadArgs->tid;
    int ret;

    while (1) {
	sem_wait(&full);
	sem_wait(&mutex);
	int head = cb->head;
	int pages = dequeue(cb, &ret);
	printf("Printer %d printing %d pages from Buffer[%d]\n", cid, pages, head);
	sleep(pages);
	sem_post(&mutex);
	sem_post(&empty);
    }
}

