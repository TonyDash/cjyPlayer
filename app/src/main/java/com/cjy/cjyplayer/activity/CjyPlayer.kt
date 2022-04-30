package com.cjy.cjyplayer.activity

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.SurfaceHolder

class CjyPlayer : GLSurfaceView, SurfaceHolder.Callback {

    constructor(context: Context):super(context){

    }

    constructor(context: Context, attributeSet: AttributeSet) : super(context, attributeSet) {

    }


    override fun surfaceCreated(holder: SurfaceHolder) {
         //初始化OpenGL EGL 显示
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {
        super.surfaceChanged(holder, format, w, h)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        super.surfaceDestroyed(holder)
    }

    external fun initView(surfaceView:Any);
}