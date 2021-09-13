package com.enjoy.opencvsample;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.camera.camera2.Camera2Config;
import androidx.camera.core.CameraSelector;
import androidx.camera.core.CameraX;
import androidx.camera.core.ImageAnalysis;
import androidx.camera.core.ImageProxy;
import androidx.camera.core.Preview;
import androidx.camera.core.SurfaceRequest;
import androidx.camera.lifecycle.ProcessCameraProvider;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.core.util.Consumer;
import androidx.lifecycle.LifecycleOwner;

import android.Manifest;
import android.annotation.SuppressLint;
import android.hardware.Camera;
import android.media.Image;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Size;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

import com.google.common.util.concurrent.ListenableFuture;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity implements
        SurfaceHolder.Callback, ImageAnalysis.Analyzer {

    static {
        System.loadLibrary("native-lib");
    }

    private FaceTracker faceTracker;
    private ListenableFuture<ProcessCameraProvider> cameraProviderFuture;
    private SurfaceView surfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //申请摄像头权限
        ActivityCompat.requestPermissions(this,
                new String[]{Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE},
                100);

        //
        String path = Utils.copyAsset2Dir(this, "lbpcascade_frontalface.xml");

        faceTracker = new FaceTracker(path);
        faceTracker.start();

        surfaceView = findViewById(R.id.surfaceView);
        surfaceView.getHolder().addCallback(this);


        /**
         *  CameraX
         */
        cameraProviderFuture = ProcessCameraProvider.getInstance(this);

        cameraProviderFuture.addListener(new Runnable() {
            @Override
            public void run() {
                try {
                    ProcessCameraProvider cameraProvider = cameraProviderFuture.get();
                    bindAnalysis(cameraProvider);
                } catch (Exception e) {
                    e.printStackTrace();
                }

            }
        }, ContextCompat.getMainExecutor(this));


    }

    private void bindAnalysis(ProcessCameraProvider cameraProvider) {
        //STRATEGY_KEEP_ONLY_LATEST :非阻塞模式，每次获得最新帧
        //STRATEGY_BLOCK_PRODUCER : 阻塞模式，处理不及时会导致降低帧率
        //图片分析：得到摄像头图像数据
        ImageAnalysis imageAnalysis =
                new ImageAnalysis.Builder()
                        .setTargetResolution(new Size(640, 480))
                        .setBackpressureStrategy(ImageAnalysis.STRATEGY_KEEP_ONLY_LATEST)
                        .build();

        imageAnalysis.setAnalyzer(ContextCompat.getMainExecutor(this), this);
        cameraProvider.unbindAll();
        //绑定生命周期
        cameraProvider.bindToLifecycle(this,
                CameraSelector.DEFAULT_BACK_CAMERA, imageAnalysis);

//  todo 切换摄像头
//        cameraProvider.unbindAll();
//        //绑定生命周期
//        cameraProvider.bindToLifecycle(this,
//                CameraSelector.DEFAULT_FRONT_CAMERA, imageAnalysis);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {

    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
        faceTracker.setSurface(holder.getSurface());
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {
        faceTracker.setSurface(null);
    }

    @Override
    public void analyze(@NonNull ImageProxy image) {
        byte[] bytes = Utils.getDataFromImage(image);
        //定位人脸，并且显示摄像头的图像
        faceTracker.detect(bytes, image.getWidth(), image.getHeight(), image.getImageInfo().getRotationDegrees());
        image.close();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        faceTracker.release();
    }
}