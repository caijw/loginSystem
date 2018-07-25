// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from loginSystem.djinni

package com.mycompany.crypto;

import java.util.concurrent.atomic.AtomicBoolean;

public abstract class Crypto {
    public abstract String getHelloWorld();

    public abstract String argon2(String message, int tCost, int mCost, int parallelism, String salt);

    public abstract String AESEnc(String message, String key);

    public abstract String AESDec(String message, String key);

    public abstract String stringWithFixedLength(String origin, int length, String padding);

    public abstract String deStringWithFixedLength(String origin, String padding);

    public static native Crypto create();

    private static final class CppProxy extends Crypto
    {
        private final long nativeRef;
        private final AtomicBoolean destroyed = new AtomicBoolean(false);

        private CppProxy(long nativeRef)
        {
            if (nativeRef == 0) throw new RuntimeException("nativeRef is zero");
            this.nativeRef = nativeRef;
        }

        private native void nativeDestroy(long nativeRef);
        public void destroy()
        {
            boolean destroyed = this.destroyed.getAndSet(true);
            if (!destroyed) nativeDestroy(this.nativeRef);
        }
        protected void finalize() throws java.lang.Throwable
        {
            destroy();
            super.finalize();
        }

        @Override
        public String getHelloWorld()
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_getHelloWorld(this.nativeRef);
        }
        private native String native_getHelloWorld(long _nativeRef);

        @Override
        public String argon2(String message, int tCost, int mCost, int parallelism, String salt)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_argon2(this.nativeRef, message, tCost, mCost, parallelism, salt);
        }
        private native String native_argon2(long _nativeRef, String message, int tCost, int mCost, int parallelism, String salt);

        @Override
        public String AESEnc(String message, String key)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_AESEnc(this.nativeRef, message, key);
        }
        private native String native_AESEnc(long _nativeRef, String message, String key);

        @Override
        public String AESDec(String message, String key)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_AESDec(this.nativeRef, message, key);
        }
        private native String native_AESDec(long _nativeRef, String message, String key);

        @Override
        public String stringWithFixedLength(String origin, int length, String padding)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_stringWithFixedLength(this.nativeRef, origin, length, padding);
        }
        private native String native_stringWithFixedLength(long _nativeRef, String origin, int length, String padding);

        @Override
        public String deStringWithFixedLength(String origin, String padding)
        {
            assert !this.destroyed.get() : "trying to use a destroyed object";
            return native_deStringWithFixedLength(this.nativeRef, origin, padding);
        }
        private native String native_deStringWithFixedLength(long _nativeRef, String origin, String padding);
    }
}
