#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <stdint.h>

typedef struct RingBuf {
  int16_t length;
  int16_t start;
  int16_t end;
  int16_t count; 
  int32_t *buf;
} RingBuf;

int32_t get_RingBuf(RingBuf const * rb, int16_t i);
void add_RingBuf(RingBuf *rb, int32_t ele);
void init_RingBuf(RingBuf *rb, int32_t buf[], int16_t length);
void empty_RingBuf(RingBuf *rb);

#endif
