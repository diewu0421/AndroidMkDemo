package com.example.androidmkdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Toast
import com.example.androidmkdemo.utils.DNFFPlayer
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    val dnFFmpegPlayer by lazy { DNFFPlayer().setContext(this)}


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        prepare_btn.setOnClickListener {

            dnFFmpegPlayer.prepare()

        }

        play_btn.setOnClickListener {
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
}