package com.xzq.jnitest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.xzq.jnitest.databinding.ActivityMainBinding;
import com.xzq.util.JvmtiHelper;


public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    static {
        System.loadLibrary("jvmti_agent");
    }

    private static final String TAG = "MainActivity";

    private ActivityMainBinding binding;
    public int age = 1;
    public String name = "XZQ";
    public static String KEY = "key";
    private int code = 10;
    private String msg = "hello world";

    private Button btnAdd, btnSub, btnMul, btnDiv;
    private EditText inputA, inputB;
    private TextView tvResult;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        setupView();
        JvmtiHelper.init(this);
        addListener();
        testNative();

    }

    private void testNative() {
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
        if (strA.isEmpty() || strB.isEmpty()) {
            return;
        }
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


    public void cCallJava(String str) {
        Log.i(TAG, "cCallJava: " + str);
        Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
    }

    public void timeMillis() {
        for (int i = 0; i < 10; i++) {
            long start = System.nanoTime();
            long startTime = SystemClock.elapsedRealtime();
            long startT = System.currentTimeMillis();
            long end = System.nanoTime();
            long endTime = SystemClock.elapsedRealtime();
            long endT = System.currentTimeMillis();
            Log.i(TAG, "---------------->>  timeMillis: " + (end - start) + "   " + (endTime - startTime) + "   " + (endT - startT));
        }
    }
}