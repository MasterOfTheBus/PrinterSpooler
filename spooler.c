#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "spooler.h"

int main(int argc, char argv[])
{
  int ret;
#if 0
  // get the option values
  if (argc != 3) {
    
  }
  int i = 0;
  for (; i < argc; i++) {
    if (
  }
#endif
    buffsize = 5;  
  int slots[buffsize];
  CircularBuffer buff;
  buff.length = 0;
  buff.head = 1;
  buff.tail = 0;

  printf("length: %d\n", buff.length);
  enqueue(&buff, slots, 3);
  enqueue(&buff, slots, 7);
  enqueue(&buff, slots, 4);
  enqueue(&buff, slots, 9);
  enqueue(&buff, slots, 2);
  int i = 0;
#if 1
  for (; i < 5; i++) {
       printf("dequeuing\n");
       dequeue(&buff, slots, &ret);
  }
#endif
#if 1
  for (i=0; i <5; i++) {
    printf("buff[%d]: %d\n", i, slots[i]);
  }
  enqueue(&buff, slots, 16);
  for (i=0; i <5; i++) {
    printf("buff[%d]: %d\n", i, slots[i]);
  }
#endif
}
