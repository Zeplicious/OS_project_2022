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
void minFunction(void* args){
  printf("************************DIMOSTRAZIONE MALLOC DI DIMENSIONI PICCOLE****************\n");
  void* p4=disastrOS_malloc( 2400);
  void* p3=disastrOS_malloc( 400);
  void* p2=disastrOS_malloc( 200);
  void* p1=disastrOS_malloc( 2);
  void* p5=disastrOS_malloc( 2400);
  disastrOS_free(p4);
  disastrOS_free(p3);
  disastrOS_free(p2);
  disastrOS_free(p1);
  disastrOS_free(p5);
  disastrOS_exit(disastrOS_getpid()+1);

}
void allocFunction(void* args){
  printf("************************DIMOSTRAZIONE MALLOC E FREE*******************************\n");
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("Alloco un array di 252KB\n");
  int* array=(int*) disastrOS_malloc(sizeof(int)*63*1024);

  for(int i=0; i<64*1024; i++)array[i]=i;
/*   printf("[");
  for(int i=0; i<64*1024; i++)printf("%d, ",array[i]);
  printf("\b\b]\n"); */
  printf("Lo libero\n");
  disastrOS_free(array);
  printf("%p\n", array);
  printf("Alloco un stringa di 60KB\n");
  char* string=(char*) disastrOS_malloc(sizeof(char)*60*1024);
  for(int i=0; i<64*1024; i++)string[i]=0;
  sprintf(string,"un po di zucchero\n");
  printf("%s\n", string);
/*   printf("[");
  for(int i=0; i<64*1024; i++)printf("%d, ",array[i]);
  printf("\b\b]\n"); */

  printf("Alloco un array di 120KB\n");
  short* cortini=(short*) disastrOS_malloc(sizeof(short)*62*1024);

  for(int i=0; i<64*1024; i++)cortini[i]=i;
/*   printf("[");
  for(int i=0; i<64*1024; i++)printf("%d, ",array[i]);
  printf("\b\b]\n"); */
  disastrOS_free(string);
  disastrOS_free(cortini);
  disastrOS_exit(disastrOS_getpid()+1);
}
void oomFunction(void* args){
  printf("************************DIMOSTRAZIONE OUT OF MEMORY*******************************\n");
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  printf("Alloco un array di 1MB\n");
  int* array=(int*) disastrOS_malloc(sizeof(int)*256*1024);
  printf("array1: %p\n", array);
  printf("\nRichiedo un secondo array da 1MB\n");
  int* arroy=(int*) disastrOS_malloc(sizeof(int)*1024*1024);
  printf("array2: %p\n", arroy);
  printf("Free del primo array\n");
  disastrOS_free(array);
  disastrOS_exit(disastrOS_getpid()+1);
}

void doubleFreeFunction(void* args){
  printf("************************DIMOSTRAZIONE DOUBLE FREE*********************************\n");
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  int* array=(int*) disastrOS_malloc(sizeof(int)*256*1024);
  printf("prima free\n");
  disastrOS_free(array);
  printf("seconda free\n");
  disastrOS_free(array);
  disastrOS_exit(disastrOS_getpid()+1);
}
/* void childFunction(void* args){
  printf("Hello, I am the child function %d\n",disastrOS_getpid());
  int type=0;
  int mode=0; 
  int fd=disastrOS_openResource(disastrOS_getpid(),type,mode);
  printf("fd=%d\n", fd);
  printf("PID: %d, terminating\n", disastrOS_getpid()); 
  printf("PID: %d\n",disastrOS_getpid());

  int* array1=(int*) disastrOS_malloc(sizeof(int)*4096*disastrOS_getpid());
  int* array2=(int*) disastrOS_malloc(sizeof(int)*2048*disastrOS_getpid());

  for(int i=0; i<2048*disastrOS_getpid(); i++)array2[i]=i;


  int* array3=(int*) disastrOS_malloc(sizeof(int)*64*disastrOS_getpid());

  for(int i=0; i<64*disastrOS_getpid(); i++)array3[i]=64*disastrOS_getpid()-i;



  disastrOS_free(array3);
  disastrOS_free(array2);
  disastrOS_free(array1);

  disastrOS_exit(disastrOS_getpid()+1);
} */


void initFunction(void* args) {
  disastrOS_printStatus();
  printf("hello, I am init and I just started\n");
  //disastrOS_spawn(sleeperFunction, 0);
  
  int alive_children=0;
  
  disastrOS_spawn(allocFunction, 0);
  alive_children++;
  disastrOS_spawn(minFunction, 0);
  alive_children++;
  
  disastrOS_spawn(oomFunction, 0);
  alive_children++;
  
  disastrOS_spawn(doubleFreeFunction, 0);
  alive_children++;



  disastrOS_printStatus();
  int retval;
  int pid;
  while(alive_children>0 && (pid=disastrOS_wait(0, &retval))>=0){ 
    disastrOS_printStatus();
    printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
	   pid, retval, alive_children);
    --alive_children;
  }
  printf("shutdown!\n");
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
  //printf("the function pointer is: %p", childFunction);
  // spawn an init process
  printf("start\n");
  disastrOS_start(initFunction, 0, logfilename);
  return 0;
}
