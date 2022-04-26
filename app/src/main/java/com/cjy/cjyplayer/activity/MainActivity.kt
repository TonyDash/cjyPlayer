package com.cjy.cjyplayer.activity

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.cjy.cjyplayer.R
import com.cjy.cjyplayer.databinding.ActivityMainBinding
import com.cjy.cjyplayer.listener.RequestPermissionListener

class MainActivity : BaseActivity() {

    private var sampleText:TextView? = null

    override fun getLayoutId(): Int = R.layout.activity_main

    override fun initLayout() {
        sampleText = findViewById(R.id.sample_text)
        sampleText?.setOnClickListener {
            initData()
        }
    }

    override fun initData() {

        checkPermission(object :RequestPermissionListener{
            override fun permissionAllGranted() {
                sampleText?.text = stringFromJNI()
            }

            override fun permissionDenied(list: MutableList<String>) {

            }

        })
    }

    override fun loadData() {

    }

    external fun stringFromJNI():String

    companion object {
        // Used to load the 'cjyplayer' library on application startup.
        init {
            System.loadLibrary("cjyplayer")
        }
    }
}