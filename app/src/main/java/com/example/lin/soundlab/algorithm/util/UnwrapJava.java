package com.example.lin.soundlab.algorithm.util;

public class UnwrapJava {
    /**
     * unwrap(rawPhase * 2) / 2 的 Java 实现
     * @param rawPhase 输入原始相位序列（单位：弧度）
     * @return 展开的相位
     */
    public static double[] unwrapHalfPhase(double[] rawPhase) {
        double[] scaled = new double[rawPhase.length];
        for (int i = 0; i < rawPhase.length; i++) {
            scaled[i] = rawPhase[i] * 2;
        }

        double[] unwrapped = unwrap(scaled);  // 调用标准 unwrap
        for (int i = 0; i < unwrapped.length; i++) {
            unwrapped[i] /= 2;
        }

        return unwrapped;
    }

    /**
     * unwrap 相位跳跃（标准 unwrap，单位弧度）
     * @param phase 输入的相位序列
     * @return 展开后的相位序列
     */
    public static double[] unwrap(double[] phase) {
        double[] unwrapped = new double[phase.length];
        if (phase.length == 0) return unwrapped;

        unwrapped[0] = phase[0];
        double prev = phase[0];
        double TWO_PI = 2.0 * Math.PI;

        for (int i = 1; i < phase.length; i++) {
            double delta = phase[i] - prev;
            if (delta > Math.PI) {
                delta -= TWO_PI;
            } else if (delta < -Math.PI) {
                delta += TWO_PI;
            }
            unwrapped[i] = unwrapped[i - 1] + delta;
            prev = phase[i];
        }

        return unwrapped;
    }
}
