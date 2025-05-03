package com.example.lin.soundlab.algorithm.util;

import uk.me.berndporr.iirj.Butterworth;

public class FiltFilt {

    // 反转数组
    public static double[] reverse(double[] input) {
        double[] reversed = new double[input.length];
        for (int i = 0; i < input.length; i++) {
            reversed[i] = input[input.length - 1 - i];
        }
        return reversed;
    }

    // 用 butterworth 滤波器做 filtfilt
    public static double[] filtfiltButterworth(double[] data, int order, double cutoff, double sampleRate) {
        Butterworth forwardFilter =  new Butterworth();
        forwardFilter.lowPass(order, sampleRate, cutoff);
        Butterworth backwardFilter = new Butterworth();
        backwardFilter.lowPass(order, sampleRate, cutoff);

        // 前向滤波
        double[] forward = new double[data.length];
        for (int i = 0; i < data.length; i++) {
            forward[i] = forwardFilter.filter(data[i]);
        }

        // 反转 + 再次滤波
        double[] reversed = reverse(forward);
        double[] backward = new double[data.length];
        for (int i = 0; i < reversed.length; i++) {
            backward[i] = backwardFilter.filter(reversed[i]);
        }

        // 最终结果反转回来
        return reverse(backward);
    }

}
