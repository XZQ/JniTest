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


    //加法
    public static native int add(int a, int b);

    //减法
    public static native int sub(int a, int b);

    //乘法
    public static native int mul(int a, int b);

    //除法
    public static native int div(int a, int b);

}
