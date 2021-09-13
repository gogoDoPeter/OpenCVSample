//
// Created by Administrator on 2021/6/19.
//

#include "FaceTracker.h"

#define ANATIVEWINDOW_RELEASE(window)   \
if (window) { \
            ANativeWindow_release(window); \
            window = 0; \
      }


FaceTracker::FaceTracker(const char *model) {
    pthread_mutex_init(&mutex, 0);
    Ptr<CascadeDetectorAdapter> mainDetector = makePtr<CascadeDetectorAdapter>(
            makePtr<CascadeClassifier>(model));
    Ptr<CascadeDetectorAdapter> trackingDetector = makePtr<CascadeDetectorAdapter>(
            makePtr<CascadeClassifier>(model));
    //跟踪器
    DetectionBasedTracker::Parameters DetectorParams;
    tracker = makePtr<DetectionBasedTracker>(DetectionBasedTracker(mainDetector, trackingDetector,
                                                                   DetectorParams));
}

FaceTracker::~FaceTracker() {
    pthread_mutex_destroy(&mutex);
    ANATIVEWINDOW_RELEASE(window);
}

void FaceTracker::setANativeWindow(ANativeWindow *window) {
    pthread_mutex_lock(&mutex);
    // 替换问题
    ANATIVEWINDOW_RELEASE(this->window);
    this->window = window;
    pthread_mutex_unlock(&mutex);
}

void FaceTracker::draw(Mat img) {
    pthread_mutex_lock(&mutex);
    do {
        if (!window) {
            break;
        }
        ANativeWindow_setBuffersGeometry(window, img.cols, img.rows,
                                         WINDOW_FORMAT_RGBA_8888);
        ANativeWindow_Buffer buffer;
        if (ANativeWindow_lock(window, &buffer, 0)) {
            ANATIVEWINDOW_RELEASE(window);
            break;
        }
        uint8_t *dstData = static_cast<uint8_t *>(buffer.bits);
        int dstlineSize = buffer.stride * 4;
        uint8_t *srcData = img.data;
        int srclineSize = img.cols * 4;
        for (int i = 0; i < buffer.height; ++i) {
            memcpy(dstData + i * dstlineSize, srcData + i * srclineSize, srclineSize);
        }
        ANativeWindow_unlockAndPost(window);
    } while (0);

    pthread_mutex_unlock(&mutex);
}