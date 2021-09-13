
cmake_minimum_required(VERSION
           3.10.2)

project("opencvsample")

#add_subdirectory(libyuv)

add_library(
        native-lib
        SHARED
        native-lib.cpp
        FaceTracker.cpp
)
# -L 指定库的查找路径
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")

#设置头文件查找路径
include_directories(include)


target_link_libraries(
        native-lib
        opencv_java4
        android
        log)