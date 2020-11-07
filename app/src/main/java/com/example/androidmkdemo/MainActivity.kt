package com.example.androidmkdemo

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.example.androidmkdemo.utils.DNFFPlayer
import com.sample.breakpad.BreakpadInit
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : AppCompatActivity() {

    val dnFFmpegPlayer by lazy { DNFFPlayer().setContext(this)}



    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        if (ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
            )
            != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                this, arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
                WRITE_EXTERNAL_STORAGE_REQUEST_CODE
            )
        } else {

            initExternalReportPath()
        }

        dnFFmpegPlayer.setSurfaceView(surfaceView)

        dnFFmpegPlayer.setOnPlayerStateChangeListener(object :
            DNFFPlayer.OnPlayerStateChangeListener {
            override fun onPrepare() {
                runOnUiThread {
                    Toast.makeText(this@MainActivity, "开始播放", Toast.LENGTH_SHORT).show()
                }
                dnFFmpegPlayer.play()
            }

        })


        prepare_btn.setOnClickListener {
            dnFFmpegPlayer.prepare()
        }

        stop_btn.setOnClickListener {
            dnFFmpegPlayer.stop()
        }

        refresh_btn.setOnClickListener {
            Log.e("MainActivity","onCreate refresh")
        }
    }

    companion object {
        init {
//            System.loadLibrary("Test")
            System.loadLibrary("testffmpeg")
//            System.loadLibrary("native-lib")
        }
        val WRITE_EXTERNAL_STORAGE_REQUEST_CODE = 100
    }

    private var externalReportPath: File? = null

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        initExternalReportPath()
    }
    private fun initExternalReportPath() {
        externalReportPath =
            File(externalCacheDir?.absolutePath, "crashDump")
        if (externalReportPath?.exists() == false) {
            externalReportPath?.mkdirs()
        }
        BreakpadInit.initBreakpad(externalReportPath!!.absolutePath)

    }

    external fun myJni():String
    fun onPrepare(){}

    fun reflectMethod(str:String) {
        Toast.makeText(this, "我是toast $str", Toast.LENGTH_SHORT).show()
    }

    override fun onResume() {
        super.onResume()
//        dnFFmpegPlayer.prepare()
    }

//    override fun onStop() {
//        super.onStop()
//        dnFFmpegPlayer.stop()
//    }

    override fun onPause() {
        super.onPause()
        dnFFmpegPlayer.pause()
    }

    override fun onDestroy() {
        super.onDestroy()
        dnFFmpegPlayer.destroy()
    }
}