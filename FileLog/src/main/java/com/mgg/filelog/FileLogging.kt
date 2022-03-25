package com.mgg.filelog

class FileLogging {

    /**
     * A native method that is implemented by the 'filelog' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'filelog' library on application startup.
        init {
            System.loadLibrary("filelog")
        }
    }
}