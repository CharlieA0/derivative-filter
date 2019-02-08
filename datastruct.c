#include "datastruct.h"
#include <stdio.h>

#define PYLIB

void init_RingBuf(RingBuf *rb, int32_t buf[], int16_t const buffer_length) {
  rb->buf = buf;
  rb->length = buffer_length;
  rb->start = 0;
  rb->end = 0;
  rb->count = 0;
}

void empty_RingBuf(RingBuf *rb) {
  rb->start = 0;
  rb->end = 0;
  rb->count = 0;
}

int32_t get_RingBuf(RingBuf const * rb, int16_t i) {
  return rb->buf[(rb->start + i) % rb->length];
}

void add_RingBuf(RingBuf *rb, int32_t ele) {

  if (rb->count < rb->length) {
    rb->buf[rb->end] = ele;
    rb->end++;
    rb->count++;
  }
  else {
    rb->start = (rb->start + 1) % rb->length;
    int16_t new_end = (rb->end) % rb->length;
    rb->buf[new_end] = ele;
    rb->end = new_end + 1;
  }
}

