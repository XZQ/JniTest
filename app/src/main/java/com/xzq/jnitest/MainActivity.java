package com.xzq.jnitest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.xzq.jnitest.databinding.ActivityMainBinding;


public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("jnitest");
    }

    private ActivityMainBinding binding;

    public int age = 1;
    public String name = "XZQ";
    public static String KEY = "key";

    private Button btnAdd, btnSub, btnMul, btnDiv;
    private EditText inputA, inputB;
    private TextView tvResult;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        setupView();
        addListener();
    }


    private void setupView() {
        btnAdd = binding.add;
        btnDiv = binding.div;
        btnMul = binding.mul;
        btnSub = binding.sub;
        inputA = binding.inputa;
        inputB = binding.inputb;
        tvResult = binding.result;
    }

    private void addListener() {
        btnAdd.setOnClickListener(this);
        btnDiv.setOnClickListener(this);
        btnMul.setOnClickListener(this);
        btnSub.setOnClickListener(this);
    }


    @Override
    public void onClick(View v) {
        double result = 0;
        String strA = inputA.getText().toString();
        String strB = inputB.getText().toString();
        int a = Integer.parseInt(strA);
        int b = Integer.parseInt(strB);
        switch (v.getId()) {
            case R.id.add:
                result = JniTools.add(a, b);
                break;
            case R.id.div:
                result = JniTools.div(a, b);
                break;
            case R.id.mul:
                result = JniTools.mul(a, b);
                break;
            case R.id.sub:
                result = JniTools.sub(a, b);
                break;
        }
        tvResult.setText("" + result);
    }


    //    int[] arr1 = {1, 1};
//    String[] arr2 = {"java value"};
//    String s = "result:" + Arrays.toString(testArray(arr1, arr2))
//            + "\n" + "arr1:" + Arrays.toString(arr1)
//            + "\n" + "arr2:" + Arrays.toString(arr2);
//        Log.e("TAG", "------------>> s=" + s);
    public native int[] testArray(int[] arr1, String[] arr2);

    // public native String stringFromJNI();

    public static native String fun1();

    public native Integer getIntegerObject(int number);

    public native void testObject();

    public native String manipulationStr(String name);

    public static String staticMethod(String name) {
        return "57";
    }


}