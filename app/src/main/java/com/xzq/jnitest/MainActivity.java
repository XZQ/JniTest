package com.xzq.jnitest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.xzq.jnitest.databinding.ActivityMainBinding;

import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'jnitest' library on application startup.
    static {
        System.loadLibrary("jnitest");
    }

    private ActivityMainBinding binding;

    public int age = 1;
    public String name = "XZQ";
    public static String KEY = "key";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
//        tv.setText(String.valueOf(getIntegerObject(110)));

//        testObject();

//        String ma = manipulationStr("Test");
//        Log.e("TAG", "------------>> ma=" + ma);

        int[] arr1 = {1, 1};
        String[] arr2 = {"java value"};
        String s = "result:" + Arrays.toString(testArray(arr1, arr2))
                + "\n" + "arr1:" + Arrays.toString(arr1)
                + "\n" + "arr2:" + Arrays.toString(arr2);
        Log.e("TAG", "------------>> s=" + s);
    }

    public native int[] testArray(int[] arr1, String[] arr2);

//    public native String stringFromJNI();

    public static native String fun1();

    public native Integer getIntegerObject(int number);

    public native void testObject();

    public native String manipulationStr(String name);

    public static String staticMethod(String name) {
        return "57";
    }
}