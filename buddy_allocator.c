#include <stdio.h>
#include <assert.h>
#include <math.h> // for floor and log2
#include "buddy_allocator.h"

// these are trivial helpers to support you in case you want
// to do a bitmap implementation
int levelIdx(size_t idx){
  return (int)floor(log2(idx));
};

int buddyIdx(int idx){
  if (idx&0x1){
    return idx-1;
  }
  return idx+1;
}

int parentIdx(int idx){
  return idx/2;
}

int startIdx(int idx){
  return (idx-(1<<levelIdx(idx)));
}


// computes the size in bytes for the allocator
/* int BuddyAllocator_calcSize(int num_levels) {
  int list_items=1<<(num_levels+1); // maximum number of allocations, used to determine the max list items
  int list_alloc_size=(sizeof(BuddyListItem)+sizeof(int))*list_items;
  return list_alloc_size;
} */


void BuddyAllocator_init(BuddyAllocator* alloc,
                        BitMap* bitmap,
                        BitMap* check_bitmap,
                        int num_levels,
                        char* memory,
                        int memory_size,
                        int min_bucket_size){

  // we need room also for level 0
  alloc->bitmap=bitmap;
  alloc->check_bitmap=check_bitmap;
  alloc->num_levels=num_levels;
  alloc->memory=memory;
  alloc->memory_size=memory_size;
  alloc->min_bucket_size=min_bucket_size;
  // we need enough memory to handle internal structures
  printf("BUDDY INITIALIZING\n");
  printf("\tlevels: %d",  alloc->num_levels);
  printf("\tbucket size:%d\n", min_bucket_size);
};
int BuddyAllocator_getBuddy(BuddyAllocator* alloc, int level){
 
  if (level<0)
    return 0;
  assert(level <= alloc->num_levels);
  int idx=1<<(level-1);
  while(BitMap_bit(alloc->bitmap,idx)||BitMap_bit(alloc->check_bitmap,idx)){
    if(++idx==1<<(level+1))return 0;
  }
  
  BitMap_setBit(alloc->bitmap,idx,1);
  
  alloc_upwardSet(alloc,idx);
  alloc_downwardSet(alloc,idx);
  return idx;
}
void alloc_upwardSet(BuddyAllocator* alloc,int idx){
  int parent=parentIdx(idx);
  if(!BitMap_bit(alloc->check_bitmap,idx)){//risalgo l'albero fino alla root o fino a che non trovo un 1
    BitMap_setBit(alloc->check_bitmap,idx,1);
    if(parent+idx!=0)alloc_upwardSet(alloc,parent);
    else return;
  }
  else return;
}
void alloc_downwardSet(BuddyAllocator* alloc,int idx){
  if(idx>=1<<alloc->num_levels)return;
  //assert(!BitMap_bit(alloc->check_bitmap,idx));
  BitMap_setBit(alloc->check_bitmap,idx,1);
  alloc_downwardSet(alloc,idx*2);
  alloc_downwardSet(alloc,idx*2+1);

}

void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int idx){
  //assert(idx <= alloc->1<<num_levels);
  
  BitMap_setBit(alloc->bitmap,idx,0);
  free_upwardSet(alloc,idx);
  free_downwardSet(alloc,idx);
  return idx;
}
void free_upwardSet(BuddyAllocator* alloc,int idx){
  int parent=parentIdx(idx);
  //assert(BitMap_bit(alloc->check_bitmap,idx));//risalgo l'albero fino alla root o fino a che non trovo un 1
  BitMap_setBit(alloc->check_bitmap,idx,0);
  if(parent+idx!=0 && (BitMap_bit(alloc->bitmap,buddyIdx(idx))|| BitMap_bit(alloc->check_bitmap,buddyIdx(idx))))free_upwardSet(alloc,parent);
  else return;
}
void free_downwardSet(BuddyAllocator* alloc,int idx){
  if(idx>=1<<alloc->num_levels)return;
 //assert(BitMap_bit(alloc->check_bitmap,idx));
  BitMap_setBit(alloc->check_bitmap,idx,1);
  free_downwardSet(alloc,idx*2);
  free_downwardSet(alloc,idx*2+1);
}


//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  // we determine the level of the page
  int mem_size=(1<<alloc->num_levels)*alloc->min_bucket_size;
  int  level=floor(log2(mem_size/(size)));

  // if the level is too small, we pad it to max
  if (level>alloc->num_levels)
    level=alloc->num_levels;
  printf("requested: %d bytes, level %d \n",
         size, level);

  // we get a buddy of that size;

  int buddy=BuddyAllocator_getBuddy(alloc, level);
  if (!buddy)
    return;

  // we write in the memory region managed the buddy address
  return  alloc->memory+buddy; 
}
//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem) {
  printf("freeing %p", mem);
  // we retrieve the buddy from the system
  char* p=(char*) mem;
  int offset=p-alloc->memory;
  int idx;
  for(int level_start=1;!BitMap_bit(alloc->bitmap,level_start+offset);level_start=level_start<<1){
    idx=level_start+offset;
  }
  //printf("level %d", buddy->level);
  // sanity check;

  BuddyAllocator_releaseBuddy(alloc, idx);
  
}
