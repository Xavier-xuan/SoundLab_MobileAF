//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// xdsterf.cpp
//
// Code generation for function 'xdsterf'
//

// Include files
#include "xdsterf.h"
#include "FiltfiltOrderThreeButter_rtwutil.h"
#include "insertionsort.h"
#include "rt_nonfinite.h"
#include "xdlaev2.h"
#include "xzlascl.h"
#include <cmath>

// Function Definitions
namespace coder {
namespace internal {
namespace reflapack {
int xdsterf(double d[3], double e[2])
{
  double b_d;
  int info;
  int jtot;
  int l1;
  info = 0;
  jtot = 0;
  l1 = 1;
  int exitg1;
  do {
    exitg1 = 0;
    if (l1 > 3) {
      insertionsort(d);
      exitg1 = 1;
    } else {
      int l;
      int lend;
      int lendsv_tmp;
      int lsv;
      int m;
      boolean_T exitg2;
      if (l1 > 1) {
        e[l1 - 2] = 0.0;
      }
      m = l1;
      exitg2 = false;
      while ((!exitg2) && (m < 3)) {
        if (std::abs(e[m - 1]) <= std::sqrt(std::abs(d[m - 1])) *
                                      std::sqrt(std::abs(d[m])) *
                                      2.2204460492503131E-16) {
          e[m - 1] = 0.0;
          exitg2 = true;
        } else {
          m++;
        }
      }
      l = l1;
      lsv = l1;
      lend = m;
      lendsv_tmp = m + 1;
      l1 = m + 1;
      if (m != l) {
        double anorm;
        double b_anorm;
        int anorm_tmp;
        int i;
        int n_tmp;
        n_tmp = m - l;
        if (n_tmp + 1 <= 0) {
          anorm = 0.0;
        } else {
          anorm = std::abs(d[(l + n_tmp) - 1]);
          i = -1;
          exitg2 = false;
          while ((!exitg2) && (i + 1 <= n_tmp - 1)) {
            anorm_tmp = l + i;
            b_anorm = std::abs(d[anorm_tmp]);
            if (std::isnan(b_anorm)) {
              anorm = rtNaN;
              exitg2 = true;
            } else {
              if (b_anorm > anorm) {
                anorm = b_anorm;
              }
              b_anorm = std::abs(e[anorm_tmp]);
              if (std::isnan(b_anorm)) {
                anorm = rtNaN;
                exitg2 = true;
              } else {
                if (b_anorm > anorm) {
                  anorm = b_anorm;
                }
                i++;
              }
            }
          }
        }
        if (!(anorm == 0.0)) {
          int iscale;
          iscale = 0;
          if (anorm > 2.2346346549904327E+153) {
            iscale = 1;
            xzlascl(anorm, 2.2346346549904327E+153, n_tmp + 1, d, l);
            b_xzlascl(anorm, 2.2346346549904327E+153, n_tmp, e, l);
          } else if (anorm < 3.02546243347603E-123) {
            iscale = 2;
            xzlascl(anorm, 3.02546243347603E-123, n_tmp + 1, d, l);
            b_xzlascl(anorm, 3.02546243347603E-123, n_tmp, e, l);
          }
          anorm_tmp = m - 1;
          for (i = l; i <= anorm_tmp; i++) {
            b_anorm = e[i - 1];
            e[i - 1] = b_anorm * b_anorm;
          }
          if (std::abs(d[m - 1]) < std::abs(d[l - 1])) {
            lend = lsv;
            l = m;
          }
          if (lend >= l) {
            int exitg4;
            do {
              exitg4 = 0;
              if (l != lend) {
                m = l;
                while ((m < lend) &&
                       (!(std::abs(e[m - 1]) <= 4.9303806576313238E-32 *
                                                    std::abs(d[m - 1]) *
                                                    std::abs(d[m])))) {
                  m++;
                }
              } else {
                m = lend;
              }
              if (m < lend) {
                e[m - 1] = 0.0;
              }
              if (m == l) {
                l++;
                if (l > lend) {
                  exitg4 = 1;
                }
              } else if (m == l + 1) {
                d[l - 1] = xdlaev2(d[l - 1], std::sqrt(e[l - 1]), d[l], b_d);
                d[l] = b_d;
                e[l - 1] = 0.0;
                l += 2;
                if (l > lend) {
                  exitg4 = 1;
                }
              } else if (jtot == 90) {
                exitg4 = 1;
              } else {
                double b_gamma;
                double c;
                double r;
                double s;
                double sigma;
                double x;
                jtot++;
                b_anorm = std::sqrt(e[l - 1]);
                r = d[l - 1];
                sigma = (d[l] - r) / (2.0 * b_anorm);
                x = rt_hypotd_snf(sigma, 1.0);
                if (!(sigma >= 0.0)) {
                  x = -x;
                }
                sigma = r - b_anorm / (sigma + x);
                c = 1.0;
                s = 0.0;
                b_gamma = d[m - 1] - sigma;
                b_anorm = b_gamma * b_gamma;
                anorm_tmp = m - 1;
                for (i = anorm_tmp; i >= l; i--) {
                  double oldc;
                  b_d = e[i - 1];
                  r = b_anorm + b_d;
                  if (i != m - 1) {
                    e[1] = s * r;
                  }
                  oldc = c;
                  c = b_anorm / r;
                  s = b_d / r;
                  b_anorm = b_gamma;
                  x = d[i - 1];
                  b_gamma = c * (x - sigma) - s * b_gamma;
                  d[i] = b_anorm + (x - b_gamma);
                  if (c != 0.0) {
                    b_anorm = b_gamma * b_gamma / c;
                  } else {
                    b_anorm = oldc * b_d;
                  }
                }
                e[l - 1] = s * b_anorm;
                d[l - 1] = sigma + b_gamma;
              }
            } while (exitg4 == 0);
          } else {
            int exitg3;
            do {
              exitg3 = 0;
              m = l;
              while ((m > lend) &&
                     (!(std::abs(e[m - 2]) <= 4.9303806576313238E-32 *
                                                  std::abs(d[m - 1]) *
                                                  std::abs(d[m - 2])))) {
                m--;
              }
              if (m > lend) {
                e[m - 2] = 0.0;
              }
              if (m == l) {
                l--;
                if (l < lend) {
                  exitg3 = 1;
                }
              } else if (m == l - 1) {
                d[l - 1] =
                    xdlaev2(d[l - 1], std::sqrt(e[l - 2]), d[l - 2], b_d);
                d[l - 2] = b_d;
                e[l - 2] = 0.0;
                l -= 2;
                if (l < lend) {
                  exitg3 = 1;
                }
              } else if (jtot == 90) {
                exitg3 = 1;
              } else {
                double b_gamma;
                double c;
                double r;
                double s;
                double sigma;
                double x;
                jtot++;
                b_anorm = std::sqrt(e[l - 2]);
                r = d[l - 1];
                sigma = (d[l - 2] - r) / (2.0 * b_anorm);
                x = rt_hypotd_snf(sigma, 1.0);
                if (!(sigma >= 0.0)) {
                  x = -x;
                }
                sigma = r - b_anorm / (sigma + x);
                c = 1.0;
                s = 0.0;
                b_gamma = d[m - 1] - sigma;
                b_anorm = b_gamma * b_gamma;
                anorm_tmp = l - 1;
                for (i = m; i <= anorm_tmp; i++) {
                  double oldc;
                  b_d = e[i - 1];
                  r = b_anorm + b_d;
                  if (i != m) {
                    e[0] = s * r;
                  }
                  oldc = c;
                  c = b_anorm / r;
                  s = b_d / r;
                  b_anorm = b_gamma;
                  b_gamma = c * (d[i] - sigma) - s * b_gamma;
                  d[i - 1] = b_anorm + (d[i] - b_gamma);
                  if (c != 0.0) {
                    b_anorm = b_gamma * b_gamma / c;
                  } else {
                    b_anorm = oldc * b_d;
                  }
                }
                e[l - 2] = s * b_anorm;
                d[l - 1] = sigma + b_gamma;
              }
            } while (exitg3 == 0);
          }
          if (iscale == 1) {
            xzlascl(2.2346346549904327E+153, anorm, lendsv_tmp - lsv, d, lsv);
          } else if (iscale == 2) {
            xzlascl(3.02546243347603E-123, anorm, lendsv_tmp - lsv, d, lsv);
          }
          if (jtot >= 90) {
            if (e[0] != 0.0) {
              info = 1;
            }
            if (e[1] != 0.0) {
              info++;
            }
            exitg1 = 1;
          }
        }
      }
    }
  } while (exitg1 == 0);
  return info;
}

} // namespace reflapack
} // namespace internal
} // namespace coder

// End of code generation (xdsterf.cpp)
