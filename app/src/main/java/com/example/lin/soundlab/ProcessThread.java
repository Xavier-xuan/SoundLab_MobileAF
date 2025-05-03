package com.example.lin.soundlab;

import android.app.Activity;
import android.media.AudioFormat;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.example.lin.soundlab.algorithm.IQDemodulator;
import com.example.lin.soundlab.view.WaveformView;

import java.util.Locale;

public class ProcessThread implements Runnable {
    private static final String TAG = "SoundLabProcessThread";

    private int sleepInterval = 50;

    private static SonicQueue sonicQueue = new SonicQueue();


    private static int processFrequency = 2;
    private static int samplingRate = 48000;
    private static final int downSampleFactor = 48;
    private static int processBufferDataSize = samplingRate / processFrequency;
    private static short[] processBufferData = new short[processBufferDataSize];
    private static int channel;

    private Activity superActivity;
    private TextView textviewTotalVolume;
    private ProgressBar progressbarVolume1;
    private TextView textviewVolume1;
    private ProgressBar progressbarVolume2;
    private TextView textviewVolume2;

    private ProgressBar progressbarBufferUsage;
    private TextView textviewBufferUsage;

    // Realtime Waveform Related
    private WaveformView waveformViewCH1;
    private WaveformView waveformViewCH2;

    private IQDemodulator iqDemodulatorCH1;
    private IQDemodulator iqDemodulatorCH2;


