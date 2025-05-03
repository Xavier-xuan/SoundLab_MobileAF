//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// FFTImplementationCallback.h
//
// Code generation for function 'FFTImplementationCallback'
//

#ifndef FFTIMPLEMENTATIONCALLBACK_H
#define FFTIMPLEMENTATIONCALLBACK_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
namespace coder {
namespace internal {
namespace fft {
class FFTImplementationCallback {
public:
  static void doHalfLengthRadix2(const double x[256], creal_T y[256]);
};

} // namespace fft
} // namespace internal
} // namespace coder

// Function Declarations
void binary_expand_op(const coder::array<double, 1U> &in1,
                      const double in2[256], creal_T in3[256]);

#endif
// End of code generation (FFTImplementationCallback.h)
