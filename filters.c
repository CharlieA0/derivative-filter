#include "filters.h"
#include "datastruct.h"

// Helpers
int32_t fixed_point_round(int32_t x, uint32_t fixed_point_scaling);

int32_t fixed_point_round(int32_t x, uint32_t fixed_point_scaling) {
  return (x + fixed_point_scaling / 2) / fixed_point_scaling;
}

/**
 * A real-time low-pass derivation filter from,
 * "A Derivative Estimation Toolbox based on Algebraic Methods - Theory and Practice"
 * by Josef Zehetner, Johann Reger, and Martin Horn
 * https://ieeexplore.ieee.org/document/4389252
 **/

// Prototypes
int32_t pi_func(int32_t T, int32_t t);
int32_t compute_df(RingBuf const * rb);

// Passed a window size T and value t
// The magic coefficents are explained in the referenced paper
int32_t pi_func(int32_t T, int32_t t) {
  return 180*t*t - 192*t*T + 36*T*T; 
}

// Computes the derivative
int32_t compute_df(RingBuf const * rb) {
  
  int32_t M = rb->count-1; // Last window element
  int32_t k = rb->count;   // Window starts at k

  // Here we will compute equation 16 from the paper
  int32_t value = 0;
  for (int32_t i = 1; i < M+1; i++) {

    int32_t y1 = get_RingBuf(rb, k-i);
    int32_t y2 = get_RingBuf(rb, k-i-1);
    
    value += y1 * pi_func(M, i-1) + y2 * pi_func(M, i);
  }

  return value/(2*M*M*M*M); // Note: we've undistributed the pi function's division

}

// Call to set up derivative filter with a given window
void begin_df(DerivativeFilter* self, int16_t window_size, int32_t window_buf[], uint32_t time_step, uint32_t fixed_point_scaling) {
  init_RingBuf(&(self->deriv_window), window_buf, window_size);
  self->fixed_point_scaling = fixed_point_scaling;
  self->time_step = time_step;
}

// Should be called at regular frequency to add new values to th filter
void push_df(DerivativeFilter* self, int32_t new_value) {
  add_RingBuf(&(self->deriv_window), (self->fixed_point_scaling)*new_value);
}

// Evaluates the filter
int32_t read_df(DerivativeFilter const * self) {
  return fixed_point_round(compute_df(&(self->deriv_window)), self->fixed_point_scaling);
}

// Call to empty current buffer of values and reset the filter
void reset_df(DerivativeFilter* self) {
  empty_RingBuf(&(self->deriv_window));
}


/**
 * IIR Low Pass Filter
 */

void begin_ilp(IIRLowPass* ilp, uint32_t decay_factor, uint32_t fixed_point_scaling) {
  ilp->decay_factor = decay_factor;
  ilp->fixed_point_scaling = fixed_point_scaling;
  ilp->empty = true;
  ilp->value = 0;
}

void push_ilp(IIRLowPass* ilp, int32_t new_value) {

  if (!ilp->empty) {
    ilp->value = ((ilp->decay_factor - 1)*ilp->value + ilp->fixed_point_scaling * new_value)/ilp->decay_factor; 
  } 
  else {
    ilp->value = new_value;
    ilp->empty = false;
  } 
}

int32_t read_ilp(IIRLowPass const * ilp) {
  return fixed_point_round(ilp->value, ilp->fixed_point_scaling);
}

void reset_ilp(IIRLowPass* ilp) {
  ilp->value = 0;
}
