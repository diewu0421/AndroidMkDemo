package com.example.androidmkdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Toast
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        tv.text = myJni()
    }

    companion object {
        init {
//            System.loadLibrary("Test")
            System.loadLibrary("testffmpeg")
//            System.loadLibrary("native-lib")

        }
    }

    external fun myJni():String

    fun reflectMethod(str:String) {
        Toast.makeText(this, "我是toast $str", Toast.LENGTH_SHORT).show()
    }
}