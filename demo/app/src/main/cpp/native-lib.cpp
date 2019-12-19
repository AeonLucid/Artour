#include <jni.h>
#include <string>
#include "artour.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_aeonlucid_artourdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string message;

    if (ArtourInit(env)) {
        message += "Initialized";
    } else {
        message += "Failed";
    }

    return env->NewStringUTF(message.c_str());
}
