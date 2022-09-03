#include <stdio.h>
#include <unistd.h>
#include <poll.h>

#include "disastrOS.h"

// we need this to handle the sleep state
void sleeperFunction(void* args){
  printf("Hello, I am the sleeper, and I sleep %d\n",disastrOS_getpid());
  while(1) {
    getc(stdin);
    disastrOS_printStatus();
  }
}

void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  /* int type=0;
  int mode=0; 
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  printf("PID: %d, terminating\n", disastrOS_getpid()); */

  printf("PID: %d\n",disastrOS_getpid());

  int* array1=(int*) disastrOS_malloc(sizeof(int)*4096*disastrOS_getpid());
  int* array2=(int*) disastrOS_malloc(sizeof(int)*2048*disastrOS_getpid());

  for(int i=0; i<2048*disastrOS_getpid(); i++)array2[i]=i;

  /* printf("[");
  for(int i=0; i<128*disastrOS_getpid(); i++)printf("%d, ",array2[i]);
  printf("\b\b]\n"); */

  int* array3=(int*) disastrOS_malloc(sizeof(int)*64*disastrOS_getpid());

  for(int i=0; i<64*disastrOS_getpid(); i++)array3[i]=64*disastrOS_getpid()-i;

  /* printf("[");
  for(int i=0; i<64*disastrOS_getpid(); i++)printf("%d, ",array3[i]);
  printf("\b\b]\n"); */

  disastrOS_free(array3);
  disastrOS_free(array2);
  disastrOS_free(array1);

  disastrOS_exit(disastrOS_getpid()+1);
}


void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  disastrOS_spawn(sleeperFunction, 0);
  

  printf("I feel like to spawn 10 nice threads\n");
  int alive_children=0;
  for (int i=0; i<10; ++i) {
    disastrOS_spawn(childFunction, 0);
    alive_children++;
  }

  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }
  printf("shutdown!");
  disastrOS_shutdown();
}

int main(int argc, char** argv){
  char* logfilename=0;
  if (argc>1) {
    logfilename=argv[1];
  }
  // we create the init process processes
  // the first is in the running variable
  // the others are in the ready queue
  printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
