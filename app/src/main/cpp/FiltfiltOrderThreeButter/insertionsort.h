//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// insertionsort.h
//
// Code generation for function 'insertionsort'
//

#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

// Include files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace coder {
class anonymous_function;

}

// Function Declarations
namespace coder {
namespace internal {
void insertionsort(double x[3]);

void insertionsort(int x[7], const anonymous_function cmp);

} // namespace internal
} // namespace coder

#endif
// End of code generation (insertionsort.h)
