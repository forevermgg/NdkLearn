package com.mgg.breakpad

class Breakpad {

    companion object {
        // Used to load the 'breakpad' library on application startup.
        init {
            System.loadLibrary("breakpad")
        }

        @JvmStatic
        external fun initBreakpad(path: String)

        @JvmStatic
        external fun unitBreakpad(path: String)
    }
}