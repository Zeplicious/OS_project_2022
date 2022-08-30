#pragma once
#include "bit_map.h"

#define LEVELS 16



typedef struct  {
  BitMap* bitmap;
  BitMap* check_bitmap;
  int num_levels;
  char* memory; // the memory area to be managed
  int memory_size;
  int min_bucket_size; // the minimum page of RAM that can be returned
} BuddyAllocator;


// computes the size in bytes for the buffer of the allocator
int BuddyAllocator_calcSize(int num_levels);


// initializes the buddy allocator, and checks that the buffer is large enough
void BuddyAllocator_init(BuddyAllocator* alloc,
                        BitMap* bitmap,
                        BitMap* check_bitmap,
                        int num_levels,
                        char* memory,
                        int memory_size,
                        int min_bucket_size);

// returns (allocates) a buddy at a given level.
// side effect on the internal structures
// 0 id no memory available
int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level);

// releases an allocated buddy, performing the necessary joins
// side effect on the internal structures
void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int idx);

//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);

void free_upwardSet(BuddyAllocator* alloc,int idx);
void alloc_upwardSet(BuddyAllocator* alloc,int idx);
void alloc_downwardSet(BuddyAllocator* alloc,int idx);
void free_downwardSet(BuddyAllocator* alloc,int idx);