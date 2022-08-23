package com.xzq.jnitest;


public class JniTools {

//    public void func() {
//        throw new IllegalArgumentException("test");
//    }

    public native void init();

    public native void init(int age);

    public native boolean init(String name);

    public native void update();

    public static native int add(int a, int b);

    public static native int sub(int a, int b);

    public static native int mul(int a, int b);

    public static native int div(int a, int b);

//    public native void testException();
//
//    public native int[] testArray(int[] arr1, String[] arr2);
//
//    public static native String fun1();
//
//    public native Integer getIntegerObject(int number);
//
//    public native String manipulationStr(String name);
//
//    public native void testCallJava();
//
//    public native String testExceptionNotCrash(int i);

    public static String staticMethod(String name) {
        return "57";
    }


    private native static void agentInit();

    private native static void agentelease();

}
