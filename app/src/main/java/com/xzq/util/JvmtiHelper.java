package com.xzq.util;

import android.content.Context;
import android.os.Build;
import android.os.Debug;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.RequiresApi;

import java.io.File;
import java.lang.reflect.Method;
import java.nio.file.Files;
import java.nio.file.Paths;

public class JvmtiHelper {


    public static void init(Context context) {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.O) {
            return;
        }

        //获取so的地址后加载
        String agentPath = getAgentLibPath(context);
        if (!TextUtils.isEmpty(agentPath)) {
            System.load(agentPath);
        }
        // 开启JVMTI
        attachAgent(agentPath, context.getClassLoader());
    }


    private static void attachAgent(String agentPath, ClassLoader classLoader) {
        try {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                Debug.attachJvmtiAgent(agentPath, null, classLoader);
            } else {
                Class<?> vmDebugClazz = Class.forName("dalvik.system.VMDebug");
                Method attachAgentMethod = vmDebugClazz.getMethod("attachAgent", String.class);
                attachAgentMethod.setAccessible(true);
                attachAgentMethod.invoke(null, agentPath);
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private static String getAgentLibPath(Context context) {
        try {
            ClassLoader classLoader = context.getClassLoader();
            Method findLibrary = ClassLoader.class.getDeclaredMethod("findLibrary", String.class);
            String jvmtiAgentLibPath = (String) findLibrary.invoke(classLoader, "jvmti_agent");
            File filesDir = context.getFilesDir();
            File jvmtiLibDir = new File(filesDir, "jvmti");
            if (!jvmtiLibDir.exists()) {
                jvmtiLibDir.mkdirs();
            }
            File agentLibSo = new File(jvmtiLibDir, "agent.so");
            if (agentLibSo.exists()) {
                agentLibSo.delete();
            }
            Files.copy(Paths.get(new File(jvmtiAgentLibPath).getAbsolutePath()), Paths.get((agentLibSo).getAbsolutePath()));
            return agentLibSo.getAbsolutePath();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }
}
