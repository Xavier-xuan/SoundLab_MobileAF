package com.example.lin.soundlab.algorithm;

import com.example.lin.soundlab.algorithm.util.FiltFilt;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.linalg.factory.Nd4j;
import org.nd4j.linalg.api.ops.impl.transforms.strict.Cos;
import org.nd4j.linalg.api.ops.impl.transforms.strict.Sin;

import java.util.ArrayDeque;
import java.util.Deque;

public class IQDemodulator {

    private final double sampleRate;
    private final double carrierFreq;
    private final int filterOrder;
    private final double cutoffFreq;
    private final int bufferLength;

    // 缓存：前两段的 I_raw 和 Q_raw
    private final Deque<double[]> iCache = new ArrayDeque<>(2);
    private final Deque<double[]> qCache = new ArrayDeque<>(2);
    private long totalSampleIndex = 0;


    public IQDemodulator(double sampleRate, double carrierFreq, int filterOrder, double cutoffFreq, int bufferLength) {
        this.sampleRate = sampleRate;
        this.carrierFreq = carrierFreq;
        this.filterOrder = filterOrder;
        this.cutoffFreq = cutoffFreq;
        this.bufferLength = bufferLength;

        // 预热：I_raw/Q_raw 置为 0 段
        iCache.addLast(new double[bufferLength]);
        iCache.addLast(new double[bufferLength]);
        qCache.addLast(new double[bufferLength]);
        qCache.addLast(new double[bufferLength]);
    }

    /**
     * 输入当前段，输出上一个段的 demodulated 相位结果（从第1帧起输出有效数据）
     */
    public double[] demodulateStream(double[] currentInput) {
        if (currentInput.length != bufferLength) {
            throw new IllegalArgumentException("Input length must be " + bufferLength);
        }

        // 使用整数 sampleIndex 计算当前段的 cos/sin
        double[] I_now = new double[bufferLength];
        double[] Q_now = new double[bufferLength];

        for (int i = 0; i < bufferLength; i++) {
            double phase = 2 * Math.PI * carrierFreq * (totalSampleIndex + i) / sampleRate;
            I_now[i] = currentInput[i] * Math.cos(phase);
            Q_now[i] = currentInput[i] * Math.sin(phase);
        }

        // 拼接前两段缓存和当前段
        double[] I_merged = mergeThree(iCache.peekFirst(), iCache.peekLast(), I_now);
        double[] Q_merged = mergeThree(qCache.peekFirst(), qCache.peekLast(), Q_now);

        // 滤波
        double[] I_filt = FiltFilt.filtfiltButterworth(I_merged, filterOrder, cutoffFreq, sampleRate);
        double[] Q_filt = FiltFilt.filtfiltButterworth(Q_merged, filterOrder, cutoffFreq, sampleRate);

        // 只输出中间段（i-1）
        /*
          ATTENTION
          In our scenario (<40k points once), calculate in a single thread is much faster
          Do NOT calculate using Nd4j with multiple threads unless calculating >100k points each time
         */
        double[] result = new double[bufferLength];
        for (int i = 0; i < bufferLength; i++) {
            int idx = bufferLength + i; // 中间段起点
            result[i] = Math.atan2(Q_filt[idx], I_filt[idx]);
        }

        // 滚动缓存
        iCache.removeFirst();
        iCache.addLast(I_now);
        qCache.removeFirst();
        qCache.addLast(Q_now);

        totalSampleIndex += bufferLength;
        return result;
    }

    private double[] mergeThree(double[] a, double[] b, double[] c) {
        double[] merged = new double[a.length + b.length + c.length];
        System.arraycopy(a, 0, merged, 0, a.length);
        System.arraycopy(b, 0, merged, a.length, b.length);
        System.arraycopy(c, 0, merged, a.length + b.length, c.length);
        return merged;
    }

    public void reset() {
        iCache.clear();
        qCache.clear();
        iCache.addLast(new double[bufferLength]);
        iCache.addLast(new double[bufferLength]);
        qCache.addLast(new double[bufferLength]);
        qCache.addLast(new double[bufferLength]);
        totalSampleIndex = 0;
    }
}
