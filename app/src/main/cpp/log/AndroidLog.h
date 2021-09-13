//
// Created by Administrator on 2021/9/13 0013.
//

#ifndef OPENCVSAMPLE_ANDROIDLOG_H
#define OPENCVSAMPLE_ANDROIDLOG_H

#include "android/log.h"

#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG, "my_tag", FORMAT, ##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, "my_tag", FORMAT, ##__VA_ARGS__);

#endif //OPENCVSAMPLE_ANDROIDLOG_H
