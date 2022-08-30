#include "buddy_allocator.h"
#include <stdio.h>

#define BUDDY_LEVELS 16
#define MEMORY_SIZE (1024*1024)
#define MIN_BUCKET_SIZE (MEMORY_SIZE>>(BUDDY_LEVELS))
#define BITMAP_SIZE 8192

char memory[MEMORY_SIZE];
uint8_t buffer1[BITMAP_SIZE];
uint8_t buffer2[BITMAP_SIZE];
BuddyAllocator alloc;
BitMap bitmap;
BitMap check_bitmap;
int main(int argc, char** argv) {
  printf("init... ");
   //1 we see if we have enough memory for the buffers
  
 BitMap_init(&bitmap,BITMAP_SIZE*8,buffer1);
  BitMap_init(&check_bitmap,BITMAP_SIZE*8,buffer2);
  //2 we initialize the allocator
  printf("init... ");
  BuddyAllocator_init(&alloc,
                      &bitmap,
                      &check_bitmap,
                      BUDDY_LEVELS,
                      memory,
                      MEMORY_SIZE,
                      MIN_BUCKET_SIZE);
  printf("DONE\n");

 void* p1=BuddyAllocator_malloc(&alloc, 100);
  void* p2=BuddyAllocator_malloc(&alloc, 100);
  void* p3=BuddyAllocator_malloc(&alloc, 100000);
  BuddyAllocator_free(&alloc, p1);
  BuddyAllocator_free(&alloc, p2);
  BuddyAllocator_free(&alloc, p3); 
  
}