    @Override
    public void run() {
        LogThread.debugLog(2, TAG, "ProcessThread.run()");
        long sleepIntervalMillis = sleepInterval; // 单位毫秒
        long lastTime = System.currentTimeMillis();

        while (true) {
            process();

            long now = System.currentTimeMillis();
            long nextExpectedTime = lastTime + sleepIntervalMillis;
            long delay = nextExpectedTime - now;

            if (delay > 0) {
                try {
                    Thread.sleep(delay);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            } else {
                // 说明 process() 执行太慢，直接下一轮，不sleep
                LogThread.debugLog(3, TAG, "Processing is slower than interval, skipping sleep");
            }

            lastTime = nextExpectedTime;
        }
    }

    public void setup(Activity activity, int recordChannel, int recordSamplingRate) {
        superActivity = activity;

        samplingRate = recordSamplingRate;
        processBufferDataSize = samplingRate / processFrequency;
        channel = recordChannel;
        processBufferData = new short[processBufferDataSize];


        // info display
        textviewTotalVolume = superActivity.findViewById(R.id.textviewTotalVolume);
        progressbarVolume1 = superActivity.findViewById(R.id.progressbarVolume1);
        textviewVolume1 = superActivity.findViewById(R.id.textviewVolume1);
        progressbarVolume2 = superActivity.findViewById(R.id.progressbarVolume2);
        textviewVolume2 = superActivity.findViewById(R.id.textviewVolume2);

        progressbarBufferUsage = superActivity.findViewById(R.id.progressbarBufferUsage);
        textviewBufferUsage = superActivity.findViewById(R.id.textviewBufferUsage);

        waveformViewCH1 = superActivity.findViewById(R.id.Channel1WaveformView);
        waveformViewCH2 = superActivity.findViewById(R.id.Channel2WaveformView);

        int channelBufferSize = recordChannel == AudioFormat.CHANNEL_IN_MONO ? processBufferDataSize : processBufferDataSize / 2;
        iqDemodulatorCH1 = new IQDemodulator(samplingRate, 18000, 3, 50, channelBufferSize);
        iqDemodulatorCH2 = new IQDemodulator(samplingRate, 18000, 3, 50, channelBufferSize);
    }

    private void process() {
        if (sonicQueue.getLength() < processBufferDataSize) {
            return;
        }
        // try to read data from buffer
        boolean isReadSuccess = read(processBufferData);
        if (!isReadSuccess) {
            return;
        }

        // update buffer usage
        double bufferUsage = (double) sonicQueue.getLength() / sonicQueue.getCapacity();
        LogThread.debugLog(0, TAG, "Buffer usage: " + bufferUsage);

        // MONO mode
        if (channel == AudioFormat.CHANNEL_IN_MONO) {
            long sum = 0;

            for (int i = 0; i < processBufferDataSize; i = i + downSampleFactor) {
                long result = processBufferData[i] * processBufferData[i];
                sum += result;
            }

            double mean = sum / (double) (processBufferDataSize / downSampleFactor);
            double volume = 10 * Math.log10(mean);
            double volumeWithCorrection = volumeCorrection(volume);
            LogThread.debugLog(0, TAG, "volume: " + volumeWithCorrection);

            double[] bufferDouble = new double[processBufferDataSize];
            for (int i = 0; i < processBufferDataSize; i++) {
                bufferDouble[i] = (double) processBufferData[i] / 32768;
            }

            // IQ 解调处理
            double[] demodulatedPhase = iqDemodulatorCH1.demodulateStream(bufferDouble);

            // 降采样（简单方式：每 downSampleFactor 个点取一个）
            int downSampledLength = demodulatedPhase.length / downSampleFactor;
            float[] downSampled = new float[downSampledLength];
            for (int i = 0; i < downSampledLength; i++) {
                downSampled[i] = (float) demodulatedPhase[i * downSampleFactor];
            }

            updateUI(volumeWithCorrection, volumeWithCorrection, volumeWithCorrection, bufferUsage, downSampled, null);
        }

        // STEREO mode
        if (channel == AudioFormat.CHANNEL_IN_STEREO) {
            long sum1 = 0;
            long sum2 = 0;

            for (int i = 0; i < processBufferDataSize; i = i + 2 * downSampleFactor) {
                sum1 += processBufferData[i] * processBufferData[i];
                sum2 += processBufferData[i + 1] * processBufferData[i + 1];
            }

            double volume1 = 10 * Math.log10(((double) sum1 / ((double) processBufferDataSize / downSampleFactor)) * 2);
            double volume2 = 10 * Math.log10(((double) sum2 / ((double) processBufferDataSize / downSampleFactor)) * 2);
            double volume1WithCorrection = volumeCorrection(volume1);
            double volume2WithCorrection = volumeCorrection(volume2);

            double[][] bufferDouble = new double[2][processBufferDataSize / 2];
            for (int i = 0; i < processBufferDataSize; i = i + 2) {
                bufferDouble[0][i / 2] = (double) processBufferData[i] / 32768;
                bufferDouble[1][i / 2] = (double) processBufferData[i + 1] / 32768;
            }
            double[] demodulatedPhaseCH1 = iqDemodulatorCH1.demodulateStream(bufferDouble[0]);
            double[] demodulatedPhaseCH2 = iqDemodulatorCH2.demodulateStream(bufferDouble[1]);

            // 降采样（简单方式：每 downSampleFactor 个点取一个）
            int downSampledLength = demodulatedPhaseCH1.length / downSampleFactor;
            float[][] downSampled = new float[2][downSampledLength];
            for (int i = 0; i < downSampledLength; i++) {
                downSampled[0][i] = (float) demodulatedPhaseCH1[i * downSampleFactor];
                downSampled[1][i] = (float) demodulatedPhaseCH2[i * downSampleFactor];
            }

            updateUI(volume1WithCorrection, volume2WithCorrection, (volume1WithCorrection + volume2WithCorrection) / 2, bufferUsage, downSampled[0], downSampled[1]);
        }

    }

    public synchronized static boolean write(byte[] data) {
        boolean isWrite = sonicQueue.write(data);
        if (isWrite) {
            return true;
        } else {
            LogThread.debugLog(4, TAG, "Write fail. The queue may be full. Buffer usage: " + (double) sonicQueue.getLength() / sonicQueue.getCapacity());
            return false;
        }
    }

    public synchronized static boolean write(short[] data) {
        boolean isWrite = sonicQueue.write(data);
        if (isWrite) {
            return true;
        } else {
            LogThread.debugLog(4, TAG, "Write fail. The queue may be full. Buffer usage: " + (double) sonicQueue.getLength() / sonicQueue.getCapacity());
            return false;
        }
    }

    public synchronized static boolean read(byte[] data) {
        return sonicQueue.read(data);
    }

    public synchronized static boolean read(short[] data) {
        return sonicQueue.read(data);
    }


    private void setTotalVolume(double volume) {
        superActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                textviewTotalVolume.setText(String.format(Locale.US, "%.2f", volume));
            }
        });
    }

    private void setVolume1(double volume) {
        superActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                textviewVolume1.setText(String.format(Locale.US, "%.2f", volume));
                progressbarVolume1.setProgress((int) volume);
            }
        });
    }

    private void setVolume2(double volume) {
        superActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                textviewVolume2.setText(String.format(Locale.US, "%.2f", volume));
                progressbarVolume2.setProgress((int) volume);
            }
        });
    }

    private void updateUI(double volume1, double volume2, double totalVolume, double bufferUsage, float[] channel1Waveform, float[] channel2Waveform) {
        superActivity.runOnUiThread(() -> {
            setVolume1(volume1);
            setVolume2(volume2);
            textviewBufferUsage.setText(String.format(Locale.US, "%.2f/%s", bufferUsage * 100, "%"));
            progressbarBufferUsage.setProgress((int) (bufferUsage * 100));
            textviewTotalVolume.setText(String.format(Locale.US, "%.2f", totalVolume));

            if (channel1Waveform != null) {
                waveformViewCH1.appendWaveform(channel1Waveform);
                waveformViewCH1.invalidate();
            }

            if (channel2Waveform != null) {
                waveformViewCH2.appendWaveform(channel2Waveform);
                waveformViewCH2.invalidate();
            }
        });
    }

    private double volumeCorrection(double volume) {
//        return 1.037*volume-7.191;
        return volume;
    }

    private void setBufferUsage(double bufferUsage) {
        superActivity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                textviewBufferUsage.setText(String.format(Locale.US, "%.2f/%s", bufferUsage * 100, "%"));
                progressbarBufferUsage.setProgress((int) (bufferUsage * 100));
            }
        });
    }

}
