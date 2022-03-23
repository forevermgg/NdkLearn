package com.mgg.breakpad

class NativeLib {

    /**
     * A native method that is implemented by the 'breakpad' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'breakpad' library on application startup.
        init {
            System.loadLibrary("breakpad")
        }
    }
}