package com.example.androidmkdemo.utils

import android.app.Activity
import android.content.Context
import android.os.Looper
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.widget.ActionMenuView
import android.widget.Toast
import com.example.androidmkdemo.listenner.IPlay
import com.example.androidmkdemo.listenner.PlayCallback

/**
 * 浙江集商优选电子商务有限公司
 * @author zenglw
 * @date   2020/10/26 10:03 AM
 */
class DNFFPlayer : IPlay, PlayCallback, SurfaceHolder.Callback {


    companion object {

        init {
            System.loadLibrary("testffmpeg")
        }

//        const val PLAY_URL = "${}"
//        const val PLAY_URL = "rtmp://live.hkstv.hk.lxdns.com/live/hks"
    }

    private var mContext: Context? = null
    private var mHolder: SurfaceHolder? = null

    fun setContext(context: Context): DNFFPlayer {
        this.mContext = context
        return this
    }

    fun setSurfaceHolder(holder: SurfaceHolder): DNFFPlayer {
        this.mHolder = holder
        return this
    }

    fun setSurfaceView(surfaceView: SurfaceView): DNFFPlayer {
        mHolder?.removeCallback(this)
        mHolder = surfaceView.holder
        mHolder?.addCallback(this)
        return this
    }

    override fun prepare() {
//        native_prepare(mContext?.externalCacheDir?.absolutePath + "/big_buck_bunny.mp4")
//        native_prepare("http://ivi.bupt.edu.cn/hls/cctv3hd.m3u8")
        native_prepare("${mContext?.externalCacheDir}/test.mp4")

    }

    override fun play() {
        native_start()
    }

    override fun stop() {
        mHolder?.removeCallback(this)
        mHolder = null
        native_stop()
    }

    fun seekTo(progress: Int) {
        native_seekTo(progress)
    }

    fun onProgress(progress: Int) {
        onProgressChangeListener?.invoke(progress)
    }
    fun getDuration(): Int {
        return native_get_duration()
    }

    private external fun native_stop()

    override fun pause() {
    }

    override fun destroy() {
        native_release()
    }

    override fun onPrepare() {
        onPlayerStateChangeListener?.onPrepare()
    }

    override fun onError(errorId: Int) {

        Log.e("DNFFPlayer", "onError Java 接收到回调 errorId: $errorId")
        (mContext as Activity).runOnUiThread {
            Toast.makeText(mContext, "errorId = $errorId", Toast.LENGTH_SHORT).show()
        }
    }


    external fun native_prepare(playUrl: String)
    external fun native_start()
    external fun native_setSurface(surface: Surface)
    external fun native_release()
    external fun native_get_duration(): Int
    external fun native_seekTo(progress: Int)

    override fun surfaceCreated(holder: SurfaceHolder) {

    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        native_setSurface(holder.surface)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
    }

    private var onPlayerStateChangeListener: OnPlayerStateChangeListener? = null
    private var onProgressChangeListener: ((Int) -> Unit)? = null

    fun setOnPlayerStateChangeListener(onPlayerStateChangeListener: OnPlayerStateChangeListener) {
        this.onPlayerStateChangeListener = onPlayerStateChangeListener
    }
//    fun setOnProgressChangeListener(onProgressChangeListener: OnProgressChangeListener) {
//        this.onProgressChangeListener = onProgressChangeListener
//    }
    fun setOnProgressChangeListener(progressListener: (Int) -> Unit) {
        this.onProgressChangeListener = progressListener
    }
    interface OnPlayerStateChangeListener {

        fun onPrepare()
    }

//    interface OnProgressChangeListener {
//        fun onProgressChange(progress: Int)
//    }
}