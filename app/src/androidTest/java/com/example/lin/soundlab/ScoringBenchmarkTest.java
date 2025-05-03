package com.example.lin.soundlab;

import android.util.Log;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.example.lin.soundlab.algorithm.CorrScore;
import com.example.lin.soundlab.algorithm.CorrScorePure;
import com.example.lin.soundlab.algorithm.FreqScore;

import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.Random;

@RunWith(AndroidJUnit4.class)
public class ScoringBenchmarkTest {


    @Test
    public void testFreqScorePerformance() {
        String TAG = "FreqScoreJUnit";
        int numChannels = 4;
        int sigLength = 3840;
        int fs = 128;
        int rounds = 5;

        FreqScore freqScore = new FreqScore();
        double[] input = new double[numChannels * sigLength];
        Random rand = new Random(42);

        for (int i = 0; i < input.length; i++) {
            input[i] = rand.nextFloat() * 2 - 1;
        }

        long totalTime = 0;
        for (int i = 0; i < rounds; i++) {
            long start = System.nanoTime();
            double result = freqScore.run(input, numChannels, sigLength, fs);
            long end = System.nanoTime();

            double durationMs = (end - start) / 1_000_000d;
            totalTime += durationMs;

            Log.d(TAG, "Round " + (i + 1) + ": result=" + result + ", time=" + durationMs + " ms");
        }

        float avgTime = totalTime / (float) rounds;
        Log.i(TAG, "Avg JNI time over " + rounds + " rounds: " + avgTime + " ms");
    }

    @Test
    public void testCorrScorePerformance(){
        String TAG = "CorrScoreJUnit";
        int numChannels = 4;
        int sigLength = 3840;
        int rounds = 5;

        CorrScore corrScore = new CorrScore();
        double[] input = new double[numChannels * sigLength];
        Random rand = new Random(42);

        for (int i = 0; i < input.length; i++) {
            input[i] = rand.nextFloat() * 2 - 1;
        }

        long totalTime = 0;
        for (int i = 0; i < rounds; i++) {
            long start = System.nanoTime();
            double result = corrScore.run(input, numChannels, sigLength);
            long end = System.nanoTime();

            double durationMs = (end - start) / 1_000_000d;
            totalTime += durationMs;

            Log.d(TAG, "Round " + (i + 1) + ": result=" + result + ", time=" + durationMs + " ms");
        }

        float avgTime = totalTime / (float) rounds;
        Log.i(TAG, "Avg JNI time over " + rounds + " rounds: " + avgTime + " ms");
    }

    @Test
    public void testCorrScorePurePerformance() {
        final String TAG = "CorrScorePureJUnit";
        int numChannels = 4;
        int sigLength = 3840;
        int rounds = 5;

        // 1. 生成随机输入（double[4][3840]）
        double[][] signals = new double[numChannels][sigLength];
        Random rand = new Random(42);
        for (int i = 0; i < numChannels; i++) {
            for (int j = 0; j < sigLength; j++) {
                signals[i][j] = rand.nextDouble() * 2 - 1;
            }
        }

        long totalTimeNs = 0;

        // 2. 多轮测试
        for (int i = 0; i < rounds; i++) {
            long start = System.nanoTime();
            double result = CorrScorePure.corrScore(signals);
            long end = System.nanoTime();

            double durationMs = (end - start) / 1_000_000d;
            totalTimeNs += (end - start);

            Log.d(TAG, "Round " + (i + 1) + ": result=" + result + ", time=" + durationMs + " ms");
        }

        double avgTimeMs = totalTimeNs / 1_000_000.0 / rounds;
        Log.i(TAG, "Avg PURE CorrScore time over " + rounds + " rounds: " + avgTimeMs + " ms");
    }

    @Test
    public void comparePureResult() {
        final String TAG = "CorrScorePureResultJUnit";

        int numChannels = 4;
        int sigLength = 3840;
        double[][] signals = new double[numChannels][sigLength];
        double fs = 128.0;    // 采样率

        for (int t = 0; t < sigLength; t++) {
            double time = t / fs;
            double s1 = Math.sin(2 * Math.PI * 11.0d  * time) + Math.sin(2 * Math.PI * 5.0d * time);
            double s2 = Math.sin(2 * Math.PI * 7.0d * time);
            double s3 = Math.sin(2 * Math.PI * 11.0d  * time);
            double s4 = Math.sin(2 * Math.PI * 13.0d  * time);

            signals[0][t] = s1;
            signals[1][t] = s2;
            signals[2][t] = s3;
            signals[3][t] = s4;
        }

        double score = CorrScorePure.corrScore(signals);
        Log.i(TAG, "Java corr_score = " + score);

    }
}