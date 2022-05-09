package com.cjy.cjyplayer.activity

import android.widget.Button
import android.widget.EditText
import com.cjy.cjyplayer.R

class ChoiceFileActivity : BaseActivity() {

    private var editPath:EditText? = null
    private var btnOpenPath:Button? = null

    override fun getLayoutId(): Int = R.layout.activity_choice_file

    override fun initLayout() {
        btnOpenPath = findViewById(R.id.btnOpenPath)
        editPath = findViewById(R.id.editLocalPath)
        btnOpenPath?.setOnClickListener {
            editPath?.run {

            }
        }
    }

    override fun initData() {
    }

    override fun loadData() {
    }
}