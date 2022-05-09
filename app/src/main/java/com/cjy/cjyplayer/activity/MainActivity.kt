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

class MainActivity : BaseActivity() {

    private var btnOpen: Button? = null
    private var seekBar: SeekBar? = null

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

    }

    override fun initData() {
        checkPermission(object : RequestPermissionListener {
            override fun permissionAllGranted() {

            }

            override fun permissionDenied(list: MutableList<String>) {

            }

        })
    }

    override fun loadData() {

    }

    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'cjyplayer' library on application startup.
        init {
            System.loadLibrary("cjyplayer")
        }
    }
}