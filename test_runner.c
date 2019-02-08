
#include "filters.h"

DerivativeFilter df;

void start(int32_t window_buf[], int16_t window_size) {
  begin_df(&df, window_size, window_buf, 10, 10);
}

void push(int32_t value) {
  push_df(&df, value);
}

void read(void) {
  read_df(&df);
}

void reset(void) {
  reset_df(&df);
}

