//
// Created by Xia on 2022/8/23.
//

#ifndef JNITEST_STRINGUTIL_H
#define JNITEST_STRINGUTIL_H

#include "../../../../../../Android/sdk/ndk/21.4.7075529/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/jni.h"

char *Jstring2CStr(JNIEnv *env, jstring jstr);

char *jstringTostr(JNIEnv *env, jstring jstr);

jstring stoJstring(JNIEnv *env, const char *pat);



#endif //JNITEST_STRINGUTIL_H
