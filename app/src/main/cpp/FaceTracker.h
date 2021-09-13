//
// Created by Administrator on 2021/6/19.
//

#ifndef OPENCVSAMPLE_FACETRACKER_H
#define OPENCVSAMPLE_FACETRACKER_H

#include <android/native_window_jni.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <pthread.h>

using namespace cv;

class CascadeDetectorAdapter : public DetectionBasedTracker::IDetector {
public:
    CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector) :
            IDetector(),
            Detector(detector) {
        CV_Assert(detector);
    }

    void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) {

        Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize,
                                   maxObjSize);

    }

    virtual ~CascadeDetectorAdapter() {

    }

private:
    CascadeDetectorAdapter();

    cv::Ptr<cv::CascadeClassifier> Detector;
};

class FaceTracker {
public:
    FaceTracker(const char *model);

    ~FaceTracker();

public:
    void setANativeWindow(ANativeWindow *window);

    void draw(Mat img);

public:
    Ptr<DetectionBasedTracker> tracker;
    pthread_mutex_t mutex;
    ANativeWindow *window = 0;
};


#endif //OPENCVSAMPLE_FACETRACKER_H
