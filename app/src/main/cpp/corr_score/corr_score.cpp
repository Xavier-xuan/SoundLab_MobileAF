//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// corr_score.cpp
//
// Code generation for function 'corr_score'
//

// Include files
#include "corr_score.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>
#include <cstring>

// Function Definitions
double corr_score(const coder::array<double, 2U> &signals)
{
  static const signed char iv[6]{4, 8, 9, 12, 13, 14};
  coder::array<double, 2U> x;
  double xy[16];
  double d[4];
  double b_d;
  double s;
  int i;
  int idx;
  int k;
  int xy_tmp;
  signed char tmp_data[6];
  x.set_size(signals.size(1), 4);
  idx = signals.size(1);
  for (i = 0; i < 4; i++) {
    for (xy_tmp = 0; xy_tmp < idx; xy_tmp++) {
      x[xy_tmp + x.size(0) * i] = signals[i + 4 * xy_tmp];
    }
  }
  idx = x.size(0) - 1;
  std::memset(&xy[0], 0, 16U * sizeof(double));
  if (x.size(0) < 2) {
    for (i = 0; i < 16; i++) {
      xy[i] = rtNaN;
    }
  } else {
    for (int j{0}; j < 4; j++) {
      s = 0.0;
      for (int b_i{0}; b_i <= idx; b_i++) {
        s += x[b_i + x.size(0) * j];
      }
      s /= static_cast<double>(x.size(0));
      for (int b_i{0}; b_i <= idx; b_i++) {
        x[b_i + x.size(0) * j] = x[b_i + x.size(0) * j] - s;
      }
    }
    for (int j{0}; j < 4; j++) {
      s = 0.0;
      for (k = 0; k <= idx; k++) {
        b_d = x[k + x.size(0) * j];
        s += b_d * b_d;
      }
      xy_tmp = j << 2;
      xy[j + xy_tmp] = s / static_cast<double>(idx);
      i = j + 2;
      for (int b_i{i}; b_i < 5; b_i++) {
        s = 0.0;
        for (k = 0; k <= idx; k++) {
          s += x[k + x.size(0) * (b_i - 1)] * x[k + x.size(0) * j];
        }
        xy[(b_i + xy_tmp) - 1] = s / static_cast<double>(idx);
      }
    }
  }
  d[0] = std::sqrt(xy[0]);
  d[1] = std::sqrt(xy[5]);
  d[2] = std::sqrt(xy[10]);
  d[3] = std::sqrt(xy[15]);
  for (int j{0}; j < 4; j++) {
    i = j + 2;
    for (int b_i{i}; b_i < 5; b_i++) {
      xy_tmp = (b_i + (j << 2)) - 1;
      xy[xy_tmp] = xy[xy_tmp] / d[b_i - 1] / d[j];
    }
    for (int b_i{i}; b_i < 5; b_i++) {
      xy_tmp = (b_i + (j << 2)) - 1;
      b_d = xy[xy_tmp];
      s = std::abs(b_d);
      if (s > 1.0) {
        b_d /= s;
        xy[xy_tmp] = b_d;
      }
      xy[j + ((b_i - 1) << 2)] = xy[xy_tmp];
    }
    idx = j + (j << 2);
    s = xy[idx];
    if (s > 0.0) {
      if (std::isnan(s)) {
        xy[idx] = rtNaN;
      } else if (s < 0.0) {
        xy[idx] = -1.0;
      } else {
        xy[idx] = (s > 0.0);
      }
    } else {
      xy[idx] = rtNaN;
    }
  }
  //  Exclude diagonal elements (self-correlations)
  //  Determine the maximum cosine similarity and filter values within 20% of
  //  the max
  if (!std::isnan(xy[4])) {
    idx = 1;
  } else {
    boolean_T exitg1;
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 7)) {
      if (!std::isnan(xy[iv[k - 1]])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }
  if (idx == 0) {
    s = xy[4];
  } else {
    s = xy[iv[idx - 1]];
    i = idx + 1;
    for (k = i; k < 7; k++) {
      b_d = xy[iv[k - 1]];
      if (s < b_d) {
        s = b_d;
      }
    }
  }
  xy_tmp = 0;
  idx = 0;
  for (int b_i{0}; b_i < 6; b_i++) {
    if (xy[iv[b_i]] >= s * 0.6) {
      xy_tmp++;
      tmp_data[idx] = static_cast<signed char>(b_i);
      idx++;
    }
  }
  //  Calculate average of filtered values
  if (xy_tmp == 0) {
    s = 0.0;
  } else {
    s = xy[iv[tmp_data[0]]];
    for (k = 2; k <= xy_tmp; k++) {
      s += xy[iv[tmp_data[k - 1]]];
    }
  }
  return s / static_cast<double>(xy_tmp);
}

// End of code generation (corr_score.cpp)
