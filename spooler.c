#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "spooler.h"

void producer() {

}

void consumer() {

}

int main(int argc, char *argv[])
{
  int ret;
  // get the option values
  if (argc != 4) {
    printf("usage: spooler -c[clients] -p[printers] -b[buffer size]\n");
    exit(1);
  }
  // for now, c is 0, p is 1, size is 2
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

  // create the circular buffer
  //int slots[buffsize];
  CircularBuffer buff = newCircularBuffer();

#if 0
  pthread_t client[clients];
  pthread_t printer[printers];

  // Create the clients
  int i = 0;
  for (; i < clients; i++) {
    pthread_create(client[i], NULL, producer(), 
  }

  // Create the printers
  i = 0;
  for (; i < printers; i++) {

  }
  #endif
}
