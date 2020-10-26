package com.example.androidmkdemo.utils

import android.app.Activity
import android.content.Context
import android.os.Looper
import android.util.Log
import android.widget.ActionMenuView
import android.widget.Toast
import com.example.androidmkdemo.listenner.IPlay
import com.example.androidmkdemo.listenner.PlayCallback

/**
 * 浙江集商优选电子商务有限公司
 * @author zenglw
 * @date   2020/10/26 10:03 AM
 */
class DNFFPlayer : IPlay, PlayCallback {


    companion object {

        init {
            System.loadLibrary("testffmpeg")
        }

        const val PLAY_URL = "http://ivi.bupt.edu.cn/hls/cctv1hd.m3u8"
    }

    private var mContext: Context? = null

    fun setContext(context: Context): DNFFPlayer {
        this.mContext = context
        return this
    }

    override fun prepare() {
        native_prepare(PLAY_URL)
    }

    override fun play() {

    }

    override fun stop() {
    }

    override fun pause() {
    }

    override fun destroy() {
    }

    override fun onPrepare() {
    }

    override fun onError(errorId: Int) {

        Log.e("DNFFPlayer", "onError Java 接收到回调 errorId: $errorId")
        (mContext as Activity).runOnUiThread {
            Toast.makeText(mContext, "errorId = $errorId", Toast.LENGTH_SHORT).show()
        }
    }

    external fun native_prepare(playUrl: String)
}