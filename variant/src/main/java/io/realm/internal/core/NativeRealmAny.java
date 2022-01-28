package io.realm.internal.core;

import org.bson.types.Decimal128;
import org.bson.types.ObjectId;

import java.util.Date;
import java.util.UUID;

import io.realm.internal.NativeContext;
import io.realm.internal.NativeObject;

/**
 * @author mengganggang
 */
public class NativeRealmAny implements NativeObject {
    private static final long nativeFinalizerPtr = nativeGetFinalizerPtr();

    private final long nativePtr;

    public NativeRealmAny(long nativePtr) {
        this.nativePtr = nativePtr;
        NativeContext.dummyContext.addReference(this);
    }

    public NativeRealmAny(Boolean value) {
        this(nativeCreateBoolean(value));
    }

    public NativeRealmAny(Number value) {
        this(nativeCreateLong(value.longValue()));
    }

    public NativeRealmAny(Float value) {
        this(nativeCreateFloat(value));
    }

    public NativeRealmAny(Double value) {
        this(nativeCreateDouble(value));
    }

    public NativeRealmAny(String value) {
        this(nativeCreateString(value));
    }

    public NativeRealmAny(byte[] value) {
        this(nativeCreateBinary(value));
    }

    public NativeRealmAny(Date value) {
        this(nativeCreateDate(value.getTime()));
    }

    public NativeRealmAny(ObjectId value) {
        this(nativeCreateObjectId(value.toString()));
    }

    public NativeRealmAny(Decimal128 value) {
        this(nativeCreateDecimal128(value.getLow(), value.getHigh()));
    }

    public NativeRealmAny(UUID value) {
        this(nativeCreateUUID(value.toString()));
    }

    public NativeRealmAny() {
        this(nativeCreateNull());
    }

    private static native long nativeCreateNull();

    private static native long nativeCreateBoolean(boolean value);

    private static native boolean nativeAsBoolean(long nativePtr);

    private static native long nativeCreateLong(long value);

    private static native long nativeAsLong(long nativePtr);

    private static native long nativeCreateFloat(float value);

    private static native float nativeAsFloat(long nativePtr);

    private static native long nativeCreateDouble(double value);

    private static native double nativeAsDouble(long nativePtr);

    private static native long nativeCreateString(String value);

    private static native String nativeAsString(long nativePtr);

    private static native long nativeCreateBinary(byte[] value);

    private static native byte[] nativeAsBinary(long nativePtr);

    private static native long nativeCreateDate(long value);

    private static native long nativeAsDate(long nativePtr);

    private static native long nativeCreateObjectId(String value);
    // private static native long nativeCreateBoolean(Boolean value);

    private static native String nativeAsObjectId(long nativePtr);

    private static native long nativeCreateDecimal128(long low, long high);

    private static native long[] nativeAsDecimal128(long nativePtr);

    private static native long nativeCreateUUID(String value);

    private static native String nativeAsUUID(long nativePtr);

    private static native int nativeGetType(long nativePtr);

    private static native boolean nativeEquals(long nativePtr, long nativeOtherPtr);

    private static native long nativeGetFinalizerPtr();

    @Override
    public long getNativePtr() {
        return nativePtr;
    }

    @Override
    public long getNativeFinalizerPtr() {
        return nativeFinalizerPtr;
    }

    public NativeRealmAnyType getType() {
        return NativeRealmAnyType.values()[nativeGetType(nativePtr)];
    }

    public boolean asBoolean() {
        return nativeAsBoolean(nativePtr);
    }

    public long asLong() {
        return nativeAsLong(nativePtr);
    }

    public float asFloat() {
        return nativeAsFloat(nativePtr);
    }

    public double asDouble() {
        return nativeAsDouble(nativePtr);
    }

    public String asString() {
        return nativeAsString(nativePtr);
    }

    public byte[] asBinary() {
        return nativeAsBinary(nativePtr);
    }

    public Date asDate() {
        return new Date(nativeAsDate(nativePtr));
    }

    public ObjectId asObjectId() {
        return new ObjectId(nativeAsObjectId(nativePtr));
    }

    public Decimal128 asDecimal128() {
        long[] data = nativeAsDecimal128(nativePtr);
        return Decimal128.fromIEEE754BIDEncoding(data[1]/*high*/, data[0]/*low*/);
    }

    public UUID asUUID() {
        return UUID.fromString(nativeAsUUID(nativePtr));
    }

    public boolean coercedEquals(NativeRealmAny nativeRealmAny) {
        return nativeEquals(nativePtr, nativeRealmAny.nativePtr);
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append(" value = ");
        switch (NativeRealmAnyType.values()[nativeGetType(nativePtr)]) {
            case NULL:
                sb.append("null");
                break;
            case LONG:
                sb.append(asLong());
                break;
            case DOUBLE:
                sb.append(asDouble());
                break;
            case BOOL:
                sb.append(asBoolean());
                break;
            case STATIC_STRING:
            case MUTABLE_STRING:
                sb.append(asString());
                break;
            case VECTOR:
            case MAP:
            case MUTABLE_BLOB:
            case STATIC_BLOB:
                sb.append("not support");
                break;
            default:
                sb.append("error");
                break;
        }
        return "NativeRealmAny{" +
                "nativePtr = " + nativePtr +
                sb +
                '}';
    }
}
