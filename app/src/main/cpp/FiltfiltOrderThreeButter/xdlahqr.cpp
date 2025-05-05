//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// xdlahqr.cpp
//
// Code generation for function 'xdlahqr'
//

// Include files
#include "xdlahqr.h"
#include "FiltfiltOrderThreeButter_rtwutil.h"
#include "rt_nonfinite.h"
#include "xdlanv2.h"
#include "xnrm2.h"
#include <cmath>

// Function Definitions
namespace coder {
namespace internal {
namespace reflapack {
int xdlahqr(int ilo, int ihi, double h[9], double &z, double wr[3],
            double wi[3])
{
  double bb;
  double h12;
  double tr;
  double tst;
  int b_i;
  int i;
  int info;
  z = 1.0;
  info = 0;
  i = static_cast<unsigned char>(ilo - 1);
  for (b_i = 0; b_i < i; b_i++) {
    wr[b_i] = h[b_i + 3 * b_i];
    wi[b_i] = 0.0;
  }
  i = ihi + 1;
  for (b_i = i; b_i < 4; b_i++) {
    wr[b_i - 1] = h[(b_i + 3 * (b_i - 1)) - 1];
    wi[b_i - 1] = 0.0;
  }
  if (ilo == ihi) {
    wr[ilo - 1] = h[(ilo + 3 * (ilo - 1)) - 1];
    wi[ilo - 1] = 0.0;
  } else {
    double smlnum;
    int kdefl;
    boolean_T exitg1;
    if (ilo <= ihi - 2) {
      h[ihi - 1] = 0.0;
    }
    smlnum = 2.2250738585072014E-308 *
             (static_cast<double>((ihi - ilo) + 1) / 2.2204460492503131E-16);
    kdefl = 0;
    b_i = ihi - 1;
    exitg1 = false;
    while ((!exitg1) && (b_i + 1 >= ilo)) {
      double d;
      double rt1r;
      double rt2r;
      int its;
      int knt;
      int l;
      boolean_T converged;
      boolean_T exitg2;
      l = ilo;
      converged = false;
      its = 0;
      exitg2 = false;
      while ((!exitg2) && (its < 301)) {
        double s;
        int k;
        boolean_T exitg3;
        k = b_i;
        exitg3 = false;
        while ((!exitg3) && (k + 1 > l)) {
          i = k + 3 * (k - 1);
          d = std::abs(h[i]);
          if (d <= smlnum) {
            exitg3 = true;
          } else {
            knt = k + 3 * k;
            h12 = h[knt];
            tr = std::abs(h12);
            bb = h[i - 1];
            tst = std::abs(bb) + tr;
            if (tst == 0.0) {
              if (k - 1 >= ilo) {
                tst = std::abs(h[k - 1]);
              }
              if (k + 2 <= ihi) {
                tst += std::abs(h[3 * k + 2]);
              }
            }
            if (d <= 2.2204460492503131E-16 * tst) {
              rt2r = std::abs(h[knt - 1]);
              h12 = std::abs(bb - h12);
              tst = std::fmax(tr, h12);
              bb = std::fmin(tr, h12);
              s = tst + bb;
              if (std::fmin(d, rt2r) * (std::fmax(d, rt2r) / s) <=
                  std::fmax(smlnum,
                            2.2204460492503131E-16 * (bb * (tst / s)))) {
                exitg3 = true;
              } else {
                k--;
              }
            } else {
              k--;
            }
          }
        }
        l = k + 1;
        if (k + 1 > ilo) {
          h[k + 3 * (k - 1)] = 0.0;
        }
        if (k + 1 >= b_i) {
          converged = true;
          exitg2 = true;
        } else {
          double v[3];
          double h22;
          int m;
          kdefl++;
          if (kdefl - kdefl / 20 * 20 == 0) {
            s = std::abs(h[b_i + 3 * (b_i - 1)]) + std::abs(h[b_i - 1]);
            tst = 0.75 * s + h[b_i + 3 * b_i];
            h12 = -0.4375 * s;
            bb = s;
            h22 = tst;
          } else if (kdefl - kdefl / 10 * 10 == 0) {
            s = std::abs(h[1]) + std::abs(h[5]);
            tst = 0.75 * s + h[0];
            h12 = -0.4375 * s;
            bb = s;
            h22 = tst;
          } else {
            knt = b_i + 3 * (b_i - 1);
            tst = h[knt - 1];
            bb = h[knt];
            knt = b_i + 3 * b_i;
            h12 = h[knt - 1];
            h22 = h[knt];
          }
          s = ((std::abs(tst) + std::abs(h12)) + std::abs(bb)) + std::abs(h22);
          if (s == 0.0) {
            rt1r = 0.0;
            tst = 0.0;
            rt2r = 0.0;
            bb = 0.0;
          } else {
            tst /= s;
            bb /= s;
            h12 /= s;
            h22 /= s;
            tr = (tst + h22) / 2.0;
            tst = (tst - tr) * (h22 - tr) - h12 * bb;
            bb = std::sqrt(std::abs(tst));
            if (tst >= 0.0) {
              rt1r = tr * s;
              rt2r = rt1r;
              tst = bb * s;
              bb = -tst;
            } else {
              rt1r = tr + bb;
              rt2r = tr - bb;
              if (std::abs(rt1r - h22) <= std::abs(rt2r - h22)) {
                rt1r *= s;
                rt2r = rt1r;
              } else {
                rt2r *= s;
                rt1r = rt2r;
              }
              tst = 0.0;
              bb = 0.0;
            }
          }
          m = b_i - 1;
          if (b_i - 1 >= 1) {
            h12 = h[0] - rt2r;
            s = (std::abs(h12) + std::abs(bb)) + std::abs(h[1]);
            tr = h[1] / s;
            v[0] = (tr * h[3] + (h[0] - rt1r) * (h12 / s)) - tst * (bb / s);
            v[1] = tr * (((h[0] + h[4]) - rt1r) - rt2r);
            v[2] = tr * h[5];
            s = (std::abs(v[0]) + std::abs(v[1])) + std::abs(v[2]);
            v[0] /= s;
            v[1] /= s;
            v[2] /= s;
          }
          for (k = m; k <= b_i; k++) {
            int nr;
            knt = (b_i - k) + 2;
            if (knt >= 3) {
              nr = 3;
            } else {
              nr = knt;
            }
            if (k > b_i - 1) {
              knt = ((k - 2) * 3 + k) - 1;
              i = static_cast<unsigned char>(nr);
              for (int b_k{0}; b_k < i; b_k++) {
                v[b_k] = h[knt + b_k];
              }
            }
            h12 = v[0];
            h22 = 0.0;
            if (nr > 0) {
              tst = blas::xnrm2(nr - 1, v);
              if (tst != 0.0) {
                bb = rt_hypotd_snf(v[0], tst);
                if (v[0] >= 0.0) {
                  bb = -bb;
                }
                if (std::abs(bb) < 1.0020841800044864E-292) {
                  knt = 0;
                  do {
                    knt++;
                    for (int b_k{2}; b_k <= nr; b_k++) {
                      v[b_k - 1] *= 9.9792015476736E+291;
                    }
                    bb *= 9.9792015476736E+291;
                    h12 *= 9.9792015476736E+291;
                  } while ((std::abs(bb) < 1.0020841800044864E-292) &&
                           (knt < 20));
                  bb = rt_hypotd_snf(h12, blas::xnrm2(nr - 1, v));
                  if (h12 >= 0.0) {
                    bb = -bb;
                  }
                  h22 = (bb - h12) / bb;
                  tst = 1.0 / (h12 - bb);
                  for (int b_k{2}; b_k <= nr; b_k++) {
                    v[b_k - 1] *= tst;
                  }
                  for (int b_k{0}; b_k < knt; b_k++) {
                    bb *= 1.0020841800044864E-292;
                  }
                  h12 = bb;
                } else {
                  h22 = (bb - v[0]) / bb;
                  tst = 1.0 / (v[0] - bb);
                  for (int b_k{2}; b_k <= nr; b_k++) {
                    v[b_k - 1] *= tst;
                  }
                  h12 = bb;
                }
              }
            }
            if (k > b_i - 1) {
              h[k - 1] = h12;
              h[k] = 0.0;
              if (k < b_i) {
                h[2] = 0.0;
              }
            }
            d = v[1];
            h12 = h22 * v[1];
            if (nr == 3) {
              rt1r = v[2];
              tst = h22 * v[2];
              for (int b_k{k}; b_k <= b_i + 1; b_k++) {
                knt = 3 * (b_k - 1);
                nr = k + knt;
                bb = h[nr - 1];
                tr = (bb + d * h[nr]) + rt1r * h[knt + 2];
                h[nr - 1] = bb - tr * h22;
                h[nr] -= tr * h12;
                h[knt + 2] -= tr * tst;
              }
              if (k + 3 <= b_i + 1) {
                i = k;
              } else {
                i = b_i - 2;
              }
              for (int b_k{1}; b_k <= i + 3; b_k++) {
                knt = (b_k + 3 * (k - 1)) - 1;
                bb = h[knt];
                nr = (b_k + 3 * k) - 1;
                tr = (bb + d * h[nr]) + rt1r * h[b_k + 5];
                h[knt] = bb - tr * h22;
                h[nr] -= tr * h12;
                h[b_k + 5] -= tr * tst;
              }
            } else if (nr == 2) {
              for (int b_k{k}; b_k <= b_i + 1; b_k++) {
                i = k + 3 * (b_k - 1);
                rt1r = h[i - 1];
                rt2r = h[i];
                tr = rt1r + d * rt2r;
                rt1r -= tr * h22;
                h[i - 1] = rt1r;
                rt2r -= tr * h12;
                h[i] = rt2r;
              }
              for (int b_k{1}; b_k <= b_i + 1; b_k++) {
                i = (b_k + 3 * (k - 1)) - 1;
                rt1r = h[i];
                knt = (b_k + 3 * k) - 1;
                rt2r = h[knt];
                tr = rt1r + d * rt2r;
                rt1r -= tr * h22;
                h[i] = rt1r;
                rt2r -= tr * h12;
                h[knt] = rt2r;
              }
            }
          }
          its++;
        }
      }
      if (!converged) {
        info = b_i + 1;
        exitg1 = true;
      } else {
        if (l == b_i + 1) {
          wr[b_i] = h[b_i + 3 * b_i];
          wi[b_i] = 0.0;
        } else if (l == b_i) {
          i = b_i + 3 * b_i;
          d = h[i - 1];
          knt = b_i + 3 * (b_i - 1);
          rt1r = h[knt];
          rt2r = h[i];
          wr[b_i - 1] =
              xdlanv2(h[knt - 1], d, rt1r, rt2r, wi[b_i - 1], tst, bb, h12, tr);
          wr[b_i] = tst;
          wi[b_i] = bb;
          h[i - 1] = d;
          h[knt] = rt1r;
          h[i] = rt2r;
        }
        kdefl = 0;
        b_i = l - 2;
      }
    }
    if (info != 0) {
      h[2] = 0.0;
    }
  }
  return info;
}

} // namespace reflapack
} // namespace internal
} // namespace coder

// End of code generation (xdlahqr.cpp)
