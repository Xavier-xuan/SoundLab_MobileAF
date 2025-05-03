//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// freq_score.h
//
// Code generation for function 'freq_score'
//

#ifndef FREQ_SCORE_H
#define FREQ_SCORE_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void freq_score(const coder::array<double, 2U> &signals, double fs,
                       double *final_score, double total_socres_data[],
                       int total_socres_size[2]);

#endif
// End of code generation (freq_score.h)
