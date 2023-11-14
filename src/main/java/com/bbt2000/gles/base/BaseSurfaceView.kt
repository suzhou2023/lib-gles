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

    var glContext: Long = 0
    private val handlerThread: HandlerThread by lazy { HandlerThread("gl-render").apply { start() } }
    val glHandler: Handler by lazy { Handler(handlerThread.looper) }

    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        glHandler.post {
            glContext = JniGL.nativeCreateGLContext(assetManager = context.assets)
            if (glContext <= 0) return@post
            JniGL.nativeEGLCreateSurface(glContext, holder.surface, 0)
            JniGL.nativeLoadVertices(glContext)
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {

    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        glHandler.post {
            JniGL.nativeDestroyGLContext(glContext)
        }
    }

    companion object {
        val TAG = BaseSurfaceView::class.simpleName
    }
}


















