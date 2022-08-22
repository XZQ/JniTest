#include <jni.h>
#include <string>
#include <iostream>

#include "jvmti.h"
#include "Logger.h"

/***
 * https://www.jianshu.com/p/423a37c19c43
 * https://www.jianshu.com/p/222761e1b685
 * https://www.jianshu.com/p/b4431ac22ec2
 * https://www.zybuluo.com/cxm-2016/note/563686
 */

char *Jstring2CStr(JNIEnv *env, jstring jstr) {
    char *pStr = nullptr;
    jclass jstrObj = env->FindClass("java/lang/String");
    jstring encode = env->NewStringUTF("UTF-8");
    jmethodID methodId = env->GetMethodID(jstrObj, "getBytes", "(Ljava/lang/String;)[B");
    auto byteArray = (jbyteArray) env->CallObjectMethod(jstr, methodId, encode);
    jsize strLen = env->GetArrayLength(byteArray);
    jbyte *jBuf = env->GetByteArrayElements(byteArray, JNI_FALSE);
    if (strLen > 0) {
        pStr = (char *) malloc(strLen + 1);
        memcpy(pStr, jBuf, strLen);
        pStr[strLen] = 0;
    }
    env->ReleaseByteArrayElements(byteArray, jBuf, 0);
    return pStr;
}

char *jstringTostr(JNIEnv *env, jstring jstr) {
    char *pStr = nullptr;
    jclass jstrObj = env->FindClass("java/lang/String");
    jstring encode = env->NewStringUTF("UTF-8");
    jmethodID methodId = env->GetMethodID(jstrObj, "getBytes", "(Ljava/lang/String;)[B");
    auto byteArray = (jbyteArray) env->CallObjectMethod(jstr, methodId, encode);
    jsize strLen = env->GetArrayLength(byteArray);
    jbyte *jBuf = env->GetByteArrayElements(byteArray, JNI_FALSE);

    if (strLen > 0) {
        pStr = (char *) malloc(strLen + 1);
        if (!pStr) {
            return nullptr;
        }
        memcpy(pStr, jBuf, strLen);
        pStr[strLen] = 0;
    }
    env->ReleaseByteArrayElements(byteArray, jBuf, 0);
    return pStr;
}

//char* to jstring
jstring stoJstring(JNIEnv *env, const char *pat) {
    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
    jstring encoding = env->NewStringUTF("utf-8");
    return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
}


//  创建Java对象
extern "C" __attribute__((unused)) JNIEXPORT jobject JNICALL
Java_com_xzq_jnitest_MainActivity_getIntegerObject(JNIEnv *env, jobject thiz, jint number) {
    // 创建Java对象
    jclass cls = env->FindClass("java/lang/Integer");
    if (cls == nullptr) {
        LOGD("------------>> %s", "find class java.lang.Integer failed");
        return nullptr;
    }
    //  获取方法ID
    jmethodID init = env->GetMethodID(cls, "<init>", "(I)V");
    if (init == nullptr) {
        LOGD("------------>> %s", "get constructor failed");
        return nullptr;
    }
    // 创建对象
    jobject integer = env->NewObject(cls, init, number);
    //  获取toString方法
    jmethodID midToString = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
    if (midToString == nullptr) {
        LOGD("------------>>  %s", "get toString() method failed");
        return integer;
    }
    // 调用integer 的 midToString 方法
    auto result = (jstring) env->CallObjectMethod(integer, midToString);

    const char *resultStr = env->GetStringUTFChars(result, nullptr);
    LOGE("------------>>  %s", resultStr);
    // 释放
    env->ReleaseStringUTFChars(result, resultStr);

    return integer;
}


