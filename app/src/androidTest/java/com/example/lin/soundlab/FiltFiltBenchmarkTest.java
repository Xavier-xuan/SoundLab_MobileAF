package com.example.lin.soundlab;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.example.lin.soundlab.algorithm.util.FiltfiltOrderThreeButterC;
import com.example.lin.soundlab.algorithm.util.FiltFiltJava;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Random;

@RunWith(AndroidJUnit4.class)
public class FiltFiltBenchmarkTest {

    private static final int REPEAT_TIMES = 10;

    @Test
    public void testFiltFiltPerformance() {
        int[] sizes = {4800 * 3, 4800 * 3, 4800 * 3, 4800 * 3}; // 4轮测试
        int filterOrder = 3;
        double cutoff = 50;
        double sampleRate = 48000;

        FiltfiltOrderThreeButterC jniFilter = new FiltfiltOrderThreeButterC();

        System.out.println("=== FiltFilt Butterworth JNI vs Java Benchmark ===");

        for (int size : sizes) {
            double[] input = generateSignal(size);

            // 预热
            FiltFiltJava.filtfiltButterworth(input, filterOrder, cutoff, sampleRate);
            jniFilter.run(input, cutoff, sampleRate);

            double totalTimeJava = 0.0;
            double totalTimeJNI = 0.0;
            double totalSumJava = 0.0;
            double totalSumJNI = 0.0;
            double totalMSE = 0.0;
            double maxDiff = 0.0;

            for (int i = 0; i < REPEAT_TIMES; i++) {
                // Java 实现计时
                long startJava = System.nanoTime();
                double[] outJava = FiltFiltJava.filtfiltButterworth(input, filterOrder, cutoff, sampleRate);
                long endJava = System.nanoTime();

                // JNI 实现计时
                long startJNI = System.nanoTime();
                double[] outJNI = jniFilter.run(input, cutoff, sampleRate);
                long endJNI = System.nanoTime();

                totalTimeJava += (endJava - startJava) / 1e6;
                totalTimeJNI += (endJNI - startJNI) / 1e6;

                double sumJava = 0.0;
                double sumJNI = 0.0;
                double mse = 0.0;
                for (int j = 0; j < outJava.length; j++) {
                    sumJava += outJava[j];
                    sumJNI += outJNI[j];
                    double diff = outJava[j] - outJNI[j];
                    mse += diff * diff;
                    maxDiff = Math.max(maxDiff, Math.abs(diff));
                }
                totalSumJava += sumJava;
                totalSumJNI += sumJNI;
                totalMSE += mse / outJava.length;
            }

            System.out.printf("Size: %6d | Java Avg: %7.2f ms | JNI Avg: %7.2f ms | " +
                            "Sum Δ: %8.3f | MSE: %.2e | MaxDiff: %.2e\n",
                    size,
                    totalTimeJava / REPEAT_TIMES,
                    totalTimeJNI / REPEAT_TIMES,
                    Math.abs(totalSumJava / REPEAT_TIMES - totalSumJNI / REPEAT_TIMES),
                    totalMSE / REPEAT_TIMES,
                    maxDiff);
        }
    }

    private double[] generateSignal(int size) {
        double[] signal = new double[size];
        Random rand = new Random(42);
        for (int i = 0; i < size; i++) {
            signal[i] = Math.sin(2 * Math.PI * 1000 * i / 48000.0) + 0.1 * (rand.nextDouble() - 0.5);
        }
        return signal;
    }
}
