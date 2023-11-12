package com.bbt2000.gles.base

import android.content.Context
import android.os.Handler
import android.os.HandlerThread
import android.util.AttributeSet
import android.view.SurfaceHolder
import com.bbt2000.gles.jni.JniGL
import com.bbt2000.gles.widget.AutoFitSurfaceView
import java.nio.ByteBuffer


/**
 *  author : suzhou
 *  date : 2023/11/12
 *  description :
 */
class BaseSurfaceView(
    context: Context,
    attrs: AttributeSet? = null
) : AutoFitSurfaceView(context, attrs), SurfaceHolder.Callback {

    private var glContext: Long = 0
    private val handlerThread: HandlerThread by lazy { HandlerThread("gl-render").apply { start() } }
    private val handler: Handler = Handler(handlerThread.looper)

    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        handler.post {
            glContext = JniGL.nativeCreateGLContext(assetManager = context.assets)
            if (glContext <= 0) return@post
            JniGL.nativeEGLCreateSurface(glContext, holder.surface, 0)
            JniGL.nativeLoadVertices(glContext)
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {

    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        handler.post {
            JniGL.nativeDestroyGLContext(glContext)
        }
    }

    companion object {
        val TAG = BaseSurfaceView::class.simpleName
    }
}


















