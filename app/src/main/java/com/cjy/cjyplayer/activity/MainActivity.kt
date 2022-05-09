package com.cjy.cjyplayer.activity

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.SeekBar
import android.widget.TextView
import com.cjy.cjyplayer.R
import com.cjy.cjyplayer.databinding.ActivityMainBinding
import com.cjy.cjyplayer.listener.RequestPermissionListener

class MainActivity : BaseActivity(),Runnable, SeekBar.OnSeekBarChangeListener {

    private var btnOpen: Button? = null
    private var seekBar: SeekBar? = null
    private var th: Thread? = null

    override fun getLayoutId(): Int = R.layout.activity_main

    override fun initLayout() {
        btnOpen = findViewById(R.id.btnOpen)
        seekBar = findViewById(R.id.seekBar)
        btnOpen?.setOnClickListener {
            checkPermission(object : RequestPermissionListener {
                override fun permissionAllGranted() {
                    startActivity(Intent(this@MainActivity, ChoiceFileActivity::class.java))
                }

                override fun permissionDenied(list: MutableList<String>) {

                }

            })
        }
        seekBar?.setOnSeekBarChangeListener(this)

    }

    override fun initData() {
        checkPermission(object : RequestPermissionListener {
            override fun permissionAllGranted() {
                //启动播放进度线程
                th = Thread(this@MainActivity)
                th?.start()
            }

            override fun permissionDenied(list: MutableList<String>) {

            }

        })
    }

    override fun loadData() {

    }


    override fun run() {
        while (true) {
            seekBar?.setProgress((PlayPos() * 1000).toInt())
            try {
                Thread.sleep(40)
            } catch (e: InterruptedException) {
                e.printStackTrace()
            }
        }
    }

    external fun stringFromJNI(): String
    external fun PlayPos(): Double
    external fun Seek(pos: Double)

    companion object {
        // Used to load the 'cjyplayer' library on application startup.
        init {
            System.loadLibrary("cjyplayer")
        }
    }

    override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {

    }

    override fun onStartTrackingTouch(seekBar: SeekBar?) {
    }

    override fun onStopTrackingTouch(seekBar: SeekBar?) {
    }
}