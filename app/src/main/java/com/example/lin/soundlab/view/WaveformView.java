package com.example.lin.soundlab.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;


public class WaveformView extends View {
    private static final int BUFFER_SIZE = 5000;
    private final float[] waveformBuffer = new float[BUFFER_SIZE];
    private int writeIndex = 0;
    private final Paint paint = new Paint();

    private float maxValue = 0.01f; // 正最大值
    private float minValue = -0.01f; // 负最小值

    public WaveformView(Context context, AttributeSet attrs) {
        super(context, attrs);
        paint.setColor(Color.GREEN);
        paint.setStrokeWidth(2f);
    }

    public synchronized void appendWaveform(float[] newData) {
        for (float s : newData) {
            waveformBuffer[writeIndex] = s;

            // 动态更新边界
            if (s > maxValue) maxValue = s;
            if (s < minValue) minValue = s;

            writeIndex = (writeIndex + 1) % BUFFER_SIZE;
        }

        // 检查是否需要重新扫描整个缓冲区以更新边界
        boolean needsRecalc = true;
        for (float s : newData) {
            if (s >= maxValue || s <= minValue) {
                needsRecalc = false;
                break;
            }
        }
        if (needsRecalc) recalculateBounds();

        invalidate(); // 请求重绘
    }

    private void recalculateBounds() {
        float max = -Float.MAX_VALUE;
        float min = Float.MAX_VALUE;
        for (float v : waveformBuffer) {
            if (v > max) max = v;
            if (v < min) min = v;
        }

        // 防止max == min导致除0
        if (max - min < 1e-6) {
            max += 0.01f;
            min -= 0.01f;
        }

        maxValue = max;
        minValue = min;
    }

    @Override
    protected synchronized void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        float viewHeight = getHeight();
        float xScale = (float) getWidth() / BUFFER_SIZE;

        float range = maxValue - minValue;

        for (int i = 0; i < BUFFER_SIZE - 1; i++) {
            int index1 = (writeIndex + i) % BUFFER_SIZE;
            int index2 = (writeIndex + i + 1) % BUFFER_SIZE;
            float x1 = i * xScale;
            float x2 = (i + 1) * xScale;

            float y1 = (1 - (waveformBuffer[index1] - minValue) / range) * viewHeight;
            float y2 = (1 - (waveformBuffer[index2] - minValue) / range) * viewHeight;

            canvas.drawLine(x1, y1, x2, y2, paint);
        }
    }
}