//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_freq_score_api.h
//
// Code generation for function 'freq_score'
//

#ifndef _CODER_FREQ_SCORE_API_H
#define _CODER_FREQ_SCORE_API_H

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
void freq_score(coder::array<real_T, 2U> *signals, real_T fs,
                real_T *final_score, real_T total_socres_data[],
                int32_T total_socres_size[2]);

void freq_score_api(const mxArray *const prhs[2], int32_T nlhs,
                    const mxArray *plhs[2]);

void freq_score_atexit();

void freq_score_initialize();

void freq_score_terminate();

void freq_score_xil_shutdown();

void freq_score_xil_terminate();

#endif
// End of code generation (_coder_freq_score_api.h)
