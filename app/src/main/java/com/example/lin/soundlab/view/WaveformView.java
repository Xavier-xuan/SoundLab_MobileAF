package com.example.lin.soundlab.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;


public class WaveformView extends View {
    private static final int BUFFER_SIZE = 5000; // 可视区域的最大采样点数量

    private static final float maxVolume = 3.15F;
    private final float[] waveformBuffer = new float[BUFFER_SIZE];
    private int writeIndex = 0;
    private final Paint paint = new Paint();

    public WaveformView(Context context, AttributeSet attrs) {
        super(context, attrs);
        paint.setColor(Color.GREEN);
        paint.setStrokeWidth(2f);
    }

    // 用环形缓冲区保存数据
    public synchronized void appendWaveform(float[] newData) {
        for (float s : newData) {
            waveformBuffer[writeIndex] = s;
            writeIndex = (writeIndex + 1) % BUFFER_SIZE;
        }
    }


    @Override
    protected synchronized void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        float halfHeight = getHeight() >> 1;
        float xScale = (float) getWidth() / BUFFER_SIZE;

        for (int i = 0; i < BUFFER_SIZE - 1; i++) {
            int index1 = (writeIndex + i) % BUFFER_SIZE;
            int index2 = (writeIndex + i + 1) % BUFFER_SIZE;
            float x1 = i * xScale;
            float x2 = (i + 1) * xScale;
            float y1 = halfHeight - waveformBuffer[index1] / maxVolume * halfHeight;
            float y2 = halfHeight - waveformBuffer[index2] / maxVolume * halfHeight;
            canvas.drawLine(x1, y1, x2, y2, paint);
        }
    }
}
