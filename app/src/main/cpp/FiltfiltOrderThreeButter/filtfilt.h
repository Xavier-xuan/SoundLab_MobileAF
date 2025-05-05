//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// filtfilt.h
//
// Code generation for function 'filtfilt'
//

#ifndef FILTFILT_H
#define FILTFILT_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
void filtfilt(const double b[4], const double a[4], const array<double, 2U> &x,
              array<double, 2U> &y);

}

#endif
// End of code generation (filtfilt.h)
