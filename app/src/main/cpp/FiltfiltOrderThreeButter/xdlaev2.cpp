//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// xdlaev2.cpp
//
// Code generation for function 'xdlaev2'
//

// Include files
#include "xdlaev2.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
namespace coder {
namespace internal {
namespace reflapack {
double xdlaev2(double a, double b, double c, double &rt2)
{
  double ab;
  double acmn;
  double acmx;
  double adf;
  double rt1;
  double sm;
  sm = a + c;
  adf = std::abs(a - c);
  ab = std::abs(b + b);
  if (std::abs(a) > std::abs(c)) {
    acmx = a;
    acmn = c;
  } else {
    acmx = c;
    acmn = a;
  }
  if (adf > ab) {
    double b_a;
    b_a = ab / adf;
    adf *= std::sqrt(b_a * b_a + 1.0);
  } else if (adf < ab) {
    double b_a;
    b_a = adf / ab;
    adf = ab * std::sqrt(b_a * b_a + 1.0);
  } else {
    adf = ab * 1.4142135623730951;
  }
  if (sm < 0.0) {
    rt1 = 0.5 * (sm - adf);
    rt2 = acmx / rt1 * acmn - b / rt1 * b;
  } else if (sm > 0.0) {
    rt1 = 0.5 * (sm + adf);
    rt2 = acmx / rt1 * acmn - b / rt1 * b;
  } else {
    rt1 = 0.5 * adf;
    rt2 = -0.5 * adf;
  }
  return rt1;
}

} // namespace reflapack
} // namespace internal
} // namespace coder

// End of code generation (xdlaev2.cpp)
