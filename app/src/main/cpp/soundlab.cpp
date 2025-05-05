#include <jni.h>
#include <string>
#include "freq_score/freq_score.h"
#include "freq_score/freq_score_terminate.h"
#include "freq_score/freq_score_initialize.h"
#include "freq_score/coder_array.h"
#include "corr_score/corr_score.h"
#include "corr_score/coder_array.h"
#include "corr_score/corr_score_initialize.h"
#include "corr_score/corr_score_terminate.h"
#include "FiltfiltOrderThreeButter/FiltfiltOrderThreeButter.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_example_lin_soundlab_algorithm_FreqScoreC_run(JNIEnv *env, jobject, jdoubleArray jSignals, jint numChannels, jint sigLength, jint fs) {
    // 1. 获取数组长度，确认是否匹配
    jsize inputLen = env->GetArrayLength(jSignals);
    if (inputLen != numChannels * sigLength) {
        return -1.0f; // 输入大小不匹配
    }

    jdouble *sigData = env->GetDoubleArrayElements(jSignals, nullptr);
    if (!sigData) return -1.0f;

    // 2. 初始化 coder::array
    coder::array<double, 2U> signals;
    signals.set_size(numChannels, sigLength);

    for (int ch = 0; ch < numChannels; ++ch) {
        for (int i = 0; i < sigLength; ++i) {
            signals[ch + numChannels * i] = static_cast<double>(sigData[ch * sigLength + i]);
        }
    }

    // 3. 初始化 + 运行 freq_score
    freq_score_initialize();
    double final_score;
    double total_scores[16]; // 最多支持 16 通道，如果你希望输出也动态返回，请扩展接口
    int total_scores_size[2];

    freq_score(signals, static_cast<double>(fs), &final_score, total_scores, total_scores_size);
    freq_score_terminate();

    env->ReleaseDoubleArrayElements(jSignals, sigData, 0);
    return final_score;
}


extern "C"
JNIEXPORT jdouble JNICALL
Java_com_example_lin_soundlab_algorithm_CorrScoreC_run(JNIEnv *env, jobject, jdoubleArray jSignals, jint numChannels, jint sigLength) {
    // 总长度校验
    jsize inputLen = env->GetArrayLength(jSignals);
    if (inputLen != numChannels * sigLength) {
        return -1.0;  // 输入错误
    }

    jdouble* inputData = env->GetDoubleArrayElements(jSignals, nullptr);
    if (!inputData) return -1.0;

    // 构造 coder::array<double, 2>，列主序（Column-major）
    coder::array<double, 2U> signals;
    signals.set_size(numChannels, sigLength);  // M x N

    for (int ch = 0; ch < numChannels; ++ch) {
        for (int i = 0; i < sigLength; ++i) {
            signals[ch + numChannels * i] = inputData[ch * sigLength + i];  // 转置布局
        }
    }

    // 初始化 MATLAB runtime
    corr_score_initialize();

    // 执行函数
    double result = corr_score(signals);

    // 清理
    corr_score_terminate();
    env->ReleaseDoubleArrayElements(jSignals, inputData, 0);

    return result;
}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_lin_soundlab_algorithm_util_FiltfiltOrderThreeButterC_run(
        JNIEnv *env,
        jclass /* this */,
        jdoubleArray inputArray,
        jdouble cutoffFreq,
        jdouble fs) {

    // Step 1: 获取输入数组长度
    jsize length = env->GetArrayLength(inputArray);
    if (length == 0) return nullptr;

    // Step 2: 将 jdoubleArray 转换为 coder::array<double, 2U> 的 [1, N] 矩阵
    jdouble *elements = env->GetDoubleArrayElements(inputArray, nullptr);

    coder::array<double, 2U> x;
    x.set_size(1, length);  // 1 行 N 列

    for (jsize i = 0; i < length; ++i) {
        x[i] = elements[i];
    }

    env->ReleaseDoubleArrayElements(inputArray, elements, JNI_ABORT);

    // Step 3: 输出结果
    coder::array<double, 2U> result;
    FiltfiltOrderThreeButter(x, cutoffFreq, fs, result);

    // Step 4: 将输出 result 转换为 jdoubleArray 返回（仍为一维 double[]）
    jdoubleArray output = env->NewDoubleArray(result.size(1));  // 输出为 [1 x N]
    if (output == nullptr) return nullptr;

    env->SetDoubleArrayRegion(output, 0, result.size(1), result.data());

    return output;
}