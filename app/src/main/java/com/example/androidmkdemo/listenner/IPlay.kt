package com.example.androidmkdemo.listenner

/**
 * 浙江集商优选电子商务有限公司
 * @author zenglw
 * @date   2020/10/26 10:37 AM
 */
interface  IPlay {
    fun prepare()
    fun  play()

    fun stop()

    fun pause()

    fun destroy()
}

interface PlayCallback {

    fun onPrepare()

    fun onError(errorId: Int)

}