package com.example.lin.soundlab.algorithm.util;

public class FiltfiltOrderThreeButterC {
    static {
        System.loadLibrary("soundlab");
    }

    // 输入一维数组，返回滤波后的结果
    public static native double[] run(double[] x, double cutoffFreq, double fs);
}
