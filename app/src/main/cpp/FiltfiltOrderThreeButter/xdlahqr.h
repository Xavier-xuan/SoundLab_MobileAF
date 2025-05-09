//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// xdlahqr.h
//
// Code generation for function 'xdlahqr'
//

#ifndef XDLAHQR_H
#define XDLAHQR_H

// Include files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
namespace reflapack {
int xdlahqr(int ilo, int ihi, double h[9], double &z, double wr[3],
            double wi[3]);

}
} // namespace internal
} // namespace coder

#endif
// End of code generation (xdlahqr.h)
