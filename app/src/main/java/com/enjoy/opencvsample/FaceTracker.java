package com.enjoy.opencvsample;

import android.view.Surface;

public class FaceTracker {
    private long mNativeObj = 0;

    /**
     *
     * @param model 人脸定位模型
     */
    public FaceTracker(String model) {
        mNativeObj = nativeCreateObject(model);
    }

    /**
     * 设置显示的画布
     * @param surface
     */
    public void setSurface(Surface surface) {
        nativeSetSurface(mNativeObj, surface);
    }

    /**
     * 释放
     */
    public void release() {
        nativeDestroyObject(mNativeObj);
        mNativeObj = 0;
    }

    /**
     * 开启人脸跟踪
     */
    public void start() {
        nativeStart(mNativeObj);
    }

    /**
     * 关闭
     */
    public void stop() {
        nativeStop(mNativeObj);
    }

    /**
     * 人脸定位 同时显示在surface中
     * @param inputImage  图像数据
     * @param width
     * @param height
     * @param rotationDegrees
     */
    public void detect(byte[] inputImage, int width, int height, int rotationDegrees) {
        nativeDetect(mNativeObj, inputImage, width, height, rotationDegrees);
    }

    private static native long nativeCreateObject(String model);

    private static native void nativeDestroyObject(long thiz);

    private static native void nativeSetSurface(long thiz, Surface surface);

    private static native void nativeStart(long thiz);

    private static native void nativeStop(long thiz);

    private static native void nativeDetect(long thiz, byte[] inputImage, int width, int height, int rotationDegrees);
}
