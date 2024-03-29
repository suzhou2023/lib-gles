/**
 *  author : suzhou
 *  date : 2023/11/12 
 *  description : 
 */
#include "egl_util.h"
#include "android_log.h"

#include <EGL/eglext.h>
#include <android/native_window_jni.h>


bool egl_createContext(GLContext *glContext, EGLContext shareContext) {
    glContext->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (glContext->eglDisplay == EGL_NO_DISPLAY) {
        LOGE("eglGetDisplay failed.");
        return false;
    }

    if (eglInitialize(glContext->eglDisplay, nullptr, nullptr) != EGL_TRUE) {
        LOGE("eglInitialize failed");
        eglTerminate(glContext->eglDisplay);
        glContext->eglDisplay = nullptr;
        return false;
    }

    EGLConfig eglConfig;
    EGLint configNum;
    EGLint configSpec[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RECORDABLE_ANDROID, 1,
            EGL_NONE
    };

    if (eglChooseConfig(
            glContext->eglDisplay,
            configSpec,
            &eglConfig,
            1,
            &configNum) != EGL_TRUE) {
        LOGE("eglChooseConfig failed.");
        eglTerminate(glContext->eglDisplay);
        glContext->eglDisplay = nullptr;
        return false;
    } else {
        glContext->eglConfig = eglConfig;
    }

    const EGLint ctxAttr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    glContext->eglContext = eglCreateContext(
            glContext->eglDisplay,
            eglConfig,
            shareContext,
            ctxAttr
    );
    if (glContext->eglContext == EGL_NO_CONTEXT) {
        LOGE("egl_createContext failed.");
        eglTerminate(glContext->eglDisplay);
        glContext->eglDisplay = nullptr;
        return false;
    }

    LOGI("egl_createContext success: %p", glContext->eglContext);
    return true;
}

bool egl_createSurface(JNIEnv *env, GLContext *glContext, jobject surface, EGLint index) {
    if (index >= sizeof(glContext->eglSurface) / sizeof(glContext->eglSurface[0])) {
        return false;
    }

    // 销毁之前的EGLSurface
    if (glContext->eglSurface[index] != nullptr) {
        EGLBoolean ret = eglDestroySurface(glContext->eglDisplay, glContext->eglSurface[index]);
        if (ret == false) {
            LOGI("eglDestroySurface success: %p", glContext->eglSurface[index]);
        }
        glContext->eglSurface[index] = nullptr;
    }

    ANativeWindow *nativeWindow = ANativeWindow_fromSurface(env, surface);
    glContext->eglSurface[index] = eglCreateWindowSurface(
            glContext->eglDisplay,
            glContext->eglConfig,
            nativeWindow,
            nullptr);

    ANativeWindow_release(nativeWindow);
    if (glContext->eglSurface[index] == EGL_NO_SURFACE) {
        LOGE("egl_createSurface failed.");
        return false;
    }

    LOGI("egl_createSurface success: %p", glContext->eglSurface[index]);
    return true;
}

bool egl_makeCurrent(GLContext *glContext, EGLSurface eglSurface) {
    EGLBoolean ret = eglMakeCurrent(
            glContext->eglDisplay,
            eglSurface,
            eglSurface,
            glContext->eglContext
    );

    if (ret != EGL_TRUE) {
        LOGE("egl_makeCurrent failed.");
        return false;
    }

    return true;
}






































