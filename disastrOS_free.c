#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "Buddy_allocator/buddy_allocator.h"
void internal_free(){
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
    BuddyAllocator_free(&allocator,(void*)running->syscall_args[0]);
    BuddyAllocator_print(&allocator);
}