//获取成员变量
extern "C" JNIEXPORT
void JNICALL
Java_com_xzq_jnitest_MainActivity_testObject(JNIEnv *env, jobject thiz) {
    jclass clazz = env->FindClass("com/xzq/jnitest/MainActivity");
    if (clazz != nullptr) {

        jfieldID ageFieldId = env->GetFieldID(clazz, "age", "I");
        jint ageJint = env->GetIntField(thiz, ageFieldId);
        LOGE("------------>>  ageJint=%d", ageJint);
        // 下面出错，还不会解
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
        jfieldID keyFieldId = env->GetStaticFieldID(clazz, "KEY", "Ljava/lang/String;");
        auto key_jsting = (jstring) env->GetStaticObjectField(clazz, keyFieldId);

        // 对应 public static String staticMethod(String name) 方法
        jmethodID staticMethodId = env->GetStaticMethodID(clazz, "staticMethod", "(Ljava/lang/String;)Ljava/lang/String;");
        jstring param_jsting = env->NewStringUTF("param");

        auto return_jsting = (jstring) env->CallStaticObjectMethod(clazz, staticMethodId, param_jsting);

        // 从 jsting 获取 C 格式字符串
        const char *key = env->GetStringUTFChars(param_jsting, nullptr);

        /*
         *  从jstring 中 获取 char *
         *  isCopy 是一个 jboolean 引用，是作为返回值的，当它返回的值是非 nullptr 时，JNI_TRUE 代表复制了一份，JNI_FALSE 代表没有复制
         *  作用: 1、返回 JNI_TRUE 的话，就可以当成一个临时的存储，安全的使用了
         *       2、如果对有没有复制不关心，直接传 nullptr
         *  最终 : 无论是否复制，都应该release
         * */
        const char *return_value = env->GetStringUTFChars(return_jsting, nullptr);
        //
        env->ReleaseStringUTFChars(key_jsting, key);
        env->ReleaseStringUTFChars(return_jsting, return_value);
        LOGE("------------>>  ageJint=%s", return_value);
    }
    env->DeleteLocalRef(clazz);
}


extern "C"
JNIEXPORT jintArray JNICALL
Java_com_xzq_jnitest_MainActivity_testArray(JNIEnv *env, jobject thiz, jintArray arr1, jobjectArray arr2) {
    jint *_arr1 = env->GetIntArrayElements(arr1, nullptr);
    int lenght1 = env->GetArrayLength(arr1);
    for (int i = 0; i < lenght1; i++) {
        _arr1[i] = 2; // 修改数组值。在这里，只修改了 _aar1 的值，aar1 的值不变
    }
    env->ReleaseIntArrayElements(arr1, _arr1, 0);

    // 引用数组，只能根据 index 取单个值
    jstring _arr2 = (jstring) env->GetObjectArrayElement(arr2, 0);
    const char *s = env->GetStringUTFChars(_arr2, nullptr);
    LOGD("------------>> [testArray] old arr2[0]:%s", s);
    jstring newArr2 = env->NewStringUTF("JNI value");
    env->SetObjectArrayElement(arr2, 0, newArr2);
    // create new array
    int array[2] = {3, 3};
    jintArray dst = env->NewIntArray(2);
    env->SetIntArrayRegion(dst, 0, 2, array);
    return dst;

}

// JNI 捕获异常处理
extern "C" JNIEXPORT void JNICALL
Java_com_xzq_jnitest_JniTools_testException(JNIEnv *env, jobject thiz) {
    jclass clazz = env->FindClass("com/xzq/jnitest/JniTools");
    if (clazz != nullptr) {
        jmethodID func = env->GetMethodID(clazz, "func", "()V");
        env->CallVoidMethod(thiz, func);
        if (func != nullptr) {
            if (env->ExceptionCheck()) {
                env->ExceptionDescribe();
                env->ExceptionClear();
            }
        }
    }
    env->DeleteLocalRef(clazz);

    std::string hello = "Hello World from C++";

    /*抛出异常
     * jclass clazz = env->FindClass("java/lang/UnsupportedOperationException");
     if (clazz != nullptr) {
         env->ThrowNew(clazz, "Sorry, device is unsupported.");
     }
     env->DeleteLocalRef(clazz);*/
}


