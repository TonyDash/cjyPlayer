# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("cjyplayer")

# 定义LIBS_DIR
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})
#set(LIBS_DIR ${CMAKE_SOURCE_DIR}/../../libs/${ANDROID_ABI})
# 添加头文件目录
include_directories(src/main/cpp/include/)
#  添加ffmpeg相关的so库
# 添加库——外部引入的库
# 库名称：avcodec（不需要包含前缀lib）
# 库类型：SHARED，表示动态库，后缀为.so（如果是STATIC，则表示静态库，后缀为.a）
# IMPORTED表明是外部引入的库
add_library( avcodec
        SHARED
        IMPORTED)
# 设置目标属性
# 设置avcodec目标库的IMPORTED_LOCATION属性，用于说明引入库的位置
# 还可以设置其他属性，格式：PROPERTIES key value
set_target_properties( avcodec
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libavcodec.so)

add_library( avfilter
        SHARED
        IMPORTED)
set_target_properties( avfilter
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libavfilter.so)

add_library( avformat
        SHARED
        IMPORTED)
set_target_properties( avformat
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libavformat.so)

add_library( avutil
        SHARED
        IMPORTED)
set_target_properties( avutil
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libavutil.so)

add_library( swresample
        SHARED
        IMPORTED)
set_target_properties( swresample
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libswresample.so)

add_library( swscale
        SHARED
        IMPORTED)
set_target_properties( swscale
        PROPERTIES IMPORTED_LOCATION
        ${LIBS_DIR}/libswscale.so)


# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# 配置目标so库编译信息
add_library( # Sets the name of the library.
        cjyplayer

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp
        src/main/cpp/FFDemux.cpp
        src/main/cpp/IDemux.cpp
        src/main/cpp/PlayerData.cpp
        src/main/cpp/PlayerLog.cpp
        src/main/cpp/PlayerThread.cpp
        src/main/cpp/IObserver.cpp
        src/main/cpp/IDecode.cpp
        src/main/cpp/FFDecode.cpp
        src/main/cpp/PlayerParameter.cpp
        src/main/cpp/GLVideoView.cpp
        src/main/cpp/IVideoView.cpp
        src/main/cpp/PlayerTexture.cpp
        src/main/cpp/PlayerEGL.cpp
        src/main/cpp/PlayerShader.cpp
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.
# 查找代码中使用到的系统库
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        # 指定目标库，native-lib 是在上面 add_library 中配置的目标库
        cjyplayer
        #连接 FFmpeg 相关的库
        avutil
        swresample
        avcodec
        avfilter
        swscale
        avformat
        #android库
        android
        #OpenGl相关的库
        OpenSLES
        #音频相关的库
        GLESv2 EGL
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})