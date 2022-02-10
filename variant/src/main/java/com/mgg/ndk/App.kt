package com.mgg.ndk

import android.app.Application
import timber.log.Timber

class App : Application() {

    override fun onCreate() {
        super.onCreate()
        if (BuildConfig.DEBUG) {
            Timber.plant(Timber.DebugTree())
        } else {
            Timber.plant(CrashReportingTree())
        }
        QtNative.setClassLoader(this.classLoader)
    }

    private class CrashReportingTree : Timber.Tree() {
        override fun log(priority: Int, message: String?, vararg args: Any?) {}
        override fun log(priority: Int, tag: String?, message: String, t: Throwable?) {
        }
    }
}