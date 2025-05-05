//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// filter.h
//
// Code generation for function 'filter'
//

#ifndef FILTER_H
#define FILTER_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
int filter(double b_data[], double a_data[], const double x[9],
           const double zi_data[], double y[9], double zf_data[]);

void filter(double b_data[], double a_data[], const array<double, 1U> &x,
            const double zi_data[], array<double, 1U> &y);

} // namespace coder

#endif
// End of code generation (filter.h)
