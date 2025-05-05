package com.example.lin.soundlab.algorithm;


import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class PulseWaveExtractor4Ch {
    private final IQDemodulator[] demodulators;
    private final int channelCount = 4;
    private final ExecutorService executor;

    /**
     * 构造函数
     *
     * @param sampleRate    采样率
     * @param carrierFreqs  四个通道的载波频率（长度必须为 4）
     * @param cutoffFreq    滤波器截止频率
     * @param bufferLength  每个 buffer 的长度
     */
    public PulseWaveExtractor4Ch(double sampleRate, double[] carrierFreqs, double cutoffFreq, int bufferLength) {
        if (carrierFreqs.length != channelCount) {
            throw new IllegalArgumentException("carrierFreqs must contain exactly 4 elements.");
        }

        demodulators = new IQDemodulator[channelCount];
        for (int i = 0; i < channelCount; i++) {
            demodulators[i] = new IQDemodulator(sampleRate, carrierFreqs[i], cutoffFreq, bufferLength);
        }

        // 创建固定大小线程池（4个线程，分别对应4个通道）
        executor = Executors.newFixedThreadPool(channelCount);
    }

    /**
     * 对同一段原始信号进行 4 通道并发解调
     *
     * @param rawInput 原始采样信号（包含四个载波）
     * @return 4 通道解调后的相位数组
     * @throws InterruptedException
     * @throws ExecutionException
     */
    public double[][] extractAllChannels(double[] rawInput) throws InterruptedException, ExecutionException {
        List<Callable<double[]>> tasks = new ArrayList<>();

        for (int i = 0; i < channelCount; i++) {
            final int index = i;
            tasks.add(() -> demodulators[index].demodulateStream(rawInput));
        }

        List<Future<double[]>> futures = executor.invokeAll(tasks);

        double[][] result = new double[channelCount][];
        for (int i = 0; i < channelCount; i++) {
            result[i] = futures.get(i).get();  // 获取解调结果
        }

        return result;
    }

    /**
     * 重置所有通道
     */
    public void reset() {
        for (IQDemodulator demod : demodulators) {
            demod.reset();
        }
    }

    /**
     * 应用结束时手动关闭线程池
     */
    public void shutdown() {
        executor.shutdown();
    }
}
