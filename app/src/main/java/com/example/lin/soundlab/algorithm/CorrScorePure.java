package com.example.lin.soundlab.algorithm;

public class CorrScorePure {

    /**
     * @param signals 2D double array of shape [numChannels][numSamples]
     * @return average cosine similarity (filtered)
     */
    public static double corrScore(double[][] signals) {
        int numChannels = signals.length;
        int sigLength = signals[0].length;

        // 1. 零均值归一化每个通道
        double[][] normed = new double[numChannels][sigLength];
        for (int i = 0; i < numChannels; i++) {
            double[] row = signals[i];
            double mean = 0.0;
            for (double v : row) mean += v;
            mean /= sigLength;

            double norm = 0.0;
            for (int j = 0; j < sigLength; j++) {
                normed[i][j] = row[j] - mean;
                norm += normed[i][j] * normed[i][j];
            }

            norm = Math.sqrt(norm) + 1e-8; // 避免除0
            for (int j = 0; j < sigLength; j++) {
                normed[i][j] /= norm;
            }
        }

        // 2. 计算通道间 cosine 相似度矩阵（对称的）
        double[][] cosineSim = new double[numChannels][numChannels];
        for (int i = 0; i < numChannels; i++) {
            for (int j = i; j < numChannels; j++) {
                double dot = 0.0;
                for (int k = 0; k < sigLength; k++) {
                    dot += normed[i][k] * normed[j][k];
                }
                cosineSim[i][j] = dot;
                cosineSim[j][i] = dot;
            }
        }

        // 3. 提取上三角（不含对角线）
        int count = 0;
        double maxSim = -Double.MAX_VALUE;
        double[] upperValues = new double[numChannels * (numChannels - 1) / 2];
        for (int i = 0; i < numChannels; i++) {
            for (int j = i + 1; j < numChannels; j++) {
                double val = cosineSim[i][j];
                upperValues[count++] = val;
                if (val > maxSim) maxSim = val;
            }
        }

        // 4. 筛选大于 0.6 * max 的相似度
        double threshold = maxSim * 0.6;
        double sum = 0.0;
        int valid = 0;
        for (int i = 0; i < count; i++) {
            if (upperValues[i] >= threshold) {
                sum += upperValues[i];
                valid++;
            }
        }

        return valid > 0 ? sum / valid : 0.0;
    }
}
