#include <string.h>
#include <pthread.h>
#include "spooler.h"

int main(int argc, char *argv[])
{
  // get the option values
  if (argc != 4) {
    printf("usage: spooler -c[clients] -p[printers] -b[buffer size]\n");
    exit(1);
  }

  int i = 1;
  for (; i < argc; i++) {
    if (strstr(argv[i], "-c") != NULL) {
      if (isdigit(argv[i][2])) {
        clients = atoi(&argv[i][2]);
      } else {
        printf("Number of clients must be an integer value\n");
        exit(1);
      }
    } else if (strstr(argv[i], "-p") != NULL) {
      if (isdigit(argv[i][2])) {
        printers = atoi(&argv[i][2]);
      } else {
        printf("Number of printers must be an integer value\n");
        exit(1);
      }
    } else if (strstr(argv[i], "-b") != NULL) {
      if (isdigit(argv[i][2])) {
        buffsize = atoi(&argv[i][2]);
      } else {
        printf("Buffer size must be an integer value\n");
        exit(1);
      }
    } else {
      printf("usage: spooler -c[clients] -p[printers] -b[buffer size]\n");
      exit(1);
    }
  }

  if (clients <= 0) {
    printf("Number of clients must be greater than 0\n");
    exit(1);
  }
  if (printers <= 0) {
    printf("Number of printers must be greater than 0\n");
    exit(1);
  }
  if (buffsize <= 0) {
    printf("Buffer size must be greater than 0\n");
    exit(1);
  }

  void *ret;
  int s;

  sem_init(&mutex, 0, 1);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, buffsize);

  // create the circular buffer
  CircularBuffer buff = newCircularBuffer();

  pthread_t client[clients];
  pthread_t printer[printers];

  // Create the clients
  i = 0;
  for (; i < clients; i++) {
    targs thread_args;
    thread_args.cb = &buff;
    thread_args.tid = i;
    s = pthread_create(&client[i], NULL, producer, &thread_args);
    if (s != 0) {
      err_exit(s, "pthread_create");
    }
  }

  // Create the printers
  i = 0;
  for (; i < printers; i++) {
    targs thread_args;
    thread_args.cb = &buff;
    thread_args.tid = i;
    s = pthread_create(&printer[i], NULL, consumer, &thread_args);
    if (s != 0) {
      err_exit(s, "pthread_create");
    }
  }

  i = 0;
  for (; i < clients; i++) {
    s = pthread_join(client[i], &ret);
    if (s != 0) {
      err_exit(s, "pthread_join");
    }
    printf("Joined with client %d, returning\n", i);
  }

  i = 0;
  for (; i < printers; i++) {
    s = pthread_cancel(printer[i]);
    if (s != 0) {
      err_exit(s, "pthread_cancel");
    }
    s = pthread_join(printer[i], &ret);
    if (s != 0) {
      err_exit(s, "pthread_join");
    }
    printf("Joined with printer %d, returning\n", i);
  }

  free(buff.slots);

  exit(0);
}
