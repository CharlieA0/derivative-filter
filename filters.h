#ifndef SENSOR_H
#define SENSOR_H
#ifdef __cplusplus
extern "C" {
#endif 

#include <stdint.h>
#include <stdbool.h>
#include "datastruct.h"

// Derivation Filter Data
typedef struct DerivativeFilter {
  
  // Filter window and unit scaling factor
  RingBuf deriv_window;
  uint32_t time_step;
  int32_t fixed_point_scaling;

} DerivativeFilter;

// Public Interface
void begin_df(DerivativeFilter* self, int16_t window_size, int32_t window_buf[], uint32_t time_step, uint32_t fixed_point_scaling);
void push_df(DerivativeFilter* self, int32_t new_value);
int32_t read_df(DerivativeFilter const * self);
void reset_df(DerivativeFilter* self);

// IIR Low Pass (Exp. Moving Avg.) Data
typedef struct IIRLowPass {

  uint32_t decay_factor;        // We divide by decay_factor each round 
  uint32_t fixed_point_scaling;
  bool empty; 
  int32_t value;

} IIRLowPass;

// Public Interface
void begin_ilp(IIRLowPass * lp, uint32_t decay_factor, uint32_t fixed_point_scaling);
void push_ilp(IIRLowPass * lp, int32_t new_value);
int32_t read_ilp(IIRLowPass const * lp);
void reset_ilp(IIRLowPass * lp);

#ifdef __cplusplus
}
#endif
#endif
