#include <jni.h>
#include <string>

#include "FaceTracker.h"
#include "log/AndroidLog.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_enjoy_opencvsample_FaceTracker_nativeCreateObject(JNIEnv *env, jclass clazz,
                                                           jstring model_) {
    const char *model = env->GetStringUTFChars(model_, 0);


    FaceTracker *tracker = new FaceTracker(model);


    env->ReleaseStringUTFChars(model_, model);
    return (jlong) tracker;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_enjoy_opencvsample_FaceTracker_nativeDestroyObject(JNIEnv *env, jclass clazz, jlong thiz) {
    if (thiz != 0) {
        FaceTracker *tracker = reinterpret_cast<FaceTracker *>(thiz);
        tracker->tracker->stop();
        delete tracker;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_enjoy_opencvsample_FaceTracker_nativeSetSurface(JNIEnv *env, jclass clazz, jlong thiz,
                                                         jobject surface) {
    if (thiz != 0) {
        FaceTracker *tracker = reinterpret_cast<FaceTracker *>(thiz);
        if (!surface) {
            tracker->setANativeWindow(0);
            return;
        }
        tracker->setANativeWindow(ANativeWindow_fromSurface(env, surface));
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_enjoy_opencvsample_FaceTracker_nativeStart(JNIEnv *env, jclass clazz, jlong thiz) {
    if (thiz != 0) {
        FaceTracker *tracker = reinterpret_cast<FaceTracker *>(thiz);
        tracker->tracker->run();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_enjoy_opencvsample_FaceTracker_nativeStop(JNIEnv *env, jclass clazz, jlong thiz) {
    if (thiz != 0) {
        FaceTracker *tracker = reinterpret_cast<FaceTracker *>(thiz);
        tracker->tracker->stop();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_enjoy_opencvsample_FaceTracker_nativeDetect(JNIEnv *env, jclass clazz, jlong thiz,
                                                     jbyteArray inputImage_,
                                                     jint width, jint height,
                                                     jint rotationDegrees) {
    if (thiz == 0) {
        return;
    }
    FaceTracker *tracker = reinterpret_cast<FaceTracker *>(thiz);
    jbyte *inputImage = env->GetByteArrayElements(inputImage_, 0);

    //I420
    Mat src(height * 3 / 2, width, CV_8UC1, inputImage);

    // 转为RGBA
    cvtColor(src, src, CV_YUV2RGBA_I420);
    //旋转
    if (rotationDegrees == 90) {
        rotate(src, src, ROTATE_90_CLOCKWISE);
    } else if (rotationDegrees == 270) {
        rotate(src, src, ROTATE_90_COUNTERCLOCKWISE);
    }
    //镜像问题，可以使用此方法进行垂直翻转
//    flip(src,src,1);
    Mat gray;
    cvtColor(src, gray, CV_RGBA2GRAY);
    equalizeHist(gray, gray);

    tracker->tracker->process(gray);
    std::vector<Rect> faces;
    tracker->tracker->getObjects(faces);

    for (Rect face:faces) {
        LOGD("face info [x,y,width,height]=[%d,%d,%d,%d]",face.x,face.y,face.width,face.height);
        std::cout << face.tl() << std::endl; // cout [0,5] Point
        std::cout << face.br() << std::endl; // cout [4,13]
        std::cout << face.size().width << "," <<face.size().height << std::endl; // cout 4 8
        std::cout << face.area() << std::endl; //cout 32
        std::cout << face.empty() << std::endl; //cout 0

        //画矩形
        rectangle(src, face, Scalar(255, 0, 255));
    }

    tracker->draw(src);


    env->ReleaseByteArrayElements(inputImage_, inputImage, 0);
}