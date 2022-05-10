//
// Created by chen jy on 2022/4/30.
//

#include "PlayerEGL.h"
#include <android/native_window_jni.h>
#include <EGL/egl.h>
#include <mutex>
#include "PlayerLog.h"

class CPlayerEGL : public PlayerEGL {
public:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLContext context = EGL_NO_CONTEXT;
    std::mutex mutex;

    void draw() override {
        mutex.lock();
        if (display == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) {
            mutex.unlock();
            return;
        }
        //交换surface到显示设备
        eglSwapBuffers(display, surface);
        mutex.unlock();
    }

    void close() override {
        mutex.lock();
        //去除绑定关系
        if (display == EGL_NO_DISPLAY) {
            mutex.unlock();
            return;
        }
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (surface != EGL_NO_SURFACE)
            eglDestroySurface(display, surface);
        if (context != EGL_NO_CONTEXT)
            eglDestroyContext(display, context);
        eglTerminate(display);

        //重新赋初始值
        display = EGL_NO_DISPLAY;
        surface = EGL_NO_SURFACE;
        context = EGL_NO_CONTEXT;
        mutex.unlock();
    }

    bool initEGL(void *win) override {
        ANativeWindow *nWin = static_cast<ANativeWindow *>(win);
        close();
        mutex.lock();
        //初始化EGL
        //1、获取EGLDisplay对象 显示设备
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            LOGD("eglGetDisplay failed!");
            mutex.unlock();
            return false;
        }
        LOGD("eglGetDisplay success!");
        //2、初始化display
        if (EGL_TRUE != eglInitialize(display, 0, 0)) {
            LOGD("eglInitialize not true");
            mutex.unlock();
            return false;
        }
        LOGD("eglInitialize TRUE");
        //3、获取配置并创建surface
        EGLint configSpec[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_NONE
        };
        EGLConfig config = 0;
        EGLint numConfigs = 0;
        if (EGL_TRUE != eglChooseConfig(display, configSpec, &config, 1, &numConfigs)) {
            LOGD("eglChooseConfig failed");
            mutex.unlock();
            return false;
        }
        //关联窗口和设备
        surface = eglCreateWindowSurface(display, config, nWin, NULL);
        //4、创建并打开EGL上下文
        const EGLint ctxAttr[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
        };
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttr);
        if (context == EGL_NO_CONTEXT) {
            LOGD("eglCreateContext FAILED");
            mutex.unlock();
            return false;
        }
        if (EGL_TRUE != eglMakeCurrent(display, surface, surface, context)) {
            LOGD("eglMakeCurrent NOT TRUE");
            mutex.unlock();
            return false;
        }
        mutex.unlock();
        return true;
    }
};

//保证单例
PlayerEGL *PlayerEGL::get() {
    static CPlayerEGL egl;
    return &egl;
}

