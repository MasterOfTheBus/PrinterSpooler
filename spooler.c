#include <string.h>
#include <pthread.h>
#include "spooler.h"

int main(int argc, char *argv[])
{
  void *ret;
  int s;
  // get the option values
  if (argc != 4) {
    printf("usage: spooler -c[clients] -p[printers] -b[buffer size]\n");
    exit(1);
  }
  // for now, c is 1, p is 2, size is 3
  #if 0
  int i = 0;
  for (; i < argc; i++) {
    if (strstr(argv[i], 
  }
  #endif
  // remember error checking
  clients = atoi(argv[1]);
  printers = atoi(argv[2]);
  buffsize = atoi(argv[3]);

  sem_init(&mutex, 0, 1);
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, buffsize);

  // create the circular buffer
  CircularBuffer buff = newCircularBuffer();

  pthread_t client[clients];
  pthread_t printer[printers];

  // Create the clients
  int i = 0;
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

  //free(ret);
  free(buff.slots);

  exit(0);
}
