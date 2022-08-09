#include <jni.h>
#include <string>
#include <iostream>


#include <android/log.h>

#define LOG_TAG "YIQI"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


/***
 * https://www.jianshu.com/p/423a37c19c43
 * https://www.jianshu.com/p/222761e1b685
 * https://www.jianshu.com/p/b4431ac22ec2
 * https://www.zybuluo.com/cxm-2016/note/563686
 */



char *Jstring2CStr(JNIEnv *pEnv, jstring pJstring);

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_xzq_jnitest_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
//    jclass clazz = env->FindClass("com/xzq/jnitest/MainActivity");
//    jstring jstring1;
//
//    if (clazz != nullptr) {
//        jfieldID ageFieldId = env->GetFieldID(clazz, "age", "I");
//        jint age = env->GetIntField(thiz, ageFieldId);
//
//        jfieldID nameFieldId = env->GetFieldID(clazz, "name", "Ljava/lang/String;");
//        jstring nameStr = (jstring) env->GetObjectField(thiz, nameFieldId);
//
//        std::cout << nameStr << std::endl;
//
//        // 从jstring 获取C格式字符串
//        char *name = (char *) env->GetStringUTFChars(nameStr, nullptr);
//        jstring1 = reinterpret_cast<jstring>(name);
//        LOGI("------------>> %s  %s", name, age);
//
//        // 释放
//        env->ReleaseStringUTFChars(nameStr, name);
//    }
//    env->DeleteLocalRef(clazz);
//    std::string hello = reinterpret_cast<const char *>(jstring1);
//    return env->NewStringUTF(hello.c_str());
//}


//
extern "C"
JNIEXPORT jstring JNICALL
Java_com_xzq_jnitest_MainActivity_fun1(JNIEnv *env, jclass clazz) {
    // TODO: implement fun1()
}




//  创建Java对象
extern "C" JNIEXPORT jobject JNICALL
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


char *Jstring2CStr(JNIEnv *env, jstring jstr) {
    char *rtn = nullptr;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);  //字符串拼接函数...
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return nullptr;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_xzq_jnitest_MainActivity_manipulationStr(JNIEnv *env, jobject thiz, jstring name) {
    char *str = "你好";
    char *string = Jstring2CStr(env, name);
    strcat(str, string);
    return env->NewStringUTF(str);
//    jstring stringValue = env->NewStringUTF(strcat((char *) env->GetStringUTFChars(name, JNI_FALSE), "你好"));
//    return stringValue;
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


static int registerNatives(JNIEnv *env) {
    // 要注册的 java 类的路径(完整的包名和类名)
    const char *className = "com/xzq/jnitest/JniTools";
    jclass clazz = env->FindClass(className);
    jint result = -1;
    if (clazz == nullptr) {
        return JNI_FALSE;
    }
    static JNINativeMethod methods[] = {
            {"init",   "()V",                   (void *) c_init1},
            {"init",   "(I)V",                  (void *) c_init2},
            {"init",   "(Ljava/lang/String;)Z", (void *) c_init3},
            {"update", "()V",                   (void *) c_update},
    };
    // 动态注册native方法
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return result;
    }

    // 返回成功
    result = JNI_VERSION_1_6;
    return result;
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

static int registerNatives1(JNIEnv *env) {
    jclass jclass1 = env->FindClass("com/xzq/jnitest/JniTools");
    const JNINativeMethod methods[] = {
            {"add", "(II)I", (void *) addNumber},
            {"sub", "(II)I", (void *) subNumber},
            {"mul", "(II)I", (void *) mulNumber},
            {"div", "(II)I", (void *) divNumber}
    };
    // 动态注册native方法
    if (env->RegisterNatives(jclass1, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return -1;
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
    if (!registerNatives1(env)) {
        return JNI_ERR;
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_xzq_jnitest_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    // TODO: implement stringFromJNI()
}