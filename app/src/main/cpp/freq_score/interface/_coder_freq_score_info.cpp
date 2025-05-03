//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// _coder_freq_score_info.cpp
//
// Code generation for function 'freq_score'
//

// Include files
#include "_coder_freq_score_info.h"
#include "emlrt.h"
#include "tmwtypes.h"

// Function Declarations
static const mxArray *emlrtMexFcnResolvedFunctionsInfo();

// Function Definitions
static const mxArray *emlrtMexFcnResolvedFunctionsInfo()
{
  const mxArray *nameCaptureInfo;
  const char_T *data[5]{
      "789cc554bb4e0241143d6b5069402cec6d6c0d8d09b13346120a048584a86b107687ec86"
      "7db10cafcec4c4d6d6f829c6cecfb0f00ffc06ef3e79c4099b10e16e"
      "e0dc33e7ce9ce1ee0d904a6509401641147603cc843c17e216e66351970418c5365273fb"
      "22fd2544c5b6381bf380582d93c53b55dbd4ad96c5eb1387c1657ddb",
      "1832d5573abac1eabac96ab3e4d2636671468a892779f9b9c6946e6d60c2d5fad31b1ab3"
      "24eec7a1f4f7ef4d25ecc7bda01ffb21be8778870baabdc52964284f"
      "784cd7e5cae4f56728c3848d3674186068e20c45aa68c1a1a749ab26a1a798f4b1c049e1"
      "b46a532ea38701718338c784aa5be8d3c3fcefa85e46072ee53dd2fb",
      "5068a7c78e49977ce69d3fa03acfdb213ea2cca5dc8d9de6fbe508fa91b45fd9058e85ba"
      "d867c40c456baa8cbbcc52a7eb0f2bfaef08fd0345b5076d834dfdb0"
      "e27c94057ed17cbc85b8b9f970fc776ed0346854a352cefd19b128f7e6248865fddf4bd8"
      "0fd1ff4706691f8f3ebea475fa7d3f173ed7e917c5a6fcc682f392ce",
      "f381c02fb7a0972a8d2ebf6e9ce4af58f9a65d56f2559d1567ee515de2b3ec1e10f0ff3e"
      "ff1751a48b49",
      ""};
  nameCaptureInfo = nullptr;
  emlrtNameCaptureMxArrayR2016a(&data[0], 1872U, &nameCaptureInfo);
  return nameCaptureInfo;
}

mxArray *emlrtMexFcnProperties()
{
  mxArray *xEntryPoints;
  mxArray *xInputs;
  mxArray *xResult;
  const char_T *propFieldName[9]{"Version",
                                 "ResolvedFunctions",
                                 "Checksum",
                                 "EntryPoints",
                                 "CoverageInfo",
                                 "IsPolymorphic",
                                 "PropertyList",
                                 "UUID",
                                 "ClassEntryPointIsHandle"};
  const char_T *epFieldName[8]{
      "Name",     "NumberOfInputs", "NumberOfOutputs", "ConstantInputs",
      "FullPath", "TimeStamp",      "Constructor",     "Visible"};
  xEntryPoints =
      emlrtCreateStructMatrix(1, 1, 8, (const char_T **)&epFieldName[0]);
  xInputs = emlrtCreateLogicalMatrix(1, 2);
  emlrtSetField(xEntryPoints, 0, "Name", emlrtMxCreateString("freq_score"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs",
                emlrtMxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs",
                emlrtMxCreateDoubleScalar(2.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  emlrtSetField(
      xEntryPoints, 0, "FullPath",
      emlrtMxCreateString(
          "Z:"
          "\\\xe8\x8d\xa3\xe8\x80\x80\xe5\x90\x88\xe4\xbd\x9c\xe9\xa1\xb9\xe7"
          "\x9b\xae\\mobile_AF\\app_implementation\\quality_assessment\\"
          "freq_score.m"));
  emlrtSetField(xEntryPoints, 0, "TimeStamp",
                emlrtMxCreateDoubleScalar(739740.8442708333));
  emlrtSetField(xEntryPoints, 0, "Constructor",
                emlrtMxCreateLogicalScalar(false));
  emlrtSetField(xEntryPoints, 0, "Visible", emlrtMxCreateLogicalScalar(true));
  xResult =
      emlrtCreateStructMatrix(1, 1, 9, (const char_T **)&propFieldName[0]);
  emlrtSetField(xResult, 0, "Version",
                emlrtMxCreateString("23.2.0.2485118 (R2023b) Update 6"));
  emlrtSetField(xResult, 0, "ResolvedFunctions",
                (mxArray *)emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "Checksum",
                emlrtMxCreateString("AU6O9lPR3JlqIbl2wn07W"));
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

// End of code generation (_coder_freq_score_info.cpp)
