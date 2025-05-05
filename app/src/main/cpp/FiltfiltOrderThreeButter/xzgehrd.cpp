//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// xzgehrd.cpp
//
// Code generation for function 'xzgehrd'
//

// Include files
#include "xzgehrd.h"
#include "rt_nonfinite.h"
#include "xzlarfg.h"
#include <cstring>

// Function Declarations
static int div_nde_s32_floor(int numerator);

// Function Definitions
static int div_nde_s32_floor(int numerator)
{
  int i;
  if ((numerator < 0) && (numerator % 3 != 0)) {
    i = -1;
  } else {
    i = 0;
  }
  return numerator / 3 + i;
}

namespace coder {
namespace internal {
namespace reflapack {
void xzgehrd(double a[9], int ilo, int ihi)
{
  double work[3];
  double tau[2];
  if ((ihi - ilo) + 1 > 1) {
    int i;
    i = static_cast<unsigned char>(ilo - 1);
    if (i - 1 >= 0) {
      std::memset(&tau[0], 0, static_cast<unsigned int>(i) * sizeof(double));
    }
    for (int b_i{ihi}; b_i < 3; b_i++) {
      tau[b_i - 1] = 0.0;
    }
    work[0] = 0.0;
    work[1] = 0.0;
    work[2] = 0.0;
    i = ihi - 1;
    for (int b_i{ilo}; b_i <= i; b_i++) {
      double alpha1;
      double d;
      double temp;
      int alpha1_tmp_tmp;
      int b_lastv;
      int c_i;
      int exitg1;
      int i1;
      int i2;
      int ia;
      int ic0;
      int in;
      int jA;
      int lastc;
      int lastv;
      int work_tmp;
      boolean_T exitg2;
      c_i = (b_i - 1) * 3;
      in = b_i * 3;
      alpha1_tmp_tmp = b_i + c_i;
      alpha1 = a[alpha1_tmp_tmp];
      lastv = ihi - b_i;
      d = xzlarfg(lastv, alpha1, a, c_i + 3);
      tau[b_i - 1] = d;
      a[alpha1_tmp_tmp] = 1.0;
      ic0 = in + 1;
      if (d != 0.0) {
        b_lastv = lastv;
        c_i = alpha1_tmp_tmp + lastv;
        while ((b_lastv > 0) && (a[c_i - 1] == 0.0)) {
          b_lastv--;
          c_i--;
        }
        lastc = ihi;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          c_i = in + lastc;
          ia = c_i;
          do {
            exitg1 = 0;
            if (ia <= c_i + (b_lastv - 1) * 3) {
              if (a[ia - 1] != 0.0) {
                exitg1 = 1;
              } else {
                ia += 3;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        b_lastv = 0;
        lastc = 0;
      }
      if (b_lastv > 0) {
        if (lastc != 0) {
          i1 = static_cast<unsigned char>(lastc);
          std::memset(&work[0], 0,
                      static_cast<unsigned int>(i1) * sizeof(double));
          c_i = alpha1_tmp_tmp;
          i1 = (in + 3 * (b_lastv - 1)) + 1;
          for (int iac{ic0}; iac <= i1; iac += 3) {
            i2 = (iac + lastc) - 1;
            for (ia = iac; ia <= i2; ia++) {
              work_tmp = ia - iac;
              work[work_tmp] += a[ia - 1] * a[c_i];
            }
            c_i++;
          }
        }
        d = -tau[b_i - 1];
        if (!(d == 0.0)) {
          jA = in;
          i1 = static_cast<unsigned char>(b_lastv);
          for (work_tmp = 0; work_tmp < i1; work_tmp++) {
            temp = a[alpha1_tmp_tmp + work_tmp];
            if (temp != 0.0) {
              temp *= d;
              i2 = jA + 1;
              c_i = lastc + jA;
              for (ic0 = i2; ic0 <= c_i; ic0++) {
                a[ic0 - 1] += work[(ic0 - jA) - 1] * temp;
              }
            }
            jA += 3;
          }
        }
      }
      jA = (b_i + in) + 1;
      d = tau[b_i - 1];
      if (d != 0.0) {
        c_i = alpha1_tmp_tmp + lastv;
        while ((lastv > 0) && (a[c_i - 1] == 0.0)) {
          lastv--;
          c_i--;
        }
        lastc = 3 - b_i;
        exitg2 = false;
        while ((!exitg2) && (lastc > 0)) {
          c_i = jA + (lastc - 1) * 3;
          ia = c_i;
          do {
            exitg1 = 0;
            if (ia <= (c_i + lastv) - 1) {
              if (a[ia - 1] != 0.0) {
                exitg1 = 1;
              } else {
                ia++;
              }
            } else {
              lastc--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        lastc = 0;
      }
      if (lastv > 0) {
        if (lastc != 0) {
          i1 = static_cast<unsigned char>(lastc);
          std::memset(&work[0], 0,
                      static_cast<unsigned int>(i1) * sizeof(double));
          i1 = jA + 3 * (lastc - 1);
          for (int iac{jA}; iac <= i1; iac += 3) {
            temp = 0.0;
            i2 = (iac + lastv) - 1;
            for (ia = iac; ia <= i2; ia++) {
              temp += a[ia - 1] * a[(alpha1_tmp_tmp + ia) - iac];
            }
            work_tmp = div_nde_s32_floor(iac - jA);
            work[work_tmp] += temp;
          }
        }
        if (!(-d == 0.0)) {
          i1 = static_cast<unsigned char>(lastc);
          for (work_tmp = 0; work_tmp < i1; work_tmp++) {
            temp = work[work_tmp];
            if (temp != 0.0) {
              temp *= -d;
              i2 = lastv + jA;
              for (ic0 = jA; ic0 < i2; ic0++) {
                a[ic0 - 1] += a[(alpha1_tmp_tmp + ic0) - jA] * temp;
              }
            }
            jA += 3;
          }
        }
      }
      a[alpha1_tmp_tmp] = alpha1;
    }
  }
}

} // namespace reflapack
} // namespace internal
} // namespace coder

// End of code generation (xzgehrd.cpp)
