#include <assert.h>
#include "bit_map.h"
#include <stdio.h>
// returns the number of bytes to store bits booleans
int BitMap_getBytes(int bits){
  return bits/8 ;
}

// initializes a bitmap on an external array
void BitMap_init(BitMap* bit_map, int num_bits, uint8_t* buffer){
  bit_map->buffer= buffer;
  bit_map->num_bits=num_bits;
  //printf("%d\n",num_bits);
  bit_map->buffer_size=BitMap_getBytes(num_bits);
  //printf("%d\n",bit_map->buffer_size);
}

// sets a the bit bit_num in the bitmap
// status= 0 or 1
void BitMap_setBit(BitMap* bit_map, int bit_num, int status){
  // get byte
  int byte_num=bit_num>>3;
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=bit_num-(byte_num<<3);
  //printf("byte_num: %d, bit_num: %d, bit_in_byte: %d\nadding: %d \n",byte_num,bit_num,bit_in_byte, (1<<bit_in_byte)); 
  if (status) {
    
    bit_map->buffer[byte_num] |= (1<<bit_in_byte);
    
  } else {
    bit_map->buffer[byte_num] &= ~(1<<bit_in_byte);
  }
  /* for (int position=byte_num*8; position< byte_num*8+8; position++){
      printf("%d: %d) ",position, BitMap_bit(bit_map,position));
    }
  printf("\n"); */
} 

// inspects the status of the bit bit_num
int BitMap_bit(const BitMap* bit_map, int bit_num){
  int byte_num=bit_num>>3; 
  assert(byte_num<bit_map->buffer_size);
  int bit_in_byte=bit_num-(byte_num<<3);
  //printf("byte_num: %d, bit_num: %d, bit_in_byte: %d\nadding: %d \n",byte_num,bit_num,bit_in_byte, (1<<bit_in_byte)); 
  return (bit_map->buffer[byte_num] & (1<<bit_in_byte))!=0;
}
