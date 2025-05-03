package com.example.lin.soundlab.algorithm;

public class CorrScore {
    static {
        System.loadLibrary("soundlab");  // 这里的名字要和你的 CMakeLists 中一致
    }

    public native double run(double[] signals, int numChannels, int sigLength);
}
