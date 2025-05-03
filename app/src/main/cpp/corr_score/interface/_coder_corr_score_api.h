//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_corr_score_api.h
//
// Code generation for function 'corr_score'
//

#ifndef _CODER_CORR_SCORE_API_H
#define _CODER_CORR_SCORE_API_H

// Include files
#include "coder_array_mex.h"
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"
#include <algorithm>
#include <cstring>

// Variable Declarations
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

// Function Declarations
real_T corr_score(coder::array<real_T, 2U> *signals);

void corr_score_api(const mxArray *prhs, const mxArray **plhs);

void corr_score_atexit();

void corr_score_initialize();

void corr_score_terminate();

void corr_score_xil_shutdown();

void corr_score_xil_terminate();

#endif
// End of code generation (_coder_corr_score_api.h)
