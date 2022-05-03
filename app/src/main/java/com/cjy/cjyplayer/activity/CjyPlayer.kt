package com.cjy.cjyplayer.activity

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.SurfaceHolder
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class CjyPlayer : GLSurfaceView, SurfaceHolder.Callback,GLSurfaceView.Renderer {

    constructor(context: Context):super(context){

    }

    constructor(context: Context, attributeSet: AttributeSet) : super(context, attributeSet) {

    }


    override fun surfaceCreated(holder: SurfaceHolder) {
         //初始化OpenGL EGL 显示
        initView(holder.surface)

        setRenderer(this)
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
    }

    external fun initView(surfaceView:Any);


    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        TODO("Not yet implemented")
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        TODO("Not yet implemented")
    }

    override fun onDrawFrame(gl: GL10?) {
        TODO("Not yet implemented")
    }
}