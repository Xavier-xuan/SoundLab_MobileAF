package com.example.lin.soundlab;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.example.lin.soundlab.algorithm.util.FiltFiltBeta;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Random;

@RunWith(AndroidJUnit4.class)
public class FiltFiltBenchmarkTest {

    private static final int REPEAT_TIMES = 10;  // 重复次数

    @Test
    public void testFiltFiltPerformance() {
        int[] sizes = {4800*3, 4800*3, 4800*3, 4800*3};
        int filterOrder = 3;
        double cutoff = 50;
        double sampleRate = 48000;

        System.out.println("=== FiltFilt Butterworth Benchmark ===");

        for (int size : sizes) {
            double[] input = generateSignal(size);

            // 预热运行一次（JIT 优化 + 内存缓存）
            FiltFiltBeta.filtfiltButterworth(input, filterOrder, cutoff, sampleRate);

            double totalTimeMs = 0.0;
            double totalSum = 0.0;

            for (int i = 0; i < REPEAT_TIMES; i++) {
                long start = System.nanoTime();
                double[] output = FiltFiltBeta.filtfiltButterworth(input, filterOrder, cutoff, sampleRate);
                long end = System.nanoTime();

                double elapsedMs = (end - start) / 1e6;
                totalTimeMs += elapsedMs;

                // 防止被优化掉
                double sum = 0;
                for (double v : output) sum += v;
                totalSum += sum;
            }

            double avgTime = totalTimeMs / REPEAT_TIMES;
            double avgSum = totalSum / REPEAT_TIMES;

            System.out.printf("Size: %6d | Avg Time: %8.3f ms | Avg Sum: %.3f\n", size, avgTime, avgSum);
        }
    }

    private double[] generateSignal(int size) {
        double[] signal = new double[size];
        Random rand = new Random(42);
        for (int i = 0; i < size; i++) {
            signal[i] = Math.sin(2 * Math.PI * 1000 * i / 48000) + 0.1 * (rand.nextDouble() - 0.5);
        }
        return signal;
    }
}