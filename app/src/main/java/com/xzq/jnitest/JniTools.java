package com.xzq.jnitest;

public class JniTools {

    public void func() {
        throw new IllegalArgumentException("test");
    }

    public native void testException();


    public native void init();

    public native void init(int age);

    public native boolean init(String name);

    public native void update();

}
