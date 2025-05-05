package com.example.lin.soundlab.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class WaveformSurfaceView extends SurfaceView implements SurfaceHolder.Callback, Runnable {

    private static final int BUFFER_SIZE = 3000;
    private final float[] waveformBuffer = new float[BUFFER_SIZE];
    private int writeIndex = 0;

    private float maxValue = 0.01f;
    private float minValue = -0.01f;

    private final Paint paint = new Paint();
    private final SurfaceHolder holder;
    private Thread drawThread;
    private volatile boolean isDrawing = false;

    public WaveformSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        holder = getHolder();
        holder.addCallback(this);

        paint.setColor(Color.GREEN);
        paint.setStrokeWidth(2f);
        setFocusable(true);
    }

    public synchronized void appendWaveform(float[] newData) {
        for (float s : newData) {
            waveformBuffer[writeIndex] = s;

            if (s > maxValue) maxValue = s;
            if (s < minValue) minValue = s;

            writeIndex = (writeIndex + 1) % BUFFER_SIZE;
        }

        boolean needsRecalc = true;
        for (float s : newData) {
            if (s >= maxValue || s <= minValue) {
                needsRecalc = false;
                break;
            }
        }
        if (needsRecalc) recalculateBounds();
    }

    private void recalculateBounds() {
        float max = -Float.MAX_VALUE;
        float min = Float.MAX_VALUE;
        for (float v : waveformBuffer) {
            if (v > max) max = v;
            if (v < min) min = v;
        }

        if (max - min < 1e-6) {
            max += 0.01f;
            min -= 0.01f;
        }

        maxValue = max;
        minValue = min;
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        isDrawing = true;
        drawThread = new Thread(this);
        drawThread.start();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        isDrawing = false;
        if (drawThread != null) {
            try {
                drawThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        // 不需要处理
    }

    @Override
    public void run() {
        long frameInterval = 1000 / 15; // 15 FPS
        while (isDrawing) {
            long start = System.currentTimeMillis();
            drawWaveform();
            long end = System.currentTimeMillis();
            long sleep = frameInterval - (end - start);
            if (sleep > 0) {
                try {
                    Thread.sleep(sleep);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void drawWaveform() {
        Canvas canvas = holder.lockCanvas();
        if (canvas == null) return;

        try {
            canvas.drawColor(Color.BLACK);

            float width = getWidth();
            float height = getHeight();
            float xScale = width / BUFFER_SIZE;
            float range = maxValue - minValue;

            // 缩放倍数定义为 1 / range（只为显示参考）
            float zoomFactor = 1.0f / range;

            synchronized (this) {
                for (int i = 0; i < BUFFER_SIZE - 1; i++) {
                    int idx1 = (writeIndex + i) % BUFFER_SIZE;
                    int idx2 = (writeIndex + i + 1) % BUFFER_SIZE;

                    float x1 = i * xScale;
                    float x2 = (i + 1) * xScale;

                    float y1 = (1 - (waveformBuffer[idx1] - minValue) / range) * height;
                    float y2 = (1 - (waveformBuffer[idx2] - minValue) / range) * height;

                    canvas.drawLine(x1, y1, x2, y2, paint);
                }
            }

            // 绘制缩放倍数信息
            Paint textPaint = new Paint();
            textPaint.setColor(Color.WHITE);
            textPaint.setTextSize(36f);
            canvas.drawText(String.format("x %.2f", zoomFactor), 20, 40, textPaint);

        } finally {
            holder.unlockCanvasAndPost(canvas);
        }
    }
}
