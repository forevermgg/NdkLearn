package com.mgg.filelog

class Log {

    companion object {
        // Used to load the 'filelog' library on application startup.
        init {
            System.loadLibrary("filelog")
        }
    }

    private val TAG = "Forever_log"

    // Global log instance.
    private var sLogInstance: Log? = null

    // Lock which is used to arbitrate access to the global logger.
    private val sLock = Any()

    // Flag which is set to false if we detect the nativeLog method has been unregistered.
    private var nativeLogAvailable = true

    /**
     * See android.util.Log.d().
     */
    fun d(tag: String, msg: String): Int {
        safeNativeLog(android.util.Log.DEBUG, tag, msg)
        return 0
    }

    /**
     * See android.util.Log.v().
     */
    fun v(tag: String, msg: String): Int {
        safeNativeLog(android.util.Log.VERBOSE, tag, msg)
        return 0
    }

    /**
     * See android.util.Log.i().
     */
    fun i(tag: String, msg: String): Int {
        safeNativeLog(android.util.Log.INFO, tag, msg)
        return 0
    }

    /**
     * See android.util.Log.w().
     */
    fun w(tag: String, msg: String): Int {
        safeNativeLog(android.util.Log.WARN, tag, msg)
        return 0
    }

    /**
     * See android.util.Log.e().
     */
    fun e(tag: String, msg: String): Int {
        safeNativeLog(android.util.Log.ERROR, tag, msg)
        return 0
    }

    /**
     * See android.util.Log.wtf().
     */
    fun wtf(tag: String, msg: String): Int {
        safeNativeLog(android.util.Log.ERROR, tag, msg)
        return 0
    }

    /**
     * Get / create the Log singleton.
     */
    fun getInstance(): Log? {
        synchronized(sLock) {
            if (sLogInstance == null) {
                android.util.Log.d(TAG, "Creating Log instance.")
                sLogInstance = Log()
                sLogInstance?.safeNativeLogInternal(
                    android.util.Log.DEBUG, TAG, sLogInstance!!.javaClass.toString()
                )
            }
        }
        return sLogInstance
    }

    /**
     * Dereference the log singleton.
     */
    fun shutdown() {
        synchronized(sLock) { sLogInstance = null }
    }

    /**
     * Static wrapper around safeNativeLogInternal which gets / creates an instance of the log
     * class then calls safeNativeLogInternal to ideally log via nativeLog or fall back to
     * android.util.Log.
     */
    private fun safeNativeLog(level: Int, tag: String, msg: String) {
        synchronized(sLock) { getInstance()!!.safeNativeLogInternal(level, tag, msg) }
    }

    /**
     * Wrapper for nativeLog that will redirect log messages to android.util.Log if a native method
     * isn't registered for the Log.nativeLog method.
     */
    private fun safeNativeLogInternal(level: Int, tag: String, msg: String) {
        if (nativeLogAvailable) {
            try {
                nativeLog(level, tag, msg)
            } catch (e: UnsatisfiedLinkError) {
                nativeLogAvailable = false
                android.util.Log.w(
                    TAG, String.format(
                        "nativeLog not registered, falling back to android.util.Log (%s)",
                        e.toString()
                    )
                )
            }
        }
        if (!nativeLogAvailable) {
            when (level) {
                android.util.Log.VERBOSE -> android.util.Log.v(tag, msg)
                android.util.Log.INFO -> android.util.Log.i(tag, msg)
                android.util.Log.WARN -> android.util.Log.w(tag, msg)
                android.util.Log.ERROR -> android.util.Log.e(tag, msg)
                android.util.Log.DEBUG -> android.util.Log.d(tag, msg)
                else -> android.util.Log.d(tag, msg)
            }
        }
    }

    private external fun nativeLog(level: Int, tag: String, msg: String)
}