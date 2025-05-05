package com.example.lin.soundlab;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.example.lin.soundlab.algorithm.IQDemodulator;
import com.example.lin.soundlab.algorithm.PulseWaveExtractor4Ch;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Random;

@RunWith(AndroidJUnit4.class)
public class IQDemodulatorBenchmarkTest {

    private static final String TAG = "IQBenchmark";
    private static final int REPEAT = 10;

    @Test
    public void benchmarkIQDemodulator() {
        int[] sizes = {9600*3, 9600*3, 9600*3, 9600*3};
        double sampleRate = 48000;
        double carrierFreq = 18000;
        int filterOrder = 3;
        double cutoffFreq = 50;

        Log.i(TAG, "IQDemodulator Benchmark");

        for (int size : sizes) {
            double[] buffer = generateSignal(size);
            IQDemodulator demod = new IQDemodulator(sampleRate, carrierFreq, cutoffFreq, size);

            // 预热运行
            demod.demodulateStream(buffer);
            demod.demodulateStream(buffer);
            demod.demodulateStream(buffer);
            demod.reset();

            double totalTime = 0;
            double totalSum = 0;

            for (int i = 0; i < REPEAT; i++) {
                demod.reset(); // 每轮重置状态

                // 连续流式三次，确保至少输出一次有效值
                demod.demodulateStream(buffer); // i-2 = 0
                demod.demodulateStream(buffer); // i-1 = 0
                long start = System.nanoTime();
                double[] result = demod.demodulateStream(buffer); // i: 当前 → 输出 i-1
                double[] result2 = demod.demodulateStream(buffer); // i: 当前 → 输出 i-1
                double[] result3 = demod.demodulateStream(buffer); // i: 当前 → 输出 i-1
                long end = System.nanoTime();

                double elapsed = (end - start) / 1e6;
                totalTime += elapsed;

                double sum = 0;
                for (double v : result) sum += v;
                for (double v : result2) sum += v;
                for (double v : result3) sum += v;
                totalSum += sum;
            }

            double avgTime = totalTime / REPEAT;
            double avgSum = totalSum / REPEAT;

            Log.i(TAG, String.format("Size: %6d | Avg Time: %8.3f ms | Avg Sum: %.3f", size, avgTime, avgSum));
        }
    }

    @Test
    public void benchmarkPulseWaveExtractor4Ch() {
        int[] sizes = {9600 * 3, 9600 * 3, 9600 * 3, 9600 * 3};
        double sampleRate = 48000;
        double[] carrierFreqs = {18000, 19000, 20000, 21000};
        double cutoffFreq = 50;

        Log.i("IQBenchmark", "PulseWaveExtractor4Ch Benchmark");

        for (int size : sizes) {
            double[] buffer = generateSignal(size);
            PulseWaveExtractor4Ch extractor = new PulseWaveExtractor4Ch(sampleRate, carrierFreqs, cutoffFreq, size);

            // 预热
            try {
                extractor.extractAllChannels(buffer);
                extractor.extractAllChannels(buffer);
            } catch (Exception e) {
                Log.e("IQBenchmark", "Warm-up error", e);
            }
            extractor.reset();

            double totalTime = 0;
            double totalSum = 0;

            for (int i = 0; i < REPEAT; i++) {
                extractor.reset();

                try {
                    extractor.extractAllChannels(buffer); // i-2
                    extractor.extractAllChannels(buffer); // i-1

                    long start = System.nanoTime();
                    double[][] result = extractor.extractAllChannels(buffer); // i
                    long end = System.nanoTime();

                    double elapsed = (end - start) / 1e6;
                    totalTime += elapsed;

                    double sum = 0;
                    for (double[] ch : result) {
                        for (double v : ch) sum += v;
                    }
                    totalSum += sum;

                } catch (Exception e) {
                    Log.e("IQBenchmark", "Execution error", e);
                }
            }

            double avgTime = totalTime / REPEAT;
            double avgSum = totalSum / REPEAT;

            Log.i("IQBenchmark", String.format("4CH Size: %6d | Avg Time: %8.3f ms | Avg Sum: %.3f", size, avgTime, avgSum));

            extractor.shutdown();
        }
    }


    private double[] generateSignal(int size) {
        double[] signal = new double[size];
        Random rand = new Random(42);
        for (int i = 0; i < size; i++) {
            signal[i] = Math.sin(2 * Math.PI * 1000 * i / 44100.0) + 0.1 * (rand.nextDouble() - 0.5);
        }
        return signal;
    }
}