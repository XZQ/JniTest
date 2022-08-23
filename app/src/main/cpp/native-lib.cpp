#include <jni.h>
#include <string>
#include <iostream>

#include "jvmti.h"
#include "Logger.h"
#include "StringUtil.h"


void c_init1(JNIEnv *env, jobject thiz) {
}

void c_init2(JNIEnv *env, jobject thiz, jint age) {
}

void c_init3(JNIEnv *env, jobject thiz, jstring name) {
}

void c_update(JNIEnv *env, jobject thiz) {

}

jint addNumber(JNIEnv *env, jclass clazz, jint a, jint b) {
    return a + b;
}

jint subNumber(JNIEnv *env, jclass clazz, jint a, jint b) {
    return a - b;
}

jint mulNumber(JNIEnv *env, jclass clazz, jint a, jint b) {
    return a * b;
}

jint divNumber(JNIEnv *env, jclass clazz, jint a, jint b) {
    return a / b;
}


const JNINativeMethod methods[] = {
        {"add",    "(II)I",                 (void *) addNumber},
        {"sub",    "(II)I",                 (void *) subNumber},
        {"mul",    "(II)I",                 (void *) mulNumber},
        {"div",    "(II)I",                 (void *) divNumber},
        {"init",   "()V",                   (void *) c_init1},
        {"init",   "(I)V",                  (void *) c_init2},
        {"init",   "(Ljava/lang/String;)Z", (void *) c_init3},
        {"update", "()V",                   (void *) c_update},
};

static int registerNatives(JNIEnv *env) {
    const char *className = "com/xzq/jnitest/JniTools";
    jclass clazz = env->FindClass(className);
    jint result = -1;
    if (clazz == nullptr) {
        return JNI_FALSE;
    }
    // 动态注册native方法
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return result;
    }
    return JNI_VERSION_1_6;
}

// System.loadLibrary 加载库文件时，系统回调 JNI_OnLoad() 函数
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
//    if (!registerNatives(env)) {
//        return JNI_ERR;
//    }
    return JNI_VERSION_1_6;
}


//jvmtiEnv *CreateJvmtiEnv(JavaVM *vm) {
//    jvmtiEnv *jvmti_env;
//    jint result = vm->GetEnv((void **) &jvmti_env, JVMTI_VERSION_1_2);
//    if (result != JNI_OK) {
//        return nullptr;
//    }
//    return jvmti_env;
//}
//
//void setAllCapabilities(jvmtiEnv *jvmti) {
//    jvmtiCapabilities capabilities;
//    jvmtiError jvmtiError;
//    jvmtiError = jvmti->GetPotentialCapabilities(&capabilities);
//    jvmtiError = jvmti->AddCapabilities(&capabilities);
//}


jvmtiEnv *mJvmtiEnv = nullptr;

/**
 * Agent attch 回调
 */
extern "C" JNIEXPORT jint JNICALL Agent_OnAttach(JavaVM *vm, char *options, void *reserved) {
//    JNIEnv *env;
//    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
//        return JNI_ERR;
//    }
//    LOGE("------------------------>>   JVM Agent_OnAttach success");
//    jvmtiEnv *jvmtiEnv = CreateJvmtiEnv(vm);
//    if (jvmtiEnv == nullptr) {
//        return JNI_ERR;
//    }
//    setAllCapabilities(jvmtiEnv);

    vm->GetEnv(reinterpret_cast<void **>(&mJvmtiEnv), JVMTI_VERSION_1_2);
    if (mJvmtiEnv == nullptr) {
        return JNI_ERR;
    }
    jvmtiCapabilities capabilities;
    mJvmtiEnv->GetPotentialCapabilities(&capabilities);
    mJvmtiEnv->AddCapabilities(&capabilities);
    return JNI_OK;
}

void JNICALL objectAlloc(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread,
                         jobject object, jclass object_klass, jlong size) {
    //对象创建
}

void JNICALL objectFree(jvmtiEnv *jvmti_env, jlong tag) {
    //对象释放
}

void JNICALL methodEntry(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
    //方法进入
}

void JNICALL methodExit(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method, jboolean was_popped_by_exception,
                        jvalue return_value) {
    //方法退出
}


extern "C" JNIEXPORT void JNICALL
Java_com_xzq_jnitest_JniTools_agentInit(JNIEnv *env, jclass clazz) {
    jvmtiEventCallbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.MethodEntry = &methodEntry;
    callbacks.MethodExit = &methodExit;
    callbacks.VMObjectAlloc = &objectAlloc;
    callbacks.ObjectFree = &objectFree;
    if (mJvmtiEnv != nullptr) {
        mJvmtiEnv->SetEventCallbacks(&callbacks, sizeof(callbacks));
        mJvmtiEnv->SetEventCallbacks(&callbacks, sizeof(callbacks));
        //开启监听
        mJvmtiEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_OBJECT_ALLOC, nullptr);
        mJvmtiEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_OBJECT_FREE, nullptr);
        mJvmtiEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, nullptr);
        mJvmtiEnv->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_EXIT, nullptr);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_xzq_jnitest_JniTools_agentelease(JNIEnv *env, jclass clazz) {
    mJvmtiEnv->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_VM_OBJECT_ALLOC, nullptr);
    mJvmtiEnv->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_OBJECT_FREE, nullptr);
    mJvmtiEnv->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_METHOD_ENTRY, nullptr);
    mJvmtiEnv->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_METHOD_EXIT, nullptr);
}


