package com.example.androidmkdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import com.example.androidmkdemo.utils.DNFFPlayer
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : AppCompatActivity() {

    val dnFFmpegPlayer by lazy { DNFFPlayer().setContext(this)}


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
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

        play_btn.setOnClickListener {
            Toast.makeText(this, "测试有没有卡死", Toast.LENGTH_SHORT).show()
        }
    }

    companion object {
        init {
//            System.loadLibrary("Test")
            System.loadLibrary("testffmpeg")
//            System.loadLibrary("native-lib")
        }
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

    override fun onStop() {
        super.onStop()
        dnFFmpegPlayer.stop()
    }

    override fun onPause() {
        super.onPause()
        dnFFmpegPlayer.pause()
    }

    override fun onDestroy() {
        super.onDestroy()
        dnFFmpegPlayer.destroy()
    }
}