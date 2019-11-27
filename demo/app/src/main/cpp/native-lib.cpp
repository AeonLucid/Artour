#include <jni.h>
#include <string>
#include "artour.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_aeonlucid_artourdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    ArtourInit();
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
