#include "buddy_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#define BUDDY_LEVELS 5
#define MEMORY_SIZE (1024)
#define MIN_BUCKET_SIZE (MEMORY_SIZE>>(BUDDY_LEVELS-1))
#define BITMAP_SIZE (1<<(BUDDY_LEVELS-3))

char memory[MEMORY_SIZE];
uint8_t buffer1[BITMAP_SIZE];
uint8_t buffer2[BITMAP_SIZE];
BuddyAllocator alloc;
BitMap bitmap;
BitMap check_bitmap;
int main(int argc, char** argv) {
  printf("init... \n");
   //1 we see if we have enough memory for the buffers
  printf("BUDDY_LEVELS: %d, BITMAP_SIZE: %d, MIN_BUCKET_SIZE: %d \n",BUDDY_LEVELS,BITMAP_SIZE,MIN_BUCKET_SIZE );
  BitMap_init(&bitmap,BITMAP_SIZE*8,buffer1);
  BitMap_init(&check_bitmap,BITMAP_SIZE*8,buffer2);
  //2 we initialize the allocator
  BuddyAllocator_init(&alloc,
                      &bitmap,
                      &check_bitmap,
                      BUDDY_LEVELS,
                      memory,
                      MEMORY_SIZE,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");
  printf("************************************************************************************************************\n");


  
/*   BuddyAllocator_print(&alloc);
  void* p3=BuddyAllocator_malloc(&alloc, 400);
  BuddyAllocator_print(&alloc);
   printf("DONE\n");
  void* p2=BuddyAllocator_malloc(&alloc, 200);
  BuddyAllocator_print(&alloc);
  printf("DONE\n");
  void* p1=BuddyAllocator_malloc(&alloc, 2);
  BuddyAllocator_print(&alloc);
  printf("DONE\n");
  BuddyAllocator_free(&alloc, p3);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc, p2);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc, p1);
  BuddyAllocator_print(&alloc);
  printf("************************************************************************************************************\n");
  
  BuddyAllocator_print(&alloc);
  p1=BuddyAllocator_malloc(&alloc, 400);
  BuddyAllocator_print(&alloc);
   printf("DONE\n");
  p2=BuddyAllocator_malloc(&alloc, 200);
  BuddyAllocator_print(&alloc);
  printf("DONE\n");
  p3=BuddyAllocator_malloc(&alloc, 2);
  BuddyAllocator_print(&alloc);
  printf("DONE\n");
  BuddyAllocator_free(&alloc, p3);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc, p2);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc, p1);
  BuddyAllocator_print(&alloc);
  printf("************************************************************************************************************\n");
  
  BuddyAllocator_print(&alloc);
  p1=BuddyAllocator_malloc(&alloc, 2);
  BuddyAllocator_print(&alloc);
   printf("DONE\n");
  p2=BuddyAllocator_malloc(&alloc, 200);
  BuddyAllocator_print(&alloc);
  printf("DONE\n");
  p3=BuddyAllocator_malloc(&alloc, 400);
  BuddyAllocator_print(&alloc);
  printf("DONE\n");
  BuddyAllocator_free(&alloc, p3);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc, p2);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc, p1);
  BuddyAllocator_print(&alloc);
  printf("************************************************************************************************************\n");

  BuddyAllocator_print(&alloc);
  p1=printf("\")(&alloc, 2);
  BuddyAllocator_print(&alloc);
   printf("DONE\n");
  p2=BuddyAllocator_malloc(&alloc, 200);
  BuddyAllocator_print(&alloc);
  printf("DONE\n");
  p3=BuddyAllocator_malloc(&alloc, 400);
  BuddyAllocator_print(&alloc);
  printf("DONE\n");
  BuddyAllocator_free(&alloc, p1);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc, p2);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc, p3);
  BuddyAllocator_print(&alloc);
  printf("************************************************************************************************************\n"); */
  /* int* array=(int*) BuddyAllocator_malloc(&alloc,sizeof(int)*256);
   BuddyAllocator_print(&alloc);
   int* arroy=(int*) BuddyAllocator_malloc(&alloc,sizeof(int)*256); */

 /*  BuddyAllocator_print(&alloc);
  int* array=(int*) BuddyAllocator_malloc(&alloc,sizeof(int)*64);
  BuddyAllocator_print(&alloc);
  for(int i=0; i<64; i++)array[i]=i;
  printf("[");
  for(int i=0; i<64; i++)printf("%d, ",array[i]);
  printf("\b\b]\n");
  BuddyAllocator_free(&alloc,array);
  BuddyAllocator_print(&alloc);
  printf("%p\n", array);

  char* string=(char*) BuddyAllocator_malloc(&alloc,sizeof(char)*64);
  BuddyAllocator_print(&alloc);
  for(int i=0; i<64; i++)string[i]=0;
  sprintf(string,"unicorni belli\n");
  printf("%s\n", string);
  printf("[");
  for(int i=0; i<64; i++)printf("%d, ",array[i]);
  printf("\b\b]\n");

  short* cortini=(char*) BuddyAllocator_malloc(&alloc,sizeof(short)*64);
  BuddyAllocator_print(&alloc);
  for(int i=0; i<64; i++)cortini[i]=i;
  printf("[");
  for(int i=0; i<64; i++)printf("%d, ",array[i]);
  printf("\b\b]\n");

  TipoLista n1= (TipoLista) BuddyAllocator_malloc(&alloc,sizeof(TipoNodo));
  TipoLista n2= (TipoLista) BuddyAllocator_malloc(&alloc,sizeof(TipoNodo));
  TipoLista n3= (TipoLista) BuddyAllocator_malloc(&alloc,sizeof(TipoNodo));
  TipoLista n4= (TipoLista) BuddyAllocator_malloc(&alloc,sizeof(TipoNodo));
  n1->info=1;
  n2->info=7;
  n3->info=12;
  n4->info=1223;
  n4->next=NULL;
  n3->next=n4;
  n2->next=n3;
  n1->next=n2;
  printlist(n1);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc,n1);
  BuddyAllocator_free(&alloc,n2);
  BuddyAllocator_free(&alloc,n3);
  BuddyAllocator_free(&alloc,n4);
  BuddyAllocator_free(&alloc,cortini);
  BuddyAllocator_print(&alloc);
  BuddyAllocator_free(&alloc,string);
  BuddyAllocator_print(&alloc); */
}
