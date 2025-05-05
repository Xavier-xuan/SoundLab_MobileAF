//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// filtfilt.cpp
//
// Code generation for function 'filtfilt'
//

// Include files
#include "filtfilt.h"
#include "FiltfiltOrderThreeButter_internal_types.h"
#include "anonymous_function.h"
#include "filter.h"
#include "introsort.h"
#include "rt_nonfinite.h"
#include "sparse.h"
#include "coder_array.h"
#include "cs.h"
#include "makeCXSparseMatrix.h"
#include "solve_from_lu.h"
#include "solve_from_qr.h"
#include <algorithm>
#include <cmath>

// Function Declarations
static int div_s32(int numerator, int denominator);

// Function Definitions
static int div_s32(int numerator, int denominator)
{
  int quotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    unsigned int tempAbsQuotient;
    unsigned int u;
    if (numerator < 0) {
      tempAbsQuotient = ~static_cast<unsigned int>(numerator) + 1U;
    } else {
      tempAbsQuotient = static_cast<unsigned int>(numerator);
    }
    if (denominator < 0) {
      u = ~static_cast<unsigned int>(denominator) + 1U;
    } else {
      u = static_cast<unsigned int>(denominator);
    }
    tempAbsQuotient /= u;
    if ((numerator < 0) != (denominator < 0)) {
      quotient = -static_cast<int>(tempAbsQuotient);
    } else {
      quotient = static_cast<int>(tempAbsQuotient);
    }
  }
  return quotient;
}

