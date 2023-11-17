package com.bbt2000.gles.jni

import android.content.res.AssetManager

/**
 *  author : sz
 *  date : 2023/7/31
 *  description :
 */
object JniGL {
    init {
        System.loadLibrary("gl-bbt")
    }

    /**
     * 创建GL context
     *
     * @param otherGLContext
     * @param assetManager
     * @return
     */
    external fun nativeCreateGLContext(otherGLContext: Long = 0, assetManager: AssetManager): Long

    /**
     * 创建EGL surface
     *
     * @param glContext
     * @param surface
     * @param index
     * @return
     */
    external fun nativeEGLCreateSurface(glContext: Long, surface: Any, index: Int = 0): Boolean

    /**
     * 创建并链接着色器程序
     *
     * @param glContext
     */
    external fun nativeCreateProgram(glContext: Long, vName: String, fName: String, index: Int = 0)

    /**
     * 加载顶点属性数组
     *
     * @param glContext
     */
    external fun nativeLoadVertices(glContext: Long)

    /**
     * 通知native窗口变化
     *
     * @param glContext
     * @param format
     * @param width
     * @param height
     */
    external fun nativeSurfaceChanged(glContext: Long, format: Int, width: Int, height: Int)

    /**
     * 创建OES纹理
     *
     * @param glContext
     * @return
     */
    external fun nativeCreateOESTexture(glContext: Long): Int

    /**
     * 创建FBO
     *
     * @param glContext
     * @param width
     * @param height
     */
    external fun nativeCreateFbo(glContext: Long, width: Int, height: Int)

    /**
     * 设置变换矩阵
     *
     * @param glContext
     * @param matrix
     */
    external fun nativeSetMatrix(glContext: Long, matrix: FloatArray)


    // 矩阵设置：根据图形和窗口的尺寸设置合适的矩阵
    external fun setMatrix(
        glContext: Long,
        programIndex: Int = 0, // 程序的索引
        frameW: Int, // 帧宽度
        frameH: Int, // 帧高度
        windowW: Int, // 窗口宽度
        windowH: Int, // 窗口高度
        scaleType: Int = 1, // 缩放类型：1 - crop，2 - fit
        rotate: Boolean = true, // 是否旋转
    )

    /**
     * 绘制
     *
     * @param glContext
     */
    external fun nativeDrawFrame(glContext: Long)

    /**
     * 销毁glContext
     *
     * @param glContext
     */
    external fun nativeDestroyGLContext(glContext: Long)
}