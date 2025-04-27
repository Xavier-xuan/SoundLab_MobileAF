package com.example.lin.soundlab;

import android.app.Activity;
import android.media.AudioFormat;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.example.lin.soundlab.view.WaveformView;

import java.util.Locale;

public class ProcessThread implements Runnable {
    private static final String TAG = "SoundLabProcessThread";

    private int sleepInterval = 50;

    private static SonicQueue sonicQueue = new SonicQueue();


    private static int processFrequency = 10;
    private static int samplingRate = 48000;
    private static final int groupSize = 12;
    private static final int downSampleFactor = 10;
    private static int processBufferDataSize = samplingRate / processFrequency;
    private static short[] processBufferData = new short[processBufferDataSize];
    private static int channel;

    private static float[] groupSamplesMono = new float[processBufferDataSize / downSampleFactor / groupSize];
    private static float[][] groupedVolumesStereo = new float[2][processBufferDataSize / downSampleFactor / groupSize / 2];


    private Activity superActivity;
    private TextView textviewTotalVolume;
    private ProgressBar progressbarVolume1;
    private TextView textviewVolume1;
    private ProgressBar progressbarVolume2;
    private TextView textviewVolume2;

    private ProgressBar progressbarBufferUsage;
    private TextView textviewBufferUsage;

    // Realtime Waveform Related
    private WaveformView channel1WaveformView;
    private WaveformView channel2WaveformView;


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

        channel1WaveformView = superActivity.findViewById(R.id.Channel1WaveformView);
        channel2WaveformView = superActivity.findViewById(R.id.Channel2WaveformView);
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
            long groupSum = 0;
            int groupIdx2Write = 0;
            short groupCounter = 0;

            for (int i = 0; i < processBufferDataSize; i = i + downSampleFactor) {
                long result = processBufferData[i] * processBufferData[i];
                sum += result;
                groupSum += result;

                groupCounter++;
                if (groupCounter == groupSize){
                    groupSamplesMono[groupIdx2Write] = (float)(10 * Math.log10( (double) groupSum / groupSize));
                    groupIdx2Write++;
                    groupCounter = 0;
                }
            }


            double mean = sum / (double) (processBufferDataSize / downSampleFactor);
            double volume = 10 * Math.log10(mean);
            double volumeWithCorrection = volumeCorrection(volume);
            LogThread.debugLog(0, TAG, "volume: " + volumeWithCorrection);

            updateUI(volumeWithCorrection, volumeWithCorrection, volumeWithCorrection, bufferUsage, groupSamplesMono, null);
        }

        // STEREO mode
        if (channel == AudioFormat.CHANNEL_IN_STEREO) {
            long sum1 = 0;
            long sum2 = 0;
            long groupSum1 = 0;
            long groupSum2 = 0;
            int groupIdx2Write = 0;
            short groupCounter = 0;
            for (int i = 0; i < processBufferDataSize; i = i + 2 * downSampleFactor) {
                long result1 = processBufferData[i] * processBufferData[i];
                long result2 = processBufferData[i + 1] * processBufferData[i + 1];
                groupSum1 += result1;
                groupSum2 += result2;
                sum1 += result1;
                sum2 += result2;

                groupCounter++;
                if (groupCounter == groupSize){
                    groupedVolumesStereo[0][groupIdx2Write] = (float) (10 * Math.log10( (double) groupSum1 / groupSize));
                    groupedVolumesStereo[1][groupIdx2Write] = (float) (10 * Math.log10( (double) groupSum2 / groupSize));
                    groupIdx2Write++;
                    groupCounter = 0;
                    groupSum1 = 0;
                    groupSum2 = 0;
                }
            }

            double volume1 = 10 * Math.log10(((double) sum1 / ((double) processBufferDataSize / downSampleFactor)) * 2);
            double volume2 = 10 * Math.log10(((double) sum2 / ((double) processBufferDataSize / downSampleFactor)) * 2);
            double volume1WithCorrection = volumeCorrection(volume1);
            double volume2WithCorrection = volumeCorrection(volume2);

            updateUI(volume1WithCorrection, volume2WithCorrection, (volume1WithCorrection + volume2WithCorrection) / 2, bufferUsage, groupedVolumesStereo[0], groupedVolumesStereo[1]);
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

            if(channel1Waveform != null){
                channel1WaveformView.appendWaveform(channel1Waveform);
                channel1WaveformView.invalidate();
            }

            if (channel2Waveform != null){
                channel2WaveformView.appendWaveform(channel2Waveform);
                channel2WaveformView.invalidate();
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
