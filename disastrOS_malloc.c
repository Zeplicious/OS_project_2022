#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "Buddy_allocator/buddy_allocator.h"
void internal_malloc(){
    BuddyAllocator allocator;
    BitMap bitmap;
    BitMap check_bitmap;
    BitMap_init(&bitmap,BITMAP_SIZE*8,running->buffer);
    BitMap_init(&check_bitmap,BITMAP_SIZE*8,running->check_buffer);
    BuddyAllocator_init(&allocator,
                        &bitmap,
                        &check_bitmap,
                        BUDDY_LEVELS,
                        running->memory,
                        MEMORY_SIZE,
                        MIN_BUCKET_SIZE);
    void* ret=BuddyAllocator_malloc(&allocator,running->syscall_args[0]);
    BuddyAllocator_print(&allocator);
    running->syscall_retvalue=(long int)ret;
}
  