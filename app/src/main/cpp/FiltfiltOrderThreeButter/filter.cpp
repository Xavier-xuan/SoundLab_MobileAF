//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// filter.cpp
//
// Code generation for function 'filter'
//

// Include files
#include "filter.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>

// Function Definitions
namespace coder {
int filter(double b_data[], double a_data[], const double x[9],
           const double zi_data[], double y[9], double zf_data[])
{
  int zf_size;
  if ((!std::isinf(a_data[0])) && (!std::isnan(a_data[0])) &&
      (!(a_data[0] == 0.0)) && (a_data[0] != 1.0)) {
    b_data[0] /= a_data[0];
    b_data[1] /= a_data[0];
    b_data[2] /= a_data[0];
    b_data[3] /= a_data[0];
    a_data[1] /= a_data[0];
    a_data[2] /= a_data[0];
    a_data[3] /= a_data[0];
    a_data[0] = 1.0;
  }
  zf_size = 3;
  zf_data[0] = 0.0;
  y[0] = zi_data[0];
  zf_data[1] = 0.0;
  y[1] = zi_data[1];
  zf_data[2] = 0.0;
  y[2] = zi_data[2];
  for (int k{0}; k < 6; k++) {
    y[k + 3] = 0.0;
  }
  for (int k{0}; k < 9; k++) {
    double as;
    int i;
    int y_tmp;
    if (9 - k < 4) {
      i = 8 - k;
    } else {
      i = 3;
    }
    for (int j{0}; j <= i; j++) {
      y_tmp = k + j;
      y[y_tmp] += x[k] * b_data[j];
    }
    if (8 - k < 3) {
      i = 7 - k;
    } else {
      i = 2;
    }
    as = -y[k];
    for (int j{0}; j <= i; j++) {
      y_tmp = (k + j) + 1;
      y[y_tmp] += as * a_data[j + 1];
    }
  }
  for (int k{0}; k < 3; k++) {
    for (int j{0}; j <= k; j++) {
      zf_data[j] += x[k + 6] * b_data[(j - k) + 3];
    }
  }
  for (int k{0}; k < 3; k++) {
    for (int j{0}; j <= k; j++) {
      zf_data[j] += -y[k + 6] * a_data[(j - k) + 3];
    }
  }
  return zf_size;
}

void filter(double b_data[], double a_data[], const array<double, 1U> &x,
            const double zi_data[], array<double, 1U> &y)
{
  int niccp;
  int nx;
  if ((!std::isinf(a_data[0])) && (!std::isnan(a_data[0])) &&
      (!(a_data[0] == 0.0)) && (a_data[0] != 1.0)) {
    b_data[0] /= a_data[0];
    b_data[1] /= a_data[0];
    b_data[2] /= a_data[0];
    b_data[3] /= a_data[0];
    a_data[1] /= a_data[0];
    a_data[2] /= a_data[0];
    a_data[3] /= a_data[0];
    a_data[0] = 1.0;
  }
  y.set_size(x.size(0));
  nx = x.size(0);
  if (x.size(0) < 3) {
    niccp = x.size(0) - 1;
  } else {
    niccp = 2;
  }
  for (int k{0}; k <= niccp; k++) {
    y[k] = zi_data[k];
  }
  niccp += 2;
  for (int k{niccp}; k <= nx; k++) {
    y[k - 1] = 0.0;
  }
  for (int k{0}; k < nx; k++) {
    double as;
    int naxpy;
    int y_tmp;
    niccp = nx - k;
    if (niccp <= 4) {
      naxpy = niccp;
    } else {
      naxpy = 4;
    }
    for (int j{0}; j < naxpy; j++) {
      y_tmp = k + j;
      y[y_tmp] = y[y_tmp] + x[k] * b_data[j];
    }
    if (niccp - 1 < 3) {
      naxpy = niccp - 2;
    } else {
      naxpy = 2;
    }
    as = -y[k];
    for (int j{0}; j <= naxpy; j++) {
      y_tmp = (k + j) + 1;
      y[y_tmp] = y[y_tmp] + as * a_data[j + 1];
    }
  }
}

} // namespace coder

// End of code generation (filter.cpp)
