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
  int idx=1<<(level);
  //printf("idx: %d \n", idx);
  while(idx!=1<<(level+1)&&(BitMap_bit(alloc->bitmap,idx)||BitMap_bit(alloc->check_bitmap,idx))){
    if(++idx==1<<(level+1))return 0;
  }

  BitMap_setBit(alloc->bitmap,idx,1);
  alloc_upwardSet(alloc,parentIdx(idx));
  alloc_downwardSet(alloc,idx);
  return idx;
}
void alloc_upwardSet(BuddyAllocator* alloc,int idx){
  int parent=parentIdx(idx);
  if(!BitMap_bit(alloc->check_bitmap,idx)){//risalgo fino a che non trovo un 1
    BitMap_setBit(alloc->check_bitmap,idx,1); //occupo il nodo
    if(idx!=1)//se non mi trovo nel nodo radice
      alloc_upwardSet(alloc,parent);
    else return;
  }
  else return;
}
void alloc_downwardSet(BuddyAllocator* alloc,int idx){
 // printf("idx: %d, max_idx: %d \n",idx,(1<<alloc->num_levels)-1);
  if(idx>=(1<<alloc->num_levels))return;
  BitMap_setBit(alloc->check_bitmap,idx,1);
  alloc_downwardSet(alloc,idx*2);
  alloc_downwardSet(alloc,(idx*2)+1);

}

void BuddyAllocator_releaseBuddy(BuddyAllocator* alloc, int idx){
  //assert(idx <= alloc->1<<num_levels);
  
  BitMap_setBit(alloc->bitmap,idx,0);
  free_upwardSet(alloc,idx);
  free_downwardSet(alloc,idx);
}
void free_upwardSet(BuddyAllocator* alloc,int idx){
  int parent=parentIdx(idx);
  //assert(BitMap_bit(alloc->check_bitmap,idx));//risalgo l'albero fino alla root o fino a che non trovo un 1
  BitMap_setBit(alloc->check_bitmap,idx,0);
  if(idx!=1 && (!BitMap_bit(alloc->bitmap,buddyIdx(idx))&& !BitMap_bit(alloc->check_bitmap,buddyIdx(idx))))free_upwardSet(alloc,parent);
  else return;
}
void free_downwardSet(BuddyAllocator* alloc,int idx){
  if(idx>=1<<alloc->num_levels)return;
 //assert(BitMap_bit(alloc->check_bitmap,idx));
  BitMap_setBit(alloc->check_bitmap,idx,0);
  free_downwardSet(alloc,idx*2);
  free_downwardSet(alloc,idx*2+1);
}


//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size) {
  // we determine the level of the page
  int mem_size=alloc->memory_size;
  int  level=floor(log2(mem_size/(size)));

  // if the level is too small, we pad it to max
  if (level>=alloc->num_levels)
    level=alloc->num_levels-1;
  printf("memory: %d bytes\n",
          mem_size);
  printf("requested: %d bytes, level %d \n",
         size, level);

  // we get a buddy of that size;

  int buddy=BuddyAllocator_getBuddy(alloc, level);
  if (buddy==0){
    printf("Out of Memory\n");
    return NULL;
  }
  int memory_offset=(buddy-(1<<(level)))*alloc->memory_size/(1<<(level));
  printf("buddy: %d, level: %d, memory_offset: %d \n", buddy, level, memory_offset);
  return  alloc->memory+memory_offset; 
}
//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem) {
  
  printf("freeing %p\n", mem);
  char* p=(char*) mem;
  int offset=p-alloc->memory;//calcolo l'offset tra il primo indirizzo del buffer, e l'indirizzo di memoria da liberare
  printf("offset: %d \n", offset);
  if(offset>=alloc->memory_size || offset<0){
    printf("Invalid free\n");
    return;
  }
  int unit=alloc->memory_size;
  int start_level=0;
  while(offset%unit){
    unit=unit>>1;
    start_level++;
  }
  int levelstart_idx=1<<(start_level);
  //printf("first index of the level: %d \n", levelstart_idx);
  int start_index=levelstart_idx+(offset/unit);
  //printf("start_idx: %d \n", start_index);
  while(1){
    if(BitMap_bit(alloc->bitmap,start_index)!=0)break;
    start_index=start_index<<1;
    start_level++;
    if(start_index>=alloc->bitmap->num_bits){
      printf("No previous allocation or Double Free\n");
      return;
    }
  }

  printf("freeing buddy: %d\n", start_index);
  BuddyAllocator_releaseBuddy(alloc, start_index);
}

void BuddyAllocator_print(BuddyAllocator* alloc){
  int position=1;
  printf("PRINT:\n");
  for (int level=0; level < alloc->num_levels; level++){
    for(int i=0; i<(1<<(alloc->num_levels-1));i++)printf("------------");
    printf("\n");
    printf("level %d:    ",level);
    for (position; position< 1<<(level+1) ; position++){
      if(position%2&&position!=1)printf("\\");
      else printf("|");
      printf("%d:(%d, %d) ",position,BitMap_bit(alloc->bitmap,position), BitMap_bit(alloc->check_bitmap,position));
      int num=1<<((alloc->num_levels)-level-1);
      num--;
      for(int i=0; i<num;i++)printf("           ");
    }
    printf("\n");
  }
  for(int i=0; i<(1<<(alloc->num_levels-1));i++)printf("------------");
  printf("\n");
  return;
}
