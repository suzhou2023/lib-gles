package com.bbt2000.gles.jni

import android.content.res.AssetManager

/**
 *  author : sz
 *  date : 2023/7/31
 *  description :
 */
object JniGL {
    init {
        System.loadLibrary("gl-common")
    }

    // 创建GLContext
    external fun createGLContext(assetManager: AssetManager): Long

    // 创建EGLSurface
    external fun createEGLSurface(glContext: Long, surface: Any, index: Int = 0): Boolean

    // 加载顶点坐标和纹理坐标
    external fun loadVertices(glContext: Long)

    // 创建着色器程序
    external fun createProgram(glContext: Long, vName: String, fName: String, index: Int = 0): Int

    // 创建OES纹理
    external fun createOESTexture(glContext: Long): Int

    // 创建FBO
    external fun createFbo(glContext: Long, textureW: Int, textureH: Int, index: Int)

    // 设置窗口尺寸
    external fun setWindowSize(glContext: Long, width: Int, height: Int)

    // 矩阵配置：根据图形和窗口的尺寸配置合适的矩阵
    external fun configMatrix(
        glContext: Long,
        programIndex: Int = 0, // 程序的索引
        frameW: Int, // 帧宽度
        frameH: Int, // 帧高度
        windowW: Int, // 窗口宽度
        windowH: Int, // 窗口高度
        scaleType: Int = 1, // 缩放类型：1 - crop，2 - fit
        rotate: Boolean = true, // 是否旋转
    )

    // 销毁GLContext
    external fun destroyGLContext(glContext: Long)
}

























