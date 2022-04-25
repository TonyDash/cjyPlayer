package com.cjy.cjyplayer.listener

interface RequestPermissionListener {
    fun permissionAllGranted()
    fun permissionDenied(list: MutableList<String>)
}