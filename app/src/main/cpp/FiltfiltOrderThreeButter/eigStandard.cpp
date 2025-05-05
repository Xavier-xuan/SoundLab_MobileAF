//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// eigStandard.cpp
//
// Code generation for function 'eigStandard'
//

// Include files
#include "eigStandard.h"
#include "rt_nonfinite.h"
#include "xdlahqr.h"
#include "xnrm2.h"
#include "xzgehrd.h"
#include "xzlascl.h"
#include <algorithm>
#include <cmath>

// Function Definitions
namespace coder {
void eigStandard(const double A[9], creal_T V[3])
{
  double b_A[9];
  double absxk;
  double anrm;
  int k;
  boolean_T converged;
  boolean_T exitg1;
  std::copy(&A[0], &A[9], &b_A[0]);
  anrm = 0.0;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 9)) {
    absxk = std::abs(A[k]);
    if (std::isnan(absxk)) {
      anrm = rtNaN;
      exitg1 = true;
    } else {
      if (absxk > anrm) {
        anrm = absxk;
      }
      k++;
    }
  }
  if (std::isinf(anrm) || std::isnan(anrm)) {
    V[0].re = rtNaN;
    V[0].im = 0.0;
    V[1].re = rtNaN;
    V[1].im = 0.0;
    V[2].re = rtNaN;
    V[2].im = 0.0;
  } else {
    double wi[3];
    double wr[3];
    double cscale;
    int exitg5;
    int i;
    int ira;
    int ix;
    int ix0_tmp;
    int kend;
    int l;
    boolean_T notdone;
    boolean_T scalea;
    cscale = anrm;
    scalea = false;
    if ((anrm > 0.0) && (anrm < 6.7178761075670888E-139)) {
      scalea = true;
      cscale = 6.7178761075670888E-139;
      internal::reflapack::xzlascl(anrm, cscale, b_A);
    } else if (anrm > 1.4885657073574029E+138) {
      scalea = true;
      cscale = 1.4885657073574029E+138;
      internal::reflapack::xzlascl(anrm, cscale, b_A);
    }
    wr[0] = 1.0;
    wr[1] = 1.0;
    wr[2] = 1.0;
    k = 1;
    l = 3;
    notdone = true;
    do {
      exitg5 = 0;
      if (notdone) {
        int exitg4;
        notdone = false;
        ira = l;
        do {
          exitg4 = 0;
          if (ira > 0) {
            boolean_T exitg6;
            converged = false;
            i = 0;
            exitg6 = false;
            while ((!exitg6) && (i <= l - 1)) {
              if ((i + 1 == ira) || (!(b_A[(ira + 3 * i) - 1] != 0.0))) {
                i++;
              } else {
                converged = true;
                exitg6 = true;
              }
            }
            if (converged) {
              ira--;
            } else {
              wr[l - 1] = ira;
              if (ira != l) {
                ix = (ira - 1) * 3;
                kend = (l - 1) * 3;
                for (int b_k{0}; b_k < l; b_k++) {
                  ix0_tmp = ix + b_k;
                  absxk = b_A[ix0_tmp];
                  i = kend + b_k;
                  b_A[ix0_tmp] = b_A[i];
                  b_A[i] = absxk;
                }
                absxk = b_A[ira - 1];
                b_A[ira - 1] = b_A[l - 1];
                b_A[l - 1] = absxk;
                absxk = b_A[ira + 2];
                b_A[ira + 2] = b_A[l + 2];
                b_A[l + 2] = absxk;
                absxk = b_A[ira + 5];
                b_A[ira + 5] = b_A[l + 5];
                b_A[l + 5] = absxk;
              }
              exitg4 = 1;
            }
          } else {
            exitg4 = 2;
          }
        } while (exitg4 == 0);
        if (exitg4 == 1) {
          if (l == 1) {
            k = 1;
            l = 1;
            exitg5 = 1;
          } else {
            l--;
            notdone = true;
          }
        }
      } else {
        notdone = true;
        while (notdone) {
          boolean_T exitg6;
          notdone = false;
          ira = k;
          exitg6 = false;
          while ((!exitg6) && (ira <= l)) {
            boolean_T exitg7;
            converged = false;
            i = k;
            exitg7 = false;
            while ((!exitg7) && (i <= l)) {
              if ((i == ira) || (!(b_A[(i + 3 * (ira - 1)) - 1] != 0.0))) {
                i++;
              } else {
                converged = true;
                exitg7 = true;
              }
            }
            if (converged) {
              ira++;
            } else {
              wr[k - 1] = ira;
              if (ira != k) {
                ix = (ira - 1) * 3;
                kend = (k - 1) * 3;
                for (int b_k{0}; b_k < l; b_k++) {
                  ix0_tmp = ix + b_k;
                  absxk = b_A[ix0_tmp];
                  i = kend + b_k;
                  b_A[ix0_tmp] = b_A[i];
                  b_A[i] = absxk;
                }
                ix = (kend + ira) - 1;
                kend = (kend + k) - 1;
                ira = static_cast<unsigned char>(4 - k);
                for (int b_k{0}; b_k < ira; b_k++) {
                  ix0_tmp = ix + b_k * 3;
                  absxk = b_A[ix0_tmp];
                  i = kend + b_k * 3;
                  b_A[ix0_tmp] = b_A[i];
                  b_A[i] = absxk;
                }
              }
              k++;
              notdone = true;
              exitg6 = true;
            }
          }
        }
        converged = false;
        exitg5 = 2;
      }
    } while (exitg5 == 0);
    if (exitg5 != 1) {
      exitg1 = false;
      while ((!exitg1) && (!converged)) {
        int exitg3;
        converged = true;
        i = k - 1;
        do {
          exitg3 = 0;
          if (i + 1 <= l) {
            double c;
            double ca;
            double r;
            double scale;
            double t;
            kend = (l - k) + 1;
            c = internal::blas::xnrm2(kend, b_A, i * 3 + k);
            ix0_tmp = (k - 1) * 3 + i;
            r = 0.0;
            if (kend >= 1) {
              if (kend == 1) {
                r = std::abs(b_A[ix0_tmp]);
              } else {
                scale = 3.3121686421112381E-170;
                kend = (ix0_tmp + (kend - 1) * 3) + 1;
                for (int b_k{ix0_tmp + 1}; b_k <= kend; b_k += 3) {
                  absxk = std::abs(b_A[b_k - 1]);
                  if (absxk > scale) {
                    t = scale / absxk;
                    r = r * t * t + 1.0;
                    scale = absxk;
                  } else {
                    t = absxk / scale;
                    r += t * t;
                  }
                }
                r = scale * std::sqrt(r);
              }
            }
            ix = i * 3;
            kend = 1;
            if (l > 1) {
              absxk = std::abs(b_A[ix]);
              for (int b_k{2}; b_k <= l; b_k++) {
                t = std::abs(b_A[(ix + b_k) - 1]);
                if (t > absxk) {
                  kend = b_k;
                  absxk = t;
                }
              }
            }
            ca = std::abs(b_A[(kend + 3 * i) - 1]);
            kend = 4 - k;
            if (4 - k < 1) {
              ira = 0;
            } else {
              ira = 1;
              if (4 - k > 1) {
                absxk = std::abs(b_A[ix0_tmp]);
                for (int b_k{2}; b_k <= kend; b_k++) {
                  t = std::abs(b_A[ix0_tmp + (b_k - 1) * 3]);
                  if (t > absxk) {
                    ira = b_k;
                    absxk = t;
                  }
                }
              }
            }
            absxk = std::abs(b_A[i + 3 * ((ira + k) - 2)]);
            if ((c == 0.0) || (r == 0.0)) {
              i++;
            } else {
              double f;
              int exitg2;
              scale = r / 2.0;
              f = 1.0;
              t = c + r;
              do {
                exitg2 = 0;
                if ((c < scale) &&
                    (std::fmax(f, std::fmax(c, ca)) < 4.9896007738368E+291) &&
                    (std::fmin(r, std::fmin(scale, absxk)) >
                     2.0041683600089728E-292)) {
                  if (std::isnan(((((c + f) + ca) + r) + scale) + absxk)) {
                    exitg2 = 1;
                  } else {
                    f *= 2.0;
                    c *= 2.0;
                    ca *= 2.0;
                    r /= 2.0;
                    scale /= 2.0;
                    absxk /= 2.0;
                  }
                } else {
                  scale = c / 2.0;
                  while ((scale >= r) &&
                         (std::fmax(r, absxk) < 4.9896007738368E+291) &&
                         (std::fmin(std::fmin(f, c), std::fmin(scale, ca)) >
                          2.0041683600089728E-292)) {
                    f /= 2.0;
                    c /= 2.0;
                    scale /= 2.0;
                    ca /= 2.0;
                    r *= 2.0;
                    absxk *= 2.0;
                  }
                  if ((!(c + r >= 0.95 * t)) &&
                      ((!(f < 1.0)) || (!(wr[i] < 1.0)) ||
                       (!(f * wr[i] <= 1.0020841800044864E-292))) &&
                      ((!(f > 1.0)) || (!(wr[i] > 1.0)) ||
                       (!(wr[i] >= 9.9792015476736E+291 / f)))) {
                    absxk = 1.0 / f;
                    wr[i] *= f;
                    kend = ix0_tmp + 1;
                    ira = (ix0_tmp + 3 * (3 - k)) + 1;
                    for (int b_k{kend}; b_k <= ira; b_k += 3) {
                      b_A[b_k - 1] *= absxk;
                    }
                    ira = ix + l;
                    for (int b_k{ix + 1}; b_k <= ira; b_k++) {
                      b_A[b_k - 1] *= f;
                    }
                    converged = false;
                  }
                  exitg2 = 2;
                }
              } while (exitg2 == 0);
              if (exitg2 == 1) {
                exitg3 = 2;
              } else {
                i++;
              }
            }
          } else {
            exitg3 = 1;
          }
        } while (exitg3 == 0);
        if (exitg3 != 1) {
          exitg1 = true;
        }
      }
    }
    internal::reflapack::xzgehrd(b_A, k, l);
    kend = internal::reflapack::xdlahqr(k, l, b_A, absxk, wr, wi);
    if (scalea) {
      internal::reflapack::xzlascl(cscale, anrm, 3 - kend, wr, kend + 1);
      internal::reflapack::xzlascl(cscale, anrm, 3 - kend, wi, kend + 1);
      if (kend != 0) {
        internal::reflapack::xzlascl(cscale, anrm, k - 1, wr, 1);
        internal::reflapack::xzlascl(cscale, anrm, k - 1, wi, 1);
      }
    }
    if (kend != 0) {
      for (i = k; i <= kend; i++) {
        wr[i - 1] = rtNaN;
        wi[i - 1] = 0.0;
      }
    }
    V[0].re = wr[0];
    V[0].im = wi[0];
    V[1].re = wr[1];
    V[1].im = wi[1];
    V[2].re = wr[2];
    V[2].im = wi[2];
  }
}

} // namespace coder

// End of code generation (eigStandard.cpp)
