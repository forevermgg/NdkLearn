package com.mgg.ndk

import android.annotation.SuppressLint
import android.content.DialogInterface
import android.content.DialogInterface.BUTTON_NEGATIVE
import android.content.DialogInterface.BUTTON_POSITIVE
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
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

        testJson()
        // testLog()

        binding.sampleText.setOnClickListener {
            testDialog { dialog, which ->
                when (which) {
                    BUTTON_POSITIVE -> {
                        Toast.makeText(this@MainActivity,"确认被点击", Toast.LENGTH_SHORT).show()
                    }
                    BUTTON_NEGATIVE -> {
                        Toast.makeText(this@MainActivity,"取消被点击", Toast.LENGTH_SHORT).show()
                    }
                    else -> {

                    }
                }
                Timber.e("DialogInterface.OnClickListener:${which} $dialog")
            }
        }
    }

    /**
     * A native method that is implemented by the 'ndk' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun testVariantTest(): Any

    external fun testScalarTypes()

    external fun testJson()

    external fun testLog()

    fun backObject(obj: Any) {
        Timber.e("backObject:$obj")
    }

    external fun testDialog(param: DialogInterface.OnClickListener)

    companion object {
        // Used to load the 'ndk' library on application startup.
        init {
            System.loadLibrary("ndk")
        }
    }
}