extern "C"
JNIEXPORT void JNICALL
Java_com_xzq_jnitest_MainActivity_testCallJava(JNIEnv *env, jobject thiz) {

    jclass cls = env->GetObjectClass(thiz);
    jclass cls1 = env->FindClass("com/xzq/jnitest/MainActivity");
    // 以上两种方式效果一样

    jfieldID codeId = env->GetFieldID(cls, "code", "I");
    jfieldID msgId = env->GetFieldID(cls, "msg", "Ljava/lang/String;");

    jint code = env->GetIntField(thiz, codeId);
    LOGE("------------>>  364 testCallJava :%d", code);
    auto msg = (jstring) env->GetObjectField(thiz, msgId);
    const char *cMsg = Jstring2CStr(env, msg);

    LOGE("------------>>  368 testCallJava :%s", cMsg);
    env->ReleaseStringUTFChars(msg, cMsg);

    // 调用Java层方法
    jmethodID callJavaMethodId = env->GetMethodID(cls, "cCallJava", "(Ljava/lang/String;)V");
    jstring nativeMsg = env->NewStringUTF("java method cCallJava called");
    env->CallVoidMethod(thiz, callJavaMethodId, nativeMsg);

    // 所有的jclass,jstring 必须释放
    env->DeleteLocalRef(msg);
    env->DeleteLocalRef(nativeMsg);
    env->DeleteLocalRef(cls);
}

// 异常
void testThrow(JNIEnv *env) {
    jthrowable error = env->ExceptionOccurred();
    if (error != nullptr) {
        // 出现了异常
    } else {
        // 没出现异常
    }
}



// 静态注册

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xzq_jnitest_JniTools_stringFromJNI(JNIEnv *env, jobject thiz) {

}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_xzq_jnitest_JniTools_fun1(JNIEnv *env, jclass clazz) {

}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_xzq_jnitest_JniTools_getIntegerObject(JNIEnv *env, jobject thiz, jint number) {
}
extern "C"
JNIEXPORT void JNICALL
Java_com_xzq_jnitest_JniTools_testCallJava(JNIEnv *env, jobject thiz) {
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_xzq_jnitest_JniTools_manipulationStr(JNIEnv *env, jobject thiz, jstring name) {
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_xzq_jnitest_JniTools_testExceptionNotCrash(JNIEnv *env, jobject thiz, jint i) {
}
extern "C"
JNIEXPORT jintArray JNICALL
Java_com_xzq_jnitest_JniTools_testArray(JNIEnv *env, jobject thiz, jintArray arr1, jobjectArray arr2) {
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xzq_jnitest_MainActivity_testExceptionNotCrash(JNIEnv *env, jobject thiz, jint i) {
    jstring hello = env->NewStringUTF("hello world");
    if (i > 100) {
        jclass ex = env->FindClass("com/xzq/util/CustomException");
        env->ThrowNew(ex, "i must < 100");
        env->DeleteLocalRef(ex);
    }
    // https://developer.android.google.cn/ndk/guides
    env->DeleteLocalRef(hello);
    return hello;
}

extern "C" JNIEXPORT void JNICALL
c_init1(JNIEnv *env, jobject thiz) {
}
extern "C" JNIEXPORT void JNICALL
c_init2(JNIEnv *env, jobject thiz, jint age) {
}
extern "C" JNIEXPORT jboolean JNICALL
c_init3(JNIEnv *env, jobject thiz, jstring name) {
}
extern "C" JNIEXPORT void JNICALL
c_update(JNIEnv *env, jobject thiz) {

}


jint addNumber(JNIEnv *env, jclass clazz, jint a, jint b) {
    LOGE("------------>> addNumber= %d %d %d", a, b, (a + b));
    return a + b;
}

jint subNumber(JNIEnv *env, jclass clazz, jint a, jint b) {
    LOGE("------------>> subNumber= %d %d %d", a, b, (a + b));
    return a - b;
}

jint mulNumber(JNIEnv *env, jclass clazz, jint a, jint b) {
    LOGE("------------>> mulNumber= %d %d %d", a, b, (a + b));
    return a * b;
}

jint divNumber(JNIEnv *env, jclass clazz, jint a, jint b) {
    LOGE("------------>> divNumber= %d %d %d", a, b, (a + b));
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
    assert(env != nullptr);
    if (!registerNatives(env)) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}
