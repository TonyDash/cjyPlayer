package com.cjy.cjyplayer.activity

import android.widget.Button
import android.widget.EditText
import com.cjy.cjyplayer.R

class ChoiceFileActivity : BaseActivity() {

    private var editPath:EditText? = null
    private var btnOpenPath:Button? = null
    private var editLive:EditText? = null
    private var btnOpenLive:Button? = null

    override fun getLayoutId(): Int = R.layout.activity_choice_file

    override fun initLayout() {
        btnOpenPath = findViewById(R.id.btnOpenPath)
        editPath = findViewById(R.id.editLocalPath)
        btnOpenLive = findViewById(R.id.btnOpenLive)
        editLive = findViewById(R.id.editLive)
        btnOpenPath?.setOnClickListener {
            editPath?.run {
                open(this.text.toString())
                this@ChoiceFileActivity.finish()
            }
        }
        btnOpenLive?.setOnClickListener {
            editLive?.run {
                open(this.text.toString())
                this@ChoiceFileActivity.finish()
            }
        }
    }

    override fun initData() {
    }

    override fun loadData() {
    }

    external fun open(url:String)
}