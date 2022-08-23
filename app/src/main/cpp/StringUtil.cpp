//
// Created by Xia on 2022/8/23.
//


#include "../../../../../../Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/jni.h"
#include "../../../../../../Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstdlib"
#include "../../../../../../Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1/cstring"
#include "../../../../../../Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/malloc.h"
#include "../../../../../../Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/lib64/clang/9.0.9/include/__clang_cuda_device_functions.h"

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
//jstring stoJstring(JNIEnv *env, const char *pat) {
//    jclass strClass = env->FindClass("java/lang/String");
//    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
//    jbyteArray bytes = env->NewByteArray(std::strlen(pat));
//    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
//    jstring encoding = env->NewStringUTF("utf-8");
//    return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
//}
