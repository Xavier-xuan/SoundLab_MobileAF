//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// eig.cpp
//
// Code generation for function 'eig'
//

// Include files
#include "eig.h"
#include "eigStandard.h"
#include "rt_nonfinite.h"
#include "xdlahqr.h"
#include "xdsterf.h"
#include "xzgehrd.h"
#include "xzlarfg.h"
#include "xzlascl.h"
#include <algorithm>
#include <cmath>

// Function Definitions
namespace coder {
void eig(const double A[9], creal_T V[3])
{
  double b_A[9];
  double e[2];
  double absx;
  int k;
  boolean_T iscale;
  iscale = true;
  for (k = 0; k < 9; k++) {
    if (iscale) {
      absx = A[k];
      if (std::isinf(absx) || std::isnan(absx)) {
        iscale = false;
      }
    } else {
      iscale = false;
    }
  }
  if (!iscale) {
    V[0].re = rtNaN;
    V[0].im = 0.0;
    V[1].re = rtNaN;
    V[1].im = 0.0;
    V[2].re = rtNaN;
    V[2].im = 0.0;
  } else {
    int exitg1;
    int i;
    boolean_T exitg2;
    iscale = true;
    k = 0;
    exitg2 = false;
    while ((!exitg2) && (k < 3)) {
      i = 0;
      do {
        exitg1 = 0;
        if (i <= k) {
          if (!(A[i + 3 * k] == A[k + 3 * i])) {
            iscale = false;
            exitg1 = 1;
          } else {
            i++;
          }
        } else {
          k++;
          exitg1 = 2;
        }
      } while (exitg1 == 0);
      if (exitg1 == 1) {
        exitg2 = true;
      }
    }
    if (iscale) {
      double a__3[3];
      double anrm;
      std::copy(&A[0], &A[9], &b_A[0]);
      anrm = 0.0;
      k = 0;
      exitg2 = false;
      while ((!exitg2) && (k < 3)) {
        i = 0;
        do {
          exitg1 = 0;
          if (i <= k) {
            absx = std::abs(A[i + 3 * k]);
            if (std::isnan(absx)) {
              anrm = rtNaN;
              exitg1 = 1;
            } else {
              if (absx > anrm) {
                anrm = absx;
              }
              i++;
            }
          } else {
            k++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);
        if (exitg1 == 1) {
          exitg2 = true;
        }
      }
      if (std::isinf(anrm) || std::isnan(anrm)) {
        a__3[0] = rtNaN;
        a__3[1] = rtNaN;
        a__3[2] = rtNaN;
      } else {
        double tau[2];
        iscale = false;
        if ((anrm > 0.0) && (anrm < 1.0010415475915505E-146)) {
          iscale = true;
          anrm = 1.0010415475915505E-146 / anrm;
          internal::reflapack::xzlascl(1.0, anrm, b_A);
        } else if (anrm > 9.9895953610111751E+145) {
          iscale = true;
          anrm = 9.9895953610111751E+145 / anrm;
          internal::reflapack::xzlascl(1.0, anrm, b_A);
        }
        for (i = 0; i < 2; i++) {
          double taui;
          int e_tmp_tmp;
          e_tmp_tmp = i + 3 * i;
          e[i] = b_A[e_tmp_tmp + 1];
          taui = internal::reflapack::xzlarfg(2 - i, e[i], b_A, i * 3 + 3);
          if (taui != 0.0) {
            double b_tau_tmp;
            double temp1;
            int b_i;
            int i1;
            int i2;
            int tau_tmp;
            b_A[e_tmp_tmp + 1] = 1.0;
            for (k = i + 1; k < 3; k++) {
              tau[k - 1] = 0.0;
            }
            b_i = 1 - i;
            i1 = 2 - i;
            for (int jj{0}; jj <= b_i; jj++) {
              k = i + jj;
              temp1 = taui * b_A[(k + 3 * i) + 1];
              absx = 0.0;
              tau_tmp = 3 * (k + 1);
              tau[k] += temp1 * b_A[(k + tau_tmp) + 1];
              i2 = jj + 2;
              for (int ii{i2}; ii <= i1; ii++) {
                b_tau_tmp = b_A[tau_tmp + 2];
                tau[1] += temp1 * b_tau_tmp;
                absx += b_tau_tmp * b_A[3 * i + 2];
              }
              tau[k] += taui * absx;
            }
            absx = 0.0;
            for (k = 0; k <= b_i; k++) {
              absx += tau[i + k] * b_A[(e_tmp_tmp + k) + 1];
            }
            absx *= -0.5 * taui;
            if (!(absx == 0.0)) {
              for (k = 0; k <= b_i; k++) {
                tau_tmp = i + k;
                tau[tau_tmp] += absx * b_A[(e_tmp_tmp + k) + 1];
              }
            }
            for (int jj{0}; jj <= b_i; jj++) {
              k = i + jj;
              temp1 = b_A[(k + 3 * i) + 1];
              absx = tau[k];
              b_tau_tmp = absx * temp1;
              tau_tmp = 3 * (k + 1);
              k = (k + tau_tmp) + 1;
              b_A[k] = (b_A[k] - b_tau_tmp) - b_tau_tmp;
              i2 = jj + 2;
              for (int ii{i2}; ii <= i1; ii++) {
                b_A[tau_tmp + 2] =
                    (b_A[tau_tmp + 2] - tau[1] * temp1) - b_A[3 * i + 2] * absx;
              }
            }
          }
          b_A[e_tmp_tmp + 1] = e[i];
          a__3[i] = b_A[e_tmp_tmp];
          tau[i] = taui;
        }
        a__3[2] = b_A[8];
        k = internal::reflapack::xdsterf(a__3, e);
        if (k != 0) {
          a__3[0] = rtNaN;
          a__3[1] = rtNaN;
          a__3[2] = rtNaN;
        } else if (iscale) {
          absx = 1.0 / anrm;
          a__3[0] *= absx;
          a__3[1] *= absx;
          a__3[2] *= absx;
        }
      }
      V[0].re = a__3[0];
      V[0].im = 0.0;
      V[1].re = a__3[1];
      V[1].im = 0.0;
      V[2].re = a__3[2];
      V[2].im = 0.0;
    } else {
      iscale = true;
      k = 0;
      exitg2 = false;
      while ((!exitg2) && (k < 3)) {
        i = 0;
        do {
          exitg1 = 0;
          if (i <= k) {
            if (!(A[i + 3 * k] == -A[k + 3 * i])) {
              iscale = false;
              exitg1 = 1;
            } else {
              i++;
            }
          } else {
            k++;
            exitg1 = 2;
          }
        } while (exitg1 == 0);
        if (exitg1 == 1) {
          exitg2 = true;
        }
      }
      if (iscale) {
        double a__3[3];
        double wi[3];
        int b_i;
        std::copy(&A[0], &A[9], &b_A[0]);
        internal::reflapack::xzgehrd(b_A, 1, 3);
        k = internal::reflapack::xdlahqr(1, 3, b_A, absx, a__3, wi);
        b_i = static_cast<unsigned char>(k);
        for (i = 0; i < b_i; i++) {
          V[i].re = rtNaN;
          V[i].im = 0.0;
        }
        b_i = k + 1;
        for (i = b_i; i < 4; i++) {
          V[i - 1].re = 0.0;
          V[i - 1].im = wi[i - 1];
        }
      } else {
        eigStandard(A, V);
      }
    }
  }
}

} // namespace coder

// End of code generation (eig.cpp)
