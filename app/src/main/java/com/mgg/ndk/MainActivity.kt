package com.mgg.ndk

import android.annotation.SuppressLint
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.mgg.log.Logger
import com.mgg.ndk.databinding.ActivityMainBinding
import io.realm.internal.core.NativeRealmAny
import timber.log.Timber
import java.util.*

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    @SuppressLint("TimberArgCount", "BinaryOperationInTimber")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI()
        val nativeCreateNull = NativeRealmAny()
        Timber.e("nativeCreateNull:${nativeCreateNull}")
        val nativeCreateBoolean = NativeRealmAny(true)
        Timber.e("nativeCreateBoolean:${nativeCreateBoolean}")
        val nativeCreateLong = NativeRealmAny(Integer.valueOf(1000))
        Timber.e("nativeCreateLong:${nativeCreateLong}")

        val nativeCreateFloat = NativeRealmAny(1000.00f)
        Timber.e("nativeCreateFloat:$nativeCreateFloat")
        val nativeCreateDouble = NativeRealmAny(10000.000)
        Timber.e("nativeCreateDouble:$nativeCreateDouble")

        val nativeCreateString = NativeRealmAny(getString(R.string.app_name))
        Timber.e("nativeCreateString:$nativeCreateString")

        val testVariantTest = testVariantTest()
        Timber.e("testVariantTest:$testVariantTest")

        testScalarTypes()
        val date = Date()
        val nativeCreateDate = NativeRealmAny(date)
        Timber.e("nativeCreateDate:${nativeCreateDate.asDate()}" + " date:${date.time}")

        val uuid = UUID.randomUUID()
        val nativeCreateUUID = NativeRealmAny(uuid)
        Timber.e("nativeCreateUUID:${nativeCreateUUID.asUUID()}" + " uuid:${uuid}")

        val objectId = org.bson.types.ObjectId.get()
        val nativeCreateObjectId = NativeRealmAny(objectId)
        Timber.e("nativeCreateObjectId:${nativeCreateObjectId.asObjectId()}" + " objectId:${objectId}")
        Logger.testPrintLog()
    }

    /**
     * A native method that is implemented by the 'ndk' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun testVariantTest(): Any

    external fun testScalarTypes()

    companion object {
        // Used to load the 'ndk' library on application startup.
        init {
            System.loadLibrary("ndk")
        }
    }
}