// https://www.jianshu.com/p/ab7bbc319dd9
// http://events.jianshu.io/p/70de92815121
// https://juejin.cn/post/7047381967058239525
// https://juejin.cn/post/6844904013876445197
// https://juejin.cn/post/7093858834476695588



////  创建Java对象
//extern "C" __attribute__((unused)) JNIEXPORT jobject JNICALL
//Java_com_xzq_jnitest_MainActivity_getIntegerObject(JNIEnv *env, jobject thiz, jint number) {
//    // 创建Java对象
//    jclass cls = env->FindClass("java/lang/Integer");
//    if (cls == nullptr) {
//        LOGD("------------>> %s", "find class java.lang.Integer failed");
//        return nullptr;
//    }
//    //  获取方法ID
//    jmethodID init = env->GetMethodID(cls, "<init>", "(I)V");
//    if (init == nullptr) {
//        LOGD("------------>> %s", "get constructor failed");
//        return nullptr;
//    }
//    // 创建对象
//    jobject integer = env->NewObject(cls, init, number);
//    //  获取toString方法
//    jmethodID midToString = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
//    if (midToString == nullptr) {
//        LOGD("------------>>  %s", "get toString() method failed");
//        return integer;
//    }
//    // 调用integer 的 midToString 方法
//    auto result = (jstring) env->CallObjectMethod(integer, midToString);
//
//    const char *resultStr = env->GetStringUTFChars(result, nullptr);
//    LOGE("------------>>  %s", resultStr);
//    // 释放
//    env->ReleaseStringUTFChars(result, resultStr);
//
//    return integer;
//}


////获取成员变量
//extern "C" JNIEXPORT
//void JNICALL
//Java_com_xzq_jnitest_MainActivity_testObject(JNIEnv *env, jobject thiz) {
//    jclass clazz = env->FindClass("com/xzq/jnitest/MainActivity");
//    if (clazz != nullptr) {
//
//        jfieldID ageFieldId = env->GetFieldID(clazz, "age", "I");
//        jint ageJint = env->GetIntField(thiz, ageFieldId);
//        LOGE("------------>>  ageJint=%d", ageJint);
//        // 下面出错，还不会解
//        jfieldID nameFieldId = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
//        jstring nameJString = (jstring) env->GetObjectField(thiz, nameFieldId);
//        LOGE("------------>>  nameJString=%s", nameJString);
//        char *name = (char *) env->GetStringUTFChars(nameJString, nullptr);
//        LOGI("------------>>  name:%s; age:%d", name, ageJint);
//        env->ReleaseStringUTFChars(nameJString, name);
// 对应 MainActivity 中的 public String name = "Jack";
//        jfieldID nameFieldId = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
//        jstring name_jstring = (jstring) env->GetObjectField(thiz, nameFieldId);
//        LOGE("------------>>  nameJString=%s", name_jstring);
//        // 从 jsting 获取 C 格式字符串
//        // 关于 GetStringUTFChars 的详细解释，请参考下文
//        char *name = (char *) env->GetStringUTFChars(name_jstring, nullptr);
//        LOGI("------------>>  name:%s; age:%d", name, ageJint);
//        env->ReleaseStringUTFChars(name_jstring, name);


