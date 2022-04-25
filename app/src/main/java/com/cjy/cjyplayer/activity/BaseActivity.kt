package com.cjy.cjyplayer.activity

import android.Manifest
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.cjy.cjyplayer.listener.RequestPermissionListener
import com.permissionx.guolindev.PermissionX

abstract class BaseActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(getLayoutId())
        initLayout()
        initData()
    }

    override fun onStart() {
        super.onStart()
        loadData()
    }

    /***
     * 布局xml文件的id
     */
    abstract fun getLayoutId(): Int

    /***
     * 初始化布局
     */
    abstract fun initLayout()

    /***
     * 初始化数据
     * 如：intent传递的bundle参数、初始值等
     */
    abstract fun initData()

    /***
     * 加载数据
     * 如：请求接口数据、读取数据库数据等
     */
    abstract fun loadData()

    /***
     * 动态请求权限
     * listener：回调函数，根据权限请求结果，执行对应的回调方法
     */
    private fun checkPermission(listener: RequestPermissionListener) {
        PermissionX.init(this)
            .permissions(
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE
            )
            .request { allGranted, grantedList, deniedList ->
                if (allGranted) {
                    listener.permissionAllGranted()
                } else {
                    Toast.makeText(this, "请打开权限！ $deniedList", Toast.LENGTH_SHORT).show()
                    listener.permissionDenied(deniedList)
                }
            }
    }

}