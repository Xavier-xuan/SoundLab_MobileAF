//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// FiltfiltOrderThreeButter.cpp
//
// Code generation for function 'FiltfiltOrderThreeButter'
//

// Include files
#include "FiltfiltOrderThreeButter.h"
#include "eig.h"
#include "filtfilt.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
void FiltfiltOrderThreeButter(const coder::array<double, 2U> &x,
                              double cutoff_freq, double fs,
                              coder::array<double, 2U> &result)
{
  creal_T c[4];
  creal_T p[3];
  double a[9];
  double t1[9];
  double t2[9];
  double b_a[4];
  double a21;
  double b_re_tmp;
  double c_a;
  double d;
  double d1;
  double maxval;
  double p_im_tmp;
  double p_re_tmp;
  double re_tmp;
  int r1;
  int r2;
  int r3;
  signed char b1[2];
  a21 = 4.0 * std::tan(3.1415926535897931 * (cutoff_freq / (fs / 2.0)) / 2.0);
  std::memset(&a[0], 0, 9U * sizeof(double));
  a[0] = -1.0;
  p[0].re = 1.0;
  p[0].im = 0.0;
  for (r1 = 0; r1 < 2; r1++) {
    maxval = 1.7320508075688774 * static_cast<double>(r1) - 0.86602540378443871;
    re_tmp = p[r1].im;
    b_re_tmp = p[r1].re;
    p[r1 + 1].re = 0.49999999999999978 * b_re_tmp - -maxval * re_tmp;
    p[r1 + 1].im = 0.49999999999999978 * re_tmp + -maxval * b_re_tmp;
    for (r2 = r1 + 1; r2 >= 2; r2--) {
      p[1].re -= -0.49999999999999978 * p[0].re - maxval * p[0].im;
      p[1].im -= -0.49999999999999978 * p[0].im + maxval * p[0].re;
    }
  }
  d = (1.0 - -p[1].re * 0.0) / 1.0000000000000002;
  b_a[1] = d;
  b_a[0] = -p[1].re - d * 0.0;
  d = (0.0 - -p[2].re * 0.0) / 1.0000000000000002;
  b_a[3] = d;
  b_a[2] = -p[2].re - d * 0.0;
  b1[1] = 0;
  b1[0] = 1;
  for (r1 = 0; r1 < 2; r1++) {
    a[r1 + 1] = b1[r1];
    d = b_a[r1 + 2];
    d1 = b_a[r1];
    a[r1 + 4] = d1 + d * 0.0;
    a[r1 + 7] = d1 * 0.0 + d * 1.0000000000000002;
  }
  std::memset(&t2[0], 0, 9U * sizeof(double));
  t2[0] = 1.0;
  t2[4] = 1.0;
  t2[8] = 1.0;
  for (r1 = 0; r1 < 9; r1++) {
    d = a21 * a[r1] * 0.5 / 2.0;
    a[r1] = d;
    d1 = t2[r1];
    t1[r1] = d1 + d;
    d1 -= d;
    t2[r1] = d1;
  }
  r1 = 0;
  r2 = 1;
  r3 = 2;
  maxval = std::abs(t2[0]);
  a21 = std::abs(t2[1]);
  if (a21 > maxval) {
    maxval = a21;
    r1 = 1;
    r2 = 0;
  }
  if (std::abs(t2[2]) > maxval) {
    r1 = 2;
    r2 = 1;
    r3 = 0;
  }
  t2[r2] /= t2[r1];
  t2[r3] /= t2[r1];
  t2[r2 + 3] -= t2[r2] * t2[r1 + 3];
  t2[r3 + 3] -= t2[r3] * t2[r1 + 3];
  t2[r2 + 6] -= t2[r2] * t2[r1 + 6];
  t2[r3 + 6] -= t2[r3] * t2[r1 + 6];
  if (std::abs(t2[r3 + 3]) > std::abs(t2[r2 + 3])) {
    int rtemp;
    rtemp = r2;
    r2 = r3;
    r3 = rtemp;
  }
  t2[r3 + 3] /= t2[r2 + 3];
  t2[r3 + 6] -= t2[r3 + 3] * t2[r2 + 6];
  d = t1[r1];
  d1 = t1[r2] - d * t2[r2];
  maxval = t2[r3 + 3];
  a21 = t2[r3 + 6];
  re_tmp = ((t1[r3] - d * t2[r3]) - d1 * maxval) / a21;
  a[2] = re_tmp;
  b_re_tmp = t2[r1 + 6];
  d -= re_tmp * b_re_tmp;
  c_a = t2[r2 + 6];
  d1 -= re_tmp * c_a;
  p_re_tmp = t2[r2 + 3];
  d1 /= p_re_tmp;
  a[1] = d1;
  p_im_tmp = t2[r1 + 3];
  d -= d1 * p_im_tmp;
  d /= t2[r1];
  a[0] = d;
  d = t1[r1 + 3];
  d1 = t1[r2 + 3] - d * t2[r2];
  re_tmp = ((t1[r3 + 3] - d * t2[r3]) - d1 * maxval) / a21;
  a[5] = re_tmp;
  d -= re_tmp * b_re_tmp;
  d1 -= re_tmp * c_a;
  d1 /= p_re_tmp;
  a[4] = d1;
  d -= d1 * p_im_tmp;
  d /= t2[r1];
  a[3] = d;
  d = t1[r1 + 6];
  d1 = t1[r2 + 6] - d * t2[r2];
  re_tmp = ((t1[r3 + 6] - d * t2[r3]) - d1 * maxval) / a21;
  a[8] = re_tmp;
  d -= re_tmp * b_re_tmp;
  d1 -= re_tmp * c_a;
  d1 /= p_re_tmp;
  a[7] = d1;
  d -= d1 * p_im_tmp;
  d /= t2[r1];
  a[6] = d;
  coder::eig(a, p);
  c_a =
      (((1.0 - p[0].re) * (1.0 - p[1].re) - (0.0 - p[0].im) * (0.0 - p[1].im)) *
           (1.0 - p[2].re) -
       ((1.0 - p[0].re) * (0.0 - p[1].im) + (0.0 - p[0].im) * (1.0 - p[1].re)) *
           (0.0 - p[2].im)) /
      8.0;
  c[0].re = 1.0;
  c[0].im = 0.0;
  for (r1 = 0; r1 < 3; r1++) {
    p_re_tmp = p[r1].re;
    p_im_tmp = p[r1].im;
    re_tmp = c[r1].im;
    b_re_tmp = c[r1].re;
    c[r1 + 1].re = -p_re_tmp * b_re_tmp - -p_im_tmp * re_tmp;
    c[r1 + 1].im = -p_re_tmp * re_tmp + -p_im_tmp * b_re_tmp;
    for (r2 = r1 + 1; r2 >= 2; r2--) {
      maxval = c[r2 - 2].im;
      a21 = c[r2 - 2].re;
      c[r2 - 1].re -= p_re_tmp * a21 - p_im_tmp * maxval;
      c[r2 - 1].im -= p_re_tmp * maxval + p_im_tmp * a21;
    }
  }
  double b_c[4];
  b_a[0] = c_a;
  b_c[0] = c[0].re;
  b_a[1] = c_a * 3.0;
  b_c[1] = c[1].re;
  b_a[2] = c_a * 3.0;
  b_c[2] = c[2].re;
  b_a[3] = c_a;
  b_c[3] = c[3].re;
  coder::filtfilt(b_a, b_c, x, result);
}

// End of code generation (FiltfiltOrderThreeButter.cpp)
