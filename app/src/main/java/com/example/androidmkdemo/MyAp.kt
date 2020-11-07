package com.example.androidmkdemo

import android.app.Application
import android.content.Context
import android.util.Log
import xcrash.XCrash

/**
 * 浙江集商优选电子商务有限公司
 * @author zenglw
 * @date   2020/11/7 10:36 PM
 */
class MyApp : Application() {
    override fun onCreate() {
        super.onCreate()
    }

    override fun attachBaseContext(base: Context) {
        super.attachBaseContext(base)
        XCrash
            .init(this, XCrash.InitParameters().apply {
                setNativeCallback { logPath, emergency ->
                    Log.e("MyApp", "attachBaseContext $logPath")
                }
                setJavaCallback { logPath, emergency ->
                    Log.e("MyApp", "attachBaseContext java callback $logPath")
                }
                setLogDir(getExternalFilesDir("xcrash").toString())
            })

    }
}