// 对应 public static String KEY = "key";
//        jfieldID keyFieldId = env->GetStaticFieldID(clazz, "KEY", "Ljava/lang/String;");
//        auto key_jsting = (jstring) env->GetStaticObjectField(clazz, keyFieldId);
//
//        // 对应 public static String staticMethod(String name) 方法
//        jmethodID staticMethodId = env->GetStaticMethodID(clazz, "staticMethod", "(Ljava/lang/String;)Ljava/lang/String;");
//        jstring param_jsting = env->NewStringUTF("param");
//
//        auto return_jsting = (jstring) env->CallStaticObjectMethod(clazz, staticMethodId, param_jsting);
//
//        // 从 jsting 获取 C 格式字符串
//        const char *key = env->GetStringUTFChars(param_jsting, nullptr);
//
//        /*
//         *  从jstring 中 获取 char *
//         *  isCopy 是一个 jboolean 引用，是作为返回值的，当它返回的值是非 nullptr 时，JNI_TRUE 代表复制了一份，JNI_FALSE 代表没有复制
//         *  作用: 1、返回 JNI_TRUE 的话，就可以当成一个临时的存储，安全的使用了
//         *       2、如果对有没有复制不关心，直接传 nullptr
//         *  最终 : 无论是否复制，都应该release
//         * */
//        const char *return_value = env->GetStringUTFChars(return_jsting, nullptr);
//        //
//        env->ReleaseStringUTFChars(key_jsting, key);
//        env->ReleaseStringUTFChars(return_jsting, return_value);
//        LOGE("------------>>  ageJint=%s", return_value);
//    }
//    env->DeleteLocalRef(clazz);
//}
//
//
//extern "C"
//JNIEXPORT jintArray JNICALL
//Java_com_xzq_jnitest_MainActivity_testArray(JNIEnv *env, jobject thiz, jintArray arr1, jobjectArray arr2) {
//    jint *_arr1 = env->GetIntArrayElements(arr1, nullptr);
//    int lenght1 = env->GetArrayLength(arr1);
//    for (int i = 0; i < lenght1; i++) {
//        _arr1[i] = 2; // 修改数组值。在这里，只修改了 _aar1 的值，aar1 的值不变
//    }
//    env->ReleaseIntArrayElements(arr1, _arr1, 0);
//
//    // 引用数组，只能根据 index 取单个值
//    jstring _arr2 = (jstring) env->GetObjectArrayElement(arr2, 0);
//    const char *s = env->GetStringUTFChars(_arr2, nullptr);
//    LOGD("------------>> [testArray] old arr2[0]:%s", s);
//    jstring newArr2 = env->NewStringUTF("JNI value");
//    env->SetObjectArrayElement(arr2, 0, newArr2);
//    // create new array
//    int array[2] = {3, 3};
//    jintArray dst = env->NewIntArray(2);
//    env->SetIntArrayRegion(dst, 0, 2, array);
//    return dst;
//
//}
//
//
//// JNI 捕获异常处理
//extern "C" JNIEXPORT void JNICALL
//Java_com_xzq_jnitest_JniTools_testException(JNIEnv *env, jobject thiz) {
//    jclass clazz = env->FindClass("com/xzq/jnitest/JniTools");
//    if (clazz != nullptr) {
//        jmethodID func = env->GetMethodID(clazz, "func", "()V");
//        env->CallVoidMethod(thiz, func);
//        if (func != nullptr) {
//            if (env->ExceptionCheck()) {
//                env->ExceptionDescribe();
//                env->ExceptionClear();
//            }
//        }
//    }
//    env->DeleteLocalRef(clazz);
//
//    std::string hello = "Hello World from C++";
//
//    /*抛出异常
//     * jclass clazz = env->FindClass("java/lang/UnsupportedOperationException");
//     if (clazz != nullptr) {
//         env->ThrowNew(clazz, "Sorry, device is unsupported.");
//     }
//     env->DeleteLocalRef(clazz);*/
//}
//
//
//// 静态注册
//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_xzq_jnitest_MainActivity_testExceptionNotCrash(JNIEnv *env, jobject thiz, jint i) {
//    jstring hello = env->NewStringUTF("hello world");
//    if (i > 100) {
//        jclass ex = env->FindClass("com/xzq/util/CustomException");
//        env->ThrowNew(ex, "i must < 100");
//        env->DeleteLocalRef(ex);
//    }
//    // https://developer.android.google.cn/ndk/guides
//    env->DeleteLocalRef(hello);
//    return hello;
//}
//
//extern "C"
//JNIEXPORT void JNICALL
//Java_com_xzq_jnitest_MainActivity_testCallJava(JNIEnv *env, jobject thiz) {
//
//    jclass cls = env->GetObjectClass(thiz);
//    jclass cls1 = env->FindClass("com/xzq/jnitest/MainActivity");
//    // 以上两种方式效果一样
//
//    jfieldID codeId = env->GetFieldID(cls, "code", "I");
//    jfieldID msgId = env->GetFieldID(cls, "msg", "Ljava/lang/String;");
//
//    jint code = env->GetIntField(thiz, codeId);
//    LOGE("------------>>  364 testCallJava :%d", code);
//    auto msg = (jstring) env->GetObjectField(thiz, msgId);
//    const char *cMsg = Jstring2CStr(env, msg);
//
//    LOGE("------------>>  368 testCallJava :%s", cMsg);
//    env->ReleaseStringUTFChars(msg, cMsg);
//
//    // 调用Java层方法
//    jmethodID callJavaMethodId = env->GetMethodID(cls, "cCallJava", "(Ljava/lang/String;)V");
//    jstring nativeMsg = env->NewStringUTF("java method cCallJava called");
//    env->CallVoidMethod(thiz, callJavaMethodId, nativeMsg);
//
//    // 所有的jclass,jstring 必须释放
//    env->DeleteLocalRef(msg);
//    env->DeleteLocalRef(nativeMsg);
//    env->DeleteLocalRef(cls);
//}
//
//// 异常
//void testThrow(JNIEnv *env) {
//    jthrowable error = env->ExceptionOccurred();
//    if (error != nullptr) {
//        // 出现了异常
//    } else {
//        // 没出现异常
//    }
//}
