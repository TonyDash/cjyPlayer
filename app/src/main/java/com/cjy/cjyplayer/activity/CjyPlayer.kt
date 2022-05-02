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
        initView(holder.surface)
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, w: Int, h: Int) {
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
    }

    external fun initView(surfaceView:Any);
}