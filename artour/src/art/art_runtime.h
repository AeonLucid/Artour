#ifndef ARTOUR_ART_RUNTIME_H
#define ARTOUR_ART_RUNTIME_H

#include <jni.h>
#include "art/art_method_offsets.h"

namespace art {

    class ArtRuntime {
    public:
        static ArtRuntime *Get();

        ArtRuntime() {}

        bool Initialize(JNIEnv *env);

        bool HookMethod(JNIEnv *env, jobject method);

        const ArtMethodOffsets *GetMethodOffsets() const {
            return &method_offsets;
        }

    private:
        ArtMethodOffsets method_offsets;
    };

}


#endif //ARTOUR_ART_RUNTIME_H
