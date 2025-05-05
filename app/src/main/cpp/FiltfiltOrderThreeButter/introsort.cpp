//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// introsort.cpp
//
// Code generation for function 'introsort'
//

// Include files
#include "introsort.h"
#include "anonymous_function.h"
#include "insertionsort.h"
#include "rt_nonfinite.h"

// Function Definitions
namespace coder {
namespace internal {
void introsort(int x[7], const anonymous_function cmp)
{
  insertionsort(x, cmp);
}

} // namespace internal
} // namespace coder

// End of code generation (introsort.cpp)
