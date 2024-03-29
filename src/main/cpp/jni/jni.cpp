/**
 *  author : suzhou
 *  date : 2023/7/16 
 *  description : 
 */

#include <jni.h>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <android/native_window_jni.h>
#include <android/asset_manager_jni.h>
#include <cstring>
#include "GLContext.h"
#include "egl_util.h"
#include "gl_util.h"


extern "C"
JNIEXPORT jlong JNICALL
Java_com_bbt2000_gles_jni_JniGL_createGLContext(JNIEnv *env, jobject thiz, jobject asset_manager) {
    auto *glContext = new GLContext();
    EGLBoolean ret = egl_createContext(glContext, EGL_NO_CONTEXT);
    if (ret <= 0) {
        delete glContext;
        return reinterpret_cast<jlong>(nullptr);
    }
    // 保存assetManager
    glContext->assetManager = AAssetManager_fromJava(env, asset_manager);
    return reinterpret_cast<jlong>(glContext);
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_bbt2000_gles_jni_JniGL_createEGLSurface(JNIEnv *env, jobject thiz, jlong gl_context, jobject surface,
                                                 jint index) {
    if (gl_context <= 0) return false;
    auto *glContext = reinterpret_cast<GLContext *>(gl_context);
    return glContext->createEGLSurface(env, surface, index);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_bbt2000_gles_jni_JniGL_loadVertices(
        JNIEnv *env, jobject thiz, jlong gl_context) {
    if (gl_context <= 0) return;
    auto *glContext = reinterpret_cast<GLContext *>(gl_context);
    return glContext->loadVertices();
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_bbt2000_gles_jni_JniGL_createProgram(
        JNIEnv *env, jobject thiz, jlong gl_context, jstring v_name, jstring f_name, jint index) {
    if (gl_context <= 0) return -1;
    auto *glContext = reinterpret_cast<GLContext *>(gl_context);
    return glContext->createProgram(env, thiz, v_name, f_name, index);
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_bbt2000_gles_jni_JniGL_createOESTexture(
        JNIEnv *env, jobject thiz, jlong gl_context) {
    if (gl_context <= 0) return -1;
    auto *glContext = reinterpret_cast<GLContext *>(gl_context);
    return glContext->createOesTexture();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_bbt2000_gles_jni_JniGL_createFbo(
        JNIEnv *env, jobject thiz, jlong gl_context, jint texture_w, jint texture_h, jint index) {
    if (gl_context <= 0) return;
    auto *glContext = reinterpret_cast<GLContext *>(gl_context);
    return glContext->createFbo(texture_w, texture_h, index);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_bbt2000_gles_jni_JniGL_setWindowSize(JNIEnv *env, jobject thiz, jlong gl_context, jint width, jint height) {
    if (gl_context <= 0) return;
    auto *glContext = reinterpret_cast<GLContext *>(gl_context);
    return glContext->setWindowSize(width, height);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_bbt2000_gles_jni_JniGL_configMatrix(
        JNIEnv *env,
        jobject thiz,
        jlong gl_context,
        jint program_index,
        jint frame_w,
        jint frame_h,
        jint window_w,
        jint window_h,
        jint scale_type,
        jboolean rotate
) {
    if (gl_context <= 0) return;
    auto *glContext = reinterpret_cast<GLContext *>(gl_context);
    glContext->configMatrix(program_index, frame_w, frame_h, window_w, window_h, scale_type, rotate);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_bbt2000_gles_jni_JniGL_destroyGLContext(JNIEnv *env, jobject thiz, jlong gl_context) {
    if (gl_context <= 0) return;
    auto *glContext = reinterpret_cast<GLContext *>(gl_context);
    delete glContext;
}






































