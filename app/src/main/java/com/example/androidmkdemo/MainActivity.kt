package com.example.androidmkdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Toast

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        myJni()
    }

    companion object {
        init {
//            System.loadLibrary("Test")
            System.loadLibrary("native-lib")
        }
    }

    external fun myJni()

    fun reflectMethod(str:String) {
        Toast.makeText(this, "我是toast $str", Toast.LENGTH_SHORT).show()
    }
}