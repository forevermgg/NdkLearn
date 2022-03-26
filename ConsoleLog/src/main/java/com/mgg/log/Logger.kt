package com.mgg.log

class Logger{

    companion object {
        // Used to load the 'log' library on application startup.
        init {
            System.loadLibrary("logging")
            initConsoleLog()
        }

        @JvmStatic
        private external fun initConsoleLog()

        @JvmStatic
        external fun initLogger(pathDir: String)

        @JvmStatic
        external fun testPrintLog()
    }
}