package com.bbt2000.gles.base

import android.content.Context
import android.os.Handler
import android.os.HandlerThread
import android.util.AttributeSet
import android.view.SurfaceHolder
import com.bbt2000.gles.jni.JniGL
import com.bbt2000.gles.widget.AutoFitSurfaceView


/**
 *  author : suzhou
 *  date : 2023/11/12
 *  description :
 */
open class BaseSurfaceView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null
) : AutoFitSurfaceView(context, attrs), SurfaceHolder.Callback {

    private lateinit var handlerThread: HandlerThread
    lateinit var glHandler: Handler
    var glContext: Long = 0

    fun addCallback() {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        handlerThread = HandlerThread("gl-render").apply { start() }
        glHandler = Handler(handlerThread.looper)

        glHandler.post {
            glContext = JniGL.createGLContext(assetManager = context.assets)
            if (glContext <= 0) return@post
            JniGL.createEGLSurface(glContext, holder.surface, 0)
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        glHandler.post {
            JniGL.setWindowSize(glContext, width, height)
        }
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        glHandler.post {
            JniGL.destroyGLContext(glContext)
        }
        glHandler.looper.quitSafely()
    }
}


