namespace coder {
void filtfilt(const double b[4], const double a[4], const array<double, 2U> &x,
              array<double, 2U> &y)
{
  static const signed char b_cidxInt[7]{1, 1, 1, 2, 3, 2, 3};
  static const signed char b_ridxInt[7]{1, 2, 3, 2, 3, 1, 2};
  sparse b_y;
  array<double, 1U> yCol;
  array<double, 1U> yc5;
  array<double, 1U> ytemp;
  double xt[9];
  double yc3[9];
  double zo_data[3];
  if (x.size(1) == 0) {
    y.set_size(0, 0);
  } else {
    cs_din *N;
    cs_dis *S;
    anonymous_function b_this;
    double vals[7];
    double a2_data[4];
    double b2_data[4];
    double outBuff[3];
    double val;
    int sortedIndices[7];
    int cptr;
    int currRowIdx;
    int i;
    int loop_ub;
    int naxpy;
    int ridx;
    signed char cidxInt[7];
    signed char ridxInt[7];
    b2_data[0] = b[0];
    a2_data[0] = a[0];
    b2_data[1] = b[1];
    a2_data[1] = a[1];
    b2_data[2] = b[2];
    a2_data[2] = a[2];
    b2_data[3] = b[3];
    a2_data[3] = a[3];
    vals[0] = a[1] + 1.0;
    vals[1] = a[2];
    vals[3] = 1.0;
    vals[5] = -1.0;
    vals[2] = a[3];
    vals[4] = 1.0;
    vals[6] = -1.0;
    outBuff[0] = b[1] - b[0] * a[1];
    outBuff[1] = b[2] - b[0] * a[2];
    outBuff[2] = b[3] - b[0] * a[3];
    for (int k{0}; k < 7; k++) {
      sortedIndices[k] = k + 1;
      b_this.workspace.a[k] = b_cidxInt[k];
      b_this.workspace.b[k] = b_ridxInt[k];
    }
    internal::introsort(sortedIndices, b_this);
    b_y.d.set_size(7);
    b_y.colidx.set_size(4);
    b_y.colidx[1] = 0;
    b_y.colidx[2] = 0;
    b_y.colidx[3] = 0;
    b_y.colidx[0] = 1;
    b_y.rowidx.set_size(7);
    for (int k{0}; k < 7; k++) {
      i = sortedIndices[k];
      cidxInt[k] = b_cidxInt[i - 1];
      ridxInt[k] = b_ridxInt[i - 1];
      b_y.d[k] = 0.0;
      b_y.rowidx[k] = 0;
    }
    cptr = 0;
    for (naxpy = 0; naxpy < 3; naxpy++) {
      while ((cptr + 1 <= 7) && (cidxInt[cptr] == naxpy + 1)) {
        b_y.rowidx[cptr] = ridxInt[cptr];
        cptr++;
      }
      b_y.colidx[naxpy + 1] = cptr + 1;
    }
    for (int k{0}; k < 7; k++) {
      b_y.d[k] = vals[sortedIndices[k] - 1];
    }
    cptr = 1;
    for (naxpy = 0; naxpy < 3; naxpy++) {
      ridx = b_y.colidx[naxpy];
      b_y.colidx[naxpy] = cptr;
      int exitg1;
      do {
        exitg1 = 0;
        i = b_y.colidx[naxpy + 1];
        if (ridx < i) {
          val = 0.0;
          currRowIdx = b_y.rowidx[ridx - 1];
          while ((ridx < i) && (b_y.rowidx[ridx - 1] == currRowIdx)) {
            val += b_y.d[ridx - 1];
            ridx++;
          }
          if (val != 0.0) {
            b_y.d[cptr - 1] = val;
            b_y.rowidx[cptr - 1] = currRowIdx;
            cptr++;
          }
        } else {
          exitg1 = 1;
        }
      } while (exitg1 == 0);
    }
    cs_di *cxA;
    b_y.colidx[3] = cptr;
    cxA = makeCXSparseMatrix(b_y.colidx[3] - 1, 3, 3, &(b_y.colidx.data())[0],
                             &(b_y.rowidx.data())[0], &(b_y.d.data())[0]);
    S = cs_di_sqr(2, cxA, 0);
    N = cs_di_lu(cxA, S, 1);
    cs_di_spfree(cxA);
    if (N == nullptr) {
      cs_di *b_cxA;
      cs_din *b_N;
      cs_dis *b_S;
      cs_di_sfree(S);
      cs_di_nfree(N);
      b_cxA =
          makeCXSparseMatrix(b_y.colidx[3] - 1, 3, 3, &(b_y.colidx.data())[0],
                             &(b_y.rowidx.data())[0], &(b_y.d.data())[0]);
      b_S = cs_di_sqr(2, b_cxA, 1);
      b_N = cs_di_qr(b_cxA, b_S);
      cs_di_spfree(b_cxA);
      qr_rank_di(b_N, &val);
      solve_from_qr_di(b_N, b_S, (double *)&outBuff[0], 3, 3);
      cs_di_sfree(b_S);
      cs_di_nfree(b_N);
    } else {
      solve_from_lu_di(N, S, (double *)&outBuff[0], 3);
      cs_di_sfree(S);
      cs_di_nfree(N);
    }
    if (x.size(1) < 10000) {
      double outBuff_data[3];
      double d;
      val = 2.0 * x[0];
      d = 2.0 * x[x.size(1) - 1];
      yc5.set_size(x.size(1) + 18);
      for (i = 0; i < 9; i++) {
        yc5[i] = val - x[9 - i];
      }
      loop_ub = x.size(1);
      for (i = 0; i < loop_ub; i++) {
        yc5[i + 9] = x[i];
      }
      for (i = 0; i < 9; i++) {
        yc5[(i + x.size(1)) + 9] = d - x[(x.size(1) - i) - 2];
      }
      double b_a2_data[4];
      double b_b2_data[4];
      b_b2_data[0] = b[0];
      b_a2_data[0] = a[0];
      b_b2_data[1] = b[1];
      b_a2_data[1] = a[1];
      b_b2_data[2] = b[2];
      b_a2_data[2] = a[2];
      b_b2_data[3] = b[3];
      b_a2_data[3] = a[3];
      outBuff_data[0] = outBuff[0] * yc5[0];
      outBuff_data[1] = yc5[0] * outBuff[1];
      outBuff_data[2] = yc5[0] * outBuff[2];
      filter(b_b2_data, b_a2_data, yc5, outBuff_data, ytemp);
      cptr = ytemp.size(0) - 1;
      yc5.set_size(ytemp.size(0));
      for (i = 0; i <= cptr; i++) {
        yc5[i] = ytemp[cptr - i];
      }
      ytemp.set_size(yc5.size(0));
      loop_ub = yc5.size(0);
      for (i = 0; i < loop_ub; i++) {
        ytemp[i] = yc5[i];
      }
      outBuff_data[0] = outBuff[0] * ytemp[0];
      outBuff_data[1] = ytemp[0] * outBuff[1];
      outBuff_data[2] = ytemp[0] * outBuff[2];
      yc5.set_size(ytemp.size(0));
      loop_ub = ytemp.size(0) - 1;
      for (i = 0; i <= loop_ub; i++) {
        yc5[i] = ytemp[i];
      }
      filter(b2_data, a2_data, yc5, outBuff_data, ytemp);
      if (ytemp.size(0) - 9 < 10) {
        i = 10;
        cptr = 1;
      } else {
        i = ytemp.size(0);
        cptr = -1;
      }
      yCol.set_size(x.size(1));
      loop_ub = x.size(1);
      for (ridx = 0; ridx < loop_ub; ridx++) {
        yCol[ridx] = ytemp[(i + cptr * ridx) - 10];
      }
    } else {
      double a__1[9];
      double b_a2_data[4];
      double b_b2_data[4];
      double outBuff_data[3];
      boolean_T b_b;
      val = 2.0 * x[0];
      for (i = 0; i < 9; i++) {
        xt[i] = -x[9 - i] + val;
      }
      b_b2_data[0] = b[0];
      b_a2_data[0] = a[0];
      b_b2_data[1] = b[1];
      b_a2_data[1] = a[1];
      b_b2_data[2] = b[2];
      b_a2_data[2] = a[2];
      b_b2_data[3] = b[3];
      b_a2_data[3] = a[3];
      outBuff_data[0] = outBuff[0] * xt[0];
      outBuff_data[1] = xt[0] * outBuff[1];
      outBuff_data[2] = xt[0] * outBuff[2];
      filter(b_b2_data, b_a2_data, xt, outBuff_data, a__1, zo_data);
      b_b2_data[0] = b[0];
      b_a2_data[0] = a[0];
      b_b2_data[1] = b[1];
      b_a2_data[1] = a[1];
      b_b2_data[2] = b[2];
      b_a2_data[2] = a[2];
      b_b2_data[3] = b[3];
      b_a2_data[3] = a[3];
      b_b = (std::isinf(a[0]) || std::isnan(a[0]));
      if ((!b_b) && (!(a[0] == 0.0)) && (a[0] != 1.0)) {
        b_b2_data[0] = b[0] / a[0];
        b_b2_data[1] = b[1] / a[0];
        b_b2_data[2] = b[2] / a[0];
        b_b2_data[3] = b[3] / a[0];
        b_a2_data[1] = a[1] / a[0];
        b_a2_data[2] = a[2] / a[0];
        b_a2_data[3] = a[3] / a[0];
        b_a2_data[0] = 1.0;
      }
      yCol.set_size(x.size(1));
      currRowIdx = x.size(1);
      outBuff_data[0] = 0.0;
      yCol[0] = zo_data[0];
      outBuff_data[1] = 0.0;
      yCol[1] = zo_data[1];
      outBuff_data[2] = 0.0;
      yCol[2] = zo_data[2];
      for (int k{4}; k <= currRowIdx; k++) {
        yCol[k - 1] = 0.0;
      }
      for (int k{0}; k < currRowIdx; k++) {
        cptr = currRowIdx - k;
        if (cptr <= 4) {
          naxpy = cptr;
        } else {
          naxpy = 4;
        }
        for (int j{0}; j < naxpy; j++) {
          ridx = k + j;
          yCol[ridx] = yCol[ridx] + x[k] * b_b2_data[j];
        }
        if (cptr - 1 < 3) {
          naxpy = cptr - 2;
        } else {
          naxpy = 2;
        }
        val = -yCol[k];
        for (int j{0}; j <= naxpy; j++) {
          ridx = (k + j) + 1;
          yCol[ridx] = yCol[ridx] + val * b_a2_data[j + 1];
        }
      }
      ridx = x.size(1) - 3;
      loop_ub = x.size(1) - 1;
      for (int k{ridx}; k <= loop_ub; k++) {
        cptr = currRowIdx - k;
        naxpy = 3 - cptr;
        for (int j{0}; j <= naxpy; j++) {
          outBuff_data[j] += x[k] * b_b2_data[cptr + j];
        }
      }
      for (int k{ridx}; k <= loop_ub; k++) {
        cptr = currRowIdx - k;
        naxpy = 3 - cptr;
        for (int j{0}; j <= naxpy; j++) {
          outBuff_data[j] += -yCol[k] * b_a2_data[cptr + j];
        }
      }
      val = 2.0 * x[x.size(1) - 1];
      for (i = 0; i < 9; i++) {
        xt[i] = -x[(x.size(1) - i) - 2] + val;
      }
      b_b2_data[0] = b[0];
      b_a2_data[0] = a[0];
      b_b2_data[1] = b[1];
      b_a2_data[1] = a[1];
      b_b2_data[2] = b[2];
      b_a2_data[2] = a[2];
      b_b2_data[3] = b[3];
      b_a2_data[3] = a[3];
      if ((!b_b) && (!(a[0] == 0.0)) && (a[0] != 1.0)) {
        b_b2_data[0] = b[0] / a[0];
        b_b2_data[1] = b[1] / a[0];
        b_b2_data[2] = b[2] / a[0];
        b_b2_data[3] = b[3] / a[0];
        b_a2_data[1] = a[1] / a[0];
        b_a2_data[2] = a[2] / a[0];
        b_a2_data[3] = a[3] / a[0];
        b_a2_data[0] = 1.0;
      }
      yc3[0] = outBuff_data[0];
      yc3[1] = outBuff_data[1];
      yc3[2] = outBuff_data[2];
      for (int k{0}; k < 6; k++) {
        yc3[k + 3] = 0.0;
      }
      for (int k{0}; k < 9; k++) {
        if (9 - k < 4) {
          i = 8 - k;
        } else {
          i = 3;
        }
        for (int j{0}; j <= i; j++) {
          cptr = k + j;
          yc3[cptr] += xt[k] * b_b2_data[j];
        }
        if (8 - k < 3) {
          i = 7 - k;
        } else {
          i = 2;
        }
        val = -yc3[k];
        for (int j{0}; j <= i; j++) {
          cptr = (k + j) + 1;
          yc3[cptr] += val * b_a2_data[j + 1];
        }
      }
      val = yc3[8];
      b_b2_data[0] = b[0];
      b_a2_data[0] = a[0];
      b_b2_data[1] = b[1];
      b_a2_data[1] = a[1];
      b_b2_data[2] = b[2];
      b_a2_data[2] = a[2];
      b_b2_data[3] = b[3];
      b_a2_data[3] = a[3];
      for (i = 0; i < 9; i++) {
        xt[i] = yc3[8 - i];
      }
      std::copy(&xt[0], &xt[9], &yc3[0]);
      outBuff_data[0] = outBuff[0] * val;
      outBuff_data[1] = outBuff[1] * val;
      outBuff_data[2] = outBuff[2] * val;
      filter(b_b2_data, b_a2_data, yc3, outBuff_data, a__1, zo_data);
      yc5.set_size(yCol.size(0));
      for (i = 0; i <= loop_ub; i++) {
        yc5[i] = yCol[loop_ub - i];
      }
      yCol.set_size(yc5.size(0));
      loop_ub = yc5.size(0);
      for (i = 0; i < loop_ub; i++) {
        yCol[i] = yc5[i];
      }
      filter(b2_data, a2_data, yCol, zo_data, yc5);
      if (yc5.size(0) < 1) {
        i = 0;
        cptr = 1;
        ridx = -1;
      } else {
        i = yc5.size(0) - 1;
        cptr = -1;
        ridx = 0;
      }
      loop_ub = div_s32(ridx - i, cptr);
      yCol.set_size(loop_ub + 1);
      for (ridx = 0; ridx <= loop_ub; ridx++) {
        yCol[ridx] = yc5[i + cptr * ridx];
      }
    }
    y.set_size(1, yCol.size(0));
    loop_ub = yCol.size(0);
    for (i = 0; i < loop_ub; i++) {
      y[i] = yCol[i];
    }
  }
}

} // namespace coder

// End of code generation (filtfilt.cpp)
